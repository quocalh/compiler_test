#ifndef ASSERT__H
#define ASSERT__H

#include <assert.h>

#define REPORT(file, debug_string) \
    printf("[%s.c]: %s\n", file, debug_string);

#define ASSERT(res, file, debug_string) \
    do { \
        /* if res is false, then stop instantly*/ \
        res? 1:\
            REPORT(file, debug_string); \
            assert(res); \
    } while(0)

#endif


/*
#define WHITE "main"
int main()
{ 
  printf("hello world\n");
  ASSERT(0, WHITE, "Nigger");
  return 0; 
}
*/ 
