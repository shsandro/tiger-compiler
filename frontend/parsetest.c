#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "util.h"

extern int yyparse(void);

void parse(string fname) {
    reset(fname);
    if (yyparse() == 0) /* parsing worked */
        fprintf(stderr, "Parsing successful!\n");
    else
        fprintf(stderr, "Parsing failed\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: a.out filename\n");
        exit(1);
    }
    parse(argv[1]);
    return 0;
}