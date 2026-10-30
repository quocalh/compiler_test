#include "parser_stmt.h"
#include "parser_expr.h"

#define PARSE_CONSUME_TOKEN(status, parser, expected, errcode, message, ...) \
    do { \
        Token* token = parser_peek_token(status, parser, parser->current);\
        if (!STATUS_OK(status)){ \
            if ((status)->code == INDEX_ERROR){ \
                CLEAR_STATUS(status); \
                SET_STATUS(status, errcode, message __VA_OPT__(,) __VA_ARGS__); \
            } \
            return NULL; \
        } \
        \
        if (token->type != expected){ \
            SET_STATUS(status, errcode, message __VA_OPT__(,) __VA_ARGS__); \
            return NULL; \
        } \
        (parser)->current++; \
    } while(0)

Stmt* parser_arena_stmt_allocate_stmt(Status* status, Parser* parser, 
    Expr* expr)
{
    Stmt* stmt = arena_allocate(status, parser->statement_arena, sizeof(*stmt));
    if (!STATUS_OK(status))
        return NULL;
    
    *stmt = (Stmt){
        .type = STMT,
        .expression = expr, 
    };
    return stmt;
}
    
PrintStmt* parser_arena_stmt_allocate_print_stmt(Status* status, Parser* parser, 
    Expr* expr)
{
    PrintStmt* stmt = arena_allocate(status, parser->statement_arena, sizeof(*stmt));
    if (!STATUS_OK(status))
        return NULL;
    
    *stmt = (PrintStmt){
        .type = PRINT_STMT,
        .expression = expr,
    };
    return stmt;
}

// maybe a macro for consuming characters if needed
Stmt* parser_parse_statement(Status* status, Parser* parser)
{
    Token* token = parser_peek_token(status, parser, parser->current);
    if (!STATUS_OK(status)) return NULL;

    if (token->type == PRINT)
        return (Stmt*)parser_print_stmt(status, parser);

    return parser_stmt(status, parser);
}

Stmt* parser_stmt(Status* status, Parser* parser)
{
    // consume expression
    Expr* expr = parser_expression(status, parser);
    if (!STATUS_OK(status)) return NULL;

    // consume a semi-colon
    PARSE_CONSUME_TOKEN(status, parser, SEMICOLON, 
        PARSER_PARSE_EXPECT_A_SEMICOLON, "expect a semicolon.");
    
    // create stmt
    Stmt* stmt = parser_arena_stmt_allocate_stmt(status, parser, expr);
    if (!STATUS_OK(status)) return NULL;

    return stmt;
}

PrintStmt* parser_print_stmt(Status* status, Parser* parser)
{
    // skip the PRINT mnemonic
    parser->current++;
    
    // consume expression
    Expr* expr = parser_expression(status, parser);
    if (!STATUS_OK(status)) return NULL;

    // consume a semi-colon
    PARSE_CONSUME_TOKEN(status, parser, SEMICOLON, 
        PARSER_PARSE_EXPECT_A_SEMICOLON, "expect a semicolon.");

    // create stmt
    PrintStmt* stmt = parser_arena_stmt_allocate_print_stmt(status, parser, expr);
    if (!STATUS_OK(status)) return NULL;

    return stmt;
}

VarDeclareStmt* parser_vardecl_stmt(Status* status, Parser* parser)
{
}