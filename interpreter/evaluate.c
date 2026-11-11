#include "evaluate.h"
#include "stdlib.h"
#include "../misc/misc.h"
#include "../environment/value.h"

#define SET_VALUE_TRUE(value) \
    do{ \
        (value)->type = DATA_TRUE; \
        (value)->payload.boolean = true; \
    } while(0)

#define SET_VALUE_FALSE(value) \
    do{ \
        (value)->type = DATA_FALSE; \
        (value)->payload.boolean = false; \
    } while(0)

Value* evaluate_expr_dispatch(Status* status, Interpreter* interpreter, Expr* expr)
{
    Value* value;
    switch (expr->type)
    {
    case LITERAL:
        value = evaluate_literal(status, interpreter, (Literal*)expr);
        break;

    // hell hole
    case BINARY:
        value = evaluate_binary(status, interpreter, (Binary*)expr);
        break;

    case UNARY:
        value = evaluate_unary(status, interpreter, (Unary*)expr);
        break;

    case ASSIGN:
        value = evaluate_assign(status, interpreter, (Assign*)expr);
        break;
    
    case POSTFIX:
        break;
    
    case VARIABLE:
        value = evaluate_variable(status, interpreter, (Variable*)expr);
        break;
    
    default:
        SET_STATUS(status, EVALUATE_EXPR_UNKNOWN_EXPR_TYPE, 
            "evaluate unknown expr type (expr id: %d)", expr->type);
        return NULL;
    }

    return value;
}

// could have been true false, so our prog is easier to walk through
// will change later
void value_is_truthy(Status* status, Value* value)
{
    DataPayLoad payload = value->payload;
    switch(value->type)
    {
        case DATA_NIL:
            SET_VALUE_FALSE(value);
            break; 

        case DATA_FALSE:
        case DATA_TRUE:
            break;

        case DATA_INT:
            if (payload.integer == 0)
                SET_VALUE_FALSE(value);
            else
                SET_VALUE_TRUE(value);
            break;

        case DATA_DOUBLE:
            if (payload.real == 0)
                SET_VALUE_FALSE(value);
            else
                SET_VALUE_TRUE(value);
            break;

        case DATA_OBJECT:
            if (payload.instance != NULL)
                SET_VALUE_TRUE(value);
            else
                SET_VALUE_FALSE(value);
            break;
             
        // case DATA_LIST:
        //     break;

        // case DATA_HASH: // boss im tired...
        //     break;

        default:
            SET_STATUS(status, EVALUATE_TRUTH_ERROR_TYPE_UNKNOWN, 
                "evaluate truth error type unknown (%s id: %d)", 
                STR(DataType), value->type);
            return;
    }
}

void value_is_not_truthy(Status* status, Value* value)
{
    value_is_truthy(status, value);

    // flip the bool
    if (value->type == DATA_TRUE)
    {
        SET_VALUE_FALSE(value);
    }
    else
    {
        SET_VALUE_TRUE(value);
    }
}

Value* evaluate_literal(Status* status, Interpreter* interpreter, Literal* literal)
{
    Value* value = value_allocate(status);
    if (!STATUS_OK(status)) return NULL;

    switch (literal->dtype)
    {
        case TRUE:
            value->type = DATA_TRUE;
            value->payload.boolean = true;
            break;

        case FALSE:
            value->type = DATA_FALSE;
            value->payload.boolean = false;
            break;

        case NIL:
            value->type = DATA_NIL;
            value->payload.instance = NULL;
            break;

        case INT:
            value->type = DATA_INT;
            // could be (int)strtol
            value->payload.integer = atoi(literal->_lexeme);
            break;
            
        case DOUBLE:
            value->type = DATA_DOUBLE;
            // could be strtod
            value->payload.real = atof(literal->_lexeme);
            break;

        case STRING:
            // temporary fix, wait til the tracing garbage collector
            value->type = DATA_STRING;
            value->payload.string = literal->_lexeme;
            break;
        case DATA_ARRAY:
        case DATA_LIST:
        case DATA_OBJECT:
            /*
                about string, since it will have its own personal heap 
                slot given in the runtime env
                
                additionally, we will have functions where new strings are born
                str1 + str2 = str3 (new entities)
                str1 * int = str2 (new entities)

                it would be more manageable to give each string its own ownership
                we could have used a flag to denote stack-belong 
                    (we are heading this direction)
            */
            SET_STATUS(status, NOT_YET_IMPLEMENTED, "not yet implemented.");
            return NULL;

        default:
            SET_STATUS(status, EVALUATE_EXPR_UNKNOWN_DATA_TYPE,
                "encounter unknown datatype (%s id: %d)", 
                STR(DataType),
                literal->dtype
            );
            return NULL;
    }
    return value;
}


// evaluate_binary composite

