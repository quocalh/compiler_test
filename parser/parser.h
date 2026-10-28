#ifndef PARSER
#define PARSER

#include "../misc/error.h"
#include "../misc/vector.h"
#include "../misc/arena.h"
#include "../misc/misc.h"
#include "../ast/stmt.h"
#include "../ast/expr.h"

#define EXPRESSION_ARENA_SIZE 100 * KB
#define STATEMENT_ARENA_SIZE 100 * KB

/*
after some careful considerations, i think, for this interpreter alone
i really should not implement dynamic-type evaluating

the idea dynamic-typed values is worth exploring but as the developing cost will be really high
and irrelevant to the motivation of this

because of the whole implmetation will consist alot of interfaces, and some really exhaustive functions, 
    solely for just matching types

hence, i think it deserves to be a separated topic diving into  
interpreter, and compiler way to handle coding languages

dynamic type is now considered something 
all NUMBER will turn into signed double now
they will for now abide the c lang arithmetic rule
any violations result in an error :D

*/
typedef struct
{
    // _underscore denotes a borrow object here
    // store the statement ptr
    Vector* _tokens;
    Vector* statements;

    // expressions will be stored here
    // consider where to create expression in the arena
    Arena* statement_arena;
    Arena* expression_arena;

    size_t current;
} Parser;

Parser* parser_init(Status* status, Vector* _tokens);
void parser_destruct(Status* status, Parser* parser);
void parser_parse(Status* status, Parser* parser);

#endif