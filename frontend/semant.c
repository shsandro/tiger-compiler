#include "include/semant.h"

#include <stdio.h>
#include <stdlib.h>

#include "include/env.h"
#include "include/error.h"
#include "include/types.h"
#include "include/util.h"

typedef void *Tr_exp;

typedef struct expty_ {
    Tr_exp exp;
    Ty_ty ty;
} expty;

expty expTy(Tr_exp exp, Ty_ty ty) {
    expty e;
    e.exp = exp;
    e.ty = ty;
    return e;
}

expty transVar(S_table venv, S_table tenv, A_var v);
expty transExp(S_table venv, S_table tenv, A_exp e);
void transDec(S_table venv, S_table tenv, A_dec d);
Ty_ty transTy(S_table tenv, A_ty t);
void checkTypeDec(S_table tenv, Ty_tyList tl);
Ty_tyList nametyList(S_table tenv, A_nametyList nl);
void checkRecord(S_table tenv, Ty_fieldList fl);

// inside flag (for loop, while loop)
static int inside = 0;

//#define dprintf(...) printf(...)
#define dprintf(...)

int SEM_transProg(A_exp exp) {
    S_table tenv = E_base_tenv();
    S_table venv = E_base_venv();

    transExp(venv, tenv, exp);

    printf("Errors? %d\n", anyErrors());

    return anyErrors();
}

Ty_ty actual_ty(Ty_ty ty) {
    assert(ty);
    Ty_ty tmpty;
    while (ty->kind == Ty_name) {
        dprintf("ty...%s\n", S_name(ty->u.name.sym));
        tmpty = ty->u.name.ty;
        if (tmpty)
            ty = tmpty;
        else
            return NULL;  // XXX ugly code
    }
    dprintf("return ty %d\n", ty->kind - Ty_record);
    return ty;
}

// Compare two actual type
// Ty_nil == Ty_record
// Ty_record or Ty_array: compare the reference
static bool actual_eq(Ty_ty source, Ty_ty target) {
    Ty_ty t1 = actual_ty(source);
    Ty_ty t2 = actual_ty(target);
    return ((t1->kind == Ty_record || t1->kind == Ty_array) && t1 == t2) ||
           (t1->kind == Ty_record && t2->kind == Ty_nil) ||
           (t1->kind == Ty_nil && t2->kind == Ty_record) ||
           (t1->kind != Ty_record && t1->kind != Ty_array &&
            t1->kind == t2->kind);
}

Ty_tyList makeFormalTyList(S_table tenv, A_fieldList afl) {
    if (!afl) return NULL; /* ugly special case */
    Ty_tyList ttl = checked_malloc(sizeof(*ttl));
    Ty_tyList ttl_hd = ttl;
    Ty_tyList last = NULL;
    while (afl != NULL) {
        ttl->tail = checked_malloc(sizeof(*(ttl->tail)));

        if (afl->head->typ == S_Symbol("int"))
            ttl->head = Ty_Int();
        else if (afl->head->typ == S_Symbol("string"))
            ttl->head = Ty_String();
        else if (afl->head->typ == S_Symbol("float"))
            ttl->head = Ty_Float();
        else
            ttl->head = S_look(tenv, afl->head->typ);

        dprintf("formal list:%s %x\n", S_name(afl->head->typ), ttl);
        if (ttl->head == NULL) {
            error(afl->head->pos, " undefined type: %s",
                  S_name(afl->head->typ));
        }
        last = ttl;
        ttl = ttl->tail;
        afl = afl->tail;
    }
    if (last) last->tail = NULL;  // have to clear it.
    return ttl_hd;
}

