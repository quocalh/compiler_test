#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"
#include "parser.h"

typedef struct{
    Expression* expr;
    Literal* literal;
} Statement;

typedef struct{
    Expression* expr;
} ExprStmt;

typedef struct{
    Expression* expr;
} PrintStmt;

typedef struct{
    const char* name;
    Expression* expr;  
} DeclareStmt;

void ParserParse(Parser* parser);
Expression* ParserDeclaration(Parser* parser);

Expression* ParserStatement(Parser* parser);
void ParserVarDeclaration(Parser* parser);

Expression* ParserExprStmt(Parser* parser);
void* ParserPrintStmt(Parser* parser);


#endif