#ifndef VALUE_H
#define VALUE_H

#include "environment.h"
#include "../misc/vector.h"
#include "../misc/error.h"

enum DataType{
    DATA_TRUE,
    DATA_FALSE,

    DATA_INT, 
    DATA_DOUBLE, 

    DATA_LIST,
    DATA_STRING,
    DATA_ARRAY,

    DATA_OBJECT,
    DATA_NIL,

    DATA_ERROR_TYPE,
}; 
typedef enum DataType DataType;

/* 
    consider we chose to destruct data everytime we do AST evaluate
    if that is the case, don't use arena
    
    since the value struct is a uniform struct, we can consier the free list approach
    
        the array problem:
            a = [1, 2, 3, 4];
            b = a;
        // should be ok as array is released upon environment destruct

        the array problem #2:
            a = [1, 2, 3, 4, ...]; // 1 billion element
            a = nil;
            // not being referenced anymore
        // should be fine too :D
        // garbage collecing problem

        destruct note:
        var a = 6;
        var b;
        b = a;
        // a won't be destroyed here,
        // distinguish the environment domain value, 
        // and the runtime domain value
        // i mean we do shallow copy

    maybe use union, 
        primitive, normal data here
        extensive data (long long double for example) 
            will use dedicated references (to designated space)
        array and string will use references
        object will use references
    
    NOTE: 
        we are still not sure how deal with value stored in arrays,
        the ownership
*/

// closures fuck up the lifetime policy of these data type
typedef struct{
    Environment* env;
    Vector vector;
} List;

typedef struct{
    Environment* env; 
} Function;

typedef struct{
    // it survives the object lifetime, will be released at the end
    Environment* env;
} String;


typedef struct Value{
    DataType type;

    union DataPayLoad{
        int integer;
        double real;

        // just in case, i wont do it for now
        long long* extensive_int;
        long double* extensive_real;

        // we will create file to deal with these, one by one
        String* string;
        List* list;
        void* instance; // i don't know you
        Function* function;

        bool boolean;
    } payload;

} Value;


#endif