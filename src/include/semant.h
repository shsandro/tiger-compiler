#ifndef SEMANT_H__
#define SEMANT_H__

#include "absyn.h"
#include "frame.h"
#include "translate.h"

typedef struct {
    int any_errors;
    Tr_exp tree_root;
    F_fragList f_frag;
} SemantReturn;

SemantReturn SEM_transProg(A_exp exp);

#endif