#include <stdio.h>

#include "misc/heap.h"

void logging(size_t i1, int i2, int i3)
{
  printf("size: %zu | length: %d | allocated: %d\n",
         i1,
         i2,
         i3);
}
int main()
{
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

  return 0;
}
