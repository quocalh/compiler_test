#ifndef SCANNER_H
#define SCANNER_H

#include "misc/heap.h"
#include "system.h"

typedef struct
{  
    // internal attribs
    const char* file_name;

    // import attribs
    Heap* tokens;    

    // working attribs
    int start;
    int current;
    int line;
} Scanner;

Scanner* ScannerInit(System* system);
void ScannerFree(Scanner* scanner);


#endif