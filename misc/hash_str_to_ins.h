#ifndef HASH_STR_TO_INS_H
#define HASH_STR_TO_INS_H

#include "uthash.h"
#include "../misc/error.h"

typedef struct{
    const char* name; // borrow
    void* ins; // versatile
    UT_hash_handle hh;
} HashStringToIns;

HashStringToIns* hash_str_to_ins_init(Status* status);
void hash_str_to_ins_destruct(Status* status, HashStringToIns** hash);

HashStringToIns* hash_str_to_ins_add_key(Status* status, HashStringToIns** hash, 
    const char* name, void* ins);

HashStringToIns* hash_str_to_ins_find(Status* status, HashStringToIns** hash,
    const char* name);

void hash_str_to_ins_modify(Status* status, HashStringToIns** hash, 
    const char* name, void* ins);


#endif