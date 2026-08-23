#ifndef FILE_H
#define FILE_H
#include <stdio.h>
#include <stdbool.h>


char* FileReadToString_(char* fileName);


bool ParseFileIntoString_(char* fileName, char** outputArray, int* count);

#endif

