#ifndef MISC
#define MISC

#include <stdio.h>

#define KB 1024
#define MB 1024 * KB
#define GB 1024 * MB

#define AS(type, object) ((type)(object))

// debug print
#define PRINT_TABS(indent) \
    for (int i = 0; i < indent; i++) \
        printf("\t"); 

#endif