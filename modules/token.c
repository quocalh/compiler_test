#include "token.h"

Token TokenInit(TokenType token_type, char lexeme,
  char* literal_ptr, unsigned int line)
{
  Token token =  {token_type, lexeme, literal_ptr, line};
  return token;
}

char* TokenToString_(Token token)
{
  // return "{token_type}" + "{lexeme}" + "{literal}";
  // int length = snprintf(NULL, 0, "%");
  return "nigger";
}




