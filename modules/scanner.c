#include <stdio.h>
#include <ctype.h>

#include "../misc/assert_.h"
#include "scanner.h"
#include "token.h"
#include "../misc/string_handling.h"
#include "../misc/numbers.h"
#include "../misc/heap.h"

#define SCANNER "scanner"

/*
 * TODO: to recreat the c slash-newline behavior
 * testing with slash new line skipping behavior in stream peek (DONE)
 * 
 * BUG FOUND ASSERT (FIXED)
 * COMMENT FIXING (FIXED)
 * 
 * FIX ALONG THE CLUNK AHH SCANNER.C TOKENIZE FUNCTION (FIXED)
 * (DONE FIXED BUT JUST THE WHITE BOX OVERALL CHECK, ONE LAST TIME)
 *      - BROKEN ONES // SEPARATE FUNCTIONS FOR HEAP INIT AND STACKK INIT:
 *          - STRING_HANDLING (FIXED)
 *          - HEAP (FIXED)
 *          - SYSTEM (FIXED)
 *          - SCANNER (~FIXED, BE CAREFUL WITH THIS ONE*)
 *          - MAY BE EVEN TOKEN? (NO)
 *      - AN ADDITIONAL OVERLAY SHOULD BE ADDED FOR THOSE WANT TO HEAP INIT IN THE HEAP (FIXED, LGTM)
 *      -> FREE THE FUNCITON TO FREE THE STRING TOKEN (DONE)
 *  - DONE* NOTE: SCANNER.C USE STREAM A LOT 
 *      - MAKE IT STATIC STRING* (INFER BEING EXTRACTED FROM THE SYSTEM)
 *      - SAME FOR TOKENS (GOOD THAT WE DONE THAT ALREADY)
 * 
 * NUMBER HANDLING (DOUBLE AND INT ONLY) (DONE)
 *  NUMBER DOUBLE HAS PROBLEM (DONE FIXED)
 *  CONSIDERING THE SIZE MISMATCH (MEM ALLOCATION FOR THE RES) (TABLET TIME)
 * KEYWORD HANDLING (DONE)
 *  REDESIGN THE DICTIONARY INTERFACE (NO NEED)
 * VAR_NAME HANDLING (DONE)
 * FINISH THE EXTRA TOKENS (< << >> <=) (DONE)
 * REWORK ON THE POLYMORPHISM SYSTEM (TABLET + SEPARATED PROJECT) (* | PLEASE BE CONSIDERATE ABOUT THE VISITOR PATTERN) 
 * (DONE | SOME DECISIONS LEFT BEFORE SCALE IT IN THIS DIR)
 * MOVE ON TO MAKING A PARSER :D
 * 
 */

Scanner* ScannerInit(System* system)
{
    Scanner* scanner = HeapInsInit(sizeof(*scanner));
    Heap* tokens = HeapInit(sizeof(Token));
    StaticString* stream = HeapInsInit(sizeof(StaticString));
    
    *scanner = (Scanner){
        .hashmap = NULL,

        .file_name = system->file_name,
        .tokens = tokens,
        .stream = stream,

        .start = 0,
        .current = 0,
        .line = 1,
    };
    
    // hashmap, filling keywords
    HashStrToIntAdd(&scanner->hashmap, "while", WHILE);
    HashStrToIntAdd(&scanner->hashmap, "for", FOR);

    HashStrToIntAdd(&scanner->hashmap, "if", IF);
    HashStrToIntAdd(&scanner->hashmap, "else", ELSE);

    HashStrToIntAdd(&scanner->hashmap, "true", TRUE);
    HashStrToIntAdd(&scanner->hashmap, "false", FALSE);
    HashStrToIntAdd(&scanner->hashmap, "nil", NIL);

    HashStrToIntAdd(&scanner->hashmap, "and", AND);
    HashStrToIntAdd(&scanner->hashmap, "or", OR);

    HashStrToIntAdd(&scanner->hashmap, "var", VAR);
    HashStrToIntAdd(&scanner->hashmap, "return", RETURN);
   
    HashStrToIntAdd(&scanner->hashmap, "function", FUNCTION);
    HashStrToIntAdd(&scanner->hashmap, "print", PRINT);
    HashStrToIntAdd(&scanner->hashmap, "super", SUPER);
    HashStrToIntAdd(&scanner->hashmap, "class", CLASS);

    // told you, it would be filled
    system->tokens = scanner->tokens;
    system->stream = scanner->stream;

    return scanner;
}

