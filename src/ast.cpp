#include "ast.h"

#include "number-parser.h"
#include "tokeniser.h"
#include "allocate.h"
#include "strings.h"
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
print_scope(ScopeInfo const & scope, Strings::Table const& strings)
{
  printf("Identifiers {  ");
  for (u32 i = 0; i < scope.identifiers.n_elements; ++i)
  {
    Identifiers::Identifier const & identifier = scope.identifiers[i];
    printf("%u: \"%.*s\" %u,  ", i,
           print_s(Strings::get(strings, identifier.string)),
           identifier.type);
  }
  printf("}\n");
}


bool
get_number(AST& ast, ScopeInfo& scope, Number& number_result, Tree::Node const * node, TypeSystem::ID type, TypeSystem::ID& type_result);

bool
get_declarations(AST& ast, ScopeInfo& scope, Declarations& declarations_result, Tree::Node const * node);

bool
get_function_signature_type(AST& ast, ScopeInfo& scope, TypeSystem::ID& type_result, Tree::Node const * node);

bool
get_function(AST& ast, ScopeInfo& scope, Function& function_result, Tree::Node const * node);

bool
get_expressions(AST& ast, ScopeInfo& scope, Expressions& expressions_result, Tree::Node const * node, u32 n_expression_types, TypeSystem::ID const * expression_types);

bool
get_literal(AST& ast, ScopeInfo& scope, Literal& literal_result, Tree::Node const * node, TypeSystem::ID type, TypeSystem::ID& result_type);

bool
get_identifier(AST& ast, ScopeInfo& scope, Identifier& identifier_result, Tree::Node const * node, TypeSystem::ID type, bool create);

bool
get_function_call(AST& ast, ScopeInfo& scope, FunctionCall& function_call_result, Tree::Node const * node, TypeSystem::ID expected_type);

bool
get_declaration(AST& ast, ScopeInfo& scope, Declaration& declaration_result, Tree::Node const * node);

bool
get_expression(AST& ast, ScopeInfo& scope, Expression& expression_result, Tree::Node const * node, TypeSystem::ID expected_type, TypeSystem::ID& result_type);

bool
get_assignment(AST& ast, ScopeInfo& scope, Assignment& assignment_result, Tree::Node const * node);

bool
get_statement(AST& ast, ScopeInfo& scope, Statement& statement_result, Tree::Node const * node, TypeSystem::ID type);

bool
get_body(AST& ast, ScopeInfo& scope, Body& body_result, Tree::Node const * node, TypeSystem::ID type);

bool
get_program(AST& ast, ScopeInfo& scope, Program& program_result, Tree::Node const * node);


bool
get_number(AST& ast, ScopeInfo& scope, Number& number_result, Tree::Node const * node, TypeSystem::ID expected_type, TypeSystem::ID& type_result)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Terminal);
  Tree::TerminalNode const * terminal = &node->terminal;

  number_result.text = Tokeniser::string(*ast.text, terminal->token);
  success &= NumberParser::parse_u32(number_result.text, number_result.number);

  if (success)
  {
    TypeSystem::Type const& type = scope.types.types[expected_type];
    switch (type.type)
    {
      case (TypeSystem::Type::BuiltIn::U8):
      {
        success &= (number_result.number <= 0xFF);
        if (!success)
        {
          printf("%u is too large to fit in u8 type (max %u)\n", number_result.number, 0xFF);
        }
      } break;
      case (TypeSystem::Type::BuiltIn::U16):
      {
        success &= (number_result.number <= 0xFFFF);
        if (!success)
        {
          printf("%u is too large to fit in u16 type (max %u)\n", number_result.number, 0xFFFF);
        }
      } break;
      case (TypeSystem::Type::BuiltIn::Func):
      {
        assert(false);
      } break;
    }
  }

  type_result = expected_type;

  if (!success)
  {
    error(*ast.text, node);
  }

  return success;
}

bool
get_type_identifier(AST& ast, ScopeInfo& scope, TypeSystem::ID& type_result, Tree::Node const * node)
{
  type_result = TypeSystem::InvalidID;

  assert(node->type == Tree::Node::Type::Terminal);
  Tree::TerminalNode const * terminal = &node->terminal;

  Strings::ID const& type_string = terminal->token.string;

  // TODO: There is no way to define user 'identifier' types yet

  type_result = TypeSystem::find(scope.types, type_string);
  bool success = (type_result != TypeSystem::InvalidID);

  if (!success)
  {
    printf("Cannot determine type name: %.*s", print_s(Strings::get(*ast.strings, type_string)));
    error(*ast.text, node);
  }

  return success;
}

