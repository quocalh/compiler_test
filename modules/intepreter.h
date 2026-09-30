#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include "expression.h"
#include "statement.h"
#include "../misc/heap.h"

typedef struct{

    // parser subscribe attribs
    ExStation* ex_station; 
    StmtStation* stmt_station;
    Heap* stmts;
    Environment* env;

} Interpreter;
Interpreter* InterpreterInit(Parser* parser);
#endif