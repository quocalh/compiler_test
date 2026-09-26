#include "parser.h"
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
    };
    return ptr;
}
void ParserDestruct(Parser* parser)
{
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
// wrapper
Token* ParserTokenPeek(Parser* parser)
{
    return ParserTokenPeekCustom(parser, parser->current);
}


// after every layer, the pointer must appear after the done processing layer, 
// standing at the current's processing token
void ParserParse(Parser* parser);
Expression* ParserExpression(Parser* parser)
{
    // for now
    return ParserTerm(parser);
}
Expression* ParserEquality(Parser* parser);
Expression* ParserComparison(Parser* parser);
Expression* ParserTerm(Parser* parser)
{
    void* a = ParserPrimary(parser);
    while (parser->current < parser->tokens->length)
    {
        Token* op_token = ParserTokenPeek(parser);
        TokenType op = op_token->type;
        if (op != PLUS &&
            op != MINUS) break;

        parser->current++;
        void* b = ParserPrimary(parser);
        a = ExBinaryInit(a, b, op);
    }
    return a;
    
}
Expression* ParserFactor(Parser* parser);
Expression* ParserUnary(Parser* parser);
Expression* ParserExponent(Parser* parser);
Expression* ParserGrouping(Parser* parser);
Expression* ParserPrimary(Parser* parser)
{
    Token* token = ParserTokenPeek(parser);
    void* literal;
    TokenType type;
    
    switch (token->type)
    {
        case INT:
        case DOUBLE: 
        case STRING:
            parser->current++;
            literal = ExLiteralInit(token->literal, token->type);
            break;
        case LEFT_PAREN:
            parser->current++; // skip the left paren
            
            // search for the closing right paren (check valid)
            int stack_layer = 1;
            int search_index = parser->current - 1;
            Token* chkpnt;
            while (true)
            {
                chkpnt = ParserTokenPeekCustom(parser, ++search_index);
                
                if (search_index >= parser->tokens->length) ERROR("right paren not found.");

                if (chkpnt->type == LEFT_PAREN) stack_layer++;
                else if (chkpnt->type == RIGHT_PAREN) stack_layer--;
                
                // found the right closing right paren
                if (stack_layer <= 0) break;
            }

            // done validate the grammar, proceed to continue the expression creation 
            literal = ParserExpression(parser);

            // skip the right paren
            if (ParserTokenPeek(parser)->type != RIGHT_PAREN) ERROR("your arithmetic seems to be broken.")
            parser->current++; 
            break;
    }
    return literal;
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
            // printf("%d", *(int*)literal->literal));
            printf("%d", *AS(int*, literal->literal));
            break;
        case DOUBLE:
            printf("%lf", *((double*)literal->literal));
            break;
        case STRING:
            printf("%s", (char*)literal->literal);
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