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

    // primitive boundary
    DATA_PRIMITIVE,

    DATA_STRING,

    DATA_LIST,
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
        // garbage collecing problem (we don't do ref count no?)

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

/*
    lifetime rule:
        primitives (register simulated): the lifetime is in the scope (the current environment)
            the env lifetime ends the whole things end too
            primitive consists of double, int, true, false, nil, 
            (and some of the extensive variable, that i not interested implementing them into the system)
        composites
            these are composite dtype, only eligible for shallow-copy, 
            instead of complete ownership transfer
            
            these dtype are governed by the runtime environment:
                til the iteference of garbage collector, these dtype stay till the end of the program

            countables
                consists of str, array, and list
            instances
                is the one of its own world
            hashes
                specialist, won't be implemented anytime soon
                (i want to do fourier :sob:)
*/

// closures fuck up the lifetime policy of these data type
typedef struct{
    Vector vector;
} List;

typedef struct{
    const Value* array; 
} Array;

typedef struct{
    Environment* env; 
} Function;

typedef struct{
    const char* _name;
    // idk man
} Instance;

typedef struct{
    const char* name;
    bool is_borrowed;
} String; 
/*

string in register should be borrowed entities, 
while the the one in the env, own that string and generate 
when do value string destruct, consider these cases:
    string borrowed from the token (borrow)
    string borrowed from the environment (borrow)
    string generated with complete ownership (owner, shit...)
            string + string;
            string * int
        idea: 
            runtime don't adopt this, only env adopt these
            runtime only adopt 
    
    what should we do when
    outer_str = inner_str; // this is a reference 
    outer_str = inner_str; // this can only be solved 
                              with a tracing garbage collector
    inner_str = outer_str;

we can't answer this as we can't revive the inner_str 
reference for the outer_str 
    => don't allow string concatenate  

will array suffer from the same problem too?
    inner_array + outer_array
technically no, once the create

even if the garbage tracing is here
    im not interested in if (arr1 == arr2) thingy
=>  we don't implement dynamic arithmetic for composite just yet

duh, move on to interprete execute binary and literal testing now

*/

typedef union DataPayLoad{
    int integer;
    double real;

    // just in case, i wont do it for now
    long long* extensive_int;
    long double* extensive_real;

    // we will create file to deal with these, one by one
    const char* string;
    List* list;
    Array* array;
    void* instance; // i don't know you
    Function* function;

    bool boolean;
} DataPayLoad;

struct Value{
    DataType type;
    DataPayLoad payload;
};
typedef struct Value Value;

Value* value_allocate(Status* status);
void value_destruct(Status* status, Value* value);

void print_value(Status* status, Value* value);

void value_primitive_conversion(Status* status, 
    Value* value, DataType desired_type);

DataType value_primitive_common_type(Status* status, 
    DataType type1, DataType type2);

bool value_is_primitive(DataType type);
bool value_is_composite(DataType type);
bool value_is_countable(DataType type);

#endif