#include "code-generator.h"

#include "instructions.h"
#include "basolls.h"
#include "strings.h"
#include "array.h"


namespace Compolls
{

namespace CodeGenerator
{

using Machine::MemoryAddress;
using namespace Instructions;


struct CodeGenerator
{
  Machine::Machine* machine;
  Strings::Table* strings;
};


struct IdentifierMapping
{
  Identifiers::ID id;
  MemoryAddress address;

  Strings::ID debug_name;
};


inline
IdentifierMapping *
find(Array::Array<IdentifierMapping> const& identifiers, Identifiers::ID id)
{
  IdentifierMapping* result = NULL;

  Array::FindFirstFunc<IdentifierMapping> find_func = [](IdentifierMapping const& i, void* user){ return i.id == *(Identifiers::ID*)user; };
  s32 index = Array::find_first(identifiers, find_func, &id);
  if (index != -1)
  {
    result = Array::get(identifiers, index);
  }

  return result;
}


struct FunctionInfo
{
  MemoryAddress data_start;
  MemoryAddress data_end;

  MemoryAddress code_start;
  MemoryAddress code_end;

  MemoryAddress return_address;
  MemoryAddress return_value;

  Array::Array<IdentifierMapping> identifiers_map;
};


//
// How to layout the program?
//
// - the bodies data goes first
// - followed by the code
//
// - build the data and code in separate lists then copy them into the correct position in memory
//   - will need relocatable code
//   - or address patching
// - or build the data and code directly in memory
//   - A. Scan through body for all declarations
//   - B. Reserve space for each declaration sequentially
//   - C. Store literal values in their reserved declarations
//     - At this point function literals will recurse into their body
//   - D. Write body's code sequentially after data section
//
//   - This relies on function bodies not depending on any of their parent scope
//     (Generally: literals are compile time constants)
//
// u8 a
// u16 b
//     |
// Func a
//   u16 a
//       |
//   u16 b
//       |
//   INST   <- Func a start
//   |
//   |
//   |
// u8 foo
// INST     <- program start
// |
// |
// |
//


// TODO: Could use a Lifo stack of the FunctionInfo's for each body-scope,
//       pushing/popping scopes on each entry/exit of generate_body_code.
//       This pattern would allow us to see the entire scope above us without
//       duplicating the arrays at each level.  This could be particularly
//       useful for shadowing variables in the AST.


bool
generate_expression_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Expression const& expression, MemoryAddress location);


bool
generate_assignment_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Assignment const& assignment)
{
  bool success = true;

  // Literals are handled in generate_body_code
  if (assignment.expression.type != AST::Expression::Type::Literal)
  {
    // Retrieve the address of this identifier's declaration

    Identifiers::ID const& identifier_id = assignment.declaration.identifier.identifier;
    IdentifierMapping const* identifier_mapping = find(func.identifiers_map, identifier_id);
    assert(identifier_mapping != NULL);  // The identifier_mapping should be created in generate_body_code

    MemoryAddress declaration_addr = identifier_mapping->address;

    // Store the result of the expression at declaration_addr
    success &= generate_expression_code(code_generator, scope, func, assignment.expression, declaration_addr);
  }

  return success;
}

bool
generate_function_call_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::FunctionCall const& function_call, MemoryAddress location)
{
  bool success = true;

  // TODO: fill with addresses of function parameters
  MemoryAddress* expression_results = Allocate::allocate<MemoryAddress>(function_call.expressions.n);

  for (u32 expression_index = 0;
       expression_index < function_call.expressions.n;
       ++expression_index)
  {
    success &= generate_expression_code(code_generator, scope, func, function_call.expressions.expressions[expression_index], expression_results[expression_index]);

    if (!success)
    {
       break;
    }
  }

  Allocate::unallocate(expression_results);

  if (success)
  {
    // Call function_call.identifier
    // put function result in location
  }

  return success;
}


