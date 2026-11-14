#include "stmt.h"

#include "../misc/misc.h"


void stmt_debug_dispatch(Status* status, void* stmt, 
    int indent)
{
    StmtType type = AS(Stmt*, stmt)->type;
    switch(type)
    {
        case STMT:
            stmt_debug_stmt(status, stmt, indent);
            break;

        case PRINT_STMT:
            stmt_debug_print_stmt(status, stmt, indent);
            break;

        case BLOCK_STMT:
            stmt_debug_block_stmt(status, stmt, indent);
            break;

        case VARDECL_STMT:
            stmt_debug_var_declare_stmt(status, stmt, indent);
            break;

        default:
            SET_STATUS(status, AST_STMT_UNKNOWN_STMT_TYPE, 
                "found an unknown stmt type (enum id: %d).", type);
            break;
    }
}

void stmt_debug_stmt(Status* status, Stmt* stmt, 
    int indent)
{
    PRINT_TABS(indent); printf("STMT:\n");

    indent++;
    expr_debug_dispatch(status, stmt->expression, indent);
    indent--;
}

void stmt_debug_print_stmt(Status* status, PrintStmt* print_stmt, 
    int indent)
{
    PRINT_TABS(indent); printf("PRINT:\n");

    indent++;
    expr_debug_dispatch(status, print_stmt->expression, indent);
    indent--;
}

void stmt_debug_block_stmt(Status* status, BlockStmt* block_stmt, 
    int indent)
{
    PRINT_TABS(indent); printf("BLOCK\n");
    PRINT_TABS(indent); printf("{\n");
    indent++;
    Stmt** stmts = (Stmt**)block_stmt->head;
    for (int i = 0; i < block_stmt->length; i++)
    {
        Stmt* stmt = stmts[i];
        stmt_debug_dispatch(status, stmt, indent);
        if (!STATUS_OK(status)) return;
    }
    indent--;
    PRINT_TABS(indent); printf("}\n");
}

void stmt_debug_var_declare_stmt(Status* status, VarDeclareStmt* var_declare_stmt, 
    int indent)
{
    PRINT_TABS(indent); printf("VAR DECL: %s\n", var_declare_stmt->name);

    indent++;
    PRINT_TABS(indent); printf("initializer: \n");

    Expr* initializer = var_declare_stmt->initializer;
    if (!initializer)
    {
        PRINT_TABS(indent + 1);printf("NULL:D\n");
    }
    else
        expr_debug_dispatch(status, initializer, indent + 1);
    indent--;
}