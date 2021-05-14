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
        "allocation [not available yet]\n");
    printf(
        "    -o               sets the name of the output binary               "
        "        [not available yet]\n");
}

SemantReturn frontend(char *input_file, A_exp *absyn_root) {
    FILE *out = stdout;

    reset(input_file);
    A_exp absyn_tree_root = parse(input_file);

    if (absyn_tree_root == NULL) {
        SemantReturn ret = {.any_errors = EXIT_FAILURE, .tree_root = NULL};
        return ret;
    }

    *absyn_root = absyn_tree_root;  // sets for return
    if (print_tree) {
        fprintf(out, "=============       AST      =============\n");
        pr_exp(out, absyn_tree_root, 0);
        fprintf(out, "\n");
    }

    SemantReturn sem_ret = SEM_transProg(absyn_tree_root);

    return sem_ret;
}

int backend(Tr_exp exp) {
    T_stmList stmList = NULL;
    F_fragList f_frags = NULL;
    F_fragList s_frags = NULL;

    Tr_procEntryExit(Tr_outermost(), exp, NULL);

    if (print_ir) {
        fprintf(stdout, "=============       IR       =============\n");
        Tr_printTree(exp);
    }

    f_frags = Tr_getResult();

    if (print_before_assembly) {
        for (s_frags = f_frags; s_frags; s_frags = s_frags->tail)
            if (s_frags->head->kind == F_stringFrag)
                fprintf(stdout, "%s: %s\n\n", Temp_labelstring(Temp_newlabel()),
                        s_frags->head->u.stringg.str);
    }

    for (F_fragList f = f_frags; f; f = f->tail) {
        stmList = C_linearize(f->head->u.proc.body);
        stmList = C_traceSchedule(C_basicBlocks(stmList));

        if (print_canonical) {
            Tr_printCanonicalTree(stmList);
        }

        if (f->head->kind == F_procFrag) {
            AS_instrList instr_l = F_codegen(f->head->u.proc.frame, stmList);

            if (print_before_assembly) {
                AS_printInstrList(stdout, instr_l, F_tempMap());
            }
        }
    }
    return 0;
}

int program(char *input_file, char *output_file) {
    A_exp absyn_root = NULL;
    SemantReturn sem_ret = frontend(input_file, &absyn_root);

    if (sem_ret.any_errors) {
        exit(EXIT_FAILURE);
    }

    return backend(sem_ret.tree_root);
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
