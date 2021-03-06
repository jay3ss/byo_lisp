#include "evaluation.h"

lval eval(mpc_ast_t* t) {
    // If tagged as a number return it directly
    if (strstr(t->tag, "number")) {
        // Check if there is some error in conversion
        errno = 0;
        long x = strtol(t->contents, NULL, 10);
        return errno != ERANGE ? lval_num(x) : lval_error(LERR_BAD_NUM);
    }

    // The operator is always second child.
    char* op = t->children[1]->contents;
    lval x = eval(t->children[2]);

    //Iterate the remaining children and combining
    int i = 3;
    while (strstr(t->children[i]->tag, "expr")) {
        x = eval_op(x, op, eval(t->children[i]));
        i++;
    }

    return x;
}

lval eval_op(lval x, char* op, lval y) {
    // If either value is an error return it
    if (x.type == LVAL_ERR) { return x; }
    if (y.type == LVAL_ERR) { return y; }

    // Otherwise do some math on the number values
    if (strcmp(op, "+") == 0) { return lval_num(x.num + y.num); }
    if (strcmp(op, "-") == 0) { return lval_num(x.num - y.num); }
    if (strcmp(op, "*") == 0) { return lval_num(x.num * y.num); }
    if (strcmp(op, "/") == 0) {
        // If the second operand is zero return error
        return y.num == 0
            ? lval_err(LERR_DIV_ZERO)
            : lval_num(x.num / y.num);

    }

    return lval_err(LERR_BAD_OP);
}
