#pragma once

#include "tokeniser.h"

#include "utils/array.h"
#include "utils/string.h"
#include "utils/string-array.h"
#include "utils/types.h"


namespace Compolls
{

namespace Parser
{

namespace Tree
{

struct ProgramNode;
struct StatementNode;
struct BodyNode;
struct ScopeNode;
struct AssignmentNode;
struct TypeNameNode;
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
  enum class Type
  {
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

struct ScopeNode
{
  Node* body;
};

struct AssignmentNode
{
  Node* declaration;
  Node* expression;
};

struct TypeNameNode
{
  enum class Type
  {
    Identifier,
    FunctionSignature
  } type;
  union
  {
    Node* identifier;
    Node* function_signature;
  };
};

struct DeclarationNode
{
  Node* type_name;
  Node* label;
};

struct ExpressionNode
{
  enum class Type
  {
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
  enum class Type
  {
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

struct FunctionSignatureNode
{
  Node* return_type;
  Node* declarations;
};

struct FunctionNode
{
  Node* function_signature;
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

  enum Type
  {
    Program,
    Statement,
    Body,
    Scope,
    Assignment,
    TypeName,
    Declaration,
    Expression,
    Literal,
    FunctionCall,
    FunctionSignature,
    Function,
    Expressions,
    Declarations,
    Terminal
  } type;
  enum { TypeCount = Type::Terminal + 1 };

  union
  {
    ProgramNode program;
    StatementNode statement;
    BodyNode body;
    ScopeNode scope;
    AssignmentNode assignment;
    TypeNameNode type_name;
    DeclarationNode declaration;
    ExpressionNode expression;
    LiteralNode literal;
    FunctionCallNode function_call;
    FunctionSignatureNode function_signature;
    FunctionNode function;
    ExpressionsNode expressions;
    DeclarationsNode declarations;
    TerminalNode terminal;
  };
};


bool
string(String::String const& text, Node* node, StringArray::StringArray& result);

} // namespace Tree

} // namespace Parser

} // namespace Compolls
