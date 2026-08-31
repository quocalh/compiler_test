#include "ast.h"
#include <assert.h>
#include <stdlib.h>

// [INIT]
Binary* astInitBinary(void* expression_1, TokenType operation, void* expression_2){
  Binary* b = malloc(sizeof(*b));
  if (!b) {printf("[ast.c] init binary | nah\n"); return NULL;}
  *((Binary*)b) = (Binary){.left = expression_1, .op = operation, .right = expression_2, .accept = BinaryAcceptKey};
  return b;
}
Grouping* astInitGrouping(void* expression){
  Grouping* g = malloc(sizeof(*g));
  if (!g) {printf("[ast.c] init binary | nah\n"); return NULL;}
  *((Grouping*)g) = (Grouping){.expression = expression, .accept = GroupAcceptKey};
  return g;
}
Unary* astInitUnary(TokenType operation, void* expression){
  Unary* u = malloc(sizeof(*u));
  if (!u) {printf("[ast.c] init unary | nah\n"); return NULL;}
  *((Unary*)u) = (Unary){.op = operation, .expression = expression, .accept = UnaryAcceptKey};
  return u;
}
Literal* astInitLiteral(TokenType dtype, void* value){
  Literal* l = malloc(sizeof(*l));
  if (!l) {printf("[ast.c] init literal | nah\n"); return NULL;}
  *((Literal*)l) = (Literal){.address = value, .type = dtype, .accept = LiteralAcceptKey};
  return l;
}

// [PORT]
void BinaryAcceptKey(void* visitor, void* binary){
  if (!visitor || !binary) assert(0);
  ((astVisitorStation*)visitor)->binaryBucketFunction(visitor, binary);
}
void GroupAcceptKey(void* visitor, void* group){
  if (!visitor || !group) assert(0);
  ((astVisitorStation*)visitor)->groupBucketFunction(visitor, group);
}
void UnaryAcceptKey(void* visitor, void* unary){
  if (!visitor || !unary) assert(0);
  ((astVisitorStation*)visitor)->unaryBucketFunction(visitor, unary);
}
void LiteralAcceptKey(void* visitor, void* literal){
  if (!visitor || !literal) assert(0);
  ((astVisitorStation*)visitor)->literalBucketFunction(visitor, literal);
}

// all malloc no stack i suppose?
// void freeExpression(astVisitorStation* station, void* expression); // recursive constructed
void freeExpression(astVisitorStation* station, Expression* expression){
  ((Expression*)expression)->accept(station, &expression);
  free(expression);
}
void freeBinary(void* station, void* b){
  Binary* binary = (Binary*)b;
  ((Expression*)binary->left)->accept(station, &binary->left); // free children
  ((Expression*)binary->right)->accept(station, &binary->right); // 1-layer polymorphism
  free(b); // free itself
}
void freeGroup(void* station, void* g){
  Grouping* group = (Grouping*)g;
  ((Expression*)group->expression)->accept(station, &group->expression);
  free(g);
}
void freeUnary(void* station, void* u){
  Unary* unary = u;
  ((Expression*)unary->expression)->accept(station, unary->expression);
  free(u);
}
void freeLiteral(void* station, void* l){
  Literal* literal = l;
  if (literal->address == NULL){printf("[ast.c] null address\n");assert(0);}
  if (literal->type == EXPRESSION) {
    literal->accept(station, &literal);
  }
  free(literal);
}

// TODO: not shure of ts gonna work
void visitorLoadFreeFunctions(astVisitorStation* station){
  station->binaryBucketFunction = freeBinary;
  station->groupBucketFunction = freeGroup;
  station->unaryBucketFunction = freeUnary;
  station->literalBucketFunction = freeLiteral;
}

// init free
astVisitorStation* initVisitorStation(){
  astVisitorStation* station = malloc(sizeof(*station));
  if (!station) {printf("[ast] init station failed\n"); assert(0);}
  return station;
}
void freeVisitorStation(astVisitorStation* visitor_bucket){
  free(visitor_bucket);
}
