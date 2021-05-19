#include "include/semant.h"

#include <stdio.h>
#include <stdlib.h>

#include "include/env.h"
#include "include/error.h"
#include "include/escape.h"
#include "include/frame.h"
#include "include/semant_errors.h"
#include "include/temp.h"
#include "include/translate.h"
#include "include/types.h"
#include "include/util.h"

static Ty_ty SEM_var(S_table venv, S_table tenv, A_var v);
static Ty_ty SEM_exp(S_table venv, S_table tenv, A_exp e);
static void SEM_dec(S_table venv, S_table tenv, A_dec d);
static Ty_ty SEM_Ty(S_table tenv, A_ty t);
void checkTypeDec(S_table tenv, Ty_tyList tl);
Ty_tyList nametyList(S_table tenv, A_nametyList nl);
void checkRecord(S_table tenv, Ty_fieldList fl);
void hoist_type_names(S_table tenv, A_dec dec);

static int inside = 0;
static Tr_exp brk[16];

int SEM_transProg(A_exp exp) {
    Esc_findEscape(exp);

    S_table tenv = E_base_tenv();
    S_table venv = E_base_venv();

    SEM_exp(venv, tenv, exp);

    return anyErrors();
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

static Ty_ty SEM_exp(S_table venv, S_table tenv, A_exp a) {
    switch (a->kind) {
        case A_varExp: {
            return SEM_var(venv, tenv, a->u.var);
        }
        case A_nilExp: {
            return Ty_Nil();
        }
        case A_intExp: {
            return Ty_Int();
        }
        case A_floatExp: {
            return Ty_Float();
        }
        case A_stringExp: {
            return Ty_String();
        }
        case A_callExp: {
            E_enventry et = S_look(venv, a->u.call.func);

            if (et == NULL || et->kind != E_funEntry) {
                error(a->pos, SEM_ERR_NOT_DEFINED, "function",
                      S_name(a->u.call.func));

                return Ty_Nil();
            }

            int count = 0;
            A_expList el = a->u.call.args;
            Ty_tyList formals = et->u.fun.formals;
            Tr_expList tr_el = NULL;
            while (formals != NULL) {
                if (el == NULL) {
                    error(a->pos, SEM_ERR_FN_TOO_FEW_ARGS,
                          S_name(a->u.call.func));
                    return Ty_Nil();
                } else {
                    ++count;
                    Ty_ty ty = SEM_exp(venv, tenv, el->head);
                    if (formals->head != ty)
                        error(a->pos, SEM_ERR_FN_ARG_WRONG_TYPE, count,
                              S_name(a->u.call.func));
                }

                formals = formals->tail;
                el = el->tail;
            }

            if (el != NULL) {
                error(a->pos, SEM_ERR_FN_TOO_MANY_ARGS, S_name(a->u.call.func));
                return et->u.fun.result;
            }

            return actual_ty(et->u.fun.result);
        }
        case A_opExp: {
            A_oper oper = a->u.op.oper;
            Ty_ty left = SEM_exp(venv, tenv, a->u.op.left);
            Ty_ty right = SEM_exp(venv, tenv, a->u.op.right);
            Tr_exp translation = Tr_noExp();

            switch (oper) {
                case A_plusOp:
                case A_minusOp:
                case A_timesOp:
                case A_divideOp:
                    if (left->kind != Ty_int && left->kind != Ty_float)
                        error(a->u.op.left->pos, SEM_ERR_OP_REQ_TYPE, "binary");
                    if (right->kind != Ty_int && right->kind != Ty_float)
                        error(a->u.op.right->pos, SEM_ERR_OP_REQ_TYPE,
                              "binary");
                    if (left->kind != right->kind) {
                        error(a->u.op.left->pos, SEM_ERR_OP_DIFF_TYPE);
                    }

                    if (right->kind == Ty_int)
                        return Ty_Int();
                    else
                        return Ty_Float();

                case A_eqOp:
                case A_neqOp:
                    switch (left->kind) {
                        case Ty_string:
                        case Ty_float:
                        case Ty_int: {
                            if (right->kind != left->kind)
                                error(a->u.op.left->pos, SEM_ERR_OP_DIFF_TYPE);
                            return Ty_Nil();
                            break;
                        }
                        case Ty_array: {
                            if (right->kind != left->kind) {
                                error(a->u.op.right->pos, SEM_ERR_NOT_RECORD,
                                      "");
                                return Ty_Nil();
                            }
                            break;
                        }
                        case Ty_record: {
                            if (right->kind != left->kind) {
                                error(a->u.op.right->pos, SEM_ERR_NOT_RECORD,
                                      "");
                                return Ty_Nil();
                            }
                            break;
                        }
                        default: {
                            error(a->u.op.right->pos, SEM_ERR_OP_REQ_TYPE,
                                  "comparison");
                            return Ty_Nil();
                        }
                    }
                    return Ty_Int();

                case A_gtOp:
                case A_ltOp:
                case A_leOp:
                case A_geOp: {
                    if (right->kind != left->kind) {
                        error(a->u.op.right->pos, SEM_ERR_OP_REQ_TYPE,
                              "comparison");
                    }
                    switch (left->kind) {
                        case Ty_float:
                        case Ty_int:
                            break;
                        case Ty_string:
                            break;
                        default: {
                            error(a->u.op.right->pos, SEM_ERR_OP_REQ_TYPE);
                        }
                    }
                    return Ty_Int();
                }
            }
        }
        case A_recordExp: {
            Ty_ty recordty = S_look(tenv, a->u.record.typ);
            if (NULL == recordty) {
                error(a->pos, SEM_ERR_NOT_DEFINED, "type",
                      S_name(a->u.record.typ));
                return Ty_Record(NULL);
            }

            if (recordty->kind != Ty_record) {
                error(a->pos, SEM_ERR_NOT_RECORD, S_name(a->u.record.typ));
                return Ty_Record(NULL);
            }

            Ty_fieldList ty_fl;
            A_efieldList fl;
            Tr_expList tr_el = NULL;
            int n_fields = 0;
            for (fl = a->u.record.fields, ty_fl = recordty->u.record;
                 fl && ty_fl; fl = fl->tail, ty_fl = ty_fl->tail, n_fields++) {
                if (fl->head->name != ty_fl->head->name) {
                    error(a->pos, SEM_ERR_UNDEFINED_FIELD,
                          S_name(fl->head->name), S_name(a->u.record.typ));
                    return Ty_Record(NULL);
                }
                Ty_ty ty = SEM_exp(venv, tenv, fl->head->exp);
                if (!actual_eq(ty, ty_fl->head->ty)) {
                    error(a->pos, SEM_ERR_FIELD_WRONG_TYPE,
                          S_name(fl->head->name), S_name(a->u.record.typ));
                    return Ty_Record(NULL);
                }
            }

            return recordty;
        }
        case A_seqExp: {
            Ty_ty ty = Ty_Void();
            A_expList el;

            for (el = a->u.seq; el; el = el->tail) {
                ty = SEM_exp(venv, tenv, el->head);
            }

            return ty;
        }
        case A_assignExp: {
            Ty_ty ty_var = SEM_var(venv, tenv, a->u.assign.var);
            Ty_ty ty_exp = SEM_exp(venv, tenv, a->u.assign.exp);
            if (ty_var != ty_exp)
                if (ty_exp != Ty_Nil()) error(a->pos, SEM_ERR_VAR_WRONG_TYPE);

            return Ty_Void();
        }
        case A_ifExp: {
            Ty_ty ty_cond = SEM_exp(venv, tenv, a->u.iff.test);
            if (ty_cond != Ty_Int()) {
                error(a->pos, SEM_ERR_IF_COND);
                return Ty_Void();
            }
            Ty_ty ty_thenet = SEM_exp(venv, tenv, a->u.iff.then);
            if (a->u.iff.elsee) {
                Ty_ty ty_elseet = SEM_exp(venv, tenv, a->u.iff.elsee);
                if (ty_elseet != ty_thenet) {
                    if (!((ty_elseet == Ty_Nil() || ty_elseet == Ty_Void()) ||
                          (ty_thenet == Ty_Nil() || ty_thenet == Ty_Void())))
                        error(a->pos, SEM_ERR_IF_ELSE_TYPE_DIFF);
                }

                return ty_thenet;
            } else if (ty_thenet != Ty_Void()) {
                error(a->pos, SEM_ERR_NOT_VOID_ELSELESS_IF);
                return Ty_Void();
            }

            return ty_thenet;
        }
        case A_whileExp: {
            Ty_ty ty_test = SEM_exp(venv, tenv, a->u.whilee.test), ty_body;
            if (ty_test != Ty_Int()) error(a->pos, SEM_ERR_WHILE_COND);

            inside++;  // inside loop
            Tr_exp done = Tr_doneExp();
            brk[inside] = done;
            ty_body = SEM_exp(venv, tenv, a->u.whilee.body);
            inside--;  // outside

            if (ty_body != Ty_Void() && ty_body != Ty_Nil()) {
                error(a->pos, SEM_ERR_WHILE_NOT_VOID);
            }

            return Ty_Void();
        }
        case A_forExp: {
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
            return SEM_exp(venv, tenv, let_exp);
        }
        case A_breakExp: {
            if (!inside) {
                error(a->pos, SEM_ERR_BREAK_OUTSIDE_LOOP);
                return Ty_Void();
            }
            return Ty_Void();
        }
        case A_letExp: {
            A_decList d;
            Tr_expList head = NULL;

            S_beginScope(venv);
            S_beginScope(tenv);

            SEM_dec(venv, tenv, d->head);
            Ty_ty ty_et = SEM_exp(venv, tenv, a->u.let.body);

            S_endScope(tenv);
            S_endScope(venv);

            return ty_et;
        }
        case A_arrayExp: {
            Ty_ty ty_init, ty_size, typ = S_look(tenv, a->u.array.typ);

            if (!typ) {
                error(a->pos, SEM_ERR_NOT_DEFINED, "type",
                      S_name(a->u.array.typ));
                return Ty_Array(NULL);
            }

            ty_init = SEM_exp(venv, tenv, a->u.array.init);

            if (typ->u.array->kind == Ty_name)
                typ->u.array = S_look(tenv, typ->u.array->u.name.sym);
            if (!actual_eq(ty_init, typ->u.array)) {
                error(a->pos, SEM_ERR_ARRAY_WRONG_TYPE);
                return Ty_Array(typ);
            }

            ty_size = SEM_exp(venv, tenv, a->u.array.size);

            if (ty_size != Ty_Int()) {
                error(a->pos, SEM_ERR_ARRAY_LEN_WRONG_TYPE);
                return Ty_Array(typ);
            }

            return typ;
        }
        default:;
    }

    assert(0);
}

static Ty_ty SEM_var(S_table venv, S_table tenv, A_var v) {
    switch (v->kind) {
        case A_simpleVar: {
            E_enventry x = S_look(venv, v->u.simple);

            if (x && x->kind == E_varEntry) {
                return actual_ty(x->u.var.ty);
            } else {
                error(v->pos, SEM_ERR_NOT_DEFINED, "variable",
                      S_name(v->u.simple));
                return Ty_Int();
            }
        }
        case A_fieldVar: {
            Ty_ty ty_et = SEM_var(venv, tenv, v->u.field.var);

            if (ty_et->kind == Ty_record) {
                Ty_fieldList fl;
                int offset = 0;

                for (fl = ty_et->u.record; fl; fl = fl->tail, offset++) {
                    if (fl->head->name == v->u.field.sym) {
                        return actual_ty(fl->head->ty);
                    }
                }

                error(v->u.field.var->pos, SEM_ERR_UNDEFINED_FIELDVAR,
                      S_name(v->u.field.sym));

            } else {
                error(v->u.field.var->pos, SEM_ERR_NOT_A_RECORD,
                      S_name(v->u.field.sym));
            }

            return Ty_Int();
        }
        case A_subscriptVar: {
            Ty_ty ty_et = SEM_var(venv, tenv, v->u.subscript.var);
            Ty_ty ty_etint = SEM_exp(venv, tenv, v->u.subscript.exp);

            if (ty_etint != Ty_Int()) {
                error(v->u.subscript.var->pos, SEM_ERR_INDEX_WRONG_TYPE);
                return Ty_Int();
            }

            if (actual_ty(ty_et)->kind == Ty_array) {
                Ty_ty arr = ty_et->u.array;
                return actual_ty(arr);
            } else {
                error(v->u.subscript.var->pos, SEM_ERR_NOT_AN_ARRAY);
                return Ty_Int();
            }
        }
        defalut:;
    }

    assert(0);
}

static void SEM_dec(S_table venv, S_table tenv, A_dec d) {
    switch (d->kind) {
        case A_varDec: {
            Ty_ty dec_ty = NULL;
            if (d->u.var.typ != NULL) {
                dec_ty = S_look(tenv, d->u.var.typ);
                if (!dec_ty) {
                    error(d->pos, SEM_ERR_NOT_DEFINED, "type",
                          S_name(d->u.var.typ));
                    return;
                }
            }

            Ty_ty ty_e = SEM_exp(venv, tenv, d->u.var.init);

            Ty_ty ty;
            if (d->u.var.typ == NULL) {
                if (ty_e == Ty_Nil()) error(d->pos, SEM_ERR_CANNOT_USE_NIL);
            } else if ((ty = S_look(tenv, d->u.var.typ)) != ty_e) {
                if (ty_e != Ty_Nil()) error(d->pos, SEM_ERR_VAR_WRONG_TYPE);
            }

            E_enventry eentry = E_VarEntry(NULL, ty_e);

            if (d->u.var.typ != NULL) {
                S_enter(venv, d->u.var.var, E_VarEntry(NULL, dec_ty));
            } else {
                S_enter(venv, d->u.var.var, eentry);
            }

            return;
        }
        case A_typeDec: {
            hoist_type_names(tenv, d);
            Ty_tyList tl = nametyList(tenv, d->u.type);
            checkTypeDec(tenv, tl);
            return;
        }
        case A_functionDec: {
            A_fundecList fun_list;

            int index = 0;
            void *typenames[10];

            for (fun_list = d->u.function; fun_list;
                 fun_list = fun_list->tail) {
                A_fieldList fl;
                Ty_ty ty;

                Ty_tyList head = NULL, tail = NULL;
                Ty_ty r;

                U_boolList m_head = NULL, m_tail = NULL;

                if (fun_list->head->result) {
                    r = S_look(tenv, fun_list->head->result);
                    if (!r) {
                        error(fun_list->head->pos, SEM_ERR_NOT_DEFINED,
                              "return type", S_name(fun_list->head->result));
                        return;
                    }
                } else {
                    r = Ty_Void();
                }

                for (fl = fun_list->head->params; fl; fl = fl->tail) {
                    ty = S_look(tenv, fl->head->typ);
                    if (!ty) {
                        error(fl->head->pos, SEM_ERR_NOT_DEFINED, "parameter",
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
                    if (m_head) {
                        m_tail->tail = U_BoolList(TRUE, NULL);
                        m_tail = m_tail->tail;
                    } else {
                        m_head = U_BoolList(TRUE, NULL);
                        m_tail = m_head;
                    }
                }

                for (int i = 0; i < index; i++) {
                    if (typenames[i] == (void *)fun_list->head->name) {
                        error(fun_list->head->pos, SEM_ERR_ALREADY_DEFINED,
                              "function", S_name(fun_list->head->name));
                        return;
                    }
                }
                typenames[index++] = (void *)fun_list->head->name;
            }

            for (fun_list = d->u.function; fun_list;
                 fun_list = fun_list->tail) {
                E_enventry fun_entry = S_look(venv, fun_list->head->name);

                S_beginScope(venv);

                A_fieldList fl;
                Ty_tyList tl = fun_entry->u.fun.formals;

                for (fl = fun_list->head->params; fl;
                     fl = fl->tail, tl = tl->tail) {
                    S_enter(venv, fl->head->name, E_VarEntry(NULL, tl->head));
                }

                Ty_ty ty_exp = SEM_exp(venv, tenv, fun_list->head->body);
                if (!actual_eq(fun_entry->u.fun.result, ty_exp)) {
                    error(d->pos, SEM_ERR_WRONG_RET_TYPE,
                          S_name(fun_list->head->name));
                    S_endScope(venv);
                    break;
                }

                S_endScope(venv);
            }
            return;
        }
    }
}

static Ty_ty SEM_Ty(S_table tenv, A_ty a) {
    if (a == NULL) {
        return Ty_Void();
    }

    switch (a->kind) {
        case A_nameTy: {
            Ty_ty ty = S_look(tenv, a->u.name);
            if (!ty) {
                error(a->pos, SEM_ERR_NOT_DEFINED, "type", S_name(a->u.name));
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
                    error(fl->head->pos, SEM_ERR_NOT_DEFINED, "type",
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
            Ty_ty ty = SEM_Ty(tenv, A_NameTy(0, a->u.array));
            return Ty_Array(ty);
        }
        default:
            assert(0);
    }
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
                error(type_list->head->ty->pos, SEM_ERR_ALREADY_DEFINED, "type",
                      S_name(type_list->head->name));
            }
        }

        typenames[index++] = (void *)type_list->head->name;
    }
}

Ty_tyList nametyList(S_table tenv, A_nametyList nl) {
    if (nl == NULL) return NULL;
    Ty_ty ty = SEM_Ty(tenv, nl->head->ty);

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
                error(0, SEM_ERR_ALREADY_DEFINED, "type",
                      S_name(ty->u.name.sym));
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
        error(0, SEM_ERR_NOT_DEFINED, "type", S_name(s));
    }
}
