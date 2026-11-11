#include "value.h"
#include "../misc/vector.h"
#include "../misc/misc.h"

Value* value_allocate(Status* status)
{
    Value* value = malloc(sizeof(*value));
    if (!value)
    {
        SET_STATUS(status, MALLOC_FAILED, "malloc failed.");
        return NULL;
    }
    return value;
}

void value_destruct(Status* status, Value* value)
{
    if (!value)
        return;

    switch (value->type)
    {
        case DATA_INT:
        case DATA_DOUBLE:
        case DATA_TRUE:
        case DATA_FALSE:
            free(value);
            break;
        
        case DATA_STRING:
            free(value);
            // for now, borrowed from the lexeme token
            break;

        case DATA_ARRAY:
            printf("nigger.\n");
            break;
        case DATA_LIST:
            printf("nigger2.\n");
            break;

        case DATA_OBJECT:
            SET_STATUS(status, NOT_YET_IMPLEMENTED, 
                "not yet implemented.");
            break;

        default:
            SET_STATUS(status, VALUE_UNKNOWN_TYPE, 
                "value unknown type (%s: %d)", 
                STR(DataType), 
                value->type
            );
            break;
    }
    return;
}

void value_primitive_conversion(Status* status,
    Value* value, DataType desired_type)
{
    if (value->type >= DATA_PRIMITIVE || desired_type >= DATA_PRIMITIVE)
    {
        SET_STATUS(status, VALUE_ERROR_PRIMITIVE_PROMOTING,
            "can't convert from (id: %d) to (id: %d) (enum: %s).",
            value->type, desired_type, STR(DataType));
        return;
    }

    // i can't think of anything better, O(1) still
    double literal;
    switch (value->type)
    {
        case DATA_INT:
            literal = (double)value->payload.integer;
            break;

        case DATA_DOUBLE:
            literal = (double)value->payload.real;
            break;

        case DATA_TRUE:
            literal = (double)1.0;
            break;

        case DATA_FALSE:
            literal = (double)0.0;
            break;

        default:
            SET_STATUS(status, VALUE_ERROR_PRIMITIVE_PROMOTING,
                "unsupported source type (id: %d)", value->type);
            return;
    }

    switch (desired_type)
    {
        case DATA_INT:
            value->payload.integer = (int)literal;
            break;

        case DATA_DOUBLE:
            value->payload.real = literal;
            break;

        case DATA_TRUE:
        case DATA_FALSE:
            value->payload.boolean = (bool)literal;
            desired_type = (literal != 0.0) ? DATA_TRUE : DATA_FALSE;
            break;

        default:
            SET_STATUS(status, VALUE_ERROR_PRIMITIVE_PROMOTING,
                "unsupported destination type (id: %d)", desired_type);
            return;
    }

    value->type = desired_type;
}

DataType value_primitive_common_type(Status* status, 
    DataType type1, DataType type2)
{
    return (type1 > type2)? type1:type2;
}

// assume that the array is the vector only, storing Value*
static void print_array(Status* status, Value* array)
{
   printf("you haven't printed any array yet.");
    SET_STATUS(status, NOT_YET_IMPLEMENTED,
        "has not implemented fixed array.");
}

static void print_list(Status* status, Value* list)
{
   printf("you haven't printed any list yet.");
   SET_STATUS(status, NOT_YET_IMPLEMENTED, 
    "implement now.");
}

void print_value(Status* status, Value* value)
{
    DataPayLoad payload = value->payload; 
    switch (value->type)
    {
        case DATA_TRUE:
            printf("true");
            break;

        case DATA_FALSE:
            printf("false");
            break;

        case DATA_NIL:
            printf("null");
            break; 

        case DATA_INT:
            printf("%d", payload.integer);
            break;

        case DATA_DOUBLE:
            printf("%f", payload.real);
            break;

        case DATA_STRING:
            printf("%s", payload.string);
            break;

        case DATA_ARRAY:
            print_array(status, value);
            break; 

        case DATA_LIST:
            print_list(status, value);
            break;

        case DATA_OBJECT:
            // return the addresss
            printf("<instance at %p>", payload.instance);
            break;

        default:
            SET_STATUS(status, NOT_YET_IMPLEMENTED, 
               "value type cannot be printed yet.");
            return;
    }
}

bool value_is_primitive(DataType type)
{
    /*
    return (
        (type == DATA_INT) ||  
        (type == DATA_DOUBLE) ||  
        (type == DATA_TRUE) ||  
        (type == DATA_FALSE) ||  
        (type == DATA_NIL) ||  
    );
    */
    return (type < DATA_PRIMITIVE);
}

bool value_is_composite(DataType type)
{
    return !value_is_primitive(type);
}

bool value_is_countable(DataType type)
{
    return (
        type == DATA_STRING ||
        type == DATA_ARRAY  ||
        type == DATA_LIST);
}
