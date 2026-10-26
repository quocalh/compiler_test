
#ifndef PARSER_AST_H
#define PARSER_AST_H

#include "parser.h"

// arena_init expr
// thinking
Expr* parser_arena_expr_allocate_expression(Status* status, Parser* parser, Expr* expr);
Binary* parser_arena_expr_allocate_binary(Status* status, Parser* parser, 
    Expr* left, Expr* right, TokenType op);
Unary* parser_arena_expr_allocate_unary(Status* status, Parser* parser,
    Expr* expr, TokenType op);
Literal* parser_arena_expr_allocate_literal(Status* status, Parser* parser, 
    const char* _lexeme, TokenType dtype);

// build the ast tree
Expr* parser_expression(Status* status, Parser* parser);
Expr* parser_equality(Status* status, Parser* parser);
Expr* parser_comparision(Status* status, Parser* parser);
Expr* parser_term(Status* status, Parser* parser);
Expr* parser_factor(Status* status, Parser* parser);
Expr* parser_unary(Status* status, Parser* parser);
Expr* parser_grouping(Status* status, Parser* parser);
Expr* parser_primary(Status* status, Parser* parser);

#endif