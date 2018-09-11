#pragma once

#include "machine.h"


namespace Basolls
{

using Machine::MemoryAddress;


template <typename ArgsType>
struct Subroutine
{
  MemoryAddress start;
  MemoryAddress return_addr;
  MemoryAddress args;
};


bool
load_os(Machine::Machine& machine);

} // namespace Basolls