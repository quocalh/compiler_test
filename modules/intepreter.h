#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include "expression.h"
#include "statement.h"
#include "../misc/heap.h"

typedef struct{
    int current;

    // parser subscribe attribs
    ExStation* ex_station; 
    StmtStation* stmt_station;
    Heap* stmts;
    Environment* env;
} Interpreter;

Interpreter* InterpreterInit(Parser* parser);
void* InterpreterDestruct(Interpreter* inp);
void InterpreterInterpret(Interpreter* inp, Parser* parser);

// parser parse
void* ParserDeclaration(Parser* parser);
void* ParserVarDeclaration(Parser* parser);
void* ParserStatement(Parser* parser);
void* ParserExprStmt(Parser* parser);
void* ParserPrintStmt(Parser* parser);

// visitor
void* StmtStationExecutePortStmt(STATION_ARG_CONFIG);
void* StmtStationExecutePortPrintStmt(STATION_ARG_CONFIG);
void* StmtStationExecutePortExprStmt(STATION_ARG_CONFIG);
void* StmtStationExecutePortVarDeclareStmt(STATION_ARG_CONFIG);
void StmtStationLoadExecute(StmtStation* station);


void* StmtStationFreePortStmt(STATION_ARG_CONFIG);
void* StmtStationFreePortPrintStmt(STATION_ARG_CONFIG);
void* StmtStationFreePortExprStmt(STATION_ARG_CONFIG);
void* StmtStationFreePortDeclareStmt(STATION_ARG_CONFIG);
void StmtStationLoadFree(StmtStation* station);

#endif