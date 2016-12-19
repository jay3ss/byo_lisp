// Parsing library
#include "mpc.h"
#include "lval.h"

lval eval(mpc_ast_t* t);
lval eval_op(long x, char* op, long y);
