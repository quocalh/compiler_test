
#include "scanner.h"
#include "token.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define FILE_MAX_WIDTH 250

Scanner ScannerInit(char* source_ptr)
{
  Scanner scanner = {source_ptr};
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
        token.token_type = LEFT_PAREN;
        break;

      case ')':
        token.token_type = RIGHT_PAREN;
        break;

      case '{':
        token.token_type = LEFT_BRACE;
        break;

      case '}':
        token.token_type = RIGHT_BRACE;
        break;

      case ',':
        token.token_type = COMMA;
        break;

      case '.':
        token.token_type = DOT;
        break;

      case '-':
        token.token_type = MINUS;
        break;

      case '+':
        token.token_type = PLUS;
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
          token.token_type = SLASH;
        }

        break;

      case '*':
        token.token_type = STAR;
        break;

      case ';':
        token.token_type = SEMICOLON;
        break;

      case '!':
        // != !
        token.token_type = (
            buffer_peek_next(&i, line_buffer, l, '=')
          )? BANG_EQUAL: BANG;
        break;

      case '=':
        // = ==
        token.token_type = (
            buffer_peek_next(&i, line_buffer, l, '=')
          )? EQUAL_EQUAL: EQUAL;
        break;

      case '<':
        // < <=
        token.token_type = (
            buffer_peek_next(&i, line_buffer, l, '=')
          )? LESS_EQUAL: LESS;
        break;

      case '>':
        // < <=
        token.token_type = (
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
  printf("no please, %d\n", count);

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
  scanner_ptr->sourceLength = currentLine;
  scanner_ptr->tokens = token_array;
  scanner_ptr->tokenCount = count;

  return true;
}

void ScannerDestruct(Scanner* scanner_ptr)
{
  free(scanner_ptr->tokens);
}

