#pragma once

#include "machine.h"

namespace Debugger
{

void
init();


void
destroy();


void
advance(Machine::Machine& machine, bool);


} // namespace Debugger