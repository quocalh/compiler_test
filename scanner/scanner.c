#include <stdlib.h>

#include "scanner.h"
#include <assert.h>
#include "../misc/error.h"
#include "../lexer/token.h"

Scanner* scanner_init(Status* status, const char* file_name)
{
    Scanner* scanner = malloc(sizeof(*scanner));
    if (!scanner)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        return NULL;
    }
    
    Vector* tokens = vector_init(status, sizeof(Token));
    if (!STATUS_OK(status))
        goto FREE_TOKENS;

    Vector* stream = vector_init(status, sizeof(char));
    if (!STATUS_OK(status))
        goto FREE_STREAM;

    *scanner = (Scanner){
        .file_name = file_name,
        .current = 0,
        .current_line = 1,

        .tokens = NULL,
        .stream = NULL,
    };
    if (!STATUS_OK(status))
        goto FREE_SCANNER;

    return scanner; 

    FREE_SCANNER: 
    free(scanner);

    FREE_STREAM:
    free(stream);

    FREE_TOKENS:
    free(tokens);

    return NULL;
}


void scanner_destruct(Status* status, Scanner* scanner)
{
    if (!scanner)
    {
        SET_STATUS(status, NULL_SCANNER, "null scanner.");
        return;
    }
    
    vector_destruct(status, scanner->stream);
    vector_destruct(status, scanner->tokens);

    free(scanner);
}


void scanner_scan(Status* status, Scanner* scanner)
{
    // read file into an array
    FILE* file = fopen(scanner->file_name, "r");

    // switch case turn into tokens 

    // (may consider jmp here)
    CLOSE_FILE:
    fclose(file);
}

static size_t scanner_get_file_length(Status* status, FILE* file, const char* file_name)
{
    if (!file)
    {
        SET_STATUS(status, NULL_FILE_PTR, "null file ptr.");
        return 0;
    }

    if (fseek(file, 0, SEEK_END) != 0)
    {
        SET_STATUS(status, SCANNER_FILEREAD_GET_LENGTH_ERROR, 
            "can't go to the end of the give file (filename: %s, fseek).",
            file_name);
        return 0;
    }
    
    long int length = ftell(file);
    if (length < 0)
    {
        SET_STATUS(status, SCANNER_FILEREAD_GET_LENGTH_ERROR,
            "can't evaluate the length of the given file (filename: %s, ftell).",
            file_name);
        return 0;
    }
    length++; // account for the '\0'

    if (fseek(file, 0, SEEK_SET) != 0)
    {
        SET_STATUS(status, SCANNER_FILEREAD_GET_LENGTH_ERROR,
            "can't reset file position (filename: %s, ftell).",
            file_name);
        return 0;
    }

    return length;
}

// can be more modular
void scanner_file_reading(Status* status, Scanner* scanner)
{
    if (!scanner)
        BACK_PROPAGATE(status, NULL_SCANNER, , 
            "null scanner.");
    
    // create a file pointer
    FILE* file = fopen(scanner->file_name, "r");
    if (!file)
    {
        SET_STATUS(status, SCANNER_CANT_OPEN_FILE, 
            "can't open the file (filename: %s).", 
            scanner->file_name);
        return;
    }

    // get an aproximation of the file char stream
    size_t file_length = scanner_get_file_length(status, file, scanner->file_name);
    if (!STATUS_OK(status))
        goto FILE_CLOSE;

    // malloc the stream, given that stream length
    char* stream = malloc(sizeof(*stream) * file_length);
    if (!stream)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        goto FILE_CLOSE;
    }

    // get the char stream's content
    size_t index = 0;
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        stream[index] = c;
        index++;
    }
    if (ferror(file)) // error check
    {
        SET_STATUS(status, SCANNER_FILEREAD_ERROR,
            "error while reading file (filename: %s).",
            scanner->file_name);
        goto FREE_STREAM;
    }
    stream[index] = '\0';
    index++;

    // shrink the stream down
    char* tmp = realloc(stream, sizeof(*stream) * index);
    if (!tmp)
    {
        SET_STATUS(status, REALLOC_FAILED, "realloc failed.");
        goto FREE_STREAM;
    }
    stream = tmp; 

    // return the stream
    scanner->stream = stream;
    scanner->stream_length = index -1;

    goto FILE_CLOSE;

    FREE_STREAM:
    free(stream);

    FILE_CLOSE:
    fclose(file);
    return;
}

// these helpers functions doesn't need to check scanner
static char peek(Status* status, Scanner* scanner, size_t index)
{
    if (index >= scanner->stream_length)
        BACK_PROPAGATE(status, INDEX_ERROR, '\0', 
            "index error (index of %zu with length of %zu)",
            index, scanner->stream_length
        );
    return scanner->stream[index];
}

static bool match(Status* status, Scanner* scanner, size_t index, const char expect)
{

}

static void consume(Status* status, Scanner* scanner, size_t index, const char expect)
{

}

static char* get_lexeme(Status* status, Scanner* scanner, size_t start, size_t end)
{
    size_t length = end - start + 1;
    size_t allocate = length + 1; // account for the '\0'

    char* lexeme = malloc(sizeof(*lexeme) * allocate);
    if (!lexeme)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc_failed.");
        goto RETURN;
    }

    for (size_t i = 0; i < length; i++)
    {
        lexeme[i] = peek(status, scanner, start + i);
        if (!STATUS_OK(status))
            goto FREE_LEXEME;
    }
    lexeme[allocate - 1] = '\0';

    return lexeme;

    FREE_LEXEME:
    free(lexeme);
    
    RETURN: 
    return NULL;
}

static void add_token(Status* status, Scanner* scanner, TokenType type)
{
    char* lexeme = get_lexeme(status, scanner, 
        scanner->start, scanner->current
    );
    if (!STATUS_OK(status))
        return;

    // ownership transfer from lexem to token->lexeme
    Token token = (Token){
        .type = type,
        .line = scanner->current_line,
        .lexeme = lexeme,
    };

    // perform a shallow copy
    vector_add(status, scanner->tokens, &token);
    if (!STATUS_OK(status))
    {
        free(lexeme);
    }
    // do not free lexeme here
}

void scanner_token_extracting(Status* status, Scanner* scanner)
{
    scanner->current_line = 0;
    scanner->current = 0;
    char c;

    while (true)
    {
        c = peek(status, scanner, scanner->current);
        scanner->start = scanner->current;
        
        // in this loop, the current pointer
        // should stop right before steping into the next token
        switch(c)
        {
            case '(':
                add_token(status, scanner, LEFT_PAREN);
                break;
            case ')':
                add_token(status, scanner, RIGHT_PAREN);
                break;
            case '{':
                add_token(status, scanner, LEFT_PAREN);
                break;
            case '}':
                add_token(status, scanner, RIGHT_PAREN);
                break;
            case '+':
                add_token(status, scanner, PLUS);
                break;
            case '-':
                add_token(status, scanner, SIGN);
                break;

        }
        
        if (!STATUS_OK(status))
        {
            printf("im so tired bro, ima sleep.\n");
            assert(0);
        }

        scanner->current++; 
    }
}

