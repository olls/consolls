#pragma once

#include "basolls.h"
#include "string.h"
#include "types.h"


namespace Compolls
{

bool
compile(String::String text, Machine::Machine& machine, Basolls::MemoryAddress& addr, Basolls::Subroutine<void>& result);

} // namespace Compolls