bool
get_type(AST& ast, ScopeInfo& scope, TypeSystem::ID& type_result, Tree::Node const * node)
{
  type_result = TypeSystem::InvalidID;

  assert(node->type == Tree::Node::Type::TypeName);
  Tree::TypeNameNode const * type_name = &node->type_name;

  switch (type_name->type)
  {
    case (Tree::TypeNameNode::Type::Identifier):
    {
      get_type_identifier(ast, scope, type_result, type_name->identifier);
    } break;
    case (Tree::TypeNameNode::Type::FunctionSignature):
    {
      get_function_signature_type(ast, scope, type_result, type_name->function_signature);
    } break;
  }

  bool success = (type_result != TypeSystem::InvalidID);

  if (!success)
  {
    error(*ast.text, node);
  }

  return success;
}


bool
get_declarations(AST& ast, ScopeInfo& scope, Declarations& declarations_result, Tree::Node const * node)
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

    success &= get_declaration(ast, scope, declarations_result.declarations[declaration_index], declaration);
  }

  if (!success)
  {
    error(*ast.text, node);
  }

  return success;
}


bool
get_function_signature_type(AST& ast, ScopeInfo& scope, TypeSystem::ID& type_result, Tree::Node const * node)
{
  type_result = TypeSystem::InvalidID;

  assert(node->type == Tree::Node::Type::FunctionSignature);
  Tree::FunctionSignatureNode const * function_signature = &node->function_signature;

  TypeSystem::ID return_type = TypeSystem::InvalidID;
  if (get_type(ast, scope, return_type, function_signature->return_type))
  {
    // TODO: When using this function for a function value, we will need to store the declarations, so we know the parameter identifiers!

    Declarations declarations;
    if (get_declarations(ast, scope, declarations, function_signature->declarations))
    {
      String::String node_string = String::sub_string(*ast.text, node->text_start, node->text_end);
      Strings::ID node_string_id = Strings::add(*ast.strings, node_string);

      TypeSystem::Type type = {
        .type = TypeSystem::Type::BuiltIn::Func,
        .string = node_string_id,
        .function.return_type = return_type,
        .function.n_arg_types = declarations.n,
        .function.arg_types = Allocate::allocate<TypeSystem::ID>(type.function.n_arg_types)
      };

      for (u32 index = 0;
           index < type.function.n_arg_types;
           ++index)
      {
        type.function.arg_types[index] = declarations.declarations[index].type;
      }

      type_result = TypeSystem::add(scope.types, type);
    }
  }

  bool success = (type_result != TypeSystem::InvalidID);
  if (!success)
  {
    error(*ast.text, node);
  }

  return success;
}


bool
get_function(AST& ast, ScopeInfo& scope, Function& function_result, Tree::Node const * node)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Function);
  Tree::FunctionNode const * function = &node->function;

  success &= get_function_signature_type(ast, scope, function_result.type, function->function_signature);

  if (success)
  {
    TypeSystem::Type const& type = scope.types.types[function_result.type];
    assert(type.type == TypeSystem::Type::BuiltIn::Func);

    function_result.body = Allocate::allocate<Body>();
    success &= get_body(ast, scope, *function_result.body, function->body, type.function.return_type);
  }

  if (!success)
  {
    error(*ast.text, node);
  }

  return success;
}


bool
get_expressions(AST& ast, ScopeInfo& scope, Expressions& expressions_result, Tree::Node const * node, u32 n_expression_types, TypeSystem::ID const * expression_types)
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
      success &= get_expression(ast, scope, expressions_result.expressions[expression_index], expression, expected_type, result_type);
      if (result_type != expected_type)
      {
        String::String result_type_string = TypeSystem::string(scope.types, result_type);
        String::String expected_type_string = TypeSystem::string(scope.types, expected_type);
        printf("Expression type %.*s does not equal expected type %.*s\n", print_s(result_type_string), print_s(expected_type_string));
        success &= false;
        break;
      }
    }
  }

  if (!success)
  {
    error(*ast.text, node);
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
get_literal(AST& ast, ScopeInfo& scope, Literal& literal_result, Tree::Node const * node, TypeSystem::ID type, TypeSystem::ID& result_type)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Literal);
  Tree::LiteralNode const * literal = &node->literal;

  if (type != TypeSystem::InvalidID)
  {
    Tree::LiteralNode::Type expected_symbol = TypeSystem::get_symbol_type(scope.types, type);
    if (expected_symbol != literal->type)
    {
      String::String expected_type_string = TypeSystem::string(scope.types, type);
      printf("Type mismatch in literal!  Expecting symbol type of %.*s %.*s, got %.*s\n",
             print_s(literal_type_string(expected_symbol)),
             print_s(expected_type_string),
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
        literal_result.type = Literal::Type::Number;
        success &= get_number(ast, scope, literal_result.number, literal->number, type, result_type);
      } break;
      case (Tree::LiteralNode::Type::Function):
      {
        literal_result.type = Literal::Type::Function;
        success &= get_function(ast, scope, literal_result.function, literal->function);
        if (success)
        {
          success &= (literal_result.function.type == type);
          if (!success)
          {
            String::String function_type_string = TypeSystem::string(scope.types, literal_result.function.type);
            String::String type_string = TypeSystem::string(scope.types, type);
            printf("Function literal type %.*s does not match expected type %.*s.\n", print_s(function_type_string), print_s(type_string));
          }
          else
          {
            result_type = literal_result.function.type;
          }
        }
      } break;
    }
  }

  if (!success)
  {
    error(*ast.text, node);
  }

  return success;
}


