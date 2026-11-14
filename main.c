
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
#include "interpreter/interpreter.h"


#define SCANNER_PHASE
#define PARSER_PHASE
#define INTERPRETER_PHASE

#define SCANNER_VERBOSE
#define PARSER_VERBOSE
#define INTERPRETER_VERBOSE

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


#ifndef SCANNER_PHASE
    return 1;
#endif
    // init scanner
    // Scanner* scanner = scanner_init(status, "test_codes/ast_expr_test.txt");
    // Scanner* scanner = scanner_init(status, "test_codes/ast_exec_block_test.txt");

    Scanner* scanner = scanner_init(status, "test_codes/ast_control_flow_test.txt");
    if (!STATUS_OK(status)) goto CLEAN_SCANNER;

    // extracting tokens
    scanner_scan(status, scanner);
    if (!STATUS_OK(status)) goto CLEAN_SCANNER;

#ifdef SCANNER_VERBOSE
    printf("scanner debug: \n");
    for (size_t i = 0; i < scanner->tokens->length; i++)
    {
        Token* token = scanner->tokens->ptr +
            i * sizeof(*token);
        printf("Token Line: %-5d| ID: %-4d| Lexeme: %s\n",
            token->line, 
            token->type,
            token->lexeme
        );
    }
    printf("\n");
#endif


#ifndef PARSER_PHASE
    goto CLEAN_SCANNER;    
#endif
    Parser* parser = parser_init(status, scanner->tokens);
    if (!STATUS_OK(status)) goto CLEAN_PARSER;

    parser_parse(status, parser);
    if (!STATUS_OK(status)) goto CLEAN_PARSER;

#ifdef PARSER_VERBOSE
    printf("parser debug: \n");
    for (int i = 0; i < parser->statements->length; i++) 
    {
        Stmt* stmt = parser_peek_stmt(status, parser, i);
        if (!STATUS_OK(status)) goto CLEAN_PARSER;

        stmt_debug_dispatch(status, stmt, 0);
        if (!STATUS_OK(status)) goto CLEAN_PARSER;
    }
#endif


#ifndef INTERPRETER_PHASE
    goto CLEAN_PARSER;
#endif

    Interpreter* interpreter = interpreter_init(status, parser->statements);
    interpreter_interpret(status, interpreter);

    CLEAN_INTERPRETER:
    interpreter_destruct(status, interpreter);

    CLEAN_PARSER:
    parser_destruct(status, parser);

    CLEAN_SCANNER:
    scanner_destruct(status, scanner);

    status_reporting(status);
}
