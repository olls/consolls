#pragma once

#include "machine/machine.h"

#include "utils/types.h"


namespace Socket
{

bool
init(bool receive);


bool
advance(bool receive, Machine::Machine& machine);


void
destroy();

} // namespace Socket
