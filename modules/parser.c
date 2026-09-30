#include "parser.h"
#include "statement.h"
#include <math.h>
#include "../misc/assert_.h"
#include "../misc/misc.h"
#define TAB "  "

#define STATION_SAFECHECK \
    if (!self) ERROR("you can't pass a null here");

/* 
 * [VISITOR EXPRESSION]: FIRST DEMO TEST WITH THE EXPRESSION TEST
 * [PARSER]: INSPECT, LISTING DOWN WHERE ERRORS CAN HAVE
 *  SIMULATE THE PROGRAM BY TABLET
 *  REVIEW HOW DID THE AUTHOR HANDLE ERRORS
 *  DESIGN A TRY CATCH SYSTEM TO SIMULATE THOSE
 */

Parser* ParserInit(System* system)
{
    Parser* ptr = HeapInsInit(sizeof(*ptr));
    *ptr = (Parser){
        .current = 0,

        .tokens = system->tokens,
        .file_name = system->file_name,

        .statements = HeapInit(sizeof(void*)),
        .ex_station = ExStationInit(),
        .stmt_station = StmtStationInit(),
        .env = EnvironmentInit(),
    };
    return ptr;
}
void ParserDestruct(Parser* parser)
{
    StmtStationDestruct(parser->stmt_station);
    ExStationDestruct(parser->ex_station);
    HeapFree(parser->statements);
    EnvironmentDestruct(parser->env);

    free(parser);
}

Token* ParserTokenPeekCustom(Parser* parser, int index)
{
    if (parser->current >= parser->tokens->length)
    {
        ERROR("peek out of bound.");
    }
    Token* tokens = parser->tokens->ptr;
    return tokens + index; // index * sizeof(Token);
}

Token* ParserTokenPeek(Parser* parser)
{
    // return ParserTokenPeekCustom(parser, parser->current);
    if (parser->current >= parser->tokens->length)
    {
        ERROR("peek out of bound.");
    }
    Heap* tokens = parser->tokens;
    return tokens->ptr + parser->current * sizeof(Token);
}

