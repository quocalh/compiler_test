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
void* arena_append_item(Status* status, Arena* arena, size_t item_size, const void* item);

typedef struct{
    char* ptr; 
    Arena* arena;
} ScopedArena;
#endif