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
  printf("a new value: %d %p\n", dtype, value);
  printf("the number: %d\n", *((int*)value));
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

// NOTE: figure out to inject this to every visitor to handle the NULL expression (not the NIL token)
// closure + wrapper
// nah stupid nigger: an accept helper is enough bruh
//
// typedef struct
// {
//   void (*ptr)(void* station, void* e, void* ext);
// } NullSafeCheckClosure;
//
// void NullSafeCheckWrapperInnerFunc(void* station, void* e, void* ext)
// {
//   void (*f)(void*, void*, void*);
//   int* n = (int*)ext;
//   if (station == NULL){printf("[ast.c] check station | null\n"); assert(0);}
//
//   if (e == NULL){
//     printftab(*n); printf("NULL\n");}
//   else{
//     f(station, e, ext);
//     // TODO: fix this closure mess
//   }
// }
//
// NullSafeCheckClosure* NullClosureSetup(void* f)
// {
//   // return the struct pointer (c), not the func pointer (c->ptr)
//   // to get the function pointer, type <var>->ptr;
//   NullSafeCheckClosure* c = malloc(sizeof(*c));
//   c->ptr = NullSafeCheckWrapperInnerFunc;
//   return c;
// }
//

void acceptSafe(void* station, void* e, void* ext){
  int* n = (int*)ext;
  if (station == NULL){printf("[ast.c] check station | null\n"); assert(0);}

  if (e == NULL){
    // printf("hello world%p\n", e);
    printftab(*n); printf("NULL\n");
    return;
  }
  printf("hello\n");
  ((Expression*)e)->accept(station, e, ext);
}

void drawExpression(void* station, void* e, void* ext){
  int* n = (int*)ext;
  Expression* expression = e;
  printftab(*n); printf("Expression\{\n");
  (*n)++;
  // ((Expression*) expression->expression)->accept(station, expression->expression, n);
  acceptSafe(station, expression->expression, n);
  (*n)--;
  printftab(*n); printf("}\n");
}void drawBinary(void* station, void* b, void* ext){
  int* n = (int*)ext;
  Binary* binary = b;
  printftab(*n); printf("Binary\{\n");
  (*n)++;
  printftab(*n); printf("left: \n");
  acceptSafe(station, binary->left, n);
  printftab(*n); printf("right: \n");
  acceptSafe(station, binary->right, n);
  (*n)--;
  printftab(*n); printf("}\n");
}void drawGrouping(void* station, void* g, void* ext){
  int* n = (int*)ext;
  Grouping* grouping = g;
  printftab(*n); printf("Grouping: (\n");
  (*n)++;
  printftab(*n);
  acceptSafe(station, grouping->expression, n);
  (*n)--;
  printftab(*n); printf(")\n");
}void drawUnary(void* station, void* u, void* ext){
  int* n = ext;
  Unary* unary = u;
  printftab(*n); printf("Unary(\n");
  (*n)++;
  acceptSafe(station, unary->expression, n);
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
    case (EXPRESSION):;
      Expression* e = (Expression*)((Literal*)literal->address);
      printf("\n");
      (*n)++;
      acceptSafe(station, e, n);
      (*n)--;
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
void acceptfreeSafe(void* station, void* e, void* ext){
  if (station == NULL){printf("[ast.c] check station | null\n"); assert(0);}
  if (e == NULL){
    printf("free NULL\n");
    return;
  }
  ((Expression*)e)->accept(station, e, ext);
}
void freeExpression(astVisitorStation* station, Expression* expression, void* ext){
  printf("free expression\n");
  // ((Expression*)expression)->accept(station, expression->expression, ext);
  acceptfreeSafe(station, expression->expression, ext);
  free(expression);
}void freeBinary(void* station, void* b, void* ext){
  printf("bin\n");
  Binary* binary = (Binary*)b;
  // ((Expression*)binary->left)->accept(station, binary->left, ext); // free children
  // ((Expression*)binary->right)->accept(station, binary->right, ext); // 1-layer polymorphism
  acceptfreeSafe(station, binary->left, ext);
  acceptfreeSafe(station, binary->right, ext);
  free(b); // free itself
}void freeGroup(void* station, void* g, void* ext){
  printf("free group\n");
  Grouping* group = (Grouping*)g;
  // ((Expression*)group->expression)->accept(station, group->expression, ext);
  acceptfreeSafe(station, group->expression, ext);
  free(g);
}void freeUnary(void* station, void* u, void* ext){
  printf("free unary\n");
  Unary* unary = u;
  // ((Expression*)unary->expression)->accept(station, unary->expression, ext);
  acceptfreeSafe(station, unary->expression, ext);
  free(u);
}void freeLiteral(void* station, void* l, void* ext){
  printf("free literal\n");
  Literal* literal = l;
  if (literal->address == NULL){printf("[ast.c] null address\n");assert(0);}
  if (literal->type == EXPRESSION) {
    acceptfreeSafe(station, literal->address, ext);
    // return;
  }
  switch ((TokenType)literal->type)
  {
    case (EXPRESSION):
      break;
    case (FLOAT):
    case (INT):
      // printf("hello igger\n");
      // free(literal->address);
      // this, if only they only get me the data from the sea (heap)
      break;

    case (STRING):
      StaticStringFree(literal->address);
      break;

    default:
      printf("[ast.c] you shouldn't be here no? (or can't free the address from the stack)\n"); assert(0);
      break;
  }
  free(literal);
}

void visitorLoadFreeFunctions(astVisitorStation* station){
  station->binaryBucketFunction = freeBinary;
  station->groupBucketFunction = freeGroup;
  station->unaryBucketFunction = freeUnary;
  station->literalBucketFunction = freeLiteral;
}

