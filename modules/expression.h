#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdarg.h>
#include "token.h"

#define PORT_ARG_CONFIG void* station, void* self, ...
#define STATION_ARG_CONFIG void* station, void* self, va_list ap
#define VISITORBASE void* (*connect)(PORT_ARG_CONFIG)

typedef struct{
    VISITORBASE;
    void* expression;
} Expression;
typedef struct{
    VISITORBASE;
    void* left;
    void* right;
    Token* op;
} Binary;
typedef struct{
    VISITORBASE;
    void* expression; 
    Token* op;
} Unary; 
typedef struct{
    VISITORBASE;
    void* literal;
    TokenType type;
} Literal;

Expression* ExpExpressionInit(void* expression);
Binary* ExBinaryInit(void* left, void* right, Token* op);
Unary* ExUnaryInit(void* expression, Token* op);
Literal* ExLiteralInit(void* literal, TokenType type);

/* 
 * create a station
 * note that the loading funtions are expected to be implemented and handled by the file using this module.
 */
typedef struct{
    void* (*port_expression)(STATION_ARG_CONFIG);
    void* (*port_binary)(STATION_ARG_CONFIG);
    void* (*port_unary)(STATION_ARG_CONFIG);
    void* (*port_literal)(STATION_ARG_CONFIG);
} ExStation;

// create the corresponding key for each struct(connect functions)
void* ExPortExpression(PORT_ARG_CONFIG);
void* ExPortBinary(PORT_ARG_CONFIG);
void* ExPortUnary(PORT_ARG_CONFIG);
void* ExPortLiteral(PORT_ARG_CONFIG);


#endif