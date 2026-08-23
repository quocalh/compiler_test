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

// bool ScannerScan_(Scanner* scanner, char* fileName);

Token ScannerScanToken(int* i, int buffer_size, char buffer[]);

bool ScannerConvertIntoTokens(Scanner* scanner_ptr, char* fileName);

void ScannerDestruct(Scanner* scanner_ptr);

#endif