bool
generate_identifier_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Identifier const& identifier, MemoryAddress location)
{
  bool success = true;

  // Retrieve the value in the identifier and store in location

  IdentifierMapping const* identifier_mapping = find(func.identifiers_map, identifier.identifier);
  assert(identifier_mapping != NULL);

  MemoryAddress identifier_addr = identifier_mapping->address;

  Basolls::push_instruction<Code::COPY>(*code_generator.machine, func.code_end, {
    .from = identifier_addr,
    .to = location
  });

  return success;
}


bool
generate_expression_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Expression const& expression, MemoryAddress location)
{
  bool success = true;

  switch (expression.type)
  {
    case (AST::Expression::Type::Literal):
    {
      // Literals are stored in the data section in generate_body_code.
      // The stored literals should be given a id.
      // Identifiers should then just point at the literals' ids.
      //
      // Then, here: we can retrieve the literal address from it's id.
      // success &= generate_literal_code(code_generator, scope, func, expression.literal, location);
    } break;
    case (AST::Expression::Type::FunctionCall):
    {
      success &= generate_function_call_code(code_generator, scope, func, expression.function_call, location);
    } break;
    case (AST::Expression::Type::Identifier):
    {
      success &= generate_identifier_code(code_generator, scope, func, expression.identifier, location);
    } break;
  }

  return success;
}


bool
generate_statement_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Statement const& statement, MemoryAddress const* result_location)
{
  bool success = true;

  switch (statement.type)
  {
    case (AST::Statement::Type::Assignment):
    {
      assert(result_location == NULL);

      AST::Assignment const& assignment = statement.assignment;
      success &= generate_assignment_code(code_generator, scope, func, assignment);
    } break;
    case (AST::Statement::Type::Expression):
    {
      // TODO: Need a dummy address to output the result of the expression to
      MemoryAddress result;
      if (result_location != NULL)
      {
        result = *result_location;
      }
      success &= generate_expression_code(code_generator, scope, func, statement.expression, result);
    } break;
  }

  return success;
}


bool
generate_body_code(CodeGenerator& code_generator, AST::ScopeInfo const& parent_scope, FunctionInfo& func, String::String debug_name, TypeSystem::ID function_type_id, AST::Body const& body);


bool
store_literal(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, String::String debug_name, AST::Literal const& literal, MemoryAddress& result_location)
{
  bool success = true;

  TypeSystem::ID type_id = literal.inherited_type;
  TypeSystem::Type type = scope.types.types[type_id];

  char const* type_debug;
  switch (type.type)
  {
    case (TypeSystem::Type::BuiltIn::U8):
    {
      result_location = Machine::advance_addr<u8>(func.data_end);

      assert(literal.type == AST::Literal::Type::Number);
      AST::Number const& number = literal.number;

      assert(number.number <= 0xFF);
      u8 data = (u8)number.number;

      Machine::set<u8>(*code_generator.machine, result_location, data);

      type_debug = "u8";
    } break;
    case (TypeSystem::Type::BuiltIn::U16):
    {
      result_location = Machine::advance_addr<u16>(func.data_end);

      assert(literal.type == AST::Literal::Type::Number);
      AST::Number const& number = literal.number;

      assert(number.number <= 0xFFFF);
      u16 data = (u16)number.number;

      Machine::set<u16>(*code_generator.machine, result_location, data);

      type_debug = "u16";
    } break;
    case (TypeSystem::Type::BuiltIn::Func):
    {
      assert(literal.type == AST::Literal::Type::Function);
      AST::Function const& function = literal.function;

      FunctionInfo child_func = {};
      child_func.data_start = func.data_end;
      child_func.data_end = child_func.data_start;
      child_func.identifiers_map += func.identifiers_map;

      success &= generate_body_code(code_generator, scope, child_func, debug_name, function.type, *function.body);

      func.data_end = child_func.code_end;
      result_location = child_func.code_start;

      Array::free_array(child_func.identifiers_map);

      type_debug = "func";
    } break;
  }

  // printf("  Storing literal %s at 0x%.4x\n", type_debug, result_location);

  return success;
}


