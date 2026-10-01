#include "intepreter.h"
#include "statement.h"
#include "parser.h"
#include "../misc/assert_.h"
#include "../misc/misc.h"

#define EVALUATE_AN_EXPRESSION(parser, expr, literal) \
    do{ \
        /* build the arithmetic tree & evaluate the tree */\
        expr = ParserExpression(parser); \
        ExStationLoadEvaluate(parser->ex_station); \
        literal = expr->connect(parser->ex_station, expr, 1, parser); \
        \
        /* free the expression*/ \
        ExStationLoadFree(parser->ex_station); \
        expr->connect(parser->ex_station, expr, 0); \
    } while(0) 

#define SEMICOLON_CONSUME(parser, token) \
    do{ \
        ASSERT_VARIADIC((token = ParserTokenPeek(parser))->type == SEMICOLON,  \
            "semicolon not found (line %d)", token->line); \
        parser->current++; \
    } while(0)

// interpret
Interpreter* InterpreterInit(Parser* parser)
{
    Interpreter* inp = HeapInsInit(sizeof(*inp));
    *inp = (Interpreter){
        // unique attribs
        .current = 0,
        
        // subscribe attribs
        .env = parser->env, // not a unique attribs cuz of stmtvardeclaration()
        .ex_station = parser->ex_station,
        .stmt_station = parser->stmt_station,
        .stmts = parser->statements,
    };
}
void* InterpreterDestruct(Interpreter* inp)
{
    free(inp);
}
void* InterpreterPeekStatement(Interpreter* inp)
{
    ASSERT(inp->current < inp->stmts->length
        , "statement index error.");
    void** stmt = inp->stmts->ptr;
    return stmt[inp->current];
}
void InterpreterInterpret(Interpreter* inp)
{
    StmtStationLoadExecute(inp->stmt_station);
    while (inp->current < inp->stmts->length)
    {
        Stmt* stmt = InterpreterPeekStatement(inp);
        stmt->connect(inp->stmt_station, stmt, 1, inp->env);
        inp->current++;
    }
}

// parser parse 
void ParserParse(Parser* parser)
{
    parser->current = 0;
    while (parser->current < parser->tokens->length)
    {
        void* stmt = ParserDeclaration(parser);
        HeapAdd(parser->statements, &stmt);
        // ASSERT(HeapAdd(parser->statements, &stmt), "failed to add a statement.");
    }
}  
void* ParserDeclaration(Parser* parser)
{
    if (ParserTokenPeek(parser)->type == VAR) 
    {
        return ParserVarDeclaration(parser);
    }
    return ParserStatement(parser);
}
void* ParserVarDeclaration(Parser* parser)
{
    // var <identifier> = <expr>;
    Token* token;
    char* name;
    
    // pass the VAR token
    parser->current++;

    ASSERT_VARIADIC((token = ParserTokenPeek(parser))->type == IDENTIFIER, 
        "expect an identifier (line %d)", token->line);
    name = HeapInsInit(sizeof(char) * (token->lexeme->length + 1));
    strcpy(name, token->lexeme->str);
    parser->current++;
    
    ASSERT_VARIADIC((token = ParserTokenPeek(parser))->type == EQUAL, 
        "expect an '=' (line%d)", token->line);
    parser->current++;

    Expression* expr, *literal; 
    EVALUATE_AN_EXPRESSION(parser, expr, literal);

    SEMICOLON_CONSUME(parser, token);

    return StmtDeclareStmtInit(name, literal);
}
void* ParserStatement(Parser* parser)
{
    Token* token;
    if ((token = ParserTokenPeek(parser))->type == PRINT)
    {
        return ParserPrintStmt(parser);
    }
    return ParserExprStmt(parser);
}
void* ParserExprStmt(Parser* parser)
{
    Expression* expr, *literal;
    EVALUATE_AN_EXPRESSION(parser, expr, literal);

    Token* token;
    SEMICOLON_CONSUME(parser, token);

    return StmtExprStmtInit(literal);
}
void* ParserPrintStmt(Parser* parser)
{
    // print <expr>;

    // skip the PRINT token
    parser->current++;

    Expression* expr, *literal; 
    EVALUATE_AN_EXPRESSION(parser, expr, literal);
    
    Token* token;
    SEMICOLON_CONSUME(parser, token);

    return StmtPrintStmtInit(literal);
}

void StmtStationLoadExecute(StmtStation* station)
{
    station->port_stmt = StmtStationExecutePortStmt;
    station->port_print_stmt = StmtStationExecutePortPrintStmt;
    station->port_expr_stmt = StmtStationExecutePortExprStmt;
    station->port_declare_smth = StmtStationExecutePortDeclareStmt;
}
void* StmtStationExecutePortStmt(STATION_ARG_CONFIG)
{
    printf("hi :D\n");
    Stmt* stmt = self;
    return stmt->expr; 
}
void* StmtStationExecutePortPrintStmt(STATION_ARG_CONFIG)
{
    PrintStmt* stmt = self;
    ExpressionPrintLiteral(stmt->expr);
    return NULL;
}
void* StmtStationExecutePortExprStmt(STATION_ARG_CONFIG)
{
    printf("hiexpr :D\n");
    ExprStmt* stmt = self;
    return stmt->expr; 
}
void* StmtStationExecutePortDeclareStmt(STATION_ARG_CONFIG)
{
    DeclareStmt* stmt = self;

}

void StmtStationLoadFree(StmtStation* station)
{
    station->port_stmt = StmtStationFreePortStmt;
    station->port_print_stmt = StmtStationFreePortPrintStmt;
    station->port_expr_stmt = StmtStationFreePortExprStmt;
    station->port_declare_smth = StmtStationFreePortDeclareStmt;
}
void* StmtStationFreePortStmt(STATION_ARG_CONFIG)
{
    Stmt* stmt = self; 
    ExpressionFreeLiteral(stmt->expr);
    free(self);
}
void* StmtStationFreePortPrintStmt(STATION_ARG_CONFIG)
{
    PrintStmt* stmt = self; 
    ExpressionFreeLiteral(stmt->expr);
    free(self);
}
void* StmtStationFreePortExprStmt(STATION_ARG_CONFIG)
{
    ExprStmt* stmt = self; 
    ExpressionFreeLiteral(stmt->expr);
    free(self);
}
void* StmtStationFreePortDeclareStmt(STATION_ARG_CONFIG)
{
    DeclareStmt* stmt = self; 
    ExpressionFreeLiteral(stmt->expr);
    free(stmt->name);
    free(self);
}
