
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>


#define FILE_MAX_WIDTH 250

Scanner ScannerInit(char* source_ptr)
{
  Scanner scanner = {source_ptr};
  return scanner;
}

bool ScannerScan_(Scanner* scanner, char* fileName)
{
  FILE *fp = fopen(fileName, "r");
  if (fp == NULL){
    return false;
  }

  // finding the end of the file
  fseek(fp, 0, SEEK_END);
  long int length = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  // allocate mem according to the byte length of the file
  Token* tokens_ptr = (Token*)malloc((length + 1) * sizeof(Token));
  if (tokens_ptr == NULL){
    return false;
  }
  length = 0;


  char line[FILE_MAX_WIDTH];

  // input every single character into the buffer
  int currentLine = 0;
  while (fgets(line, sizeof(line), fp))
  {
    for (int i  = 0; line[i] != '\n'; i++)
    {
      char c = line[i];
      Token token = {0, c, 0, currentLine};
      tokens_ptr[length] = token;
      length ++;
    }
    currentLine++;
    printf("i was here\n");
  }

  // shrink the buffer down
  Token* tmp = realloc(tokens_ptr, length * sizeof(Token));
  if (tmp == NULL){
    printf("i have abs no idea how we encounter this bs\n");
    return false;
  }
  tokens_ptr = tmp;

  // let me test bro
  for (int i = 0; i < length; i++)
  {
    // printf("%c", tokens_ptr[i]);
    printf("%c | %d\n", tokens_ptr[i].lexeme, tokens_ptr[i].line);
  }
  printf("\n");
 
  // return value into the scanner
  scanner->tokens_ptr = tokens_ptr;
  scanner->sourceLength = length;
  scanner->fileName = fileName;

  fclose(fp);

  return true;
}

bool ScannerConvertIntoTokens(Scanner* scanner_ptr, char c)
{ 
  return 1;
}


bool isAtEnd(Scanner *scanner, int current_line)
{
  // if (scanner) return false;
}

bool ScannerScanToken(Token* token_ptr)
{
  switch (token_ptr->lexeme){

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
      printf("error: i have never seen this character before (%c)\n", token_ptr->lexeme);
      return false;

  }
  return true;
    
}

void ScannerDestruct(Scanner* scanner_ptr)
{
  free(scanner_ptr->tokens_ptr);
}