expty transExp(S_table venv, S_table tenv, A_exp a) {
    switch (a->kind) {
        case A_varExp: {
            dprintf("evaluate var exp\n");
            return transVar(venv, tenv, a->u.var);
        }
        case A_nilExp: {
            dprintf(" nil!\n");
            return expTy(NULL, Ty_Nil());
        }
        case A_intExp: {
            dprintf(" int!\n");
            return expTy(NULL, Ty_Int());
        }
        case A_floatExp: {
            dprintf(" float!\n");
            return expTy(NULL, Ty_Float());
        }
        case A_stringExp: {
            dprintf(" string!\n");
            return expTy(NULL, Ty_String());
        }
        case A_callExp: {
            E_enventry et = S_look(venv, a->u.call.func);
            printf(" call! look in %p [%s:%p]\n", venv, S_name(a->u.call.func),
                   et);
            if (et == NULL || et->kind != E_funEntry) {
                error(a->pos, "call expression: undefined type %s",
                      S_name(a->u.call.func));
                return expTy(NULL, Ty_Nil());
            }
            A_expList el = a->u.call.args;
            Ty_tyList formals = et->u.fun.formals;
            expty tmp;
            while (formals != NULL) {
                if (el == NULL) {
                    error(a->pos, "call expression: too few arguments");
                    return expTy(NULL, Ty_Nil());
                } else {
                    tmp = transExp(venv, tenv, el->head);
                    if (formals->head != tmp.ty)
                        error(a->pos,
                              "call expression: argument type dosen't match "
                              "the paramater");
                }
                formals = formals->tail;
                el = el->tail;
            }
            if (el != NULL) {
                error(a->pos, "call expression: too many arguments %s",
                      S_name(a->u.call.func));
                return expTy(NULL, et->u.fun.result);
            } else {
                return expTy(NULL, et->u.fun.result);
            }
        }
        case A_opExp: {
            dprintf(" op!\n");
            A_oper oper = a->u.op.oper;
            expty left = transExp(venv, tenv, a->u.op.left);
            expty right = transExp(venv, tenv, a->u.op.right);
            if (oper == A_plusOp || oper == A_minusOp || oper == A_timesOp ||
                oper == A_divideOp) {
                // operand must be integer or float, return value must be
                // integer or float
                if (left.ty->kind != Ty_int && left.ty->kind != Ty_float)
                    error(a->u.op.left->pos,
                          "binary operation: integer or float required");
                if (right.ty->kind != Ty_int && right.ty->kind != Ty_float)
                    error(a->u.op.right->pos,
                          "binary operation: integer or float required");
                if (right.ty->kind != right.ty->kind)
                    error(a->u.op.left->pos,
                          "binary operation: same type required");
                // arithmatic return.
                if (right.ty->kind == Ty_int)
                    return expTy(NULL, Ty_Int());
                else
                    return expTy(NULL, Ty_Float());
            } else if (left.ty->kind != right.ty->kind && left.ty != Ty_Nil() &&
                       right.ty != Ty_Nil()) {
                error(a->u.op.left->pos,
                      "operators compare: different type for compare");
            } else if (oper != A_eqOp && oper != A_neqOp) {  // lt, le, gt, ge
                if ((left.ty->kind != Ty_int && left.ty->kind != Ty_float) ||
                    (right.ty->kind != Ty_int && right.ty->kind != Ty_float)) {
                    error(a->u.op.left->pos,
                          "binary compare: integer or float required");
                }
            } else if (!(left.ty->kind == Ty_int || left.ty->kind == Ty_array ||
                         left.ty->kind == Ty_record ||
                         left.ty->kind == Ty_string)) {
                if (right.ty != Ty_Nil()) {
                    error(a->u.op.left->pos, "binary compare: can't compare!");
                }
            }

            // next step we need to discriminate return value by string or not
            return expTy(NULL, Ty_Int());
        }
        case A_recordExp: {
            dprintf(" record!\n");
            Ty_ty recordty = S_look(tenv, a->u.record.typ);
            if (NULL == recordty) {
                error(a->pos, "record expression: undefined type %s",
                      S_name(a->u.record.typ));
                return expTy(NULL, Ty_Record(NULL));
            }
            if (recordty->kind != Ty_record) {
                error(a->pos, "record expression: <%s> is not a record type",
                      S_name(a->u.record.typ));
                return expTy(NULL, Ty_Record(NULL));
            }

            Ty_fieldList ty_fl;
            A_efieldList fl;
            for (fl = a->u.record.fields, ty_fl = recordty->u.record;
                 fl && ty_fl; fl = fl->tail, ty_fl = ty_fl->tail) {
                if (fl->head->name != ty_fl->head->name) {
                    error(a->pos,
                          "record expression: <%s> not a valid field name",
                          S_name(fl->head->name));
                    return expTy(NULL, Ty_Record(NULL));
                }
                expty exp = transExp(venv, tenv, fl->head->exp);
                if (!actual_eq(exp.ty, ty_fl->head->ty)) {
                    error(a->pos,
                          "record expression: both field types dismatch");
                    return expTy(NULL, Ty_Record(NULL));
                }
            }

            return expTy(NULL, recordty);
        }
        case A_seqExp: {
            dprintf(" seq!\n");
            expty exp = expTy(NULL, Ty_Void());
            A_expList el;
            for (el = a->u.seq; el; el = el->tail) {
                exp = transExp(venv, tenv, el->head);
            }
            return exp;
        }
        case A_assignExp: {
            dprintf(" assgin!\n");
            expty var = transVar(venv, tenv, a->u.assign.var);
            expty exp = transExp(venv, tenv, a->u.assign.exp);
            if (var.ty != exp.ty)
                error(a->pos,
                      "assign expression: dismatch type between variable and "
                      "expression <%d> <%d>",
                      var.ty->kind, exp.ty->kind);
            return expTy(NULL, Ty_Void());
        }
        case A_ifExp: {
            dprintf(" if!\n");
            expty cond = transExp(venv, tenv, a->u.iff.test);
            if (cond.ty != Ty_Int()) {
                error(a->pos, "condition expression: if test must produce int");
                return expTy(NULL, Ty_Void());
            }
            dprintf("  then!\n");
            expty thenet = transExp(venv, tenv, a->u.iff.then);
            if (a->u.iff.elsee) {  // if-then-else
                dprintf("  else!\n");
                expty elseet = transExp(venv, tenv, a->u.iff.elsee);
                // if-then: no return value. if-then-else: can have return value
                if (elseet.ty != thenet.ty) {
                    // XXX: shoud this semantic contain Ty_Void()?
                    if (!((elseet.ty == Ty_Nil() || elseet.ty == Ty_Void()) ||
                          (thenet.ty == Ty_Nil() || thenet.ty == Ty_Void())))
                        error(a->pos,
                              "condition expression: then-else section must be "
                              "the same type");
                }
                return expTy(NULL, thenet.ty);
            } else if (thenet.ty != Ty_Void()) {  // if-then
                error(a->pos,
                      "condition expression: if-then exp's body must produce "
                      "no value");
            }
            return expTy(NULL, thenet.ty);
        }
        case A_whileExp: {
            dprintf(" while!\n");
            expty test = transExp(venv, tenv, a->u.whilee.test), body;
            if (test.ty != Ty_Int())
                error(a->pos, "while loop: while test must produce int");

            inside++;  // inside loop
            body = transExp(venv, tenv, a->u.whilee.body);
            inside--;  // outside

            if (body.ty != Ty_Void() && body.ty != Ty_Nil()) {
                error(a->pos, "while loop: body section must produce no value");
                return expTy(NULL, Ty_Void());
            }

            return expTy(NULL, Ty_Void());
        }
        case A_forExp: {
            expty lo = transExp(venv, tenv, a->u.forr.lo);
            expty hi = transExp(venv, tenv, a->u.forr.hi);

            if (lo.ty->kind != Ty_int) {
                error(a->u.forr.lo->pos,
                      "for loop: lower part must be integer");
                return expTy(NULL, Ty_Void());
            }

            if (hi.ty->kind != Ty_int) {
                error(a->u.forr.hi->pos,
                      "for loop: higer part must be integer");
                return expTy(NULL, Ty_Void());
            }

            S_beginScope(venv);
            S_enter(venv, a->u.forr.var, Ty_Int());

            inside++;  // inside loop
            expty body = transExp(venv, tenv, a->u.forr.body);
            inside--;  // outside

            if (body.ty->kind != Ty_void) {
                error(a->u.forr.body->pos, "for loop: body part must be void");
                return expTy(NULL, Ty_Void());
            }

            S_endScope(venv);

            return expTy(NULL, Ty_Void());
        }
        case A_breakExp: {
            dprintf(" break!\n");
            if (!inside) {
                error(a->pos,
                      "break expression: break expression outside loop");
            }
            return expTy(NULL, Ty_Void());
        }
        case A_letExp: {
            dprintf(" let!\n");
            A_decList d;
            S_beginScope(venv);
            S_beginScope(tenv);

            for (d = a->u.let.decs; d; d = d->tail) {
                // append in a reversed order. but don't worry. check return.
                transDec(venv, tenv, d->head);
            }

            dprintf(" let body\n");
            expty et = transExp(venv, tenv, a->u.let.body);
            dprintf(" end let body\n");

            S_endScope(tenv);
            S_endScope(venv);
            // seqExp reverse the list again. so everything is good.
            return et;
        }
        case A_arrayExp: {
            dprintf(" array!\n");
            expty init, size;
            Ty_ty typ = S_look(tenv, a->u.array.typ);
            dprintf(" looking %s:%d\n", S_name(a->u.array.typ), typ->kind);

            if (!typ) {
                error(a->pos, "array expression: undefined type %s",
                      S_name(a->u.array.typ));
                return expTy(NULL, Ty_Array(NULL));
            }

            init = transExp(venv, tenv, a->u.array.init);

            if (typ->u.array->kind == Ty_name)
                typ->u.array = S_look(tenv, typ->u.array->u.name.sym);
            if (init.ty != typ->u.array) {
                error(a->pos,
                      "array expression: initialize type does not match with "
                      "given type");
                return expTy(NULL, Ty_Array(typ));
            }

            size = transExp(venv, tenv, a->u.array.size);

            if (size.ty != Ty_Int()) {
                error(a->pos, "array expression: array length must be int.");
                return expTy(NULL, Ty_Array(typ));
            }

            return expTy(NULL, typ);
        }
        default:;
    }
    assert(0);  // should have returned from some clause of the switch
}

