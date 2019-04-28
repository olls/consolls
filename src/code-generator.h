#pragma once

#include "ast.h"
#include "machine.h"
#include "types.h"


namespace Compolls
{

namespace CodeGenerator
{

bool
generate_code(Machine::Machine* machine, Machine::MemoryAddress data_start, AST::AST const & ast, Machine::MemoryAddress& code_start_result);

} // namespace CodeGenerator

} // namespace Compolls