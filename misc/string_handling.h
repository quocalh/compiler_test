#ifndef STRING_HANDLING_H
#define STRING_HANDLING_H

typedef struct{
  int length;
  char* str;
} StaticString;

StaticString* StaticStringInit(const char* buffer);

StaticString* StaticStringSubstring(StaticString* str, int start, int end);

void StaticStringFree(StaticString* str);

#endif
