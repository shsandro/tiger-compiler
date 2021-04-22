#include "include/semant.h"

#include <stdio.h>
#include <stdlib.h>

#include "include/env.h"
#include "include/error.h"
#include "include/escape.h"
#include "include/frame.h"
#include "include/temp.h"
#include "include/translate.h"
#include "include/types.h"
#include "include/util.h"

// typedef void *Tr_exp;

typedef struct expty_ {
    Tr_exp exp;
    Ty_ty ty;
} expty;

static expty expTy(Tr_exp exp, Ty_ty ty) {
    expty e;

    e.exp = exp;
    e.ty = ty;

    return e;
}

static expty transVar(Tr_level level, S_table venv, S_table tenv, A_var v);
static expty transExp(Tr_level level, S_table venv, S_table tenv, A_exp e);
static Tr_exp transDec(Tr_level level, S_table venv, S_table tenv, A_dec d);
static Ty_ty transTy(S_table tenv, A_ty t);
void checkTypeDec(S_table tenv, Ty_tyList tl);
Ty_tyList nametyList(S_table tenv, A_nametyList nl);
void checkRecord(S_table tenv, Ty_fieldList fl);
void hoist_type_names(S_table tenv, A_dec dec);

// inside flag (for loop, while loop)
static int inside = 0;
static Tr_exp brk[16];

int SEM_transProg(A_exp exp, int print_ir) {
    Esc_findEscape(exp);

    S_table tenv = E_base_tenv();
    S_table venv = E_base_venv();

    expty trans_expr = transExp(Tr_outermost(), venv, tenv, exp);

    int any_errors = anyErrors();

    if (!any_errors && print_ir) Tr_printTree(trans_expr.exp);

    return any_errors;
}

Ty_ty actual_ty(Ty_ty ty) {
    assert(ty);

    Ty_ty tmpty;
    while (ty->kind == Ty_name) {
        tmpty = ty->u.name.ty;

        if (tmpty)
            ty = tmpty;
        else
            return NULL;
    }

    return ty;
}

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
    if (!afl) return NULL;

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

        if (ttl->head == NULL) {
            error(afl->head->pos, " undefined type: %s",
                  S_name(afl->head->typ));
        }

        last = ttl;
        ttl = ttl->tail;
        afl = afl->tail;
    }

    if (last) last->tail = NULL;

    return ttl_hd;
}

