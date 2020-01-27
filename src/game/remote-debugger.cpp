#include "remote-debugger.h"

#include "socket.h"


namespace RemoteDebugger
{

using Machine::MemoryAddress;


void
init(Options::Args const& args)
{
  if (args.debugger)
  {
    Socket::init(args.client);
  }
}


void
destroy(Options::Args const& args)
{
  if (args.debugger)
  {
    Socket::destroy();
  }
}


void
advance(Options::Args const& args, Machine::Machine& machine)
{
  if (args.debugger)
  {
    Socket::advance(args.client, machine);
  }
}

} // namespace RemoteDebugger
