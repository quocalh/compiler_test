#include <stdlib.h>
#include <ctype.h>

#include "scanner.h"
#include <assert.h>
#include "../misc/error.h"
#include "../lexer/token.h"

static HashStringToInt* scanner_set_keywords(Status* status)
{
    typedef struct{
        const char* key;
        TokenType type;
    } KeyWord;

    static const KeyWord keywords[] = {
        {"function", FUNCTION},
        {"var",      VAR},
        {"for",      FOR},
        {"while",    WHILE},
        {"if",       IF},
        {"else",     ELSE},
        {"true",     TRUE},
        {"false",    FALSE},
        {"nil",      NIL},
        {"EOF",      END_OF_FILE},
    };

    // wil return a null upon a success init
    HashStringToInt* hash = hash_str_to_int_init(status); 
    if (!STATUS_OK(status))
        return NULL;

    for (unsigned int i = 0; i < sizeof(keywords) / sizeof(*keywords); i++)
    {
        hash_str_to_int_add_key(status, &hash, 
            keywords[i].key, keywords[i].type);
        if (!STATUS_OK(status))
            goto FAILED;
    }
    return hash;

    FAILED:
    hash_str_to_int_destruct(status, &hash);
    return NULL;
}

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
    
    HashStringToInt* keywords = scanner_set_keywords(status);
    if (!STATUS_OK(status))
    {
        // the set_keywords function handle its own cleanup on failure
        goto FREE_TOKENS;
    }
    

    *scanner = (Scanner){
        .file_name = file_name,
        .current = 0,
        .current_line = 1,

        .stream_length = 0,
        .stream = NULL,

        .tokens = tokens,
        .keywords_map = keywords,
    };
    if (!STATUS_OK(status))
        goto FREE_SCANNER;

    return scanner; 

    FREE_SCANNER: 
    free(scanner);

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
    
    free(scanner->stream);

    // free lexeme
    for (int i = 0; i < scanner->tokens->length; i++)
    {
        Token* token = (Token*)(scanner->tokens->ptr) + i;
        free(token->lexeme);
    }
    
    vector_destruct(status, scanner->tokens);

    hash_str_to_int_destruct(status, &scanner->keywords_map);

    free(scanner);
}


void scanner_scan(Status* status, Scanner* scanner)
{
    // turn the file into a char stream, stored in char* stream
    scanner_file_reading(status, scanner);
    if (!STATUS_OK(status))
        return;
    
    // turn char* stream into tokens
    scanner_token_extracting(status, scanner);
    if (!STATUS_OK(status))
        return;
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
    if (index >= scanner->stream_length + 1)
        BACK_PROPAGATE(status, INDEX_ERROR, '\0', 
            "index error (index of %zu, exceed the allocated length of %zu + 1)",
            index, scanner->stream_length
        );
    return scanner->stream[index];
}

static bool match(Status* status, Scanner* scanner, size_t index, const char expect)
{
    char c = peek(status, scanner, index);
    return (c == expect);
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
        goto EXIT;
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
    
    EXIT: 
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

static void scanner_scan_number(Status* status, Scanner* scanner)
{
    // 5..123 -> INVALID
    // .5. -> INVALID

    // .5 -> VALID
    // 67.435 -> VALID
    // 67 -> VALID

    char c = peek(status, scanner, scanner->current);
    if (c != '.' && !isdigit(c))
        return;

    bool dot = false;
    bool number_seen = false;
    while (true)
    {
        c = peek(status, scanner, scanner->current); 
        
        if (c != '.' && !isdigit(c))
            break;

        // may contain at most one decimal point 
        if (c == '.')
        {
            if (dot)
            {
                SET_STATUS(status, SCANNER_SCANNUMBER_NUMBER_ERROR,
                    "uhmm, multiple decimal points is invalid you know "
                    "(e.g. 5...5. is not a valid number)."
                );
                return;
            }
            dot = true;
        }
        else
        {
            number_seen = true;
        }

        // stoping at the first non-matching character
        scanner->current++; 
    }
    TokenType type = dot ? DOUBLE : INT;

    // edge case '.' as a number
    if (!number_seen)
    {
        // if that so, then pretend nothing happens
        scanner->current = scanner->start;
        return;
    }

    // convert into inclusive end index
    // since get lexeme go for the [a, b] instead of the conventional [a, b)
    scanner->current--;
    add_token(status, scanner, type);

    // number get the current pointer to be at the start of the next non-matching character
    scanner->current++;
}

static void scanner_scan_string(Status* status, Scanner* scanner)
{
    char c = peek(status, scanner, scanner->current);
    if (c != '"')
    {
        SET_STATUS(status, SCANNER_CONSUME_EXPECT_DIFFERENT_CHAR,
            "expect a '\"', got a '%c' instead.", c);
        return;
    }

    // advance til hit the closing double quotation mark
    do
    {
        // current must stop at the last '"'
        c = peek(status, scanner, ++scanner->current);

        // handling reaching the end of the input before finding the closing quote
        if (status->code == INDEX_ERROR)
        {
            CLEAR_STATUS_MSG_BUFFER(status);
            SET_STATUS(status, SCANNER_CONSUME_EXPECT_DIFFERENT_CHAR,
            "can't find the closing double quotation of the string.");
            return;
        }
    } while (c != '"');

    add_token(status, scanner, STRING);
    // stop right before the next non-matching characters
}

static void scanner_scan_keyword_and_identifier(Status* status, Scanner* scanner)
{
    char c;
    char null_ = '\0';

    while (true)
    {
        c = peek(status, scanner, scanner->current);
        if (!STATUS_OK(status))
            return;

        if (!isalnum(c) && c != '_')
            break;

        scanner->current++;
    }
    // atp, we have the scanner->current at the first non-matching char  
    scanner->current--;

    size_t length = scanner->current - scanner->start + 1;

    char* word = malloc(sizeof(char) * (length + 1));
    if (!word)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        return;
    }
    
    // put the lexeme into a tmp malloc
    memcpy(word, (char*)scanner->stream + scanner->start, length);
    word[length] = '\0';
    
    HashStringToInt* pair = hash_str_to_int_find(status, &scanner->keywords_map, word);
    if (!STATUS_OK(status))
        goto FREE_WORD;
    
    if (!pair)
        add_token(status, scanner, IDENTIFIER);
    else
        add_token(status, scanner, pair->number);
    // this is the end, no need to check status here

    FREE_WORD:
    free(word);
}

