#include <stdio.h>

#include "../misc/assert_.h"
#include "scanner.h"
#include "token.h"
#include "../misc/string_handling.h"
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

void ScannerAddToken(Scanner* scanner, TokenType type, void* literal)
{
    printf("hello world\n");
    Token token;
    token.lexeme = StaticStringSubstring(&scanner->stream, scanner->start, scanner->current);
    token.line = scanner->line;
    token.literal = literal;
    token.type = type;
    HeapAdd(scanner->tokens, &token);
}

char CharStreamPeek(StaticString* stream, int index)
{
    if (index >= stream->length) ERROR(SCANNER, "peek | given index exceed the available length of the heap array.");
    if (!stream->str) ERROR(SCANNER, "peek | the ptr in the stream is a NULL pointer.");
    // slash-new line behavior here
    return ((char*)stream->str)[index];
}
void ScannerDiscardComment(Scanner* scanner)
{
    while (true){
        char c = CharStreamPeek(&scanner->stream, ++scanner->current);
        if (c == '\n') break;
    }
}
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
    //     if (c == '\\'){
    //         bool skipped = false;
    //         // skips the ' ', '\t', '\n'
    //         while (
    //                ((c = fgetc(file)) != EOF) &&
    //                (c == ' ' || c == '\t' || c == '\n')
    //             ){skipped = true;}
    //         // if haven't skipped (false) -> pretend nothing happens
    //         if (!skipped) {
    //             stream[i++] = '\\';}
    //     }
        stream[i++] = c;
    }
    stream[i] = '\0'; i++;
    // shrink the stream down to its correct size
    char* tmp = realloc(stream, i * sizeof(*tmp));
    ASSERT(tmp, SCANNER, "can't shrink the allocated string stream.");
    stream = tmp;

    // scanner->stream (Static string init) 
    scanner->stream.str = stream;
    scanner->stream.length = i;
    
    // debug this
    printf("debug string\n");
    for (int j = 0; j < i; j++)
    {
        printf("%c", stream[j]);
    }
    printf("\n");
    
    // translating string into tokens, add into token array
    // NOTE: last element should be EOF
    int* start = &scanner->start;
    int* current = &scanner->current;
    int* line = &scanner->line;
    Heap* tokens = scanner->tokens;

    while ((*start) < (scanner->stream).length)
    {
        *start = *current;

        char c = stream[scanner->start];
        // TODO: to recreat the c slash-newline behavior
        // to handling current-start relation
        // add token policy

        // need planning
        
        switch (c)
        {
            case '(':
                ScannerAddToken(scanner, LEFT_PAREN, NULL);
                break;
            case ')':
                ScannerAddToken(scanner, RIGHT_PAREN, NULL);
                break;

            case '+':
                ScannerAddToken(scanner, PLUS, NULL);
                break;
            case '-':
                ScannerAddToken(scanner, MINUS, NULL);
                break;
            case '*':
                ScannerAddToken(scanner, STAR, NULL);
                break;
            
            case '\n':
                (*line)++;
            case '\t':
            case ' ':
                break;
            
            case '/':;
                // peek the next char
                char next = CharStreamPeek(&(scanner->stream), (*current) + 1);

                if (next == '/')
                {
                    ScannerDiscardComment(scanner);
                }
                else{
                    ScannerAddToken(scanner, SLASH, NULL);     
                }
                break;
            
            // string
            case '"':
                break;


            default:
                // handling keywords
                // handling var
                break;
        }
        (*current)++;
    }
    // debug token testing
    for (int i = 0; i < scanner->tokens->length; i++)
    {
        Heap* heap = scanner->tokens;
        Token* token = heap->ptr + i * heap->size;
        // Token token = (heap->ptr)[i];
        printf("Line: %d | TokenTypeID: %d | Lexeme: %s\n", token->line, token->type, token->lexeme);
    }
    
    
    // collapse
    free(stream);
    fclose(file);
}

void ScannerDestruct(Scanner* scanner)
{
    free(scanner);
}