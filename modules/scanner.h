#ifndef SCANNER_H
#define SCANNER_H

#include "../misc/heap.h"
#include "../misc/string_handling.h"
#include "system.h"

typedef struct
{  
    // internal attribs

    // import attribs (System)
    const char* file_name;
    Heap* tokens;    
    StaticString* stream;

    // working attribs
    int start;
    int current;
    int line;
} Scanner;

Scanner* ScannerInit(System* system);
Scanner ScannerInitHeap(System* system);
void ScannerDestruct(Scanner* scanner);

void ScannerScan(Scanner* scanner, System* system);

#endif