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
void
push_data(Machine::Machine& machine, Machine::MemoryAddress& addr, u32 size, type* data)
{
  void* ptr = Machine::get_ptr<void>(machine, addr);
  memcpy(ptr, data, size);

  addr += size;
}


void
load_compiler_program(Machine::Machine& machine)
{
  using Machine::MemoryAddress;

  MemoryAddress addr = Machine::Reserved::UserStart;
  Machine::set<MemoryAddress>(machine, Machine::Reserved::NI, addr);

  MemoryAddress vars_start = 0x100;
  MemoryAddress text_start = 0x200;

  MemoryAddress next_var = vars_start;
  MemoryAddress stride = next_var; next_var += 2;
  MemoryAddress colour = next_var; next_var += 1;
  MemoryAddress colour_a = next_var; next_var += 2;
  MemoryAddress counter = next_var; next_var += 2;
  MemoryAddress pixel_pos = next_var; next_var += 2;
  MemoryAddress offset = next_var; next_var += 2;

  assert(next_var < text_start);

  const char test_program[] = "u8 One := 1;\n";
  push_data(machine, text_start, sizeof(test_program), test_program);

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

  assert(addr < vars_start);
}

} // namespace Basolls