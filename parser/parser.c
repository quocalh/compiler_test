#include "parser.h"

Parser* parser_init(Status* status, Vector* _tokens)
{
    Parser* parser = malloc(sizeof(*parser));
    if (!parser)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        goto EXIT;
    }

    Arena* arena = arena_init(status, EXPRESSION_ARENA_SIZE);
    if (!STATUS_OK(status))
        goto FREE_ARENA;

    Vector* statements = vector_init(status, sizeof(Stmt*));
    if (!STATUS_OK(status))
        goto FREE_STATEMENT;
    
    (*parser) = (Parser){
        .statements = statements,
        .expression_arena = arena,
        ._tokens = _tokens,
    };

    return parser;

    FREE_STATEMENT:
    free(statements);

    FREE_ARENA:
    free(arena);
    
    EXIT:
    return NULL;
}

void parser_destruct(Status* status, Parser* parser)
{
    if (!parser)
    {
        SET_STATUS(status, NULL_PARSER, "null parser.");
        return;
    }

    // destruct functions, status check and sudden return (control flow break) 
    // is prohibited here
    arena_destruct(status, parser->expression_arena);
    vector_destruct(status, parser->statements);
}

static Stmt* stmt_peek(Status status, Parser* parser, size_t current)
{
    // (Stmt*)* -> array storing stmt pointers
    Stmt** stmt = (Stmt**)parser->statements->ptr; 
    return stmt[current];
}

void parser_parser(Status* status, Parser* parser)
{

}