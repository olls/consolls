#include "ast.h"

#include "number-parser.h"
#include "tokeniser.h"
#include "allocate.h"
#include "string.h"
#include "debug.h"


namespace Compolls
{

namespace AST
{

using namespace Parser;


#define error(text, node) _error((__FUNCTION__), (text), (node))

void
_error(char const *const name, String::String const & text, Tree::Node const * node)
{
  String::String token_string = Tokeniser::string(text, {node->text_start, node->text_end});
  printf("... %s:\n\"%.*s\"\n\n", name, print_s(token_string));
}


void
print_scope(ScopeInfo const & scope)
{
  printf("Identifiers {  ");
  for (u32 i = 0; i < scope.identifiers.n_elements; ++i)
  {
    Identifiers::Identifier const & identifier = scope.identifiers[i];
    printf("%u/%u  ", identifier.string, identifier.type);
  }
  printf("}\n");
}


bool
get_number(String::String const & text, AST& ast, ScopeInfo& scope, Number& number_result, Tree::Node const * node);

bool
get_type(String::String const & text, AST& ast, ScopeInfo& scope, Type& type_result, Tree::Node const * node);

bool
get_declarations(String::String const & text, AST& ast, ScopeInfo& scope, Declarations& declarations_result, Tree::Node const * node);

bool
get_function(String::String const & text, AST& ast, ScopeInfo& scope, Function& function_result, Tree::Node const * node);

bool
get_expressions(String::String const & text, AST& ast, ScopeInfo& scope, Expressions& expressions_result, Tree::Node const * node, u32 n_expression_types, TypeSystem::ID const * expression_types);

bool
get_literal(String::String const & text, AST& ast, ScopeInfo& scope, Literal& literal_result, Tree::Node const * node, TypeSystem::ID type, TypeSystem::ID& result_type);

bool
get_identifier(String::String const & text, AST& ast, ScopeInfo& scope, Identifier& identifier_result, Tree::Node const * node, TypeSystem::ID type, bool create);

bool
get_function_call(String::String const & text, AST& ast, ScopeInfo& scope, FunctionCall& function_call_result, Tree::Node const * node, TypeSystem::ID expected_type);

bool
get_declaration(String::String const & text, AST& ast, ScopeInfo& scope, Declaration& declaration_result, Tree::Node const * node);

bool
get_expression(String::String const & text, AST& ast, ScopeInfo& scope, Expression& expression_result, Tree::Node const * node, TypeSystem::ID expected_type, TypeSystem::ID& result_type);

bool
get_assignment(String::String const & text, AST& ast, ScopeInfo& scope, Assignment& assignment_result, Tree::Node const * node);

bool
get_statement(String::String const & text, AST& ast, ScopeInfo& scope, Statement& statement_result, Tree::Node const * node, TypeSystem::ID type);

bool
get_body(String::String const & text, AST& ast, ScopeInfo& scope, Body& body_result, Tree::Node const * node, TypeSystem::ID type);

bool
get_program(String::String const & text, AST& ast, ScopeInfo& scope, Program& program_result, Tree::Node const * node);


bool
get_number(String::String const & text, AST& ast, ScopeInfo& scope, Number& number_result, Tree::Node const * node)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Terminal);
  Tree::TerminalNode const * terminal = &node->terminal;

  number_result.text = Tokeniser::string(text, terminal->token);
  success &= NumberParser::parse_u32(number_result.text, number_result.number);

  if (!success)
  {
    error(text, node);
  }

  return success;
}


bool
get_type(String::String const & text, AST& ast, ScopeInfo& scope, Type& type_result, Tree::Node const * node)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Terminal);
  Tree::TerminalNode const * terminal = &node->terminal;

  String::String label = Tokeniser::string(text, terminal->token);
  type_result.label = Strings::add(ast.strings, label);

  type_result.type = TypeSystem::find(scope.types, type_result.label);

  if (type_result.type == TypeSystem::InvalidID)
  {
    success &= false;

    printf("Unrecognised type \"%.*s\"\n", print_s(label));
  }

  if (!success)
  {
    error(text, node);
  }

  return success;
}


