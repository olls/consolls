#include "code-generator.h"

#include "built-in-code.h"
#include "disassembler.h"
#include "basolls.h"


namespace Compolls
{

namespace CodeGenerator
{

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


// TODO: Could use a Lifo stack of the FunctionScope's for each body-scope,
//       pushing/popping scopes on each entry/exit of generate_body_code.
//       This pattern would allow us to see the entire scope above us without
//       duplicating the arrays at each level.  This could be particularly
//       useful for shadowing variables in the AST.


bool
generate_expression_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionScope& func, AST::Expression const& expression, MemoryAddress const* location);


bool
generate_assignment_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionScope& func, AST::Assignment const& assignment)
{
  bool success = true;

  // Literals are handled in generate_body_code
  if (assignment.expression.type != AST::Expression::Type::Literal)
  {
    // Retrieve the address of this identifier's declaration

    Identifiers::ID const& identifier_id = assignment.declaration.identifier.identifier;
    IdentifierMapping const* identifier_mapping = Map::find(func.identifiers_map, identifier_id);
    assert(identifier_mapping != NULL);  // The identifier_mapping should be created in generate_body_code

    MemoryAddress declaration_addr = identifier_mapping->address;

    // Store the result of the expression at declaration_addr
    success &= generate_expression_code(code_generator, scope, func, assignment.expression, &declaration_addr);
  }

  return success;
}

bool
generate_function_call_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionScope& func, AST::FunctionCall const& function_call, MemoryAddress const* location)
{
  bool success = true;

  FunctionInfo* const info_ptr = Map::find(func.functions_map, function_call.identifier.identifier);
  assert(info_ptr);
  FunctionInfo const info = *info_ptr;

  assert(info.parameter_offsets.n_elements == function_call.expressions.n);

  for (u32 expression_index = 0;
       expression_index < function_call.expressions.n;
       ++expression_index)
  {
    AST::Expression const& expression = function_call.expressions.expressions[expression_index];

    MemoryAddress expression_result = info.parameter_offsets[expression_index];

    success &= generate_expression_code(code_generator, scope, func, expression, &expression_result);

    if (!success)
    {
       break;
    }
  }

  if (success)
  {
    IdentifierMapping* mapping = Map::find(func.identifiers_map, function_call.identifier.identifier);
    assert(mapping != NULL);
    MemoryAddress function_addr = mapping->address;
    assert(function_addr == info.code_start);

    Basolls::push_instruction<Code::SET_VW>(*code_generator.machine, func.info.code_end, {
      .addr = info.return_address,
      .value = 0
    });
    // Store the address of the value parameter of the SET_VW above
    MemoryAddress return_location_address = func.info.code_end - sizeof(MemoryAddress);

    Basolls::push_instruction<Code::JUMP_V>(*code_generator.machine, func.info.code_end, {function_addr});

    // Function returns to here, store this address in the value parameter of SET_VW
    MemoryAddress return_location = func.info.code_end;
    Machine::set<MemoryAddress>(*code_generator.machine, return_location_address, return_location);

    if (location != NULL)
    {
      // Copy the function return value into our location
      switch (info.return_value_bytes)
      {
        case (1):
        {
          Basolls::push_instruction<Code::COPY>(*code_generator.machine, func.info.code_end, {
            .from = info.return_value,
            .to = *location
          });
        } break;
        case (2):
        {
          Basolls::push_instruction<Code::COPY_W>(*code_generator.machine, func.info.code_end, {
            .from = info.return_value,
            .to = *location
          });
        } break;
        default:
        {
          assert(!"Unsupported return value size");
        }
      }
    }
  }

  return success;
}


bool
generate_identifier_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionScope& func, AST::Identifier const& identifier, MemoryAddress const* location)
{
  bool success = true;

  // Retrieve the value in the identifier and store in location

  IdentifierMapping const* identifier_mapping = Map::find(func.identifiers_map, identifier.identifier);
  assert(identifier_mapping != NULL);

  MemoryAddress identifier_addr = identifier_mapping->address;

  TypeSystem::ID type_id = scope.identifiers[identifier.identifier].type;
  TypeSystem::Type const& type = scope.types.types[type_id];

  if (location != NULL)
  {
    switch (type.type)
    {
      case (TypeSystem::Type::BuiltIn::U8):
      {
        Basolls::push_instruction<Code::COPY>(*code_generator.machine, func.info.code_end, {
          .from = identifier_addr,
          .to = *location
        });
      } break;
      case (TypeSystem::Type::BuiltIn::U16):
      {
        Basolls::push_instruction<Code::COPY_W>(*code_generator.machine, func.info.code_end, {
          .from = identifier_addr,
          .to = *location
        });
      } break;
      case (TypeSystem::Type::BuiltIn::Func):
      {
        assert(false);  // Not yet supported
      } break;
    }
  }

  return success;
}


