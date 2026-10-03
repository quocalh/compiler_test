#include "token.h"
#include "stdio.h"
#include "../misc/assert_.h"

int size_of_token_type(TokenType type)
{
    size_t size;
    switch (type)
    {
        case INT:
            size = sizeof(int);
            break;
        case DOUBLE:
            size = sizeof(double);
            break;
        case STRING:
            size = sizeof(void*);
            break;
        case TRUE:
            size = sizeof(true);
            break;
        case FALSE:
            size = sizeof(false);
            break;
        case NIL:
            size = sizeof(NULL);
            break;
        default:
            ERROR_VARIADIC("why are you here (TokenType: %d)", type);
            break;
    }
    return size;
}