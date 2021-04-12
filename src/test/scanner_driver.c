#include <stdio.h>
#include <stdlib.h>

#include "../include/error.h"
#include "../include/util.h"

typedef enum {
    ID = 258,
    STRING,
    INT,
    FLOAT,
    COMMA,
    COLON,
    SEMICOLON,
    LPAREN,
    RPAREN,
    LBRACK,
    RBRACK,
    LBRACE,
    RBRACE,
    DOT,
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    EQ,
    NEQ,
    LT,
    LE,
    GT,
    GE,
    AND,
    OR,
    ASSIGN,
    ARRAY,
    IF,
    THEN,
    ELSE,
    WHILE,
    FOR,
    TO,
    DO,
    LET,
    IN,
    END,
    OF,
    BREAK,
    NIL,
    FUNCTION,
    VAR,
    TYPE,
} TokenType;

typedef union {
    int pos;
    int ival;
    float fval;
    string sval;
} Token;

Token yylval;

int yylex(void); /* prototype for the lexing function */

string toknames[] = {
    "ID",     "STRING", "INT",    "FLOAT",  "COMMA",  "COLON",  "SEMICOLON",
    "LPAREN", "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE", "DOT",
    "PLUS",   "MINUS",  "TIMES",  "DIVIDE", "EQ",     "NEQ",    "LT",
    "LE",     "GT",     "GE",     "AND",    "OR",     "ASSIGN", "ARRAY",
    "IF",     "THEN",   "ELSE",   "WHILE",  "FOR",    "TO",     "DO",
    "LET",    "IN",     "END",    "OF",     "BREAK",  "NIL",    "FUNCTION",
    "VAR",    "TYPE"};

string tokname(int tok) {
    return tok < 258 || tok > 301 ? "\033[1;31mBAD_TOKEN\033[0m"
                                  : toknames[tok - 258];
}

int main(int argc, char **argv) {
    string fname;
    int tok;
    if (argc != 2) {
        fprintf(stderr, "usage: a.out filename\n");
        exit(1);
    }
    fname = argv[1];
    reset(fname);
    for (;;) {
        tok = yylex();
        if (tok == 0) break;
        switch (tok) {
            case ID:
            case STRING:
                printf("%10s %4d %s\n", tokname(tok), tokPos, yylval.sval);
                break;
            case INT:
                printf("%10s %4d %d\n", tokname(tok), tokPos, yylval.ival);
                break;
            case FLOAT:
                printf("%10s %4d %f\n", tokname(tok), tokPos, yylval.fval);
                break;
            default:
                printf("%10s %4d\n", tokname(tok), tokPos);
        }
    }
    return 0;
}
