#include "string_handling.h"
#include <string.h>
#include <stdlib.h>

StaticString* StaticStringInit(const char* buffer)
{
  if (buffer == NULL) return NULL;
  
  // create a heap to store actual string (buffer)
  int l = strlen(buffer) + 1;
  char* str = (char*)malloc(l);
  if (!str) return NULL;
  memcpy(str, buffer, l);
  
  // create static string in heap
  StaticString* ptr = malloc(sizeof(*ptr));
  if (!ptr) return NULL;
  
  ptr->length = l - 1;
  ptr->str = str;

  return ptr;
}


StaticString StaticStringSubstring(StaticString* str, int start, int end)
{
  if (str == NULL) return (StaticString){0, 0};
  if (start > end)
  {
    return (StaticString){0, 0};
  }
  if (start < 0 || end >= str->length)
  {
    return (StaticString){0, 0};
  }
  
  int l = end - start + 1;
  char* heap = (char*)malloc((l + 1) * sizeof(char));
  if (!heap) return (StaticString){0, 0};

  // add the \0 at the end
  memcpy(heap, str->str + start, l);
  heap[l] = '\0';

  return (StaticString){l, heap};
}

void StaticStringFree(StaticString* str)
{
  free(str->str);
  free(str);
}
