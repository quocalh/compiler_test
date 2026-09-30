#include "statement.h"
#include "parser.h"
#include "../misc/assert_.h"


// after every layer, the pointer must appear after the done processing layer, 
// standing at the current's processing token
void ParserParse(Parser* parser)
{
    while (parser->current < parser->tokens->length)
    {
        HeapAdd(parser->statements, ParserDeclaration(parser));
    }
}

Expression* ParserDeclaration(Parser* parser)
{
    Expression* literal = NULL;
    TokenType type = ParserTokenPeek(parser)->type;
    switch (type)
    {
        case VAR:
            ParserVarDeclaration(parser);
            break;
        default:
            literal = ParserStatement(parser);
            break; 
    }
    Token* token = ParserTokenPeek(parser);
    ASSERT_VARIADIC(token->type == SEMICOLON, 
        "expect a ';' after an expression.");
    parser->current++;

    return literal;
}

Expression* ParserStatement(Parser* parser)
{
    if (ParserTokenPeek(parser)->type == PRINT)
        return ParserPrintStmt(parser);
    return ParserExprStmt(parser);
}
void ParserVarDeclaration(Parser* parser)
{
    // var x = expression

    // skip the VAR token
    parser->current++;
    
    // catch the IDENTIFIER
    Token* identifier = ParserTokenPeek(parser);
    ASSERT_VARIADIC(identifier->type != IDENTIFIER, 
        "var not found. (%s)", identifier->lexeme);
    parser->current++;

    // catch the = (if catched, then fetch the following expression)
    if (ParserTokenPeek(parser)->type == EQUAL)
    {
        parser->current++;
        
        // build a tree & evaluate
        void* args[0];
        Expression* expr = ParserExpression(parser);
        ExStationEvaluateBuild(parser->ex_station);
        Expression* literal = expr->connect(parser->ex_station, expr, 0);

        // free
        ExStationLoadFree(parser->ex_station);
        expr->connect(parser->ex_station, expr, 0);

        // 
    }

}

Expression* ParserExprStmt(Parser* parser)
{
    // recursive descent parsing
    Expression* tree = ParserExpression(parser);
    
    // evaluate
    ExStationEvaluateBuild(parser->ex_station);
    Expression* literal = tree->connect(parser->ex_station, tree, 0);
    
    // free
    ExStationLoadFree(parser->ex_station);
    tree->connect(parser->ex_station, tree, 0);

    return literal;
}
void* ParserPrintStmt(Parser* parser)
{
    // grammar: PrintStmt -> print Expression
    parser->current++;

    // recursive descent parsing
    Expression* tree = ParserExpression(parser);

    // evaluate 
    ExStationEvaluateBuild(parser->ex_station);
    Expression* literal = tree->connect(parser->ex_station, tree, 0);

    // print
    int indent = 0; 
    void* args[1] = {&indent};
    ExStationDebugPrintPortLiteral(parser->ex_station, literal, 1, args);

    // free the arithmetic tree
    void* args1[0] = {};
    ExStationFreePortLiteral(parser->ex_station, tree, 0, args1);

    return literal;
    
}
