
#include "error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

int any_errors = FALSE;

static string filename = "";
static int line_num = 1;

int tokPos = 0;

extern FILE *yyin;

typedef struct intList {
    int i;
    struct intList *rest;
} * IntList;

static IntList intList(int i, IntList rest) {
    IntList l = checked_malloc(sizeof *l);

    l->i = i;
    l->rest = rest;

    return l;
}

static IntList linePos = NULL;

void newline() {
    line_num++;
    linePos = intList(tokPos, linePos);
}

void error(int pos, char *message, ...) {
    va_list ap;
    IntList lines = linePos;
    int num = line_num;

    any_errors = TRUE;
    while (lines && lines->i >= pos) {
        lines = lines->rest;
        num--;
    }

    if (filename) fprintf(stderr, "%s:", filename);
    if (lines) fprintf(stderr, "%d.%d: ", num, pos - lines->i);

    va_start(ap, message);
    vfprintf(stderr, message, ap);
    va_end(ap);
    fprintf(stderr, "\n");
}

void reset(string fname) {
    any_errors = FALSE;
    filename = fname;
    line_num = 1;
    linePos = intList(0, NULL);
    yyin = fopen(fname, "r");
    if (!yyin) {
        error(0, "cannot open");
        exit(1);
    }
}