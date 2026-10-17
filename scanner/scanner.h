#ifndef SCANNER
#define SCANNER

#include "../lexer/token.h"
#include "../misc/vector.h"
#include "../misc/error.h"
#include "../misc/arena.h"
#include "../misc/misc.h"

#define SCANNER_ARENA_SIZE 2 * MB


typedef struct{
    const char* file_name; 
    
    size_t start;
    size_t current;
    size_t current_line;
    
    /*
    arena is used here (acess of scope)
        - assume that stream (string stream) and tokens 
        will not be collected at the same time
        - could have been better if that was block arena 
        (expandable arena (will create if needed)) 
        - presume that this architecture follows the stack format
            - arena, scoped arenas are freed at the end 

        tokens has to be vector as tokens will be borrowed by another modules
    */ 

    char* stream;
    size_t stream_length;

    Vector* tokens;

} Scanner;

Scanner* scanner_init(Status* status, const char* file_name);
void scanner_destruct(Status* status, Scanner* scanner);
void scanner_scan(Status* status, Scanner* scanner);

// these are no interfaces; but rather internals
// solely being here for testing, benchmark purposes
void scanner_file_reading(Status* status, Scanner* scanner);

#endif
