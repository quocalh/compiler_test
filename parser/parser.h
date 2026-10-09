#ifndef SCANNER
#define SCANNER

typedef struct{
    const char* file_name;
    int current;
} Scanner;

Scanner* scanner_init();
void scanner_destruct(Scanner* scanner);

#endif