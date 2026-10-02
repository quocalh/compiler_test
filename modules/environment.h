#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "expression.h"
#include "../misc/hashmap.h"

typedef struct 
{
    const char* name; 
    void* ptr;
    UT_hash_handle hh; 
} VarMap;

typedef struct
{
    VarMap* map;
    void* enclosing;
} Environment;

VarMap* VarMapInit(Environment* env);
VarMap* VarMapFind(VarMap** var_map, char* key);
void VarMapAdd(VarMap** var_map, char* key, void* ptr);
void VarMapModify(VarMap** var_map, char* key, void* new_ptr);
void VarMapDelete(VarMap** var_map, char* key);
void VarMapFree(VarMap** var_map);

Environment* EnvironmentInit();
void EnvironmentDestruct(Environment* env);

void EnvironmentDefine(Environment* env, char* name, Literal* literal);
void EnvironmentAssign(Environment* env, char* name, Literal* literal);
Literal* EnvironmentGet(Environment* env,  char* name);

#endif

/*
note that we allow pre-defined functions
however, "pre-defined", more like undefined var are unacceptable

for example: 

acceptable:
function odd(number)
{
    return even(number); > reaching this define new func (predefine as NULL ptr, that's ok)
}
function even(number) > reaching this, immediate fill the definition of the func
{
    return odd(number);
}

unacceptable:
hello = "too soon bro";
var hello = "too late";

assert(0) immediately

 */