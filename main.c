#include <stdio.h>

#include "misc/assert_.h"
#include "modules/scanner.h"

#define WHITE "WHITE"

int main()
{ 
  printf("hello world\n");
  // System* system = SystemInit("src.txt");
  // Scanner* scanner = ScannerInit(system);
  
  // ScannerScan(scanner);

  // ScannerDestruct(scanner);
  // SystemDestruct(system);

  Heap* heap = HeapInit(sizeof(char));
  char* tmp = "nagger";
  HeapAdd(heap, tmp + 0);
  HeapAdd(heap, tmp + 1);
  HeapAdd(heap, tmp + 2);
  HeapAdd(heap, tmp + 3);
  HeapAdd(heap, tmp + 4);
  HeapAdd(heap, tmp + 5);
  HeapAdd(heap, tmp + 6);
  // *((char*) heap->ptr + 0)
  // HeapAdd(heap, tmp + 7);

  printf("hello world\n");

  char* buffer = (char*)heap->ptr;
  for (size_t i = 0; i < heap->length; i++) {
      // Use %c for characters, or %d if you want to see the ASCII code (e.g., 'n' is 110)
      printf("Index %zu: %c\n", i, buffer[i]); 
  }

  HeapFree(heap);

  return 0; 
}
