#include <stdio.h>
#include <stdlib.h>

#include "include/absyn.h"
#include "include/error.h"
#include "include/parse.h"
#include "include/prabsyn.h"
#include "include/util.h"

int main(int argc, char** argv) {
    A_exp absyn_root;
    FILE* out = stdout;

    if (argc != 2) {
        fprintf(stderr, "usage: a.out filename\n");
        exit(1);
    }
    absyn_root = parse(argv[1]);

    if (!absyn_root) exit(1);

#if 1
    pr_exp(out, absyn_root, 0); /* print absyn data structure */
    fprintf(out, "\n");
#endif
    return 0;
}