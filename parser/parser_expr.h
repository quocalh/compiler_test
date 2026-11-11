
#ifndef PARSER_AST_H
#define PARSER_AST_H

#include "parser.h"

// arena_init expr
// thinking
Expr* parser_arena_expr_allocate_expression(Status* status, Parser* parser, 
    Expr* expr);
Assign* parser_arena_expr_allocate_assign(Status* status, Parser* parser, 
    ExpressionType lvalue_type, const char* _name, Expr* rvalue);
Binary* parser_arena_expr_allocate_binary(Status* status, Parser* parser, 
    Expr* left, Expr* right, TokenType op);
Unary* parser_arena_expr_allocate_unary(Status* status, Parser* parser,
    Expr* expr, TokenType op);
Grouping* parser_arena_expr_allocate_grouping(Status* status, Parser* parser,
    Expr* expr);
PostFix* parser_arena_expr_allocate_postfix(Status* status, Parser* parser, 
    Expr* array, Expr* index);
Literal* parser_arena_expr_allocate_literal(Status* status, Parser* parser, 
    const char* _lexeme, TokenType dtype);
ListLiteral* parser_arena_expr_allocate_list_literal(Status* status, Parser* parser, 
    Expr* element, Expr* next);

// build the ast tree
Expr* parser_expression(Status* status, Parser* parser);
Expr* parser_assignment(Status* status, Parser* parser);
Expr* parser_equality(Status* status, Parser* parser);
Expr* parser_comparision(Status* status, Parser* parser);
Expr* parser_term(Status* status, Parser* parser);
Expr* parser_factor(Status* status, Parser* parser);
Expr* parser_unary(Status* status, Parser* parser);
Expr* parser_grouping(Status* status, Parser* parser);
Expr* parser_postfix(Status* status, Parser* parser);
Expr* parser_primary(Status* status, Parser* parser);
Expr* parser_list_creation(Status* status, Parser* parser);

#endif