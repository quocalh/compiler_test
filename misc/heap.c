#include "heap.h"
#include <assert.h>

// [internal func]
bool HeapSucessfullyAllocated(void* ptr)
{
  if (ptr != NULL)
  {
    return true;
  }
  return false;

}
//
Heap* HeapInit(size_t size)
{
  void* ptr = malloc(size * 1);
  return ptr;
}

// [interface] append for heap
bool HeapAdd(Heap* heap, const void* item)
{
    if (heap == NULL || heap->ptr == NULL || item == NULL)
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
}

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


