%{
#include <string.h>
#include "util.h"
#include "error.h"
#include "token.h"

int char_pos = 1;

int yywrap(void) {
    char_pos = 1;
    return 1;
}

void adjust(void) {
    tokPos=char_pos;
    char_pos+=yyleng;
}

%}

%%


","	                  {adjust(); return COMMA;}
":"                   {adjust(); return COLON;}
";"                   {adjust(); return SEMICOLON;}
"("                   {adjust(); return LPAREN;}
")"                   {adjust(); return RPAREN;}
"["                   {adjust(); return LBRACK;}
"]"                   {adjust(); return RBRACK;}
"{"                   {adjust(); return LBRACE;}
"}"                   {adjust(); return RBRACE;}
"."                   {adjust(); return DOT;}
"+"                   {adjust(); return PLUS;}
"-"                   {adjust(); return MINUS;}
"*"                   {adjust(); return TIMES;}
"/"                   {adjust(); return DIVIDE;}
"="                   {adjust(); return EQ;}
"<>"                  {adjust(); return NEQ;}
"<"                   {adjust(); return LT;}
"<="                  {adjust(); return LE;}
">"                   {adjust(); return GT;}
">="                  {adjust(); return GE;}
"&"                   {adjust(); return AND;}
"|"                   {adjust(); return OR;}
":="                  {adjust(); return ASSIGN;}
array                 {adjust(); return ARRAY;}
if                    {adjust(); return IF;}
then                  {adjust(); return THEN;}
else                  {adjust(); return ELSE;}
while                 {adjust(); return WHILE;}
for  	              {adjust(); return FOR;}
to                    {adjust(); return TO;}
do                    {adjust(); return DO;}
let                   {adjust(); return LET;}
in                    {adjust(); return IN;}
end                   {adjust(); return END;}
of                    {adjust(); return OF;}
break                 {adjust(); return BREAK;}
nil                   {adjust(); return NIL;}
function              {adjust(); return FUNCTION;}
var                   {adjust(); return VAR;}
type                  {adjust(); return TYPE;}
\"[a-zA-z0-9]*\"      {adjust(); yylval.sval = String(yytext); return STRING;}
[a-zA-Z][a-zA-Z0-9_]* {adjust(); yylval.sval = Id(yytext); return ID;}
[0-9]+	              {adjust(); yylval.ival=atoi(yytext); return INT;}
\n	                  {adjust(); newline(); continue;}
(" "|"\t")            {adjust(); continue;}
.               	  {adjust(); error(tokPos,"illegal token");}
