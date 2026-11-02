#include "hash_str_to_ins.h"
#include <stdbool.h>

HashStringToIns* hash_str_to_ins_init(Status* status)
{
    return NULL;
}

// assume ins's lifetime is handled by someone else
void hash_str_to_ins_destruct(Status* status, HashStringToIns** hash)
{
    if (!hash)
    {
        SET_STATUS(status, NULL_INVALID_VALUE, "hash can't not be null.");
        return;
    }

    if (!*hash)
        return;

    HashStringToIns* slot, *tmp;
    HASH_ITER(hh, *hash, slot, tmp)
    {
        HASH_DEL(*hash, slot);
        free(slot);
    }

    free(*hash);
    *hash = NULL;
}

HashStringToIns* hash_str_to_ins_add_key(Status* status, HashStringToIns** hash, 
    const char* name, void* ins)
{
    HashStringToIns* slot;    

    slot = hash_str_to_ins_find(status, hash, name);
    if (slot)
    {
        SET_STATUS(status, HASH_KEY_DUPLICATION_FOUND, 
            "found a key duplication (key: %s).", name);
        return NULL;
    } 

    slot = malloc(sizeof(*slot));
    if (!slot)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        return NULL;
    }
    
    slot->name = name;
    slot->ins = ins;

    HASH_ADD_KEYPTR(hh, *hash, name, strlen(name), slot);
    return slot;
}

HashStringToIns* hash_str_to_ins_find(Status* status, HashStringToIns** hash,
    const char* name)
{
    HashStringToIns* slot;
    HASH_FIND_STR(*hash, name, slot);
    return slot;
}

void hash_str_to_ins_modify(Status* status, HashStringToIns** hash, 
    const char* name, void* ins)
{
    HashStringToIns* slot;
    HASH_FIND_STR(*hash, name, slot);
    if (!slot)
    {
        SET_STATUS(status, HASH_KEY_NOT_FOUND, 
            "hash key not found (key: %s).", name);
        return;
    }

    slot->ins = ins;
}