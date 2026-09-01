#ifndef PARSER_H
#define PARSER_H

#include "../misc/heap.h"
#include "ast.h"

extern bool* PARSER_NIL;
extern bool* PARSER_TRUE;
extern void** PARSER_FALSE;

typedef struct
{
  Heap tokens; 
  Expression* root;

  // working parameters
  int start;
  int current;
} Parser;

Parser parserInit();
void parserDestruct(Parser* parser);

/*
 * brief walkthrough the hierarchy
 * (this is the order to do the calculation)
 * expression -> equality -> comparison
 * -> term -> factor -> unary -> primary
 * */



void* parserPrimary(Parser* parser);
void* parserUnary(Parser* parser);
void* parserFactor(Parser* parser);
void* parserTerm(Parser* parser);
void* parserComparison(Parser* parser);
void* parserEquality(Parser* parser);
void* parserExpression(Parser* parser);

// diagnostic func
void parserError();

#endif
