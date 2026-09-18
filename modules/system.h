#ifndef SYSTEM_H
#define SYSTEM_H

#include "../misc/heap.h"
#include "../misc/string_handling.h"

typedef struct{
    Heap* tokens;
    StaticString* stream;
    const char* file_name;
} System;

System* SystemInit(const char* file_name);
void* SystemDestruct(System* system);

#endif