#pragma once

#include "basolls.h"
#include "string.h"
#include "types.h"


namespace Compolls
{

bool
compile(String::String text, Machine::Machine& machine, Machine::MemoryAddress& addr, Machine::MemoryAddress& start_result);

} // namespace Compolls