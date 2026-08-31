#ifndef PARSER_H
#define PARSER_H

#include "../misc/heap.h"

typedef struct
{
  Heap tokens; 
  
  // working parameters
  int start;
  int current;
  int currentLine;
} Parser;

void parserInit(Parser* parser);
void parserDestruct(Parser* parser);

/*
 * brief walkthrough the hierarchy
 * (this is the order to do the calculation)
 * expression -> equality -> comparison
 * -> term -> factor -> unary -> primary
 * */

void* Primary(Parser* parser);
void* parserUnary(Parser* parser);
void* parserFactor(Parser* parser);
void* parserTerm(Parser* parser);
void* parserComparison(Parser* parser);
void* parserEquality(Parser* parser);
void* parserExpression(Parser* parser);

// diagnostic func
void parserError();

#endif
