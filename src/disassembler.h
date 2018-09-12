#pragma once

#include "machine.h"

namespace Disassembler
{

void
disassemble(Machine::Machine& machine, Machine::MemoryAddress from, Machine::MemoryAddress to);

} // namespace Disassembler