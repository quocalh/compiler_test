#ifndef MISC
#define MISC

#include <stdio.h>

#define KB 1024
#define MB 1024 * KB
#define GB 1024 * MB

#define AS(type, object) ((type)(object))
#define STR(str) #str

// debug print
#define PRINT_TABS(indent) \
    for (int i = 0; i < indent; i++) \
        printf("\t"); 

#define min(a, b) ((a) < (b))? (a) : (b)
#define max(a, b) ((a) > (b))? (a) : (b)

#endif