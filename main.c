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

    // scanner phase
    ScannerScan(scanner, system);

    // parser phase
    ParserParse(parser);
    
    Interpreter* interpreter = InterpreterInit(parser);
    InterpreterInterpret(interpreter);

    InterpreterDestruct(interpreter);
    ScannerDestruct(scanner);
    ParserDestruct(parser);
    SystemDestruct(system);

    // test environment
    int* test = malloc(sizeof(int) * 8);
    test[0] = 0;
    test[1] = 1;
    test[2] = 2;

    Environment* e1 = EnvironmentInit();
    EnvironmentDefine(e1, "quoc", ExLiteralInit(test + 0, INT));
    Literal* tmp = EnvironmentGet(e1, "quoc");
    EnvironmentDefine(e1, "quoc", ExLiteralInit(test + 1, INT));
    tmp = EnvironmentGet(e1, "quoc");
    EnvironmentDefine(e1, "anh", ExLiteralInit(test + 2, INT));
    tmp = EnvironmentGet(e1, "anh");

    free(test);

    EnvironmentDestruct(e1);

    return 0; 
}