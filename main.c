#include <stdio.h>

#include "misc/assert_.h"
#include "modules/scanner.h"
#include "modules/parser.h"
#include "misc/hashmap.h"
#include "modules/expression.h"


int main()
{ 
    // System* system = SystemInit("src.txt");
    // Scanner* scanner = ScannerInit(system);

    // ScannerScan(scanner, system);

    // ScannerDestruct(scanner);
    // SystemDestruct(system);

    // expression visitor pattern testing (larp polymorphism)
    // let only literal and expression 
    // run accept both, and let's see

    double* n0 = malloc(sizeof(*n0)); *n0 = 67.67;
    double* n1 = malloc(sizeof(*n0)); *n1 = 123.456;
    char* n2 = malloc(sizeof(char) * 7); strcpy(n2, "nagger");
    Expression* expression =  ExpExpressionInit(
        ExBinaryInit(
        ExLiteralInit(n0, DOUBLE),
        ExUnaryInit(
            ExLiteralInit(n1, DOUBLE),
            // ExLiteralInit(n2, STRING),
            MINUS
        ),
        PLUS 
        )
    );

    ExStation* station = ExStationInit();
    ExStationLoadDebugPrint(station);

    // do the thing
    int indent = 0;
    expression->connect(station, expression, 1, &indent);

    free(n0);

    ExStationDestruct(station);
    return 0; 
}