expty transVar(S_table venv, S_table tenv, A_var v) {
    switch (v->kind) {
        case A_simpleVar: {
            E_enventry x = S_look(venv, v->u.simple);
            printf(" simple var! %s\n", S_name(v->u.simple));

            if (x && x->kind == E_varEntry) {
                return expTy(NULL, actual_ty(x->u.var.ty));
            } else {
                error(v->pos, "simple var expression: undefined variable %s",
                      S_name(v->u.simple));
                return expTy(NULL, Ty_Int());
            }
        }
        case A_fieldVar: {
            dprintf(" fieldvar!\n");
            expty et = transVar(venv, tenv, v->u.field.var);

            if (et.ty->kind == Ty_record) {
                Ty_fieldList fl;

                for (fl = et.ty->u.record; fl; fl = fl->tail) {
                    if (fl->head->name == v->u.field.sym) {
                        return expTy(NULL, actual_ty(fl->head->ty));
                    }
                }

                error(v->u.field.var->pos,
                      "field var expression: no such field <%s> in the record",
                      S_name(v->u.field.sym));

                return expTy(NULL, Ty_Int());
            } else {
                error(v->u.field.var->pos,
                      "field var expression: not a record type variable");
                return expTy(NULL, Ty_Int());
            }
        }
        case A_subscriptVar: {
            dprintf("subscript!\n");
            expty et = transVar(venv, tenv, v->u.subscript.var);
            expty etint = transExp(venv, tenv, v->u.subscript.exp);

            if (etint.ty != Ty_Int()) {
                error(v->u.subscript.var->pos,
                      "subscript var expression: int required in array "
                      "index");
                return expTy(NULL, Ty_Int());
            }

            if (actual_ty(et.ty)->kind == Ty_array) {
                Ty_ty arr = et.ty->u.array;
                return expTy(NULL, arr);
            } else {
                error(v->u.subscript.var->pos,
                      "subscript var expression: not an array type variable");
                return expTy(NULL, Ty_Int());
            }
        }
        defalut:;
    }
    assert(0);  // wrong kind
}

