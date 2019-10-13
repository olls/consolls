#include "code-generator.h"

#include "built-in-code.h"
#include "machine/disassembler.h"
#include "machine/basolls.h"


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


void
debug_print_func(CodeGenerator const& code_generator, FunctionScope const& func_scope, String::String debug_name)
{
  FunctionInfo const& func_info = func_scope.functions_map[func_scope.function_index].value;

  printf("\n%.*s {\n", print_s(debug_name));
  printf("  data start: %#.4x\n", func_info.data_start);
  printf("  data end: %#.4x\n", func_info.data_end);
  printf("  code start: %#.4x\n", func_info.code_start);
  printf("  code end: %#.4x\n", func_info.code_end);

  printf("  return address: %#.4x\n", func_info.return_address);
  printf("  return value: %#.4x\n", func_info.return_value);

  printf("  parameters:\n");
  for (MemoryAddress parameter : func_info.parameter_addrs)
  {
    printf("    0x%x\n", parameter);
  }

  printf("  identifiers (%u):\n", func_scope.identifiers_map.n_elements);
  for (IdentifiersMap::Type const& pair : func_scope.identifiers_map)
  {
    Identifiers::ID id = pair.key;
    IdentifierMapping const& identifier = pair.value;

    printf("    %u : \"%.*s\" : %#.4x\n",
           id,
           print_s(Strings::get(*code_generator.strings, identifier.debug_name)),
           identifier.address);
  }
  Disassembler::disassemble(*code_generator.machine, func_info.code_start, func_info.code_end, "  ");

  printf("}\n\n");
}


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

  FunctionInfo& func_info = func.functions_map[func.function_index].value;

  FunctionInfo* const call_info = Map::find(func.functions_map, function_call.identifier.identifier);
  assert(call_info);
  assert(call_info->parameter_addrs.n_elements == function_call.expressions.n);

  for (u32 expression_index = 0;
       expression_index < function_call.expressions.n;
       ++expression_index)
  {
    AST::Expression const& expression = function_call.expressions.expressions[expression_index];

    MemoryAddress expression_result = call_info->parameter_addrs[expression_index];

    success &= generate_expression_code(code_generator, scope, func, expression, &expression_result);

    if (!success)
    {
       break;
    }
  }

  if (success)
  {
    // IdentifierMapping* mapping = Map::find(func.identifiers_map, function_call.identifier.identifier);
    // assert(mapping != NULL);
    // MemoryAddress function_addr = mapping->address;
    // assert(function_addr == call_info->code_start);
    MemoryAddress function_addr = call_info->code_start;

    Basolls::push_instruction<Code::SET_VW>(*code_generator.machine, func_info.code_end, {
      .addr = call_info->return_address,
      .value = 0
    });
    // Store the address of the value parameter of the SET_VW above
    MemoryAddress return_location_address = func_info.code_end - sizeof(MemoryAddress);

    Basolls::push_instruction<Code::JUMP_V>(*code_generator.machine, func_info.code_end, {function_addr});

    // Function returns to here, store this address in the value parameter of SET_VW
    MemoryAddress return_location = func_info.code_end;
    Machine::set<MemoryAddress>(*code_generator.machine, return_location_address, return_location);

    if (location != NULL)
    {
      // Copy the function return value into our location
      switch (call_info->return_value_bytes)
      {
        case (1):
        {
          Basolls::push_instruction<Code::COPY>(*code_generator.machine, func_info.code_end, {
            .from = call_info->return_value,
            .to = *location
          });
        } break;
        case (2):
        {
          Basolls::push_instruction<Code::COPY_W>(*code_generator.machine, func_info.code_end, {
            .from = call_info->return_value,
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

  FunctionInfo& func_info = func.functions_map[func.function_index].value;

  if (location != NULL)
  {
    switch (type.type)
    {
      case (TypeSystem::Type::BuiltIn::U8):
      {
        Basolls::push_instruction<Code::COPY>(*code_generator.machine, func_info.code_end, {
          .from = identifier_addr,
          .to = *location
        });
      } break;
      case (TypeSystem::Type::BuiltIn::U16):
      {
        Basolls::push_instruction<Code::COPY_W>(*code_generator.machine, func_info.code_end, {
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

  FunctionInfo& func_info = func.functions_map[func.function_index].value;

  if (location != NULL)
  {
    // Copy literal value to location
    switch (type.type)
    {
      case (TypeSystem::Type::BuiltIn::U8):
      {
        Basolls::push_instruction<Code::COPY>(*code_generator.machine, func_info.code_end, {
          .from = literal_location,
          .to = *location
        });
      } break;
      case (TypeSystem::Type::BuiltIn::U16):
      {
        Basolls::push_instruction<Code::COPY_W>(*code_generator.machine, func_info.code_end, {
          .from = literal_location,
          .to = *location
        });
      } break;
      case (TypeSystem::Type::BuiltIn::Func):
      {
        // TODO: Not sure if this will actually work
        Basolls::push_instruction<Code::COPY_W>(*code_generator.machine, func_info.code_end, {
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
store_literal(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionScope& func, String::String debug_name, AST::Literal const& literal, MemoryAddress& result_location, u32 function_index)
{
  bool success = true;

  TypeSystem::ID type_id = literal.inherited_type;
  TypeSystem::Type type = scope.types.types[type_id];

  FunctionInfo& func_info = func.functions_map[func.function_index].value;

  char const* type_debug;
  String::String value_debug;
  switch (type.type)
  {
    case (TypeSystem::Type::BuiltIn::U8):
    {
      result_location = Machine::advance_addr<u8>(func_info.data_end);

      assert(literal.type == AST::Literal::Type::Number);
      AST::Number const& number = literal.number;

      assert(number.number <= 0xFF);
      u8 data = (u8)number.number;

      Machine::set<u8>(*code_generator.machine, result_location, data);

      type_debug = "u8";
      value_debug = String::format("%u", data);
    } break;
    case (TypeSystem::Type::BuiltIn::U16):
    {
      result_location = Machine::advance_addr<u16>(func_info.data_end);

      assert(literal.type == AST::Literal::Type::Number);
      AST::Number const& number = literal.number;

      assert(number.number <= 0xFFFF);
      u16 data = (u16)number.number;

      Machine::set<u16>(*code_generator.machine, result_location, data);

      type_debug = "u16";
      value_debug = String::format("%u", data);
    } break;
    case (TypeSystem::Type::BuiltIn::Func):
    {
      assert(literal.type == AST::Literal::Type::Function);
      AST::Function const& function = literal.function;

      FunctionScope child_func = {};
      // TODO: We are duplicating the maps into the child scope.
      //       So when we modify them in the child scope, they are not
      //       modified in the parent scope.  This is a problem, because the
      //       parent scope is the scope they are being defined into!
      //       The stack system would solve this much more elegantly...
      child_func.identifiers_map += func.identifiers_map;
      child_func.literals_map += func.literals_map;
      child_func.functions_map += func.functions_map;

      child_func.function_index = function_index;

      FunctionInfo& child_func_info = child_func.functions_map[function_index].value;
      child_func_info.data_start = func_info.data_end;
      child_func_info.data_end = child_func_info.data_start;

      success &= generate_body_code(code_generator, scope, child_func, debug_name, function.type, *function.body);

      result_location = child_func_info.code_start;

      // Re: Above comment; for now, we can hack around it by copying the data
      //     back from the child maps... :s
      for (u32 parent_func_index = 0;
           parent_func_index < func.functions_map.n_elements;
           ++parent_func_index)
      {
        func.functions_map[parent_func_index] = child_func.functions_map[parent_func_index];
      }

      func_info.data_end = child_func_info.code_end;

      Array::free_array(child_func.identifiers_map);
      Array::free_array(child_func.literals_map);
      Array::free_array(child_func.functions_map);

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
      success &= store_literal(code_generator, scope, func, "expression literal", expression.literal, location, 0);
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
    u32 function_index;
    if (expression.literal.type == AST::Literal::Type::Function)
    {
      // Create the entry before we generate the function, so the function is
      // able to call itself.
      function_index = func.functions_map.n_elements;
      FunctionsMap::Type& pair = Array::new_element(func.functions_map);
      pair.key = declaration.identifier.identifier;
      pair.value = {};
    }

    success &= store_literal(code_generator, scope, func, debug_label, expression.literal, decl_location, function_index);
  }
  else
  {
    // Otherwise store all the literals in the expression as normal, then allocate separately for the declaration
    success &= store_literals(code_generator, scope, func, expression);

    FunctionInfo& func_info = func.functions_map[func.function_index].value;

    TypeSystem::Type const& type = scope.types.types[declaration.type];
    switch (type.type)
    {
      case (TypeSystem::Type::BuiltIn::U8):
      {
        decl_location = Machine::advance_addr<u8>(func_info.data_end);
      } break;
      case (TypeSystem::Type::BuiltIn::U16):
      {
        decl_location = Machine::advance_addr<u16>(func_info.data_end);
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
reserve_type(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo* func_info, TypeSystem::ID type_id, u32* result_bytes = NULL)
{
  MemoryAddress result;
  u32 bytes = 0;

  TypeSystem::Type type = scope.types.types[type_id];

  switch (type.type)
  {
    case (TypeSystem::Type::BuiltIn::U8):
    {
      result = Machine::advance_addr<u8>(func_info->data_end);
      bytes = 1;
    } break;
    case (TypeSystem::Type::BuiltIn::U16):
    {
      result = Machine::advance_addr<u16>(func_info->data_end);
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

  FunctionInfo& func_info = func.functions_map[func.function_index].value;

  // Reserve return address and value space
  if (function_type_id != TypeSystem::InvalidID)
  {
    func_info.return_address = Machine::advance_addr<MemoryAddress>(func_info.data_end);

    TypeSystem::Type const& function_type = parent_scope.types.types[function_type_id];
    assert(function_type.type == TypeSystem::Type::BuiltIn::Func);
    func_info.return_value = reserve_type(code_generator, parent_scope, &func_info, function_type.function.return_type, &func_info.return_value_bytes);
  }

  // Reserve function parameter space

  AST::Declarations const& arg_declarations = body.arg_declarations;
  for (u32 arg_index = 0;
       arg_index < arg_declarations.n;
       ++arg_index)
  {
    AST::Declaration const& arg_declaration = arg_declarations.declarations[arg_index];

    // Store the offset of each parameter from data_start, used for generating calling code.
    Array::add(func_info.parameter_addrs, func_info.data_end);

    // TODO: We do not currently support passing functions in the code-gen
    assert(parent_scope.types.types[arg_declaration.type].type != TypeSystem::Type::BuiltIn::Func);
    MemoryAddress arg_location = reserve_type(code_generator, parent_scope, &func_info, arg_declaration.type);

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

  func_info.code_start = func_info.data_end;
  func_info.code_end = func_info.code_start;

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
      result_location = &func_info.return_value;
    }

    success &= generate_statement_code(code_generator, body.scope, func, body.statements[statement_index], result_location);
  }

  // Return to caller code
  if (function_type_id != TypeSystem::InvalidID)
  {
    Basolls::push_instruction<Code::JUMP>(*code_generator.machine, func_info.code_end, {
      .addr = func_info.return_address
    });
  }

  debug_print_func(code_generator, func, debug_name);

  if (func_info.code_end > Machine::Reserved::UserEnd)
  {
    printf("Error: Overran user memory.  Out of space\n");
    success = false;
  }

  return success;
}


bool
generate_code(Machine::Machine* machine, MemoryAddress& addr, AST::AST const& ast, MemoryAddress& code_start_result)
{
  bool success = true;

  CodeGenerator code_generator = {};
  code_generator.machine = machine;
  code_generator.strings = ast.strings;

  FunctionScope global_func = {};
  global_func.function_index = global_func.functions_map.n_elements;

  FunctionsMap::Type& pair = Array::new_element(global_func.functions_map);
  pair.key = -1;
  FunctionInfo& global_func_info = pair.value;

  global_func_info.data_start = addr;
  global_func_info.data_end = addr;

  BuiltInCode::generate_code(code_generator, ast.built_in_identifiers, global_func);

  AST::ScopeInfo dummy_scope = {};

  success &= generate_body_code(code_generator, dummy_scope, global_func, "global_scope", TypeSystem::InvalidID, ast.program.body);

  Array::free_array(global_func.identifiers_map);
  Array::free_array(global_func.literals_map);
  Array::free_array(global_func.functions_map);

  code_start_result = global_func_info.code_start;
  addr = global_func_info.code_end;

  return success;
}

} // namespace CodeGenerator

} // namespace Compolls