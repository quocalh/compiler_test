#include <stdlib.h>

#include "system.h"

#include "token.h"
#include "../misc/heap.h"
#include "../misc/assert_.h"

#define SYSTEM "system.c"

System* SystemInit(const char* file_name)
{
    StaticString* ss = StaticStringInit(file_name);

    System* system = malloc(sizeof(*system));
    if (!system) return NULL;

    // scanner will fill those
    system->tokens = NULL;
    system->stream = NULL;
    system->file_name = file_name;

    return system;
}

void* SystemDestruct(System* system)
{
    HeapFree(system->tokens);
    StaticStringFree(system->stream);
    free(system);
}