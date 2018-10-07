#include "debugger.h"

#include "debug.h"
#include "disassembler.h"
#include "socket.h"


namespace Debugger
{

using Machine::MemoryAddress;


void
init(Options::Args args)
{
  if (args.debugger)
  {
    Socket::init(args.client);
  }
}


void
destroy(Options::Args args)
{
  if (args.debugger)
  {
    Socket::destroy();
  }
}


void
advance(Options::Args args, Machine::Machine& machine, bool blit)
{
  if (args.debugger)
  {
    MemoryAddress ni = Machine::get<MemoryAddress>(machine, Machine::Reserved::NI);
    Disassembler::disassemble_instruction(machine, ni);

    if (blit)
    {
      Machine::set<u8>(machine, Machine::Reserved::Blit, 1);
      Socket::advance(args.client, machine);
      Machine::set<u8>(machine, Machine::Reserved::Blit, 0);
    }
  }
}

} // namespace Debugger