bool
store_literals(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Expression const& expression)
{
  bool success = true;

  switch (expression.type)
  {
    case (AST::Expression::Type::Literal):
    {
      MemoryAddress location;  // TODO: Probably want to do something with this
      success &= store_literal(code_generator, scope, func, "expression literal", expression.literal, location);
    } break;
    case (AST::Expression::Type::FunctionCall):
    {
      AST::FunctionCall const& function_call = expression.function_call;
      AST::Expressions const& expressions = function_call.expressions;
      for (u32 expression_index = 0;
           expression_index < expressions.n;
           ++expression_index)
      {
        AST::Expression const& expression = expressions.expressions[expression_index];
        success &= store_literals(code_generator, scope, func, expression);

        if (!success)
        {
          break;
        }
      }
    } break;
    case (AST::Expression::Type::Identifier):
    {
      // Nothing to do?
    } break;
  }

  return success;
}


bool
store_literals(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Assignment const& assignment)
{
  bool success = true;

  AST::Expression const& expression = assignment.expression;
  AST::Declaration const& declaration = assignment.declaration;

  MemoryAddress decl_location;
  if (expression.type == AST::Expression::Type::Literal)
  {
    // If the assignment is to a literal we re-use the literal's location as the declaration location
    String::String label = Strings::get(*code_generator.strings, declaration.identifier.label);
    success &= store_literal(code_generator, scope, func, label, expression.literal, decl_location);
  }
  else
  {
    // Otherwise store all the literals in the expression as normal, then allocate separately for the declaration
    success &= store_literals(code_generator, scope, func, expression);

    TypeSystem::Type const& type = scope.types.types[declaration.type];
    switch (type.type)
    {
      case (TypeSystem::Type::BuiltIn::U8):
      {
        decl_location = Machine::advance_addr<u8>(func.data_end);
      } break;
      case (TypeSystem::Type::BuiltIn::U16):
      {
        decl_location = Machine::advance_addr<u16>(func.data_end);
      } break;
      case (TypeSystem::Type::BuiltIn::Func):
      {
        // Currently we enforce all function declarations to be to a literal.
        //
        // This is because we are storing the function definition at the
        // declaration site.  If we stored the function elsewhere then the
        // function declaration could just be a MemoryAddress, and this
        // limitation would not be needed.
        //
        // Alternatively, the function declarations could be different sizes
        // depending on whether they are defined with a literal or not, but
        // this would might get unnecessarily complex.
        assert(0);
      } break;
    }
  }

  // Save the identifier -> MemoryAddress mapping in this function scope
  IdentifierMapping identifier = {};
  identifier.id = declaration.identifier.identifier;
  identifier.address = decl_location;
  identifier.debug_name = declaration.identifier.label;
  Array::add(func.identifiers_map, identifier);

  return success;
}


MemoryAddress
reserve_type(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, TypeSystem::ID type_id)
{
  MemoryAddress result;

  TypeSystem::Type type = scope.types.types[type_id];

  switch (type.type)
  {
    case (TypeSystem::Type::BuiltIn::U8):
    {
      result = Machine::advance_addr<u8>(func.data_end);
    } break;
    case (TypeSystem::Type::BuiltIn::U16):
    {
      result = Machine::advance_addr<u16>(func.data_end);
    } break;
    case (TypeSystem::Type::BuiltIn::Func):
    {
      // TODO: We do not currently support passing functions in the code-gen
      assert(0);
    } break;
  }

  return result;
}


