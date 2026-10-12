#include "hash_str_to_int.h"

HashStringToInt* hash_str_to_int_init(Status* status)
{
    // HashStringToInt* hash = malloc(sizeof(*hash));
    // if (!hash)
    // {
    //     SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
    //     return NULL;
    // }

    // declare the hash
    // hash = NULL;
    return NULL;
}

void hash_str_to_int_destruct(Status* status, HashStringToInt** hash)
{
    if (!hash)
    {
        SET_STATUS(status, NULL_INVALID_VALUE, "hash can not be null.");
        return;
    }
    
    // free the malloc pair
    HashStringToInt* slot, *tmp;
    HASH_ITER(hh, *hash, slot, tmp)
    {
        HASH_DEL(*hash, slot);
        free(slot);
    }

    // free the hash ptr
    free(*hash);
    *hash = NULL;
}

HashStringToInt* hash_str_to_int_add_key(Status* status, HashStringToInt** hash, const char* key, int value)
{
    HashStringToInt* slot;

    // if already found | return the DUPLICATED KEY FOUND
    if (hash_str_to_int_find(status, hash, key) != NULL)
    {
        SET_STATUS(status, HASH_KEY_DUPLICATION_FOUND, "found a key duplication (key: %s)", key);
        return NULL;
    }
    
    slot = malloc(sizeof(*slot));
    if (!slot)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        return NULL;
    }

    slot->name = key;
    slot->number = value;
    HASH_ADD_KEYPTR(hh, *hash, key, strlen(key), slot);

    return slot;
}

HashStringToInt* hash_str_to_int_find(Status* status, HashStringToInt** hash, const char* key)
{
    HashStringToInt* slot;
    HASH_FIND_STR(*hash, key, slot);
    return slot;
}

void hash_str_to_int_modify(Status*, HashStringToInt** hash, const char* key, int new_value)
{
    // i will write when i use it
    return;
}

// find function but with fallback tolerance
HashStringToInt* hash_str_to_int_get(Status* status, HashStringToInt** hash, const char* key, int fallback_value)
{
    HashStringToInt* slot; 
    slot = hash_str_to_int_find(status, hash, key);

    // if found key
    if (slot != NULL)
    {
        return slot;
    }

    // if not found key, then return with number as fallback value
    return hash_str_to_int_add_key(status, hash, key, fallback_value);
}