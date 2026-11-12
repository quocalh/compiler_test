#ifndef EXECUTE_H
#define EXECUTE_H

#include "../misc/error.h"
#include "../ast/stmt.h"
#include "interpreter.h"

// #include "interpreter.h"
// typedef struct Interpreter Interpreter;

void interpreter_execute_stmt_dispatch(Status* status, 
    Interpreter* interpreter, Stmt* stmt);

void interpreter_execute_statement(Status* status, 
    Interpreter* interpreter, Stmt* stmt);

void interpreter_execute_print_statement(Status* status, 
    Interpreter* interpreter, PrintStmt* stmt);

void interpreter_execute_block_statement(Status* status, 
    Interpreter* interpreter, BlockStmt* stmt);

void interpreter_execute_vardecl_statement(Status* status,
    Interpreter* interpreter, VarDeclareStmt* stmt);

#endif