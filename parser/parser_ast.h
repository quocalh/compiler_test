
#ifndef PARSER_AST_H
#define PARSER_AST_H

#include "parser.h"

// arena_init expr
// thinking
void parser_arena_expr_allocate_expression(Status* status);
void parser_arena_expr_allocate_binary(Status* status);
void parser_arena_expr_allocate_unary(Status* status);
void parser_arena_expr_allocate_literal(Status* status);

// build ast tree
void parser_expression(Status* status);
void parser_equality();
void parser_comparision();
void parser_term();
void parser_factor();
void parser_unary();
void parser_primary();

#endif