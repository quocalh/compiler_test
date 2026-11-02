#include "environment.h"

Environment* environment_init(Status* status, Environment* enclosing)
{
    Environment* env = malloc(sizeof(*env));
    if (!env)
    {
        SET_STATUS(status, MALLOC_FAILED, 
            "malloc failed.");
        goto FREE_ENV;
    }
    
    HashStringToIns* map = hash_str_to_ins_init(status);
    if (!STATUS_OK(status)) goto FREE_ENV;

    *env = (Environment){
        .enclosing = enclosing,
        .map = map,
    };
    return env;
    
    FREE_ENV:
    free(env);
    return NULL;
}

void environment_destruct(Status* status, Environment* env)
{
    // don't do status check return in destruct funciton
    if (!env)
    {
        SET_STATUS(status, NULL_INVALID_VALUE, 
            "env can't be null.");
        return;
    }

    hash_str_to_ins_destruct(status, &env->map);
    free(env);
}

void environment_define(Status* status, Environment* env, 
    const char* name, Value* value)
{
    hash_str_to_ins_add_key(status, &env->map, name, value);
}

Value* environment_find(Status* status, Environment* env, 
    const char* name)
{
    HashStringToIns* slot = hash_str_to_ins_find(status, &env->map, name);
    if (!STATUS_OK(status)) return NULL;
    if (slot) return (Value*)(slot->ins);

    // fallback into parent's scope (recursively)
    if (env->enclosing)
        return environment_find( status, env->enclosing, name);
    
    SET_STATUS(status, ENV_VARIABLE_NOT_FOUND, 
        "environment variable not found (%s)", name);
    return NULL;
}