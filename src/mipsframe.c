#include "include/assem.h"
#include "include/frame.h"
#include "include/util.h"

#define F_MAX_REGS 32

const int F_K = 6;
const int F_WORD_SIZE = 4;

struct F_frame_ {
    Temp_label name;
    F_accessList formals;
    unsigned int locals;
};

struct F_access_ {
    enum { inFrame, inReg } kind;
    union {
        int offset;     // in frame
        Temp_temp reg;  // in register
    } u;
};

static Temp_temp regs[F_MAX_REGS];
static void F_regs_init();
static Temp_tempList F_spec_regs(int r[], int n);

static void F_regs_init() {
    if (regs[0] == NULL) {
        for (int i = 0; i < F_MAX_REGS; i++) {
            regs[i] = Temp_newtemp();
        }
    }
}

static Temp_tempList F_spec_regs(int r[], int n) {
    assert(n < F_MAX_REGS);
    Temp_tempList list = NULL, last = NULL;
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            list = Temp_TempList(regs[r[i]], NULL);
            last = list;
        } else {
            last->tail = Temp_TempList(regs[r[i]], NULL);
            last = last->tail;
        }
    }
    return list;
}

Temp_map F_tempMap() {
    static Temp_map F_tempMap = NULL;

    if (regs[0] == NULL) {
        F_regs_init();
    }

    if (F_tempMap == NULL) {
        F_tempMap = Temp_empty();
        Temp_enter(F_tempMap, regs[0], "$zero");
        Temp_enter(F_tempMap, regs[1], "$at");
        Temp_enter(F_tempMap, regs[2], "$v0");
        Temp_enter(F_tempMap, regs[3], "$v1");
        Temp_enter(F_tempMap, regs[4], "$a0");
        Temp_enter(F_tempMap, regs[5], "$a1");
        Temp_enter(F_tempMap, regs[6], "$a2");
        Temp_enter(F_tempMap, regs[7], "$a3");
        Temp_enter(F_tempMap, regs[8], "$t0");
        Temp_enter(F_tempMap, regs[9], "$t1");
        Temp_enter(F_tempMap, regs[10], "$t2");
        Temp_enter(F_tempMap, regs[11], "$t3");
        Temp_enter(F_tempMap, regs[12], "$t4");
        Temp_enter(F_tempMap, regs[13], "$t5");
        Temp_enter(F_tempMap, regs[14], "$t6");
        Temp_enter(F_tempMap, regs[15], "$t7");
        Temp_enter(F_tempMap, regs[16], "$s0");
        Temp_enter(F_tempMap, regs[17], "$s1");
        Temp_enter(F_tempMap, regs[18], "$s2");
        Temp_enter(F_tempMap, regs[19], "$s3");
        Temp_enter(F_tempMap, regs[20], "$s4");
        Temp_enter(F_tempMap, regs[21], "$s5");
        Temp_enter(F_tempMap, regs[22], "$s6");
        Temp_enter(F_tempMap, regs[23], "$s7");
        Temp_enter(F_tempMap, regs[24], "$t8");
        Temp_enter(F_tempMap, regs[25], "$t9");
        Temp_enter(F_tempMap, regs[26], "$k0");
        Temp_enter(F_tempMap, regs[27], "$k1");
        Temp_enter(F_tempMap, regs[28], "$gp");
        Temp_enter(F_tempMap, regs[29], "$sp");
        Temp_enter(F_tempMap, regs[30], "$fp");
        Temp_enter(F_tempMap, regs[31], "$ra");
        F_tempMap = Temp_layerMap(F_tempMap, Temp_name());
    }
    return F_tempMap;
}

static F_access InFrame(int offset);  // memory location at offset X from the fp
static F_access InReg(Temp_temp reg);  // held in reg X
static F_accessList F_AccessList(F_access head, F_accessList tail);

F_frame F_newFrame(Temp_label name, U_boolList formals);
Temp_label F_name(F_frame f);
F_accessList F_formals(F_frame f);
F_access F_allocLocal(F_frame f, bool escape);

F_frame F_newFrame(Temp_label name, U_boolList formals) {
    F_frame f = checked_malloc(sizeof(*f));

    f->name = name;
    f->formals = NULL;
    f->locals = 0;

    U_boolList b;
    int offset = -F_WORD_SIZE;  // zero reserved for return address
    // assume all the formals are in frame
    for (b = formals; b; b = b->tail) {
        f->formals = F_AccessList(InFrame(offset), f->formals);
        offset -= F_WORD_SIZE;
        f->locals++;
    }
    return f;
}

Temp_label F_name(F_frame f) { return f->name; }

F_accessList F_formals(F_frame f) { return f->formals; }

