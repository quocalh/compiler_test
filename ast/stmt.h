#ifndef STMT_H
#define STMT_H

#include "../misc/error.h"
#include "../ast/expr.h"
#include "../ast/expr.h"
#include "../misc/vector.h"

typedef enum{
    STMT,
    PRINT_STMT,
    BLOCK_STMT,
    VARDECL_STMT,
} StmtType;

typedef struct{
    StmtType type; 
    Expr* expression;
} Stmt;

typedef struct{
    StmtType type; 
    Expr* expression;
} PrintStmt;

typedef struct{
    StmtType type;
} EndLineStmt;

typedef struct{
    StmtType type;
    Stmt* head;
    size_t length;
} BlockStmt;

typedef struct{
    StmtType type; 
    const char* name;
    Expr* initializer; 
} VarDeclareStmt;


// init (arena created struct set -> skipped)

// debug print ()
// indents are for control flow stmts
void stmt_debug_dispatch(Status* status, void* stmt, 
    int indent);

void stmt_debug_stmt(Status* status, Stmt* stmt, 
    int indent);
void stmt_debug_print_stmt(Status* status, PrintStmt* print_stmt, 
    int indent);
void stmt_debug_block_stmt(Status* status, BlockStmt* block_stmt, 
    int indent);

void stmt_debug_var_declare_stmt(Status* status, VarDeclareStmt* 
    var_declare_stmt, int indent);

// destruct (arena created struct set -> skipped)

#endif