Expression* ParserExpression(Parser* parser)
{
    return ParserEquality(parser);
}
Expression* ParserAssign(Parser* parser)
{
    // return ParserEquality(parser);
}
Expression* ParserEquality(Parser* parser)
{
    void* expr = ParserComparison(parser);

    // equality = comparison ((!= | ==) comparison)*
    while (parser->current < parser->tokens->length)
    {
        TokenType op = ParserTokenPeek(parser)->type;
        if (op != EQUAL_EQUAL && op != BANG_EQUAL)
            break;
        
        parser->current++;

        void* rhs = ParserComparison(parser);
        expr = ExBinaryInit(expr, rhs, op);
    }

    return expr;
}
Expression* ParserComparison(Parser* parser)
{
    void* expr = ParserTerm(parser);

    // comparison = term ((!= | ==) term)*
    while (parser->current < parser->tokens->length)
    {
        TokenType op = ParserTokenPeek(parser)->type;
        if (op != LESS && op != LESS_EQUAL &&
            op != GREATER && op != GREATER_EQUAL)
            break;
        
        parser->current++;

        void* rhs = ParserTerm(parser);
        expr = ExBinaryInit(expr, rhs, op);
    }

    return expr;
}
Expression* ParserTerm(Parser* parser)
{
    void* expr = ParserFactor(parser);

    // term = factor ((+ | -) factor)*
    while (parser->current < parser->tokens->length)
    {
        TokenType op = ParserTokenPeek(parser)->type;

        if (op != PLUS && op != MINUS)
            break;

        parser->current++;

        void* rhs = ParserFactor(parser);
        expr = ExBinaryInit(expr, rhs, op);
    }

    return expr;
}
Expression* ParserFactor(Parser* parser)
{
    void* expr = ParserExponent(parser);

    // term = unary ((* | /) unary)*
    while (parser->current < parser->tokens->length)
    {
        TokenType op = ParserTokenPeek(parser)->type;

        if (op != STAR && op != SLASH)
            break;

        parser->current++;

        void* rhs = ParserExponent(parser);
        expr = ExBinaryInit(expr, rhs, op);
    }

    return expr;
}
Expression* ParserExponent(Parser* parser)
{
    void* expr = ParserUnary(parser);
    
    // exponent = primary ((** | ^) primary)*
    while (parser->current < parser->tokens->length)
    {
        TokenType op = ParserTokenPeek(parser)->type;
        if (op != HAT && op != EXPONENT)
            break;

        parser->current++;

        void* rhs = ParserUnary(parser);
        expr = ExBinaryInit(expr, rhs, op);
    }

    return expr;
}
Expression* ParserUnary(Parser* parser)
{

    // unary = (! | -)* exponent
    TokenType op = ParserTokenPeek(parser)->type;

    if (op != BANG & op != MINUS)
        return ParserPrimary(parser);

    parser->current++;
    void* expr = ParserUnary(parser);
    expr =  ExUnaryInit(expr, op);
    return expr;
}
Expression* ParserPrimary(Parser* parser)
{
    Token* token = ParserTokenPeek(parser);
    void* expr;
    
    switch (token->type)
    {
        case INT:
        case DOUBLE: 
        case STRING:
            parser->current++;
            expr = ExLiteralInit(token->literal, token->type);
            break;

        case IDENTIFIER:
            parser->current++;
            expr =  ExVariableInit(token->literal, token->lexeme->str);
            break;

        case LEFT_PAREN:
            parser->current++;
            expr = ParserGrouping(parser);
            break;

        default:
            ERROR("expect an expression.");
    }

    return expr;
}
Expression* ParserGrouping(Parser* parser)
{
    void* expr = ParserExpression(parser);

    if (ParserTokenPeek(parser)->type != RIGHT_PAREN)
        ERROR("expect a ')'.");
    
    parser->current++;
    return expr;
}

#define PRINT_TABS(cache, count)\
    do{ \
        cache = 0; \
        while (cache++ < *indent) printf("\t"); \
    }while(0)

// void* station, void* self, va_list ap
void* ExStationDebugPrintPortExpression(STATION_ARG_CONFIG)
{
    // fetch args
    int* indent = args[0];

    // print debug
    int i = 0; 
    while (i++ < *indent) printf("\t");
    printf("expression: \n");

    // recursively visit the child with copied arguments
    (*indent)++;
    Expression* child = ((Expression*)self)->expression;
    child->connect(station, child, argc, indent); 
    (*indent)--;

    return NULL;
}
// IMPORTANT NOTE: <Expression>->connect is using the PORT_ARG_CONFIG
// of which is the variadic format
// (i mean we don't pass the arg) but to pass the var directly into it
void* ExStationDebugPrintPortBinary(STATION_ARG_CONFIG)
{
    // fetch args
    int* indent = args[0];

    // recursively visit the child with copied arguments
    Expression* left = ((Binary*)self)->left;
    Expression* right = ((Binary*)self)->right;

    int i; 
    PRINT_TABS(i, *indent - 1);
    printf("binary{\n");

    PRINT_TABS(i, *indent - 1);
    printf("left: \n");
    (*indent)++;
    left->connect(station, left, argc, indent); 
    (*indent)--;

    PRINT_TABS(i, *indent - 1);
    printf("right: \n");
    (*indent)++;
    right->connect(station, right, argc, indent); 
    (*indent)--;

    PRINT_TABS(i, *indent - 1);
    printf("}\n");

    return NULL;

}
void* ExStationDebugPrintPortUnary(STATION_ARG_CONFIG)
{
    int* indent = args[0];
    Expression* expression = ((Unary*)self)->expression;

    int i;
    PRINT_TABS(i, *indent);
    printf("unary{\n");

    (*indent)++;
    expression->connect(station, expression, argc, indent);
    (*indent)--;

    PRINT_TABS(i, *indent);
    printf("}\n");

    return NULL;
}
void* ExStationDebugPrintPortLiteral(STATION_ARG_CONFIG)
{    
    // fetch the indentation
    int* indent = args[0];
    

    // print debug
    Literal* literal = self;
    int i = 0; 
    while (i++ < *indent) printf("\t", TAB);
    printf("(literal): ");
    
    switch (literal->type)
    {
        case INT:
            printf("%d", *AS(int*, literal->literal));
            break;
        case DOUBLE:
            printf("%lf", *((double*)literal->literal));
            break;
        case STRING:
            printf("%s", (char*)((StaticString*)literal->literal)->str);
            break;
        default:
            break;
    }
    printf("\n");

    return NULL;
}
void ExStationLoadDebugPrint(ExStation* station)
{
    station->port_expression = ExStationDebugPrintPortExpression;
    station->port_binary = ExStationDebugPrintPortBinary;
    station->port_unary = ExStationDebugPrintPortUnary;
    station->port_literal = ExStationDebugPrintPortLiteral;
}

