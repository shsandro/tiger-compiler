#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "frontend/include/absyn.h"
#include "frontend/include/error.h"
#include "frontend/include/parse.h"
#include "frontend/include/prabsyn.h"
#include "frontend/include/semant.h"

#define TRUE 1
#define FALSE 0

void help_menu(const char *prog_name) {
    printf("Usage: %s [flags]\n", prog_name);
    printf("    -h               prints this usage guide\n");
    printf("    -p               sets the input file path\n");
    printf("    -a               prints the abstract syntax tree\n");
    printf(
        "    -o               sets the name of the output binary     [not "
        "available yet]\n");
    printf(
        "    -i               prints the intermediate representation [not "
        "available yet]\n");
    printf(
        "    -s               prints the generated assembly code     [not "
        "available yet]\n");
}

int frontend(char *input_file, int print_tree, A_exp *absyn_root) {
    FILE *out = stdout;

    reset(input_file);
    A_exp absyn_tree_root = parse(input_file);

    if (absyn_tree_root == NULL) {
        return EXIT_FAILURE;
    }

    *absyn_root = absyn_tree_root;  // sets for return
    if (print_tree) {
        pr_exp(out, absyn_tree_root, 0);
        fprintf(out, "\n");
    }

    return SEM_transProg(absyn_tree_root);
}

int main(int argc, char *const *argv) {
    int ch;
    char *output_file, *input_file = NULL;
    int print_tree = FALSE, print_ir = FALSE, print_assembly = FALSE;
    A_exp absyn_root = NULL;

    while ((ch = getopt(argc, argv, "aho:p:is")) != -1) {
        switch (ch) {
            case 'p':
                input_file = (char *)malloc(strlen(optarg));
                strcpy(input_file, optarg);
                break;

            case 'a':
                print_tree = TRUE;
                break;

            case 'o':
                output_file = (char *)malloc(strlen(optarg));
                strcpy(output_file, optarg);
                break;

            case 'i':
                print_ir = TRUE;
                break;

            case 's':
                print_assembly = TRUE;
                break;

            case 'h':
                help_menu(input_file);
                exit(EXIT_SUCCESS);
                break;

            default:
                help_menu(input_file);
                exit(EXIT_FAILURE);
                break;
        }
    }

    if (!input_file) {
        help_menu(input_file);
        exit(EXIT_FAILURE);
    }

    if (!frontend(input_file, print_tree, &absyn_root)) {
        exit(EXIT_FAILURE);
    }
}
