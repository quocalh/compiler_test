#include <stdio.h>
// #include "misc/file.h"
// #include "misc/file.h"
// #include "misc/numbers.h"
#include "modules/scanner.h"
// #include <stdlib.h>

int main()
{
  Scanner scanner = ScannerInit(".tmp/src.txt");
  int success;
  success = ScannerConvertIntoTokens1(&scanner, ".tmp/src.txt");

  printf("Hello world\n");
  
  for (int i = 0; i < scanner.tokens.length; i++)
  {
    // void* pointer thingy, bear through it
    Token* token = &((Token*)scanner.tokens.ptr)[i];

    printf("LINE: %d | TYPE: %d | LITERAL: %p | LEXEME: %s \n", 
           token->line, 
           token->TokenType, 
           token->literal_ptr, 
           token->lexeme.str);
  }

  ScannerDestruct(&scanner);

  // int res;
  // NumberStringIntoInt("12345", 5, &res);
  // printf("res: %d \n", res);
  //
  // double dre;
  // NumberStringIntoDouble("123.123", 7, 3, &dre);
  // printf("res: %lf \n", dre);

  return 0;  
}

