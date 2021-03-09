%{
#include <stdio.h>
#include <stdlib.h>
#include "include/util.h"
#include "include/symbol.h" 
#include "include/error.h"
#include "include/absyn.h"

int yylex(void);

A_exp absyn_root;

void yyerror(char *s) {
   error(tokPos, "%s", s);
   exit(1);
}

%}


%union {
	int pos;
	int ival;
   float fval;
	string sval;
	A_var var;
	A_exp exp;
	A_dec dec;
	A_decList decs;
	A_expList exps;
	A_ty ty;
	S_symbol symbol;
	A_fieldList fields;
   A_efieldList efields;
   A_fundec fundec;
   A_fundecList fundecs;
   A_namety namety;
	A_nametyList nametys;
	}

%token <sval> ID STRING
%token <ival> INT
%token <fval> FLOAT

%token 
  COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK 
  LBRACE RBRACE DOT 
  PLUS MINUS TIMES DIVIDE EQ NEQ LT LE GT GE
  AND OR ASSIGN
  ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF 
  BREAK NIL
  FUNCTION VAR TYPE 

%type <exp> exp program 
%type <var> lvalue
%type <dec> dec vardec
%type <decs> decs 
%type <fundec> fundec
%type <fundecs> fundecs
%type <namety> tydec
%type <nametys> tydecs
%type <ty> ty
%type <exps> explist paras
%type <fields> tyfields
%type <efields> efields

%start program

%nonassoc LOW
%nonassoc DO OF
%nonassoc ASSIGN
%nonassoc TYPE FUNCTION
%nonassoc ID
%nonassoc LBRACK
%nonassoc THEN
%nonassoc ELSE

%left OR
%left AND
%nonassoc EQ NEQ LT GT LE GE
%left PLUS MINUS
%left TIMES DIVIDE
%left UMINUS


%%

program: exp    { absyn_root=$1; }

exp: lvalue                          { $$=A_VarExp(tokPos, $1); }
   | BREAK                           { $$=A_BreakExp(tokPos); }
   | NIL                             { $$=A_NilExp(tokPos); }
   | LPAREN explist RPAREN           { $$=A_SeqExp(tokPos, $2); }
   | INT                             { $$=A_IntExp(tokPos, $1);}  
   | FLOAT                           { $$=A_FloatExp(tokPos, $1);}  
   | STRING                          { $$=A_StringExp(tokPos, $1); }
   | ID LPAREN paras RPAREN          { $$=A_CallExp(tokPos, S_Symbol($1), $3); }
   | exp PLUS exp                    { $$=A_OpExp(tokPos, A_plusOp, $1, $3); }
   | exp MINUS exp                   { $$=A_OpExp(tokPos, A_minusOp, $1, $3); }
   | exp TIMES exp                   { $$=A_OpExp(tokPos, A_timesOp, $1, $3); }
   | exp DIVIDE exp                  { $$=A_OpExp(tokPos, A_divideOp, $1, $3); }
   | MINUS exp %prec UMINUS          { $$=A_OpExp(tokPos, A_minusOp, A_IntExp(0, 0), $2); }
   | exp EQ  exp                     { $$=A_OpExp(tokPos, A_eqOp, $1, $3); }
   | exp NEQ exp                     { $$=A_OpExp(tokPos, A_neqOp, $1, $3); }
   | exp GT  exp                     { $$=A_OpExp(tokPos, A_gtOp, $1, $3); }
   | exp LT  exp                     { $$=A_OpExp(tokPos, A_ltOp, $1, $3); }
   | exp GE  exp                     { $$=A_OpExp(tokPos, A_geOp, $1, $3); }
   | exp LE  exp                     { $$=A_OpExp(tokPos, A_leOp, $1, $3); }
   | exp AND exp                     { $$=A_IfExp(tokPos, $1, $3, A_IntExp(tokPos, 0)); }
   | exp OR exp                      { $$=A_IfExp(tokPos, $1, A_IntExp(tokPos, 1), $3); }
   | ID LBRACE efields RBRACE        { $$=A_RecordExp(tokPos, S_Symbol($1), $3); }
   | ID LBRACK exp RBRACK OF exp     { $$=A_ArrayExp(tokPos, S_Symbol($1), $3, $6); }
   | IF exp THEN exp ELSE exp        { $$=A_IfExp(tokPos, $2, $4, $6); }
   | IF exp THEN exp                 { $$=A_IfExp(tokPos, $2, $4, NULL); }
   | lvalue ASSIGN exp               { $$=A_AssignExp(tokPos, $1, $3); }
   | WHILE exp DO exp                { $$=A_WhileExp(tokPos, $2, $4); }
   | FOR ID ASSIGN exp TO exp DO exp { $$=A_ForExp(tokPos, S_Symbol($2), $4, $6, $8); }
   | LET decs IN explist END         { $$=A_LetExp(tokPos, $2, A_SeqExp($4->pos, $4));}  

