#include <stdio.h>

#include "scanner.h"
#include "../misc/heap.h"

#define SCANNER "scanner"

Scanner* ScannerInit(System* system)
{
    Scanner* scanner = HeapInsInit(sizeof(*scanner));
    
    *scanner = (Scanner){
        .file_name = system->file_name,
        
        .tokens = system->tokens,

        .start = 0,
        .current = 0,
        .line = 0,
    };
    return scanner;
}

void ScannerScan(Scanner* scanner)
{
    // read file
    FILE* file = fopen(scanner->file_name, "r");
    ASSERT(file, SCANNER, "can't allocate space for string stream. (file ~ fopen)");
    
    // allocating space for the file string stream
    
    // translating string into tokens
    
    // collapse
    fclose(file);
    
}

void ScannerFree(Scanner* scanner)
{
    HeapInsFree(scanner->tokens);
}