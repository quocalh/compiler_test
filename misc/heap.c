#include "heap.h"
#include <assert.h>

void* HeapInsInit(size_t size)
{
    void* ptr = malloc(size);
    if (!ptr) {printf("[heap.c] can't allocate"); assert(0);}
    
    return ptr;
}
void HeapInsFree(void* heap)
{
    free(heap);
}

// [internal func]
bool HeapSucessfullyAllocated(void* ptr)
{
  if (ptr != NULL) return true;
  return false;

}

// Heap Init but not in heap
// sorry for this bullshit
Heap* HeapInit(size_t size)
{
    void* ptr = malloc(size * 1);
    if (!ptr) return NULL;

    Heap* heap = malloc(sizeof(*heap));
    if (!heap) return NULL;

    heap->ptr = ptr;
    heap->size = size;
    heap->length = 0;
    heap->allocated_length = 1;

    return heap;
}
// Heap* HeapInit(size_t size)
// {
//     void* ptr = malloc(size * 1);
//     if (!HeapSucessfullyAllocated(ptr)) return NULL;

//     Heap* heap = HeapInsInit(sizeof(*heap));
//     Heap copy = HeapInit_(size);
    
//     memcpy(heap, &copy, sizeof(Heap));

//     // any debug code pin here
    
//     return heap;
// }


// [interface] append for heap
bool HeapAdd(Heap* heap, const void* item)
{
    if (heap == NULL || heap->ptr == NULL)
        return false;

    if (heap->length >= heap->allocated_length)
    {
        size_t new_length = heap->allocated_length == 0
                        ? 1
                        : heap->allocated_length * 2;

        void* new_ptr = realloc(heap->ptr, new_length * heap->size);
        if (new_ptr == NULL)
            return false;

        heap->ptr = new_ptr;
        heap->allocated_length = new_length;
    }

    void* destination =
        (char*)heap->ptr + heap->length * heap->size;

    memcpy(destination, item, heap->size);
    heap->length++;

    return true;
}

// free mem
void HeapFree(Heap* heap)
{
    free(heap->ptr);
    free(heap);
}

