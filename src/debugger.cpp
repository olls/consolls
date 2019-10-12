#include "debugger.h"

#include "disassembler.h"
#include "socket.h"


namespace Debugger
{

using Machine::MemoryAddress;


void
init(Options::Args const & args)
{
  if (args.debugger)
  {
    Socket::init();
  }
}


void
destroy(Options::Args const & args)
{
  if (args.debugger)
  {
    Socket::destroy();
  }
}


void
advance(Options::Args const & args, Machine::Machine& machine)
{
  if (args.debugger)
  {
    MemoryAddress ni = Machine::get<MemoryAddress>(machine, Machine::Reserved::NI);
    Disassembler::disassemble_instruction(machine, ni);

    Socket::advance(args.client, machine);
  }
}

} // namespace Debugger