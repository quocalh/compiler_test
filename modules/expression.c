#include "stdarg.h"
#include "expression.h"
#include "../misc/heap.h"
#include "../misc/assert_.h"



// port functions (accept)
void* ExPortExpression(PORT_ARG_CONFIG){
    PORT_SAFECHECK;
    ExStation* station_ = station;

    // get the stream of args
    UNPACK_VARIADIC_ARGS(argc, args);

    // passing down
    void* ptr = station_->port_expression(station, self, argc, args);
    return ptr;
}
void* ExPortVariable(PORT_ARG_CONFIG){
    PORT_SAFECHECK;
    ExStation* station_ = station;

    // get the stream of args
    UNPACK_VARIADIC_ARGS(argc, args);

    // passing down
    void* ptr = station_->port_variable(station, self, argc, args);
    return ptr;

}
void* ExPortBinary(PORT_ARG_CONFIG){
    PORT_SAFECHECK;
    ExStation* station_ = station;

    // get the stream of args
    UNPACK_VARIADIC_ARGS(argc, args);

    // passing down
    void* ptr = station_->port_binary(station, self, argc, args);
    return ptr;
}
void* ExPortUnary(PORT_ARG_CONFIG){
    PORT_SAFECHECK;
    ExStation* station_ = station;

    // get the stream of args
    UNPACK_VARIADIC_ARGS(argc, args);

    // passing down
    void* ptr = station_->port_unary(station, self, argc, args);
    return ptr;
}
void* ExPortLiteral(PORT_ARG_CONFIG){
    PORT_SAFECHECK;
    ExStation* station_ = station;

    // get the stream of args
    UNPACK_VARIADIC_ARGS(argc, args);

    // passing down
    void* ptr = station_->port_literal(station, self, argc, args);
    return ptr;
}


// expressions
Expression* ExpExpressionInit(void* expression)
{
    Expression* e = HeapInsInit(sizeof(*e));
    *e = (Expression){
        .expression = expression,
        .connect = ExPortExpression
    };
    return e;
}
Variable* ExVariableInit(void* expr, const char* name)
{
    Variable* a = HeapInsInit(sizeof(*a));
    *a = (Variable){
        .expr = expr,
        .name = name,
        .connect = ExPortVariable,
    };
    return a;
}
Binary* ExBinaryInit(void* left, void* right, TokenType op)
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
Unary* ExUnaryInit(void* expression, TokenType op)
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

// station
ExStation* ExStationInit()
{
    ExStation* ptr = HeapInsInit(sizeof(*ptr));
    return ptr;
}
void ExStationDestruct(ExStation* station)
{
    free(station);
}