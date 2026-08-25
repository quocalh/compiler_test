
// worst piece of dog shit i ever written
#include "scanner.h"
#include "token.h"
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "../misc/heap.h"
#include "../misc/file.h"
#include "../misc/string_handling.h"
#include "../misc/numbers.h"


#define FILE_MAX_WIDTH 250

Scanner ScannerInit(char* fileName)
{
  Scanner scanner;
  scanner.fileName = fileName;
  scanner.tokens = HeapInit(sizeof(Token));
  ParseFileIntoString_(fileName, &scanner.stream.str, &scanner.stream.length);

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
  return scanner_ptr->stream.str[i];
}

// internal function
bool match(Scanner* scanner_ptr, int i, char expected)
{
  char c = peek(scanner_ptr, i);
  if (c == '\0') return false;
 
  if (scanner_ptr->stream.str[i] != expected)
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

  // printf("LINE: %d | TYPE: %d | LITERAL: %p | LEXEME: %s \n", token.line, token.TokenType, token.literal_ptr, token.lexeme.array);
  // printf("range: %d -> %d | %s \n", scanner_ptr->start, scanner_ptr->current, token.lexeme.array);


  return token;
}

// internal function
bool TokenAdd(Scanner* scanner_ptr, TokenType Type, void* literal_ptr)
{
  Token token = Tokenize(scanner_ptr, Type, literal_ptr);
  HeapAdd(&scanner_ptr->tokens, &token);
  // token = ((Token*)scanner_ptr->tokens.ptr)[scanner_ptr->tokens.length - 1];
  // printf("tikiLINE: %d | TYPE: %d | LITERAL: %p | LEXEME: %s \n", token.line, token.TokenType, token.literal_ptr, token.lexeme.array);
  return true;
}

// Number
TokenType TokenStringNumberGetTokenType(int dot)
{
  return (dot == -1)? INT: DOUBLE;
}

void ScannerNumber(Scanner* scanner_ptr)
{
  // note: literal_ptr points to heap
  // e.g. -> number -> number[type] | float, int, double
  // e.g. -> string -> static string | dynamic string
  // TokenType type = INT;
  // value
  int dot = -1;
  
  // get the range of the number string
  while (true)
  {
    char c = peek(scanner_ptr, scanner_ptr->current);
    if ( (!isdigit(c)) && c!= '.') break;

    // 5. is invalid but 5.0 is not
    if (c == '.')
    {

      if (dot != -1){
        printf("[scanner.c] numbers | you tryna parse an IP or something?\n");
        break;}
      char n = peek(scanner_ptr, scanner_ptr->current + 1); 
      if (!isdigit(n)) {
        printf("[scanner.c] numbers | (lexical handling is suckass) 5. is not allowed but 5.0 is\n\n");
        break;}

      dot = scanner_ptr->current - scanner_ptr->start;

    }
    scanner_ptr->current++;
  }
  scanner_ptr->current--;

  // classify it 
  TokenType type = TokenStringNumberGetTokenType(dot);
  // printf("what: %zu\n", TokenGetNumberEnumSize(type));
  
  // convert it
  void* literal_ptr = malloc(TokenGetNumberEnumSize(type));
  if (literal_ptr == NULL){printf("i love medicine\n"); return;}
  switch (type)
  {
    case (INT):
      NumberStringIntoInt(scanner_ptr->stream.str + scanner_ptr->start, 
        scanner_ptr->current - scanner_ptr->start + 1, literal_ptr);
      // printf("hello world: %d\n", *(int*)literal_ptr);
      break;

    case (DOUBLE):
      NumberStringIntoDouble(scanner_ptr->stream.str + scanner_ptr->start, 
        scanner_ptr->current - scanner_ptr->start + 1, dot,literal_ptr);
      // printf("hello world : %lf\n", *(double*)literal_ptr);
      break;

    default:
      printf("[scanner.c] number | why are you here, you re not a number type\n");
      break;
  }
  

  // for now
  literal_ptr = NULL;

  TokenAdd(scanner_ptr, type, literal_ptr);
 
}

// String
bool ScannerString(Scanner* scanner_ptr)
{
   
  // usually got send here by the '"'
  // Heap literal_ptr = heapInit();
  scanner_ptr->current++;
  int lineAddend = 0;

  while (true)
  {
    bool stop = false;
    char c = peek(scanner_ptr, scanner_ptr->current);
    switch (c)
    {
      case '\0':
        printf("[scanner.c]: string terminated by EOF\n");
        printf("you likely forgot the closing \" (a dangling \" indeed)");
        return false;
        break;

      case '\n':
        lineAddend++;
        printf("[scanner.c]: string - for now we only deal with simple one line string\n");
        return false;
        break;

      // case '\\':
      //  \" -> char "
      //  \n -> line breaker
      //  \t -> tab 
      //  \r -> carriage return (what?)
      //  \  -> line reserver

      case '"':
        stop = true;
        break;

      default:
        
        break;
    }
    if (stop == true) break;

    scanner_ptr->current++;
  }
  
  // give the start and the end of the string -> create the string
    // lexeme
  StaticString lexeme = StaticStringSubstring(&scanner_ptr->stream,
                                              scanner_ptr->start,
                                              scanner_ptr->current);
    // literal copy
  StaticString* literal_ptr = (StaticString*)malloc(sizeof(StaticString));
  if (literal_ptr == NULL) {
    printf("[scanner.c / string]: Saa \
      Se sse sse no yoi yoi yoi \
      Uso ga honto o zenbu nomikonde \
      Se sse ssei no yoi yoi yoi \
      Kimi no sono te de isso raku ni shite\n");
    return false;
  }
  StaticString literal = StaticStringSubstring(&scanner_ptr->stream,
                                               scanner_ptr->start + 1,
                                               scanner_ptr->current - 1);
  memcpy(literal_ptr, &literal, sizeof(StaticString));

  TokenAdd(scanner_ptr, STRING, literal_ptr);

  scanner_ptr->currentLine += lineAddend;

  // current at ending ", try to catch no " next current + 1
  return true;
}

