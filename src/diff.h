#pragma once

#include "buffer.h"
#include "types.h"

#include "allocate.h"


namespace Diff
{

Buffer::Buffer
create(u16 state_size, u8 const * const previous_state, u8 const * const state);


void
apply(Buffer::Buffer const & diff, u16 state_size, u8 * const state);


inline void
print(u16 s, u8 const * const b)
{
  for (u16 i = 0;
       i < s;
       ++i)
  {
    printf("%u ", b[i]);
  }
  printf("\n");
}

inline void
test()
{
  u16 size = 16;
  u8* a = Allocate::allocate<u8>(size);
  u8* b = Allocate::allocate<u8>(size);

  b[5] = 1;
  b[7] = 2;
  b[8] = 2;
  b[9] = 2;
  b[10] = 2;
  b[11] = 1;
  b[12] = 2;
  b[15] = 9;

  print(size, a);
  print(size, b);

  Buffer::Buffer diff = create(size, a, b);
  apply(diff, size, a);
  print(size, a);

  printf("\n");

  for (u32 i = 0;
       i < diff.size_used;
       ++i)
  {
    printf("%u\n", diff.buffer[i]);
  }
}

} // namespace Diff