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

        case BLOCK_STMT:
            interpreter_execute_block_statement(status, 
                interpreter, (BlockStmt*)stmt);
            break;
        
        case IF_STMT:
            interpreter_execute_if_statement(status, 
                interpreter, (IfStmt*)stmt);
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
    // if fail to evaluate, 
    // the evaluate will not return, nor create any trailing malloc
    if (!STATUS_OK(status)) return;

    print_value(status, value);
    printf("\n");
    
    value_destruct(status, value);
}

void interpreter_execute_block_statement(Status* status, 
    Interpreter* interpreter, BlockStmt* stmt)
{
    // create + assign new env
    Environment* new = environment_init(status, interpreter->cur_env);
    if (!STATUS_OK(status)) return;
    interpreter->cur_env = new;

    Stmt** stmts = (Stmt**)stmt->head;
    for (size_t i = 0; i < stmt->length; i++)
    {
        Stmt* inner_stmt = stmts[i];
        interpreter_execute_stmt_dispatch(status, interpreter, inner_stmt);
        if (!STATUS_OK(status)) goto FREE_NEW_ENV;
    }

    // failed -> return the prev env
    FREE_NEW_ENV:

    // reset prev env + destruct new env
    interpreter->cur_env = new->enclosing;
    interpreter_env_erase(status, new);
    environment_destruct(status, new);
}

static bool evaluate_truthy_bool_value(Value* value)
{
    return (value->payload.boolean);
}
void interpreter_execute_if_statement(Status* status, 
    Interpreter* interpreter, IfStmt* stmt)
{
    Value* value = evaluate_expr_dispatch(status, interpreter, stmt->cond);
    if (!STATUS_OK(status)) return;
    value_is_truthy(status, value);
    if (!STATUS_OK(status)) goto VALUE_DESTRUCT;

    bool cond_truthy = evaluate_truthy_bool_value(value);

    value_destruct(status, value);
    if (!STATUS_OK(status)) return;

    if (cond_truthy)
    {
        interpreter_execute_stmt_dispatch(status, interpreter, stmt->then_brnch);
    }
    else if (stmt->else_brnch)
    {
        interpreter_execute_stmt_dispatch(status, interpreter, stmt->else_brnch);
    }
    return;

    VALUE_DESTRUCT:
    value_destruct(status, value);
    return;
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
            stmt->initializer);
        if (!STATUS_OK(status)) goto ERROR_EVALUATE;

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
    if (!STATUS_OK(status)) goto ERROR_EVALUATE;


    HashStringToIns* slot;
    slot = hash_str_to_ins_find(status, &interpreter->cur_env->map, stmt->name);
    if (!STATUS_OK(status)) goto ERROR_EVALUATE;

    // if not yet defined then fill in
    if (!slot)
    {
        hash_str_to_ins_add_key(status, &interpreter->cur_env->map, stmt->name, value); 
        if (!STATUS_OK(status)) goto ERROR_EVALUATE;
    }
    // if already defined, then replace the value if it is not NULL
    else
    {
        if (value)
        {
            *(Value*)slot->ins = *value;
            value_destruct(status, value);
            // error status also go in the success route
            // to be catched in the parent func
        }
    }

    return;

    ERROR_CONTROVERSIAL_TYPE:
    ERROR_EVALUATE:
    value_destruct(status, value);
    return;
}