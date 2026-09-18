#include "../misc/string_handling.h"

typedef enum {
    // evil
    LEFT_PAREN, RIGHT_PAREN, 
    LEFT_BRACE, RIGHT_BRACE,

    // arithmetic operations
    COMMA, DOT, MINUS, PLUS, 
    STAR, SLASH, BACKSLASH,

    SEMICOLON, 

    // literal
    NUMBER, STRING, 
    TRUE, FALSE, NIL,


    // truth operations
    AND, OR, 
    EQUAL, EQUAL_EQUAL, 
    BANG, BANG_EQUAL, 
    LESS, LESS_EQUAL,
    GREATER, GREATER_EQUAL, 

    // key word
    IF, FOR, WHILE, 
    VAR, 

    RETURN, 
} TokenType;
/*
 * I KNOW IM NOT DOING INT FLOAT SEPERATION, 
 * I WANT TO SEE MY TREE WALK INTEPRETER DOES WORK 
 */

typedef struct{
    TokenType type;
    StaticString lexeme;
    void* literal;
    int line;
} Token;