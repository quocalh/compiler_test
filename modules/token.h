#ifndef TOKEN_C
#define TOKEN_C

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

  // Literals
  IDENTIFIER, STRING, NUMBER,

  // key words (verbs)
  IF, ELSE, 
  AND, NOT,
  SUPER, CLASS, THIS,
  TRUE, FAlSE, 
  FUNC,
  FOR, WHILE,
  RETURN,
  PRINT, // what i thought this is a function? (uhmm, i don't do IO manipulation)
  NIL, 
  LEOF,
} TokenType;

typedef struct
{
  TokenType token_type;
  void* lexeme;
  void* literal_ptr;
  unsigned int line;
} Token;

#endif

