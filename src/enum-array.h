#pragma once

#include "assert.h"
#include "types.h"


namespace EnumArray
{


template <typename t_Index, typename t_Element>
struct EnumArray
{
  using IndexType = t_Index;
  using ElementType = t_Element;
  static size_t const size = (size_t)IndexType::COUNT - (size_t)IndexType::START;

  ElementType const&
  operator[](IndexType index) const
  {
    assert(index >= IndexType::START && index < IndexType::COUNT);
    size_t offset = (size_t)index - (size_t)IndexType::START;
    return members[offset];
  }
  ElementType&
  operator[](IndexType index)
  {
    assert(index >= IndexType::START && index < IndexType::COUNT);
    size_t offset = (size_t)index - (size_t)IndexType::START;
    return members[offset];
  }

  ElementType members[size];
};


}  // namespace EnumArray
