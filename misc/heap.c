#include "heap.h"

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
Heap HeapInit(size_t size)
{
  void* ptr = malloc(size * 1);
  if (!HeapSucessfullyAllocated(ptr))
  {
    Heap heap = {0}; // equals to NULL
    return heap;
  }
  Heap heap = {size, ptr, 0, 1};
  return heap;
}

// [internal func]to allocated more space to the heap (grow twice each growth)
void HeapExpand(Heap* heap)
{
  
}

// [interface] append for heap
bool HeapAdd(Heap* heap, void* item_ptr)
{
  if (heap->ptr == NULL){
    printf("null heap pointer");
    return false;
  }
  // insufficient space -> double the space -> sufficient again
  if (heap->length + 1 > heap->allocated_length)
  {
    heap->ptr = malloc(heap->size * heap->allocated_length * 2);
    // broke
    if (heap->ptr == NULL)
    {
      free(heap->ptr);
      return false;
    }
    heap->allocated_length *= 2;
  }
  // guarantee sufficient -> add an item
  memcpy((char*)heap->ptr + heap->length,
         (char*)item_ptr,
         heap->size);
  heap->length += 1;

  return true;
}

// free mem
void HeapFree(Heap* heap)
{
  free(heap->ptr);
}

void what(){
  printf("thy end is now\n");
}

