/*
 * parse.c - Parse source file.
 */

#include "include/parse.h"

#include <stdio.h>

#include "include/absyn.h"
#include "include/error.h"
#include "include/symbol.h"
#include "include/util.h"

extern int yyparse(void);
extern A_exp absyn_root;

/* parse source file fname;
   return abstract syntax data structure */
A_exp parse(string fname) {
    reset(fname);
    if (yyparse() == 0) {
        fprintf(stderr, "Parsing successful!\n\n"); /* parsing worked */
        return absyn_root;
    } else {
        fprintf(stderr, "Parsing failed\n");
        return NULL;
    }
}
