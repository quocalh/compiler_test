#include <stdio.h>
// #include "misc/file.h"
#include "modules/scanner.h"

int main()
{
  Scanner scanner = {0, 0, 0};
  ScannerScan_(&scanner, "src.txt");
 
  // FileReadToString_("src.txt");
  printf("Hello world\n");

  ScannerDestruct(&scanner);

  return 0;
}
