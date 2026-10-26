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

/*
    TEST CHECK_PROPAGATE
    APPLY GOTO CYCLE CLEANUP
    DEMONSTRATE THE USE OF HASHSTRINGTOINT INTERFACE
*/

#define KILO 1024
#define MEGA 1024 * KILO
#define GIGA  1024 * GIGA

int main()
{
    Status _status;
    status_setup(&_status);
    Status* status = &_status;
    
    // init scanner
    Scanner* scanner = scanner_init(status, "test_codes/scanner_test.txt");
    if (!STATUS_OK(status))
    {
        status_reporting(status);
        goto CLEAN_SCANNER;
    }

    // handling file
    scanner_scan(status, scanner);
    if (!STATUS_OK(status))
    {
        status_reporting(status);
        goto CLEAN_SCANNER;
    }

    CLEAN_SCANNER:
    scanner_destruct(status, scanner);
    
    char* v1 = "4";
    char* v2 = "5.67";
    char* v3 = "5.76";

    printf("hello world\n");

    Expr* expression = expr_expr_init(status, 
        (Expr*)expr_binary_init(
            status,
            (Expr*)expr_literal_init(status, v1, INT),
            (Expr*)expr_grouping_init(
                status,
                (Expr*)expr_binary_init(
                    status,
                    (Expr*)expr_literal_init(status, v2, DOUBLE),
                    (Expr*)expr_unary_init(
                        status,
                        (Expr*)expr_literal_init(status, v3, DOUBLE),
                        SIGN
                    ),
                    PLUS
                )
            ),
            PLUS
        )
    );


    // debug print
    expr_debug_dispatch(status, expression, 0);
    if (!STATUS_OK(status))
    {
        status_reporting(status);
    }

    // free it 
    expr_destruct_dispatch(status, expression);

}