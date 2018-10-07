#pragma once


#include "machine.h"
#include "options.h"


namespace Debugger
{

void
init(Options::Args args);


void
destroy(Options::Args args);


void
advance(Options::Args args, Machine::Machine& machine, bool blit);


} // namespace Debugger