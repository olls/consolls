#pragma once

#include "machine.h"

namespace Disassembler
{

Machine::MemoryAddress
disassemble_instruction(Machine::Machine& machine, Machine::MemoryAddress addr);


void
disassemble(Machine::Machine& machine, Machine::MemoryAddress from, Machine::MemoryAddress to);

} // namespace Disassembler