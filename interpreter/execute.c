#include "execute.h"
#include "evaluate.h"
#include "../misc/misc.h"
#include "../ast/stmt.h"
#include "interpreter.h"

void interpreter_execute_stmt_dispatch(Status* status, 
    Interpreter* interpreter, Stmt* stmt)
{
    switch(stmt->type)
    {
        case STMT:
            interpreter_execute_statement(status, 
                interpreter, (Stmt*)stmt);
            break;

        case PRINT_STMT:
            interpreter_execute_print_statement(status, 
                interpreter, (PrintStmt*)stmt);
            break;

        case VARDECL_STMT:
            interpreter_execute_vardecl_statement(status, 
                interpreter, (VarDeclareStmt*)stmt);
            break;

        default:
            SET_STATUS(status, EXECUTE_STMT_UNKNOWN_STMT_TYPE, 
                "encountered an unknown stmt type. (%s id: %d)", 
                STR(StmtType), stmt->type);
            return;
    }
}


void interpreter_execute_statement(Status* status, 
    Interpreter* interpreter, Stmt* stmt)
{
    Value* value = evaluate_expr_dispatch(status, 
        interpreter, stmt->expression);
    value_destruct(status, value);
}

void interpreter_execute_print_statement(Status* status, 
    Interpreter* interpreter, PrintStmt* stmt)
{
    Value* value = evaluate_expr_dispatch(status, 
        interpreter, stmt->expression);

    print_value(status, value);
    printf("\n");
    value_destruct(status, value);
}

void interpreter_execute_vardecl_statement(Status* status,
    Interpreter* interpreter, VarDeclareStmt* stmt)
{
    // composite related

    // get the init value
    Value* value;
    if (stmt->initializer)
    {
        value = evaluate_expr_dispatch(status, 
            interpreter, 
            stmt->initializer
        );
        if (!STATUS_OK(status)) goto ERROR_CONTROVERSIAL_TYPE;

        // since we are not yet ready to evaluate lists, we don't accept that for now
        // look into the lifetime problem in the codebase
        // will be looked into during variable implementation
        if (value->type > DATA_PRIMITIVE)
        {
            SET_STATUS(status, NOT_YET_IMPLEMENTED, 
                "the lifetime of the composite data is not yet settled (don't use it for now).");
            goto ERROR_CONTROVERSIAL_TYPE;
        }
    }
    else
    {
        value = NULL;
    }
    if (!STATUS_OK(status)) goto ERROR_CONTROVERSIAL_TYPE;

    HashStringToIns* slot;
    slot = hash_str_to_ins_find(status, &interpreter->cur_env->map, stmt->name);
    if (!STATUS_OK(status)) goto ERROR_CONTROVERSIAL_TYPE;

    // if not yet defined then fill in
    if (!slot)
    {
        hash_str_to_ins_add_key(status, &interpreter->cur_env->map, stmt->name, value); 
        if (!STATUS_OK(status)) goto ERROR_CONTROVERSIAL_TYPE;
    }
    // if already defined, then replace the value if it is not NULL
    else
    {
        if (value)
        {
            *(Value*)slot->ins = *value;
            value_destruct(status, value);
            // error status also counted as a success route
        }
    }

    return;

    ERROR_CONTROVERSIAL_TYPE:
    value_destruct(status, value);
    return;
}