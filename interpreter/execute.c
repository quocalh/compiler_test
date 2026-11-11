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
    // value_destruct(status, value);
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
    printf("hello world :D\n");
}