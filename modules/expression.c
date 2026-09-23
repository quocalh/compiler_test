#include "stdarg.h"
#include "expression.h"
#include "../misc/heap.h"
#include "../misc/assert_.h"

#define PORT_SAFECHECK \
    if (!station) \
        ERROR("station is null"); \
    if (!self) \
        ERROR("self is null"); \

// port functions (accept)
void* port_expression(PORT_ARG_CONFIG){
    PORT_SAFECHECK;
    ExStation* station_ = station;
    
    va_list ap, passing_ap;
    va_start(ap); va_copy(passing_ap, ap);
    void* ptr = station_->port_expression(station, self, passing_ap);

    va_end(ap);
    return ptr;
}
void* port_binary(PORT_ARG_CONFIG){
    PORT_SAFECHECK;
    return NULL;
}
void* port_unary(PORT_ARG_CONFIG){

    PORT_SAFECHECK;
    return NULL;
}
void* ExPortLiteral(PORT_ARG_CONFIG){

    PORT_SAFECHECK;
    return NULL;
}


Expression* ExpExpressionInit(void* expression)
{
    Expression* e = HeapInsInit(sizeof(*e));
    *e = (Expression){
        .expression = expression,
        .connect = ExPortExpression
    };
    return e;
}
Binary* ExBinaryInit(void* left, void* right, Token* op)
{
    Binary* b = HeapInsInit(sizeof(*b));
    *b = (Binary){
        .left = left,
        .right = right,
        .op = op,
        .connect = ExPortBinary
    };
    return b;
}
Unary* ExUnaryInit(void* expression, Token* op)
{
    Unary* u = HeapInsInit(sizeof(*u));
    *u = (Unary){
        .expression = expression, 
        .op = op,
        .connect = ExPortUnary
    };
    return u;
}
Literal* ExLiteralInit(void* literal, TokenType type)
{
    Literal* l = HeapInsInit(sizeof(*l));
    *l = (Literal){
        .literal = literal,
        .type = type,
        .connect = ExPortLiteral
    };
    return l;
}