#include <stdlib.h>
#include <string.h>

#include "arena.h"
#include "error.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/mman.h>
    #include <unistd.h>
#endif

/*
desire ouput
    1       : 4096
    2       : 4096
    4096    : 4096
    8192    : 8192
    8193    : 12288
derivation of such formula?

naive approach:
    ((reserve / page) + 1) * page
    would failed if reserved  = k pages (K ∈ Z)
correct approach:
    ((reserve + page - 1) / page) * page
notes about this are documented in the scrap notes on interpreter
*/

Arena* arena_init(Status* status, size_t raw_reserved_size)
{
    if (raw_reserved_size == 0)
        BACK_PROPAGATE(status, RESERVED_SIZE_BEING_ZERO, 
            NULL, "reserved size should not be zero no?");

    Arena* arena = malloc(sizeof(*arena));
    if (!arena)
        BACK_PROPAGATE(status, MALLOC_FAILED, 
            NULL, "malloc failed.");

    size_t page_size = get_page_size(status);
    if (!STATUS_OK(status))
    {
        free(arena);
        return NULL;
    }
    size_t reserved_size = raw_reserved_size;

    // align the reservation up to the closest right-hand page-size
    reserved_size = (raw_reserved_size - 1 + page_size) / page_size;
    reserved_size *= page_size;

    // virtual allocating
#ifdef _WIN32
    void* block = VirtualAlloc(NULL, reserved_size, MEM_RESERVE, PAGE_NOACCESS);
    if (!block)
    {
        free(arena);
        BACK_PROPAGATE(status, VIRTUAL_MEM_ALLOC_FAILED, NULL, 
            "virtual alloc failed (arena).");
    }
#else
    // fd = -1 (file descriptor)
    void* block = mmap(NULL, reserved_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (block == MAP_FAILED)
    {
        free(arena);
        BACK_PROPAGATE(status, VIRTUAL_MEM_ALLOC_FAILED, NULL, 
            "virtual alloc failed (arena).");
    }
#endif

    // allocating attribs
    arena->ptr = (char*)block;
    arena->reserved_size = reserved_size;
    arena->page_size = page_size;
    arena->commited_size = 0;
    arena->current_offset = 0;

    return arena;
}

void* arena_allocate(Status* status, Arena* arena, size_t patch_size)
{
    if (!arena)
        BACK_PROPAGATE(status, NULL_ARENA, NULL, "arena being null.");
    
    if (patch_size == 0)
        BACK_PROPAGATE(status, ARENA_ADD_ZERO_SIZE_ITEM, NULL, 
            "an item size of 0.");

    // check valid new_offset
    size_t new_offset = arena->current_offset + patch_size; 
    if (new_offset > arena->reserved_size)
        BACK_PROPAGATE(status, ARENA_SIZE_OVERFLOW, NULL, 
            "arena size overflow.");


    // have to make space
    if (new_offset > arena->commited_size)
    {
        // fill the closest right-hand page frame
        size_t new_commit_size = (arena->page_size + new_offset - 1) / arena->page_size;
        new_commit_size *= arena->page_size;
        if (new_commit_size > arena->reserved_size)
            new_commit_size = arena->reserved_size;
        
        // commit mem (malloc same shit)
        size_t commiting_size = new_commit_size - arena->commited_size;
        void* commiting_address = arena->ptr + arena->commited_size;

#ifdef _WIN32
        if (!VirtualAlloc(commiting_address, commiting_size, MEM_COMMIT, PAGE_READ_WRITE))
            BACK_PROPAGATE(status, ARENA_VIRTUAL_MEM_COMMIT_FAILED, NULL, "virtual mem commit failed.");
#else
        if (mprotect(commiting_address, commiting_size, PROT_READ | PROT_WRITE) != 0)
            BACK_PROPAGATE(status, ARENA_VIRTUAL_MEM_COMMIT_FAILED, NULL, "virtual mem commit failed.");
#endif

        arena->commited_size = new_commit_size;
    }

    void* address = arena->ptr + arena->current_offset;
    arena->current_offset = new_offset;

    return address;
    
}

// wrapper
void* arena_append_item(Status* status, Arena* arena, size_t item_size, const void* source_item)
{
    if (!source_item)
        BACK_PROPAGATE(status, NULL_ITEM, NULL, 
            "souce item is null.");

    char* ptr = arena_allocate(status, arena, item_size);
    if (!STATUS_OK(status))
        return NULL;
    
    memcpy(ptr, source_item, item_size);
    return ptr;
}

void arena_reset(Status* status, Arena* arena)
{
    if (!arena)
        BACK_PROPAGATE(status, NULL_ARENA, , "null arena.");
    arena->current_offset = 0;
}

void arena_destruct(Status* status, Arena* arena)
{
    if (!arena)
        BACK_PROPAGATE(status, NULL_ARENA, , "null arena.");

#ifdef _WIN32
    if (!VirtualFree(arena->ptr, 0, MEM_RELEASE))
        BACK_PROPAGATE(status, ARENA_VIRTUAL_MEM_FREE_FAILED, ,
            "failed to release virtual mem.");
#else
    if (munmap(arena->ptr, arena->reserved_size) != 0)
        BACK_PROPAGATE(status, ARENA_VIRTUAL_MEM_FREE_FAILED, ,
            "failed to release virtual mem.");
#endif

    free(arena);
}

ScopedArena* scope_arena_init(Status* status, Arena* parent)
{
    if (!parent)
        BACK_PROPAGATE(status, NULL_ARENA, NULL, "null arena.");

    ScopedArena* arena = malloc(sizeof(*arena));
    if (!arena)
        BACK_PROPAGATE(status, MALLOC_FAILED, NULL, "malloc failed.");

    arena->ptr = parent->ptr + parent->current_offset;
    arena->parent = parent;
    arena->original_offset = arena->parent->current_offset;

    return arena;
}

void* scope_arena_allocate(Status* status, ScopedArena* arena, size_t patch_size)
{
    if (!arena)
        BACK_PROPAGATE(status, NULL_SCOPED_ARENA, NULL, "null arena.");

    void* ptr = arena_allocate(status, arena->parent, patch_size);
    if (!STATUS_OK(status))
        return NULL;

    return ptr;
}

void* scope_arena_append_item(Status* status, ScopedArena* arena, size_t item_size, const void* item)
{
    if (!arena)
        BACK_PROPAGATE(status, NULL_SCOPED_ARENA, NULL, "null arena.");

    void* ptr = arena_append_item(status, arena->parent, item_size, item);
    if (!STATUS_OK(status))
        return NULL;

    return ptr;
}

void scope_arena_deallocate(Status* status, ScopedArena* arena)
{
    if (!arena)
        BACK_PROPAGATE(status, NULL_SCOPED_ARENA, , "null arena.");
    if (!arena->parent)
        BACK_PROPAGATE(status, NULL_ARENA, , "null arena.");

    arena->parent->current_offset = arena->original_offset; 
}