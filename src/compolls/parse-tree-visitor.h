#pragma once

#include "parse-tree.h"

#include "utils/types.h"


namespace Compolls
{

namespace Parser
{

namespace Tree
{

enum VisitorEvent
{
  Enter,
  Leave
};


template <typename State>
struct Visitor
{
  bool (*generic_func)            (State&, VisitorEvent, Node*);

  bool (*program_func)            (State&, VisitorEvent, ProgramNode*);
  bool (*statement_func)          (State&, VisitorEvent, StatementNode*);
  bool (*body_func)               (State&, VisitorEvent, BodyNode*);
  bool (*scope_func)              (State&, VisitorEvent, ScopeNode*);
  bool (*assignment_func)         (State&, VisitorEvent, AssignmentNode*);
  bool (*type_name_func)          (State&, VisitorEvent, TypeNameNode*);
  bool (*declaration_func)        (State&, VisitorEvent, DeclarationNode*);
  bool (*expression_func)         (State&, VisitorEvent, ExpressionNode*);
  bool (*literal_func)            (State&, VisitorEvent, LiteralNode*);
  bool (*function_call_func)      (State&, VisitorEvent, FunctionCallNode*);
  bool (*function_signature_func) (State&, VisitorEvent, FunctionSignatureNode*);
  bool (*function_func)           (State&, VisitorEvent, FunctionNode*);
  bool (*expressions_func)        (State&, VisitorEvent, ExpressionsNode*);
  bool (*declarations_func)       (State&, VisitorEvent, DeclarationsNode*);
  bool (*terminal_func)           (State&, VisitorEvent, TerminalNode*);
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
visit(State& state, Visitor<State>& visitor, ScopeNode* scope_node);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, AssignmentNode* assignment_node);

template <typename State>
bool
visit(State& state, Visitor<State>& visitor, TypeNameNode* type_name_node);

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
visit(State& state, Visitor<State>& visitor, FunctionSignatureNode* function_signature_node);

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

  success &= (visitor.generic_func == NULL) || visitor.generic_func(state, VisitorEvent::Enter, node);

