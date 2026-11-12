#include <stdio.h>
#include <stdlib.h>

#include "misc/error.h"
#include "misc/vector.h"
#include "misc/misc.h"

// only have tested the general cases, more edgy cases are skipped as this project only active for 2 months

int main()
{
    printf("That was so demoralizing. But here I am.\n");
    printf("Hello world!\n");

    Status status_;
    status_setup(&status_);
    Status* status = &status_;

    // generating test cases
    int l = 19;
    int* n = malloc(sizeof(*n) * l);
    if (!n) 
        status_reporting(status);
    for (int i = 0; i < l; i++)
    {
        n[i] = i;
    }

    Vector* vector = vector_init(status, sizeof(*n));
    for (int i = 0; i < l; i++)
    {
        vector_add(status, vector, n + i);

        for (int j = 0; j < vector->length; j++)
        {
            printf("item %d: %d\n", j + 1, AS(int*, vector->ptr)[j]);
        }

        printf("_length_: %d\n", vector->length);
        printf("allocated_space: %d\n", vector->allocated_length);
        printf("\n");
    }
    vector_destruct(status, vector);
    
    free(n);
}