#include "parse-tree.h"


namespace Compolls
{

namespace Parser
{

namespace Tree
{

#define INDENT() { for (u32 i = 0; i < depth; ++i) { result += S("  "); } }

void
string(String::String const & text, StringArray::StringArray& result, Node const * node, u32 depth)
{
  assert(node);

  INDENT();
  depth += 1;

  switch (node->type)
  {
    case (Node::Program):
    {
      result += S("Program {\n");
      string(text, result, &node->program, depth);
    } break;
    case (Node::Statement):
    {
      result += S("Statement {\n");
      string(text, result, &node->statement, depth);
    } break;
    case (Node::Body):
    {
      result += S("Body {\n");
      string(text, result, &node->body, depth);
    } break;
    case (Node::Assignment):
    {
      result += S("Assignment {\n");
      string(text, result, &node->assignment, depth);
    } break;
    case (Node::Declaration):
    {
      result += S("Declaration {\n");
      string(text, result, &node->declaration, depth);
    } break;
    case (Node::Expression):
    {
      result += S("Expression {\n");
      string(text, result, &node->expression, depth);
    } break;
    case (Node::Literal):
    {
      result += S("Literal {\n");
      string(text, result, &node->literal, depth);
    } break;
    case (Node::FunctionCall):
    {
      result += S("FunctionCall {\n");
      string(text, result, &node->function_call, depth);
    } break;
    case (Node::Function):
    {
      result += S("Function {\n");
      string(text, result, &node->function, depth);
    } break;
    case (Node::Expressions):
    {
      result += S("Expressions {\n");
      string(text, result, &node->expressions, depth);
    } break;
    case (Node::Declarations):
    {
      result += S("Declarations {\n");
      string(text, result, &node->declarations, depth);
    } break;
    case (Node::Terminal):
    {
      result += S("Terminal {\n");
      string(text, result, &node->terminal, depth);
    } break;
  }

  depth -= 1;
  INDENT();
  result += S("}\n");
}


void
string(String::String const & text, StringArray::StringArray& result, ProgramNode const * node, u32 depth)
{
  string(text, result, node->body, depth);
}

void
string(String::String const & text, StringArray::StringArray& result, StatementNode const * node, u32 depth)
{
  switch (node->type)
  {
    case (StatementNode::Type::Assignment):
    {
      string(text, result, node->assignment, depth);
    } break;
    case (StatementNode::Type::Expression):
    {
      string(text, result, node->expression, depth);
    } break;
  }
}

void
string(String::String const & text, StringArray::StringArray& result, BodyNode const * node, u32 depth)
{
  for (u32 statement_index = 0;
       statement_index < node->statements.n_elements;
       ++statement_index)
  {
    string(text, result, node->statements[statement_index], depth);
  }
}

void
string(String::String const & text, StringArray::StringArray& result, AssignmentNode const * node, u32 depth)
{
  string(text, result, node->declaration, depth);
  string(text, result, node->expression, depth);
}

void
string(String::String const & text, StringArray::StringArray& result, DeclarationNode const * node, u32 depth)
{
  string(text, result, node->type, depth);
  string(text, result, node->label, depth);
}

void
string(String::String const & text, StringArray::StringArray& result, ExpressionNode const * node, u32 depth)
{
  switch (node->type)
  {
    case (ExpressionNode::Type::Literal):
    {
      string(text, result, node->literal, depth);
    } break;
    case (ExpressionNode::Type::FunctionCall):
    {
      string(text, result, node->function_call, depth);
    } break;
    case (ExpressionNode::Type::Identifier):
    {
      string(text, result, node->identifier, depth);
    } break;
  }
}

void
string(String::String const & text, StringArray::StringArray& result, LiteralNode const * node, u32 depth)
{
  switch (node->type)
  {
    case (LiteralNode::Type::Number):
    {
      string(text, result, node->number, depth);
    } break;
    case (LiteralNode::Type::Function):
    {
      string(text, result, node->function, depth);
    } break;
  }
}

void
string(String::String const & text, StringArray::StringArray& result, FunctionCallNode const * node, u32 depth)
{
  string(text, result, node->label, depth);
  string(text, result, node->expressions, depth);
}

void
string(String::String const & text, StringArray::StringArray& result, FunctionNode const * node, u32 depth)
{
  string(text, result, node->return_type, depth);
  string(text, result, node->declarations, depth);
  string(text, result, node->body, depth);
}

void
string(String::String const & text, StringArray::StringArray& result, ExpressionsNode const * node, u32 depth)
{
  for (u32 expression_index = 0;
       expression_index < node->expressions.n_elements;
       ++expression_index)
  {
    string(text, result, node->expressions[expression_index], depth);
  }
}

void
string(String::String const & text, StringArray::StringArray& result, DeclarationsNode const * node, u32 depth)
{
  for (u32 declaration_index = 0;
       declaration_index < node->declarations.n_elements;
       ++declaration_index)
  {
    string(text, result, node->declarations[declaration_index], depth);
  }
}

void
string(String::String const & text, StringArray::StringArray& result, TerminalNode const * node, u32 depth)
{
  INDENT();
  result += S("\"");
  result += Tokeniser::string(text, node->token);
  result += S("\"\n");
}

} // namespace Tree

} // namespace Parser

} // namespace Compolls