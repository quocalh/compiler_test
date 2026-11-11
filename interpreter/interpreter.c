#include "interpreter.h"
#include "../parser/parser.h"
#include "execute.h"
#include "../misc/hash_str_to_ins.h"
#include "../misc/uthash.h"

Interpreter* interpreter_init(Status* status, Vector* _statments)
{
    Interpreter* interpreter = malloc(sizeof(*interpreter));
    if (!interpreter)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        goto ERROR_RETURN;
    }

    // Environment* runtime_env = environment_init(status, NULL);
    // if (!STATUS_OK(status))
    //     goto ERROR_MALLOC;

    // assume that this prog only run once
    // if not, then env is null, will be init in the interpret function
    Environment* env = environment_init(status, NULL);
    if (!STATUS_OK(status))
        // goto ERROR_OMNI_ENV;
        goto ERROR_MALLOC;
    
    *interpreter = (Interpreter) {
        ._statments = _statments,

        .cur_env = env,
        // .runtime_env = runtime_env,

        .current = 0,
    };
    return interpreter;

    ERROR_ENV:
    environment_destruct(status, env);

    // ERROR_OMNI_ENV:
    // environment_destruct(status, runtime_env);
        
    ERROR_MALLOC:
    free(interpreter);

    ERROR_RETURN:
    return NULL;
}

void interpreter_destruct(Status* status, Interpreter* interpreter)
{
    // strict safety check (can be removed for more flexibility)
    if (!interpreter)
    {
        SET_STATUS(status, NULL_INVALID_VALUE, 
            "interpreter cant be null.");
    }

    // check if this is the root env
    if (interpreter->cur_env->enclosing)
    {
        SET_STATUS(status, INTERPRETER_INVALID_ROOT_ENV,
            "this is not the root env.");
    }

    interpreter_env_destruct(status, interpreter->cur_env);
    // interpreter_env_destruct(status, interpreter->runtime_env);

    environment_destruct(status, interpreter->cur_env);
    // environment_destruct(status, interpreter->runtime_env);
    
    free(interpreter); 
}

void interpreter_interpret(Status* status, 
    Interpreter* interpreter)
{
    printf("\n---io output---\n\n");

    // start here
    Stmt** stmts = (Stmt**)(interpreter->_statments->ptr);
    for (int i = 0; i < interpreter->_statments->index; i++)
    {
        Stmt* stmt = stmts[i];
        interpreter_execute_stmt_dispatch(status, interpreter, stmt);
        if (!STATUS_OK(status)) return;
    }

    printf("\n---end---\n\n");
    printf("\n");
}

void interpreter_env_destruct(Status* status, 
    Environment* env)
{
    HashStringToIns* slot, *tmp;
    HASH_ITER(hh, env->map, slot, tmp)
    {
        HASH_DEL(env->map, slot); 
        value_destruct(status, slot->ins);
        free(slot);
    } 
}

// produce the blueprint data, don't modify the value_slot
// a status variable_not_found wrapper of environment_find
Value* interpreter_lookup_variable(Status* status, 
    Interpreter* interpreter, const char* name)
{
    // check the current env
    Value* value = environment_find(status, 
        interpreter->cur_env, name);
    if (!STATUS_OK(status)) return NULL;
    if (value) return value;

    // check the runtime env
    // value = environment_find(status, 
    //     interpreter->runtime_env, name);
    // if (!STATUS_OK(status)) return NULL;
    // if (value) return value;
    
    // can't find anything, return error
    SET_STATUS(status, ENV_VARIABLE_NOT_FOUND, 
        "variable not found "
        "(var name: %s)", 
        name);
    return NULL;
    
}