F_access F_allocLocal(F_frame f, bool escape) {
    assert(f && (escape == FALSE || escape == TRUE));
    f->locals++;
    return escape ? InFrame(-f->locals * F_WORD_SIZE) : InReg(Temp_newtemp());
    // return InFrame(-(f->locals * F_WORD_SIZE));
}

static F_accessList F_AccessList(F_access head, F_accessList tail) {
    F_accessList al = checked_malloc(sizeof(*al));
    al->head = head;
    al->tail = tail;
    return al;
}

// indicates a memory location at offset x from the frame pointer
static F_access InFrame(int offset) {
    F_access a = checked_malloc(sizeof(*a));
    a->kind = inFrame;
    a->u.offset = offset;
    return a;
}

// indicates it will be held in register t
static F_access InReg(Temp_temp reg) {
    F_access a = checked_malloc(sizeof(*a));
    a->kind = inReg;
    a->u.reg = reg;
    return a;
}

T_exp F_Exp(F_access acc, T_exp framePtr) {
    if (acc->kind == inFrame) {
        return T_Mem(T_Binop(T_plus, framePtr, T_Const(acc->u.offset)));
    } else {
        return T_Temp(acc->u.reg);
    }
}

T_stm F_procEntryExit1(F_frame frame, T_stm stm) { return stm; }

T_exp F_externalCall(string s, T_expList args) {
    return T_Call(T_Name(Temp_namedlabel(s)), args);
}

// fragment implement
F_frag F_StringFrag(Temp_label label, string str) {
    F_frag f = checked_malloc(sizeof(*f));
    f->kind = F_stringFrag;
    f->u.stringg.label = label;
    f->u.stringg.str = str;
    return f;
}

F_frag F_ProcFrag(T_stm body, F_frame frame) {
    F_frag f = checked_malloc(sizeof(*f));
    f->kind = F_procFrag;
    f->u.proc.body = body;
    f->u.proc.frame = frame;
    return f;
}

F_fragList F_FragList(F_frag head, F_fragList tail) {
    F_fragList fl = checked_malloc(sizeof(*fl));
    fl->head = head;
    fl->tail = tail;
    return fl;
}

Temp_temp F_ZERO() {
    if (regs[0] == NULL) {
        F_regs_init();
    }
    return regs[0];
}

Temp_temp F_RV() {
    if (regs[2] == NULL) {
        F_regs_init();
    }
    return regs[2];
}

Temp_temp F_SP() {
    if (regs[29] == NULL) {
        F_regs_init();
    }
    return regs[29];
}

Temp_temp F_FP() {
    if (regs[30] == NULL) {
        F_regs_init();
    }
    return regs[30];
}

Temp_temp F_RA() {
    if (regs[31] == NULL) {
        F_regs_init();
    }
    return regs[31];
}

// $s0-$s7
Temp_tempList F_CalleeSaves() {
    static Temp_tempList list = NULL;
    if (list == NULL) {
        int index[] = {16, 17, 18, 19, 20, 21, 22, 23};
        list = F_spec_regs(index, sizeof(index) / sizeof(index[0]));
    }
    return list;
}

// $t0-$t9
Temp_tempList F_CallerSaves() {
    static Temp_tempList list = NULL;
    if (list == NULL) {
        int index[] = {8, 9, 10, 11, 12, 13, 14, 15, 24, 25};
        list = F_spec_regs(index, sizeof(index) / sizeof(index[0]));
    }
    return list;
}

// $a0-$a3
Temp_tempList F_ArgsRegs() {
    static Temp_tempList list = NULL;
    if (list == NULL) {
        int index[] = {4, 5, 6, 7};
        list = F_spec_regs(index, sizeof(index) / sizeof(index[0]));
    }
    return list;
}

// registers that are to be STORE in stack, would be trashed by callee.
Temp_tempList F_CallSaves() {
    static Temp_tempList list = NULL;
    if (list == NULL) {
        int index[] = {2,  3,  4,  5,  6,  7,  8,  9, 10,
                       11, 12, 13, 14, 15, 24, 25, 31};
        list = F_spec_regs(index, sizeof(index) / sizeof(index[0]));
    }
    return list;
}

AS_instrList F_procEntryExit2(AS_instrList body) {
    static Temp_tempList returnSink = NULL;
    if (!returnSink)
        returnSink = Temp_TempList(
            F_ZERO(),
            Temp_TempList(F_RA(), Temp_TempList(F_SP(), F_CalleeSaves())));
    return AS_splice(body,
                     AS_InstrList(AS_Oper("", NULL, returnSink, NULL), NULL));
}

AS_proc F_procEntryExit3(F_frame frame, AS_instrList body) {
    char buf[100];
    sprintf(buf, "PROCEDURE %s\n", S_name(frame->name));
    return AS_Proc(String(buf), body, "END\n");
}