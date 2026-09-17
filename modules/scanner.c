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
        .line = 1,
    };
    return scanner;
}

void ScannerAddToken(Scanner* scanner, TokenType type, void* literal)
{
    Token token;
    token.lexeme = StaticStringSubstring(&scanner->stream, scanner->start, scanner->current);
    token.line = scanner->line;
    token.literal = literal;
    token.type = type;
    HeapAdd(scanner->tokens, &token);
}

char CharStreamPeek(Scanner* scanner)
{
    int current = scanner->current;
    StaticString* stream = &(scanner->stream);

    if (current >= stream->length) return '\0';
    if (!stream->str) ERROR("peek | the ptr in the stream is a NULL pointer.");

    // slash-newline skipping behavior
    int skipped_index = current;

    const char* str = (const char *)stream->str;
    if (str[skipped_index] == '\\')
    {
        do{
            if (str[++skipped_index] == '\n') scanner->line++;
        }
        while(str[skipped_index] == ' '  ||
              str[skipped_index] == '\t' || 
              str[skipped_index] == '\n' );

        if (skipped_index > current + 1){
            scanner->current = skipped_index;
        }
    }

    if (scanner->current >= stream->length) ERROR("peek | given skipped index exceed the available length of the heap array.");
    return ((char*)stream->str)[scanner->current];
}
void ScannerDiscardComment(Scanner* scanner)
{
    while (true){
        (++(scanner->current));
        char c = CharStreamPeek(scanner);
        if (c == '\0') break;
        if (c == '\n') {++scanner->line;break;}
    }
}
void ScannerScan(Scanner* scanner)
{
    // read file
    FILE* file = fopen(scanner->file_name, "r");
    ASSERT(file, "can't allocate space for string stream. (file ~ fopen)");
    
    // allocating space for the file string stream
    fseek(file, 0, SEEK_END);
    long int stream_length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // fetch the stream, put into the heap
    char* stream = malloc((stream_length + 1) * sizeof(*stream));
    ASSERT(stream, "can't allocate mem for the string stream.");

    int i = 0;
    char c;
    while ((c = fgetc(file)) != EOF){
        stream[i++] = c;
    }
    stream[i] = '\0'; i++;

    // shrink the stream down to its correct size
    char* tmp = realloc(stream, i * sizeof(*tmp));
    ASSERT(tmp, "can't shrink the allocated string stream.");
    stream = tmp;

    // scanner->stream (Static string init) 
    scanner->stream.str = stream;
    scanner->stream.length = i;
    
    // /*
    // debug this
    printf("debug string\n");
    for (int j = 0; j < i; j++)
    {
        printf("%c", stream[j]);
    }
    printf("\n");
    // */

    
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
        // testing with slash new line skipping behavior in stream peek (DONE)
        // 
        // BUG FOUND ASSERT (FIXED)
        // COMMENT FIXING (FIXED)
        // 
        // FIX ALONG THE CLUNK AHH SCANNER.C TOKENIZE FUNCTION
        // 
        // BEFORE MOVING ON TO THE
        // to handling current-start relation
        // add token policies

        
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
                (*current)++;
                char next = CharStreamPeek(scanner);

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
        printf("Line: %d | TokenTypeID: %d | Lexeme: %s\n", token->line, token->type, token->lexeme.str);
    }
    
    
    // collapse
    free(stream);
    fclose(file);
}

void ScannerDestruct(Scanner* scanner)
{
    free(scanner);
}