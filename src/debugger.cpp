#include "debugger.h"

#include "disassembler.h"
#include "socket.h"


namespace Debugger
{

using Machine::MemoryAddress;


void
init()
{
  Socket::init();
}


void
destroy()
{
  Socket::destroy();
}


void
advance(Machine::Machine& machine, bool blit)
{
  MemoryAddress ni = Machine::get<MemoryAddress>(machine, Machine::Reserved::NI);
  Disassembler::disassemble_instruction(machine, ni);

  if (blit)
  {
    Machine::set<u8>(machine, Machine::Reserved::Blit, 1);
    Socket::advance(machine);
    Machine::set<u8>(machine, Machine::Reserved::Blit, 0);
  }
}

} // namespace Debugger