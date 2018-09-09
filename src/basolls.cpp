#include "basolls.h"

#include "palette.h"
#include "instructions.h"

namespace Basolls
{

template <typename Args>
void
push_instruction(Machine::Machine& machine, Machine::MemoryAddress& addr, Args args)
{
  Machine::advance_addr<Instructions::Code>(machine, addr) = Instructions::Args_to_Code<Args>;
  Machine::advance_addr<Args>(machine, addr) = args;
}


template <typename type>
Machine::MemoryAddress
push_data(Machine::Machine& machine, Machine::MemoryAddress& addr, u32 size, type* data)
{
  void* ptr = Machine::get_ptr<void>(machine, addr);
  memcpy(ptr, data, size);

  Machine::MemoryAddress start = addr;
  addr += size;

  return start;
}


Machine::MemoryAddress
demo_program(Machine::Machine& machine, Machine::MemoryAddress& addr)
{
  using Machine::MemoryAddress;

  MemoryAddress stride = addr; addr += 2;
  MemoryAddress colour = addr; addr += 1;
  MemoryAddress colour_a = addr; addr += 2;
  MemoryAddress counter = addr; addr += 2;
  MemoryAddress pixel_pos = addr; addr += 2;
  MemoryAddress offset = addr; addr += 2;

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


Machine::MemoryAddress
compiler_program(Machine::Machine& machine, Machine::MemoryAddress& addr)
{
  using Machine::MemoryAddress;

  Machine::set<MemoryAddress>(machine, Machine::Reserved::NI, addr);

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