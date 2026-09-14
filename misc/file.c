#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define FILE_MAX_WIDTH 250

// DEPRICATED
// things will be all done in scannner.c

/*
 * turning a file into bunch of keywords
 * saved into the heap array (discard all the \n thingy)
 * for example "for i in range(100): \n\t print("hello world")"
 * the res = [for , i, in, range, (, 100, ), [print, (, "hello world", )]]
 * type shii
 * */

char* FileReadToString_(char* fileName)
{
  FILE *fp = fopen(fileName, "r");

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
  buffer[length] = '\0';

  // shrink the buffer down
  char* tmp = realloc(buffer, (length + 1) * sizeof(char));

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


// LLM save me this time
// i forget the \0 thingy but the LLM code handles the file stream way more smoother than what am doing (using fread)
bool ParseFileIntoString_(char* fileName, char** outputArray, int* count) {
    // Open in text mode
    FILE* fp = fopen(fileName, "r"); 
    if (fp == NULL) {
        return false;
    }
    // Get physical file length
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return false;
    }
    long int fileSize = ftell(fp);
    if (fileSize < 0) {
        fclose(fp);
        return false;
    }
    rewind(fp);

    // Allocate memory based on physical size (+1 for '\0')
    // This is a safe upper bound for text files
    char* stream = (char*)malloc(sizeof(char) * fileSize + 1);
    if (stream == NULL) {
        fclose(fp);
        return false;
    }

    // Read the text file
    size_t bytesRead = fread(stream, sizeof(char), fileSize, fp);
    
    stream[bytesRead] = '\0'; 

    fclose(fp);

    // If needed, shrink the buffer to save memory (Optional)
    // if (bytesRead < (size_t)fileSize) {
    //     char* smallerStream = (char*)realloc(stream, bytesRead + 1);
    //     if (smallerStream != NULL) {
    //         stream = smallerStream;
    //     }
    // }

    // Assign outputs
    *outputArray = stream;
    *count = (int)bytesRead;
  
    return true;
}

