#include "token.h"

// Token TokenInit(TokenType token_type, char lexeme,
//   char* literal_ptr, unsigned int line)
// {
//   Token token =  {token_type, lexeme, literal_ptr, line};
//   return token;
// }

char* TokenToString_(Token token)
{
  // return "{token_type}" + "{lexeme}" + "{literal}";
  // int length = snprintf(NULL, 0, "%");
  return "nigger";
}

size_t TokenGetNumberEnumSize(TokenType type)
{
  size_t size;
  switch(type)
  {
    case (INT):
      size = sizeof(int);
      break;

    case (FLOAT):
      size = sizeof(float);
      break;

    case (DOUBLE):
      size = sizeof(double);
      break;

    default:
      printf("[token.c]: you shouldn't be here\n");
      size = 0;
      break;
  }
  return size; 
}

