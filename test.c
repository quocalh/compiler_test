#include <stdio.h>
#include "assert.h"

#include "misc/hashmap.h"
#include "modules/scanner.h"
#include "modules/token.h"
// #include <stdlib.h>
// #include "misc/heap.h"

int main()
{
  Scanner scanner = ScannerInit(".tmp/src.txt");
 
  HashStrToInt* item = HashStrToIntFind(&mnemonics, "for");
  if (item)
  {
    printf("yes love train\n");
    printf("key: %s | value: %d\n", item->strkey, item->bucket);
    printf("key: %s | value: %d\n", item->strkey, FOR);
  }

  int success;
  success = ScannerConvertIntoTokens1(&scanner, scanner.fileName);

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

