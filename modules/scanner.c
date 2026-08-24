
// worst piece of dog shit i ever written
#include "scanner.h"
#include "token.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../misc/heap.h"
#include "../misc/file.h"
#include "../misc/string_handling.h"


#define FILE_MAX_WIDTH 250

Scanner ScannerInit(char* fileName)
{
  Scanner scanner;
  scanner.fileName = fileName;
  scanner.tokens = HeapInit(sizeof(Token));
  ParseFileIntoString_(fileName, &scanner.stream.array, &scanner.stream.length);

  scanner.current = 0;
  scanner.start = 0;
  scanner.currentLine = 1;
  return scanner;
}


bool buffer_peek_next(int* i, char* buffer, int buffer_size, char expected)
{
  // return true if buffer[i + 1] = expected
  // at the same time, i++ in advance (skip the accepted char)
  int i_1 = (*i) + 1;
  if (i_1 >= buffer_size)
  {
    return false;
  }
  if (buffer[i_1] != expected)
  {
    return false;
  }
  (*i)++;
  return true;
}

bool ScannerConvertIntoTokens(Scanner* scanner_ptr, char* fileName)
{ 
  FILE* fp = fopen(fileName, "r");
  
  // get length of the file
  fseek(fp, 0, SEEK_END);
  long int length = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  
  // heap array
  Token* token_array = (Token*)malloc(sizeof(Token) * (length + 1));
  if (token_array == NULL){
    return false;
  }

  char line_buffer[FILE_MAX_WIDTH];
  int count = 0;
  int currentLine = 0;
  // for line in the files 
  // (god forgive me i can't help myself)
  while (fgets(line_buffer, sizeof(line_buffer), fp))
  {
    printf("line buffer: %s", line_buffer);

    // scaning tokens
    int l = strlen(line_buffer);
    int i = 0;
    currentLine ++;
    while (i < l)
    {
      char c = line_buffer[i]; 
      Token token;
      bool isTokenExist = true;
      switch (c)
      {

      case '(':
        token.TokenType = LEFT_PAREN;
        break;

      case ')':
        token.TokenType = RIGHT_PAREN;
        break;

      case '{':
        token.TokenType = LEFT_BRACE;
        break;

      case '}':
        token.TokenType = RIGHT_BRACE;
        break;

      case ',':
        token.TokenType = COMMA;
        break;

      case '.':
        token.TokenType = DOT;
        break;

      case '-':
        token.TokenType = MINUS;
        break;

      case '+':
        token.TokenType = PLUS;
        break;

      // //
      case '/':
        bool isComment = buffer_peek_next(
            &i, line_buffer, l, '/');
        if (isComment)
        {
          i = l;// force stop next iteration
          isTokenExist = false;
          break;
        }
        else
        {
          token.TokenType = SLASH;
        }

        break;

      case '*':
        token.TokenType = STAR;
        break;

      case ';':
        token.TokenType = SEMICOLON;
        break;

      case '!':
        // != !
        token.TokenType = (
            buffer_peek_next(&i, line_buffer, l, '=')
          )? BANG_EQUAL: BANG;
        break;

      case '=':
        // = ==
        token.TokenType = (
            buffer_peek_next(&i, line_buffer, l, '=')
          )? EQUAL_EQUAL: EQUAL;
        break;

      case '<':
        // < <=
        token.TokenType = (
            buffer_peek_next(&i, line_buffer, l, '=')
          )? LESS_EQUAL: LESS;
        break;

      case '>':
        // < <=
        token.TokenType = (
            buffer_peek_next(&i, line_buffer, l, '=')
          )? GREATER_EQUAL: GREATER;
        break;

      case '\n': 
        currentLine++;
      case ' ':
      case '\t':
        isTokenExist = false;
        break;

      default:
        printf("error: i have never seen this character before (%c)\n", c);
        return false;

      }

      if (isTokenExist)
      {
        token.line = currentLine;
        // printf("hello: %d\n", token.token_type);
        token_array[count] = token;
        count++;
        printf("%d LINE: %d | char: %c | count: %d\n", i, currentLine, line_buffer[i], count);
      }
      i++;
    }
  }

  // shrink the heap array down   
  Token* tmp = (Token*)malloc(sizeof(Token) * count);
  if (tmp == NULL)
  {
    printf("i have no idea.\n");
    return 0;
  }
  token_array = tmp;
  

  fclose(fp);
  
  scanner_ptr->fileName = fileName;
  // scanner_ptr->sourceLength = currentLine;
  // scanner_ptr->tokens = token_array;
  // scanner_ptr->tokenCount = count;

  return true;
}