bool
get_declarations(String::String const & text, AST& ast, ScopeInfo& scope, Declarations& declarations_result, Tree::Node const * node)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Declarations);
  Tree::DeclarationsNode const * declarations = &node->declarations;

  declarations_result.n = declarations->declarations.n_elements;
  declarations_result.declarations = Allocate::allocate<Declaration>(declarations_result.n);

  for (u32 declaration_index = 0;
       declaration_index < declarations->declarations.n_elements;
       ++declaration_index)
  {
    Tree::Node const * declaration = declarations->declarations[declaration_index];

    success &= get_declaration(text, ast, scope, declarations_result.declarations[declaration_index], declaration);
  }

  if (!success)
  {
    error(text, node);
  }

  return success;
}


bool
get_function(String::String const & text, AST& ast, ScopeInfo& scope, Function& function_result, Tree::Node const * node, TypeSystem::ID& result_type)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Function);
  Tree::FunctionNode const * function = &node->function;

  success &= get_type(text, ast, scope, function_result.return_type, function->return_type);
  success &= get_declarations(text, ast, scope, function_result.declarations, function->declarations);

  function_result.body = Allocate::allocate<Body>();
  success &= get_body(text, ast, scope, *function_result.body, function->body, function_result.return_type.type);

  TypeSystem::Type type = {
    .type = TypeSystem::Type::BuiltIn::Func,
    .string = ast.built_in_types.func_string,
    .function.return_type = function_result.return_type.type
  };

  type.function.n_arg_types = function_result.declarations.n;
  type.function.arg_types = Allocate::allocate<TypeSystem::ID>(type.function.n_arg_types);
  for (u32 index = 0;
       index < type.function.n_arg_types;
       ++index)
  {
    type.function.arg_types[index] = function_result.declarations.declarations[index].type.type;
  }

  result_type = TypeSystem::add(scope.types, type);

  if (!success)
  {
    error(text, node);
  }

  return success;
}


bool
get_expressions(String::String const & text, AST& ast, ScopeInfo& scope, Expressions& expressions_result, Tree::Node const * node, u32 n_expression_types, TypeSystem::ID const * expression_types)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Expressions);
  Tree::ExpressionsNode const * expressions = &node->expressions;

  expressions_result.n = expressions->expressions.n_elements;
  expressions_result.expressions = Allocate::allocate<Expression>(expressions_result.n);

  if (n_expression_types == TypeSystem::InvalidID)
  {
    // Infer types from parse-tree
    n_expression_types = expressions->expressions.n_elements;
    expression_types = NULL;
  }

  if (expressions->expressions.n_elements != n_expression_types)
  {
    printf("Type error: mismatching number of arguments, expecting %u, got %u\n", n_expression_types, expressions->expressions.n_elements);
    success &= false;
  }
  else
  {
    for (u32 expression_index = 0;
         expression_index < expressions->expressions.n_elements;
         ++expression_index)
    {
      Tree::Node const * expression = expressions->expressions[expression_index];
      assert(expression->type == Tree::Node::Type::Expression);

      TypeSystem::ID expected_type = TypeSystem::InvalidID;
      if (expression_types != NULL)
      {
        expected_type = expression_types[expression_index];
      }

      TypeSystem::ID result_type;
      success &= get_expression(text, ast, scope, expressions_result.expressions[expression_index], expression, expected_type, result_type);
      if (result_type != expected_type)
      {
        printf("result_type(%u) != expected_type(%u)\n", result_type, expected_type);
        success &= false;
        break;
      }
    }
  }

  if (!success)
  {
    error(text, node);
  }

  return success;
}


String::String
literal_type_string(Tree::LiteralNode::Type const & type)
{
  String::String result = {};
  switch (type)
  {
    case (Tree::LiteralNode::Type::Number):
    {
      result = "Number";
    } break;
    case (Tree::LiteralNode::Type::Function):
    {
      result = "Function";
    } break;
  }
  return result;
}


bool
get_literal(String::String const & text, AST& ast, ScopeInfo& scope, Literal& literal_result, Tree::Node const * node, TypeSystem::ID type, TypeSystem::ID& result_type)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Literal);
  Tree::LiteralNode const * literal = &node->literal;

  if (type != TypeSystem::InvalidID)
  {
    Tree::LiteralNode::Type expected_symbol;
    if (!TypeSystem::resolve_literal_symbol_type(scope.types, ast.built_in_types, type, &expected_symbol))
    {
      success &= false;
    }
    else if (expected_symbol != literal->type)
    {
      printf("Type mismatch in literal!  Expecting symbol type of \"%.*s\" (%u), got \"%.*s\"\n",
             print_s(literal_type_string(expected_symbol)), type,
             print_s(literal_type_string(literal->type)));
      success &= false;
    }
  }

  if (success)
  {
    switch (literal->type)
    {
      case (Tree::LiteralNode::Type::Number):
      {
        // TODO: get_number does not distinguish different types
        result_type = type;

        literal_result.type = Literal::Type::Number;
        success &= get_number(text, ast, scope, literal_result.number, literal->number);
      } break;
      case (Tree::LiteralNode::Type::Function):
      {
        literal_result.type = Literal::Type::Function;
        success &= get_function(text, ast, scope, literal_result.function, literal->function, result_type);
      } break;
    }
  }

  if (!success)
  {
    error(text, node);
  }

  return success;
}


