#ifndef ERROR
#define ERROR

#include <stdarg.h>
#define ERROR_MESSAGE_LENGTH 256

#define SET_STATUS(status, code, message, ...) \
    do{ \
        status_set(status, code, \
            __LINE__, __FILE__, \
            message __VA_OPT__(, __VA_ARGS__)); \
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