static expty transExp(Tr_level level, S_table venv, S_table tenv, A_exp a) {
    switch (a->kind) {
        case A_varExp: {
            return transVar(level, venv, tenv, a->u.var);
        }
        case A_nilExp: {
            return expTy(Tr_nilExp(), Ty_Nil());
        }
        case A_intExp: {
            return expTy(Tr_intExp(a->u.intt), Ty_Int());
        }
        case A_floatExp: {
            return expTy(Tr_floatExp(a->u.floatt), Ty_Float());
        }
        case A_stringExp: {
            return expTy(Tr_stringExp(a->u.stringg), Ty_String());
        }
        case A_callExp: {
            E_enventry et = S_look(venv, a->u.call.func);

            if (et == NULL || et->kind != E_funEntry) {
                error(a->pos, "call expression: undefined type %s",
                      S_name(a->u.call.func));

                return expTy(Tr_noExp(), Ty_Nil());
            }

            A_expList el = a->u.call.args;
            Ty_tyList formals = et->u.fun.formals;
            expty tmp;
            Tr_expList tr_el = NULL;
            while (formals != NULL) {
                if (el == NULL) {
                    error(a->pos, "call expression: too few arguments");
                    return expTy(Tr_noExp(), Ty_Nil());
                } else {
                    tmp = transExp(level, venv, tenv, el->head);
                    if (formals->head != tmp.ty)
                        error(a->pos,
                              "call expression: argument type dosen't match "
                              "the paramater");
                }

                formals = formals->tail;
                el = el->tail;
                tr_el = Tr_ExpList(tmp.exp, tr_el);
            }

            if (el != NULL) {
                error(a->pos, "call expression: too many arguments %s",
                      S_name(a->u.call.func));
                return expTy(Tr_noExp(), et->u.fun.result);
            }

            return expTy(
                Tr_callExp(level, et->u.fun.level, et->u.fun.label, tr_el),
                et->u.fun.result);
        }
        case A_opExp: {
            A_oper oper = a->u.op.oper;
            expty left = transExp(level, venv, tenv, a->u.op.left);
            expty right = transExp(level, venv, tenv, a->u.op.right);
            if (oper == A_plusOp || oper == A_minusOp || oper == A_timesOp ||
                oper == A_divideOp) {
                if (left.ty->kind != Ty_int && left.ty->kind != Ty_float)
                    error(a->u.op.left->pos,
                          "binary operation: integer or float required");

                if (right.ty->kind != Ty_int && right.ty->kind != Ty_float)
                    error(a->u.op.right->pos,
                          "binary operation: integer or float required");

                if (right.ty->kind != right.ty->kind)
                    error(a->u.op.left->pos,
                          "binary operation: same type required");

                Tr_exp tr_exp = Tr_arithExp(oper, left.exp, right.exp);

                if (right.ty->kind == Ty_int)
                    return expTy(tr_exp, Ty_Int());
                else
                    return expTy(tr_exp, Ty_Float());
            } else if (left.ty->kind != right.ty->kind && left.ty != Ty_Nil() &&
                       right.ty != Ty_Nil()) {
                error(a->u.op.left->pos,
                      "operators compare: different type for compare");
                return expTy(Tr_noExp(), Ty_Int());
            } else if (oper != A_eqOp && oper != A_neqOp) {  // lt, le, gt, ge
                if ((left.ty->kind != Ty_int && left.ty->kind != Ty_float) ||
                    (right.ty->kind != Ty_int && right.ty->kind != Ty_float)) {
                    error(a->u.op.left->pos,
                          "binary compare: integer or float required");
                    return expTy(Tr_noExp(), Ty_Int());
                }
            } else if (!(left.ty->kind == Ty_int || left.ty->kind == Ty_array ||
                         left.ty->kind == Ty_record ||
                         left.ty->kind == Ty_string)) {
                if (right.ty != Ty_Nil()) {
                    error(a->u.op.left->pos, "binary compare: can't compare!");
                    return expTy(Tr_noExp(), Ty_Int());
                }
            }

            return expTy(Tr_relExp(oper, left.exp, right.exp), Ty_Int());
        }
        case A_recordExp: {
            Ty_ty recordty = S_look(tenv, a->u.record.typ);
            if (NULL == recordty) {
                error(a->pos, "record expression: undefined type %s",
                      S_name(a->u.record.typ));
                return expTy(Tr_noExp(), Ty_Record(NULL));
            }

            if (recordty->kind != Ty_record) {
                error(a->pos, "record expression: <%s> is not a record type",
                      S_name(a->u.record.typ));
                return expTy(Tr_noExp(), Ty_Record(NULL));
            }

            Ty_fieldList ty_fl;
            A_efieldList fl;
            Tr_expList tr_el = NULL;
            int n_fields = 0;
            for (fl = a->u.record.fields, ty_fl = recordty->u.record;
                 fl && ty_fl; fl = fl->tail, ty_fl = ty_fl->tail, n_fields++) {
                if (fl->head->name != ty_fl->head->name) {
                    error(a->pos,
                          "record expression: <%s> not a valid field name",
                          S_name(fl->head->name));
                    return expTy(Tr_noExp(), Ty_Record(NULL));
                }
                expty exp = transExp(level, venv, tenv, fl->head->exp);
                if (!actual_eq(exp.ty, ty_fl->head->ty)) {
                    error(a->pos,
                          "record expression: both field types dismatch");
                    return expTy(Tr_noExp(), Ty_Record(NULL));
                }

                tr_el = Tr_ExpList(exp.exp, tr_el);
            }

            return expTy(Tr_recordExp(tr_el, n_fields), recordty);
        }
        case A_seqExp: {
            expty exp = expTy(Tr_noExp(), Ty_Void());
            A_expList el;
            Tr_expList tr_el = NULL;

            for (el = a->u.seq; el; el = el->tail) {
                exp = transExp(level, venv, tenv, el->head);
                tr_el = Tr_ExpList(exp.exp, tr_el);
            }

            if (tr_el == NULL) tr_el = Tr_ExpList(exp.exp, tr_el);

            return expTy(Tr_seqExp(tr_el), exp.ty);
        }
        case A_assignExp: {
            expty var = transVar(level, venv, tenv, a->u.assign.var);
            expty exp = transExp(level, venv, tenv, a->u.assign.exp);
            if (var.ty != exp.ty)
                error(a->pos,
                      "assign expression: dismatch type between variable and "
                      "expression <%d> <%d>",
                      var.ty->kind, exp.ty->kind);

            return expTy(Tr_assignExp(var.exp, exp.exp), Ty_Void());
        }
        case A_ifExp: {
            expty cond = transExp(level, venv, tenv, a->u.iff.test);
            if (cond.ty != Ty_Int()) {
                error(a->pos, "condition expression: if test must produce int");
                return expTy(Tr_noExp(), Ty_Void());
            }
            expty thenet = transExp(level, venv, tenv, a->u.iff.then);
            if (a->u.iff.elsee) {  // if-then-else
                expty elseet = transExp(level, venv, tenv, a->u.iff.elsee);
                // if-then: no return value. if-then-else: can have return value
                if (elseet.ty != thenet.ty) {
                    if (!((elseet.ty == Ty_Nil() || elseet.ty == Ty_Void()) ||
                          (thenet.ty == Ty_Nil() || thenet.ty == Ty_Void())))
                        error(a->pos,
                              "condition expression: then-else section must be "
                              "the same type");
                }

                return expTy(Tr_ifExp(cond.exp, thenet.exp, elseet.exp),
                             thenet.ty);
            } else if (thenet.ty != Ty_Void()) {
                error(a->pos,
                      "condition expression: if-then exp's body must produce "
                      "no value");
                return expTy(Tr_noExp(), Ty_Void());
            }

            return expTy(Tr_ifExp(cond.exp, thenet.exp, NULL), thenet.ty);
        }
        case A_whileExp: {
            expty test = transExp(level, venv, tenv, a->u.whilee.test), body;
            if (test.ty != Ty_Int())
                error(a->pos, "while loop: while test must produce int");

            inside++;  // inside loop
            Tr_exp done = Tr_doneExp();
            brk[inside] = done;
            body = transExp(level, venv, tenv, a->u.whilee.body);
            inside--;  // outside

            if (body.ty != Ty_Void() && body.ty != Ty_Nil()) {
                error(a->pos, "while loop: body section must produce no value");
            }

            return expTy(Tr_whileExp(test.exp, done, body.exp), Ty_Void());
        }
        case A_forExp: {
            // expty lo = transExp(level, venv, tenv, a->u.forr.lo);
            // expty hi = transExp(level, venv, tenv, a->u.forr.hi);

            // if (lo.ty->kind != Ty_int) {
            //     error(a->u.forr.lo->pos,
            //           "for loop: lower part must be integer");
            // }

            // if (hi.ty->kind != Ty_int) {
            //     error(a->u.forr.hi->pos,
            //           "for loop: higer part must be integer");
            // }

            // S_beginScope(venv);
            // Tr_access m_access = Tr_allocLocal(level, a->u.forr.escape);
            // S_enter(venv, a->u.forr.var, E_VarEntry(m_access, Ty_Int()));

            // inside++;  // inside loop
            // expty body = transExp(level, venv, tenv, a->u.forr.body);
            // inside--;  // outside

            // if (body.ty->kind != Ty_void) {
            //     error(a->u.forr.body->pos, "for loop: body part must be
            //     void");
            // }

            // S_endScope(venv);

            // return expTy(NULL, Ty_Void());

            A_dec i = A_VarDec(a->pos, a->u.forr.var, NULL, a->u.forr.lo);
            A_dec limit =
                A_VarDec(a->pos, S_Symbol("limit"), NULL, a->u.forr.hi);
            A_decList let_declare = A_DecList(i, A_DecList(limit, NULL));

            A_exp increment_exp = A_AssignExp(
                a->pos, A_SimpleVar(a->pos, a->u.forr.var),
                A_OpExp(a->pos, A_plusOp,
                        A_VarExp(a->pos, A_SimpleVar(a->pos, a->u.forr.var)),
                        A_IntExp(a->pos, 1)));
            A_exp while_test = A_OpExp(
                a->pos, A_leOp,
                A_VarExp(a->pos, A_SimpleVar(a->pos, a->u.forr.var)),
                A_VarExp(a->pos, A_SimpleVar(a->pos, S_Symbol("limit"))));
            A_exp while_body = A_SeqExp(
                a->pos, A_ExpList(a->pos, a->u.forr.body,
                                  A_ExpList(a->pos, increment_exp, NULL)));
            A_exp let_body = A_SeqExp(
                a->pos,
                A_ExpList(a->pos, A_WhileExp(a->pos, while_test, while_body),
                          NULL));
            A_exp let_exp = A_LetExp(a->pos, let_declare, let_body);
            expty exp = transExp(level, venv, tenv, let_exp);
            return exp;
        }
        case A_breakExp: {
            if (!inside) {
                error(a->pos,
                      "break expression: break expression outside loop");
                return expTy(Tr_noExp(), Ty_Void());
            }
            return expTy(Tr_breakExp(brk[inside]), Ty_Void());
        }
        case A_letExp: {
            A_decList d;
            Tr_expList head = NULL;

            S_beginScope(venv);
            S_beginScope(tenv);

            for (d = a->u.let.decs; d; d = d->tail) {
                // append in a reversed order. but don't worry. check return.
                head = Tr_ExpList(transDec(level, venv, tenv, d->head), head);
            }

            expty et = transExp(level, venv, tenv, a->u.let.body);
            head = Tr_ExpList(et.exp, head);

            S_endScope(tenv);
            S_endScope(venv);

            return expTy(Tr_seqExp(head), et.ty);
        }
        case A_arrayExp: {
            expty init, size;
            Ty_ty typ = S_look(tenv, a->u.array.typ);

            if (!typ) {
                error(a->pos, "array expression: undefined type %s",
                      S_name(a->u.array.typ));
                return expTy(Tr_noExp(), Ty_Array(NULL));
            }

            init = transExp(level, venv, tenv, a->u.array.init);

            if (typ->u.array->kind == Ty_name)
                typ->u.array = S_look(tenv, typ->u.array->u.name.sym);
            if (init.ty != typ->u.array) {
                error(a->pos,
                      "array expression: initialize type does not match with "
                      "given type");
                return expTy(Tr_noExp(), Ty_Array(typ));
            }

            size = transExp(level, venv, tenv, a->u.array.size);

            if (size.ty != Ty_Int()) {
                error(a->pos, "array expression: array length must be int.");
                return expTy(Tr_noExp(), Ty_Array(typ));
            }

            return expTy(Tr_arrayExp(size.exp, init.exp), typ);
        }
        default:;
    }

    assert(0);  // not recognized
}

