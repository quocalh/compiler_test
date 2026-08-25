#ifndef TOKEN_C
#define TOKEN_C

#include <stdio.h>
#include "../misc/string_handling.h"

typedef enum
{
  // single character tokens
  LEFT_PAREN, RIGHT_PAREN,
  LEFT_BRACE, RIGHT_BRACE,
  COMMA,
  DOT,

  MINUS, PLUS, SLASH, STAR,
  SEMICOLON,

  // One or two characters tokens
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL, 
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,
  LEFT_COMMENT_BRACKET, RIGHT_COMMENT_BRACKET,

  // Literals
  IDENTIFIER, STRING,
  NUMBER,
  INT, FLOAT, DOUBLE,

  // key words (verbs)
  IF, ELSE, 
  AND, NOT,
  SUPER, CLASS, THIS,
  TRUE, FAlSE, 
  FUNC,
  FOR, WHILE,
  RETURN,
  PRINT, // what i thought this is a function?
  NIL, 
  END_OF_FILE,
} TokenType;

typedef struct
{
  TokenType TokenType;
  StaticString lexeme;
  void* literal_ptr;
  unsigned int line;
} Token;

size_t TokenGetNumberEnumSize(TokenType type);

#endif

