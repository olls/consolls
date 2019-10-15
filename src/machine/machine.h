#pragma once

#include "palette.h"

#include "utils/texture.h"
#include "utils/assert.h"
#include "utils/types.h"


namespace Machine
{

using MemoryAddress = u16;


template <u32 screen_buffer_width, u32 screen_buffer_height>
struct Memory
{
  static const u32 sb_width = screen_buffer_width;
  static const u32 sb_height = screen_buffer_height;

  static constexpr u32 pixel_size = 4; // bits
  static_assert(pixel_size < 8);

  static const u32 screen_buffer_size = (screen_buffer_width * screen_buffer_height * pixel_size) / 8;
  static const u32 general_buffer_size = 0x2000;
  static const u32 size = (screen_buffer_size + general_buffer_size);

  u8 bytes[size];

  // General buffer: b_000X_XXXX_XXXX_XXXX
  // Screen buffer:  b_001X_XXXX_XXXX_XXXX

  //
  // Screen buffer:
  // 128*128 pixels
  //  * 4 bit colours
  //  = 0x2000 bytes
  //
  // 1 row = 128 pixels = 0x40 bytes
  //
  // r0   = 0x0000
  // r8   = 0x0200
  // r16  = 0x0400
  // r32  = 0x0800
  // r64  = 0x1000
  // r127 = 0x2000-0x40 = 0x1fC0
  //
  // 0x2000 = 8 KiB
  //
  // User memory   = [0x0000, 0x2000)  8 KiB
  // Screen buffer = [0x2000, 0x4000)  8 KiB
  // Total = 16 KiB
  //
  // 16KiB of address space requires 14 bits or 2 bytes to index. (u16)
  //
  // Instructions have a set specified length of their inputs, which is the gap until the next instruction.
  // All instructions are byte aligned.
  //
  // 0x102A+2 = shorthand for the bytes [0x102A, 0x102B, 0x102C] inclusive.
  //
  // 0x0000+1 is the next instruction register.
  //
};


namespace Reserved
{
enum : MemoryAddress
{
  NI = 0x0000,
  UserStart = 0x0002,

  PollInput = 0x1FFE,
  Blit = 0x1FFF,
  ScreenBuffer = 0x2000,

  FlagsStart = PollInput,
  UserEnd = FlagsStart
};
} // namespace Reserved


struct Machine
{
  Memory<128, 128> memory;
};


template <typename element_type>
inline element_type* const
get_ptr(Machine const& machine, MemoryAddress addr)
{
  assert(addr < machine.memory.size);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-align"
  return (element_type* const)(machine.memory.bytes + addr);
#pragma clang diagnostic pop
}


template <typename element_type>
inline element_type &
get(Machine const& machine, MemoryAddress addr)
{
  return *get_ptr<element_type>(machine, addr);
}


template <typename element_type>
inline void
set(Machine& machine, MemoryAddress addr, element_type byte_value)
{
  *get_ptr<element_type>(machine, addr) = byte_value;
}


template <typename element_type>
inline void
copy(Machine const& machine, MemoryAddress start, element_type const* elements, u32 n_elements)
{
  memcpy(machine.memory.bytes + start, elements, n_elements * sizeof(element_type));
}


inline void
set(Machine& machine, MemoryAddress start, u8 element, u32 n_copies)
{
  memset(machine.memory.bytes + start, element, n_copies);
}


template <typename element_type>
inline MemoryAddress
advance_addr(MemoryAddress& addr)
{
  MemoryAddress result = addr;
  addr += sizeof(element_type);
  return result;
}

template <>
inline MemoryAddress
advance_addr<void>(MemoryAddress& addr)
{
  return addr;
}


template <typename element_type>
inline element_type &
advance_addr(Machine const& machine, MemoryAddress& addr)
{
  return *get_ptr<element_type>(machine, advance_addr<element_type>(addr));
}


void
advance(Machine &machine);


bool
consume_signal_register(Machine& machine, MemoryAddress addr);


void
output_screen_buffer(Machine const& machine, Texture::Texture<Palette::Colour>& texture);

} // namespace Machine
