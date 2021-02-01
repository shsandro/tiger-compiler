%{
#include <string.h>
#include <limits.h>
#include "util.h"
#include "error.h"
#include "token.h"

int char_pos = 1;
int  comment_depth = 0;
char string_text[16384] = "";
int yywrap();
void adjust();

%}

%x COMMENT STRING_S STRING_ML
asc_range ([01][0-9]{2}|2[0-4][0-9]|25[0-5])

%%

"\""                        {adjust(); BEGIN(STRING); string_text[0]= '\0'; strcat(string_text,"\"");}

"/*"                        {adjust(); BEGIN(COMMENT); comment_depth++;}

","	                        {adjust(); return COMMA;}
":"                         {adjust(); return COLON;}
";"                         {adjust(); return SEMICOLON;}
"("                         {adjust(); return LPAREN;}
")"                         {adjust(); return RPAREN;}
"["                         {adjust(); return LBRACK;}
"]"                         {adjust(); return RBRACK;}
"{"                         {adjust(); return LBRACE;}
"}"                         {adjust(); return RBRACE;}
"."                         {adjust(); return DOT;}
"+"                         {adjust(); return PLUS;}
"-"                         {adjust(); return MINUS;}
"*"                         {adjust(); return TIMES;}
"/"                         {adjust(); return DIVIDE;}
"="                         {adjust(); return EQ;}
"<>"                        {adjust(); return NEQ;}
"<"                         {adjust(); return LT;}
"<="                        {adjust(); return LE;}
">"                         {adjust(); return GT;}
">="                        {adjust(); return GE;}
"&"                         {adjust(); return AND;}
"|"                         {adjust(); return OR;}
":="                        {adjust(); return ASSIGN;}
array                       {adjust(); return ARRAY;}
if                          {adjust(); return IF;}
then                        {adjust(); return THEN;}
else                        {adjust(); return ELSE;}
while                       {adjust(); return WHILE;}
for  	                    {adjust(); return FOR;}
to                          {adjust(); return TO;}
do                          {adjust(); return DO;}
let                         {adjust(); return LET;}
in                          {adjust(); return IN;}
end                         {adjust(); return END;}
of                          {adjust(); return OF;}
break                       {adjust(); return BREAK;}
nil                         {adjust(); return NIL;}
function                    {adjust(); return FUNCTION;}
var                         {adjust(); return VAR;}
type                        {adjust(); return TYPE;}
[a-zA-Z][a-zA-Z0-9_]*|_main {adjust(); yylval.sval = Id(yytext); return ID;}
([0-9]*[.])[0-9]+           {adjust(); yylval.fval = atof(yytext); return FLOAT;}
[0-9]+	                    {adjust(); 
                                long int value = strtol(yytext, NULL, 10); 
                                if (value == LONG_MAX || value > INT_MAX) {
                                    error(tokPos, "invalid integer"); 
                                    yyterminate();
                                } else {
                                    yylval.ival = atoi(yytext); 
                                    return INT;
                                }
                            }
\n	                        {adjust(); newline(); continue; }
(" "|"\t")                  {adjust(); continue;}
.               	        {adjust(); error(tokPos,"illegal token");}

<STRING_ML>[ \t]            {adjust();  continue;}
<STRING_ML>"\n"             {adjust();  newline(); continue;}
<STRING_ML>\\               {adjust();  BEGIN(STRING_S);}
<STRING_ML>.                {adjust();  error(tokPos,"invalid character in multiline string"); yyterminate();}

<STRING_S>"\""              {adjust();  BEGIN(INITIAL); strcat(string_text,"\""); yylval.sval=String(string_text); return STRING;}
<STRING_S><<EOF>>           {newline(); BEGIN(INITIAL);  error(tokPos,"unclosed multiline string"); yyterminate();}
<STRING_S>"\\n"             {adjust();  strcat(string_text,"\n"); continue;}
<STRING_S>"\\t"             {adjust();  strcat(string_text,"\t"); continue;}
<STRING_S>"\\\\"            {adjust();  strcat(string_text,"\\"); continue;}
<STRING_S>"\\\""            {adjust();  strcat(string_text,"\""); continue;}
<STRING_S>\\^[a-z]          {adjust(); 
                                if (strchr("abcdefghijklmnopqrstuvwxyz", yytext[2])) {
                                    char p[2];
                                    p[0] = (yytext[2] - 'a' + 1);
                                    p[1] = '\0';

                                    strcat(string_text, p);
                                    continue;
                                } else {
                                    error(tokPos, "illegal escape sequence");
                                    yyterminate();
                                }
                            }
<STRING_S>\\{asc_range}     {adjust();  char p[2]; p[0] = (char)atoi(&yytext[1]); p[1] = '\0'; strcat(string_text, p); continue;}
<STRING_S>\\[ \t]+          {adjust();  BEGIN(STRING_ML);}
<STRING_S>\\\n              {adjust();  newline(); BEGIN(STRING_ML);}
<STRING_S>\\.               {adjust();  error(tokPos,"invalid character or ASC code"); yyterminate();}
<STRING_S>.                 {adjust();  strcat(string_text,yytext); continue;}

<COMMENT>"*/"               {adjust(); if (--comment_depth == 0) { BEGIN(INITIAL); }}
<COMMENT>"/*"               {adjust(); comment_depth++;}
<COMMENT>\n                 {adjust(); newline(); continue;}
<COMMENT><<EOF>>            {error(tokPos,"unclosed comment"); yyterminate();}
<COMMENT>.                  {adjust();}

%%

int yywrap() {
    char_pos = 1;
    return 1;
}

void adjust() {
    tokPos=char_pos;
    char_pos+=yyleng;
}