// evaluating expressions
bool ExTruthCheck(TokenType type, void* literal)
{
    bool res = true;
    switch (type)
    {
        case NIL:
            res = false;

        case INT:
            if (VA(int*, literal) == 0) 
                res = false;
            break;

        case DOUBLE:
            if (VA(double*, literal) == 0) 
                res = false;
            break;

        case STRING:
            if (AS(StaticString*, literal)->length == 0)
                res = false;
            break;

        default:
            ERROR_VARIADIC("encounter a weird token (TOKEN ID: %d)", type);
    }
    return res;
}
void* ExStationEvaluatePortExpression(STATION_ARG_CONFIG)
{
    Expression* expr = self;
    return AS(Expression*, expr->expression)->connect(station, expr->expression, argc);
}
int size_of_token_type(TokenType type)
{
    size_t size;
    switch (type)
    {
        case INT:
            size = sizeof(int);
            break;
        case DOUBLE:
            size = sizeof(double);
            break;
        case STRING:
            size = sizeof(void*);
            ERROR("string have not been built to do these set of arithmetics.");
            break;
        case TRUE:
            size = sizeof(true);
            break;
        case FALSE:
            size = sizeof(false);
            break;
        case NIL:
            size = sizeof(NULL);
            break;
        default:
            ERROR_VARIADIC("why are you here (TokenType: %d)", type);
            break;
    }
    return size;
}
#define CONVERT_TYPECAST(type) \
    do{ \
        case INT\
    } while (0) \

