#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>
#include "token.h"

typedef struct
{
  char* fileName;
  int sourceLength;
  Token* tokens_ptr;
} Scanner;

Scanner ScannerInit(char* source_ptr);

bool ScannerScan_(Scanner* scanner, char* fileName);

bool ScannerConvertIntoTokens(Scanner* scanner_ptr, char c);

bool isAtEnd();

void Free(Scanner* scanner);

bool ScannerScanToken(Token* token_ptr);

void ScannerDestruct(Scanner* scanner_ptr);

#endif
