#pragma once

#include "basolls.h"
#include "string.h"
#include "types.h"


namespace Compolls
{

bool
compile_file(char const* filename, Machine::Machine& machine, Machine::MemoryAddress& addr, Machine::MemoryAddress& start_result, String::String* error = NULL);


bool
compile(String::String text, Machine::Machine& machine, Machine::MemoryAddress& addr, Machine::MemoryAddress& start_result);

} // namespace Compolls