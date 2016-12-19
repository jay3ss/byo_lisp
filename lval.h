#include "mpc.h"

// Create Enumeration of Possible lval Types
enum { LVAL_NUM, LVAL_ERR };

// Create Enumeration of Possible Error Types
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

// Declare New lval Struct
typedef struct {
    int type;
    long num;
    int err;
} lval;

lval lval_num(long x);
lval lval_err(int x);
void lval_print(lval v);
void lval_println(lval v);
lval eval(mpc_ast_t* t);
lval eval_op(lval x, char* op, lval y);
