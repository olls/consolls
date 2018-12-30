#pragma once

#include "tokeniser.h"
#include "array.h"
#include "string.h"
#include "string-array.h"
#include "types.h"


namespace Compolls
{

namespace Parser
{

namespace Tree
{

struct ProgramNode;
struct StatementNode;
struct BodyNode;
struct AssignmentNode;
struct DeclarationNode;
struct ExpressionNode;
struct LiteralNode;
struct FunctionCallNode;
struct FunctionNode;
struct ExpressionsNode;
struct DeclarationsNode;
struct TerminalNode;
struct Node;

struct ProgramNode
{
  Node* body;
};

struct StatementNode
{
  enum class Type {
    Assignment,
    Expression
  } type;
  union
  {
    Node* assignment;
    Node* expression;
  };
};

struct BodyNode
{
  Array::Array<Node*> statements;
};

struct AssignmentNode
{
  Node* declaration;
  Node* expression;
};

struct DeclarationNode
{
  Node* type;
  Node* label;
};

struct ExpressionNode
{
  enum class Type {
    Literal,
    FunctionCall,
    Identifier
  } type;
  union
  {
    Node* literal;
    Node* function_call;
    Node* identifier;
  };
};

struct LiteralNode
{
  enum class Type {
    Number,
    Function
  } type;
  union
  {
    Node* number;
    Node* function;
  };
};

struct FunctionCallNode
{
  Node* label;
  Node* expressions;
};

struct FunctionNode
{
  Node* return_type;
  Node* declarations;
  Node* body;
};

struct ExpressionsNode
{
  Array::Array<Node*> expressions;
};

struct DeclarationsNode
{
  Array::Array<Node*> declarations;
};

struct TerminalNode
{
  Tokeniser::Token token;
};

struct Node
{
  u32 text_start;
  u32 text_end;

  enum Type {
    Program,
    Statement,
    Body,
    Assignment,
    Declaration,
    Expression,
    Literal,
    FunctionCall,
    Function,
    Expressions,
    Declarations,
    Terminal
  } type;

  union
  {
    ProgramNode program;
    StatementNode statement;
    BodyNode body;
    AssignmentNode assignment;
    DeclarationNode declaration;
    ExpressionNode expression;
    LiteralNode literal;
    FunctionCallNode function_call;
    FunctionNode function;
    ExpressionsNode expressions;
    DeclarationsNode declarations;
    TerminalNode terminal;
  };
};


bool
string(String::String const & text, Node *node, StringArray::StringArray& result);

} // namespace Tree

} // namespace Parser

} // namespace Compolls