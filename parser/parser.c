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
        ._tokens = _tokens,
        .statements = statements,

        .expression_arena = arena,

        .current = 0
    };

    return parser;

    FREE_STATEMENT:
    free(statements);

    FREE_ARENA:
    free(arena);
    
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
}