// internal function
char advance(Scanner* scanner_ptr, char* buffer, int buffer_length)
{
  // return buffer[i], automatically inc i by 1
  // return \0 if found nothing
  
  if (scanner_ptr->start >= buffer_length)
  {
    return '\0';
  }
  return buffer[scanner_ptr->start];
}

// internal function
// char peek(int i, char* buffer, int buffer_length, int offset) // offset = 1 (default)
char peek(Scanner* scanner_ptr, int i)
{
  if (i >= scanner_ptr->stream.length)
  {
    return '\0';
  }
  // printf("voila: %c %d\n", scanner_ptr->stream.array[i], scanner_ptr->stream.array[i]);
  return scanner_ptr->stream.array[i];
}

// internal function
bool match(Scanner* scanner_ptr, int i, char expected, int offset)
{
  char c = peek(scanner_ptr, i + offset);
  if (c == '\0') return false;
 
  if (scanner_ptr->stream.array[i] != expected)
  {
    return false;
  }
  return true;
}

// internal function
Token Tokenize(Scanner* scanner_ptr, TokenType Type, void* Literal)
{
  Token token;

  token.TokenType = Type;
  token.literal_ptr = Literal;
  token.lexeme = StaticStringSubstring(&scanner_ptr->stream, scanner_ptr->start, scanner_ptr->current);
  token.line = scanner_ptr->currentLine;

  printf("LINE: %d | TYPE: %d | LITERAL: %p | LEXEME: %s \n", token.line, token.TokenType, token.literal_ptr, token.lexeme.array);
  // printf("range: %d -> %d | %s \n", scanner_ptr->start, scanner_ptr->current, token.lexeme.array);


  return token;
}

// internal function
bool TokenAdd(Scanner* scanner_ptr, TokenType Type, void* Literal)
{
  Token token = Tokenize(scanner_ptr, Type, Literal);
  HeapAdd(&scanner_ptr->tokens, &token);
  // token = ((Token*)scanner_ptr->tokens.ptr)[scanner_ptr->tokens.length - 1];
  // printf("tikiLINE: %d | TYPE: %d | LITERAL: %p | LEXEME: %s \n", token.line, token.TokenType, token.literal_ptr, token.lexeme.array);
  return true;
}

bool ScannerConvertIntoTokens1(Scanner* scanner_ptr, char* fileName)
{
  char* buffer;
  int buffer_length;
  ParseFileIntoString_(fileName, &buffer, &buffer_length);
  scanner_ptr->stream = (StaticString){buffer_length, buffer};

  scanner_ptr->current = -1;
  scanner_ptr->start = -1;
  while (true)
  {
    // reset the start to the current, peek the next char
    scanner_ptr->start = scanner_ptr->current + 1;
    scanner_ptr->current++;
    char c = peek(scanner_ptr, scanner_ptr->start);

    if (buffer[scanner_ptr->start] == '\0')
    {
      break;
    }

    switch (c)
    {
      case '(':
        TokenAdd(scanner_ptr, LEFT_PAREN, NULL);
        Token token = ((Token*)scanner_ptr->tokens.ptr)[scanner_ptr->tokens.length - 1];
        printf("LINE: %d | TYPE: %d | LITERAL: %p | LEXEME: %s \n", token.line, token.TokenType, token.literal_ptr, token.lexeme.array);

        break;

      case ')':
        TokenAdd(scanner_ptr, RIGHT_PAREN, NULL);
        break;

      case '\n':
        scanner_ptr->currentLine++;
      case ' ':
        break;

      default:
        printf("[Scanner]: i have never seen those character before\n");
        break;
    }
  }
  return 1;
}

void ScannerDestruct(Scanner* scanner_ptr)
{
  // free the static string lexeme
  for (int i = 0; i < scanner_ptr->tokens.length; i++){
    Token* token = &((Token*)scanner_ptr->tokens.ptr)[i];
    StaticStringFree(&token->lexeme);
  }
  HeapFree(&scanner_ptr->tokens);
  StaticStringFree(&scanner_ptr->stream);
}

