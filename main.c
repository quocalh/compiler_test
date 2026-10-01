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


    ExStation* st = ExStationInit();
    int* t1 = malloc(sizeof(*t1)); *t1 = 2;
    int* t2 = malloc(sizeof(*t2)); *t2 = 7;

    Expression* expr = ExExpressionInit(
        ExBinaryInit(
            ExVariableInit("my_var"),
            ExLiteralInit(t2, INT),
            PLUS
        )
    );

    ExStationLoadDebugPrint(st);
    int* indent = 0;
    expr->connect(st, expr, 1, &indent);

    ExStationLoadFree(st);
    expr->connect(st, expr, 0);

    free(t1);
    free(t2);

    ExStationDestruct(st);

    /*
    // scanner phase
    ScannerScan(scanner, system);


    // parser phase (statement parser)
    ParserParse(parser);
    
    // interpreter phase (execute statement)
    InterpreterInterpret(interpreter);

    InterpreterDestruct(interpreter);
    */



    ScannerDestruct(scanner);
    ParserDestruct(parser);
    SystemDestruct(system);

    // test environment
    int* test0 = malloc(sizeof(int));
    int* test1 = malloc(sizeof(int));
    int* test2 = malloc(sizeof(int));

    Environment* e1 = EnvironmentInit();
    EnvironmentDefine(e1, "quoc", ExLiteralInit(test0, INT));
    Literal* tmp = EnvironmentGet(e1, "quoc");
    EnvironmentDefine(e1, "quoc", ExLiteralInit(test1, INT));
    tmp = EnvironmentGet(e1, "quoc");
    EnvironmentDefine(e1, "anh", ExLiteralInit(test2, INT));
    tmp = EnvironmentGet(e1, "anh");

    EnvironmentDestruct(e1);

    return 0; 
}