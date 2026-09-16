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
  if (ptr != NULL)
  {
    return true;
  }
  return false;

}

Heap* HeapInit(size_t size)
{
    void* ptr = malloc(size * 1);
    if (!HeapSucessfullyAllocated(ptr))
    {
        Heap heap = {0}; // equals to NULL
        return NULL;
    }
    Heap* heap = HeapInsInit(sizeof(*heap));
    heap->ptr = ptr;
    heap->size = size;
    heap->length = 0;
    heap->allocated_length = 1;
    
    return heap;
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
// bool HeapAdd(Heap* heap, const void* item_ptr)
// {
//   if (heap->ptr == NULL){
//     printf("null heap pointer");
//     return false;
//   }
//   // insufficient space -> double the space -> sufficient again
//   if (heap->length + 1 > heap->allocated_length)
//   {
//     void* tmp = realloc(heap->ptr, heap->size * heap->allocated_length * 2);
//     if (tmp == NULL)
//     {
//       return false;
//     }
//     heap->ptr = tmp; 
//     heap->allocated_length *= 2;
//   }
//   // guarantee sufficient -> add an item
//   memcpy((char*)heap->ptr + heap->length * heap->size,
//          (char*)item_ptr,
//          heap->size);
//   heap->length += 1;

//   return true;
// }

// free mem
void HeapFree(Heap* heap)
{
    free(heap->ptr);
    free(heap);
}

