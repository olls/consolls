#include "code-generator.h"

#include "instructions.h"
#include "basolls.h"
#include "array.h"


namespace Compolls
{

namespace CodeGenerator
{

using Machine::MemoryAddress;
using namespace Instructions;


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


struct Identifier
{
  Identifiers::ID id;
  MemoryAddress address;
};


struct FunctionInfo
{
  MemoryAddress data_start;
  MemoryAddress data_end;

  MemoryAddress code_start;
  MemoryAddress code_end;

  Array::Array<Identifier> identifiers;
};


Identifier *
find(Array::Array<Identifier> const& identifiers, Identifiers::ID id)
{
  Identifier* result = NULL;

  Array::FindFirstFunc<Identifier> find_func = [](Identifier const& i, void* user){ return i.id == *(Identifiers::ID*)user; };
  s32 index = Array::find_first(identifiers, find_func, &id);
  if (index != -1)
  {
    result = Array::get(identifiers, index);
  }

  return result;
}


// TODO: Could use a Lifo stack of the FunctionInfo's for each body-scope,
//       pushing/popping scopes on each entry/exit of generate_body_code.
//       This pattern would allow us to see the entire scope above us without
//       duplicating the arrays at each level.  This could be particularly
//       useful for shadowing variables in the AST.


bool
generate_statement_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Statement const& statement);

bool
generate_body_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Body const& body);

bool
generate_assignment_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Assignment const& assignment);

bool
generate_expression_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Expression const& expression, MemoryAddress location);

bool
generate_function_call_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::FunctionCall const& function_call, MemoryAddress location);

bool
generate_identifier_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Identifier const& identifier, MemoryAddress location);


bool
generate_body_code(CodeGenerator& code_generator, AST::ScopeInfo const& parent_scope, FunctionInfo& parent_func, String::String debug_name, AST::Body const& body)
{
  bool success = true;

  AST::ScopeInfo const& scope = body.scope;

  // TODO: Function parameters
  //       - reserve data space for parameters
  //       - presumably calling code would handle filling in the parameter data

  FunctionInfo func = {};
  func.data_start = parent_func.code_end;
  func.data_end = func.data_start;
  func.identifiers += parent_func.identifiers;

  // Reserve data space for all declarations
  //
  // All literal values are stored at this point so we know how much space is
  // used, and thus where to start generating the code.
  // Functions are the only variable size literals, but we handle all literals
  // here for consistency.
  for (u32 statement_index = 0;
       statement_index < body.n;
       ++statement_index)
  {
    AST::Statement const& statement = body.statements[statement_index];
    if (statement.type == AST::Statement::Type::Assignment)
    {
      AST::Assignment const& assignment = statement.assignment;
      AST::Declaration const& declaration = assignment.declaration;
      TypeSystem::Type const& type = scope.types.types[declaration.type];

      MemoryAddress decl_location;

      switch (type.type)
      {
        case (TypeSystem::Type::BuiltIn::U8):
        {
          decl_location = Machine::advance_addr<u8>(func.data_end);

          // Store literal value
          if (assignment.expression.type == AST::Expression::Type::Literal)
          {
            AST::Literal const& literal = assignment.expression.literal;
            assert(literal.type == AST::Literal::Type::Number);
            AST::Number const& number = literal.number;

            assert(number.number <= 0xFF);
            u8 data = (u8)number.number;

            Machine::set<u8>(*code_generator.machine, decl_location, data);
          }
        } break;
        case (TypeSystem::Type::BuiltIn::U16):
        {
          decl_location = Machine::advance_addr<u16>(func.data_end);

          // Store literal value
          if (assignment.expression.type == AST::Expression::Type::Literal)
          {
            AST::Literal const& literal = assignment.expression.literal;
            assert(literal.type == AST::Literal::Type::Number);
            AST::Number const& number = literal.number;

            assert(number.number <= 0xFFFF);
            u16 data = (u16)number.number;

            Machine::set<u16>(*code_generator.machine, decl_location, data);
          }
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
          //
          assert(assignment.expression.type == AST::Expression::Type::Literal);

          // Store literal value
          if (assignment.expression.type == AST::Expression::Type::Literal)
          {
            AST::Literal const& literal = assignment.expression.literal;
            assert(literal.type == AST::Literal::Type::Function);
            AST::Function const& function = literal.function;

            String::String label = Strings::get(*code_generator.strings, declaration.identifier.label);

            success &= generate_body_code(code_generator, scope, func, label, *function.body);

            // TODO: Store child-func code-start in decl_location
          }
        } break;
      }

      // Save the identifier -> MemoryAddress mapping in this function scope
      Identifier identifier = {};
      identifier.id = declaration.identifier.identifier;
      identifier.address = decl_location;
      Array::add(func.identifiers, identifier);
    }
  }

  func.code_start = func.data_end;
  func.code_end = func.code_start;

  // Write code
  for (u32 statement_index = 0;
       statement_index < body.n;
       ++statement_index)
  {
    success &= generate_statement_code(code_generator, body.scope, func, body.statements[statement_index]);
  }

  // TODO: Return to caller code

  // TODO: Store func somewhere?
  //       Presumably the parent scope will need to know it's code_start so it can call it

  printf("%.*s identifiers (%u):\n", print_s(debug_name), func.identifiers.n_elements);
  for (Identifier const& identifier : func.identifiers)
  {
    printf("%u : 0x%.4x\n", identifier.id, identifier.address);
  }

  Array::free_array(func.identifiers);

  return success;
}