bool
get_identifier(AST& ast, ScopeInfo& scope, Identifier& identifier_result, Tree::Node const * node, TypeSystem::ID type, bool create)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Terminal);
  Tree::TerminalNode const * terminal = &node->terminal;

  String::String label = Tokeniser::string(*ast.text, terminal->token);
  identifier_result.label = Strings::add(*ast.strings, label);

  Identifiers::ID identifier_id = Identifiers::find(scope.identifiers, identifier_result.label);
  if (identifier_id == Identifiers::InvalidID)
  {
    if (create)
    {
      Identifiers::Identifier identifier = {};
      identifier.string = identifier_result.label;
      identifier.type = type;

      identifier_result.identifier = Identifiers::add(scope.identifiers, identifier);
    }
    else
    {
      printf("Unknown identifier: \"%.*s\"\n", print_s(label));
      success &= false;
    }
  }
  else
  {
    if (create)
    {
      printf("Identifier with name %.*s already exists.\n", print_s(label));
      success &= false;
    }
    else
    {
      identifier_result.identifier = identifier_id;
      Identifiers::Identifier const& identifier = scope.identifiers[identifier_id];

      if (type != TypeSystem::InvalidID &&
          identifier.type != type)
      {
        String::String identifier_type_string = TypeSystem::string(scope.types, identifier.type);
        String::String type_string = TypeSystem::string(scope.types, type);
        printf("Identifier \"%.*s\" type %.*s does not match expected type %.*s\n",
               print_s(label), print_s(identifier_type_string), print_s(type_string));
        success &= false;
      }
    }
  }

  if (!success)
  {
    error(*ast.text, node);
  }

  return success;
}


bool
get_function_call(AST& ast, ScopeInfo& scope, FunctionCall& function_call_result, Tree::Node const * node, TypeSystem::ID expected_type)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::FunctionCall);
  Tree::FunctionCallNode const * function_call = &node->function_call;

  success &= get_identifier(ast, scope, function_call_result.identifier, function_call->label, TypeSystem::InvalidID, false);

  if (success)
  {
    // Look up the function label to get the arg-types

    Identifiers::ID identifier_id = function_call_result.identifier.identifier;
    Identifiers::Identifier const & identifier = scope.identifiers[identifier_id];
    TypeSystem::Type func_type = scope.types.types[identifier.type];

    if (func_type.type != TypeSystem::Type::BuiltIn::Func)
    {
      String::String identifier_string = Strings::get(*ast.strings, function_call_result.identifier.label);
      String::String identifier_type_string = TypeSystem::string(scope.types, identifier.type);
      printf("Function call identifier \"%.*s\" type %.*s is not a function\n", print_s(identifier_string), print_s(identifier_type_string));
      success &= false;
    }
    else
    {
      if (expected_type != TypeSystem::InvalidID &&
          func_type.function.return_type != expected_type)
      {
        String::String name = Strings::get(*ast.strings, identifier.string);
        String::String return_type = TypeSystem::string(scope.types, func_type.function.return_type);
        String::String expected_type_string = TypeSystem::string(scope.types, expected_type);
        printf("Function %.*s return type %.*s does not match expected %.*s\n",
               print_s(name), print_s(return_type), print_s(expected_type_string));
        success &= false;
      }
      else
      {
        success &= get_expressions(ast, scope, function_call_result.expressions, function_call->expressions, func_type.function.n_arg_types, func_type.function.arg_types);
      }
    }
  }

  if (!success)
  {
    error(*ast.text, node);
  }

  return success;
}


bool
get_declaration(AST& ast, ScopeInfo& scope, Declaration& declaration_result, Tree::Node const * node)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Declaration);
  Tree::DeclarationNode const * declaration = &node->declaration;

  success &= get_type(ast, scope, declaration_result.type, declaration->type_name);

  if (success)
  {
    success &= get_identifier(ast, scope, declaration_result.identifier, declaration->label, declaration_result.type, true);
  }

  if (!success)
  {
    error(*ast.text, node);
  }

  return success;
}


