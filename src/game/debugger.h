#pragma once


#include "machine/machine.h"
#include "loader/options.h"


namespace Debugger
{

void
init(Options::Args const & args);


void
destroy(Options::Args const & args);


void
advance(Options::Args const & args, Machine::Machine& machine);

} // namespace Debugger