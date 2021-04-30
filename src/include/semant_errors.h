#ifndef SEMANT_ERRORS_H
#define SEMANT_ERRORS_H

typedef char* string;

string SEM_ERR_NOT_DEFINED = "%s \"%s\" is not defined";
string SEM_ERR_ALREADY_DEFINED = "%s \"%s\" is already defined in this scope";

string SEM_ERR_FN_TOO_FEW_ARGS = "too few arguments to function call \"%s\"";
string SEM_ERR_FN_ARG_WRONG_TYPE =
    "argument %d of function call doesn't match declared parameter of function "
    "\"%s\"";
string SEM_ERR_FN_TOO_MANY_ARGS = "too many arguments to function call \"%s\"";

string SEM_ERR_OP_REQ_TYPE =
    "only integers or floats can be used in %s operations";
string SEM_ERR_OP_DIFF_TYPE = "cannot operate with different types";

string SEM_ERR_NOT_RECORD = "\"%s\" is not a record type";

string SEM_ERR_UNDEFINED_FIELD = "\"%s\" is not defined in \"%s\" record";
string SEM_ERR_FIELD_WRONG_TYPE =
    "field \"%s\" argument does not match the type declared in \"%s\" record";

string SEM_ERR_VAR_WRONG_TYPE = "declared type does not match the assign type";

string SEM_ERR_IF_COND = "if conditions must produce an integer";
string SEM_ERR_IF_ELSE_TYPE_DIFF =
    "then and else branches of if statements must produce the same type";
string SEM_ERR_NOT_VOID_ELSELESS_IF =
    "if statements with no else branches must produce no value";

string SEM_ERR_WHILE_COND = "while loop conditions must produce an integer";
string SEM_ERR_WHILE_NOT_VOID = "while loops must produce no value";

string SEM_ERR_ARRAY_WRONG_TYPE =
    "declared type of array does not match the initializer";
string SEM_ERR_ARRAY_LEN_WRONG_TYPE =
    "the array length type must be an integer";

string SEM_ERR_UNDEFINED_FIELDVAR = "\"%s\" is not defined in the record";
string SEM_ERR_NOT_A_RECORD = "cannot access \"%s\": the type is not a record";

string SEM_ERR_INDEX_WRONG_TYPE = "array indexing must be an integer";
string SEM_ERR_NOT_AN_ARRAY = "cannot index variable: not an array";

string SEM_ERR_CANNOT_USE_NIL =
    "cannot initialize with \"nil\" with no type specification";

string SEM_ERR_WRONG_RET_TYPE =
    "resultant body type in function \"%s\" does not match the declared "
    "function return type";

string SEM_ERR_BREAK_OUTSIDE_LOOP =
    "break expressions are not allowed outside a loop";

#endif