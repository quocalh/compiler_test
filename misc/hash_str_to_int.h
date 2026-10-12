#ifndef HASH_STR_TO_INT
#define HASH_STR_TO_INT

#include "uthash.h"
#include "error.h"

/*
remember we want **users not *users
    Passing the hash pointer into functions
    https://troydhanson.github.io/uthash/userguide.html#_add_item
test field

    > test if object is hard copy or shallow copy (unlikely)
        > inspect the retrieved key address
        > try modify the name and see how it goes
    ANSWER: yeah, everything only a shallow copy (give the int -> return the pair address mechanic)

    > test if the key is hard copy or shallow copy
        > create a key -> free that key -> see what is return (NULL or old address)
    ANSWER: would return an invalid ptr anyways
*/


/*
 *  name's lifetime is managed by another instance
 *  hashmaps are only the interfaces to interact with
 */
typedef struct
{
    const char* name; 
    int number;
    UT_hash_handle hh;
} HashStringToInt;

HashStringToInt* hash_str_to_int_init(Status* status);
void hash_str_to_int_destruct(Status* status, HashStringToInt** hash);
HashStringToInt* hash_str_to_int_add_key(Status* status, HashStringToInt** hash, const char* key, int value);
HashStringToInt* hash_str_to_int_find(Status* status, HashStringToInt** hash, const char* key);
void hash_str_to_int_modify(Status*, HashStringToInt** hash, const char* key, int new_value);
HashStringToInt* hash_str_to_int_get(Status* status, HashStringToInt** hash, const char* key, int fallback_value);

#endif

/*
    // TEST USED TO TEST, ANSWER THOSE ABOVE QUESTIONS
    // test deep copy or not
    Hash* hash = NULL;
    Hash a;

    const char* sample = "nagger";
    const char* similar = "nagger";

    a.name = sample;
    a.number = 67;


    HASH_ADD_KEYPTR(hh, hash, sample, strlen(sample), &a);

    Hash* find;
    HASH_FIND_STR(hash, "nagger", find);
    if (find)
    {
        a.number = 68;
        printf("hello world\n");
    }
    
*/