// mutate means results are stored in the left value
Value* evaluate_binary_primitive_mutate(Status* status, 
    Value* left, Value* right, TokenType op)
{
    DataType common = max(left->type, right->type);
    // for simplicity, this should be alr
    if (common == DATA_TRUE || common == DATA_FALSE)
        common = DATA_INT;

    value_primitive_conversion(status, left, common);
    if (!STATUS_OK(status)) return NULL;

    value_primitive_conversion(status, right, common);
    if (!STATUS_OK(status)) return NULL;

    left->type = common;
    right->type = common;

    switch (common)
    {
        case DATA_TRUE:
        case DATA_FALSE:
        case DATA_INT:
        {
            switch(op)
            {
                case PLUS:
                    left->payload.integer = (
                        left->payload.integer + 
                        right->payload.integer
                    ); break;
                case SIGN:
                    left->payload.integer = (
                        left->payload.integer -
                        right->payload.integer
                    ); break;
                case STAR:
                    left->payload.integer = (
                        left->payload.integer *
                        right->payload.integer
                    ); break;
                case SLASH:
                    if (right->payload.integer == 0)
                        goto DIVISION_BY_ZERO;
                    left->payload.integer = (
                        left->payload.integer /
                        right->payload.integer
                    ); break;

                case GREATER:
                {
                    bool res = left->payload.integer > right->payload.integer;
                    left->type = res? DATA_TRUE: DATA_FALSE;
                    left->payload.boolean = res;
                    break;
                }
                case GREATER_EQUAL:
                {
                    bool res = left->payload.integer >= right->payload.integer;
                    left->type = res? DATA_TRUE: DATA_FALSE;
                    left->payload.boolean = res;
                    break;
                }
                case LESS:
                {
                    bool res = left->payload.integer < right->payload.integer;
                    left->type = res? DATA_TRUE: DATA_FALSE;
                    left->payload.boolean = res;
                    break;
                }
                case LESS_EQUAL:
                {
                    bool res = left->payload.integer <= right->payload.integer;
                    left->type = res? DATA_TRUE: DATA_FALSE;
                    left->payload.boolean = res;
                    break;
                }
                case BANG_EQUAL:
                {
                    bool res = left->payload.integer != right->payload.integer;
                    left->type = res? DATA_TRUE: DATA_FALSE;
                    left->payload.boolean = res;
                    break;
                }
                case EQUAL_EQUAL:
                {
                    bool res = left->payload.integer == right->payload.integer;
                    left->type = res? DATA_TRUE: DATA_FALSE;
                    left->payload.boolean = res;
                    break;
                }

                default:
                    goto INVALID_OPERATOR;
            }
            break;
        }
        case DATA_DOUBLE:
        {
            switch(op)
            {
                case PLUS:
                    left->payload.real = (
                        left->payload.real + 
                        right->payload.real
                    ); break;
                case SIGN:
                    left->payload.real = (
                        left->payload.real -
                        right->payload.real
                    ); break;
                case STAR:
                    left->payload.real = (
                        left->payload.real * 
                        right->payload.real
                    ); break;
                case SLASH: 
                if (right->payload.real == 0)
                    goto DIVISION_BY_ZERO;
                left->payload.real = (
                        left->payload.real / 
                        right->payload.real
                    ); break;

                case GREATER:
                {
                    bool res = left->payload.real > right->payload.real;
                    left->type = res? DATA_TRUE: DATA_FALSE;
                    left->payload.boolean = res;
                    break;
                }
                case GREATER_EQUAL:
                {
                    bool res = left->payload.real >= right->payload.real;
                    left->type = res? DATA_TRUE: DATA_FALSE;
                    left->payload.boolean = res;
                    break;
                }
                case LESS:
                {
                    bool res = left->payload.real < right->payload.real;
                    left->type = res? DATA_TRUE: DATA_FALSE;
                    left->payload.boolean = res;
                    break;
                }
                case LESS_EQUAL:
                {
                    bool res = left->payload.real <= right->payload.real;
                    left->type = res? DATA_TRUE: DATA_FALSE;
                    left->payload.boolean = res;
                    break;
                }
                case BANG_EQUAL:
                {
                    bool res = left->payload.real != right->payload.real;
                    left->type = res? DATA_TRUE: DATA_FALSE;
                    left->payload.boolean = res;
                    break;
                }
                case EQUAL_EQUAL:
                {
                    bool res = left->payload.real == right->payload.real;
                    left->type = res? DATA_TRUE: DATA_FALSE;
                    left->payload.boolean = res;
                    break;
                }
                default:
                    goto INVALID_OPERATOR;
            }
            break;
        }

        default:
            INVALID_OPERATOR:
            SET_STATUS(status, EVALUATE_INVALID_ARITHMETIC,
                "invalid primitive operator (%s ID: %d)", 
                STR(TokenType), op);
            return NULL;
    }
    return left;
    
    DIVISION_BY_ZERO:
    SET_STATUS(status, EVALUATE_INVALID_ARITHMETIC,
        "divide by 0.");
    return NULL;
}