void ScannerAddToken(Scanner* scanner, TokenType type, void* literal)
{
    Token token;
    token.lexeme = StaticStringSubstring(scanner->stream, scanner->start, scanner->current);
    token.line = scanner->line;
    token.literal = literal;
    token.type = type;
    HeapAdd(scanner->tokens, &token);
}

char ScannerStreamPeek(Scanner* scanner)
{
    int current = scanner->current;
    StaticString* stream = scanner->stream;

    if (current >= stream->length) return '\0';
    if (!stream->str) ERROR("peek | the ptr in the stream is a NULL pointer.");

    // slash-newline skipping behavior
    /* */
    int skipped_index = current;

    const char* str = (const char *)stream->str;
    if (str[skipped_index] == '\\')
    {
        do{
            ++skipped_index;
            if (str[skipped_index] == '\n') scanner->line++;
        }
        while(
              str[skipped_index] == ' '  ||
              str[skipped_index] == '\t' || 
              str[skipped_index] == '\n' );

        // after this: min(skipped_index) = current + 1
        if (skipped_index == current + 1){
            skipped_index--;
        }
        scanner->current = skipped_index;
        
    }

    if (scanner->current >= stream->length) ERROR("peek | given skipped index exceed the available length of the heap array.");
    return ((char*)stream->str)[scanner->current];
}
void ScannerDiscardComment(Scanner* scanner)
{
    while (true){
        (++(scanner->current));
        char c = ScannerStreamPeek(scanner);
        if (c == '\0') break;
        if (c == '\n') {++scanner->line;break;}
    }
}
void ScannerScanString(Scanner* scanner)
{
    // DEV NOTE: we can do a string \" accept here, but i'm too lazy
    ASSERT(ScannerStreamPeek(scanner) == '"', "the starting char is not a '\"'");
    
    // A temp buffer to store the string
    Heap* str_vector = HeapInit(sizeof(char));
    char c;
    (scanner->current)++;
    while ((c = ScannerStreamPeek(scanner)) != '"')
    {
        HeapAdd(str_vector, &c);
        (scanner->current)++;
    }
    
    // StaticString string retrieval O(N) + Tokenize the string
    StaticString* ss = StaticStringInit(str_vector->ptr);
    // StaticString* ss = StaticStringSubstring(scanner->stream, scanner->start, scanner->current);
    ScannerAddToken(scanner, STRING, ss);
    free(str_vector);
}
void ScannerScanNumber(Scanner* scanner)
{
    // scan the number + determine if that is float or int
    bool dot = false;
    
    // scan the number's lexeme
    char c;
    while (isdigit(c = ScannerStreamPeek(scanner)) || c == '.') {
        scanner->current++;
        if (c == '.') dot = true;
    }

    scanner->current--;
    StaticString* lexeme = StaticStringSubstring(
        scanner->stream, scanner->start, scanner->current
    );

    TokenType type;
    void* cache;
    if (dot){
        type = DOUBLE;
        cache = HeapInsInit(sizeof(double));
        *(double*)cache = (double) atof(lexeme->str);
    }
    else {
        type = INT;
        cache = HeapInsInit(sizeof(int));
        *(int*)cache = (int) atoi(lexeme->str);
    }

    ScannerAddToken(scanner, type, cache);
}
void ScannerScanKeyWordsAndVars(Scanner* scanner) 
{
    Heap* array = HeapInit(sizeof(char));

    char c;
    char null = '\0';
    
    HeapAdd(array, &null);
    char* str = array->ptr;
    while(isalnum(c = ScannerStreamPeek(scanner)) || c == '_')
    {
        HeapAdd(array, &null);
        str[array->length -1 -1] = c;
        scanner->current++;
    }
    scanner->current--;
    
    // get token type: either var or [keywords]
    TokenType type;
    HashStrToInt *res = HashStrToIntFind(&scanner->hashmap, str);
    if (!res) type = IDENTIFIER;
    else type = res->bucket;
   
    ScannerAddToken(scanner, type, NULL);
    HeapFree(array);
}
void ScannerScan(Scanner* scanner, System* system)
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
    while ((c = fgetc(file)) != EOF)
    {
        stream[i++] = c;
    }
    stream[i] = '\0'; i++;

    // shrink the stream down to its correct size
    char* tmp = realloc(stream, i * sizeof(*tmp));
    ASSERT(tmp, "can't shrink the allocated string stream.");
    stream = tmp;

    // scanner->stream (Static string init) 
    scanner->stream->str = stream;
    scanner->stream->length = i;
    
    // /*
    // debug
    scanner->current = 0;
    char ci;
    printf("DEBUG\n");
    while (scanner->current < i)
    {
        ci = ScannerStreamPeek(scanner);
        printf("%c", ci);
        scanner->current++;
    }
    printf("\nDEBUG\n");
    scanner->current = 0;
    // */
    
    // translating string into tokens, add into token array
    // NOTE: last element should be EOF
    int* start = &scanner->start;
    int* current = &scanner->current;
    int* line = &scanner->line;
    Heap* tokens = scanner->tokens;
    while ((*start) < scanner->stream->length)
    {
        *start = *current;

        char c = stream[scanner->start];

        
        switch (c)
        {
            case '(':
                ScannerAddToken(scanner, LEFT_PAREN, NULL);
                break;
            case ')':
                ScannerAddToken(scanner, RIGHT_PAREN, NULL);
                break;
            case '{':
                ScannerAddToken(scanner, LEFT_BRACE, NULL);
                break;
            case '}':
                ScannerAddToken(scanner, RIGHT_BRACE, NULL);
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
            case '\\':
                ScannerAddToken(scanner, BACKSLASH, NULL);
                break;
            case '^':
                ScannerAddToken(scanner, HAT, NULL);
                break;
 
            case '\n':
                (*line)++;
            case '\t':
            case ' ':
                break;           

            case '!':
                // peek the next char
                (*current)++;
                if ((c = ScannerStreamPeek(scanner)) == '=') // !=
                {
                    ScannerAddToken(scanner, BANG_EQUAL, NULL);
                }
                else
                {
                    (*current)--;
                    ScannerAddToken(scanner, BANG, NULL);
                }
                break;
            
            case '=':
                (*current)++;
                if ((c = ScannerStreamPeek(scanner)) == '=')
                {
                    ScannerAddToken(scanner, EQUAL_EQUAL, NULL);
                }
                else
                {
                    (*current)--;
                    ScannerAddToken(scanner, EQUAL, NULL);
                }
                break;

             case '<':
                (*current)++;
                if ((c = ScannerStreamPeek(scanner)) == '=')
                {
                    ScannerAddToken(scanner, LESS_EQUAL, NULL);
                }
                else
                {
                    (*current)--;
                    ScannerAddToken(scanner, LESS, NULL);
                }
                break;

             case '>':
                (*current)++;
                if ((c = ScannerStreamPeek(scanner)) == '=')
                {
                    ScannerAddToken(scanner, GREATER_EQUAL, NULL);
                }
                else
                {
                    (*current)--;
                    ScannerAddToken(scanner, GREATER, NULL);
                }
                break;

            case '/':;
                (*current)++;
                char next = ScannerStreamPeek(scanner);

                if (next == '/')
                {
                    ScannerDiscardComment(scanner);
                }
                else{
                    (*current)--;
                    ScannerAddToken(scanner, SLASH, NULL);     
                }
                break;
            
            // string
            case '"':
                ScannerScanString(scanner);
                break;


            default:
                // handling numbers
                if (isdigit(c))
                {
                    ScannerScanNumber(scanner);
                    break;
                }
                // handling keywords and vars
                else if (isalpha(c))
                {
                    ScannerScanKeyWordsAndVars(scanner); 
                    break;
                }
        }
        (*current)++;
    }

    // debug token testing
    for (int i = 0; i < scanner->tokens->length; i++)
    {
        Heap* heap = scanner->tokens;
        Token* token = heap->ptr + i * heap->size;
        // Token token = (heap->ptr)[i];
        printf("Line: %d | TokenTypeID: %d | Lexeme: %s\n", token->line, token->type, token->lexeme->str);
    }
    // collapse
    fclose(file);
}

void ScannerDestruct(Scanner* scanner)
{
    printf("free hash\n");
    HashStrToIntFree(&scanner->hashmap);
    free(scanner);
}