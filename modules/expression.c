#include "stdarg.h"
#include "expression.h"
#include "../misc/heap.h"
#include "../misc/assert_.h"
#include "../misc/misc.h"



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
Expression* ExExpressionInit(void* expression)
{
    Expression* e = HeapInsInit(sizeof(*e));
    *e = (Expression){
        .expression = expression,
        .connect = ExPortExpression
    };
    return e;
}
Assign* ExAssignInit(void* expression, char* name)
{
    Assign* a = HeapInsInit(sizeof(*a));
    *a = (Assign){
        .expression = expression,
        .name = name,
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
Variable* ExVariableInit(char* name)
{
    Variable* v = HeapInsInit(sizeof(*v));
    *v = (Variable){
        .name = name,
        // .type = IDENTIFIER, // i feel like sb gonna use it?
        .connect = ExPortVariable,
    };
    return v;
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

// i know, smelly code, but i can't help
// plus the station aint free the literals
void ExpressionFreeLiteral(void* literal)
{
    Literal* l = literal;
    switch (l->type)
    {
    case INT:
    case DOUBLE:
        free(l->literal);
        break;

    case STRING:
        StaticStringFree(l->literal);

    default:
        ERROR_VARIADIC("who are you ?(%p)", literal);
        break;
    }
}
void ExpressionPrintLiteral(void* literal)
{
    printf("(literal) ");
    Literal* l = literal;

    switch (l->type)
    {
        case INT:
            printf("%d", *AS(int*, l->literal));
            break;
        case DOUBLE:
            printf("%lf", *((double*)l->literal));
            break;
        case STRING:
            printf("%s", (char*)((StaticString*)l->literal)->str);
            break;
        default:
            break;
    }
    printf("\n"); 
    
}