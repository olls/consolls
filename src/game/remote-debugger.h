#pragma once

#include "loader/options.h"

#include "machine/machine.h"


namespace RemoteDebugger
{

void
init(Options::Args const& args);


void
destroy(Options::Args const& args);


void
advance(Options::Args const& args, Machine::Machine& machine);

}  // namespace RemoteDebugger
