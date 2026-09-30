#include "statement.h"
#include "parser.h"
#include "expression.h"
#include "../misc/misc.h"
#include "../misc/assert_.h"


Stmt* StmtStmtInit(Expression* expr)
{
    Stmt* stmt = HeapInsInit(sizeof(*stmt));
    *stmt = (Stmt){
        .connect = StmtPortStmt,
        .expr = expr,
    };
    return stmt;
}
ExprStmt* StmtExprStmtInit(Expression* expr)
{
    ExprStmt* expr_stmt = HeapInsInit(sizeof(*expr_stmt));
    *expr_stmt = (ExprStmt){
        .connect = StmtPortExprStmt,
        .expr = expr,
    };
    return expr_stmt;
}
PrintStmt* StmtPrintStmtInit(Expression* expr)
{
    PrintStmt* print_stmt = HeapInsInit(sizeof(*print_stmt));
    *print_stmt = (PrintStmt){
        .connect = StmtPortPrintStmt,
        .expr = expr,
    };
    return print_stmt;
}
DeclareStmt* StmtDeclareStmtInit(char* name, Expression* expr)
{
    DeclareStmt* declare_stmt = HeapInsInit(sizeof(*declare_stmt));
    *declare_stmt = (DeclareStmt){
        .connect = StmtPortDeclareStmt,
        .name = name,
        .expr = expr,
    };
    return declare_stmt;
}

// station
StmtStation* StmtStationInit()
{
    StmtStation* ptr = HeapInsInit(sizeof(*ptr));
    return ptr;
}
void StmtStationDestruct(StmtStation* station)
{
    free(station);
}


// statement port
void* StmtPortStmt(PORT_ARG_CONFIG)
{
    PORT_SAFECHECK; 
    UNPACK_VARIADIC_ARGS(argc, args);
    return AS(StmtStation*, station)->port_stmt(station, self, argc, args);
}
void* StmtPortExprStmt(PORT_ARG_CONFIG)
{
    PORT_SAFECHECK; 
    UNPACK_VARIADIC_ARGS(argc, args);
    return AS(StmtStation*, station)->port_expr_stmt(station, self, argc, args);
}
void* StmtPortPrintStmt(PORT_ARG_CONFIG)
{
    PORT_SAFECHECK; 
    UNPACK_VARIADIC_ARGS(argc, args);
    return AS(StmtStation*, station)->port_print_stmt(station, self, argc, args);
}
void* StmtPortDeclareStmt(PORT_ARG_CONFIG)
{
    PORT_SAFECHECK; 
    UNPACK_VARIADIC_ARGS(argc, args);
    return AS(StmtStation*, station)->port_declare_smth(station, self, argc, args);
}
