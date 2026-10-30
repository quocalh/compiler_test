#ifndef PARSER_STMT_H
#define PARSER_STMT_H

#include "../misc/error.h"
#include "parser.h"
#include "../ast/expr.h"
#include "../ast/stmt.h"

Stmt* parser_arena_stmt_allocate_stmt(Status* status, Parser* parser, 
    Expr* expr);
PrintStmt* parser_arena_stmt_allocate_print_stmt(Status* status, Parser* parser, 
    Expr* expr);

Stmt* parser_parse_statement(Status* status, Parser* parser);
Stmt* parser_stmt(Status* status, Parser* parser);
PrintStmt* parser_print_stmt(Status* status, Parser* parser);
VarDeclareStmt* parser_vardecl_stmt(Status* status, Parser* parser);

#endif