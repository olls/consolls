#pragma once

#include "ast.h"
#include "machine.h"
#include "strings.h"
#include "types.h"


namespace Compolls
{

namespace CodeGenerator
{

using Machine::MemoryAddress;


struct CodeGenerator
{
  Machine::Machine* machine;
  Strings::Table* strings;
};


bool
generate_code(Machine::Machine* machine, Machine::MemoryAddress data_start, AST::AST const & ast);

} // namespace CodeGenerator

} // namespace Compolls