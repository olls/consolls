#include "buffer.h"

#include "allocate.h"
#include "maths.h"


namespace Buffer
{

void
append(Buffer& buffer, u8 const * elements, u32 n_elements)
{
  if (n_elements > 0)
  {
    u32 size_needed = buffer.size_used + n_elements;

    if (buffer.allocated_size < size_needed)
    {
      u32 new_size = buffer.allocated_size;

      if (new_size == 0)
      {
        new_size = INITIAL_SIZE;
      }

      while (new_size < size_needed)
      {
        new_size *= 2;
      }

      resize(buffer, new_size);
    }

    memcpy(buffer.buffer + buffer.size_used, elements, n_elements);
    buffer.size_used += n_elements;
  }
}


void
resize(Buffer& buffer, u32 new_size)
{
  assert(buffer.size_used <= buffer.allocated_size);

  if (new_size != buffer.allocated_size)
  {
    u32 n_elements_to_copy = Maths::min(buffer.size_used, new_size);
    u8* old_buffer = buffer.buffer;

    buffer.buffer = Allocate::allocate<u8>(new_size);
    buffer.allocated_size = new_size;
    buffer.size_used = 0;

    append(buffer, old_buffer, n_elements_to_copy);
    Allocate::unallocate(old_buffer);
  }
}

} // namespace Buffer