void* ExStationEvaluatePortBinary(STATION_ARG_CONFIG)
{
    
    // extract the literal from children
    Binary* b = self;
    Literal* lhs = AS(Expression*, b->left)->connect(station, b->left, 0);
    Literal* rhs = AS(Expression*, b->right)->connect(station, b->right, 0);

    // determine the potential size of the result (uhmm, just get the larger onh)
    TokenType type = (size_of_token_type(lhs->type) > size_of_token_type(rhs->type))? lhs->type: rhs->type;
    size_t res_size = size_of_token_type(type);
    
    // dynamic typed language (so we have to determine the size beforehand)
    Literal* literal = ExLiteralInit(HeapInsInit(res_size), type);

    double c, d;
    if (lhs->type == DOUBLE)
        c = VA(double*, lhs->literal);
    else
        c = VA(int*, lhs->literal);

    if (rhs->type == DOUBLE)
        d = VA(double*, rhs->literal);
    else
        d = VA(int*, rhs->literal);

    TokenType op = AS(Binary*, self)->op;
    
    switch (type)
    {
        case DOUBLE:
        {
            double a = (lhs->type == DOUBLE)
                ? VA(double*, lhs->literal)
                : VA(int*, lhs->literal);

            double b = (rhs->type == DOUBLE)
                ? VA(double*, rhs->literal)
                : VA(int*, rhs->literal);

            switch (op)
            {
                case PLUS:
                    VA(double*, literal->literal) = a + b;
                    break;
                case MINUS:
                    VA(double*, literal->literal) = a - b;
                    break;
                case STAR:
                    VA(double*, literal->literal) = a * b;
                    break;
                case SLASH:
                    VA(double*, literal->literal) = a / b;
                    break;
                case HAT:
                    VA(double*, literal->literal) = pow(a, b);
                    break;
            }
            break;
        }

        case INT:
        {
            int a = VA(int*, lhs->literal);
            int b = VA(int*, rhs->literal);

            switch (op)
            {
                case PLUS:
                    VA(int*, literal->literal) = a + b;
                    break;
                case MINUS:
                    VA(int*, literal->literal) = a - b;
                    break;
                case STAR:
                    VA(int*, literal->literal) = a * b;
                    break;
                case SLASH:
                    VA(int*, literal->literal) = a / b;
                    break;
                case HAT:
                    VA(int*, literal->literal) = (int)(pow(a, b) + 0.001);
                    break;
            }
            break;
        }

        case EQUAL_EQUAL:
        case BANG_EQUAL:
        case LESS_EQUAL:
        case GREATER_EQUAL:
        case LESS:
        case GREATER:
            break;
        default:
            break;
    }        

    return literal;
}
void* ExStationEvaluatePortUnary(STATION_ARG_CONFIG)
{

}
void* ExStationEvaluatePortLiteral(STATION_ARG_CONFIG)
{
    Literal* source = self;
    Literal* copy = HeapInsInit(sizeof(*copy));
    *copy = *source;

    switch (source->type)
    {
        case INT:
            copy->literal = HeapInsInit(sizeof(int));
            *(int*)copy->literal = *(int*)source->literal;
            break;

        case DOUBLE:
            copy->literal = HeapInsInit(sizeof(double));
            *(double*)copy->literal = *(double*)source->literal;
            break;

        case STRING:
            copy->literal = StaticStringInit(((StaticString*)source->literal)->str);
            ASSERT(copy->literal != NULL, "failed to copy string literal");
            break;

        default:
            ERROR_VARIADIC("cannot copy literal (token type %d)", source->type);
    }

return copy;
}
void ExStationLoadEvaluate(ExStation* station)
{
    station->port_expression = ExStationEvaluatePortExpression;
    station->port_binary = ExStationEvaluatePortBinary;
    station->port_unary = ExStationEvaluatePortUnary;
    station->port_literal = ExStationEvaluatePortLiteral;
}

// free variables
void ExStationLoadFree(ExStation* station)
{
    station->port_expression = ExStationFreePortExpression;
    station->port_binary = ExStationFreePortBinary;
    station->port_unary = ExStationFreePortUnary;
    station->port_literal = ExStationFreePortLiteral;
}
void* ExStationFreePortExpression(STATION_ARG_CONFIG)
{
    Expression* child = ((Expression*)self)->expression;
    child->connect(station, child, 0);
    free(self);
}
void* ExStationFreePortBinary(STATION_ARG_CONFIG)
{
    Expression* left = ((Binary*)self)->left;
    Expression* right = ((Binary*)self)->right;
    left->connect(station, left, 0);
    right->connect(station, right, 0);
    free(self);
}
void* ExStationFreePortUnary(STATION_ARG_CONFIG)
{
    Expression* scalar = ((Unary*)self)->expression;
    scalar->connect(station, scalar, 0);
    free(self);
}
void* ExStationFreePortLiteral(STATION_ARG_CONFIG)
{
    Literal* literal = self;
    // DO NOTE THAT TOKENS ARE FREED BY THE SYSTEM AT THE END, THIS IS NOT OUR JOB
    // switch (literal->type)
    // {
    //     case INT:
    //     case DOUBLE:
    //         free(literal->literal);
    //         break;
    //     case STRING:
    //         StaticStringFree(literal->literal);
    //         break;
    //     default:    
    //         ERROR("who are you :sob:??")
    // }
    free(self);
}