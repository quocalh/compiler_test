#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include "uthash.h"

// an interface for uthash.h
// either int -> str
// or str -> int (would use this)

// for now let's just make a str -> int one
typedef struct{
  char* strkey;
  int bucket;
  UT_hash_handle hh;
} HashStrToInt;


/*
 * find, add, del, clear, interate
 */

void HashStrToIntInit(HashStrToInt** map);
HashStrToInt* HashStrToIntFind(HashStrToInt** map, char* key);
bool HashStrToIntAdd(HashStrToInt** map, char* key, int value);
// bool HashStrToIntModify(HashStrToInt** map, char* key, int new_value);
// bool HashStrToIntDelete(HashStrToInt** map, char* key);
// bool HashStrToIntIterate(HashStrToInt** map); // not sure one this one how to handle
void HashStrToIntClear(HashStrToInt** map);
// later on, we can try the struct -> struct (no pointer allowed)

#endif
