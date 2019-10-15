#pragma once

#include "utils/assert.h"
#include "utils/types.h"
#include "utils/mem.h"

#if defined(__MACH__)
#include <cstdlib>
#else
#include <malloc.h>
#endif

#include <initializer_list>  // This is simple self contained impl


namespace Allocate
{

template <typename X>
X*
allocate(size_t n_elements)
{
  size_t const bytes_size = sizeof(X) * n_elements;
  X* result = (X*)malloc(bytes_size);
  memset(result, 0, bytes_size);

  return result;
}


template <typename X>
X*
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
X*
copy(X const& x)
{
  X* result = allocate<X>();
  memcpy(result, &x, sizeof(X));
  return result;
}


template <typename X>
X*
copy(u32& n_elements_result, std::initializer_list<X> xs)
{
  assert(xs.size() > 0);
  n_elements_result = (u32)xs.size();

  X* result = allocate<X>(xs.size());
  X* ptr = result;
  for (X const& x : xs)
  {
    memcpy(ptr, &x, sizeof(X));
    ptr += 1;
  }
  return result;
}

} // namespace size_t
