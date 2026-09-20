#include <stdio.h>

#include "misc/assert_.h"
#include "modules/scanner.h"
#include "misc/hashmap.h"

#define WHITE "WHITE"

int main()
{ 
  System* system = SystemInit("src.txt");
  Scanner* scanner = ScannerInit(system);
  
  ScannerScan(scanner, system);

  ScannerDestruct(scanner);
  SystemDestruct(system);
  
  // HashStrToInt* hash = NULL;

  // void* tmp = NULL;

  // HashStrToIntAdd(&hash, "ching", 67);
  // tmp = NULL;
  // HashStrToIntAdd(&hash, "chong", 123);
  // tmp = NULL;
  // HashStrToIntAdd(&hash, "ding", 67);
  // tmp = NULL;
  // HashStrToIntAdd(&hash, "ching", 123);
  // tmp = NULL;

  return 0; 
}
