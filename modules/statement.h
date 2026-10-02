#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"
// #include "parser.h"

typedef struct{
    VISITORBASE;
    Expression* expr;
} Stmt;
typedef struct{
    VISITORBASE;
    Expression* expr;
} ExprStmt;
typedef struct{
    VISITORBASE;
    Expression* expr;
} PrintStmt;
typedef struct{
    VISITORBASE;
    char* name;
    Expression* expr;  
} DeclareStmt; // var x = 5;
typedef struct{
    VISITORBASE;
    char* name;
    Expression* expr;  
} AssignStmt; // x = 5;

// init statement
Stmt* StmtStmtInit(Expression* expr);
DeclareStmt* StmtDeclareStmtInit(char* name, Expression* expr);
ExprStmt* StmtExprStmtInit(Expression* expr);
PrintStmt* StmtPrintStmtInit(Expression* expr);

typedef struct{
    void* (*port_stmt)(STATION_ARG_CONFIG);
    void* (*port_expr_stmt)(STATION_ARG_CONFIG);
    void* (*port_print_stmt)(STATION_ARG_CONFIG);
    void* (*port_declare_smth)(STATION_ARG_CONFIG);
} StmtStation;

StmtStation* StmtStationInit();
void StmtStationDestruct(StmtStation* station);

void* StmtPortStmt(PORT_ARG_CONFIG);
void* StmtPortExprStmt(PORT_ARG_CONFIG);
void* StmtPortPrintStmt(PORT_ARG_CONFIG);
void* StmtPortDeclareStmt(PORT_ARG_CONFIG);


#endif