bool
generate_literal_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionScope& func, AST::Literal const& literal, MemoryAddress const* location)
{
  bool success = true;

  // Literals are stored in the data section in generate_body_code.
  // The stored literals should be given a id.
  // Identifiers should then just point at the literals' ids.
  //
  // Then we can retrieve the literal address from it's id.

  MemoryAddress *literal_location_ptr = Map::find(func.literals_map, &literal);
  assert(literal_location_ptr != NULL);
  MemoryAddress literal_location = *literal_location_ptr;

  TypeSystem::Type const& type = scope.types.types[literal.inherited_type];

  if (location != NULL)
  {
    // Copy literal value to location
    switch (type.type)
    {
      case (TypeSystem::Type::BuiltIn::U8):
      {
        Basolls::push_instruction<Code::COPY>(*code_generator.machine, func.info.code_end, {
          .from = literal_location,
          .to = *location
        });
      } break;
      case (TypeSystem::Type::BuiltIn::U16):
      {
        Basolls::push_instruction<Code::COPY_W>(*code_generator.machine, func.info.code_end, {
          .from = literal_location,
          .to = *location
        });
      } break;
      case (TypeSystem::Type::BuiltIn::Func):
      {
        // TODO: Not sure if this will actually work
        Basolls::push_instruction<Code::COPY_W>(*code_generator.machine, func.info.code_end, {
          .from = literal_location,
          .to = *location
        });
      } break;
    }
  }

  return success;
}


bool
generate_expression_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionScope& func, AST::Expression const& expression, MemoryAddress const* location)
{
  bool success = true;

  // TODO: Take location as a ptr, so if we don't need the expression result, we don't need to bother with a dummy address.

  switch (expression.type)
  {
    case (AST::Expression::Type::Literal):
    {
      success &= generate_literal_code(code_generator, scope, func, expression.literal, location);
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
generate_statement_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionScope& func, AST::Statement const& statement, MemoryAddress const* result_location)
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
      success &= generate_expression_code(code_generator, scope, func, statement.expression, result_location);
    } break;
  }

  return success;
}


bool
generate_body_code(CodeGenerator& code_generator, AST::ScopeInfo const& parent_scope, FunctionScope& func, String::String debug_name, TypeSystem::ID function_type_id, AST::Body const& body);


bool
store_literal(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionScope& func, String::String debug_name, AST::Literal const& literal, MemoryAddress& result_location, FunctionInfo* result_func_info)
{
  bool success = true;

  TypeSystem::ID type_id = literal.inherited_type;
  TypeSystem::Type type = scope.types.types[type_id];

  char const* type_debug;
  String::String value_debug;
  switch (type.type)
  {
    case (TypeSystem::Type::BuiltIn::U8):
    {
      result_location = Machine::advance_addr<u8>(func.info.data_end);

      assert(literal.type == AST::Literal::Type::Number);
      AST::Number const& number = literal.number;

      assert(number.number <= 0xFF);
      u8 data = (u8)number.number;

      Machine::set<u8>(*code_generator.machine, result_location, data);

      type_debug = "u8";
      value_debug = String::string_f("%u", data);
    } break;
    case (TypeSystem::Type::BuiltIn::U16):
    {
      result_location = Machine::advance_addr<u16>(func.info.data_end);

      assert(literal.type == AST::Literal::Type::Number);
      AST::Number const& number = literal.number;

      assert(number.number <= 0xFFFF);
      u16 data = (u16)number.number;

      Machine::set<u16>(*code_generator.machine, result_location, data);

      type_debug = "u16";
      value_debug = String::string_f("%u", data);
    } break;
    case (TypeSystem::Type::BuiltIn::Func):
    {
      assert(literal.type == AST::Literal::Type::Function);
      AST::Function const& function = literal.function;

      FunctionScope child_func = {};
      child_func.info.data_start = func.info.data_end;
      child_func.info.data_end = child_func.info.data_start;
      child_func.identifiers_map += func.identifiers_map;
      child_func.literals_map += func.literals_map;
      child_func.functions_map += func.functions_map;

      success &= generate_body_code(code_generator, scope, child_func, debug_name, function.type, *function.body);

      func.info.data_end = child_func.info.code_end;
      result_location = child_func.info.code_start;

      Array::free_array(child_func.identifiers_map);
      Array::free_array(child_func.literals_map);
      Array::free_array(child_func.functions_map);

      // Pass the function info back out
      // TODO: If thjs assert fires, it means we are trying to generate a
      //       function literal in a place where the calling function does not
      //       expect/yet-support it.
      assert(result_func_info != NULL);
      *result_func_info = child_func.info;

      type_debug = "func";
      value_debug = "function-DEBUG-TODO";
    } break;
  }

  printf("Storing literal %p, %s(%.*s) at %#.4x\n", &literal, type_debug, print_s(value_debug), result_location);
  Array::add(func.literals_map, {&literal, result_location});

  return success;
}