void transDec(S_table venv, S_table tenv, A_dec d) {
    switch (d->kind) {
        case A_varDec: {
            dprintf(" dec:vardec:%p %p\n", d->u.var.var, d->u.var.typ);
            expty e = transExp(venv, tenv, d->u.var.init);

            // Where escape happens.
            E_enventry eentry = E_VarEntry(e.ty);
            Ty_ty ty;

            if (d->u.var.typ == NULL) {
                if (e.ty == Ty_Nil())
                    error(d->pos,
                          "variable declare: init should not be nil without "
                          "type specified");
            } else if ((ty = S_look(tenv, d->u.var.typ)) != e.ty) {
                if (e.ty != Ty_Nil())
                    error(d->pos,
                          "variable declare: dismatch type between declare and "
                          "initialze");
            }

            dprintf(" put to %p [%p:%p]\n", venv, d->u.var.var, eentry);
            S_enter(venv, d->u.var.var, eentry);
            return;
        }
        case A_typeDec: {
            dprintf(" dec:typedec:%p\n", d->u.type);
            Ty_tyList tl = nametyList(tenv, d->u.type);
            checkTypeDec(tenv, tl);
            return;
        }
        case A_functionDec: {
            A_fundecList fun_list;

            int index = 0;
            void *typenames[10];  // store typenames in list, check for
                                  // redeclaration

            // example: function treeLeaves(t: tree): int =
            // treelistLeaves(t.children) 1: add header (function treeLeaves(t:
            // tree): int =)
            for (fun_list = d->u.function; fun_list;
                 fun_list = fun_list->tail) {
                // i need elements of E_FunEntry, because its value environment
                A_fieldList fl;
                Ty_ty ty;

                Ty_tyList head = NULL, tail = NULL;
                Ty_ty r;

                // return type
                if (fun_list->head->result) {
                    r = S_look(tenv, fun_list->head->result);
                    if (!r) {
                        error(fun_list->head->pos,
                              "function declare: undefined return type %s",
                              S_name(fun_list->head->result));
                        return;
                    }
                } else {
                    r = Ty_Void();
                }
                // parameters
                for (fl = fun_list->head->params; fl; fl = fl->tail) {
                    ty = S_look(tenv, fl->head->typ);
                    if (!ty) {
                        error(fl->head->pos,
                              "function declare: undefined parameter type %s",
                              S_name(fl->head->typ));
                        return;
                    }
                    if (head) {
                        tail->tail = Ty_TyList(ty, NULL);
                        tail = tail->tail;
                    } else {
                        head = Ty_TyList(ty, NULL);
                        tail = head;
                    }
                }
                S_enter(venv, fun_list->head->name, E_FunEntry(head, r));
                dprintf(" \tfuncdec:label [%s]\n", S_name(f->name));

                for (int i = 0; i < index; i++) {
                    if (typenames[i] == (void *)fun_list->head->name) {
                        error(
                            fun_list->head->pos,
                            "type declare: redeclaration type <%s>, there are "
                            "two types with the same name in the same "
                            "(consecutive) batch of mutually recursive types.",
                            S_name(fun_list->head->name));
                    }
                }
                typenames[index++] = (void *)fun_list->head->name;
            }
            // 2: translate body (treelistLeaves(t.children))
            for (fun_list = d->u.function; fun_list;
                 fun_list = fun_list->tail) {
                E_enventry fun_entry = S_look(venv, fun_list->head->name);
                S_beginScope(venv);
                // add parameters into environment
                A_fieldList fl;
                Ty_tyList tl = fun_entry->u.fun.formals;

                for (fl = fun_list->head->params; fl;
                     fl = fl->tail, tl = tl->tail) {
                    S_enter(venv, fl->head->name, E_VarEntry(tl->head));
                }
                // translate body
                expty exp = transExp(venv, tenv, fun_list->head->body);
                // compare return type and body type
                if (!actual_eq(fun_entry->u.fun.result, exp.ty)) {
                    error(
                        d->pos,
                        "function declare: body type and return type with <%s>",
                        S_name(fun_list->head->name));
                    S_endScope(venv);
                    return;
                }
                S_endScope(venv);
            }
            return;
        }
    }
}

