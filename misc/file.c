#include "file.h"

#define FILE_MAX_WIDTH 250

/*
 * turning a file into bunch of keywords
 * saved into the heap array (discard all the \n thingy)
 * for example "for i in range(100): \n\t print("hello world")"
 * the res = [for , i, in, range, (, 100, ), [print, (, "hello world", )]]
 * type shii
 * */

char* FileReadToString_(char* fileName)
{
  FILE *fp = fopen("src.txt", "r");

  if (fp == NULL){
    return (char*)0; // equals to NULL
  }


  // finding the end of the file
  fseek(fp, 0, SEEK_END);
  long int length = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  // allocate mem according to the byte length of the file
  char* buffer = (char*)malloc((length + 1) * sizeof(char));
  if (buffer == NULL){
    return (char*)0;
  }
  length = 0;


  char line_buffer[FILE_MAX_WIDTH];

  // input every single character into the buffer
  while (fgets(line_buffer, sizeof(line_buffer), fp))
  {
    for (int i  = 0; line_buffer[i] != '\n'; i++)
    {
      buffer[length] = line_buffer[i];
      length ++;
    }
  }

  // shrink the buffer down
  char* tmp = realloc(buffer, length * sizeof(char));

  if (tmp == NULL){
    printf("i have abs no idea how we encounter this bs\n");
    return (char*)0;
  }
  buffer = tmp;

  // let me test bro
  for (int i = 0; i < length; i++)
  {
    printf("%c", buffer[i]);
  }
  printf("\n");

  fclose(fp);

  return buffer;
}

