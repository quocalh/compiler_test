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
Heap* HeapInit(size_t size);
// [internal func]to allocated more space to the heap (grow twice each growth)
bool HeapAdd(Heap* heap, const void* item_ptr);
// free mem
void HeapFree(Heap* heap);


// to create custom ins (slightly faster interface)
// panic quit :D
void* HeapInsInit(size_t size);
void HeapInsFree(void* heap);

#endif

/* USAGE
  void logging(size_t i1, int i2, int i3)
  {
    printf("size: %zu | length: %d | allocated: %d\n",
          i1,
          i2,
          i3);
  }
  
  Heap heap = HeapInit(sizeof(int));
  logging(heap.size, heap.length, heap.allocated_length);

  int x = 5;
  HeapAdd(&heap, &x);
  logging(heap.size, heap.length, heap.allocated_length);

  x = 6;
  HeapAdd(&heap, &x);
  logging(heap.size, heap.length, heap.allocated_length);

  x = 67;
  HeapAdd(&heap, &x);
  logging(heap.size, heap.length, heap.allocated_length);

  x = 69;
  HeapAdd(&heap, &x);
  logging(heap.size, heap.length, heap.allocated_length);

  x = 12;
  HeapAdd(&heap, &x);
  logging(heap.size, heap.length, heap.allocated_length);

  x = 78;
  HeapAdd(&heap, &x);
  logging(heap.size, heap.length, heap.allocated_length);

  printf("Hello world\n");

  printf("%p\n", heap.ptr);

  heap.ptr = ((int*)heap.ptr); // can't do shit bro
  printf("%d\n", ((int*)heap.ptr)[1]);

  free(heap.ptr);
 */
