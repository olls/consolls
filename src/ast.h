#pragma once

#include "identifiers.h"
#include "type-system.h"
#include "strings.h"
#include "parse-tree.h"
#include "string.h"
#include "types.h"


namespace Compolls
{

namespace AST
{

struct ScopeInfo
{
  TypeSystem::Types types;
  Identifiers::Identifiers identifiers;
};


struct Program;
struct Body;
struct Statement;
struct Assignment;
struct Declaration;
struct Expression;
struct FunctionCall;
struct Literal;
struct Function;
struct Number;
struct Declarations;
struct Expressions;
struct Type;
struct Identifier;


struct Identifier
{
  Strings::ID label;
  Identifiers::ID identifier;
};


struct Type
{
  Strings::ID label;
  TypeSystem::ID type;
};


struct Expressions
{
  u32 n;
  Expression* expressions;
};


struct Declarations
{
  u32 n;
  Declaration* declarations;
};


struct Number
{
  String::String text;
  u32 number;
};


struct Function
{
  Type return_type;
  Declarations declarations;
  Body* body;
};


struct Literal
{
  enum class Type {
    Number,
    Function
  } type;
  union
  {
    Number number;
    Function function;
  };
};


struct FunctionCall
{
  Identifier identifier;
  Expressions expressions;
};


struct Expression
{
  enum class Type {
    Literal,
    FunctionCall,
    Identifier
  } type;
  union
  {
    Literal literal;
    FunctionCall function_call;
    Identifier identifier;
  };

  Strings::ID return_type;
};


struct Declaration
{
  Type type;
  Identifier identifier;
};


struct Assignment
{
  Declaration declaration;
  Expression expression;
};


struct Statement
{
  enum class Type
  {
    Assignment,
    Expression
  } type;
  union
  {
    Assignment assignment;
    Expression expression;
  };
};


struct Body
{
  u32 n;
  Statement* statements;

  ScopeInfo scope;
};


struct Program
{
  Body body;
};


struct AST
{
  Program program;

  Strings::Table strings;

  TypeSystem::BuiltInTypes built_in_types;
};


bool
make_ast(String::String const & text, AST& result, Parser::Tree::Node *const node);

} // namespace AST

} // namespace Compolls