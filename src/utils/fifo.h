#pragma once

#include "array.h"
#include "types.h"


namespace Fifo
{

template <typename T>
struct Fifo
{
  Array::Array<T> elements;
  u32 position;
};


template <typename T>
s32
elements_remaining(Fifo<T> const& fifo)
{
  return fifo.elements.n_elements - fifo.position;
}


template <typename T>
bool
empty(Fifo<T> const& fifo)
{
  return elements_remaining(fifo) <= 0;
}


template <typename T>
T&
pop(Fifo<T>& fifo)
{
  assert(!empty(fifo));

  T& result = fifo.elements[fifo.position];
  fifo.position += 1;
  return result;
}

} // namespace Fifo
