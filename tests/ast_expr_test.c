#include <stdio.h>
#include <stdlib.h>

#include "misc/error.h"
#include "misc/vector.h"
#include "misc/misc.h"
#include "misc/uthash.h"
#include "misc/hash_str_to_int.h"
#include "misc/arena.h"
#include "scanner/scanner.h"

#include "profiling/stop_watch.h"
#include "ast/expr.h"
#include <unistd.h>

// creation
Expr* expr_expr_init(Status* status, Expr* expr);
Binary* expr_binary_init(Status* status, Expr* left, Expr* right, TokenType op);
Unary* expr_unary_init(Status* status, Expr* expr, TokenType op);
Grouping* expr_grouping_init(Status* status, Expr* expr);
Literal* expr_literal_init(Status* status, const char* _lexeme, TokenType dtype);

// destruction
void expr_destruct_dispatch(Status* status, void* expr);
void expr_expr_destruct(Status* status, Expr* expr);
void expr_binary_destruct(Status* status, Binary* binary);
void expr_unary_destruct(Status* status, Unary* unary);
void expr_grouping_destruct(Status* status, Grouping* grouping);
void expr_literal_destruct(Status* status, Literal* literal);

int main()
{
    Status _status;
    status_setup(&_status);
    Status* status = &_status;
    
    // init scanner
    Scanner* scanner = scanner_init(status, "test_codes/scanner_test.txt");
    if (!STATUS_OK(status))
    {
        status_reporting(status);
        goto CLEAN_SCANNER;
    }

    // handling file
    scanner_scan(status, scanner);
    if (!STATUS_OK(status))
    {
        status_reporting(status);
        goto CLEAN_SCANNER;
    }

    CLEAN_SCANNER:
    scanner_destruct(status, scanner);
    
    char* v1 = "4";
    char* v2 = "5.67";
    char* v3 = "5.76";

    printf("hello world\n");

    Expr* expression = expr_expr_init(status, 
        (Expr*)expr_binary_init(
            status,
            (Expr*)expr_literal_init(status, v1, INT),
            (Expr*)expr_grouping_init(
                status,
                (Expr*)expr_binary_init(
                    status,
                    (Expr*)expr_literal_init(status, v2, DOUBLE),
                    (Expr*)expr_unary_init(
                        status,
                        (Expr*)expr_literal_init(status, v3, DOUBLE),
                        SIGN
                    ),
                    PLUS
                )
            ),
            PLUS
        )
    );


    // debug print
    expr_debug_dispatch(status, expression, 0);
    if (!STATUS_OK(status))
    {
        status_reporting(status);
    }

    // free it 
    expr_destruct_dispatch(status, expression);

}

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

Grouping* expr_grouping_init(Status* status, Expr* expr)
{
    Grouping* grouping = malloc(sizeof(*grouping));
    if (!grouping)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        return NULL;
    }

    (*grouping) = (Grouping){
        .expr = expr,
        .type = GROUPING
    };

    return grouping;
}

Literal* expr_literal_init(Status* status, const char* _lexeme, TokenType dtype)
{
    Literal* literal = malloc(sizeof(*literal));
    if (!literal)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        return NULL;
    }
    
    (*literal) = (Literal){
        .dtype = dtype,
        ._lexeme = _lexeme,
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
        
        case GROUPING:
            expr_grouping_destruct(status, expr);
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
void expr_grouping_destruct(Status* status, Grouping* grouping)
{
    expr_destruct_dispatch(status, grouping->expr);
    free(grouping);
}
void expr_literal_destruct(Status* status, Literal* literal)
{
    free(literal);
}