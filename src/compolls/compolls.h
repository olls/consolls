#pragma once

#include "machine/basolls.h"
#include "machine/machine.h"

#include "utils/string.h"
#include "utils/types.h"


namespace Compolls
{

bool
compile_file(char const* filename, Machine::Machine& machine, Machine::MemoryAddress& addr, Machine::MemoryAddress& start_result, String::String* error = NULL);


bool
compile(String::String text, Machine::Machine& machine, Machine::MemoryAddress& addr, Machine::MemoryAddress& start_result);

} // namespace Compolls
