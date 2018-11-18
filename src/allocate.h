#pragma once

#include "types.h"

#if defined(__MACH__)
#include <cstdlib>
#else
#include <malloc.h>
#endif

#include <cstring>


namespace Allocate
{

template <typename X>
X *
allocate(u32 n_elements)
{
  u32 const bytes_size = sizeof(X) * n_elements;
  X *result = (X *)malloc(bytes_size);
  memset(result, 0, bytes_size);

  return result;
}


template <typename X>
X *
allocate()
{
  return allocate<X>(1);
}


template <typename X>
inline void
unallocate(X* & ptr)
{
  free(ptr);
  ptr = NULL;
}


template <typename X>
X *
copy(X const & x)
{
  X* result = allocate<X>();
  memcpy(result, &x, sizeof(X));
  return result;
}

} // namespace Allocate