static expty transVar(Tr_level level, S_table venv, S_table tenv, A_var v) {
    switch (v->kind) {
        case A_simpleVar: {
            E_enventry x = S_look(venv, v->u.simple);

            if (x && x->kind == E_varEntry) {
                return expTy(Tr_simpleVar(x->u.var.access, level),
                             actual_ty(x->u.var.ty));
            } else {
                error(v->pos, "simple var expression: undefined variable %s",
                      S_name(v->u.simple));
                return expTy(Tr_noExp(), Ty_Int());
            }
        }
        case A_fieldVar: {
            expty et = transVar(level, venv, tenv, v->u.field.var);

            if (et.ty->kind == Ty_record) {
                Ty_fieldList fl;
                int offset = 0;

                for (fl = et.ty->u.record; fl; fl = fl->tail, offset++) {
                    if (fl->head->name == v->u.field.sym) {
                        return expTy(Tr_fieldVar(et.exp, offset),
                                     actual_ty(fl->head->ty));
                    }
                }

                error(v->u.field.var->pos,
                      "field var expression: no such field <%s> in the record",
                      S_name(v->u.field.sym));

            } else {
                error(v->u.field.var->pos,
                      "field var expression: not a record type variable");
            }

            return expTy(Tr_noExp(), Ty_Int());
        }
        case A_subscriptVar: {
            expty et = transVar(level, venv, tenv, v->u.subscript.var);
            expty etint = transExp(level, venv, tenv, v->u.subscript.exp);

            if (etint.ty != Ty_Int()) {
                error(v->u.subscript.var->pos,
                      "subscript var expression: int required in array "
                      "index");
                return expTy(Tr_noExp(), Ty_Int());
            }

            if (actual_ty(et.ty)->kind == Ty_array) {
                Ty_ty arr = et.ty->u.array;
                return expTy(Tr_subscriptVar(et.exp, etint.exp), arr);
            } else {
                error(v->u.subscript.var->pos,
                      "subscript var expression: not an array type variable");
                return expTy(Tr_noExp(), Ty_Int());
            }
        }
        defalut:;
    }

    assert(0);  // not recognized
}

