#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *checked_malloc(int len) {
    void *p = malloc(len);

    if (!p) {
        fprintf(stderr, "\nRan out of memory!\n");
        exit(1);
    }

    return p;
}

string String(char *s) {
    string p = checked_malloc(strlen(s) - 1);
    s++;
    memcpy(p, s, strlen(s) - 1);
    p[strlen(s) - 1] = '\0';

    return p;
}

string Id(char *s) {
    string p = checked_malloc(strlen(s) + 1);
    strcpy(p, s);
    return p;
}
