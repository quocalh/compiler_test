#include <stdbool.h>
#include "parser.h"
// #include "ast.h"
#include "token.h"

// internal functions
Token* peek(Parser* parser, int i)
{
  if (!parser || i >= parser->tokens.length) return NULL;
  return parser->tokens.ptr + i;
}
bool matchType(Parser* parser, int i, int expect)
{
  Token* token = peek(parser, parser->current);
  return (token->TokenType == expect);
}

Token* consume(Parser* parser, int i)
{
  return NULL;
}

void* parserExpression(Parser* parser)
{
  return parserEquality(parser);
}

void* parserEquality(Parser* parser)
{
  // equality → comparison ( ( "!=" | "==" ) comparison )* ;
  void* comp = parserComparison(parser);

  // TODO: NEED SERIOUS OVERHEAD PLANNING, REST TOMMOROW IF U WANT
  while (true)
  {
    bool t = matchType(parser, parser->current, EQUAL_EQUAL);
    t = t || matchType(parser, parser->current, BANG_EQUAL);
    if (!t) break;
    // get the "second" expression
  }
  // stop at the next expression [stop]
  // a == b != c [+] d
  return comp;
}

void* parserComparison(Parser* parser)
{
  // comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
  // i don't want a == b == c as statement though (should i?)
  // comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )? ;
  void* parserTerm(Parser* parser);
 
  while (true)
  {
    bool t = matchType(parser, parser->current, GREATER);
    t = t || matchType(parser, parser->current, GREATER_EQUAL);
    t = t || matchType(parser, parser->current, LESS);
    t = t || matchType(parser, parser->current, LESS_EQUAL);
    if (!t) break;
    // TokenType operator =
  }
  return NULL;
}


