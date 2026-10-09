#include "vector.h"
#include <stdlib.h>
#include <string.h>

#include "error.h"

Vector* vector_init(Status* status, size_t size)
{
    Vector* vector = malloc(sizeof(*vector));
    if (!vector)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        return NULL;
    }

    void* ptr = malloc(size * 1);
    if (!ptr)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        return NULL;
    }

    vector->ptr = ptr;
    vector->allocated_length = 1;
    vector->length = 0;
    vector->item_size = size;

    return vector;
}


void vector_double_allocate(Status* status, Vector* vector)
{
    if (vector->allocated_length > VECTOR_SIZE_MAX / 2)
    {
        SET_STATUS(status, VECTOR_CAPACITY_OVERFLOW, "vector capacity overflow.");
        return;
    }

    void* ptr = realloc(vector->ptr, vector->item_size * vector->allocated_length * 2);
    if (!ptr)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        return;
    }

    vector->allocated_length *= 2;
    vector->ptr = ptr;
}


void vector_add(Status* status, Vector* vector, void* item)
{
    if (vector == NULL)
    {
        SET_STATUS(status, NULL_RETURN, "null return."); 
        return;
    }
    if (vector->ptr == NULL)
    {
        SET_STATUS(status, NULL_RETURN, "null vector->ptr return."); 
        return;
    }
    if (!item)
    {
        SET_STATUS(status, VECTOR_NULL_INVALID, "item ptr can't be null."); 
        return;
    }


    if (vector->length >= vector->allocated_length)
    {
        // double allocate
        vector_double_allocate(status, vector);
        if (!STATUS_OK(status))
            return;
    }
    
    // sufficient space to play with
    char* dest = (char*)vector->ptr + vector->item_size * vector->length;
    memcpy(dest, item, vector->item_size);
    vector->length ++;
}


void vector_destruct(Status* status, Vector* vector)
{
    if (!vector)
        return;
    free(vector->ptr);
    free(vector);
}
