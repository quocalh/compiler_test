#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdarg.h>
#include "token.h"

#define PORT_ARG_CONFIG void* station, void* self, int argc, ...
#define STATION_ARG_CONFIG void* station, void* self, int argc, void* args[argc]
#define VISITORBASE void* (*connect)(PORT_ARG_CONFIG)

#define PORT_SAFECHECK \
    /*if (!station) \
        ERROR("station is null"); \ */ \
    if (!self) \
        ERROR("self is null"); \

// Using 'macro_i' to prevent variable collisions in the body function
#define UNPACK_VARIADIC_ARGS(count_var, array_name) \
    void* array_name[(count_var)]; \
    do { \
        va_list macro_ap; \
        va_start(macro_ap); \
        for (int macro_i = 0; macro_i < (count_var); macro_i++) { \
            array_name[macro_i] = va_arg(macro_ap, void*); \
        } \
        va_end(macro_ap); \
    } while(0)


// Expression
typedef struct{
    VISITORBASE;
    void* expression;
} Expression;
typedef struct{
    VISITORBASE;
    const char* name;
    void* expression;
} Assign;
typedef struct{
    VISITORBASE;
    void* left;
    void* right;
    TokenType op;
} Binary;
typedef struct{
    VISITORBASE;
    void* expression; 
    TokenType op;
} Unary; 
typedef struct{
    VISITORBASE;
    void* literal;
    TokenType type;
} Literal;
typedef struct{
    VISITORBASE;
    void* expr;
    const char* name;
} Variable;

Expression* ExpExpressionInit(void* expression);
Variable* ExVariableInit(void* expr, const char* name);
Binary* ExBinaryInit(void* left, void* right, TokenType op);
Unary* ExUnaryInit(void* expression, TokenType op);
Literal* ExLiteralInit(void* literal, TokenType type);

/* 
 * create a station
 * note that the loading funtions are expected to be implemented and handled by the file using this module.
 */
typedef struct{
    void* (*port_expression)(STATION_ARG_CONFIG);
    void* (*port_assign)(STATION_ARG_CONFIG);
    void* (*port_binary)(STATION_ARG_CONFIG);
    void* (*port_unary)(STATION_ARG_CONFIG);
    void* (*port_literal)(STATION_ARG_CONFIG);
    void* (*port_variable)(STATION_ARG_CONFIG);
} ExStation;
ExStation* ExStationInit();
void ExStationDestruct(ExStation* station);

// create the corresponding key for each struct(connect functions)
void* ExPortExpression(PORT_ARG_CONFIG);
void* ExPortAssign(PORT_ARG_CONFIG);
void* ExPortBinary(PORT_ARG_CONFIG);
void* ExPortUnary(PORT_ARG_CONFIG);
void* ExPortLiteral(PORT_ARG_CONFIG);

void ExpressionFreeLiteral(void* literal);
void ExpressionPrintLiteral(void* literal);

#endif