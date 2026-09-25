#include "parser.h"
#include "../misc/assert_.h"
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

    // print debug

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
            printf("%d", ((int*)literal->literal));
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
    
    // passing down the arg pointer (ap) to the next one

    return NULL;
}
void ExStationLoadDebugPrint(ExStation* station)
{
    station->port_expression = ExStationDebugPrintPortExpression;
    station->port_binary = ExStationDebugPrintPortBinary;
    station->port_unary = ExStationDebugPrintPortUnary;
    station->port_literal = ExStationDebugPrintPortLiteral;
}