#include <stdbool.h>
#include <assert.h>
#include "parser.h"
#include "ast.h"
#include "token.h"


bool* PARSER_NIL = NULL;
bool* PARSER_TRUE = NULL;
void** PARSER_FALSE = NULL;

// internal functions
Token* parserpeek(Parser* parser, int i)
{
  if (!parser || i >= parser->tokens.length) return NULL;
  return parser->tokens.ptr + sizeof(Token) * i;
}
bool matchType(Parser* parser, int i, int expect)
{
  Token* token = parserpeek(parser, parser->current);
  return (token->TokenType == expect);
}

bool consume(Parser* parser, int i, TokenType expected)
{
  while(1)
  {
    Token* c = parserpeek(parser, parser->current++);
    if (c->TokenType == expected) return true;
    if (c == NULL) break; // EOF
  }
  return false;
}







// PARSING FUNCTIONS
void* parserExpression(Parser* parser)
{
  printf("hello equality\n");
  return parserEquality(parser);
}

void* parserEquality(Parser* parser)
{
  // equality → comparison ( ( "!=" | "==" ) comparison )* ;
  printf("hello comparison\n");
  void* comp = parserComparison(parser);

  while (true)
  {
    // after "void* comp", the next one is guaranteed to be same or higher class
    Token* token = parserpeek(parser, parser->current);
    if (!token) break;
    TokenType op = token->TokenType;

    bool t = (op == BANG_EQUAL) || \
             (op == EQUAL_EQUAL);
    if (!t) break;
    parser->current++; // skip the op token, to the term

    void* comp2 = parserComparison(parser);
    comp = astInitBinary(comp, op, comp2);
  }
  return comp;
}

void* parserComparison(Parser* parser)
{
  printf("hello term\n");
  // comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
  // i don't want a == b == c as statement though (should i?)
  // comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )? ;
  void* term = parserTerm(parser);
 

  while (true)
  {
    Token* token = parserpeek(parser, parser->current);
    if (!token) break;
    TokenType op = token->TokenType;
    bool t = (op == GREATER) || \
             (op == GREATER_EQUAL) || \
             (op == LESS) || \
             (op == LESS_EQUAL);
    if (!t) break;
    parser->current++; // skip the op token, to the term

    void* term2 = parserTerm(parser);
    term = astInitBinary(term, op, term2);
  }
  return term;
}


void* parserTerm(Parser* parser)
{
  printf("hello factor\n");
  void* factor = parserFactor(parser);
  while (true)
  {
    Token* token = parserpeek(parser, parser->current);
    if (!token) break;
    TokenType op = token->TokenType;
    bool t = (op == MINUS) || \
             (op == PLUS);
    printf("thug back %d %d\n", parser->current, op);
    
    Token* debug = parserpeek(parser, 1);
    printf("%s\n", debug->lexeme.str);
    
    if (!t) break;
    parser->current++; // skip the op token, to the term

    void* factor2 = parserFactor(parser);
    factor = astInitBinary(factor, op, factor2);
  }
  return factor;
}

void* parserFactor(Parser* parser)
{
  printf("hello unary\n");
  void* unary = parserUnary(parser);
  while (true)
  {
    Token* token = parserpeek(parser, parser->current);
    if (!token) break;
    TokenType op = token->TokenType;
    bool t = (op == SLASH) || \
             (op == STAR);
    if (!t) break;
    parser->current++; // skip the op token, to the term

    void* unary2 = parserUnary(parser);
    unary = astInitBinary(unary, op, unary2);
  }
  return unary;
}

void* parserUnary(Parser* parser)
{
  printf("hello literal%d\n", parser->current);
  // unary          → ( "!" | "-" ) unary
  //              | primary ;
  Token* token = parserpeek(parser, parser->current);
  TokenType op = token->TokenType;
  bool t = (op == MINUS) || \
           (op == BANG);
  if (t)
  {
    // inc by one, to scan the next token
    parser->current++; 
    void* unary2 = parserUnary(parser);
    return astInitUnary(op, unary2);
  }
  return parserPrimary(parser);
}

void* parserPrimary(Parser* parser)
{
  printf("hello mem\n");
  // primary        → NUMBER | STRING | "true" | "false" | "nil"
  //                | "(" expression ")" ;
  Token* token = parserpeek(parser, parser->current);
  parser->current++;
  // printf("hello mem%p\n", token);

  TokenType type = token->TokenType;
  printf("hello the world\n");
  switch (type)
  {
    case TRUE:
      return astInitLiteral(type, PARSER_TRUE);
    case FALSE:
      return astInitLiteral(type, PARSER_FALSE);
    case NIL:
      return astInitLiteral(type, PARSER_NIL);

    case FLOAT:
    case INT:
    case STRING:
      return astInitLiteral(type, token->literal_ptr);

    case LEFT_PAREN:;
      // parser->current++; // skip the left paren
      void* c = parserExpression(parser);

      // astVisitorStation* station = initVisitorStation();
      // visitorLoadDebugPrintFunctions(station);
      // int n = 0;
      // acceptSafe(station, c, &n);
      // freeVisitorStation(station);
      
      if (parserpeek(parser, parser->current)->TokenType == RIGHT_PAREN)
      {
        printf("neeat.\n");
        parser->current++;
        return c;
      }
      // else {
      //   assert(0);
      // }
      break;

    default:
      printf("you shouldn't be here no?\n");
      assert(0);
  }

}


// INIT
Parser parserInit()
{
  PARSER_TRUE = malloc(sizeof(*PARSER_TRUE));
  PARSER_FALSE = malloc(sizeof(*PARSER_FALSE));
  PARSER_NIL = malloc(sizeof(*PARSER_NIL));

  if (!PARSER_TRUE  || !PARSER_FALSE || !PARSER_NIL) {printf("[parser.c] init | can't allocate mem for true false nil"); assert(0);}

  *PARSER_TRUE = true;
  *PARSER_FALSE = false;
  *PARSER_NIL = NULL;

  // not yet concern about the root and the tokens
  Parser parser = (Parser){.current = 0};
  return parser;
}

void parserDestruct(Parser* parser)
{

  free(PARSER_TRUE);
  free(PARSER_FALSE);
  free(PARSER_NIL);

  HeapFree(&parser->tokens);
}

