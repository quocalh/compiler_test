#include <stdio.h>

// #include "misc/file.h"
// #include "misc/file.h"
#include "modules/scanner.h"
// #include <stdlib.h>
// #include "misc/heap.h"

int main()
{ 
  Scanner scanner = ScannerInit("src.txt");
  int success;
  success = ScannerConvertIntoTokens1(&scanner, "src.txt");

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


  return 0; 
}
