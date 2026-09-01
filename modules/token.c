#include "token.h"
#include "../misc/hashmap.h"

HashStrToInt* mnemonics = NULL;

void TokenCreateMnemonicMap(HashStrToInt** mnemonics)
{
  HashStrToIntAdd(mnemonics, "if", IF);
  HashStrToIntAdd(mnemonics, "else", ELSE);
  HashStrToIntAdd(mnemonics, "break", ELSE);

  HashStrToIntAdd(mnemonics, "and", AND);
  HashStrToIntAdd(mnemonics, "not", AND);
  HashStrToIntAdd(mnemonics, "or", OR);

  HashStrToIntAdd(mnemonics, "true", TRUE);
  HashStrToIntAdd(mnemonics, "false", FALSE);

  HashStrToIntAdd(mnemonics, "for", FOR);
  HashStrToIntAdd(mnemonics, "while", WHILE);
}

void TokenClearMnemonicTable(HashStrToInt** mnemonics)
{
  HashStrToIntClear(mnemonics);
}

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

