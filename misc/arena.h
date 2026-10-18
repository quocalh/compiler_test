#ifndef ARENA
#define ARENA

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/mman.h>
    #include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

static inline size_t get_page_size(Status* status)
{
#ifdef _WIN32 
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwPageSize;
#else
    long res = sysconf(_SC_PAGE_SIZE);
    if (res == -1)
    {
        BACK_PROPAGATE(status, PAGE_SIZE_QUERY_FAILED, 0, 
            "genuine question, what OS are you using??");
    }
    return res;
#endif
} 

// i love the idea here
/*

struct Arena{
    char* ptr;
    size_t reserved_size;
    size_t commited_size;
    size_t current_offset;

    struct Arena* next;
};
typedef struct Arena Arena;


for scoped arena, the idea is even crazier 
    (a fragmented scope walker)
    through linked list, it can hop between memory holes (idk)
+ given the first child, he can walk you through the entire list, hopping through holes
- absoluate malloc (we avoid this ig)
- we can't free it the easy way, the only way is to reset the arena

struct ScopedArena{
    Arena* parent;
    size_t ptr; 

    size_t capacity;  
    struct ScopedArena next*;     
};
typedef struct ScopedArena ScopedArena

*/

/*
 * reference:
 *  https://andreleite.com/posts/2025/nstl/virtual-memory-arena-allocator/
 *  https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/
 * 
 * this make use of the mem page in Window and Linux
 * by preallocate a specific amount of page table (and allocate more if need more)
 * continous mem (data locality) offer maximum performance (maximize TBL hit)
 * 
 * advantages:
 *      + continous, no fragment, unecessary meta-data info in virtual mem
 *      + stable mem address (realloc may change address (sad))
 * 
 * drawbacks:
 *      - object are quite inconvenient to modify (maybe cache their address would help)
 *      - flush (enforce same lifetime on all objects)
 */

typedef struct{
    char* ptr;
    size_t reserved_size;
    size_t commited_size;
    size_t current_offset;

    size_t page_size;
} Arena;


Arena* arena_init(Status* status, size_t reserved_size);
void arena_destruct(Status* status, Arena* arena);
void arena_reset(Status* status, Arena* arena);

void* arena_allocate(Status* status, Arena* arena, size_t patch_size);
void* arena_append_item(Status* status, Arena* arena, size_t item_size, const void* source_item);

typedef struct{
    char* ptr; 
    Arena* parent;

    size_t original_offset;
} ScopedArena;

ScopedArena* scope_arena_init(Status* status, Arena* parent);
void* scope_arena_allocate(Status* status, ScopedArena* arena, size_t patch_size);
void* scope_arena_append_item(Status* status, ScopedArena* arena, size_t item_size, const void* item);
void scope_arena_deallocate(Status* status, ScopedArena* arena);

inline size_t scope_arena_get_current_offset(Status* status, ScopedArena* arena)
{
    return (size_t)arena->parent->ptr + arena->parent->current_offset - (size_t)arena->ptr;
}

#endif