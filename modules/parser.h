#ifndef PARSER_H
#define PARSER_H

#include "expression.h"
#include "../misc/heap.h"
#include "system.h"

typedef struct{
    // internal attribs
    int current;

    // import attribs (System)
    Heap* tokens;
    const char* file_name;

} Parser;

Parser* ParserInit(System* system);
void ParserDestruct(Parser* parser);

// build the AST tree
void ParserParse(Parser* parser);
Expression* ParserExpression(Parser* parser);
Expression* ParserEquality(Parser* parser);
Expression* ParserComparison(Parser* parser);
Expression* ParserTerm(Parser* parser);
Expression* ParserFactor(Parser* parser);
Expression* ParserUnary(Parser* parser);
Expression* ParserExponent(Parser* parser);
Expression* ParserGrouping(Parser* parser);
Expression* ParserPrimary(Parser* parser);

// visitor patterns domain

// print AST debug tree
void ExStationLoadDebugPrint(ExStation* station);
void* ExStationDebugPrintPortExpression(STATION_ARG_CONFIG);
void* ExStationDebugPrintPortBinary(STATION_ARG_CONFIG);
void* ExStationDebugPrintPortUnary(STATION_ARG_CONFIG);
void* ExStationDebugPrintPortLiteral(STATION_ARG_CONFIG);

// build an AST tree () thinking of an error catcher in advance
void ExStationLoadBuild(ExStation* station);
void* ExStationBuildPortExpression(STATION_ARG_CONFIG);
void* ExStationBuildPortBinary(STATION_ARG_CONFIG);
void* ExStationBuildPortUnary(STATION_ARG_CONFIG);
void* ExStationBuildPortLiteral(STATION_ARG_CONFIG);

// free expressions
void ExStationLoadFree(ExStation* station);
void* ExStationFreePortExpression(STATION_ARG_CONFIG);
void* ExStationFreePortBinary(STATION_ARG_CONFIG);
void* ExStationFreePortUnary(STATION_ARG_CONFIG);
void* ExStationFreePortLiteral(STATION_ARG_CONFIG);
#endif