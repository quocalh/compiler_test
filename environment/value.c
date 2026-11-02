#include "value.h"

// failed prototype
/*
DataType BinaryType(Status* status, DataType lhs, DataType rhs)
{
    DataType a, b, res;
    // lhs => rhs

    a = max(lhs, rhs);

    // can't be computed 
    if (a > DATA_LIST)
    {
        SET_STATUS(status, VALUE_ERROR_TYPE, 
            "value error type (binary op "
            "between (data ID: %d))."
            "and (data ID: %d)).", lhs, rhs
        );
        goto DATA_ERROR_EXIT;
    }

    // return the maximum
    if (a <= DATA_DOUBLE)
        return a;
    

    if (a == DATA_LIST)
    {
        b = min(lhs, rhs);
        // int * vector = vector
        if (b >= DATA_INT)
            return a;

        // 
        else if (b)

        goto DATA_ERROR_EXIT;
    }
    
    DATA_ERROR_EXIT:
    SET_STATUS(status, VALUE_ERROR_TYPE, 
        "value error type (binary op "
        "between (data ID: %d))."
        "and (data ID: %d)).", lhs, rhs
    );
    return DATA_ERROR_TYPE;
}
*/