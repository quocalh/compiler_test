#include "parser.h"
#include "parser_stmt.h"

Parser* parser_init(Status* status, Vector* _tokens)
{
    Parser* parser = malloc(sizeof(*parser));
    if (!parser)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        goto EXIT;
    }

    Arena* expr_arena = arena_init(status, EXPRESSION_ARENA_SIZE);
    if (!STATUS_OK(status))
        goto FREE_STMT_ARENA;

    Arena* stmt_arena = arena_init(status, STATEMENT_ARENA_SIZE);
    if (!STATUS_OK(status))
        goto FREE_EXPR_ARENA;

    Vector* statements = vector_init(status, sizeof(Stmt*));
    if (!STATUS_OK(status))
        goto FREE_STATEMENT;
    
    (*parser) = (Parser){
        ._tokens = _tokens,
        .statements = statements,

        .expression_arena = expr_arena,
        .statement_arena = stmt_arena,

        .current = 0
    };

    return parser;

    FREE_STATEMENT:
    free(statements);

    FREE_EXPR_ARENA:
    free(expr_arena);

    FREE_STMT_ARENA:
    free(stmt_arena);
    
    EXIT:
    return NULL;
}


// destruct functions, status check and sudden return 
// (control flow break) is strictly prohibited here
void parser_destruct(Status* status, Parser* parser)
{
    if (!parser)
    {
        SET_STATUS(status, NULL_PARSER, "null parser.");
        return;
    }

    arena_destruct(status, parser->expression_arena);
    arena_destruct(status, parser->statement_arena);
    vector_destruct(status, parser->statements);

    free(parser);
}

static Stmt* stmt_peek(Status status, Parser* parser, size_t current)
{
    // (Stmt*)* -> array storing stmt pointers
    Stmt** stmt = (Stmt**)parser->statements->ptr; 
    return stmt[current];
}

void parser_parse(Status* status, Parser* parser)
{
    parser->current = 0;

    while (parser->current < parser->_tokens->length)
    {
        Stmt* stmt = parser_parse_declaration(status, parser);
        if (!STATUS_OK(status)) goto EXIT;

        vector_add(status, parser->statements, &stmt);
        if (!STATUS_OK(status)) goto EXIT;
    }

    EXIT:
    return;
}

Stmt* parser_parse_declaration(Status* status, Parser* parser)
{
    Token* token = parser_peek_token(status, parser, parser->current);
    if (!STATUS_OK(status)) return NULL;

    if (token->type == VAR)
    {
        return (Stmt*)parser_vardecl_stmt(status, parser);
    }

    return parser_parse_statement(status, parser);
}

Stmt* parser_parse_statement(Status* status, Parser* parser)
{
    Token* token = parser_peek_token(status, parser, parser->current);
    if (!STATUS_OK(status)) return NULL;

    if (token->type == PRINT)
        return (Stmt*)parser_print_stmt(status, parser);

    if (token->type == LEFT_BRACE)
        return (Stmt*)parser_block_stmt(status, parser);
    
    if (token->type == IF)
        return (Stmt*)parser_if_stmt(status, parser);

    return parser_stmt(status, parser);
}


// helper functions
Token* parser_peek_token(Status* status, Parser* parser, size_t index)
{
    if (index >= parser->_tokens->length)
    {
        SET_STATUS(status, INDEX_ERROR, 
        "index error (index of %zu, exceeding the allocated of %zu + 1)", 
        index, parser->_tokens->length);
        return NULL;
    }

    return (Token*)parser->_tokens->ptr + index;
}

Stmt* parser_peek_stmt(Status* status, Parser* parser, size_t index)
{
    if (index >= parser->statements->length)
    {
        SET_STATUS(status, INDEX_ERROR, 
        "index error (index of %zu, exceeding the allocated of %zu + 1)", 
        index, parser->_tokens->length);
        return NULL;
    }

    // (Stmt*)* array of stmt pointers
    Stmt** stmts = (Stmt**)parser->statements->ptr;
    return stmts[index];
}