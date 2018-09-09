#include "basolls.h"

#include "palette.h"
#include "instructions.h"

namespace Basolls
{

using Machine::MemoryAddress;


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
  memcpy(ptr, data, size);

  MemoryAddress start = addr;
  addr += size;

  return start;
}


template <typename width>
MemoryAddress
push_variable(MemoryAddress& addr)
{
  MemoryAddress result = addr;
  addr += sizeof(width);
  return result;
}


MemoryAddress
demo_program(Machine::Machine& machine, MemoryAddress& addr)
{
  MemoryAddress stride = push_variable<u16>(addr);
  MemoryAddress colour = push_variable<u8>(addr);
  MemoryAddress colour_a = push_variable<u16>(addr);
  MemoryAddress counter = push_variable<u16>(addr);
  MemoryAddress pixel_pos = push_variable<u16>(addr);
  MemoryAddress offset = push_variable<u16>(addr);

  MemoryAddress program_start = addr;

  push_instruction<Instructions::SET<u16>>(machine, addr, {
    .addr = stride,
    .value = 71
  });

  push_instruction<Instructions::SET<u8>>(machine, addr, {
    .addr = colour,
    .value = Palette::Red
  });

  push_instruction<Instructions::SET<u16>>(machine, addr, {
    .addr = colour_a,
    .value = colour
  });

  push_instruction<Instructions::SET<u16>>(machine, addr, {
    .addr = offset,
    .value = Machine::Reserved::ScreenBuffer
  });

  push_instruction<Instructions::SET<u16>>(machine, addr, {
    .addr = counter,
    .value = 0
  });

  MemoryAddress loop = addr;

  push_instruction<Instructions::ADD<u16>>(machine, addr, {
    .a = counter,
    .b = offset,
    .result = pixel_pos
  });

  push_instruction<Instructions::COPY<u8>>(machine, addr, {
    .from = colour_a,
    .to = pixel_pos
  });

  push_instruction<Instructions::ADD<u16>>(machine, addr, {
    .a = counter,
    .b = stride,
    .result = counter
  });

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

  push_instruction<Instructions::JUMP>(machine, addr, {
    .addr = loop
  });

  return program_start;
}


MemoryAddress
compiler_program(Machine::Machine& machine, MemoryAddress& addr)
{
  const char test_program[] = "SET 0x300 0xFF\n";
  MemoryAddress text = push_data(machine, addr, sizeof(test_program), test_program);

  MemoryAddress program_start = addr;

  push_instruction<Instructions::SET<u16>>(machine, addr, {
    .addr = 0x300,
    .value = 71
  });

  return program_start;
}

} // namespace Basolls