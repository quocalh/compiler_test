#ifndef SYSTEM_H
#define SYSTEM_H

#include "../misc/heap.h"

typedef struct{
    Heap* tokens;
    const char* file_name;
} System;

System* SystemInit(const char* file_name);
void* SystemDestruct(System* system);

#endif