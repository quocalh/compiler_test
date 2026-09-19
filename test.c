#include <stdio.h>
#include "assert.h"

// #include "modules/ast.h"
#include "modules/scanner.h"
// #include "modules/parser.h"
#include "modules/token.h"
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
  int d = 9;

  // create a token tree now (6 + 7 + 8)
  // Binary* obj = astInitBinary(
  //                 astInitLiteral(INT, &a), PLUS, astInitBinary(
  //                   astInitLiteral(INT, &b), PLUS, astInitLiteral(INT, &c)
  //                 )
  //               );


  // (a == b) - (c + d)
  Binary* obj = \
  astInitBinary(
    astInitLiteral(
      EXPRESSION,
      astInitBinary(
        astInitLiteral(INT, &a),
        EQUAL_EQUAL,
        astInitLiteral(INT, &b)
      )
    ),
    MINUS,
    astInitBinary(
        astInitLiteral(INT, &c),
        PLUS,
        astInitUnary(
          MINUS,
          // astInitLiteral(INT, &d)
          NULL
        )
      )
  );

  printf("yes: my address: %p\n", obj);
  visitorLoadDebugPrintFunctions(station);
  int n = 0;
  obj->accept(station, obj, &n);
  // acceptSafe(station, obj, &n);


  visitorLoadFreeFunctions(station);
  obj->accept(station, obj, NULL);

  for (int i = 0; i < scanner.tokens.length; i++)
  {
    // Token* token = &((Token*)scanner.tokens.ptr)[i];
    Token* token = (Token*)scanner.tokens.ptr + i;

    printf("LINE: %d | TYPE: %d | LITERAL: %p | LEXEME: %s \n",
           token->line,
           token->TokenType,
           token->literal_ptr,
           token->lexeme->str);
  }

  parser.tokens = scanner.tokens;

  parser.root = parserExpression(&parser);

  // draw the tree
  printf("draw the tree\n");
  visitorLoadDebugPrintFunctions(station);
  acceptSafe(station, parser.root, &n);
  n = 0;



  // free the tree
  visitorLoadFreeFunctions(station);
  acceptfreeSafe(station, parser.root, NULL);
  // parser.root->accept(station, parser.root, NULL);


  ScannerDestruct(&scanner);
  parserDestruct(&parser);


  return 0;
}