decs: dec decs { $$=A_DecList($1,$2); }
	 | dec      { $$=A_DecList($1, NULL); }

dec: tydecs    { $$=A_TypeDec(tokPos, $1); }
   | vardec    { $$=$1; }
   | fundecs   { $$=A_FunctionDec(tokPos, $1); }

tydec: TYPE ID EQ ty       { $$=A_Namety(S_Symbol($2), $4); }

tydecs: tydec %prec LOW    { $$=A_NametyList($1,NULL); }
      | tydec tydecs       { $$=A_NametyList($1, $2); }

ty: ID                     { $$=A_NameTy(tokPos, S_Symbol($1)); }
  | LBRACE tyfields RBRACE { $$=A_RecordTy(tokPos, $2); }
  | ARRAY OF ID            { $$=A_ArrayTy(tokPos, S_Symbol($3)); }

tyfields: /* empty */                { $$=NULL; }
		  | ID COLON ID                { $$=A_FieldList(A_Field(tokPos, S_Symbol($1), S_Symbol($3)), NULL); }
		  | ID COLON ID COMMA tyfields { $$=A_FieldList(A_Field(tokPos, S_Symbol($1), S_Symbol($3)), $5); }


vardec: VAR ID ASSIGN exp          { $$=A_VarDec(tokPos, S_Symbol($2), NULL, $4); }
	   | VAR ID COLON ID ASSIGN exp { $$=A_VarDec(tokPos, S_Symbol($2), S_Symbol($4), $6); }

fundec: FUNCTION ID LPAREN tyfields RPAREN EQ exp          { $$= A_Fundec(tokPos, S_Symbol($2), $4, NULL, $7); }
	   | FUNCTION ID LPAREN tyfields RPAREN COLON ID EQ exp { $$= A_Fundec(tokPos, S_Symbol($2), $4, S_Symbol($7), $9); }

fundecs: fundec %prec LOW { $$=A_FundecList($1,NULL); }
       | fundec fundecs   { $$=A_FundecList($1, $2); }


lvalue: ID                              { $$=A_SimpleVar(tokPos, S_Symbol($1)); }
      | lvalue DOT ID                   { $$=A_FieldVar(tokPos, $1, S_Symbol($3)); }
      | lvalue LBRACK exp RBRACK        { $$=A_SubscriptVar(tokPos, $1, $3); }
      | ID LBRACK exp RBRACK            { $$=A_SubscriptVar(tokPos, A_SimpleVar(tokPos, S_Symbol($1)), $3); }
      | lvalue DOT ID LBRACK exp RBRACK { $$=A_SubscriptVar(tokPos, A_FieldVar(tokPos, $1, S_Symbol($3)), $5); }

paras: /* empty */     { $$=NULL; }
     | exp             { $$=A_ExpList(tokPos, $1, NULL); }
	  | exp COMMA paras { $$=A_ExpList(tokPos, $1, $3); }
   
efields: /* empty */             { $$=NULL; }
       | ID EQ exp               { $$=A_EfieldList(A_Efield(S_Symbol($1), $3), NULL); }
       | ID EQ exp COMMA efields { $$=A_EfieldList(A_Efield(S_Symbol($1), $3), $5); }

explist: /* empty */           { $$=NULL; }
       | exp                   { $$=A_ExpList(tokPos, $1, NULL); }
	    | exp SEMICOLON explist { $$=A_ExpList(tokPos, $1, $3); }