Value* evaluate_binary(Status* status, 
    Interpreter* interpreter, Binary* binary)
{

    /*
    primitive interacting 
    int double true false on +-/*

    dynamic interacting (array, str and list) (countables)
        array/vector + array/vector = vector
        array/vector * int = vector

        str + str = str
        str * int = str
    */

    Value* left = evaluate_expr_dispatch(status, 
        interpreter, binary->left);
    if (!STATUS_OK(status)) goto ERROR_LEFT;

    Value* right = evaluate_expr_dispatch(status, 
        interpreter, binary->right);
    if (!STATUS_OK(status)) goto ERROR_RIGHT;
    
    // primitive
    if (value_is_primitive(left->type) && value_is_primitive(right->type))
    {
        left = evaluate_binary_primitive_mutate(status, 
            left, 
            right, 
            binary->op
        );
        if (!STATUS_OK(status)) goto PRIMITIVE_ERROR;
    }
    else
    {
        SET_STATUS(status, NOT_YET_IMPLEMENTED, 
            "not yet implemented evaluate_binary_composite.");
        goto COMPOSITE_ERROR;
    }

    value_destruct(status, right);
    if (!STATUS_OK(status)) return NULL;

    return left;

    PRIMITIVE_ERROR:
    COMPOSITE_ERROR:
    value_destruct(status, left);
    value_destruct(status, right);
    return NULL;

    ERROR_RIGHT:
    value_destruct(status, left);

    ERROR_LEFT:
    return NULL;
}

Value* evaluate_unary(Status* status, Interpreter* interpreter, Unary* unary)
{
    Value* value = evaluate_expr_dispatch(status, interpreter, unary->expr);
    if (!STATUS_OK(status)) goto ERROR_REACH;
    
    switch(unary->op)
    {
        case SIGN:
        {
            // only deal with primitive values (this)
            switch (value->type)
            {
                case DATA_INT:
                    value->payload.integer = -value->payload.integer; 
                    break;

                case DATA_DOUBLE:
                    value->payload.real = -value->payload.real;
                    break;

                case DATA_TRUE:
                case DATA_FALSE:
                {
                    value_primitive_conversion(status, value, DATA_INT);
                    if (!STATUS_OK(status)) goto ERROR_REACH;
                    value->payload.integer = -value->payload.integer; 
                    break;
                }
                default:
                    SET_STATUS(status, EVALUATE_INVALID_ARITHMETIC, 
                        "invalid arithmetic operation in unary. (%s id: %d)",
                        STR(TokenType), unary->op
                    );
                    goto ERROR_REACH;
            }
            break;
        }
        case BANG:
        {
            value_is_not_truthy(status, value);
            if (!STATUS_OK(status)) goto ERROR_REACH;
            break;
        }
        default:
        {
            SET_STATUS(status, EVALUATE_INVALID_ARITHMETIC, 
                "invalid arithmetic operation in unary. (%s id: %d)",
                STR(TokenType), unary->op
            );
            goto ERROR_REACH;
        }
    }
    return value;

    ERROR_REACH:
    value_destruct(status, value);
    return NULL;
}
    

Value* evaluate_assign(Status* status, Interpreter* interpreter, Assign* assign)
{
    Value* eax_value = evaluate_expr_dispatch(status, interpreter, assign->rvalue); // register
    if (!STATUS_OK(status)) goto EAX_ERROR;

    // get the word in the environment dict, ready for primitive memcpy
    Value* origin =  interpreter_lookup_variable(status, interpreter, assign->_name);
    if (!STATUS_OK(status)) goto LOOK_UP_ERROR;

    // memcpy eax into the variable's slot (shallow copy)
    memcpy(origin, eax_value, sizeof(*eax_value));
    return eax_value;

    LOOK_UP_ERROR: 
    value_destruct(status, eax_value);
     
    EAX_ERROR:
    return NULL;
}

Value* evaluate_variable(Status* status, Interpreter* interpreter, Variable* variable)
{
    const Value* origin = interpreter_lookup_variable(status, interpreter, variable->_name);
    if (!STATUS_OK(status)) return NULL;

    // create a shallow copy of the subject
    Value* mirror = value_allocate(status);
    if (!STATUS_OK(status)) return NULL;

    memcpy(mirror, origin, sizeof(*mirror));
    return mirror;     
}

/*
the blue print
Value* evaluate_binary(Status* status, Interpreter* interpreter, Binary* binary)
{
    Value* left = evaluate_expr_dispatch(status, interpreter, binary->left);
    if (!STATUS_OK(status)) return NULL;

    Value* right = evaluate_expr_dispatch(status, interpreter, binary->right);
    if (!STATUS_OK(status))
    {
        value_destruct(status, &left);
        return NULL;
    }

    // Apply operation to left and right.
    // Store result in left.

    value_destruct(status, &right);

    return left;
}
*/
