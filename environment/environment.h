#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../misc/hash_str_to_ins.h"
#include "../misc/error.h"

typedef struct Value Value;

struct Environment{ 
    struct Environment* enclosing;
    HashStringToIns* map; 
};
typedef struct Environment Environment;


Environment* environment_init(Status* status, Environment* enclosing);
void environment_destruct(Status* status, Environment* env);

void environment_define(Status* status, Environment* env, 
    const char* name, Value* value);

Value* environment_find(Status* status, Environment* env, 
    const char* name);

#endif