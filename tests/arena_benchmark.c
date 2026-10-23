#include <stdio.h>
#include <stdlib.h>

#include "misc/error.h"
#include "misc/vector.h"
#include "misc/misc.h"
#include "misc/uthash.h"
#include "misc/hash_str_to_int.h"
#include "misc/arena.h"

#include "profiling/stop_watch.h"
#include <unistd.h>

/*
    TEST CHECK_PROPAGATE
    APPLY GOTO CYCLE CLEANUP
    DEMONSTRATE THE USE OF HASHSTRINGTOINT INTERFACE
*/


#define MAX_ARENA_SIZE 1024 * 1024 * 1024 
#define KILO 1024
#define MEGA 1024 * KILO
#define GIGA  1024 * GIGA

typedef struct{
    int level;
    int age;
} A;

typedef struct{
    const char* what;
    char name[10];
    int age; 
    int headphone;
    long int id;
} B;

/*

test's output:
    malloc/free
    allocating...diff_sec: 0 | diff_nsec: 421976
    simulate data flow...diff_sec: 0 | diff_nsec: 2973
    deallocate data...diff_sec: 0 | diff_nsec: 4740

    arena
    allocating...diff_sec: 0 | diff_nsec: 25189
    manipulating...diff_sec: 0 | diff_nsec: 2135
    destructing...diff_sec: 0 | diff_nsec: 7855

conclusion:
    arena allocating => arena is 16.8 times faster
    arena munmap vs single malloc => arena is 0.6 times faster -> 1.66 times slower 

 */
int main()
{
    
    // test perf of malloc (1000 obj A, 50 obj B) vs arena allocator 
    Status status;
    status.code = NO_ERROR;

    // vector
    // raw malloc (vector)
    Vector* v_a = vector_init(&status, sizeof(A)); 
    Vector* v_b = vector_init(&status, sizeof(B));
    CHECK_PROPAGATE(&status, VECTOR_CLEANUP);
    
    // allocation space
    printf("allocating...");
    STOPWATCH
    {
        for (int i = 0; i < 1000; i++)
        {
            A a;
            a.level = 2 * i;
            a.age = 67 + i;
            vector_add(&status, v_a, &a);
        }
        for (int i = 0; i < 50; i++)
        {
            B b;
            b.what = NULL;
            strcpy(b.name, "John");
            b.age = 67 + i;
            b.headphone = 2 * i;
            b.id = 36 - i;
            vector_add(&status, v_b, &b);
        }
    }

    // simulate modification of datas    
    printf("simulate data flow...");
    STOPWATCH
    {
        for (int i = 0; i < 1000; i++)
        {
            A* a = (A*)v_a->ptr + i;
            a->level = 2 * i;
            a->age = 67 + i;
        }
        for (int i = 0; i < 50; i++)
        {
            B* b = (B*)v_b->ptr + i;
            b->what = NULL;
            strcpy(b->name, "John");
            b->age = 36 - i;
            b->headphone = 3 * i;
            b->id = 67 - i;
        }
    }

    VECTOR_CLEANUP:
    // deallocate things 
    printf("deallocate data...");
    STOPWATCH{
        vector_destruct(&status, v_a);
        vector_destruct(&status, v_b);
    }

    printf("\n");

    // arena allocator
    // arena alloc
    Arena* arena = arena_init(&status, 10 * KILO);
    CHECK_PROPAGATE(&status, ARENA_CLEANUP);

    // allocating space
    printf("allocating...");
    A* a_array_ptr;
    B* b_array_ptr;
    STOPWATCH
    {
        a_array_ptr = (A*)(arena->ptr + arena->current_offset);
        for (int i = 0; i < 1000; i++)
        {
            A a;
            a.age = i;
            a.level = i * 2;

            arena_append_item(&status, arena, sizeof(A), &a);
            CHECK_PROPAGATE(&status, ARENA_CLEANUP);
        }

        b_array_ptr = (B*)(arena->ptr + arena->current_offset);
        for (int i = 0; i < 50; i++)
        {
            B b;
            b.age = 18 + i;
            b.headphone = 36 + i;
            b.id = 67 - i;
            strcpy(b.name, "madihu");
            b.what = NULL;

            arena_append_item(&status, arena, sizeof(B), &b);
            CHECK_PROPAGATE(&status, ARENA_CLEANUP);
        }
    }

    // manipulating datas
    printf("manipulating...");
    STOPWATCH
    {
        for (int i = 0; i < 1000; i++)
        {
            A* a = a_array_ptr + i;
            a->level = 2 * i;
            a->age = 67 + i;
        }

        for (int i = 0; i < 50; i++)
        {
            B* b = b_array_ptr + i;
            b->what = NULL;
            strcpy(b->name, "John");
            b->age = 36 - i;
            b->headphone = 3 * i;
            b->id = 67 - i;
        }
    }

    // arena destruct
    ARENA_CLEANUP:
    printf("destructing...");
    STOPWATCH
    {
        arena_destruct(&status, arena);
    }

    CLEANUP:
        
}