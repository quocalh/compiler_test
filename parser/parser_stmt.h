#ifndef PARSER_STMT_H
#define PARSER_STMT_H

#include "../misc/error.h"
#include "parser.h"
#include "../ast/expr.h"
#include "../ast/stmt.h"

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
    Expr* expr);

PrintStmt* parser_arena_stmt_allocate_print_stmt(Status* status, Parser* parser, 
    Expr* expr);

BlockStmt* parser_arena_stmt_allocate_block_stmt(Status* status, Parser* parser, 
    size_t length, Stmt* head);

VarDeclareStmt* parser_arena_stmt_allocate_vardecl_stmt(Status* status, Parser* parser, 
    const char* name, Expr* initializer);

VarDeclareStmt* parser_arena_stmt_allocate_vardecl_stmt(Status* status, Parser* parser, 
    const char* name, Expr* initializer);

// function used by parser_parse
Stmt* parser_parse_statement(Status* status, Parser* parser);
Stmt* parser_parse_declaration(Status* status, Parser* parser);

// build the statement tree
Stmt* parser_stmt(Status* status, Parser* parser);
PrintStmt* parser_print_stmt(Status* status, Parser* parser);
BlockStmt* parser_block_stmt(Status* staus, Parser* parser);
IfStmt* parser_if_stmt(Status* status, Parser* parser);
VarDeclareStmt* parser_vardecl_stmt(Status* status, Parser* parser);

#endif