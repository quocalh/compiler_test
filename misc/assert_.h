#ifndef ASSERT__H
#define ASSERT__H

#include <stdio.h>
#include <assert.h>

// printf("[%s.c: %d]: %s\n", file, __LINE__, debug_string);
// printf("[%s: %d]: %s\n", __FILE__, __LINE__, debug_string);
#define REPORT(debug_string) \
    printf("[%s:%d]: %s\n", __FILE__, __LINE__, debug_string);

#define ASSERT(res, debug_string) \
    do { \
        /* if res is false, then stop instantly*/ \
        res? 1:\
            REPORT(debug_string); \
            assert(res); \
    } while(0);

#define ERROR(debug_string) \
    do { \
        REPORT(debug_string); \
        assert(0); \
    } while(0);

#define ERROR_TE(debug_string) \
    do { \
        printf("[%s:%d]: %s\n", __FILE__, __LINE__, debug_string); \
        assert(0); \
    } while(0);

#define REPORT_VARIADIC(debug_string, ...) \
    printf(debug_string __VA_OPT__(,) __VA_ARGS__); 
    // printf(debug_string, ##__VA_ARGS__)


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
