#ifndef TOKEN
#define TOKEN

typedef enum
{
    // single-char
    LEFT_PAREN, RIGHT_PAREN, 
    LEFT_BRACE, RIGHT_BRACE, 

    PLUS, SIGN, STAR, SLASH, 

    SEMICOLON,

    // value type (we need a value evaluator tree before dealing with evaluating values)
    INT, DOUBLE,
    STRING, 
    TRUE, FALSE, 
    NIL,
    IDENTIFIER, 
    
    // multi-char
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    EQUAL, EQUAL_EQUAL,
    BANG, BANG_EQUAL,

    // key words
    FUNCTION,
    VAR,
    FOR, WHILE, 
    IF, ELSE, 
} TokenType; 

typedef struct
{
    TokenType type;
    const char* lexeme;
    int line;
} Token;
// designing phase

#endif