#include <stdio.h>A
#include "misc/arena.h"

// it should be some number (prefer to be divisible by 4)
#define RESERVED_SIZE 120

// 10-byte object testing
typedef struct{
    int age; 
    char name[8];
} TestingStruct;

int main()
{
    Status _status;
    Status* status = &_status;
    status_setup(status);

    // arena init
    Arena* arena = arena_init(status, RESERVED_SIZE);
    if (!STATUS_OK(status))
    {
        status_reporting(status);
        goto ARENA_FREE;
    }
    printf("hello world\n");
    printf("sizeof testing struct: %zu\n", sizeof(TestingStruct));

    // arena capacity testing
    size_t theoretical_max_slot = RESERVED_SIZE / sizeof(TestingStruct);
    printf("maximum allcated slot: %zu\n", theoretical_max_slot);

    int i = 0;
    while (true)
    {
        TestingStruct obj = (TestingStruct){.age = i};

        strcpy(obj.name, "yesiam");
        arena_append_item(status, arena, sizeof(TestingStruct), &obj);
        if (!STATUS_OK(status))
        {
            status_reporting(status);
            break;
        }
        i++;
    }

    // clear the intended error, if not that error
    if (!STATUS_OK(status))
    {
        if (status->code == ARENA_SIZE_OVERFLOW) 
            CLEAR_STATUS(status)
        else 
            goto ARENA_FREE;
    }

    printf("actual allocated slot: %d\n", i);
    size_t actual_max_slot = i;

    // arena object query
    for (int i = 0; i < actual_max_slot; i++)
    {
        TestingStruct* obj = (TestingStruct*)(arena->ptr + i * sizeof(TestingStruct));
        printf("address for debug-mode checking (%dth item): %p\n", i, obj);

        // verification
        if (obj->age != i || strcmp(obj->name, "yesiam") != 0)
        {
            printf("verification failed at %dth obj.\n", i);
        }
    }

    ARENA_FREE:
    arena_destruct(status, arena);
    if (!STATUS_OK(status))
        status_reporting(status);
}