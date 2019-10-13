#pragma once

#include "machine.h"

#include "utils/types.h"


namespace Disassembler
{

Machine::MemoryAddress
disassemble_instruction(Machine::Machine const & machine, Machine::MemoryAddress addr);


void
disassemble(Machine::Machine const & machine, Machine::MemoryAddress from, Machine::MemoryAddress to, char const* prefix = NULL);

} // namespace Disassembler