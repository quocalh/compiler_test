#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include "token.h"

typedef struct
{
  char* source_ptr;
  Token* token_array_ptr;
} Scanner;

Scanner ScannerInit(char* source_ptr);

#endif