void scanner_token_extracting(Status* status, Scanner* scanner)
{
    scanner->current_line = 1;
    scanner->current = 0;
    char c;

    /*
    in this loop, the current pointer
        should stop right before steping into the next token
    avoid any status check in the switch case bracket
        (avoid unecessary jumps, as status is checked after every switch case)
    for situation, where it is justified to create a var
        then put it in the scope (or it will break, as there is no scope to hold its value)
    */
    while (true)
    {
        // break if reach the end of char* stream
        if (scanner->current >= scanner->stream_length) 
            break;

        scanner->start = scanner->current;
        scanner_scan_number(status, scanner);
        if (!STATUS_OK(status))
        {
            goto EXIT;
        }

        scanner->start = scanner->current;
        c = peek(status, scanner, scanner->current);
        switch(c)
        {
            case '(':
                add_token(status, scanner, LEFT_PAREN);
                break;
            case ')':
                add_token(status, scanner, RIGHT_PAREN);
                break;
            case '{':
                add_token(status, scanner, LEFT_BRACE);
                break;
            case '}':
                add_token(status, scanner, RIGHT_BRACE);
                break;
            case '+':
                add_token(status, scanner, PLUS);
                break;
            case '-':
                add_token(status, scanner, SIGN);
                break;
            case '*':
                add_token(status, scanner, STAR);
                break;
            case '/':
            {
                char c;
                // comment, proceed to skip all char til the line break
                if (peek(status, scanner, scanner->current + 1) == '/')
                {
                    do {
                        c = peek(status, scanner, (++scanner->current) + 1);
                    // if c = \n or \0 then break
                    } while(c != '\n' && c != '\0');

                    goto SLASH_BREAK;
                }
                // just an ordinary slash
                add_token(status, scanner, SLASH);

                SLASH_BREAK:
                break;
            }
            
            case '\n':
                scanner->current_line++;
            case '\t':
            case '\r':
            case ' ':
                break;

            case '=':
            {
                // ==
                if (match(status, scanner, scanner->current + 1, '='))
                {
                    scanner->current++;
                    add_token(status, scanner, EQUAL_EQUAL);
                    goto EQUAL_BREAK;
                }

                // =
                add_token(status, scanner, EQUAL);

                EQUAL_BREAK:
                break;
            }

            case '<':
            {
                // <=
                if (match(status, scanner, scanner->current + 1, '='))
                {
                    scanner->current++;
                    add_token(status, scanner, LESS_EQUAL);
                    goto LESS_BREAK;
                }

                // <
                add_token(status, scanner, LESS);

                LESS_BREAK:
                break;
            }

            case '>':
            {
                // >=
                if (match(status, scanner, scanner->current + 1, '='))
                {
                    scanner->current++;
                    add_token(status, scanner, GREATER_EQUAL);
                    goto GREATER_BREAK;
                }

                // >
                add_token(status, scanner, GREATER);

                GREATER_BREAK:
                break;

            }
            
            
            case ';':
                add_token(status, scanner, SEMICOLON);
                break;
            case ',':
                add_token(status, scanner, COMA);
                break;
            case '.':
                add_token(status, scanner, DOT);
                break;
            
            case '"':
                scanner_scan_string(status, scanner);
                break;
            
            default:
            { 
                if (isalnum(c) || c == '_')
                {
                    scanner_scan_keyword_and_identifier(status, scanner);
                    break;
                }

                SET_STATUS(status, SCANNER_UNKNOWN_STRING_TOKEN, 
                    "unknown string token (unknown char: '%c')", c
                );
                break;
            }
        }
        
        if (!STATUS_OK(status))
            goto EXIT;

        scanner->current++; 
    }
    // check advancement

    EXIT:
    return;
}

