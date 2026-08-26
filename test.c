#include <stdio.h>
#include "assert.h"

#include "misc/hashmap.h"

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

