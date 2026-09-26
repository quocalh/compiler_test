#include <stdio.h>
 #include "modules/scanner.h"
#include "modules/parser.h"
#include "misc/hashmap.h"
#include "modules/expression.h"

/*
TODO: 
    PARSER: 
        PRINT DEBUG TEST THE PARSER (DONE)
        CREATE AND TEST THE FREE VISITOR FAMILY (DONE)
        SKETCHING ON THE ERROR FINDING
            CONSIDER PUTTING THE TRY CATCH 
                *NOTE: FREEING THING IN THAT SCOPE
                    HOW TO?
                TRY CATCH SYSTEM THAT DELETE AND FREE EVERYTHING IN THAT SCOPE?
*/
int main()
{ 
    System* system = SystemInit("src2.txt");
    Scanner* scanner = ScannerInit(system);
    Parser* parser = ParserInit(system);

    ScannerScan(scanner, system);

    // expression visitor pattern testing (larp polymorphism)
    // let only literal and expression 
    // run accept both, and let's see

    double* n0 = malloc(sizeof(*n0)); *n0 = 67.67;
    double* n1 = malloc(sizeof(*n1)); *n1 = 123.456;
    double* n3 = malloc(sizeof(*n3)); *n3 = 45.456;
    double* n4 = malloc(sizeof(*n4)); *n4 = 36.36;
    char* n2 = malloc(sizeof(char) * 7); strcpy(n2, "yes");

    Expression* expression =  ExpExpressionInit(
        ExBinaryInit(
            ExLiteralInit(n0, DOUBLE),
            ExBinaryInit(
                ExLiteralInit(n1, DOUBLE),
                ExBinaryInit(
                    ExLiteralInit(n3, DOUBLE),
                    ExLiteralInit(n4, DOUBLE),
                    MINUS
                ),
                PLUS
            ),
            PLUS 
        )
    );

    ExStation* station = ExStationInit();

    // do the thing
    int indent = 0;
    ExStationLoadDebugPrint(station);
    expression->connect(station, expression, 1, &indent);
    ExStationLoadFree(station);
    expression->connect(station, expression, 0);
    free(n0);
    free(n1);
    free(n2);
    free(n3);
    free(n4);
    printf("\n");

    // parser
    Expression* res = ParserExpression(parser);
    indent = 0;
    ExStationLoadDebugPrint(station);
    res->connect(station, res, 1, &indent);
    ExStationLoadFree(station);
    res->connect(station, res, 0);

    // end
    ExStationDestruct(station);

    ScannerDestruct(scanner);
    ParserDestruct(parser);
    SystemDestruct(system);


    return 0; 
}
