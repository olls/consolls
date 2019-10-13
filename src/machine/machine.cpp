#include "machine.h"

#include "palette.h"
#include "instructions.h"
#include "utils/assert.h"


namespace Machine
{

template <Instructions::Code Code>
void
inst(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Code> args)
{}


template <>
void
inst<Instructions::Code::ADD>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::ADD> args)
{
  u8 a = get<u8>(machine, args.a);
  u8 b = get<u8>(machine, args.b);
  u8 result = a + b;

  set<u8>(machine, args.result, result);
}

template <>
void
inst<Instructions::Code::ADD_W>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::ADD_W> args)
{
  u16 a = get<u16>(machine, args.a);
  u16 b = get<u16>(machine, args.b);
  u16 result = a + b;

  set<u16>(machine, args.result, result);
}


template <>
void
inst<Instructions::Code::SUB>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::SUB> args)
{
  u8 a = get<u8>(machine, args.a);
  u8 b = get<u8>(machine, args.b);
  u8 result = a - b;

  set<u8>(machine, args.result, result);
}

template <>
void
inst<Instructions::Code::SUB_W>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::SUB_W> args)
{
  u16 a = get<u16>(machine, args.a);
  u16 b = get<u16>(machine, args.b);
  u16 result = a - b;

  set<u16>(machine, args.result, result);
}


template <>
void
inst<Instructions::Code::MUL>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::MUL> args)
{
  u8 a = get<u8>(machine, args.a);
  u8 b = get<u8>(machine, args.b);
  u8 result = a * b;

  set<u8>(machine, args.result, result);
}

template <>
void
inst<Instructions::Code::MUL_W>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::MUL_W> args)
{
  u16 a = get<u16>(machine, args.a);
  u16 b = get<u16>(machine, args.b);
  u16 result = a * b;

  set<u16>(machine, args.result, result);
}


template <>
void
inst<Instructions::Code::DIV>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::DIV> args)
{
  u8 a = get<u8>(machine, args.a);
  u8 b = get<u8>(machine, args.b);
  u8 result = a / b;

  set<u8>(machine, args.result, result);
}

template <>
void
inst<Instructions::Code::DIV_W>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::DIV_W> args)
{
  u16 a = get<u16>(machine, args.a);
  u16 b = get<u16>(machine, args.b);
  u16 result = a / b;

  set<u16>(machine, args.result, result);
}


template <>
void
inst<Instructions::Code::NOT>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::NOT> args)
{
  u8 a = get<u8>(machine, args.in);
  set<u8>(machine, args.result, ~a);
}


template <>
void
inst<Instructions::Code::LSHIFT>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::LSHIFT> args)
{
  u8 in = get<u8>(machine, args.in);
  u8 bits = get<u8>(machine, args.bits);
  set<u8>(machine, args.result, in << bits);
}


template <>
void
inst<Instructions::Code::RSHIFT>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::RSHIFT> args)
{
  u8 in = get<u8>(machine, args.in);
  u8 bits = get<u8>(machine, args.bits);
  set<u8>(machine, args.result, in >> bits);
}


template <>
void
inst<Instructions::Code::AND>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::AND> args)
{
  u8 a = get<u8>(machine, args.a);
  u8 b = get<u8>(machine, args.b);
  set<u8>(machine, args.result, a & b);
}


template <>
void
inst<Instructions::Code::OR>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::OR> args)
{
  u8 a = get<u8>(machine, args.a);
  u8 b = get<u8>(machine, args.b);
  set<u8>(machine, args.result, a | b);
}


template <>
void
inst<Instructions::Code::XOR>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::XOR> args)
{
  u8 a = get<u8>(machine, args.a);
  u8 b = get<u8>(machine, args.b);
  set<u8>(machine, args.result, a ^ b);
}


template <>
void
inst<Instructions::Code::JUMP_V>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::JUMP_V> args)
{
 instruction_ptr = args.addr;
}


template <>
void
inst<Instructions::Code::JUMP>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::JUMP> args)
{
 instruction_ptr = get<MemoryAddress>(machine, args.addr);
}


template <>
void
inst<Instructions::Code::CJUMP>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::CJUMP> args)
{
  u8 a = get<u8>(machine, args.a);
  u8 b = get<u8>(machine, args.b);

  if (a > b)
  {
    instruction_ptr = args.addr;
  }
}

