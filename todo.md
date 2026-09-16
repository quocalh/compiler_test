evaluating expression (Interpreter)

list all the defects in the module
clean up the code

redesign the whole thing (2 days), rewrite it (should only take a week)

# Redesign notes
## principle
let's be real with our expectation, we are making high level language, not to make another c
(im stupid yk)

The expression struct in ast could have been in the diff .h .c file, namely exp.c and exp.h
The token ptr in scanner and parser is one defined entity, a diff client class should store it (in this perspective, scanner and parser are servers).
    -> System struct? maybe
        server:
            scanner
            parser
            interpreter
            
## ast.c -> system.c
the false, true, null pointer handling could have been in the system struct too, ready to be freed at the end 
create a new type call: BOOLEAN, AND NULL
introduce another new type called CHAR (same with int and float)
NOTE: tokenType are only user definition (all to warn the users)
    union set array can help us with this one?
    or another set of grammar idk...

the accept functions (port) are defined as followed:
    accept0(void* station, void* expression) // takes no extra arg
    accept1(void* station, void* expression, void* ext1) // takes 1 extra arg
    accept2(void* station, void* expression, void* ext1, void* ext2) // takes 2 extra args
and the station, idk
// NOTE: this should be consider throughfully
// uhh, no, just use stdarg.h lib in c

## scanner.c
the scanner can be made more complicated especially with string and the \
    "\" alone denotes a discarded line break \n

## parser.c
make an proper error handling system in parsing file

# C NOTE
- no ptr post-fix in variable's names
- no camelCase, snake_case in naming function (use TitleCase)
read: https://www.reddit.com/r/C_Programming/comments/1bvnasy/what_naming_convention_do_you_prefer_in_c/
read: https://www.reddit.com/r/C_Programming/comments/1bvnasy/comment/ky0ylwh/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button


https://martinfowler.com/bliki/ArchitectureDecisionRecord.html