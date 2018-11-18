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


void
string(String::String const & text, StringArray::StringArray& result, Node const * node, u32 depth = 0);

void
string(String::String const & text, StringArray::StringArray& result, ProgramNode const * node, u32 depth = 0);

void
string(String::String const & text, StringArray::StringArray& result, StatementNode const * node, u32 depth = 0);

void
string(String::String const & text, StringArray::StringArray& result, BodyNode const * node, u32 depth = 0);

void
string(String::String const & text, StringArray::StringArray& result, AssignmentNode const * node, u32 depth = 0);

void
string(String::String const & text, StringArray::StringArray& result, DeclarationNode const * node, u32 depth = 0);

void
string(String::String const & text, StringArray::StringArray& result, ExpressionNode const * node, u32 depth = 0);

void
string(String::String const & text, StringArray::StringArray& result, LiteralNode const * node, u32 depth = 0);

void
string(String::String const & text, StringArray::StringArray& result, FunctionCallNode const * node, u32 depth = 0);

void
string(String::String const & text, StringArray::StringArray& result, FunctionNode const * node, u32 depth = 0);

void
string(String::String const & text, StringArray::StringArray& result, ExpressionsNode const * node, u32 depth = 0);

void
string(String::String const & text, StringArray::StringArray& result, DeclarationsNode const * node, u32 depth = 0);

void
string(String::String const & text, StringArray::StringArray& result, TerminalNode const * node, u32 depth = 0);

} // namespace Tree

} // namespace Parser

} // namespace Compolls