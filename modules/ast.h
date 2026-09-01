#ifndef AST_H
#define AST_H

#include "token.h"
#define VISITORBASE void (*accept)(void* visitor, void* obj, void* ext);

// polymorphimsm larping for visitor
// larp all func with visitor-accept interface
// source:
// https://www.reddit.com/r/C_Programming/comments/1d2jj16/comment/l6330ae/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
// https://medium.com/@iamprovidence/visitor-pattern-is-underrated-11d196f9db5f

/*
 * note: use the polymorphism most of the time
 * only visitor if too many new functions (hard to maintain each classes)
 * polymorphism still work in this case, but a more elegant solution, in this case, would be visitor pattern :D
 *
 * note:
 * struct{accept};
 *
 * each struct get a unique accept method
 *  get to the right struct offset
 *
 * bucket struct
 * {
 *  // contains the corresponding function pointer (a station)
 *
 *  NOTE: CHANGE: I NAME THOSE STATIONS
 *  // those all called visitors (function pointers) e.g.
 *  void (*visitorA)(void* a);
 *  void (*visitorB)(void* b);
 *  void (*visitorC)(void* c);
 *  ...
 * }
 *
 * note that the accept func:
 * void accept (void* visitor, void* self)
 * => the accept func of A will point to the <func tuned for A> in that struct
 *
 * to use that function on the batches, proceed to give the visitors struct
 *
 * e.g. void A_accept_func(void* visitors_list_struct, void* a) 
 *
 * NOTE: CHANGE: I NAME THOSE PORTS)
 * {
 *    visitors_list_struct->a_visit(a);
 *    return;
 * }
 *
 * cons: only deal with internal var in a, no external packages
 * an external package should be delivered through a more exotic, complex implementation
 * e.g.
 * typedef struct
 * {
 *    void (*visitorA)(void* a, void* external_package);
 * } bucket1;
 *
 * usage:
 * Ext *ext = malloc(sizeof(ext)); // Ext is a struct
 * A_accept_func(visitor_struct, &a, ext)
 *
 * */

typedef struct
{
  VISITORBASE
    void* expression;
} Expression;

typedef struct
{
  VISITORBASE
  void* left;
  TokenType op;
  void* right;
} Binary;
Binary* astInitBinary(void* expression_1, TokenType operation, void* expression_2);

typedef struct
{
  VISITORBASE
  void* expression;
} Grouping;
Grouping* astInitGrouping(void* expression);

typedef struct
{
  VISITORBASE
  TokenType op;
  void* expression;
} Unary;
Unary* astInitUnary(TokenType operation, void* expression);

typedef struct
{
  VISITORBASE
  void* address;
  TokenType type;
} Literal; // only for now, this shit gonna explodes into hundreds
Literal* astInitLiteral(TokenType dtype, void* value);

// unique key to access (for each struct)
void ExpressionAcceptKey(void* visitor, void* expression, void* ext);
void BinaryAcceptKey(void* visitor, void* binary, void* ext);
void GroupAcceptKey(void* visitor, void* group, void* ext);
void UnaryAcceptKey(void* visitor, void* unary, void* ext);
void LiteralAcceptKey(void* visitor, void* literal, void* ext);

typedef struct
{
  void (*expressionBucketFunction)(void* visitor, void* expression, void* ext);
  void (*binaryBucketFunction)(void* visitor, void* binary, void* ext);
  void (*groupBucketFunction)(void* visitor, void* group, void* ext);
  void (*unaryBucketFunction)(void* visitor, void* unary, void* ext);
  void (*literalBucketFunction)(void* visitor, void* literal, void* ext);
} astVisitorStation;

astVisitorStation* initVisitorStation();
void freeVisitorStation(astVisitorStation* visitor_bucket);

// for drawing arithmetic trees
void visitorLoadDebugPrintFunctions(astVisitorStation*);

// for freeing expressions
void visitorLoadFreeFunctions(astVisitorStation*);


#endif
