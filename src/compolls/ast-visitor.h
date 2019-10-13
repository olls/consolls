#pragma once

#include "ast.h"

#include "utils/types.h"


namespace Compolls
{

namespace AST
{

enum class VisitorEvent
{
  Enter,
  Leave
};


template <typename State>
struct Visitor
{
  bool (*node_func)          (State&, VisitorEvent);

  bool (*program_func)       (State&, VisitorEvent, void*& user, Program*);
  bool (*body_func)          (State&, VisitorEvent, void*& user, Body*);
  bool (*statement_func)     (State&, VisitorEvent, void*& user, Statement*);
  bool (*assignment_func)    (State&, VisitorEvent, void*& user, Assignment*);
  bool (*declaration_func)   (State&, VisitorEvent, void*& user, Declaration*);
  bool (*expression_func)    (State&, VisitorEvent, void*& user, Expression*);
  bool (*function_call_func) (State&, VisitorEvent, void*& user, FunctionCall*);
  bool (*literal_func)       (State&, VisitorEvent, void*& user, Literal*);
  bool (*function_func)      (State&, VisitorEvent, void*& user, Function*);
  bool (*number_func)        (State&, VisitorEvent, void*& user, Number*);
  bool (*declarations_func)  (State&, VisitorEvent, void*& user, Declarations*);
  bool (*expressions_func)   (State&, VisitorEvent, void*& user, Expressions*);
  bool (*identifier_func)    (State&, VisitorEvent, void*& user, Identifier*);
};


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Program* program);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Body* body);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Statement* statement);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Assignment* assignment);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Declaration* declaration);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Expression* expression);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, FunctionCall* function_call);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Literal* literal);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Function* function);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Number* number);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Declarations* declarations);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Expressions* expressions);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Identifier* identifier);


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, AST* ast)
{
  bool success = true;

  success &= visit(state, visitor, &ast->program);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Program* program)
{
  bool success = true;

  void* user = NULL;
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Enter);
  success &= (visitor.program_func == NULL) || visitor.program_func(state, VisitorEvent::Enter, user, program);
  success &= visit(state, visitor, &program->body);
  success &= (visitor.program_func == NULL) || visitor.program_func(state, VisitorEvent::Leave, user, program);
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Leave);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Body* body)
{
  bool success = true;

  void* user = NULL;
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Enter);
  success &= (visitor.body_func == NULL) || visitor.body_func(state, VisitorEvent::Enter, user, body);

  for (u32 statement_index = 0;
       statement_index < body->n;
       ++statement_index)
  {
    success &= visit(state, visitor, body->statements + statement_index);
  }

  success &= (visitor.body_func == NULL) || visitor.body_func(state, VisitorEvent::Leave, user, body);
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Leave);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Statement* statement)
{
  bool success = true;

  void* user = NULL;
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Enter);
  success &= (visitor.statement_func == NULL) || visitor.statement_func(state, VisitorEvent::Enter, user, statement);

  switch (statement->type)
  {
    case (Statement::Type::Assignment):
    {
      success &= visit(state, visitor, &statement->assignment);
    } break;
    case (Statement::Type::Expression):
    {
      success &= visit(state, visitor, &statement->expression);
    } break;
  }

  success &= (visitor.statement_func == NULL) || visitor.statement_func(state, VisitorEvent::Leave, user, statement);
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Leave);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Assignment* assignment)
{
  bool success = true;

  void* user = NULL;
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Enter);
  success &= (visitor.assignment_func == NULL) || visitor.assignment_func(state, VisitorEvent::Enter, user, assignment);
  success &= visit(state, visitor, &assignment->declaration);
  success &= visit(state, visitor, &assignment->expression);
  success &= (visitor.assignment_func == NULL) || visitor.assignment_func(state, VisitorEvent::Leave, user, assignment);
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Leave);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Declaration* declaration)
{
  bool success = true;

  void* user = NULL;
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Enter);
  success &= (visitor.declaration_func == NULL) || visitor.declaration_func(state, VisitorEvent::Enter, user, declaration);
  success &= visit(state, visitor, &declaration->identifier);
  success &= (visitor.declaration_func == NULL) || visitor.declaration_func(state, VisitorEvent::Leave, user, declaration);
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Leave);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Expression* expression)
{
  bool success = true;

  void* user = NULL;
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Enter);
  success &= (visitor.expression_func == NULL) || visitor.expression_func(state, VisitorEvent::Enter, user, expression);

  switch (expression->type)
  {
    case (Expression::Type::Literal):
    {
      success &= visit(state, visitor, &expression->literal);
    } break;
    case (Expression::Type::FunctionCall):
    {
      success &= visit(state, visitor, &expression->function_call);
    } break;
    case (Expression::Type::Identifier):
    {
      success &= visit(state, visitor, &expression->identifier);
    } break;
  }
  success &= (visitor.expression_func == NULL) || visitor.expression_func(state, VisitorEvent::Leave, user, expression);
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Leave);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, FunctionCall* function_call)
{
  bool success = true;

  void* user = NULL;
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Enter);
  success &= (visitor.function_call_func == NULL) || visitor.function_call_func(state, VisitorEvent::Enter, user, function_call);
  success &= visit(state, visitor, &function_call->identifier);
  success &= visit(state, visitor, &function_call->expressions);
  success &= (visitor.function_call_func == NULL) || visitor.function_call_func(state, VisitorEvent::Leave, user, function_call);
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Leave);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Literal* literal)
{
  bool success = true;

  void* user = NULL;
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Enter);
  success &= (visitor.literal_func == NULL) || visitor.literal_func(state, VisitorEvent::Enter, user, literal);
  switch (literal->type)
  {
    case (Literal::Type::Number):
    {
      success &= visit(state, visitor, &literal->number);
    } break;
    case (Literal::Type::Function):
    {
      success &= visit(state, visitor, &literal->function);
    } break;
  }
  success &= (visitor.literal_func == NULL) || visitor.literal_func(state, VisitorEvent::Leave, user, literal);
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Leave);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Function* function)
{
  bool success = true;

  void* user = NULL;
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Enter);
  success &= (visitor.function_func == NULL) || visitor.function_func(state, VisitorEvent::Enter, user, function);
  success &= visit(state, visitor, function->body);
  success &= (visitor.function_func == NULL) || visitor.function_func(state, VisitorEvent::Leave, user, function);
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Leave);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Number* number)
{
  bool success = true;

  void* user = NULL;
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Enter);
  success &= (visitor.number_func == NULL) || visitor.number_func(state, VisitorEvent::Enter, user, number);
  success &= (visitor.number_func == NULL) || visitor.number_func(state, VisitorEvent::Leave, user, number);
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Leave);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Declarations* declarations)
{
  bool success = true;

  void* user = NULL;
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Enter);
  success &= (visitor.declarations_func == NULL) || visitor.declarations_func(state, VisitorEvent::Enter, user, declarations);

  for (u32 declaration_index = 0;
       declaration_index < declarations->n;
       ++declaration_index)
  {
    success &= visit(state, visitor, declarations->declarations + declaration_index);
  }

  success &= (visitor.declarations_func == NULL) || visitor.declarations_func(state, VisitorEvent::Leave, user, declarations);
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Leave);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Expressions* expressions)
{
  bool success = true;

  void* user = NULL;
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Enter);
  success &= (visitor.expressions_func == NULL) || visitor.expressions_func(state, VisitorEvent::Enter, user, expressions);

  for (u32 expression_index = 0;
       expression_index < expressions->n;
       ++expression_index)
  {
    success &= visit(state, visitor, expressions->expressions + expression_index);
  }

  success &= (visitor.expressions_func == NULL) || visitor.expressions_func(state, VisitorEvent::Leave, user, expressions);
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Leave);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Identifier* identifier)
{
  bool success = true;

  void* user = NULL;
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Enter);
  success &= (visitor.identifier_func == NULL) || visitor.identifier_func(state, VisitorEvent::Enter, user, identifier);
  success &= (visitor.identifier_func == NULL) || visitor.identifier_func(state, VisitorEvent::Leave, user, identifier);
  success &= (visitor.node_func == NULL) || visitor.node_func(state, VisitorEvent::Leave);

  return success;
}

} // namespace AST

} // namespace Compolls