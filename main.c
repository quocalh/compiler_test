#include <stdio.h>

#include "misc/assert_.h"
#include "modules/scanner.h"
#include "misc/hashmap.h"
#include "modules/expression.h"


int main()
{ 
  // System* system = SystemInit("src.txt");
  // Scanner* scanner = ScannerInit(system);
  
  // ScannerScan(scanner, system);

  // ScannerDestruct(scanner);
  // SystemDestruct(system);

  // expression visitor pattern testing (larp polymorphism)
  // let only literal and expression 
  // run accept both, and let's see

  Expression* expression =  ExpExpressionInit(NULL);

  return 0; 
}