static Tr_exp transDec(Tr_level level, S_table venv, S_table tenv, A_dec d) {
    switch (d->kind) {
        case A_varDec: {
            Ty_ty dec_ty = NULL;  // declare type maybe NULL
            if (d->u.var.typ != NULL) {
                dec_ty = S_look(tenv, d->u.var.typ);
                if (!dec_ty) {
                    error(d->pos, "variable declare: undefined type %s",
                          S_name(d->u.var.typ));
                    return Tr_noExp();
                }
            }

            expty e = transExp(level, venv, tenv, d->u.var.init);

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

            Tr_access m_access = Tr_allocLocal(level, d->u.var.escape);
            E_enventry eentry = E_VarEntry(m_access, e.ty);

            S_enter(venv, d->u.var.var, eentry);
            return Tr_assignExp(Tr_simpleVar(m_access, level), e.exp);
        }
        case A_typeDec: {
            hoist_type_names(tenv, d);
            Ty_tyList tl = nametyList(tenv, d->u.type);
            checkTypeDec(tenv, tl);
            return Tr_noExp();
        }
        case A_functionDec: {
            A_fundecList fun_list;

            int index = 0;
            void *typenames[10];

            /* example:
            function treeLeaves(t: tree): int =
            treelistLeaves(t.children) 1: add header
            (functiontreeLeaves(t:tree): int =) */
            for (fun_list = d->u.function; fun_list;
                 fun_list = fun_list->tail) {
                // i need elements of E_FunEntry, because its value environment
                A_fieldList fl;
                Ty_ty ty;

                Ty_tyList head = NULL, tail = NULL;
                Ty_ty r;

                // boolList of parameters, indicates vars escape or not
                U_boolList m_head = NULL, m_tail = NULL;

                // return type
                if (fun_list->head->result) {
                    r = S_look(tenv, fun_list->head->result);
                    if (!r) {
                        error(fun_list->head->pos,
                              "function declare: undefined return type %s",
                              S_name(fun_list->head->result));
                        break;
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
                        break;
                    }

                    if (head) {
                        tail->tail = Ty_TyList(ty, NULL);
                        tail = tail->tail;
                    } else {
                        head = Ty_TyList(ty, NULL);
                        tail = head;
                    }

                    if (m_head) {
                        m_tail->tail = U_BoolList(TRUE, NULL);
                        m_tail = m_tail->tail;
                    } else {
                        m_head = U_BoolList(TRUE, NULL);
                        m_tail = m_head;
                    }
                }

                Temp_label m_label = Temp_newlabel();
                Tr_level m_level = Tr_newLevel(level, m_label, m_head);
                S_enter(venv, fun_list->head->name,
                        E_FunEntry(m_level, m_label, head, r));

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

                Tr_accessList m_accessList = Tr_formals(fun_entry->u.fun.level);
                for (fl = fun_list->head->params; fl; fl = fl->tail,
                    tl = tl->tail, m_accessList = m_accessList->tail) {
                    S_enter(venv, fl->head->name,
                            E_VarEntry(m_accessList->head, tl->head));
                }

                // translate body
                expty exp = transExp(level, venv, tenv, fun_list->head->body);
                // compare return type and body type
                if (!actual_eq(fun_entry->u.fun.result, exp.ty)) {
                    error(
                        d->pos,
                        "function declare: body type and return type with <%s>",
                        S_name(fun_list->head->name));
                    S_endScope(venv);
                    break;
                }

                S_endScope(venv);
            }
            return Tr_noExp();
        }
    }
}

