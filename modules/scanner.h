#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>
#include <stdio.h>
#include "token.h"

typedef struct
{
  char* fileName;
  int sourceLength;
  Token* tokens;
  int tokenCount;
} Scanner;

Scanner ScannerInit(char* source_ptr);

bool ScannerConvertIntoTokens(Scanner* scanner_ptr, char* fileName);

bool ScannerConvertIntoTokens1(Scanner* scanner_ptr, char* buffer, int buffer_length);

// bool ScannerString();

void ScannerDestruct(Scanner* scanner_ptr);

bool ScannerNumber();

bool ScannerString();

#endif
