#include "misc/hash_str_to_ins.h"
#include "misc/error.h"

typedef struct{
    int age;
    int level;
} RandomKid;

void main()
{
    Status _status;
    status_setup(&_status);
    Status* status = &_status;

    HashStringToIns* hash = hash_str_to_ins_init(status);
    
    int* a = malloc(sizeof(*a));
    if (!a)
        return;
    *a = 67;

    RandomKid* kid = malloc(sizeof(*kid));
    if (!kid)
        return;

    kid->age = 36;
    kid->level = 123;
    
    hash_str_to_ins_add_key(status, &hash, "the kid", kid);
    hash_str_to_ins_add_key(status, &hash, "the num", a);

    // test find key
    HashStringToIns* slot;
    slot = hash_str_to_ins_find(status, &hash, "the kid");
    slot = hash_str_to_ins_find(status, &hash, "the num");

    hash_str_to_ins_destruct(status, &hash);

    free(kid);
    free(a);
}