template <>
void
inst<Instructions::Code::CJUMP_W>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::CJUMP_W> args)
{
  u16 a = get<u16>(machine, args.a);
  u16 b = get<u16>(machine, args.b);

  if (a > b)
  {
    instruction_ptr = args.addr;
  }
}


template <>
void
inst<Instructions::Code::CMP>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::CMP> args)
{
  u8 a = get<u8>(machine, args.a);
  u8 b = get<u8>(machine, args.b);

  if (a == b)
  {
    set<u8>(machine, args.result, 0x01);
  }
}

template <>
void
inst<Instructions::Code::CMP_W>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::CMP_W> args)
{
  u16 a = get<u16>(machine, args.a);
  u16 b = get<u16>(machine, args.b);

  if (a == b)
  {
    set<u8>(machine, args.result, 0x00);
  }
}


template <>
void
inst<Instructions::Code::SET_V>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::SET_V> args)
{
  set<u8>(machine, args.addr, args.value);
}

template <>
void
inst<Instructions::Code::SET_VW>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::SET_VW> args)
{
  set<u16>(machine, args.addr, args.value);
}


template <>
void
inst<Instructions::Code::COPY>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::COPY> args)
{
  u8 from_value = get<u8>(machine, args.from);
  set<u8>(machine, args.to, from_value);
}

template <>
void
inst<Instructions::Code::COPY_W>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::COPY_W> args)
{
  u16 from_value = get<u16>(machine, args.from);
  set<u16>(machine, args.to, from_value);
}


template <>
void
inst<Instructions::Code::GET>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::GET> args)
{
  MemoryAddress from = get<MemoryAddress>(machine, args.from_p);
  u8 from_value = get<u8>(machine, from);
  set<u8>(machine, args.to, from_value);
}

template <>
void
inst<Instructions::Code::GET_W>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::GET_W> args)
{
  MemoryAddress from = get<MemoryAddress>(machine, args.from_p);
  u16 from_value = get<u16>(machine, from);
  set<u16>(machine, args.to, from_value);
}


template <>
void
inst<Instructions::Code::SET>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::SET> args)
{
  MemoryAddress to = get<MemoryAddress>(machine, args.to_p);
  u8 from_value = get<u8>(machine, args.from);
  set<u8>(machine, to, from_value);
}

template <>
void
inst<Instructions::Code::SET_W>(Machine& machine, MemoryAddress& instruction_ptr, Instructions::Args<Instructions::Code::SET_W> args)
{
  MemoryAddress to = get<MemoryAddress>(machine, args.to_p);
  u16 from_value = get<u16>(machine, args.from);
  set<u16>(machine, to, from_value);
}


template <Instructions::Code Code>
void
exe_inst(Machine& machine, MemoryAddress& instruction_ptr)
{
  Instructions::Args<Code> const args = advance_addr<Instructions::Args<Code>>(machine, instruction_ptr);

  inst<Code>(machine, instruction_ptr, args);
}


