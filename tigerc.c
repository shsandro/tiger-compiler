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

static void do_proc(FILE *out, F_frame frame, T_stm body) {
    AS_instrList instr_l = NULL;
    T_stmList stm_l = NULL;

    stm_l = C_linearize(body);
    stm_l = C_traceSchedule(C_basicBlocks(stm_l));

    if (print_canonical) {
        fprintf(stdout, "============= Canonical Tree =============\n");
        Tr_printCanonicalTree(stm_l);
        print_canonical = 0;
    }

    instr_l = F_codegen(frame, stm_l);

    if (print_before_assembly) {
        fprintf(stdout, "============= Crude Assembly =============\n");
        AS_printInstrList(out, instr_l, F_tempMap());
    }
}

int backend(F_fragList frags) {
    AS_instrList instr_l = NULL;
    T_stmList stm_l = NULL;

    F_fragList f = frags;
    for (; f; f = f->tail) {
        if (f->head->kind == F_procFrag) {
            do_proc(stdout, f->head->u.proc.frame, f->head->u.proc.body);
        }
    }
}

int main(int argc, char *const *argv) {
    int ch;
    char *output_file, *input_file = NULL;
    A_exp absyn_root = NULL;
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

    SemantReturn sem_ret = frontend(input_file, &absyn_root);

    if (sem_ret.any_errors) {
        exit(EXIT_FAILURE);
    }

    if (print_ir) {
        fprintf(stdout, "=============       IR       =============\n");
        Tr_printTree(sem_ret.tree_root);
    }

    backend(sem_ret.f_frag);
}
