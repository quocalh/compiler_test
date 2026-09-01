#include <stdio.h>
#include "assert.h"

#include "modules/ast.h"
#include "modules/scanner.h"
#include "modules/parser.h"
// #include "modules/token.h"

int main()
{
  Scanner scanner = ScannerInit(".tmp/src.txt");
  Parser parser = parserInit();

  int success;
  success = ScannerConvertIntoTokens1(&scanner, scanner.fileName);

  printf("Hello world\n");

  astVisitorStation* station = initVisitorStation();
  visitorLoadDebugPrintFunctions(station);
  
  int a = 6;
  int b = 7;
  int c = 8;

  // create a token tree now (6 + 7 + 8)
  Binary* obj = astInitBinary(
                  astInitLiteral(INT, &a), PLUS, astInitBinary(
                    astInitLiteral(INT, &b), PLUS, astInitLiteral(INT, &c)
                  )
                );
  printf("pray: %d\n", *(int*)((Literal*)((Binary*)obj)->left)->address);
  printf("yes: nigger: %d\n", RIGHT_PAREN);

  printf("yes: my address: %p\n", obj);
  visitorLoadDebugPrintFunctions(station);
  int n = 0;
  obj->accept(station, obj, &n);

  // for (int i = 0; i < scanner.tokens.length; i++)
  // {
  //   Token* token = &((Token*)scanner.tokens.ptr)[i];
  //
  //   printf("LINE: %d | TYPE: %d | LITERAL: %p | LEXEME: %s \n", 
  //          token->line, 
  //          token->TokenType, 
  //          token->literal_ptr, 
  //          token->lexeme.str);
  // }

  ScannerDestruct(&scanner);
  parserDestruct(&parser);


  return 0;
}

