#include <stdio.h>
#include <stdlib.h>

// Parsing library
#include "mpc.h"

// Evaluation functions
#include "evaluation.h"

/*if we are compiling on Windows compile these functions */
#ifdef _WIN32
#include "readline_fake.h"

/* Otherwise include the editline headers */
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char** argv) {

    // Create Some Parsers
    mpc_parser_t* Number   = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr     = mpc_new("expr");
    mpc_parser_t* Lispy    = mpc_new("lispy");

    // Define them with the following Language
    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                       \
            number   : /-?[0-9]+/ ;                             \
            operator : '+' | '-' | '*' | '/';                   \
            expr     : <number> | '(' <operator> <expr>+ ')' ;  \
            lispy    : /^/ <operator> <expr>+ /$/ ;             \
        ",
        Number, Operator, Expr, Lispy);

    /* Print Version and Exit Information */
    puts("Lispy Version 0.0.0.0.3");
    puts("Press Ctrl+c to Exit\n");

    /* In a never ending loop */
    while (1) {

        /* Output our prompt and get input */
        char* input = readline("lispy> ");

        /* Add input to history */
        add_history(input);

        /* Echo input back to user */
        // printf("No you're a %s\n", input);

        // Attempt to Parse the user Input
        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lispy, &r)) {
            // On success Print the AST
            long result = eval(r.output);
            printf("%li\n", result);
            mpc_ast_delete(r.output);
        } else {
            // Otherwise Print the Error
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        /* Free retrieved input */
        free(input);
    }

    // Undefine and Delete our Parsers
    mpc_cleanup(4, Number, Operator, Expr, Lispy);

    return 0;
}
