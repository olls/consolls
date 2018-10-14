#pragma once


#include "machine.h"
#include "options.h"


namespace Debugger
{

void
init(Options::Args const & args);


void
destroy(Options::Args const & args);


void
advance(Options::Args const & args, Machine::Machine& machine, bool blit);

} // namespace Debugger