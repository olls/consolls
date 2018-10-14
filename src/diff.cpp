#include "diff.h"


namespace Diff
{

Buffer::Buffer
create(u16 const state_size, u8 const * const previous_state, u8 const * const state)
{
  Buffer::Buffer result = {};

  bool in_diff_section = false;
  u16 diff_section_start;

  // Loops through one extra byte so that the final diff section can be closed.
  for (u16 byte_index = 0;
       byte_index <= state_size;
       ++byte_index)
  {
    if (byte_index < state_size &&
        previous_state[byte_index] != state[byte_index])
    {
      if (!in_diff_section)
      {
        in_diff_section = true;
        diff_section_start = byte_index;
      }
    }
    else
    {
      if (in_diff_section)
      {
        in_diff_section = false;
        u16 diff_section_length = byte_index - diff_section_start;

        // Write header and diff section to result
        result += diff_section_start;
        result += diff_section_length;
        Buffer::append(result, state + diff_section_start, diff_section_length);
      }
    }
  }

  return result;
}


void
apply(Buffer::Buffer const & diff, u16 state_size, u8 * const state)
{
  Buffer::Reader diff_reader = {diff, 0};

  while (diff_reader.read_offset < diff.size_used)
  {
    u16 position = Buffer::get<u16>(diff_reader);
    u16 length = Buffer::get<u16>(diff_reader);

    memcpy(state+position, diff.buffer+diff_reader.read_offset, length);
    diff_reader.read_offset += length;
  }

  assert(diff_reader.read_offset == diff.size_used);
}

} // namespace Diff