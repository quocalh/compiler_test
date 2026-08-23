#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>
#include "token.h"

typedef struct
{
  char* fileName;
  int sourceLength;
  char* string_tokens;
  Token* tokens;
} Scanner;

Scanner ScannerInit(char* source_ptr);

bool ScannerScan_(Scanner* scanner, char* fileName);

bool ScannerConvertIntoTokens(Scanner* scanner_ptr, char c);

void Free(Scanner* scanner);

bool ScannerScanToken(Token* token_ptr);

void ScannerDestruct(Scanner* scanner_ptr);

#endif
