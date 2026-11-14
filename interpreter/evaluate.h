#ifndef EVALUATE_H
#define EVALUATE_H

#include "../misc/error.h"
#include "../environment/value.h"
#include "../ast/expr.h"
#include "interpreter.h"


void value_is_truthy(Status* status, Value* value);
void value_is_not_truthy(Status* status, Value* value);

Value* value_allocate(Status* status);
void value_destruct(Status* status, Value* value);

Value* evaluate_expr_dispatch(Status* status, Interpreter* interpreter, Expr* expr);
Value* evaluate_literal(Status* status, Interpreter* interpreter, Literal* literal);
Value* evaluate_logic(Status* status, Interpreter* interpreter, Logical* logic);
Value* evaluate_binary(Status* status, Interpreter* interpreter, Binary* binary);
Value* evaluate_unary(Status* status, Interpreter* interpreter, Unary* unary);
Value* evaluate_assign(Status* status, Interpreter* interpreter, Assign* assign);
Value* evaluate_variable(Status* status, Interpreter* interpreter, Variable* variable);

#endif