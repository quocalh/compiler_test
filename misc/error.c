#include "error.h"
#include <stdio.h>
#include <stdarg.h>

void status_setup(Status* status)
{
    status->code = NO_ERROR;
}


void status_set(Status* status, ErrorCode code, 
    int line, const char* file_name, 
    char* message, ...)
{
    status->code = code;
    status->line = line;
    status->file_name = file_name;

    va_list va;
    va_start(va);

    // v: va_list | s: string write | n: string of length n
    vsnprintf(
        status->message, 
        ERROR_MESSAGE_LENGTH, 
        message, 
        va
    );
    va_end(va);
}


bool STATUS_OK(Status* status)
{
    return status->code == NO_ERROR;
}


void status_reporting(Status* status)
{
    printf("ERROR: [%s:%d]: ", status->file_name, status->line);
    printf("%s\n", status->message);
}


void status_interpret_code(ErrorCode code)
{
    switch (code)
    {
        case NO_ERROR:
            break;

        case MALLOC_FAILED:
            break;

        default:
            break;
    }
}