Ty_ty transTy(S_table tenv, A_ty a) {
    // assert(a != NULL); // might cause trouble in recursive dec!
    if (a == NULL) {
        dprintf(" type void\n");
        return Ty_Void();
    }
    switch (a->kind) {
        case A_nameTy: {
            Ty_ty ty = S_look(tenv, a->u.name);
            if (!ty) {
                error(a->pos, "translate name type: undefined type %s",
                      S_name(a->u.name));
                return Ty_Void();
            }

            if (a->u.name == S_Symbol("int")) {
                dprintf(" type int\n");
            } else if (a->u.name == S_Symbol("string")) {
                dprintf(" type string\n");
            } else if (a->u.name == S_Symbol("string")) {
                dprintf(" type float\n");
            } else {
                dprintf(" type name\n");
                // return Ty_Name(a->u.name, NULL);  // set to null first
            }

            return ty;
        }
        case A_recordTy: {
            dprintf(" type record\n");
            A_fieldList fl;
            Ty_field ty_f;
            Ty_fieldList ty_fl_head = NULL, ty_fl_tail = NULL;
            Ty_ty ty;
            for (fl = a->u.record; fl; fl = fl->tail) {
                ty = S_look(tenv, fl->head->typ);
                if (!ty) {
                    error(fl->head->pos,
                          "translate record type: undefined type %s",
                          S_name(fl->head->typ));
                    return Ty_Void();
                }
                ty_f = Ty_Field(fl->head->name, ty);
                if (ty_fl_head) {
                    ty_fl_tail->tail = Ty_FieldList(ty_f, NULL);
                    ty_fl_tail = ty_fl_tail->tail;
                } else {
                    ty_fl_head = Ty_FieldList(ty_f, NULL);
                    ty_fl_tail = ty_fl_head;
                }
            }
            return Ty_Record(ty_fl_head);
        }
        case A_arrayTy: {
            Ty_ty ty = transTy(tenv, A_NameTy(0, a->u.array));
            dprintf(" type array of %s\n", S_name(a->u.array));
            return Ty_Array(ty);
        }
        default:
            dprintf(" a:%p, %x\n", a, a->kind);
            assert(0);
            return Ty_Void();
    }
}

