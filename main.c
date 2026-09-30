#include <stdio.h>
#include "modules/scanner.h"
#include "modules/parser.h"
#include "misc/hashmap.h"
#include "modules/expression.h"
#include "misc/assert_.h"
#include "modules/intepreter.h"

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

    // scanner phase
    ScannerScan(scanner, system);

    // parser phase
    ParserParse(parser);
    
    Interpreter* interpreter = InterpreterInit(parser);
    InterpreterInterpret(interpreter);
     

    /* OLD TEST
    // parser phase
    ExStation* station = ExStationInit();

    // test expression
    Expression* res = ParserExpression(parser);

    // debug expression
    int indent = 0;
    ExStationLoadDebugPrint(station);
    res->connect(station, res, 1, &indent);

    // evaluate 
    ExStationLoadEvaluate(station);
    Literal* eva = res->connect(station, res, 0);

    printf("%lf\n", (double*)eva->literal);

    // free expression
    ExStationLoadFree(station);
    res->connect(station, res, 0);


    // parser phase end
    ExStationDestruct(station);
    */

    InterpreterDestruct(interpreter);
    ScannerDestruct(scanner);
    ParserDestruct(parser);
    SystemDestruct(system);

    return 0; 
}