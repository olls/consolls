#include "assert.h"
#include "types.h"


namespace Buffer
{

static const u32 INITIAL_SIZE = 16;


struct Buffer
{
  u32 allocated_size;
  u32 size_used;
  u8* buffer;
};


void
resize(Buffer& buffer, u32 new_size);


void
append(Buffer& buffer, u8 const * elements, u32 n_elements);


template <typename T>
Buffer &
operator+=(Buffer& buffer, T const & element)
{
  append(buffer, (u8 const *)&element, sizeof(T));
  return buffer;
}


struct Reader
{
  Buffer const &buffer;
  u32 read_offset;
};


template <typename T>
T &
get(Reader& reader)
{
  T* result = (T *)(reader.buffer.buffer + reader.read_offset);

  reader.read_offset += sizeof(T);
  assert(reader.read_offset <= reader.buffer.size_used);

  return *result;
}

} // namespace Buffer