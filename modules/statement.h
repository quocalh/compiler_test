#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"
#include "parser.h"

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
    const char* name;
    Expression* expr;  
} DeclareStmt;

typedef struct
{
    void* (*port_expr_stmt)(STATION_ARG_CONFIG);
    void* (*port_print_stmt)(STATION_ARG_CONFIG);
    void* (*declare_smth)(STATION_ARG_CONFIG);
} StmtStation;

void ParserParse(Parser* parser);
Expression* ParserDeclaration(Parser* parser);

Expression* ParserStatement(Parser* parser);
void ParserVarDeclaration(Parser* parser);

Expression* ParserExprStmt(Parser* parser);
void* ParserPrintStmt(Parser* parser);


#endif