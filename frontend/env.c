#include "include/env.h"

#include <stdio.h>

#include "include/symbol.h"
#include "include/types.h"
#include "include/util.h"

E_enventry E_VarEntry(Ty_ty ty) {
    E_enventry e = checked_malloc(sizeof(*e));
    e->kind = E_varEntry;
    e->u.var.ty = ty;
    return e;
}

E_enventry E_FunEntry(Ty_tyList formals, Ty_ty result) {
    E_enventry e = checked_malloc(sizeof(*e));
    e->kind = E_funEntry;
    e->u.fun.formals = formals;
    e->u.fun.result = result;
    return e;
}

/* contains predefined functions */
S_table E_base_tenv(void) {
    S_table tenv = S_empty();
    S_enter(tenv, S_Symbol("int"), Ty_Int());
    S_enter(tenv, S_Symbol("float"), Ty_Float());
    S_enter(tenv, S_Symbol("string"), Ty_String());
    S_enter(tenv, S_Symbol("nil"), Ty_Nil());
    return tenv;
}

#define ORD "ord"
#define GETCHAR "getchar"
#define CHR "chr"
#define PRINT "print"
#define PRINTI "printi"
#define END ""

char* externalCallNames[] = {ORD, GETCHAR, CHR, PRINT, PRINTI, END};

S_table E_base_venv(void) {
    S_table venv = S_empty();
    S_enter(venv, S_Symbol(ORD),
            E_FunEntry(Ty_TyList(Ty_String(), NULL), Ty_Int()));
    S_enter(venv, S_Symbol(GETCHAR), E_FunEntry(NULL, Ty_String()));
    S_enter(venv, S_Symbol(CHR),
            E_FunEntry(Ty_TyList(Ty_Int(), NULL), Ty_String()));
    S_enter(venv, S_Symbol(PRINT),
            E_FunEntry(Ty_TyList(Ty_String(), NULL), Ty_Void()));
    S_enter(venv, S_Symbol(PRINTI),
            E_FunEntry(Ty_TyList(Ty_Int(), NULL), Ty_Void()));

    return venv;
}
