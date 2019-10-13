#pragma once

#include "utils/assert.h"
#include "utils/types.h"


namespace EnumArray
{

template <typename t_Index, typename t_Element>
struct EnumArray
{
  using IndexType = t_Index;
  using ElementType = t_Element;
  static u32 const size = (u32)IndexType::COUNT - (u32)IndexType::START;

  ElementType const&
  operator[](IndexType index) const
  {
    assert(index >= IndexType::START && index < IndexType::COUNT);
    u32 offset = (u32)index - (u32)IndexType::START;
    return members[offset];
  }
  ElementType&
  operator[](IndexType index)
  {
    assert(index >= IndexType::START && index < IndexType::COUNT);
    u32 offset = (u32)index - (u32)IndexType::START;
    return members[offset];
  }

  ElementType members[size];
};

}  // namespace EnumArray
