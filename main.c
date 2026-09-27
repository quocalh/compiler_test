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
        CREATE A BARE MINIMUM FUNCTIONAL PARSER (DONE)
            SCALE IT TO ANOTHER ONE (DONE)
        SKETCHING ON THE ERROR FINDING (*)
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

    // parser phase
    ExStation* station = ExStationInit();

    Expression* res = ParserExpression(parser);
    int indent = 0;
    ExStationLoadDebugPrint(station);
    res->connect(station, res, 1, &indent);

    ExStationLoadFree(station);
    res->connect(station, res, 0);

    // parser phase end
    ExStationDestruct(station);

    ScannerDestruct(scanner);
    ParserDestruct(parser);
    SystemDestruct(system);


    return 0; 
}