#include <stdio.h>
#include <stdlib.h>

#include "misc/error.h"
#include "misc/vector.h"
#include "misc/misc.h"
#include "misc/uthash.h"
#include "misc/hash_str_to_int.h"

/*
    TEST CHECK_PROPAGATE
    APPLY GOTO CYCLE CLEANUP
    DEMONSTRATE THE USE OF HASHSTRINGTOINT INTERFACE
*/
int main()
{

    printf("That was so demoralizing. But here I am.\n");
    printf("Hello world!\n");

    Status status;
    status_setup(&status);

    HashStringToInt* hash = hash_str_to_int_init(&status);
    CHECK_PROPAGATE(&status, CLEANUP);

    // check basic adding key and buckets
    hash_str_to_int_add_key(&status, &hash, "ninja", 67);
    hash_str_to_int_add_key(&status, &hash, "lamp", 333);
    hash_str_to_int_add_key(&status, &hash, "head", 666);
    CHECK_PROPAGATE(&status, CLEANUP);
    
    // check each key
    HashStringToInt* slot;
    slot = hash_str_to_int_find(&status, &hash, "lamp");
    slot = hash_str_to_int_find(&status, &hash, "head");

    // check duplication guard
    hash_str_to_int_add_key(&status, &hash, "ninja", 56);
    CHECK_PROPAGATE(&status, CLEANUP);

    // check each key (no, to check if they skip)
    slot = hash_str_to_int_find(&status, &hash, "lamp");
    slot = hash_str_to_int_find(&status, &hash, "head");
    
    CLEANUP: 
    hash_str_to_int_destruct(&status, &hash);
    CHECK_PROPAGATE(&status, END);
    END:
}