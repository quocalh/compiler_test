#ifndef VECTOR
#define VECTOR

#include <stdio.h>
#include <stdint.h>
#include "error.h"

#define VECTOR_SIZE_MAX SIZE_MAX

typedef struct{
    void* ptr;    
    size_t item_size;

    int length;
    int allocated_length;
} Vector;

Vector* vector_init(Status* status, size_t size);
void vector_double_allocate(Status* status, Vector* vector);
void vector_add(Status* status, Vector* vector, void* item);
void vector_destruct(Status* status, Vector* vector);

#endif 