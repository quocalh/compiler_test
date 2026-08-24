#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>
#include <stdio.h>
#include "token.h"
#include "../misc/heap.h"

typedef struct
{
  char* fileName;
  Heap tokens;
  StaticString stream;

  // working parameters
  int start;
  int current;
  int currentLine;
  
} Scanner;

Scanner ScannerInit(char* fileName);

bool ScannerConvertIntoTokens(Scanner* scanner_ptr, char* fileName);

bool ScannerConvertIntoTokens1(Scanner* scanner_ptr, char* fileName);

// bool ScannerString();

void ScannerDestruct(Scanner* scanner_ptr);

bool ScannerNumber();

bool ScannerString();

#endif
