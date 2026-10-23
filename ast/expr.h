#ifndef EXPR
#define EXPR

#include "../misc/error.h"
#include "../lexer/token.h"

typedef enum{
    EXPRESSION,
    BINARY,
    UNARY,    
    // GROUPING,
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
    void* left;
    void* right;
    TokenType op; 
} Binary;

typedef struct{
    ExpressionType type;
    void* expr;
    TokenType op;
} Unary;

typedef union{
    Expr* expr;
    const char* _lexeme;
} LiteralStorage;

typedef struct{
    ExpressionType type;

    TokenType dtype;

    // either another expression
    // or _lexeme char borrowed from scanner
    // check dtype (dtype = EXPRESSION or otherwise)
    LiteralStorage literal;
} Literal;

// creation
Expr* expr_expr_init(Status* status, Expr* expr);
Binary* expr_binary_init(Status* status, Expr* left, Expr* right, TokenType op);
Unary* expr_unary_init(Status* status, Expr* expr, TokenType op);
Literal* expr_literal_init(Status* status, const LiteralStorage* _literal, TokenType dtype);

// destruction
void expr_destruct_dispatch(Status* status, void* expr);
void expr_expr_destruct(Status* status, Expr* expr);
void expr_binary_destruct(Status* status, Binary* binary);
void expr_unary_destruct(Status* status, Unary* unary);
void expr_literal_destruct(Status* status, Literal* literal);

// debug print
void expr_debug_dispatch(Status* status, Expr* expr, int indent);
void expr_debug_expr(Status* status, Expr* expr, int indent);
void expr_debug_binary(Status* status, Binary* binary, int indent);
void expr_debug_unary(Status* status, Unary* unary, int indent);
void expr_debug_literal(Status* status, Literal* literal, int indent);

// testing features
typedef void (*test_func)(Status*, void*);
test_func func_return_func(Status* status);

#endif