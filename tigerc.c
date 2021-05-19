#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "src/include/absyn.h"
#include "src/include/canon.h"
#include "src/include/codegen.h"
#include "src/include/error.h"
#include "src/include/frame.h"
#include "src/include/parse.h"
#include "src/include/prabsyn.h"
#include "src/include/semant.h"
#include "src/include/translate.h"

#define TRUE 1
#define FALSE 0

int print_tree = FALSE, print_ir = FALSE, print_before_assembly = FALSE,
    print_after_assembly = FALSE, print_canonical = FALSE;

void help_menu(const char *prog_name) {
    printf("Usage: %s [flags]\n", prog_name);
    printf("    -h               prints this usage guide\n");
    printf("    -p               sets the input file path\n");
    printf("    -a               prints the abstract syntax tree\n");
    printf("    -i               prints the intermediate representation\n");
    printf(
        "    -c               prints the canonical intermediate representation "
        "tree\n");
    printf(
        "    -s               prints the generated assembly code before regs "
        "allocation\n");
    printf(
        "    -S               prints the generated assembly code after regs "
        "allocation [not available]\n");
    printf(
        "    -o               sets the name of the output binary               "
        "        [not available]\n");
}

int frontend(char *input_file, A_exp *absyn_root) {
    FILE *out = stdout;

    reset(input_file);
    A_exp absyn_tree_root = parse(input_file);

    if (absyn_tree_root == NULL) {
        return EXIT_FAILURE;
    }

    *absyn_root = absyn_tree_root;  // sets for return
    if (print_tree) {
        fprintf(out, "=============       AST      =============\n");
        pr_exp(out, absyn_tree_root, 0);
        fprintf(out, "\n");
    }

    return SEM_transProg(absyn_tree_root);
}

int program(char *input_file, char *output_file) {
    A_exp absyn_root = NULL;
    int sem_ret = frontend(input_file, &absyn_root);

    if (sem_ret) {
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

int main(int argc, char *const *argv) {
    int ch;
    char *output_file, *input_file = NULL;
    Tr_exp ir = NULL;

    while ((ch = getopt(argc, argv, "aho:p:isSc")) != -1) {
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

            case 'c':
                print_canonical = TRUE;
                break;

            case 's':
                print_before_assembly = TRUE;
                break;

            case 'S':
                print_after_assembly = TRUE;
                break;

            case 'h':
                input_file ? help_menu(input_file) : help_menu("./tigerc");
                exit(EXIT_SUCCESS);
                break;

            default:
                help_menu(input_file);
                exit(EXIT_FAILURE);
                break;
        }
    }

    if (!input_file) {
        help_menu("./tigerc");
        exit(EXIT_FAILURE);
    }

    return program(input_file, output_file);
}
