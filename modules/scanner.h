#ifndef SCANNER_H
#define SCANNER_H

#include "../misc/heap.h"
#include "../misc/string_handling.h"
#include "system.h"

typedef struct
{  
    // internal attribs
    const char* file_name;

    // import attribs
    Heap* tokens;    
    StaticString stream;

    // working attribs
    int start;
    int current;
    int line;
} Scanner;

Scanner* ScannerInit(System* system);
void ScannerDestruct(Scanner* scanner);

void ScannerScan(Scanner* scanner);

#endif