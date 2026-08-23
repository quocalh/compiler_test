#include "token.h"
#include <stdio.h>

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

void TokenScanToken(Token* token_ptr)
{
  // the operation usually (99%) of the time would be 2 
  // -> if else handling should be enough
  // i just wonder there is a more sophisticate aproach
  // for those (suppose they exist) poses 5 consecutive chars
  char c = token_ptr->lexeme;
  
  switch (c) {
  case '(':
    token_ptr->token_type = LEFT_PAREN;
    break;

  case ')':
    token_ptr->token_type = RIGHT_PAREN;
    break;
  
  case '{':
    token_ptr->token_type = LEFT_BRACE;
    break;

  case '}':
    token_ptr->token_type = RIGHT_BRACE;
    break;

  case ',':
    token_ptr->token_type = COMMA;
    break;

  case '.':
    token_ptr->token_type = DOT;
    break;

  case '-':
    token_ptr->token_type = MINUS;
    break;

  case '+':
    token_ptr->token_type = PLUS;
    break;

  case '/':
    token_ptr->token_type = SLASH;
    break;

  case '*':
    token_ptr->token_type = STAR;
    break;

  case ';':
    token_ptr->token_type = SEMICOLON;
    break;


  default:
    printf("error: i have never seen this character before (%c)\n", c);
    break;
}


}