static Ty_ty transTy(S_table tenv, A_ty a) {
    // assert(a != NULL); // might cause trouble in recursive dec!
    if (a == NULL) {
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

            return ty;
        }
        case A_recordTy: {
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
            return Ty_Array(ty);
        }
        default:
            assert(0);
    }
}

Ty_fieldList makeTyFieldList(S_table tenv, A_fieldList afl) {
    if (afl == NULL) return NULL;

    A_ty aty = A_NameTy(afl->head->pos, afl->head->typ);
    Ty_field head = Ty_Field(afl->head->name, transTy(tenv, aty));

    return Ty_FieldList(head, makeTyFieldList(tenv, afl->tail));
}

void hoist_type_names(S_table tenv, A_dec dec) {
    A_nametyList type_list;
    void *typenames[10];
    int index = 0;

    for (type_list = dec->u.type; type_list; type_list = type_list->tail) {
        S_enter(tenv, type_list->head->name,
                Ty_Name(type_list->head->name, NULL));

        for (int i = 0; i < index; i++) {
            if (typenames[i] == (void *)type_list->head->name) {
                error(type_list->head->ty->pos,
                      "type declare: redeclaration type <%s>",
                      S_name(type_list->head->name));
            }
        }

        typenames[index++] = (void *)type_list->head->name;
    }
}

