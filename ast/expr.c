#include <stdio.h>
#include <stdlib.h>

#include "expr.h"
#include "../misc/misc.h"


void expr_debug_dispatch(Status* status, Expr* expr, int indent)
{
    switch (expr->type)
    {
        case EXPRESSION:
            expr_debug_expr(status, expr, indent);
            break;
        
        case ASSIGN:
            expr_debug_assign(status, (Assign*)expr, indent);
            break;

        case BINARY:
            expr_debug_binary(status, (Binary*)expr, indent);
            break;

        case UNARY:
            expr_debug_unary(status, (Unary*)expr, indent);
            break;
        
        case GROUPING:
            expr_debug_grouping(status, (Grouping*)expr, indent);
            break;

        case LITERAL:
            expr_debug_literal(status, (Literal*)expr, indent);
            break;
        
        case VARIABLE:
            expr_debug_variable(status, (Variable*)expr, indent);
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

void expr_debug_assign(Status* status, Assign* assign, int indent)
{
    PRINT_TABS(indent); printf("assign: \n");

    // lvalue handle
    switch (assign->lvalue_type)
    {
        case VARIABLE:
            PRINT_TABS(indent); printf("name:\n");
            PRINT_TABS(indent + 1); printf("%s\n", assign->_name);
            break;
        
    }
    PRINT_TABS(indent); printf("rvalue:\n");
    indent++;
    expr_debug_dispatch(status, assign->rvalue, indent);
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

void expr_debug_grouping(Status* status, Grouping* grouping, int indent)
{
    PRINT_TABS(indent);
    printf("grouping: \n");

    indent++;
    expr_debug_dispatch(status, grouping->expr, indent);
    indent--;
}

void expr_debug_literal(Status* status, Literal* literal, int indent)
{
    switch(literal->dtype)
    {
        case INT:
            PRINT_TABS(indent);
            printf("(int): %s\n", literal->_lexeme);
            break;

        case DOUBLE:
            PRINT_TABS(indent);
            printf("(double): %s\n", literal->_lexeme);
            break;

        case STRING:
            PRINT_TABS(indent);
            printf("(str): %s\n", literal->_lexeme);
            break;

        default:
        {
            SET_STATUS(status, AST_LITERAL_UNKNOWN_DTYPE, 
                "unknown type of data (dtype id: %d)", literal->dtype);
            break;
        }
    }
}

void expr_debug_variable(Status* status, Variable* variable, int indent)
{
    PRINT_TABS(indent); printf("variable: %s\n", 
        variable->_name);
}
