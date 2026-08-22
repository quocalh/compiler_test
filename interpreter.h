#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdbool.h>


typedef struct
{
  bool hadError; // False
}Interpreter;

void InterpreterReport(int line, char where[], char message[]);

void InterpreterError(int line, char message[]);


#endif
