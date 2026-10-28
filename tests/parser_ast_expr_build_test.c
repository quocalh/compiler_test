#include <stdio.h>
#include <stdlib.h>

#include "misc/error.h"
#include "misc/vector.h"
#include "misc/misc.h"
#include "misc/uthash.h"
#include "misc/hash_str_to_int.h"
#include "misc/arena.h"
#include "scanner/scanner.h"
#include "parser/parser.h"
#include "parser/parser_ast.h"
#include "profiling/stop_watch.h"
#include "ast/expr.h"


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
    Scanner* scanner = scanner_init(status, "test_codes/ast_expr_test.txt");
    if (!STATUS_OK(status))
    {
        status_reporting(status);
        goto CLEAN_SCANNER;
    }

    // extracting tokens
    scanner_scan(status, scanner);
    if (!STATUS_OK(status))
    {
        status_reporting(status);
        goto CLEAN_SCANNER;
    }

    Parser* parser = parser_init(status, scanner->tokens);
    if (!STATUS_OK(status)) 
    {
        status_reporting(status);
        goto CLEAN_PARSER;
    }

    // testing 
    Expr* output = parser_expression(status, parser);

    expr_debug_dispatch(status, output, 0);
    if (!STATUS_OK(status))
    {
        status_reporting(status);
        goto CLEAN_PARSER;
    }

    CLEAN_PARSER:
    parser_destruct(status, parser);

    CLEAN_SCANNER:
    scanner_destruct(status, scanner);

    if (!STATUS_OK(status))
        status_reporting(status);
}
// it looks hard to read ngl

/*
test case: 
    (((!!3))) * -(-5) * 6 + (1 + 2 + (3 * 5) * 5 + "nagger")

result:

binary: 
left:
        binary: 
        left:
                binary: 
                left:
                        unary: 
                                unary: 
                                        (int): 3
                right:
                        unary: 
                                unary: 
                                        (int): 5
        right:
                (int): 6
right:
        binary: 
        left:
                binary: 
                left:
                        binary: 
                        left:
                                (int): 1
                        right:
                                (int): 2
                right:
                        binary: 
                        left:
                                binary: 
                                left:
                                        (int): 3
                                right:
                                        (int): 5
                        right:
                                (int): 5
        right:
                (str): "nagger"
*/