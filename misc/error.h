#ifndef ERROR
#define ERROR

#include <stdarg.h>
#define ERROR_MESSAGE_LENGTH 256

/*
cache the error message, and send back
in the future the status could have been dynamic + queue + interface
     to report multiple layers, to report multiple errors 
     (arrays of errors) along the tranverse fallback flow
     error report (midly tolerate to be slower)
     no, i think binary tree helps that time
*/

#define SET_STATUS(status, code, message, ...) \
    do{ \
        status_set(status, code, \
            __LINE__, __FILE__, \
            message __VA_OPT__(, __VA_ARGS__)); \
    } while(0);

#define CLEAR_STATUS_MSG_BUFFER(status) \
    do { \
        for (size_t i = 0; i < ERROR_MESSAGE_LENGTH; i++) \
        { \
            (status)->message[i] = '\0'; \
        } \
    } while(0);

#define CLEAR_STATUS(status) \
    do { \
        CLEAR_STATUS_MSG_BUFFER(status); \
        (status)->code = NO_ERROR; \
    } while(0);

// i don't know how to name this one
#define CHECK_PROPAGATE(status, dest) \
    do{ \
        if (!STATUS_OK(status)) \
        { \
            status_reporting(status); \
            goto dest; \
        } \
    } while(0);

#define BACK_PROPAGATE(status, code, return_value, message, ...) \
    do{ \
        SET_STATUS(status, code, message __VA_OPT__(, __VA_ARGS__)); \
        return return_value; \
    } while(0);

/*
new interface: 
EXPR_PROPAGATE(statement, goto label)
{
    // cleanup
}
*/
typedef enum{
    
    // general
    MALLOC_FAILED,
    REALLOC_FAILED,
    NULL_FILE_PTR,
    NO_ERROR,
    INDEX_ERROR,
    NULL_RETURN,
    NULL_INVALID_VALUE,

    // misc/vector.h
    VECTOR_ALLOCATE_FAILED, 
    VECTOR_INDEXING_ERROR,
    VECTOR_CURRENT_OVERLOAD_ALLOCATED,
    VECTOR_CAPACITY_OVERFLOW,
    VECTOR_NULL_INVALID,
    
    // misc/hash string to int
    HASH_STR_INT_ERROR, 
    HASH_KEY_DUPLICATION_FOUND,

    // arena
    VIRTUAL_MEM_ALLOC_FAILED,
    RESERVED_SIZE_BEING_ZERO,
    NULL_ARENA,
    ARENA_ADD_ZERO_SIZE_ITEM,
    ARENA_SIZE_OVERFLOW,
    ARENA_VIRTUAL_MEM_COMMIT_FAILED,
    ARENA_VIRTUAL_MEM_FREE_FAILED,
    PAGE_SIZE_QUERY_FAILED,
    NULL_ITEM,
    // scoped arena
    NULL_SCOPED_ARENA, 

    // scanner
    NULL_SCANNER,
    SCANNER_CANT_OPEN_FILE,
    SCANNER_FILEREAD_GET_LENGTH_ERROR,
    SCANNER_FILEREAD_ERROR,
    SCANNER_UNKNOWN_STRING_TOKEN,
    SCANNER_SCANNUMBER_NUMBER_ERROR,
    SCANNER_CONSUME_EXPECT_DIFFERENT_CHAR,

    // expr
    AST_EXPR_UNKNOWN_EXPR_TYPE,
    AST_LITERAL_UNKNOWN_DTYPE,

    // parser
    NULL_PARSER, 
    PARSER_BUILD_AST_UNKNOWN_LITERAL_TYPE,
    PARSER_BUILD_AST_EXPECT_A_RIGHT_PAREN

} ErrorCode;

/* this struct strictly stay in heap*/
typedef struct{
    ErrorCode code; 
    char message[ERROR_MESSAGE_LENGTH];

    int line;
    const char* file_name;
} Status;

void status_setup(Status* status);

void status_set(Status* status, ErrorCode code, 
    int line, const char* file_name, 
    char* message, ...);

bool STATUS_OK(Status* status);

void status_reporting(Status* status);
void status_interpret_code(ErrorCode code);

#endif