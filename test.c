#include <stdio.h>
// #include "misc/file.h"
#include "modules/scanner.h"

int main()
{
  Scanner scanner = {0, 0, 0};
  // ScannerScan_(&scanner, "src.txt");
  printf("Hello\n");
 
  int success;
  success = ScannerConvertIntoTokens(&scanner, "src.txt");

  printf("Hello world\n");
  
  for (int i = 0; i < scanner.tokenCount; i++){
    printf("%d: %d\n", i, scanner.tokens[i].token_type);
    // printf("%d", scanner.tokens[i].token_type);
  }
  printf("\n");

  ScannerDestruct(&scanner);

  return 0;
}
