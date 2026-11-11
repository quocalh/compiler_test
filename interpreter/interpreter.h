#ifndef INTERPRET_H
#define INTERPRET_H

#include "../environment/environment.h"
#include "../misc/vector.h"
#include "../environment/value.h"

typedef struct{
    // this later one can be removed to perform
    // statement parsing and interpret iterate
    Vector* _statments;

    // a tracing garbage collector will  be implemented, no need for an external env
    // Environment* runtime_env;
    Environment* cur_env;

    // can be disposed if do parsing and interpret at the same time
    size_t current;

} Interpreter;

Interpreter* interpreter_init(Status* status, Vector* _statments);
void interpreter_destruct(Status* status, Interpreter* interpreter);

void interpreter_interpret(Status* status, 
    Interpreter* interpreter);

void interpreter_env_destruct(Status* status, 
    Environment* env);

Value* interpreter_lookup_variable(Status* status, 
    Interpreter* interpreter, const char* name);

#endif