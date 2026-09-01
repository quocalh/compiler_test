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
void BinaryAcceptKey(void* visitor, void* binary, void* ext){
  if (!visitor || !binary) assert(0);
  ((astVisitorStation*)visitor)->binaryBucketFunction(visitor, binary, ext);
}
void GroupAcceptKey(void* visitor, void* group, void* ext){
  if (!visitor || !group) assert(0);
  ((astVisitorStation*)visitor)->groupBucketFunction(visitor, group, ext);
}
void UnaryAcceptKey(void* visitor, void* unary, void* ext){
  if (!visitor || !unary) assert(0);
  ((astVisitorStation*)visitor)->unaryBucketFunction(visitor, unary, ext);
}
void LiteralAcceptKey(void* visitor, void* literal, void* ext){
  if (!visitor || !literal) assert(0);
  ((astVisitorStation*)visitor)->literalBucketFunction(visitor, literal, ext);
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


// for drawing arithmetic trees
void printftab(int n) {for (int i = 0; i < n; i++) printf("\t");}
void drawExpression(void* station, void* e, void* ext){
  int* n = (int*)ext;
  Expression* expression = e;
  printftab(*n); printf("Expression\{\n");
  (*n)++;
  ((Expression*) expression)->accept(station, &expression->expression, n);
  (*n)--;
  printftab(*n); printf("}\n");
}void drawBinary(void* station, void* b, void* ext){
  int* n = (int*)ext;
  Binary* binary = b;
  printftab(*n); printf("Binary\{\n");
  (*n)++;
  printftab(*n); printf("left: \n"); ((Expression*)binary->left)->accept(station, binary->left, n);
  printftab(*n); printf("right: \n"); ((Expression*)binary->right)->accept(station, binary->right, n);
  (*n)--;
  printftab(*n); printf("}\n");
}void drawGrouping(void* station, void* g, void* ext){
  int* n = (int*)ext; 
  Grouping* grouping = g;
  printftab(*n); printf("Grouping: (\n");
  (*n)++;
  printftab(*n); ((Expression*)grouping->expression)->accept(station, grouping->expression, n);
  (*n)--;
  printftab(*n); printf(")\n");
}void drawUnary(void* station, void* u, void* ext){
  int* n = ext;
  Unary* unary = u;
  printftab(*n); printf("Unary(\n");
  (*n)++;
  printftab(*n); ((Expression*)unary->expression)->accept(station, unary->expression, n);
  (*n)--;
  printftab(*n); printf(")\n");
}void drawLiteral(void* station, void* l, void* ext){
  int* n = ext;
  Literal* literal = l;
  // (*n)++;
  printftab(*n); printf("(literal): ");
  switch (literal->type)
  {
    case (INT):
      printf("%d", *(int*)literal->address);
      break;
    case (FLOAT):
      printf("%lf", *(double*)literal->address);
      break;
    case (STRING):
      printf("%s", (char*)literal->address);
      break;
    default:
      printf("[ast.c] literal printing | nigger die ( %d )\n", literal->type);
      assert(0);
  }
  printf("\n");
  // (*n)--;
}
void visitorLoadDebugPrintFunctions(astVisitorStation* station)
{
  station->expressionBucketFunction = drawExpression;
  station->binaryBucketFunction = drawBinary;
  station->groupBucketFunction = drawGrouping;
  station->unaryBucketFunction = drawUnary;
  station->literalBucketFunction = drawLiteral;
}

// for freeing expressions
void freeExpression(astVisitorStation* station, Expression* expression, void* ext){
  ((Expression*)expression)->accept(station, &expression->expression, ext);
  free(expression);
}void freeBinary(void* station, void* b, void* ext){
  Binary* binary = (Binary*)b;
  ((Expression*)binary->left)->accept(station, &binary->left, ext); // free children
  ((Expression*)binary->right)->accept(station, &binary->right, ext); // 1-layer polymorphism
  free(b); // free itself
}void freeGroup(void* station, void* g, void* ext){
  Grouping* group = (Grouping*)g;
  ((Expression*)group->expression)->accept(station, &group->expression, ext);
  free(g);
}void freeUnary(void* station, void* u, void* ext){
  Unary* unary = u;
  ((Expression*)unary->expression)->accept(station, &unary->expression, ext);
  free(u);
}void freeLiteral(void* station, void* l, void* ext){
  Literal* literal = l;
  if (literal->address == NULL){printf("[ast.c] null address\n");assert(0);}
  if (literal->type == EXPRESSION) {
    literal->accept(station, &literal, ext);
  }
  free(literal);}

void visitorLoadFreeFunctions(astVisitorStation* station){
  station->binaryBucketFunction = freeBinary;
  station->groupBucketFunction = freeGroup;
  station->unaryBucketFunction = freeUnary;
  station->literalBucketFunction = freeLiteral;
}

