%{
#include <string.h>
#include "util.h"
#include "error.h"
#include "token.h"

int char_pos = 1;
int  comment_depth = 0;
char string_text[5000] = "";
int yywrap();
void adjust();

%}

%x COMMENT STRING
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
[0-9]+	                    {adjust(); yylval.ival = atoi(yytext); return INT;}
\n	                        {adjust(); newline(); continue; }
(" "|"\t")                  {adjust(); continue;}
.               	        {adjust(); error(tokPos,"illegal token");}

<STRING>"\""              {adjust();  BEGIN(INITIAL); strcat(string_text,"\""); yylval.sval=String(string_text); return STRING;}
<STRING><<EOF>>           {newline(); BEGIN(INITIAL);  error(tokPos,"unclosed string"); yyterminate();}
<STRING>"\\n"             {adjust();  strcat(string_text,"\n"); continue;}
<STRING>"\\t"             {adjust();  strcat(string_text,"\t"); continue;}
<STRING>"\\\\"            {adjust();  strcat(string_text,"\\"); continue;}
<STRING>"\\\""            {adjust();  strcat(string_text,"\""); continue;}
<STRING>\\{asc_range}     {adjust();  strcat(string_text,yytext); continue;}
<STRING>\\[ \t]+          {adjust();}
<STRING>\n                {adjust();  error(tokPos,"multiline in string without '\\'"); yyterminate();}
<STRING>\\\n              {adjust();  newline(); continue;}
<STRING>\\.               {adjust();  error(tokPos,"invalid ASC code"); yyterminate();}
<STRING>.                 {adjust();  strcat(string_text, yytext); continue;}

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