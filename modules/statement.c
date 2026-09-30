#include "statement.h"
#include "parser.h"
#include "../misc/assert_.h"


// after every layer, the pointer must appear after the done processing layer, 
// standing at the current's processing token
void ParserParse(Parser* parser)
{
    while (parser->current >= parser->tokens->length)
    {
        HeapAdd(parser->statements, ParserDeclaration(parser));
    }
}

Expression* ParserDeclaration(Parser* parser)
{
    TokenType type = ParserTokenPeek(parser)->type;
    switch (type)
    {
        case VAR:
            ParserVarDeclaration(parser);
            break;
        default:
            ParserStatement(parser);
            break; 
    }
    Token* token = ParserTokenPeek(parser);
    ASSERT_VARIADIC(token->type != SEMICOLON, 
        "expect a ';' after an expression.");
}

Expression* ParserStatement(Parser* parser)
{
    if (ParserTokenPeek(parser)->type == PRINT)
        return ParserPrintStmt(parser);
    return ParserExprStmt(parser);
}
void ParserVarDeclaration(Parser* parser)
{
    // var x = expression;
    // NOTE: pls

    // skip the VAR token
    parser->current++;
    
    // catch the IDENTIFIER
    Token* identifier = ParserTokenPeek(parser);
    ASSERT_VARIADIC(identifier->type != IDENTIFIER, 
        "var not found. (%s)", identifier->lexeme);
    parser->current++;

    ASSERT(ParserTokenPeek(parser), "where is the '='?");
    parser->current++;

    // build a tree & evaluate
    ExStationEvaluateBuild(parser->station);
    Expression* expr = ParserExpression(parser);
    Expression* literal = NULL;

    // free
    ExStationLoadFree(parser->station);
}

Expression* ParserExprStmt(Parser* parser)
{
    // recursive descent parsing
    Expression* tree = ParserExpression(parser);
    
    // evaluate
    ExStationEvaluateBuild(parser->station);
    Expression* literal = tree->connect(parser->station, tree, 0);
    
    // free
    ExStationLoadFree(parser->station);
    tree->connect(parser->station, tree, 0);

    return literal;
}
void* ParserPrintStmt(Parser* parser)
{
    // grammar: PrintStmt -> print Expression
    parser->current++;

    // recursive descent parsing
    Expression* tree = ParserExpression(parser);

    // evaluate 
    ExStationEvaluateBuild(parser->station);
    Expression* literal = tree->connect(parser->station, tree, 0);

    // print
    int indent = 0; 
    void* args[1] = {&indent};
    ExStationDebugPrintPortLiteral(parser->station, literal, 1, args);

    // free the arithmetic tree
    void* args1[0] = {};
    ExStationFreePortLiteral(parser->station, tree, 0, args1);

    return NULL;
    
}
