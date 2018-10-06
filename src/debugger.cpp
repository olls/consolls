#include "debugger.h"

#include "disassembler.h"


namespace Debugger
{

using Machine::MemoryAddress;


void
advance(Machine::Machine& machine)
{
  MemoryAddress ni = Machine::get<MemoryAddress>(machine, Machine::Reserved::NI);
  Disassembler::disassemble_instruction(machine, ni);
}

} // namespace Debugger