// Store all the literals in the scope in the data section of the FunctionInfo
// Declarations re-use the literal values as their storage location, therefore
// with this implementation we cannot share literal values with multiple uses
bool
generate_body_code(CodeGenerator& code_generator, AST::ScopeInfo const& parent_scope, FunctionInfo& func, String::String debug_name, TypeSystem::ID function_type_id, AST::Body const& body)
{
  bool success = true;

  // Reserve return address and value space
  if (function_type_id != TypeSystem::InvalidID)
  {
    func.return_address = Machine::advance_addr<MemoryAddress>(func.data_end);

    TypeSystem::Type const& function_type = parent_scope.types.types[function_type_id];
    assert(function_type.type == TypeSystem::Type::BuiltIn::Func);
    func.return_value = reserve_type(code_generator, parent_scope, func, function_type.function.return_type);
  }

  // Reserve function parameter space

  AST::Declarations const& arg_declarations = body.arg_declarations;
  for (u32 arg_index = 0;
       arg_index < arg_declarations.n;
       ++arg_index)
  {
    AST::Declaration const& arg_declaration = arg_declarations.declarations[arg_index];

    // TODO: We do not currently support passing functions in the code-gen
    assert(parent_scope.types.types[arg_declaration.type].type != TypeSystem::Type::BuiltIn::Func);
    MemoryAddress arg_location = reserve_type(code_generator, parent_scope, func, arg_declaration.type);

    // Store the arg-name -> reserved-location mapping
    IdentifierMapping identifier = {};
    identifier.id = arg_declaration.identifier.identifier;
    identifier.address = arg_location;
    identifier.debug_name = arg_declaration.identifier.label;
    Array::add(func.identifiers_map, identifier);
  }

  // Reserve and store literal values

  for (u32 statement_index = 0;
       statement_index < body.n;
       ++statement_index)
  {
    AST::Statement const& statement = body.statements[statement_index];

    switch (statement.type)
    {
      case (AST::Statement::Type::Assignment):
      {
        success &= store_literals(code_generator, body.scope, func, statement.assignment);
      } break;
      case (AST::Statement::Type::Expression):
      {
        success &= store_literals(code_generator, body.scope, func, statement.expression);
      } break;
    }
  }

  // Write code

  func.code_start = func.data_end;
  func.code_end = func.code_start;

  for (u32 statement_index = 0;
       statement_index < body.n;
       ++statement_index)
  {
    MemoryAddress const* result_location = NULL;
    if (statement_index == (body.n-1))
    {
      result_location = &func.return_value;
    }
    success &= generate_statement_code(code_generator, body.scope, func, body.statements[statement_index], result_location);
  }

  // Return to caller code
  if (function_type_id != TypeSystem::InvalidID)
  {
    Basolls::push_instruction<Code::JUMP>(*code_generator.machine, func.code_end, {
      .addr = func.return_address
    });
  }

  printf("%.*s {\n", print_s(debug_name));
  printf("  data start: 0x%.4x\n", func.data_start);
  printf("  data end: 0x%.4x\n", func.data_end);
  printf("  code start: 0x%.4x\n", func.code_start);
  printf("  code end: 0x%.4x\n", func.code_end);

  printf("  return address: 0x%.4x\n", func.return_address);
  printf("  return value: 0x%.4x\n", func.return_value);

  printf("  identifiers (%u):\n", func.identifiers_map.n_elements);
  for (IdentifierMapping const& identifier : func.identifiers_map)
  {
    printf("    %u : \"%.*s\" : 0x%.4x\n",
           identifier.id,
           print_s(Strings::get(*code_generator.strings, identifier.debug_name)),
           identifier.address);
  }
  printf("}\n");

  if (func.code_end > Machine::Reserved::UserEnd)
  {
    printf("Error: Overran user memory.  Out of space\n");
    success = false;
  }

  return success;
}


bool
generate_code(Machine::Machine* machine, MemoryAddress data_start, AST::AST const& ast, MemoryAddress& code_start_result)
{
  bool success = true;

  CodeGenerator code_generator = {};
  code_generator.machine = machine;
  code_generator.strings = ast.strings;

  FunctionInfo global_func = {};
  global_func.data_start = data_start;
  global_func.data_end = data_start;

  AST::ScopeInfo dummy_scope = {};

  success &= generate_body_code(code_generator, dummy_scope, global_func, "global_scope", TypeSystem::InvalidID, ast.program.body);

  Array::free_array(global_func.identifiers_map);

  code_start_result = global_func.code_start;

  return success;
}

} // namespace CodeGenerator

} // namespace Compolls