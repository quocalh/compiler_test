#include "parser_stmt.h"
#include "parser_expr.h"

// allocate
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
    if (!STATUS_OK(status)) return NULL;
    
    *stmt = (PrintStmt){
        .type = PRINT_STMT,
        .expression = expr,
    };
    return stmt;
}

VarDeclareStmt* parser_arena_stmt_allocate_vardecl_stmt(Status* status, Parser* parser, 
    const char* name, Expr* initializer)
{
    VarDeclareStmt* stmt = arena_allocate(status, parser->statement_arena, sizeof(*stmt));
    if (!STATUS_OK(status)) return NULL;

    *stmt = (VarDeclareStmt){
        .type = VARDECL_STMT,
        .name = name,
        .initializer = initializer,
    };
    return stmt;
}

// parser statements
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
    // var a ( = 5);

    // consume the var keyword
    parser->current++;

    // consume the identifier
    Token* token = parser_peek_token(status, parser, parser->current); 
    if (!STATUS_OK(status)) return NULL;
    if (token->type != IDENTIFIER)
    {
        SET_STATUS(status, PARSER_PARSE_EXPECT_CHARACTER, 
            "expect an identifier.");
        return NULL;
    }
    const char* _name = token->lexeme;
    parser->current++;

    Expr* initializer = NULL;

    // check for = (var a '= expr';)
    token = parser_peek_token(status, parser, parser->current); 
    if (!STATUS_OK(status)) return NULL;
    if (token->type == EQUAL)
    {
        parser->current++;
        initializer = parser_expression(status, parser);
        if (!STATUS_OK(status)) return NULL;
    }

    // consume the semicolon
    PARSE_CONSUME_TOKEN(status, parser, SEMICOLON, 
        PARSER_PARSE_EXPECT_A_SEMICOLON, "expect a semicolon.");

    // create stmt
    VarDeclareStmt* stmt = parser_arena_stmt_allocate_vardecl_stmt(status, parser, 
        _name, initializer);
    if (!STATUS_OK(status)) return NULL;

    return stmt;
}