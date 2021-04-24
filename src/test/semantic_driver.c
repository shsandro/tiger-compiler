#include <stdio.h>
#include <stdlib.h>

#include "../include/absyn.h"
#include "../include/error.h"
#include "../include/parse.h"
#include "../include/semant.h"
#include "../include/symbol.h"
#include "../include/util.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: a.out filename\n");
        exit(1);
    }

    reset(argv[1]);
    A_exp absyn_tree_root = parse(argv[1]);

    if (absyn_tree_root == NULL) {
        return 1;
    }

    return SEM_transProg(absyn_tree_root).any_errors;
}
