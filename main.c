#include <stdio.h>
#include "modules/scanner.h"
#include "modules/parser.h"
#include "misc/hashmap.h"
#include "modules/expression.h"
#include "misc/assert_.h"
#include "modules/intepreter.h"
#include "modules/environment.h"

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
    Interpreter* interpreter = InterpreterInit(parser);

    // scanner phase
    ScannerScan(scanner, system);


    // parser phase (statement parser)
    // ParserParse(parser, interpreter);
    
    // interpreter phase (execute statement)
    InterpreterInterpret(interpreter, parser);

    InterpreterDestruct(interpreter);
    ParserDestruct(parser);
    ScannerDestruct(scanner);
    SystemDestruct(system);

    return 0; 
}