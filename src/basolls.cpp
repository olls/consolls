#include "basolls.h"

#include "palette.h"
#include "instructions.h"

namespace Basolls
{

template <typename Args>
void
push_instruction(Machine::Machine& machine, MemoryAddress& addr, Args args)
{
  Machine::advance_addr<Instructions::Code>(machine, addr) = Instructions::Args_to_Code<Args>;
  Machine::advance_addr<Args>(machine, addr) = args;
}


template <typename type>
MemoryAddress
push_data(Machine::Machine& machine, MemoryAddress& addr, u32 size, type* data)
{
  void* ptr = Machine::get_ptr<void>(machine, addr);
  memcpy(ptr, data, size*sizeof(type));

  MemoryAddress start = addr;
  addr += size*sizeof(type);

  return start;
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


template <typename ArgsType=void>
Subroutine
push_subroutine_start(Machine::Machine& machine, MemoryAddress& addr)
{
  Subroutine subroutine;

  // Reserve space for the caller to store args
  subroutine.args = push_variable<ArgsType>(addr);

  // Reserve a MemoryAddress for the caller to store their return address
  subroutine.return_addr = push_variable<MemoryAddress>(addr);
  subroutine.start = addr;

  return subroutine;
}


void
push_subroutine_end(Machine::Machine& machine, MemoryAddress& addr, Subroutine subroutine)
{
  push_instruction<Instructions::JUMP_I>(machine, addr, {
    .addr = subroutine.return_addr
  });
}


void
push_subroutine_call(Machine::Machine& machine, MemoryAddress& addr, Subroutine subroutine)
{
  // The two bytes prior to the subroutine's address are reserved for the caller to store their return address
  MemoryAddress return_location = addr + (sizeof(Instructions::Code::SET) + sizeof(Instructions::SET<u16>) +
                                          sizeof(Instructions::Code::JUMP) + sizeof(Instructions::JUMP));

  push_instruction<Instructions::SET<u16>>(machine, addr, {
    .addr = subroutine.return_addr,
    .value = return_location
  });

  push_instruction<Instructions::JUMP>(machine, addr, {subroutine.start});
}


#define SUBROUTINE_ARG_POSITION(ArgsType, subroutine, arg) (MemoryAddress)((subroutine).args + offsetof(ArgsType, arg))


struct DotSubroutineArgs
{
  MemoryAddress pixel_pos;
  u8 colour;
};

Subroutine
push_dot_subroutine(Machine::Machine& machine, MemoryAddress& addr)
{
  Subroutine subroutine = push_subroutine_start<DotSubroutineArgs>(machine, addr);

  MemoryAddress pixel_pos = SUBROUTINE_ARG_POSITION(DotSubroutineArgs, subroutine, pixel_pos);
  MemoryAddress colour = SUBROUTINE_ARG_POSITION(DotSubroutineArgs, subroutine, colour);

  push_instruction<Instructions::SET_I<u8>>(machine, addr, {
    .addr = pixel_pos,
    .value = colour
  });

  push_subroutine_end(machine, addr, subroutine);

  return subroutine;
}


Subroutine
push_demo_program(Machine::Machine& machine, MemoryAddress& addr)
{
  // Subroutines

  Subroutine dot = push_dot_subroutine(machine, addr);

  // Variables / Constants

  MemoryAddress colour = push_value<u8>(machine, addr, Palette::Red);
  MemoryAddress stride = push_value<u16>(machine, addr, 125);
  MemoryAddress counter = push_value<u16>(machine, addr, 0);
  MemoryAddress offset = push_value<u16>(machine, addr, Machine::Reserved::ScreenBuffer);

  MemoryAddress pixel_pos = push_variable<u16>(addr);

  // Start routine

  Subroutine subroutine = push_subroutine_start(machine, addr);

  MemoryAddress loop = addr;

  push_instruction<Instructions::ADD<u16>>(machine, addr, {
    .a = counter,
    .b = offset,
    .result = pixel_pos
  });

  // Set dots args
  push_instruction<Instructions::COPY<u16>>(machine, addr, {
    .from = pixel_pos,
    .to = SUBROUTINE_ARG_POSITION(DotSubroutineArgs, dot, pixel_pos)
  });

  push_instruction<Instructions::COPY<u8>>(machine, addr, {
    .from = colour,
    .to = SUBROUTINE_ARG_POSITION(DotSubroutineArgs, dot, colour)
  });

  // Call dot
  push_subroutine_call(machine, addr, dot);

  // Increment counter

  push_instruction<Instructions::ADD<u16>>(machine, addr, {
    .a = counter,
    .b = stride,
    .result = counter
  });
  // Check (and reset) counter

  push_instruction<Instructions::CMP<u16>>(machine, addr, {
    .a = offset,
    .b = counter,
    .addr = loop
  });

  push_instruction<Instructions::SUB<u16>>(machine, addr, {
    .a = counter,
    .b = offset,
    .result = counter
  });

  // Blit!

  push_instruction<Instructions::SET<u8>>(machine, addr, {
    .addr = Machine::Reserved::Blit,
    .value = 0x01
  });

  push_instruction<Instructions::JUMP>(machine, addr, {loop});

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