#pragma once

#include "machine.h"


namespace Basolls
{

Machine::MemoryAddress
compiler_program(Machine::Machine& machine, Machine::MemoryAddress& addr);


Machine::MemoryAddress
demo_program(Machine::Machine& machine, Machine::MemoryAddress& addr);

} // namespace Basolls