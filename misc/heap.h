#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Heap
{
  size_t size;
  void* ptr;
  unsigned int length;
  unsigned int allocated_length;
} Heap;

// [internal func]: check if we have successfully allocate the space
bool HeapSucessfullyAllocated(void* ptr);
//
Heap HeapInit(size_t size);
// [internal func]to allocated more space to the heap (grow twice each growth)
void HeapExpand(Heap* heap);
// "append" interface for heap
bool HeapAdd(Heap* heap, void* item_ptr);
// free mem
void HeapFree(Heap* heap);



#endif
