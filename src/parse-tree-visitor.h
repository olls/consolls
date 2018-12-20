#pragma once

#include "types.h"
#include "parse-tree.h"


namespace Compolls
{

namespace Parser
{

namespace Tree
{

template <typename State>
struct Visitor
{
  enum Event
  {
    Begin,
    End
  };

  bool (*generic_func)       (State&, Event, Node*);

  bool (*program_func)       (State&, Event, ProgramNode*);
  bool (*statement_func)     (State&, Event, StatementNode*);
  bool (*body_func)          (State&, Event, BodyNode*);
  bool (*assignment_func)    (State&, Event, AssignmentNode*);
  bool (*declaration_func)   (State&, Event, DeclarationNode*);
  bool (*expression_func)    (State&, Event, ExpressionNode*);
  bool (*literal_func)       (State&, Event, LiteralNode*);
  bool (*function_call_func) (State&, Event, FunctionCallNode*);
  bool (*function_func)      (State&, Event, FunctionNode*);
  bool (*expressions_func)   (State&, Event, ExpressionsNode*);
  bool (*declarations_func)  (State&, Event, DeclarationsNode*);
  bool (*terminal_func)      (State&, Event, TerminalNode*);
};


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, ProgramNode* program_node);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, StatementNode* statement_node);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, BodyNode* body_node);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, AssignmentNode* assignment_node);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, DeclarationNode* declaration_node);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, ExpressionNode* expression_node);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, LiteralNode* literal_node);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, FunctionCallNode* function_call_node);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, FunctionNode* function_node);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, ExpressionsNode* expressions_node);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, DeclarationsNode* declarations_node);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, TerminalNode* terminal_node);


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, Node* node)
{
  bool success = true;

  success &= (visitor.generic_func != NULL) && visitor.generic_func(state, Visitor<State>::Begin, node);

  switch (node->type)
  {
    case (Parser::Tree::Node::Program):
    {
      success &= (visitor.program_func != NULL) && visitor.program_func(state, Visitor<State>::Begin, &node->program);
      success &= visit(state, visitor, &node->program);
      success &= (visitor.program_func != NULL) && visitor.program_func(state, Visitor<State>::End, &node->program);
    } break;
    case (Parser::Tree::Node::Statement):
    {
      success &= (visitor.statement_func != NULL) && visitor.statement_func(state, Visitor<State>::Begin, &node->statement);
      success &= visit(state, visitor, &node->statement);
      success &= (visitor.statement_func != NULL) && visitor.statement_func(state, Visitor<State>::End, &node->statement);
    } break;
    case (Parser::Tree::Node::Body):
    {
      success &= (visitor.body_func != NULL) && visitor.body_func(state, Visitor<State>::Begin, &node->body);
      success &= visit(state, visitor, &node->body);
      success &= (visitor.body_func != NULL) && visitor.body_func(state, Visitor<State>::End, &node->body);
    } break;
    case (Parser::Tree::Node::Assignment):
    {
      success &= (visitor.assignment_func != NULL) && visitor.assignment_func(state, Visitor<State>::Begin, &node->assignment);
      success &= visit(state, visitor, &node->assignment);
      success &= (visitor.assignment_func != NULL) && visitor.assignment_func(state, Visitor<State>::End, &node->assignment);
    } break;
    case (Parser::Tree::Node::Declaration):
    {
      success &= (visitor.declaration_func != NULL) && visitor.declaration_func(state, Visitor<State>::Begin, &node->declaration);
      success &= visit(state, visitor, &node->declaration);
      success &= (visitor.declaration_func != NULL) && visitor.declaration_func(state, Visitor<State>::End, &node->declaration);
    } break;
    case (Parser::Tree::Node::Expression):
    {
      success &= (visitor.expression_func != NULL) && visitor.expression_func(state, Visitor<State>::Begin, &node->expression);
      success &= visit(state, visitor, &node->expression);
      success &= (visitor.expression_func != NULL) && visitor.expression_func(state, Visitor<State>::End, &node->expression);
    } break;
    case (Parser::Tree::Node::Literal):
    {
      success &= (visitor.literal_func != NULL) && visitor.literal_func(state, Visitor<State>::Begin, &node->literal);
      success &= visit(state, visitor, &node->literal);
      success &= (visitor.literal_func != NULL) && visitor.literal_func(state, Visitor<State>::End, &node->literal);
    } break;
    case (Parser::Tree::Node::FunctionCall):
    {
      success &= (visitor.function_call_func != NULL) && visitor.function_call_func(state, Visitor<State>::Begin, &node->function_call);
      success &= visit(state, visitor, &node->function_call);
      success &= (visitor.function_call_func != NULL) && visitor.function_call_func(state, Visitor<State>::End, &node->function_call);
    } break;
    case (Parser::Tree::Node::Function):
    {
      success &= (visitor.function_func != NULL) && visitor.function_func(state, Visitor<State>::Begin, &node->function);
      success &= visit(state, visitor, &node->function);
      success &= (visitor.function_func != NULL) && visitor.function_func(state, Visitor<State>::End, &node->function);
    } break;
    case (Parser::Tree::Node::Expressions):
    {
      success &= (visitor.expressions_func != NULL) && visitor.expressions_func(state, Visitor<State>::Begin, &node->expressions);
      success &= visit(state, visitor, &node->expressions);
      success &= (visitor.expressions_func != NULL) && visitor.expressions_func(state, Visitor<State>::End, &node->expressions);
    } break;
    case (Parser::Tree::Node::Declarations):
    {
      success &= (visitor.declarations_func != NULL) && visitor.declarations_func(state, Visitor<State>::Begin, &node->declarations);
      success &= visit(state, visitor, &node->declarations);
      success &= (visitor.declarations_func != NULL) && visitor.declarations_func(state, Visitor<State>::End, &node->declarations);
    } break;
    case (Parser::Tree::Node::Terminal):
    {
      success &= (visitor.terminal_func != NULL) && visitor.terminal_func(state, Visitor<State>::Begin, &node->terminal);
      success &= visit(state, visitor, &node->terminal);
      success &= (visitor.terminal_func != NULL) && visitor.terminal_func(state, Visitor<State>::End, &node->terminal);
    } break;
  }

  success &= (visitor.generic_func != NULL) && visitor.generic_func(state, Visitor<State>::End, node);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, ProgramNode* program_node)
{
  bool success = true;

  success &= visit(state, visitor, program_node->body);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, StatementNode* statement_node)
{
  bool success = true;

  switch (statement_node->type)
  {
    case (StatementNode::Type::Assignment):
    {
      success &= visit(state, visitor, statement_node->assignment);
    } break;
    case (StatementNode::Type::Expression):
    {
      success &= visit(state, visitor, statement_node->expression);
    } break;
  }

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, BodyNode* body_node)
{
  bool success = true;

  for (u32 statement_index = 0;
       statement_index < body_node->statements.n_elements;
       ++statement_index)
  {
    success &= visit(state, visitor, body_node->statements[statement_index]);
  }

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, AssignmentNode* assignment_node)
{
  bool success = true;

  success &= visit(state, visitor, assignment_node->declaration);
  success &= visit(state, visitor, assignment_node->expression);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, DeclarationNode* declaration_node)
{
  bool success = true;

  success &= visit(state, visitor, declaration_node->type);
  success &= visit(state, visitor, declaration_node->label);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, ExpressionNode* expression_node)
{
  bool success = true;

  switch (expression_node->type)
  {
    case (ExpressionNode::Type::Literal):
    {
      success &= visit(state, visitor, expression_node->literal);
    } break;
    case (ExpressionNode::Type::FunctionCall):
    {
      success &= visit(state, visitor, expression_node->function_call);
    } break;
    case (ExpressionNode::Type::Identifier):
    {
      success &= visit(state, visitor, expression_node->identifier);
    } break;
  }

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, LiteralNode* literal_node)
{
  bool success = true;

  switch (literal_node->type)
  {
    case (LiteralNode::Type::Number):
    {
      success &= visit(state, visitor, literal_node->number);
    } break;
    case (LiteralNode::Type::Function):
    {
      success &= visit(state, visitor, literal_node->function);
    } break;
  }

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, FunctionCallNode* function_call_node)
{
  bool success = true;

  success &= visit(state, visitor, function_call_node->label);
  success &= visit(state, visitor, function_call_node->expressions);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, FunctionNode* function_node)
{
  bool success = true;

  success &= visit(state, visitor, function_node->return_type);
  success &= visit(state, visitor, function_node->declarations);
  success &= visit(state, visitor, function_node->body);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, ExpressionsNode* expressions_node)
{
  bool success = true;

  for (u32 expression_index = 0;
       expression_index < expressions_node->expressions.n_elements;
       ++expression_index)
  {
    success &= visit(state, visitor, expressions_node->expressions[expression_index]);
  }

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, DeclarationsNode* declarations_node)
{
  bool success = true;

  for (u32 declaration_index = 0;
       declaration_index < declarations_node->declarations.n_elements;
       ++declaration_index)
  {
    success &= visit(state, visitor, declarations_node->declarations[declaration_index]);
  }

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, TerminalNode* terminal_node)
{
  bool success = true;

  return success;
}

} // namespace Tree

} // namespace Parser

} // namespace Compolls