bool
generate_statement_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Statement const& statement)
{
  bool success = true;

  switch (statement.type)
  {
    case (AST::Statement::Type::Assignment):
    {
      AST::Assignment const& assignment = statement.assignment;
      success &= generate_assignment_code(code_generator, scope, func, assignment);
    } break;
    case (AST::Statement::Type::Expression):
    {
      MemoryAddress result;  // Not used.
      success &= generate_expression_code(code_generator, scope, func, statement.expression, result);
    } break;
  }

  return success;
}


bool
generate_assignment_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Assignment const& assignment)
{
  bool success = true;

  // Literals are handled in generate_body_code
  if (assignment.expression.type != AST::Expression::Type::Literal)
  {
    // Retrieve the address of this identifier's declaration

    Identifiers::ID const& identifier_id = assignment.declaration.identifier.identifier;
    Identifier const* identifier = find(func.identifiers, identifier_id);
    assert(identifier != NULL);  // The identifier should be created in generate_body_code

    MemoryAddress declaration_addr = identifier->address;

    // Store the result of the expression at declaration_addr
    success &= generate_expression_code(code_generator, scope, func, assignment.expression, declaration_addr);
  }

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
      // Literals in assignments are stored in the assignment in generate_body_code
      // but literals as parts of expressions are not.
      // Instead of storing all declarations in the data section in generate_body_code,
      // we should be storing all literals.
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
generate_function_call_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::FunctionCall const& function_call, MemoryAddress location)
{
  bool success = true;

  MemoryAddress expression_results[function_call.expressions.n];
  for (u32 expression_index = 0;
       expression_index < function_call.expressions.n;
       ++expression_index)
  {
    success &= generate_expression_code(code_generator, scope, func, function_call.expressions.expressions[expression_index], expression_results[expression_index]);
  }

  // Call function_call.identifier
  // put function result in location

  return success;
}


bool
generate_identifier_code(CodeGenerator& code_generator, AST::ScopeInfo const& scope, FunctionInfo& func, AST::Identifier const& identifier, MemoryAddress location)
{
  bool success = true;

  // Retrieve the value in the identifier and store in location

  Identifier const* stored_identifier = find(func.identifiers, identifier.identifier);
  assert(stored_identifier != NULL);

  MemoryAddress identifier_addr = stored_identifier->address;

  Basolls::push_instruction<Code::COPY>(*code_generator.machine, func.code_end, {
    .from = identifier_addr,
    .to = location
  });

  return success;
}


bool
generate_code(Machine::Machine* machine, MemoryAddress data_start, AST::AST const& ast)
{
  bool success = true;

  CodeGenerator code_generator = {};
  code_generator.machine = machine;
  code_generator.strings = ast.strings;

  AST::ScopeInfo scope = {}; // Dummy scope until we get to the first AST::Body

  FunctionInfo global_func = {};
  global_func.data_start = data_start;
  global_func.data_end = data_start;

  success &= generate_body_code(code_generator, scope, global_func, "global scope", ast.program.body);

  Array::free_array(global_func.identifiers);

  return success;
}

} // namespace CodeGenerator

} // namespace Compolls