void
advance(Machine& machine)
{
  MemoryAddress instruction_ptr = get<MemoryAddress>(machine, Reserved::NI);
  Instructions::Code instruction = advance_addr<Instructions::Code>(machine, instruction_ptr);

  assert(instruction_ptr < Reserved::ScreenBuffer);

  switch (instruction)
  {
    case (Instructions::Code::NOP):
      break;
    case (Instructions::Code::ADD):       exe_inst<Instructions::Code::ADD>(machine, instruction_ptr);
      break;
    case (Instructions::Code::ADD_W):     exe_inst<Instructions::Code::ADD_W>(machine, instruction_ptr);
      break;
    case (Instructions::Code::SUB):       exe_inst<Instructions::Code::SUB>(machine, instruction_ptr);
      break;
    case (Instructions::Code::SUB_W):     exe_inst<Instructions::Code::SUB_W>(machine, instruction_ptr);
      break;
    case (Instructions::Code::MUL):       exe_inst<Instructions::Code::MUL>(machine, instruction_ptr);
      break;
    case (Instructions::Code::MUL_W):     exe_inst<Instructions::Code::MUL_W>(machine, instruction_ptr);
      break;
    case (Instructions::Code::DIV):       exe_inst<Instructions::Code::DIV>(machine, instruction_ptr);
      break;
    case (Instructions::Code::DIV_W):     exe_inst<Instructions::Code::DIV_W>(machine, instruction_ptr);
      break;
    case (Instructions::Code::NOT):       exe_inst<Instructions::Code::NOT>(machine, instruction_ptr);
      break;
    case (Instructions::Code::LSHIFT):    exe_inst<Instructions::Code::LSHIFT>(machine, instruction_ptr);
      break;
    case (Instructions::Code::RSHIFT):    exe_inst<Instructions::Code::RSHIFT>(machine, instruction_ptr);
      break;
    case (Instructions::Code::AND):       exe_inst<Instructions::Code::AND>(machine, instruction_ptr);
      break;
    case (Instructions::Code::OR):        exe_inst<Instructions::Code::OR>(machine, instruction_ptr);
      break;
    case (Instructions::Code::XOR):       exe_inst<Instructions::Code::XOR>(machine, instruction_ptr);
      break;
    case (Instructions::Code::JUMP_V):    exe_inst<Instructions::Code::JUMP_V>(machine, instruction_ptr);
      break;
    case (Instructions::Code::JUMP):      exe_inst<Instructions::Code::JUMP>(machine, instruction_ptr);
      break;
    case (Instructions::Code::CJUMP):     exe_inst<Instructions::Code::CJUMP>(machine, instruction_ptr);
      break;
    case (Instructions::Code::CJUMP_W):   exe_inst<Instructions::Code::CJUMP_W>(machine, instruction_ptr);
      break;
    case (Instructions::Code::CMP):       exe_inst<Instructions::Code::CMP>(machine, instruction_ptr);
      break;
    case (Instructions::Code::CMP_W):     exe_inst<Instructions::Code::CMP_W>(machine, instruction_ptr);
      break;
    case (Instructions::Code::SET_V):     exe_inst<Instructions::Code::SET_V>(machine, instruction_ptr);
      break;
    case (Instructions::Code::SET_VW):    exe_inst<Instructions::Code::SET_VW>(machine, instruction_ptr);
      break;
    case (Instructions::Code::COPY):      exe_inst<Instructions::Code::COPY>(machine, instruction_ptr);
      break;
    case (Instructions::Code::COPY_W):    exe_inst<Instructions::Code::COPY_W>(machine, instruction_ptr);
      break;
    case (Instructions::Code::GET):      exe_inst<Instructions::Code::GET>(machine, instruction_ptr);
      break;
    case (Instructions::Code::GET_W):    exe_inst<Instructions::Code::GET_W>(machine, instruction_ptr);
      break;
    case (Instructions::Code::SET):      exe_inst<Instructions::Code::SET>(machine, instruction_ptr);
      break;
    case (Instructions::Code::SET_W):    exe_inst<Instructions::Code::SET_W>(machine, instruction_ptr);
      break;
  }

  set<MemoryAddress>(machine, Reserved::NI, instruction_ptr);
}


bool
consume_signal_register(Machine& machine, MemoryAddress addr)
{
  bool result = get<u8>(machine, addr);
  if (result)
  {
    get<u8>(machine, addr) = 0x00;
  }

  return result;
}


void
output_screen_buffer(Machine const & machine, Texture::Texture<Palette::Colour>& texture)
{
  u8 const *const sb = &machine.memory.bytes[Reserved::ScreenBuffer];

  u8 const pixel_mask = (1 << machine.memory.pixel_size) - 1;

  for (u32 y = 0;
       y < machine.memory.sb_height;
       ++y)
  {
    for (u32 x = 0;
         x < machine.memory.sb_width;
         ++x)
    {
      u32 pixel_offset = (y * machine.memory.sb_width) + x;
      r32 byte_offset_frac = pixel_offset * (machine.memory.pixel_size / 8.0);

      u32 byte_offset = u32(byte_offset_frac);
      assert(byte_offset < machine.memory.screen_buffer_size);

      u8 byte = sb[byte_offset];

      u32 bit_offset = 8*(byte_offset_frac - byte_offset);
      assert(bit_offset < 8);

      Palette::Index colour_index = (Palette::Index)((byte >> bit_offset) & pixel_mask);
      Palette::Colour colour = Palette::get_colour(colour_index);

      Texture::set_pixel(texture, x, y, colour);
    }
  }
}

} // namespace Machine