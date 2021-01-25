#include "util.h"

typedef enum {
    ID = 1,
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

extern Token yylval;