bool
get_identifier(String::String const & text, AST& ast, ScopeInfo& scope, Identifier& identifier_result, Tree::Node const * node, TypeSystem::ID type, bool create)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Terminal);
  Tree::TerminalNode const * terminal = &node->terminal;

  String::String label = Tokeniser::string(text, terminal->token);
  identifier_result.label = Strings::add(ast.strings, label);

  Identifiers::ID identifier_id = Identifiers::find(scope.identifiers, identifier_result.label);
  if (identifier_id == Identifiers::InvalidID)
  {
    if (create)
    {
      Identifiers::Identifier identifier = {};
      identifier.string = identifier_result.label;
      identifier.type = type;

      identifier_result.identifier = scope.identifiers.n_elements;
      Array::add(scope.identifiers, identifier);
    }
    else
    {
      printf("Unknown identifier: \"%.*s\"\n", print_s(label));
      success &= false;
    }
  }
  else
  {
    identifier_result.identifier = identifier_id;
    Identifiers::Identifier const& identifier = scope.identifiers[identifier_id];

    if (type != TypeSystem::InvalidID &&
        identifier.type != type)
    {
      printf("Identifier type %u does not match expected type %u\n", identifier.type, type);
      success &= false;
    }
  }

  if (!success)
  {
    error(text, node);
  }

  return success;
}


bool
get_function_call(String::String const & text, AST& ast, ScopeInfo& scope, FunctionCall& function_call_result, Tree::Node const * node, TypeSystem::ID expected_type)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::FunctionCall);
  Tree::FunctionCallNode const * function_call = &node->function_call;

  success &= get_identifier(text, ast, scope, function_call_result.identifier, function_call->label, TypeSystem::InvalidID, false);

  if (success)
  {
    // Look up the function label to get the arg-types

    Identifiers::ID identifier_id = function_call_result.identifier.identifier;
    Identifiers::Identifier const & identifier = scope.identifiers[identifier_id];
    TypeSystem::Type func_type = scope.types[identifier.type];

    if (func_type.type != TypeSystem::Type::BuiltIn::Func)
    {
      printf("Function call identifier type (%u) is not Func\n", identifier.type);
      success &= false;
    }
    else
    {
      if (expected_type != TypeSystem::InvalidID &&
          func_type.function.return_type != expected_type)
      {
        printf("Function %u return type %u does not match expected %u\n", identifier.string, func_type.function.return_type, expected_type);
        success &= false;
      }
      else
      {
        success &= get_expressions(text, ast, scope, function_call_result.expressions, function_call->expressions, func_type.function.n_arg_types, func_type.function.arg_types);
      }
    }

    if (!success)
    {
      printf("Function type %u\n", identifier.type);
    }
  }

  if (!success)
  {
    error(text, node);
  }

  return success;
}


bool
get_declaration(String::String const & text, AST& ast, ScopeInfo& scope, Declaration& declaration_result, Tree::Node const * node)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Declaration);
  Tree::DeclarationNode const * declaration = &node->declaration;

  success &= get_type(text, ast, scope, declaration_result.type, declaration->type);

  if (success)
  {
    TypeSystem::ID type = declaration_result.type.type;
    success &= get_identifier(text, ast, scope, declaration_result.identifier, declaration->label, type, true);
  }

  if (!success)
  {
    error(text, node);
  }

  return success;
}


bool
get_expression(String::String const & text, AST& ast, ScopeInfo& scope, Expression& expression_result, Tree::Node const * node, TypeSystem::ID expected_type, TypeSystem::ID& result_type)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Expression);
  Tree::ExpressionNode const * expression = &node->expression;

  switch (expression->type)
  {
    case (Tree::ExpressionNode::Type::Literal):
    {
      expression_result.type = Expression::Type::Literal;
      success &= get_literal(text, ast, scope, expression_result.literal, expression->literal, expected_type, result_type);
    } break;

    case (Tree::ExpressionNode::Type::FunctionCall):
    {
      expression_result.type = Expression::Type::FunctionCall;
      success &= get_function_call(text, ast, scope, expression_result.function_call, expression->function_call, expected_type);

      result_type = expected_type;
    } break;

    case (Tree::ExpressionNode::Type::Identifier):
    {
      expression_result.type = Expression::Type::Identifier;
      success &= get_identifier(text, ast, scope, expression_result.identifier, expression->identifier, expected_type, false);

      result_type = expected_type;
    } break;
  }

  if (!success)
  {
    error(text, node);
  }

  return success;
}


