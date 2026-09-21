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

  return 0; 
}
