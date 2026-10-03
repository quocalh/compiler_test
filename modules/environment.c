#include "environment.h"
#include "expression.h"
#include "statement.h"
#include "../misc/heap.h"
#include "../misc/uthash.h"
#include "../misc/assert_.h"

VarMap* VarMapInit(Environment* env)
{
    env->map = NULL;
}
void VarMapAdd(VarMap** var_map, char* key, void* ptr)
{
    VarMap* item = NULL;
    HASH_FIND_STR(*var_map, key, item);
    if (item == NULL)
    {
        item = (VarMap*)malloc(sizeof(*item));
        ASSERT_VARIADIC(item, "can't allocate slots for varMap (%s)", key); 
        item->name = key;
        HASH_ADD_KEYPTR(hh, *var_map, item->name, strlen(item->name), item);
    }
    item->ptr = ptr; 
}
VarMap* VarMapFind(VarMap** var_map, char* key)
{
    VarMap* item;
    HASH_FIND_STR(*var_map, key, item);
    return item; 
}
void VarMapModify(VarMap** var_map, char* key, void* new_ptr)
{
    VarMap* item;
    HASH_FIND_STR(*var_map, key, item);
    ASSERT_VARIADIC(item, "var_map key not found (%s)", key);
    item->ptr = new_ptr;
}
void VarMapDelete(VarMap** var_map, char* key)
{
    VarMap* item;
    HASH_FIND_STR(*var_map, key, item);
    ASSERT_VARIADIC(item, "var_map key not found (%s)", key);
    HASH_DEL(*var_map, item);
}
void VarMapFree(VarMap** var_map)
{
    VarMap* current;
    VarMap* tmp;
    HASH_ITER(hh, *var_map, current, tmp)
    {
        HASH_DEL(*var_map, current);
    }
}

// Environment* EnvironmentInit(Environment* enclosing)
Environment* EnvironmentInit()
{
    Environment* env = HeapInsInit(sizeof(*env));
    *env = (Environment){
        .map = NULL,
        .enclosing = NULL
    };
    return env;
}

void EnvironmentDestruct(Environment* env)
{
    VarMap* current;
    VarMap* tmp;

    // free env literal
    HASH_ITER(hh, env->map, current, tmp)
    {
        Literal* literal = current->ptr;
        ExpressionFreeLiteral(literal);
        HASH_DEL(env->map, current);
        free(current);
    }

    free(env);
}
void EnvironmentDefine(Environment* env, char* name, Literal* literal)
{
    VarMap* var_map = VarMapFind(&(env->map), name);
    if (!var_map)
        VarMapAdd(&(env->map), name, literal);
    else
        // var_map->ptr = literal;
        ERROR_VARIADIC("you didn't define the var yet (%s)", name);
}
void EnvironmentAssign(Environment* env, char* name, Literal* literal)
{
    // recursive
    VarMap* var_map = VarMapFind(&(env->map), name);
    ASSERT_VARIADIC(var_map, "var name '%s' not yet defined.", name);
    var_map->ptr = literal;
}
Literal* EnvironmentGet(Environment* env,  char* name)
{
    // recursive
    VarMap* var_map = VarMapFind(&(env->map), name);
    ASSERT_VARIADIC(var_map, "var name '%s' not yet defined.", name);
    return var_map->ptr;
}