bool
get_expression(AST& ast, ScopeInfo& scope, Expression& expression_result, Tree::Node const * node, TypeSystem::ID expected_type, TypeSystem::ID& result_type)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Expression);
  Tree::ExpressionNode const * expression = &node->expression;

  switch (expression->type)
  {
    case (Tree::ExpressionNode::Type::Literal):
    {
      expression_result.type = Expression::Type::Literal;
      success &= get_literal(ast, scope, expression_result.literal, expression->literal, expected_type, result_type);
    } break;

    case (Tree::ExpressionNode::Type::FunctionCall):
    {
      expression_result.type = Expression::Type::FunctionCall;
      success &= get_function_call(ast, scope, expression_result.function_call, expression->function_call, expected_type);

      result_type = expected_type;
    } break;

    case (Tree::ExpressionNode::Type::Identifier):
    {
      expression_result.type = Expression::Type::Identifier;
      success &= get_identifier(ast, scope, expression_result.identifier, expression->identifier, expected_type, false);

      result_type = expected_type;
    } break;
  }

  if (!success)
  {
    error(*ast.text, node);
  }

  return success;
}


bool
get_assignment(AST& ast, ScopeInfo& scope, Assignment& assignment_result, Tree::Node const * node)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Assignment);
  Tree::AssignmentNode const * assignment = &node->assignment;

  success &= get_declaration(ast, scope, assignment_result.declaration, assignment->declaration);

  if (success)
  {
    TypeSystem::ID result_type = TypeSystem::InvalidID;
    success &= get_expression(ast, scope, assignment_result.expression, assignment->expression, assignment_result.declaration.type, result_type);
    assert(!success || result_type == assignment_result.declaration.type);
  }

  if (!success)
  {
    error(*ast.text, node);
  }

  return success;
}


bool
get_statement(AST& ast, ScopeInfo& scope, Statement& statement_result, Tree::Node const * node, TypeSystem::ID type)
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
        // Error if we were asked to validate the statement's type
        printf("Assignment statement does not return a value.\n");
        success &= false;
      }
      else
      {
        statement_result.type = Statement::Type::Assignment;
        success &= get_assignment(ast, scope, statement_result.assignment, statement->assignment);
      }
    } break;

    case (Tree::StatementNode::Type::Expression):
    {
      statement_result.type = Statement::Type::Expression;

      TypeSystem::ID expression_type = TypeSystem::InvalidID;
      success &= get_expression(ast, scope, statement_result.expression, statement->expression, type, expression_type);

      assert(type == TypeSystem::InvalidID ||
             type == expression_type);
    } break;
  }

  if (!success)
  {
    error(*ast.text, node);
  }

  return success;
}


bool
get_body(AST& ast, ScopeInfo& scope, Body& body_result, Tree::Node const * node, TypeSystem::ID type)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Body);
  Tree::BodyNode const * body = &node->body;

  body_result.scope = {};

  // Initialise scope with copy of current scope
  // TODO: Set up proper copy-constructors for the arrays?
  body_result.scope = {};
  body_result.scope.types.built_in = scope.types.built_in;
  body_result.scope.types.strings = scope.types.strings;
  body_result.scope.types.types += scope.types.types;
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

    success &= get_statement(ast, body_result.scope, body_result.statements[statement_index], statement, statement_type);

    if (!success)
    {
      break;
    }
  }

  print_scope(body_result.scope, *ast.strings);

  if (!success)
  {
    error(*ast.text, node);
  }

  return success;
}


bool
get_program(AST& ast, ScopeInfo& scope, Program& program_result, Tree::Node const * node)
{
  bool success = true;

  assert(node->type == Tree::Node::Type::Program);
  Tree::ProgramNode const * program = &node->program;

  success &= get_body(ast, scope, program_result.body, program->body, TypeSystem::InvalidID);

  if (!success)
  {
    error(*ast.text, node);
  }

  return success;
}


bool
make_ast(String::String const & text, Strings::Table& strings, AST& result, Tree::Node *const node)
{
  bool success = true;

  result.text = &text;
  result.strings = &strings;

  ScopeInfo global_scope = {};
  global_scope.types = {};
  global_scope.types.strings = &strings;
  TypeSystem::init_built_in_types(global_scope.types);

  global_scope.identifiers = {};
  Identifiers::init_built_in_identifiers(global_scope.identifiers, *result.strings, global_scope.types);

  success &= get_program(result, global_scope, result.program, node);

  return success;
}

} // namespace AST

} // namespace Compolls