  switch (node->type)
  {
    case (Parser::Tree::Node::Program):
    {
      success &= (visitor.program_func == NULL) || visitor.program_func(state, VisitorEvent::Enter, &node->program);
      success &= visit(state, visitor, &node->program);
      success &= (visitor.program_func == NULL) || visitor.program_func(state, VisitorEvent::Leave, &node->program);
    } break;
    case (Parser::Tree::Node::Statement):
    {
      success &= (visitor.statement_func == NULL) || visitor.statement_func(state, VisitorEvent::Enter, &node->statement);
      success &= visit(state, visitor, &node->statement);
      success &= (visitor.statement_func == NULL) || visitor.statement_func(state, VisitorEvent::Leave, &node->statement);
    } break;
    case (Parser::Tree::Node::Body):
    {
      success &= (visitor.body_func == NULL) || visitor.body_func(state, VisitorEvent::Enter, &node->body);
      success &= visit(state, visitor, &node->body);
      success &= (visitor.body_func == NULL) || visitor.body_func(state, VisitorEvent::Leave, &node->body);
    } break;
    case (Parser::Tree::Node::Scope):
    {
      success &= (visitor.scope_func == NULL) || visitor.scope_func(state, VisitorEvent::Enter, &node->scope);
      success &= visit(state, visitor, &node->scope);
      success &= (visitor.scope_func == NULL) || visitor.scope_func(state, VisitorEvent::Leave, &node->scope);
    } break;
    case (Parser::Tree::Node::Assignment):
    {
      success &= (visitor.assignment_func == NULL) || visitor.assignment_func(state, VisitorEvent::Enter, &node->assignment);
      success &= visit(state, visitor, &node->assignment);
      success &= (visitor.assignment_func == NULL) || visitor.assignment_func(state, VisitorEvent::Leave, &node->assignment);
    } break;
    case (Parser::Tree::Node::Declaration):
    {
      success &= (visitor.declaration_func == NULL) || visitor.declaration_func(state, VisitorEvent::Enter, &node->declaration);
      success &= visit(state, visitor, &node->declaration);
      success &= (visitor.declaration_func == NULL) || visitor.declaration_func(state, VisitorEvent::Leave, &node->declaration);
    } break;
    case (Parser::Tree::Node::TypeName):
    {
      success &= (visitor.type_name_func == NULL) || visitor.type_name_func(state, VisitorEvent::Enter, &node->type_name);
      success &= visit(state, visitor, &node->type_name);
      success &= (visitor.type_name_func == NULL) || visitor.type_name_func(state, VisitorEvent::Leave, &node->type_name);
    } break;
    case (Parser::Tree::Node::Expression):
    {
      success &= (visitor.expression_func == NULL) || visitor.expression_func(state, VisitorEvent::Enter, &node->expression);
      success &= visit(state, visitor, &node->expression);
      success &= (visitor.expression_func == NULL) || visitor.expression_func(state, VisitorEvent::Leave, &node->expression);
    } break;
    case (Parser::Tree::Node::Literal):
    {
      success &= (visitor.literal_func == NULL) || visitor.literal_func(state, VisitorEvent::Enter, &node->literal);
      success &= visit(state, visitor, &node->literal);
      success &= (visitor.literal_func == NULL) || visitor.literal_func(state, VisitorEvent::Leave, &node->literal);
    } break;
    case (Parser::Tree::Node::FunctionCall):
    {
      success &= (visitor.function_call_func == NULL) || visitor.function_call_func(state, VisitorEvent::Enter, &node->function_call);
      success &= visit(state, visitor, &node->function_call);
      success &= (visitor.function_call_func == NULL) || visitor.function_call_func(state, VisitorEvent::Leave, &node->function_call);
    } break;
    case (Parser::Tree::Node::FunctionSignature):
    {
      success &= (visitor.function_signature_func == NULL) || visitor.function_signature_func(state, VisitorEvent::Enter, &node->function_signature);
      success &= visit(state, visitor, &node->function_signature);
      success &= (visitor.function_signature_func == NULL) || visitor.function_signature_func(state, VisitorEvent::Leave, &node->function_signature);
    } break;
    case (Parser::Tree::Node::Function):
    {
      success &= (visitor.function_func == NULL) || visitor.function_func(state, VisitorEvent::Enter, &node->function);
      success &= visit(state, visitor, &node->function);
      success &= (visitor.function_func == NULL) || visitor.function_func(state, VisitorEvent::Leave, &node->function);
    } break;
    case (Parser::Tree::Node::Expressions):
    {
      success &= (visitor.expressions_func == NULL) || visitor.expressions_func(state, VisitorEvent::Enter, &node->expressions);
      success &= visit(state, visitor, &node->expressions);
      success &= (visitor.expressions_func == NULL) || visitor.expressions_func(state, VisitorEvent::Leave, &node->expressions);
    } break;
    case (Parser::Tree::Node::Declarations):
    {
      success &= (visitor.declarations_func == NULL) || visitor.declarations_func(state, VisitorEvent::Enter, &node->declarations);
      success &= visit(state, visitor, &node->declarations);
      success &= (visitor.declarations_func == NULL) || visitor.declarations_func(state, VisitorEvent::Leave, &node->declarations);
    } break;
    case (Parser::Tree::Node::Terminal):
    {
      success &= (visitor.terminal_func == NULL) || visitor.terminal_func(state, VisitorEvent::Enter, &node->terminal);
      success &= visit(state, visitor, &node->terminal);
      success &= (visitor.terminal_func == NULL) || visitor.terminal_func(state, VisitorEvent::Leave, &node->terminal);
    } break;
  }

  success &= (visitor.generic_func == NULL) || visitor.generic_func(state, VisitorEvent::Leave, node);

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
visit(State& state, Visitor<State>& visitor, ScopeNode* scope_node)
{
  bool success = true;

  success &= visit(state, visitor, scope_node->body);

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
visit(State& state, Visitor<State>& visitor, TypeNameNode* type_name_node)
{
  bool success = true;

  switch (type_name_node->type)
  {
    case (TypeNameNode::Type::Identifier):
    {
      success &= visit(state, visitor, type_name_node->identifier);
    } break;
    case (TypeNameNode::Type::FunctionSignature):
    {
      success &= visit(state, visitor, type_name_node->function_signature);
    } break;
  }

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, DeclarationNode* declaration_node)
{
  bool success = true;

  success &= visit(state, visitor, declaration_node->type_name);
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
visit(State& state, Visitor<State>& visitor, FunctionSignatureNode* function_signature_node)
{
  bool success = true;

  success &= visit(state, visitor, function_signature_node->return_type);
  success &= visit(state, visitor, function_signature_node->declarations);

  return success;
}


template <typename State>
bool
visit(State& state, Visitor<State>& visitor, FunctionNode* function_node)
{
  bool success = true;

  success &= visit(state, visitor, function_node->function_signature);
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
