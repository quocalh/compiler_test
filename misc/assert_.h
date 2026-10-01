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
        if (!(res)) { \
            REPORT(debug_string); \
            abort(); \
        } \
    } while (0)

#define ERROR(debug_string) \
    do { \
        REPORT(debug_string); \
        assert(0); \
    } while(0);

#define REPORT_VARIADIC(debug_string, ...) \
    do{ \
        printf("[%s:%d]: ", __FILE__, __LINE__); \
        printf(debug_string __VA_OPT__(,)__VA_ARGS__); \
        printf("\n"); \
    } while (0)

#define ERROR_VARIADIC(debug_string, ...) \
    do{ \
        printf("[%s:%d]: ", __FILE__, __LINE__); \
        printf(debug_string __VA_OPT__(,)__VA_ARGS__); \
        printf("\n"); \
        assert(0); \
    } while(0)

#define ASSERT_VARIADIC(res, debug_string, ...) \
    do{ \
        if (!(res)) \
        { \
            printf("[%s:%d]: ", __FILE__, __LINE__); \
            printf(debug_string __VA_OPT__(,)__VA_ARGS__); \
            printf("\n"); \
            assert(0); \
        } \
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