bool ScannerConvertIntoTokens1(Scanner* scanner_ptr, char* fileName)
{
  char* buffer;
  int buffer_length;
  ParseFileIntoString_(fileName, &buffer, &buffer_length);
  scanner_ptr->stream = (StaticString){buffer_length, buffer};

  scanner_ptr->current = 0;
  scanner_ptr->start = 0;
  while (true)
  {
    // reset the start to the current, peek the next char
    char c = peek(scanner_ptr, scanner_ptr->start);

    if (buffer[scanner_ptr->start] == '\0')
    {
      break;
    }

    switch (c)
    {
      case '(':
        TokenAdd(scanner_ptr, LEFT_PAREN, NULL);
        break;

      case ')':
        TokenAdd(scanner_ptr, RIGHT_PAREN, NULL);
        break;

      case '{':
        TokenAdd(scanner_ptr, LEFT_BRACE, NULL);
        break;

      case '}':
        TokenAdd(scanner_ptr, RIGHT_BRACE, NULL);
        break;

      case ',':
        TokenAdd(scanner_ptr, COMMA, NULL);
        break;

      case '.':
        TokenAdd(scanner_ptr, DOT, NULL);
        break;

      case '-':
        TokenAdd(scanner_ptr, MINUS, NULL);
        break;

      case '+':
        TokenAdd(scanner_ptr, PLUS, NULL);
        break;

      case '*':
        TokenAdd(scanner_ptr, STAR, NULL);
        break;

      case ';':
        TokenAdd(scanner_ptr, SEMICOLON, NULL);
        break;

      // FORBIDDEN ZONE
      case '/':
        // a comment //
        if (match(scanner_ptr, scanner_ptr->current + 1, '/'))
        {
          while (!match(scanner_ptr, ++scanner_ptr->current, '\n'));
          scanner_ptr->current--; // (at the char left to \n) to catch the \n next current + 1
          printf("char: %d\n", scanner_ptr->stream.str[scanner_ptr->current - 1]);
        }
        else 
        { // a slash /
          TokenAdd(scanner_ptr, SLASH, NULL);
        }
        break;

      case '!': 
        // !=
        if (match(scanner_ptr, scanner_ptr->current + 1, '='))
        {
          scanner_ptr->current++;
          TokenAdd(scanner_ptr, BANG_EQUAL, NULL);
        }
        // !
        else
        {
          TokenAdd(scanner_ptr, BANG, NULL);
        }
        break;

      case '=':
        // ==
        if (match(scanner_ptr, scanner_ptr->current + 1, '='))
        {
          scanner_ptr->current++;
          TokenAdd(scanner_ptr, EQUAL_EQUAL, NULL);
        }
        // =
        else
        {
          TokenAdd(scanner_ptr, EQUAL, NULL);
        }
        break;

      case '<':
         // <=
        if (match(scanner_ptr, scanner_ptr->current + 1, '='))
        {
          scanner_ptr->current++;
          TokenAdd(scanner_ptr, LESS_EQUAL, NULL);
        }
        // >
        else
        {
          TokenAdd(scanner_ptr, LESS, NULL);
        }
        break;

      case '>':
         // >=
        if (match(scanner_ptr, scanner_ptr->current + 1, '='))
        {
          scanner_ptr->current++;
          TokenAdd(scanner_ptr, GREATER_EQUAL, NULL);
        }
        // >
        else
        {
          TokenAdd(scanner_ptr, GREATER, NULL);
        }
        break;

      // string // get the current to the next " should be good
      case '"':
        bool success = ScannerString(scanner_ptr);
        if (!success) {return 0;}
        break;

      case '\n':
        scanner_ptr->currentLine++;
      case '\t':
      case '\r':
      case ' ':
        break;

      default:
        if (isdigit(c)){
          ScannerNumber(scanner_ptr);
          break;
        }
        printf("[Scanner]: i have never seen those character before (%c)\n", c);
        break;
    }

    scanner_ptr->start = scanner_ptr->current + 1;
    scanner_ptr->current++;

  }
  return 1;
}

void ScannerDestruct(Scanner* scanner_ptr)
{
  // free the static string lexeme
  for (int i = 0; i < scanner_ptr->tokens.length; i++){
    Token* token = &((Token*)scanner_ptr->tokens.ptr)[i];
    StaticStringFree(&token->lexeme);

    switch (token->TokenType)
    {
      case (STRING):
        StaticStringFree(((StaticString*)token->literal_ptr));
        free(token->literal_ptr);
        break;

      default:
        break;
    }
  }
  HeapFree(&scanner_ptr->tokens);
  StaticStringFree(&scanner_ptr->stream);
}

