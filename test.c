#include <stdio.h>
// #include "misc/file.h"
// #include "misc/file.h"
#include "modules/scanner.h"
// #include <stdlib.h>

int main()
{
  // int count;
  // char* ptr;
  // ParseFileIntoString_("src.txt", &ptr, &count);
  // printf("total word count: %d\n", count);
  // printf("address of the heap: %p\n\n", ptr);
  // free((char*)ptr);
  //
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
           token->lexeme.array);
  }

  ScannerDestruct(&scanner);


  return 0;  
}


// int main1()
// {
//   Scanner scanner = {0, 0, 0};
//   // ScannerScan_(&scanner, "src.txt");
//   printf("Hello\n");
//
//   int success;
//   success = ScannerConvertIntoTokens1(&scanner, ".tmp/src.txt");
//
//   printf("Hello world\n");
//
//   for (int i = 0; i < scanner.tokens.length; i++)
//   {
//     Token* token = &scanner.tokens.ptr[i];
//     printf("LINE: %d | TYPE: %d | LITERAL: %p | LEXEME%s \n", token->line, token->TokenType, token->literal_ptr, token->lexeme.array);
//   }
//
//   ScannerDestruct(&scanner);
//
//   return 0;
// }
