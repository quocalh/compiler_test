#ifndef HASH_STR_TO_INS_H
#define HASH_STR_TO_INS_H

#include "uthash.h"

typedef struct{
    const char* name;
    void* ins;
    UT_hash_handle hh;
} HashStringToIns;

#endif