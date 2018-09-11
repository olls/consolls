#include "basolls.h"

#include "palette.h"
#include "instructions.h"

namespace Basolls
{

template <Instructions::Code Code>
void
push_instruction(Machine::Machine& machine, MemoryAddress& addr, Instructions::Args<Code> args)
{
  Machine::advance_addr<Instructions::Code>(machine, addr) = Code;
  Machine::advance_addr<Instructions::Args<Code>>(machine, addr) = args;
}


template <typename type>
void
set_data(Machine::Machine& machine, MemoryAddress addr, u32 size, type* data)
{
  void* ptr = Machine::get_ptr<void>(machine, addr);
  memcpy(ptr, data, size*sizeof(type));
}


template <typename type>
MemoryAddress
push_data(Machine::Machine& machine, MemoryAddress& addr, u32 size, type* data)
{
  set_data<type>(machine, addr, size, data);

  MemoryAddress start = addr;
  addr += size*sizeof(type);

  return start;
}


template <typename type>
void
set_value(Machine::Machine& machine, MemoryAddress addr, type value)
{
  set_data<type>(machine, addr, 1, &value);
}


template <typename type>
MemoryAddress
push_value(Machine::Machine& machine, MemoryAddress& addr, type value)
{
  return push_data<type>(machine, addr, 1, &value);
}


template <typename width>
MemoryAddress
push_variable(MemoryAddress& addr)
{
  MemoryAddress result = addr;
  addr += sizeof(width);
  return result;
}

template <>
MemoryAddress
push_variable<void>(MemoryAddress& addr)
{
  return addr;
}


template <typename ArgsType>
Subroutine<ArgsType>
push_subroutine_start(Machine::Machine& machine, MemoryAddress& addr)
{
  Subroutine<ArgsType> subroutine;

  // Reserve space for the caller to store args
  subroutine.args = push_variable<ArgsType>(addr);

  // Reserve a MemoryAddress for the caller to store their return address
  subroutine.return_addr = push_variable<MemoryAddress>(addr);
  subroutine.start = addr;

  return subroutine;
}


template <typename ArgsType>
void
push_subroutine_end(Machine::Machine& machine, MemoryAddress& addr, Subroutine<ArgsType> subroutine)
{
  push_instruction<Instructions::Code::JUMP_I>(machine, addr, {
    .addr = subroutine.return_addr
  });
}


template <typename width>
void
push_type_copy(Machine::Machine& machine, MemoryAddress& addr, MemoryAddress from, MemoryAddress to)
{
  for (MemoryAddress byte_n = 0;
       byte_n < sizeof(width);
       ++byte_n)
  {
    push_instruction<Instructions::Code::COPY>(machine, addr, {
      .from = (MemoryAddress)(from + byte_n),
      .to = (MemoryAddress)(to + byte_n)
    });
  }
}

template <>
void
push_type_copy<void>(Machine::Machine& machine, MemoryAddress& addr, MemoryAddress from, MemoryAddress to)
{}


template <typename ArgsType>
void
push_load_subroutine_args(Machine::Machine& machine, MemoryAddress& addr, Subroutine<ArgsType> subroutine, MemoryAddress args_addr)
{
  push_type_copy<ArgsType>(machine, addr, args_addr, subroutine.args);
}


template <typename ArgsType>
void
push_subroutine_call(Machine::Machine& machine, MemoryAddress& addr, Subroutine<ArgsType> subroutine)
{
  // The two bytes prior to the subroutine's address are reserved for the caller to store their return address
  MemoryAddress return_location = addr + (sizeof(Instructions::Code::SET) + sizeof(Instructions::Args<Instructions::Code::SET_W>) +
                                          sizeof(Instructions::Code::JUMP) + sizeof(Instructions::Args<Instructions::Code::JUMP>));

  push_instruction<Instructions::Code::SET_W>(machine, addr, {
    .addr = subroutine.return_addr,
    .value = return_location
  });

  push_instruction<Instructions::Code::JUMP>(machine, addr, {subroutine.start});
}


#define SUBROUTINE_ARG_POSITION(ArgsType, subroutine_args, arg) (MemoryAddress)((subroutine_args) + offsetof(ArgsType, arg))


struct PACKED DotSubroutineArgs
{
  MemoryAddress pixel_pos;
  u8 colour;
};

Subroutine<DotSubroutineArgs>
push_dot_subroutine(Machine::Machine& machine, MemoryAddress& addr)
{
  Subroutine subroutine = push_subroutine_start<DotSubroutineArgs>(machine, addr);

  MemoryAddress pixel_pos = SUBROUTINE_ARG_POSITION(DotSubroutineArgs, subroutine.args, pixel_pos);
  MemoryAddress colour = SUBROUTINE_ARG_POSITION(DotSubroutineArgs, subroutine.args, colour);

  push_instruction<Instructions::Code::SET_I>(machine, addr, {
    .addr = pixel_pos,
    .value = colour
  });

  push_subroutine_end(machine, addr, subroutine);

  return subroutine;
}


Subroutine<void>
push_demo_program(Machine::Machine& machine, MemoryAddress& addr)
{
  // Subroutines

  Subroutine dot = push_dot_subroutine(machine, addr);

  // Variables / Constants

  MemoryAddress dot_args = push_variable<DotSubroutineArgs>(addr);
  MemoryAddress pixel_pos = SUBROUTINE_ARG_POSITION(DotSubroutineArgs, dot_args, pixel_pos);
  MemoryAddress colour = SUBROUTINE_ARG_POSITION(DotSubroutineArgs, dot_args, colour);

  set_value<u8>(machine, colour, Palette::Red);

  MemoryAddress stride = push_value<u16>(machine, addr, 125);
  MemoryAddress counter = push_value<u16>(machine, addr, 0);
  MemoryAddress offset = push_value<u16>(machine, addr, Machine::Reserved::ScreenBuffer);

  // Start routine
  Subroutine subroutine = push_subroutine_start<void>(machine, addr);

  MemoryAddress loop = addr;

  push_instruction<Instructions::Code::ADD_W>(machine, addr, {
    .a = counter,
    .b = offset,
    .result = pixel_pos
  });

  // Call dot
  push_load_subroutine_args(machine, addr, dot, dot_args);
  push_subroutine_call(machine, addr, dot);

  // Increment counter
  push_instruction<Instructions::Code::ADD_W>(machine, addr, {
    .a = counter,
    .b = stride,
    .result = counter
  });

  // Check (and reset) counter
  push_instruction<Instructions::Code::CMP_W>(machine, addr, {
    .a = offset,
    .b = counter,
    .addr = loop
  });

  push_instruction<Instructions::Code::SUB_W>(machine, addr, {
    .a = counter,
    .b = offset,
    .result = counter
  });

  // Blit!
  push_instruction<Instructions::Code::SET>(machine, addr, {
    .addr = Machine::Reserved::Blit,
    .value = 0x01
  });

  push_instruction<Instructions::Code::JUMP>(machine, addr, {loop});

  // End routine

  push_subroutine_end(machine, addr, subroutine);

  return subroutine;
}


bool
load_os(Machine::Machine& machine)
{
  bool success =  true;

  MemoryAddress addr = Machine::Reserved::UserStart;

  Subroutine demo_program = push_demo_program(machine, addr);

  MemoryAddress os_start = addr;
  push_subroutine_call(machine, addr, demo_program);

  Machine::set<MemoryAddress>(machine, Machine::Reserved::NI, os_start);

  return success;
}


} // namespace Basolls