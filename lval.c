#include "lval.h"

// Create a new number type lval
lval lval_num(long x) {
    lval v;
    v.type = LVAL_NUM;
    v.num = x;
    return v;
}

// Create a new error type lval
lval lval_err(int x) {
    lval v;
    v.type = LVAL_ERR;
    v.err = x;
    return v;
}

// Print an "lval"
void lval_print(lval v) {
    switch (v.type) {
        // In the case the type is a number print it
        // Then 'break' out of the switch
        case LVAL_NUM:
            printf("%li", v.num);
            break;

        // In the case the type is an error
        case LVAL_ERR:
            if (v.err == LERR_DIV_ZERO) {
                printf("Error: Division By Zero!");
            }
            if (v.err == LERR_BAD_OP) {
                printf("Error: Invalid Operator!");
            }
            if (v.err == LERR_BAD_NUM) {
                printf("Error: Invalid Number!");
            }
        break;
    }
}

void lval_println(lval v) {
    lval_print(v);
    putchar('\n');
}

lval eval(mpc_ast_t* t) {
    // If tagged as a number return it directly
    if (strstr(t->tag, "number")) {
        // Check if there is some error in conversion
        errno = 0;
        long x = strtol(t->contents, NULL, 10);
        return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
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
