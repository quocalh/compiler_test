#include <stdio.h>

#include "../misc/assert_.h"
#include "scanner.h"
#include "token.h"
#include "../misc/heap.h"

#define SCANNER "scanner"

Scanner* ScannerInit(System* system)
{
    Scanner* scanner = HeapInsInit(sizeof(*scanner));
    
    *scanner = (Scanner){
        .file_name = system->file_name,
        
        .tokens = system->tokens,

        .start = 0,
        .current = 0,
        .line = 0,
    };
    return scanner;
}

// void ScannerPeek(Scanner* scanner, int)
void ScannerScan(Scanner* scanner)
{
    // read file
    FILE* file = fopen(scanner->file_name, "r");
    ASSERT(file, SCANNER, "can't allocate space for string stream. (file ~ fopen)");
    
    // allocating space for the file string stream
    fseek(file, 0, SEEK_END);
    long int stream_length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // fetch the stream, put into the heap
    char* stream = malloc((stream_length + 1) * sizeof(*stream));
    ASSERT(stream, SCANNER, "can't allocate mem for the string stream.");

    int i = 0;
    char c;
    while ((c = fgetc(file)) != EOF){
        /*
         * skip the escape string
         * example:
         * printf("hello world");
         * 
         * is equivalent to this:
         * printf("hello \
         *        world"); // skips all the ' ', '\t', and '\n' after it
         */
        if (c == '\\'){
            bool skipped = false;
            // skips the ' ', '\t', '\n'
            while (
                   ((c = fgetc(file)) != EOF) &&
                   (c == ' ' || c == '\t' || c == '\n')
                ){skipped = true;}
            
            // if haven't skipped -> pretend nothing happens
            if (!skipped) {
                stream[i++] = '\\';}
        }
        
        stream[i++] = c;
    }
    i--;
    stream[i] = '\0'; i++;
    
    // shrink the stream down to its correct size
    char* tmp = realloc(stream, i * sizeof(*tmp));
    ASSERT(tmp, SCANNER, "can't shrink the allocated string stream.");
    stream = tmp;
    
    // debug this
    printf("debug string\n");
    for (int j = 0; j < i; j++)
    {
        printf("%c", stream[j]);
    }
    printf("\n");
    
    // translating string into tokens, add into token array
    // NOTE: last element should be EOF
    while (false)
    {
        Token new_token;
        scanner->start = scanner->current;
        char c = stream[scanner->start];

        // need planning
        TokenType type;
        switch (c)
        {
            case '(':
                break;
            case ')':
                break;

            case '+':
                break;
            case '-':
                break;
            case '*':
                break;
            
            case '\n':
            case ' ':
                break;
            
            case '\\':
                break;

            default:
                // handling keywords
                // handling var
                break;
        }
    }
    
    
    // collapse
    free(stream);
    fclose(file);
}

void ScannerDestruct(Scanner* scanner)
{
    free(scanner);
}