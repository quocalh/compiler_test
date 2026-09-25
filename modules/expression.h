#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdarg.h>
#include "token.h"

#define PORT_ARG_CONFIG void* station, void* self, int argc, ...
#define STATION_ARG_CONFIG void* station, void* self, int argc, void* args[argc]
#define VISITORBASE void* (*connect)(PORT_ARG_CONFIG)

typedef struct{
    VISITORBASE;
    void* expression;
} Expression;
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

Expression* ExpExpressionInit(void* expression);
Binary* ExBinaryInit(void* left, void* right, TokenType op);
Unary* ExUnaryInit(void* expression, TokenType op);
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
ExStation* ExStationInit();
void ExStationDestruct(ExStation* station);

// create the corresponding key for each struct(connect functions)
void* ExPortExpression(PORT_ARG_CONFIG);
void* ExPortBinary(PORT_ARG_CONFIG);
void* ExPortUnary(PORT_ARG_CONFIG);
void* ExPortLiteral(PORT_ARG_CONFIG);


#endif