#include "interpreter.h"
#include <stdio.h>

void InterpreterReport(int line, char where[], char message[])
{
  printf("[line %d] Error %s: %s", line, where, message);
}

void InterpreterError(int line, char message[])
{

}