Ty_tyList nametyList(S_table tenv, A_nametyList nl) {
    if (nl == NULL) return NULL;
    Ty_ty ty = transTy(tenv, nl->head->ty);

    Ty_ty head;
    if (ty->kind == Ty_int || ty->kind == Ty_string || ty->kind == Ty_float ||
        ty->kind == Ty_name) {
        head = Ty_Name(nl->head->name, ty);
    } else {
        head = ty;
    }

    S_enter(tenv, nl->head->name, ty);

    return Ty_TyList(head, nametyList(tenv, nl->tail));
}

void checkTypeDec(S_table tenv, Ty_tyList tl) {
    Ty_ty ty, tmpty;
    S_table dup_check = S_empty();
    for (; tl; tl = tl->tail) {
        ty = tl->head;

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
                if (S_look(cycle_check, nty->u.name.sym)) {
                    error(0, "illegal type cycle");
                    return;
                }

                S_enter(cycle_check, nty->u.name.sym, (void *)1);
                tmpty = S_look(tenv, nty->u.name.sym);

                nty = tmpty;
            }

            if (nty == NULL) {
                error(0, "contains undeclared type");
            } else {
                ty->u.name.ty = nty;
                S_enter(tenv, ty->u.name.sym, nty);
            }
        }
    }

    return;
}

void checkRecord(S_table tenv, Ty_fieldList fl) {
    if (fl == NULL) return;
    Ty_ty ty = fl->head->ty, tmpty;
    S_symbol s;

    S_table cycle_check = S_empty();
    while (ty && ty->kind == Ty_name) {
        if (S_look(cycle_check, ty->u.name.sym)) {
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
        checkRecord(tenv, fl->tail);
    } else {
        error(0, " undefined type %s", S_name(s));
    }
}
