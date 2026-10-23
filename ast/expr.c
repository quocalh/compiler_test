#include <stdio.h>
#include <stdlib.h>

#include "expr.h"
#include "../misc/misc.h"

// creation
/*
 * these will not be used by the actual program, 
 * as expression are inited in the arena (they handle the init)
 * these exist solely for testing purposese, separated from the program
 */

Expr* expr_expr_init(Status* status, Expr* expr)
{
    Expr* expression = malloc(sizeof(*expr));
    if (!expr)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        return NULL;
    }

    (*expression) = (Expr){
        .expr = expr,
        .type = EXPRESSION
    };

    return expression;
}

Binary* expr_binary_init(Status* status, Expr* left, Expr* right, TokenType op)
{
    Binary* binary = malloc(sizeof(*binary));
    if (!binary)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        return NULL;
    }

    (*binary) = (Binary){
        .left = left,
        .right = right,
        .op = op,
        .type = BINARY,
    };

    return binary;
}

Unary* expr_unary_init(Status* status, Expr* expr, TokenType op)
{
    Unary* unary = malloc(sizeof(*unary));
    if (!unary)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        return NULL;
    }
    
    (*unary) = (Unary){
        .expr = expr,
        .op = op,
        .type = UNARY
    };

    return unary;
}

Literal* expr_literal_init(Status* status, const LiteralStorage* _literal, TokenType dtype)
{
    Literal* literal = malloc(sizeof(*literal));
    if (!literal)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        return NULL;
    }
    
    (*literal) = (Literal){
        .dtype = dtype,
        .literal = *_literal,
        .type = LITERAL
    };

    return literal;
}



// destruct (cleanup helper class, don't break control flow midway)
/*
 * these will not be used by the actual program, 
 * as expression are stored and freed by the arena
 * these exist solely for testing purposese, separated from the program
 */
void expr_destruct_dispatch(Status* status, void* expr)
{
    ExpressionType type = AS(Expr*, expr)->type;
    switch (type)
    {
        case EXPRESSION:
            expr_expr_destruct(status, expr);
            break;

        case BINARY:
            expr_binary_destruct(status, expr);
            break;

        case UNARY:
            expr_unary_destruct(status, expr);
            break;

        case LITERAL:
            expr_literal_destruct(status, expr);
            break;

        default:
            SET_STATUS(status, AST_EXPR_UNKNOWN_EXPR_TYPE, 
                "found an unknown expr type (enum id: %d).", type);
            break;
    }
    // all flows reach here, don't need a status check here
}

void expr_expr_destruct(Status* status, Expr* expr)
{
    expr_destruct_dispatch(status, expr->expr);
    free(expr);
}
void expr_binary_destruct(Status* status, Binary* binary)
{
    expr_destruct_dispatch(status, binary->left);
    expr_destruct_dispatch(status, binary->right);
    free(binary);
}
void expr_unary_destruct(Status* status, Unary* unary)
{
    expr_destruct_dispatch(status, unary->expr);
    free(unary);
}

void expr_literal_destruct(Status* status, Literal* literal)
{
    if (literal->dtype == EXPRESSION)
    {
        expr_destruct_dispatch(status, literal->literal.expr);
    }
    free(literal);
}

// debug print
#define PRINT_TABS(indent) \
    for (int i = 0; i < indent; i++) \
        printf("\t"); 

void expr_debug_dispatch(Status* status, Expr* expr, int indent)
{
    switch (expr->type)
    {
        case EXPRESSION:
            expr_debug_expr(status, expr, indent);
            break;

        case BINARY:
            expr_debug_binary(status, (Binary*)expr, indent);
            break;

        case UNARY:
            expr_debug_unary(status, (Unary*)expr, indent);
            break;

        case LITERAL:
            expr_debug_literal(status, (Literal*)expr, indent);
            break;

        default:
            SET_STATUS(status, AST_EXPR_UNKNOWN_EXPR_TYPE, 
                "unknown type of expression (expression id: %d)", expr->type);
            break;
    }
}

void expr_debug_expr(Status* status, Expr* expr, int indent)
{
    PRINT_TABS(indent);
    printf("expression: \n");
    
    indent++;
    expr_debug_dispatch(status, expr->expr,  indent);
    indent--;
}

void expr_debug_binary(Status* status, Binary* binary, int indent)
{
    PRINT_TABS(indent);
    printf("binary: \n");
    
    PRINT_TABS(indent); printf("left:\n");
    indent++;
    expr_debug_dispatch(status, binary->left, indent);
    indent--;

    PRINT_TABS(indent); printf("right:\n");
    indent++;
    expr_debug_dispatch(status, binary->right, indent);
    indent--;
}

void expr_debug_unary(Status* status, Unary* unary, int indent)
{
    PRINT_TABS(indent);
    printf("unary: \n");
    
    indent++;
    expr_debug_dispatch(status, unary->expr,  indent);
    indent--;
}

void expr_debug_literal(Status* status, Literal* literal, int indent)
{
    switch(literal->dtype)
    {
        case INT:
            PRINT_TABS(indent);
            printf("(int): %s\n", literal->literal._lexeme);
            break;

        case DOUBLE:
            PRINT_TABS(indent);
            printf("(double): %s\n", literal->literal._lexeme);
            break;

        case STRING:
            PRINT_TABS(indent);
            printf("(str): %s\n", literal->literal._lexeme);
            break;

        case EXPRESSION:
        {
            indent++;
            expr_debug_expr(status, literal->literal.expr, indent);
            indent--;
            break;
        }

        default:
        {
            SET_STATUS(status, AST_LITERAL_UNKNOWN_DTYPE, 
                "unknown type of data (dtype id: %d)", literal->dtype);
            break;
        }
    }
}

// i don't think i will use this... (but i can consider it later)
test_func func_return_func(Status* status)
{
    return (test_func)expr_binary_destruct;
}
