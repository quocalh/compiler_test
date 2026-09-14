#include <stdlib.h>

#include "system.h"

#include "token.h"
#include "../misc/heap.h"
#include "../misc/assert_.h"

#define SYSTEM "system.c"

System* SystemInit(const char* file_name)
{
    Heap* new_heap = HeapInit(sizeof(Token));

    System* system = malloc(sizeof(*system));
    ASSERT(system, SYSTEM, "can't allocate mem for system");

    system->tokens = new_heap;
    system->file_name = file_name;

    return system;
}

void* SystemDestruct(System* system)
{
   HeapFree(system->tokens);
   free(system);
}