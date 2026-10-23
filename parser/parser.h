#ifndef PARSER
#define PARSER

#include "../misc/error.h"
#include "../misc/vector.h"
#include "../misc/arena.h"
#include "../misc/misc.h"
#include "../ast/stmt.h"
#include "../ast/expr.h"

#define EXPRESSION_ARENA_SIZE 100 * KB

typedef struct
{
    // _underscore denotes a borrow object here
    Vector* _tokens;

    // store the statement ptr
    Vector* statements;
    Arena* statment_arena;

    // expressions will be stored here
    // consider where to create expression in the arena
    Arena* expression_arena;

    size_t current;
} Parser;

Parser* parser_init(Status* status, Vector* _tokens);
void parser_destruct(Status* status, Parser* parser);
void parser_parser(Status* status, Parser* parser);

#endif