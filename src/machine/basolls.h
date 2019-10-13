#pragma once

#include "machine.h"
#include "instructions.h"

#include "utils/types.h"


namespace Basolls
{

using Machine::MemoryAddress;
using Instructions::Code;


template <typename ArgsType>
struct Subroutine
{
  MemoryAddress start;
  MemoryAddress end;
  MemoryAddress return_addr;
  MemoryAddress args;
};


template <Code inst>
void
push_instruction(Machine::Machine& machine, MemoryAddress& addr, Instructions::Args<inst> args)
{
  Machine::advance_addr<Code>(machine, addr) = inst;
  Machine::advance_addr<Instructions::Args<inst>>(machine, addr) = args;
}


template <typename type>
MemoryAddress
push_data(Machine::Machine& machine, MemoryAddress& addr, u32 size, type const * data)
{
  void *const ptr = Machine::get_ptr<void>(machine, addr);
  memcpy(ptr, data, size*sizeof(type));

  MemoryAddress start = addr;
  addr += size*sizeof(type);

  return start;
}


template <typename type>
MemoryAddress
push_value(Machine::Machine& machine, MemoryAddress& addr, type value)
{
  MemoryAddress value_addr = addr;
  Machine::advance_addr<type>(machine, addr) = value;
  return value_addr;
}


template <typename ArgsType>
Subroutine<ArgsType>
push_subroutine_start(MemoryAddress& addr)
{
  Subroutine<ArgsType> subroutine;

  // Reserve space for the caller to store args
  subroutine.args = Machine::advance_addr<ArgsType>(addr);

  // Reserve a MemoryAddress for the caller to store their return address
  subroutine.return_addr = Machine::advance_addr<MemoryAddress>(addr);
  subroutine.start = addr;

  return subroutine;
}


template <typename ArgsType>
void
push_subroutine_end(Machine::Machine& machine, MemoryAddress& addr, Subroutine<ArgsType>& subroutine)
{
  push_instruction<Code::JUMP>(machine, addr, {
    .addr = subroutine.return_addr
  });
  subroutine.end = addr;
}


template <typename width>
void inline
push_copy(Machine::Machine& machine, MemoryAddress& addr, MemoryAddress from, MemoryAddress to)
{
  for (MemoryAddress byte_n = 0;
       byte_n < sizeof(width);
       ++byte_n)
  {
    push_instruction<Code::COPY>(machine, addr, {
      .from = (MemoryAddress)(from + byte_n),
      .to = (MemoryAddress)(to + byte_n)
    });
  }
}

template <>
void inline
push_copy<void>(Machine::Machine& machine, MemoryAddress& addr, MemoryAddress from, MemoryAddress to)
{}


template <typename ArgsType>
void
push_copy_subroutine_args(Machine::Machine& machine, MemoryAddress& addr, Subroutine<ArgsType> const & subroutine, MemoryAddress args_addr)
{
  push_copy<ArgsType>(machine, addr, args_addr, subroutine.args);
}


template <typename ArgsType>
void
push_subroutine_call(Machine::Machine& machine, MemoryAddress& addr, Subroutine<ArgsType> const & subroutine)
{
  MemoryAddress return_location = addr + (sizeof(Code::SET_VW) + sizeof(Instructions::Args<Code::SET_VW>) +
                                          sizeof(Code::JUMP_V) + sizeof(Instructions::Args<Code::JUMP_V>));

  push_instruction<Code::SET_VW>(machine, addr, {
    .addr = subroutine.return_addr,
    .value = return_location
  });

  push_instruction<Code::JUMP_V>(machine, addr, {subroutine.start});
}


bool
load_os(Machine::Machine& machine);

} // namespace Basolls