bool
get_assignment(String::String const & text, AST& ast, ScopeInfo& scope, Assignment& assignment_result, Tree::Node const * node)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Assignment);
  Tree::AssignmentNode const * assignment = &node->assignment;

  success &= get_declaration(text, ast, scope, assignment_result.declaration, assignment->declaration);

  if (success)
  {
    TypeSystem::ID declaration_type = assignment_result.declaration.type.type;
    TypeSystem::ID result_type;
    success &= get_expression(text, ast, scope, assignment_result.expression, assignment->expression, declaration_type, result_type);
    assignment_result.declaration.type.type = result_type;
    Identifiers::Identifier& identifier = scope.identifiers[assignment_result.declaration.identifier.identifier];
    identifier.type = result_type;
  }

  if (!success)
  {
    error(text, node);
  }

  return success;
}


bool
get_statement(String::String const & text, AST& ast, ScopeInfo& scope, Statement& statement_result, Tree::Node const * node, TypeSystem::ID type)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Statement);
  Tree::StatementNode const * statement = &node->statement;

  switch (statement->type)
  {
    case (Tree::StatementNode::Type::Assignment):
    {
      if (type != TypeSystem::InvalidID)
      {
        printf("Cannot specify statement return type when statement is an assignment\n");
        success &= false;
      }
      else
      {
        statement_result.type = Statement::Type::Assignment;
        success &= get_assignment(text, ast, scope, statement_result.assignment, statement->assignment);
      }
    } break;

    case (Tree::StatementNode::Type::Expression):
    {
      statement_result.type = Statement::Type::Expression;

      TypeSystem::ID return_type;
      success &= get_expression(text, ast, scope, statement_result.expression, statement->expression, type, return_type);

      assert(type == TypeSystem::InvalidID ||
             type == return_type);
    } break;
  }

  if (!success)
  {
    error(text, node);
  }

  return success;
}


bool
get_body(String::String const & text, AST& ast, ScopeInfo& scope, Body& body_result, Tree::Node const * node, TypeSystem::ID type)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Body);
  Tree::BodyNode const * body = &node->body;

  body_result.scope = {};

  // Initialise scope with copy of current scope
  body_result.scope = {};

  body_result.scope.types += scope.types;
  body_result.scope.identifiers += scope.identifiers;

  body_result.n = body->statements.n_elements;
  body_result.statements = Allocate::allocate<Statement>(body_result.n);

  for (u32 statement_index = 0;
       statement_index < body->statements.n_elements;
       ++statement_index)
  {
    Tree::Node const * statement = body->statements[statement_index];

    TypeSystem::ID statement_type = TypeSystem::InvalidID;
    if (statement_index == body->statements.n_elements - 1)
    {
      statement_type = type;
    }

    success &= get_statement(text, ast, body_result.scope, body_result.statements[statement_index], statement, statement_type);

    if (!success)
    {
      break;
    }
  }

  if (!success)
  {
    error(text, node);
  }

  return success;
}


bool
get_program(String::String const & text, AST& ast, ScopeInfo& scope, Program& program_result, Tree::Node const * node)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Program);
  Tree::ProgramNode const * program = &node->program;

  success &= get_body(text, ast, scope, program_result.body, program->body, TypeSystem::InvalidID);

  if (!success)
  {
    error(text, node);
  }

  return success;
}


bool
make_ast(String::String const & text, AST& result, Tree::Node *const node)
{
  bool success = true;

  result.strings = {};

  result.built_in_types = {};

  ScopeInfo global_scope = {};
  global_scope.types = {};
  TypeSystem::init_built_in_strings(global_scope.types, result.built_in_types, result.strings);

  global_scope.identifiers = {};
  Identifiers::init_built_in_identifiers(global_scope.identifiers, result.strings, global_scope.types, result.built_in_types);

  success &= get_program(text, result, global_scope, result.program, node);

  return success;
}

} // namespace AST

} // namespace Compolls