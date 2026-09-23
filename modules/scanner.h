#ifndef SCANNER_H
#define SCANNER_H

#include "../misc/heap.h"
#include "../misc/string_handling.h"
#include "system.h"
#include "../misc/hashmap.h"

typedef struct
{  
    // internal attribs
    HashStrToInt* hashmap; 
    int start;
    int current;
    int line;

    // import attribs (System)
    const char* file_name;
    Heap* tokens;    
    StaticString* stream;

} Scanner;

Scanner* ScannerInit(System* system);
void ScannerDestruct(Scanner* scanner);

void ScannerScan(Scanner* scanner, System* system);

#endif