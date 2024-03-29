#ifndef UTIL_H
#define UTIL_H
#include <assert.h>
#include <stdbool.h>

typedef char *string;

#define TRUE 1
#define FALSE 0

void *checked_malloc(int);
string String(char *);
string Id(char *);

typedef struct U_boolList_ *U_boolList;
struct U_boolList_ {
    bool head;
    U_boolList tail;
};
U_boolList U_BoolList(bool head, U_boolList tail);

string FormatString(string s, ...);

#endif