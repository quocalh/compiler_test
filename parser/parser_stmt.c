#include "parser_stmt.h"
#include "parser_expr.h"
#include "parser.h"
#include <string.h>

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

BlockStmt* parser_arena_stmt_allocate_block_stmt(Status* status, Parser* parser, 
    size_t length, Stmt* head)
{
    BlockStmt* stmt = arena_allocate(status, parser->statement_arena, sizeof(*stmt));
    if (!STATUS_OK(status)) return NULL;

    *stmt = (BlockStmt){
        .type = BLOCK_STMT,
        .length = length,
        .head = head
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

BlockStmt* parser_block_stmt(Status* status, Parser* parser)
{
    BlockStmt* stmt = NULL;

    // consume the left brace
    parser->current++;

    Vector* arena_loc_buffer = vector_init(status, sizeof(Stmt*));
    if (!STATUS_OK(status)) return NULL;
    size_t length = 0;
    Stmt* head = NULL;

    Token* token;
    while (true)
    {
        token = parser_peek_token(status, parser, parser->current);
        if (!STATUS_OK(status)) goto EXIT;

        // stop cond
        if (token->type == RIGHT_BRACE || 
            parser->current >= parser->_tokens->length - 1
        ) break;

        Stmt* stmt = parser_parse_declaration(status, parser);
        if (!STATUS_OK(status)) goto EXIT;

        vector_add(status, arena_loc_buffer, &stmt);
        if (!STATUS_OK(status)) goto EXIT;
        length++;
    }
    PARSE_CONSUME_TOKEN(status, parser, RIGHT_BRACE, 
        PARSER_PARSE_EXPECT_CHARACTER, "expect a right brace '}'.");
    

    // create a continous array of Stmt* in arena

    head = (Stmt*)(parser->statement_arena->ptr + 
        parser->statement_arena->current_offset);
    for (size_t i = 0; i < length; i++)
    {
        Stmt* slot = arena_allocate(status, parser->statement_arena, 
            sizeof(Stmt*));
        if (!STATUS_OK(status)) goto EXIT;
        
        memcpy(
            slot, 
            arena_loc_buffer->ptr + sizeof(Stmt*) * i, 
            sizeof(Stmt*)
        );
    }
    
    stmt = parser_arena_stmt_allocate_block_stmt(status, parser, 
        length, head);

    EXIT: 
    vector_destruct(status, arena_loc_buffer);
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