#include "numbers.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>

// isdigit
int NumberConvertDigitIntoInt(char c)
{
  switch (c)
  {
    case '0':
      return 0;

    case '1':
      return 1;

    case '2':
      return 2;

    case '3':
      return 3;

    case '4':
      return 4;

    case '5':
      return 5;

    case '6':
      return 6;

    case '7':
      return 7;

    case '8':
      return 8;

    case '9':
      return 9;

  }
  printf("[numbers.c] convert char into int| what have you sent?\n");
  return 0;
}

void NumberStringIntoInt(char* str, int l, int *i)
{
  if (l >= 10){
    printf("[numbers.c] int conversion| value being too large no?\n");
    return;
  }
  if (str == NULL) return;
  int number = 0;

  for (int j = 0; j < l; j++)
  {
    char c = *(str + j);
    if (!isdigit(c)){
      printf("[numbers.c] int conversion| what are you throwing at me (%c)\n", c);
      return;
    }
    number = number * 10 + NumberConvertDigitIntoInt(c);
  }
  (*i) = number;
}

void NumberStringIntoFloat(char* str, int l, int dot, float* f)
{

}

int ipow(int base, int exp)
{
  // e.g. a^67 = a^(2^0 + 2^1 + 2^6) = a^(1 + 2 + 64)
  // combine with the property: bin / 2 -> shift to the right (and read the first bit)
  int res = 1;
  if (exp < 0) return 0;

  while (exp > 0){
    if (exp % 2 == 1){
      res *= base;
    }
    exp /= 2;
    base *= base; 
  }
  return res; 
}

void NumberStringIntoDouble(char* str, int l, int dot, double* d)
{
  if (l >= 15 + 1){ // +1 account for the dot
    printf("[numbers.c] int conversion| value being too large i afraid\n");
    return;
  }
  if (str[dot] != '.'){
    printf("[numbers.c] a slight misaligment in floating paramater (%*s have a dot at %d?)\n", l, str, dot + 1);
    assert(0);
  }
  if (str == NULL) return;
  int number = 0;

  for (int j = 0; j < l; j++)
  {
    char c = *(str + j);
    if (str[j] == '.') continue;

    number = number * 10 + NumberConvertDigitIntoInt(c);
  }
  
  // the pow of math.h is O(1) but has error
  // the O(n) algorithm is boring
  // O(log2(n)) algorithm is kinda overengineering (i love it)
  (*d) = (double)number / (double)ipow(10, l - dot - 1);

}
