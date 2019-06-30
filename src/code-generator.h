#pragma once

#include "identifiers.h"
#include "ast.h"
#include "instructions.h"
#include "machine.h"
#include "strings.h"
#include "map.h"
#include "array.h"
#include "types.h"


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


struct FunctionInfo
{
  Array::Array<MemoryAddress> parameter_offsets;

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


struct FunctionScope
{
  FunctionInfo info;

  IdentifiersMap identifiers_map;
  LiteralsMap literals_map;
  FunctionsMap functions_map;
};


bool
generate_code(Machine::Machine* machine, Machine::MemoryAddress data_start, AST::AST const & ast, Machine::MemoryAddress& code_start_result);

} // namespace CodeGenerator

} // namespace Compolls