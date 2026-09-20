#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include "uthash.h"

// an interface for uthash.h
// either int -> str
// or str -> int (would use this)

// for now let's just make a str -> int one
typedef struct{
  const char* strkey;
  int bucket;
  UT_hash_handle hh;
} HashStrToInt;


/*
 * find, add, del, clear, interate
 */

void HashStrToIntInit(HashStrToInt** map);
HashStrToInt* HashStrToIntFind(HashStrToInt** map, char* key);
bool HashStrToIntAdd(HashStrToInt** map, char* key, int value); // already do the modification
// bool HashStrToIntModify(HashStrToInt** map, char* key, int new_value); // render this redundant
bool HashStrToIntDelete(HashStrToInt** map, char* key);
void HashStrToIntIterate(HashStrToInt** map); // not sure one this one how to handle
void HashStrToIntFree(HashStrToInt** map);
// later on, we can try the struct -> struct (no pointer allowed)

#endif

/*
int main()
{
  printf("hello world\n");

  HashStrToInt* map = NULL;
  // HashStrToIntInit(&map);
 
 
  HashStrToIntAdd(&map, "ching", 67);
  HashStrToIntAdd(&map, "chong", 123);
  HashStrToIntAdd(&map, "ding", 76);
  HashStrToIntAdd(&map, "dong", 52);

  HashStrToInt* res = HashStrToIntFind(&map, "dong");
  printf("%p\n", res);
  assert(res);
  printf("key: %s | value: %d\n", res->strkey, res->bucket);

  // ding check
  res = HashStrToIntFind(&map, "ding");
  printf("%p\n", res);
  assert(res);
  printf("key: %s | value: %d\n", res->strkey, res->bucket);
  // ding del
  HashStrToIntDelete(&map, "ding");
  // ding check
  res = HashStrToIntFind(&map, "ding");
  printf("%p\n", res);
  // assert(res);
  // printf("key: %s | value: %d\n", res->strkey, res->bucket);

  HashStrToIntIterate(&map);

  HashStrToIntClear(&map);

}

-- IO output
hello world
0x5582730db770
key: dong | value: 52
0x5582730db720
key: ding | value: 76
(nil)
 i have no idea how to .. interface? this. Take this as a code sample, yay.
user id 67: name ching
user id 123: name chong
user id 52: name dong


 * */