bool
store_literals(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionScope& func, AST::Expression const& expression)
{
  bool success = true;

  switch (expression.type)
  {
    case (AST::Expression::Type::Literal):
    {
      MemoryAddress location;  // Not used, literal address is stored in func.literals_map
      success &= store_literal(code_generator, scope, func, "expression literal", expression.literal, location, NULL);
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
      // Nothing to do
    } break;
  }

  return success;
}


bool
store_literals(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionScope& func, AST::Assignment const& assignment)
{
  bool success = true;

  AST::Expression const& expression = assignment.expression;
  AST::Declaration const& declaration = assignment.declaration;

  MemoryAddress decl_location;
  if (expression.type == AST::Expression::Type::Literal)
  {
    // If the assignment is to a literal we re-use the literals location as the declaration location
    // TODO: This could be a premature optimisation, esp if we add an opt pass after code gen.

    String::String debug_label = Strings::get(*code_generator.strings, declaration.identifier.label);

    // If this literal is a function type, pass in a FunctionInfo so we can
    // store that along with it's identifier.
    FunctionInfo function_info = {};
    FunctionInfo* function_info_ptr = NULL;
    if (expression.literal.type == AST::Literal::Type::Function)
    {
      function_info_ptr = &function_info;
    }

    success &= store_literal(code_generator, scope, func, debug_label, expression.literal, decl_location, function_info_ptr);

    if (expression.literal.type == AST::Literal::Type::Function)
    {
      // Save the FunctionInfo in our FunctionsMap
      Array::add(func.functions_map, {declaration.identifier.identifier, function_info});
    }
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
        decl_location = Machine::advance_addr<u8>(func.info.data_end);
      } break;
      case (TypeSystem::Type::BuiltIn::U16):
      {
        decl_location = Machine::advance_addr<u16>(func.info.data_end);
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
  identifier.address = decl_location;
  identifier.debug_name = declaration.identifier.label;
  Array::add(func.identifiers_map, {declaration.identifier.identifier, identifier});

  return success;
}


MemoryAddress
reserve_type(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionScope& func, TypeSystem::ID type_id, u32* result_bytes = NULL)
{
  MemoryAddress result;
  u32 bytes = 0;

  TypeSystem::Type type = scope.types.types[type_id];

  switch (type.type)
  {
    case (TypeSystem::Type::BuiltIn::U8):
    {
      result = Machine::advance_addr<u8>(func.info.data_end);
      bytes = 1;
    } break;
    case (TypeSystem::Type::BuiltIn::U16):
    {
      result = Machine::advance_addr<u16>(func.info.data_end);
      bytes = 2;
    } break;
    case (TypeSystem::Type::BuiltIn::Func):
    {
      // TODO: We do not currently support passing functions in the code-gen
      assert(0);
    } break;
  }

  if (result_bytes != NULL)
  {
    *result_bytes = bytes;
  }

  return result;
}


// Store all the literals in the scope in the data section of the FunctionInfo
// Declarations re-use the literal values as their storage location, therefore
// with this implementation we cannot share literal values with multiple uses
bool
generate_body_code(CodeGenerator& code_generator, AST::ScopeInfo const& parent_scope, FunctionScope& func, String::String debug_name, TypeSystem::ID function_type_id, AST::Body const& body)
{
  bool success = true;

  // Reserve return address and value space
  if (function_type_id != TypeSystem::InvalidID)
  {
    func.info.return_address = Machine::advance_addr<MemoryAddress>(func.info.data_end);

    TypeSystem::Type const& function_type = parent_scope.types.types[function_type_id];
    assert(function_type.type == TypeSystem::Type::BuiltIn::Func);
    func.info.return_value = reserve_type(code_generator, parent_scope, func, function_type.function.return_type, &func.info.return_value_bytes);
  }

  // Reserve function parameter space

  AST::Declarations const& arg_declarations = body.arg_declarations;
  for (u32 arg_index = 0;
       arg_index < arg_declarations.n;
       ++arg_index)
  {
    AST::Declaration const& arg_declaration = arg_declarations.declarations[arg_index];

    // Store the offset of each parameter from data_start, used for generating calling code.
    Array::add(func.info.parameter_offsets, func.info.data_end);

    // TODO: We do not currently support passing functions in the code-gen
    assert(parent_scope.types.types[arg_declaration.type].type != TypeSystem::Type::BuiltIn::Func);
    MemoryAddress arg_location = reserve_type(code_generator, parent_scope, func, arg_declaration.type);

    // Store the arg-name -> reserved-location mapping
    IdentifierMapping identifier = {};
    identifier.address = arg_location;
    identifier.debug_name = arg_declaration.identifier.label;
    Array::add(func.identifiers_map, {arg_declaration.identifier.identifier, identifier});
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

  func.info.code_start = func.info.data_end;
  func.info.code_end = func.info.code_start;

  for (u32 statement_index = 0;
       statement_index < body.n;
       ++statement_index)
  {
    // If this is the last statement in the body, pass the return value address ptr in.
    // Otherwise pass NULL ptr.
    // No return type implies this body does not return, e.g: top level body.
    MemoryAddress const* result_location = NULL;
    if (statement_index == (body.n-1) && function_type_id != TypeSystem::InvalidID)
    {
      result_location = &func.info.return_value;
    }

    success &= generate_statement_code(code_generator, body.scope, func, body.statements[statement_index], result_location);
  }

  // Return to caller code
  if (function_type_id != TypeSystem::InvalidID)
  {
    Basolls::push_instruction<Code::JUMP>(*code_generator.machine, func.info.code_end, {
      .addr = func.info.return_address
    });
  }

  printf("\n%.*s {\n", print_s(debug_name));
  printf("  data start: %#.4x\n", func.info.data_start);
  printf("  data end: %#.4x\n", func.info.data_end);
  printf("  code start: %#.4x\n", func.info.code_start);
  printf("  code end: %#.4x\n", func.info.code_end);

  printf("  return address: %#.4x\n", func.info.return_address);
  printf("  return value: %#.4x\n", func.info.return_value);

  printf("  identifiers (%u):\n", func.identifiers_map.n_elements);
  for (IdentifiersMap::Type const& pair : func.identifiers_map)
  {
    Identifiers::ID id = pair.key;
    IdentifierMapping const& identifier = pair.value;

    printf("    %u : \"%.*s\" : %#.4x\n",
           id,
           print_s(Strings::get(*code_generator.strings, identifier.debug_name)),
           identifier.address);
  }
  Disassembler::disassemble(*code_generator.machine, func.info.code_start, func.info.code_end, "  ");

  printf("}\n\n");

  if (func.info.code_end > Machine::Reserved::UserEnd)
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

  FunctionScope global_func = {};
  global_func.info.data_start = data_start;
  global_func.info.data_end = data_start;

  BuiltInCode::generate_code(code_generator, ast.built_in_identifiers, global_func);

  AST::ScopeInfo dummy_scope = {};

  success &= generate_body_code(code_generator, dummy_scope, global_func, "global_scope", TypeSystem::InvalidID, ast.program.body);

  Array::free_array(global_func.identifiers_map);
  Array::free_array(global_func.literals_map);

  code_start_result = global_func.info.code_start;

  return success;
}

} // namespace CodeGenerator

} // namespace Compolls