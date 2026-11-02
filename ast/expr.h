#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../misc/error.h"
#include "../lexer/token.h"

typedef enum{
    EXPRESSION,
    BINARY,
    UNARY,    
    GROUPING,
    LITERAL,
    VARIABLE,
    ASSIGN,

    // propose an existence of an array, vector
    INDEX,
    // propose an existence of a hash map (valuetype, value -> buffer)
    GET,
} ExpressionType;

// NOTE: the type variable must be on the top
// for the polymorphism to work correctly
typedef struct Expr{
    ExpressionType type;
    struct Expr* expr;
} Expr;

typedef struct{
    ExpressionType type;
    ExpressionType lvalue_type;
    const char* _name; // lvalue kinda
    Expr* rvalue;
} Assign;

typedef struct{
    ExpressionType type;
    void* left;
    void* right;
    TokenType op; 
} Binary;

typedef struct{
    ExpressionType type;
    void* expr;
    TokenType op;
} Unary;

typedef struct{
    ExpressionType type;
    void* expr;
} Grouping;

typedef struct{
    ExpressionType type;

    TokenType dtype;
    const char* _lexeme;
} Literal;

typedef struct{
    ExpressionType type;

    // ownership? borrow it or manually free it?
    // -> borrow from lexeme, since scanner not gonna be destructed mid prog
    const char* _name;
} Variable;

// creation and destruction are handled by the arena

// debug print
void expr_debug_dispatch(Status* status, Expr* expr, int indent);
void expr_debug_expr(Status* status, Expr* expr, int indent);
void expr_debug_assign(Status* status, Assign* assign, int indent);
void expr_debug_binary(Status* status, Binary* binary, int indent);
void expr_debug_unary(Status* status, Unary* unary, int indent);
void expr_debug_grouping(Status* status, Grouping* grouping, int indent);
void expr_debug_literal(Status* status, Literal* literal, int indent);
void expr_debug_variable(Status* status, Variable* variable, int indent);

#endif