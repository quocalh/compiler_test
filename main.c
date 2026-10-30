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
#include "parser/parser_expr.h"
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
    // Scanner* scanner = scanner_init(status, "test_codes/ast_expr_test.txt");
    Scanner* scanner = scanner_init(status, "test_codes/ast_stmt_test.txt");
    if (!STATUS_OK(status)) goto CLEAN_SCANNER;

    // extracting tokens
    scanner_scan(status, scanner);
    if (!STATUS_OK(status)) goto CLEAN_SCANNER;

    Parser* parser = parser_init(status, scanner->tokens);
    if (!STATUS_OK(status)) goto CLEAN_PARSER;

    parser_parse(status, parser);
    if (!STATUS_OK(status)) goto CLEAN_PARSER;

    for (int i = 0; i < parser->statements->length; i++) 
    {
        Stmt* stmt = parser_peek_stmt(status, parser, i);
        if (!STATUS_OK(status)) goto CLEAN_PARSER;

        stmt_debug_dispatch(status, stmt, 0);
        if (!STATUS_OK(status)) goto CLEAN_PARSER;
    }

    CLEAN_PARSER:
    parser_destruct(status, parser);

    CLEAN_SCANNER:
    scanner_destruct(status, scanner);

    status_reporting(status);
}
