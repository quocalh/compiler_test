#include "intepreter.h"
#include "statement.h"
#include "parser.h"
#include "../misc/assert_.h"
#include "../misc/misc.h"
#include "../misc/uthash.h"

// #define BUILD_AN_EXPRESSION(parser, expr)\
//     do{ \
//         expr = ParserExpression(parser); \
//     } while(0);
#define EVALUATE_AN_EXPRESSION_AND_FREE_EXPRESSION(parser, expr, literal) \
    do{ \
        /* evaluate the tree */\
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
        .env = NULL,
        .stmts = HeapInit(sizeof(void*)),
        
        // subscribe attribs
        .ex_station = parser->ex_station,
        .stmt_station = parser->stmt_station,
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


void InterpreterEnvironmentDestruct(Interpreter* inp)
{
    Environment* env = inp->env;
    ASSERT(env->enclosing == NULL, "the block scope is unfinished.");

    VarMap* current; 
    VarMap* tmp;
    HASH_ITER(hh, env->map, current, tmp)
    {
        HASH_DEL(env->map, current);
        Literal* literal = current->ptr;
        ExpressionFreeLiteral(literal);
    }
    free(inp->env);
}
void InterpreterStatementsFree(Interpreter* inp)
{
    Heap* stmts = inp->stmts;
    HeapFree(stmts);
}

void InterpreterInterpret(Interpreter* inp, Parser* parser) 
{
    // create the root environment 
    inp->env = EnvironmentInit();
    parser->env = inp->env;

    // create statements
    inp->stmts = HeapInit(sizeof(void*));
    parser->stmts = inp->stmts;

    ParserParse(parser, inp);

    // execute
    StmtStationLoadExecute(inp->stmt_station);
    inp->current = 0;
    while (inp->current < inp->stmts->length)
    {
        Stmt* stmt = InterpreterPeekStatement(inp);
        stmt->connect(inp->stmt_station, stmt, 1, parser);
        inp->current++;
    }

    // delete the env after done using
    EnvironmentDestruct(inp->env);

    // delete statements
    InterpreterStatementsFree(inp);
}

// parser parse 
void ParserParse(Parser* parser, Interpreter* inp)
{
    parser->stmts = inp->stmts;

    parser->current = 0;
    while (parser->current < parser->tokens->length)
    {
        void* stmt = ParserDeclaration(parser);
        HeapAdd(parser->stmts, &stmt);
        // ASSERT(HeapAdd(parser->stmts, &stmt), "failed to add a statement.");
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
    
    // skip the VAR token
    parser->current++;

    // deal witht the l-value
    ASSERT_VARIADIC((token = ParserTokenPeek(parser))->type == IDENTIFIER, "expect an identifier (line %d)", token->line);
    name = token->lexeme->str;
    parser->current++;
    // ONLY IF INTEPRETER IS DESTRUCTED BEFORE SYSTEM (WHICH IS ALWAYS THE CASE (PREASSUMPTION)
    // name = HeapInsInit(sizeof(char) * (token->lexeme->length + 1));
    // strcpy(name, token->lexeme->str);

    
    // skip the equal(=)
    ASSERT_VARIADIC((token = ParserTokenPeek(parser))->type == EQUAL, 
        "expect an '=' (line%d)", token->line);
    parser->current++;

    // evaluate the expression (r-value)
    Expression* expr = ParserExpression(parser);

    // skip the ;
    SEMICOLON_CONSUME(parser, token);

    return StmtDeclareStmtInit(name, expr);
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
    Expression* expr = ParserExpression(parser);

    Token* token;
    SEMICOLON_CONSUME(parser, token);

    return StmtExprStmtInit(expr);
}
void* ParserPrintStmt(Parser* parser)
{
    // print <expr>;

    // skip the PRINT token
    parser->current++;

    Expression* expr = ParserExpression(parser); 
    
    Token* token;
    SEMICOLON_CONSUME(parser, token);

    return StmtPrintStmtInit(expr);
}

void StmtStationLoadExecute(StmtStation* station)
{
    station->port_stmt = StmtStationExecutePortStmt;
    station->port_print_stmt = StmtStationExecutePortPrintStmt;
    station->port_expr_stmt = StmtStationExecutePortExprStmt;
    station->port_declare_smth = StmtStationExecutePortVarDeclareStmt;
}
void* StmtStationExecutePortStmt(STATION_ARG_CONFIG)
{
    printf("hi :D\n");
    Parser* parser = args[0];
    Stmt* stmt = self;
    Literal* literal;
    EVALUATE_AN_EXPRESSION_AND_FREE_EXPRESSION(parser, stmt->expr, literal);
    ExpressionFreeLiteral(literal);
    return NULL; 
}
void* StmtStationExecutePortPrintStmt(STATION_ARG_CONFIG)
{
    Parser* parser = args[0];
    PrintStmt* stmt = self;
    Literal* literal;
    EVALUATE_AN_EXPRESSION_AND_FREE_EXPRESSION(parser, stmt->expr, literal);
    ExpressionPrintLiteral(literal);
    ExpressionFreeLiteral(literal);
    return NULL;
}
void* StmtStationExecutePortExprStmt(STATION_ARG_CONFIG)
{
    printf("hiexpr :D\n");
    Parser* parser = args[0];
    ExprStmt* stmt = self;
    Literal* literal;
    EVALUATE_AN_EXPRESSION_AND_FREE_EXPRESSION(parser, stmt->expr, literal);
    ExpressionFreeLiteral(literal);
    return NULL; 
}
void* StmtStationExecutePortVarDeclareStmt(STATION_ARG_CONFIG)
{
    DeclareStmt* stmt = self;
    Parser* parser = args[0];
    Literal* literal;
    EVALUATE_AN_EXPRESSION_AND_FREE_EXPRESSION(parser, stmt->expr, literal);
    
    EnvironmentDefine(parser->env, stmt->name, literal);
    // VarMap* hello = VarMapFind(&(env->map), "a");
    // DO YOU THINK YOU SHOULD RETURN THIS (NO WE DONT'T RETURN ANYTHING)
    return NULL;
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