Ty_fieldList makeTyFieldList(S_table tenv, A_fieldList afl) {
    if (afl == NULL) return NULL;
    A_ty aty = A_NameTy(afl->head->pos, afl->head->typ);
    Ty_field head = Ty_Field(afl->head->name, transTy(tenv, aty));
    return Ty_FieldList(head, makeTyFieldList(tenv, afl->tail));
}

Ty_tyList nametyList(S_table tenv, A_nametyList nl) {
    if (nl == NULL) return NULL;
    Ty_ty ty = transTy(tenv, nl->head->ty);  // for myint, get ty_int
    dprintf(" dec:nametylist:%p, %d\n", nl, ty->kind);

    Ty_ty head;
    if (ty->kind == Ty_int || ty->kind == Ty_string || ty->kind == Ty_name) {
        dprintf(" creating a name type, %s->%d\n", S_name(nl->head->name),
                ty->kind);
        head = Ty_Name(nl->head->name, ty);
    } else {
        head = ty;
    }
    S_enter(tenv, nl->head->name,
            ty);  // now if you S_look arrtype, it'll be a type
    dprintf(" senter:%s = %d\n", S_name(nl->head->name), ty->kind);
    return Ty_TyList(head, nametyList(tenv, nl->tail));
}

void checkTypeDec(S_table tenv, Ty_tyList tl) {
    Ty_ty ty, tmpty;
    S_table dup_check = S_empty();
    for (; tl; tl = tl->tail) {
        ty = tl->head;
        dprintf(" check type.. %d\n", ty->kind);

        if (ty->kind == Ty_record) {
            checkRecord(tenv, ty->u.record);
        }

        if (ty->kind == Ty_name) {
            if (S_look(dup_check, ty->u.name.sym)) {
                error(0, "two types have the same name");
            }
            S_enter(dup_check, ty->u.name.sym, (void *)1);

            S_table cycle_check = S_empty();
            Ty_ty nty = ty;
            while (nty && nty->kind == Ty_name) {
                dprintf(" check cycle.. %s\n", S_name(nty->u.name.sym));
                if (S_look(cycle_check,
                           nty->u.name.sym)) {  // != NULL. entered before
                    error(0, "illegal type cycle");
                    return;
                }
                S_enter(cycle_check, nty->u.name.sym, (void *)1);
                tmpty = S_look(tenv, nty->u.name.sym);

                dprintf(" now jump namety %s to %d\n", S_name(nty->u.name.sym),
                        tmpty->kind);
                nty = tmpty;
                dprintf(" nty%p nty->kind == Ty_name?:%d\n", nty,
                        (nty != NULL) ? (nty->kind == Ty_name) : 0);
            }

            if (nty == NULL) {
                error(0, "%%s contains undeclared type" /*, S_name(s)*/);
            } else {
                ty->u.name.ty = nty;
                S_enter(tenv, ty->u.name.sym, nty);
            }
        }
    }
    dprintf(" end of check type\n");

    return;
}

void checkRecord(S_table tenv, Ty_fieldList fl) {
    if (fl == NULL) return;
    dprintf("!!!!!!\n\n\n\n");
    Ty_ty ty = fl->head->ty, tmpty;
    S_symbol s;

    if (ty->kind == Ty_array) {
        dprintf("!!!!!!%d\n", ty->u.array->kind);
    }

    S_table cycle_check = S_empty();
    while (ty && ty->kind == Ty_name) {
        if (S_look(cycle_check, ty->u.name.sym)) {  // != NULL. entered before
            error(0, "illegal type cycle");
            return;
        }
        s = ty->u.name.sym;
        S_enter(cycle_check, s, (void *)1);
        tmpty = S_look(tenv, s);
        ty->u.name.ty = tmpty;
        ty = tmpty;
    }

    if (ty) {
        checkRecord(tenv, fl->tail);  // move to next
        return;
    } else {
        error(0, " undefined type %s", S_name(s));
    }
    return;
}
