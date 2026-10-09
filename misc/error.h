#ifndef ERROR
#define ERROR

#include <stdarg.h>
#define ERROR_MESSAGE_LENGTH 256

#define SET_STATUS(status, code, message, ...) \
    do{ \
        status_set(status, code, \
            __LINE__, __FILE__, \
            message __VA_OPT__(, __VA_ARGS__)); \
    }while(0);

typedef enum{
    
    // general
    MALLOC_FAILED,
    NO_ERROR,
    NULL_RETURN,

    // misc/vector.h
    VECTOR_ALLOCATE_FAILED, 
    VECTOR_INDEXING_ERROR,
    VECTOR_CURRENT_OVERLOAD_ALLOCATED,
    VECTOR_CAPACITY_OVERFLOW,
    VECTOR_NULL_INVALID,
    
    // 
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