#include <stdio.h>
#include <stdlib.h>

#include "misc/error.h"
#include "misc/vector.h"
#include "misc/misc.h"
#include "misc/uthash.h"
#include "misc/hash_str_to_int.h"
#include "misc/arena.h"
#include "scanner/scanner.h"

#include "profiling/stop_watch.h"
#include "ast/expr.h"
#include <unistd.h>

int main()
{
    Status _status;
    Status* status = &_status;
    status_setup(status);

    LiteralStorage v1 = (LiteralStorage){._lexeme = "4"};
    LiteralStorage v2 = (LiteralStorage){._lexeme = "5.67"};
    LiteralStorage v3 = (LiteralStorage){._lexeme = "5.67"};

    printf("hello world\n");

    Expr* expression = expr_expr_init(status, 
        (Expr*)expr_binary_init(
            status,
            (Expr*)expr_literal_init(status, &v1, INT),
            (Expr*)expr_binary_init(
                status,
                (Expr*)expr_literal_init(status, &v2, DOUBLE),
                (Expr*)expr_unary_init(
                    status,
                    (Expr*)expr_literal_init(status, &v3, DOUBLE),
                    SIGN
                ),
                PLUS
            ),
            PLUS
        )
    );

    // debug
    expr_debug_dispatch(status, expression, 0);
    if (!STATUS_OK(status))
    {
        status_reporting(status);
    }
    

    // free it 
    expr_destruct_dispatch(status, expression);

}