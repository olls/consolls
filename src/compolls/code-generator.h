#pragma once

#include "identifiers.h"
#include "ast.h"
#include "machine/instructions.h"
#include "machine/machine.h"
#include "strings.h"
#include "utils/map.h"
#include "utils/array.h"
#include "utils/types.h"


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


// Maps Identifers::IDs to MemoryAddresses of their stored location
struct IdentifierMapping
{
  MemoryAddress address;

  Strings::ID debug_name;
};

using IdentifiersMap = Map::Map<Identifiers::ID, IdentifierMapping>;


// Maps AST::Literal addresses to MemoryAddresses of their stored location
using LiteralsMap = Map::Map<AST::Literal const*, MemoryAddress>;


// Stored function info used for generating calling code
struct FunctionInfo
{
  Array::Array<MemoryAddress> parameter_addrs;

  MemoryAddress data_start;
  MemoryAddress data_end;

  MemoryAddress code_start;
  MemoryAddress code_end;

  MemoryAddress return_address;
  MemoryAddress return_value;
  u32 return_value_bytes;
};

// Mapping used to store the function info for retrieving when generating calling code
using FunctionsMap = Map::Map<Identifiers::ID, FunctionInfo>;


// Local function state, used only within the function's compilation
struct FunctionScope
{
  u32 function_index;

  IdentifiersMap identifiers_map;
  LiteralsMap literals_map;
  FunctionsMap functions_map;
};


bool
generate_code(Machine::Machine* machine, Machine::MemoryAddress& addr, AST::AST const & ast, Machine::MemoryAddress& code_start_result);

} // namespace CodeGenerator

} // namespace Compolls