#include "basolls.h"

#include "disassembler.h"
#include "palette.h"

#include "utils/debug.h"
#include "utils/string.h"


namespace Basolls
{

#define SUBROUTINE_ARG_POSITION(ArgsType, subroutine_args, arg) (MemoryAddress)((subroutine_args) + offsetof(ArgsType, arg))


struct PACKED DotSubroutineArgs
{
  MemoryAddress offset;
  MemoryAddress pixel_pos;
  u8 colour;
};

Subroutine<DotSubroutineArgs>
push_dot_subroutine(Machine::Machine& machine, MemoryAddress& addr)
{
  MemoryAddress const one = push_value<u8>(machine, addr, 0x1);
  MemoryAddress const two = push_value<u16>(machine, addr, 0x2);
  MemoryAddress const mask = Machine::advance_addr<u8>(addr);
  MemoryAddress const pixel_shift = Machine::advance_addr<u8>(addr);
  MemoryAddress const old_colour = Machine::advance_addr<u8>(addr);

  Subroutine<DotSubroutineArgs> subroutine = push_subroutine_start<DotSubroutineArgs>(addr);

  MemoryAddress const offset = SUBROUTINE_ARG_POSITION(DotSubroutineArgs, subroutine.args, offset);
  MemoryAddress const pixel_pos = SUBROUTINE_ARG_POSITION(DotSubroutineArgs, subroutine.args, pixel_pos);
  MemoryAddress const colour = SUBROUTINE_ARG_POSITION(DotSubroutineArgs, subroutine.args, colour);

  push_instruction<Code::AND>(machine, addr, {
    .a = pixel_pos,
    .b = one,
    .result = pixel_shift
  });

  push_instruction<Code::LSHIFT>(machine, addr, {
    .in = pixel_shift,
    .bits = two,
    .result = pixel_shift
  });

  push_instruction<Code::LSHIFT>(machine, addr, {
    .in = colour,
    .bits = pixel_shift,
    .result = colour
  });

  push_instruction<Code::DIV_W>(machine, addr, {
    .a = pixel_pos,
    .b = two,
    .result = pixel_pos
  });

  // MASK
  push_instruction<Code::SET_V>(machine, addr, {
    .value = 0xf0,
    .addr = mask
  });

  push_instruction<Code::RSHIFT>(machine, addr, {
    .in = mask,
    .bits = pixel_shift,
    .result = mask
  });

  push_instruction<Code::ADD_W>(machine, addr, {
    .a = pixel_pos,
    .b = offset,
    .result = pixel_pos
  });

  push_instruction<Code::GET>(machine, addr, {
    .from_p = pixel_pos,
    .to = old_colour
  });

  // Clear the bits we are or-ing into
  push_instruction<Code::AND>(machine, addr, {
    .a = old_colour,
    .b = mask,
    .result = old_colour
  });

  push_instruction<Code::OR>(machine, addr, {
    .a = old_colour,
    .b = colour,
    .result = colour
  });

  push_instruction<Code::SET>(machine, addr, {
    .from = colour,
    .to_p = pixel_pos
  });

  push_subroutine_end(machine, addr, subroutine);

  printf("Dot:\n");
  Disassembler::disassemble(machine, subroutine.start, subroutine.end);
  printf("\n\n");

  return subroutine;
}


Subroutine<void>
push_demo_program(Machine::Machine& machine, MemoryAddress& addr)
{
  // Subroutines

  Subroutine<DotSubroutineArgs> dot = push_dot_subroutine(machine, addr);

  // Variables / Constants

  MemoryAddress const dot_args = Machine::advance_addr<DotSubroutineArgs>(addr);
  MemoryAddress const offset = SUBROUTINE_ARG_POSITION(DotSubroutineArgs, dot_args, offset);
  MemoryAddress const pixel_pos = SUBROUTINE_ARG_POSITION(DotSubroutineArgs, dot_args, pixel_pos);
  MemoryAddress const colour = SUBROUTINE_ARG_POSITION(DotSubroutineArgs, dot_args, colour);

  Machine::get<u16>(machine, offset) = Machine::Reserved::ScreenBuffer;
  Machine::get<u8>(machine, colour) = Palette::Red;
  Machine::get<u16>(machine, pixel_pos) = 0x0000;

  MemoryAddress const n_pixels = push_value<u16>(machine, addr, 0x4000);
  MemoryAddress const stride = push_value<u16>(machine, addr, 0x0031);

  // Start routine
  Subroutine<void> subroutine = push_subroutine_start<void>(addr);

  MemoryAddress const loop = addr;

  // Call dot
  push_copy_subroutine_args(machine, addr, dot, dot_args);
  push_subroutine_call(machine, addr, dot);

  // Increment pixel_pos
  push_instruction<Code::ADD_W>(machine, addr, {
    .a = pixel_pos,
    .b = stride,
    .result = pixel_pos
  });

  // Check (and reset) pixel_pos
  push_instruction<Code::CJUMP_W>(machine, addr, {
    .a = n_pixels,
    .b = pixel_pos,
    .addr = loop
  });

  push_instruction<Code::SUB_W>(machine, addr, {
    .a = pixel_pos,
    .b = n_pixels,
    .result = pixel_pos
  });

  // Blit!
  push_instruction<Code::SET_V>(machine, addr, {
    .addr = Machine::Reserved::Blit,
    .value = 0x01
  });

  push_instruction<Code::JUMP_V>(machine, addr, {loop});

  // End routine
  push_subroutine_end(machine, addr, subroutine);

  printf("Demo:\n");
  Disassembler::disassemble(machine, subroutine.start, subroutine.end);
  printf("\n\n");

  return subroutine;
}


bool
load_os(Machine::Machine& machine)
{
  bool success = true;

  MemoryAddress addr = Machine::Reserved::UserStart;

  Subroutine<void> const demo_program = push_demo_program(machine, addr);

  MemoryAddress const os_start = addr;
  push_subroutine_call(machine, addr, demo_program);

  Machine::set<MemoryAddress>(machine, Machine::Reserved::NI, os_start);

  printf("OS:\n");
  Disassembler::disassemble(machine, os_start, addr);
  printf("\n\n");

  return success;
}

} // namespace Basolls