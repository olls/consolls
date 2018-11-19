#include "types.h"


namespace Meta
{

template <typename T, T item, typename t_Next>
struct Item
{
  using Next = t_Next;

  static T
  Get(u32 index)
  {
    if (index == 0)
    {
      return item;
    }
    else
    {
      index -= 1;
      return Next::Get(index);
    }
  }
};


template <typename T>
struct End
{
  static T
  Get(u32 index)
  {
    assert(0);
  }
};


template <typename T, T item1>
struct List1 : Item<T, item1, End<T>>
{};

template <typename T, T item1, T item2>
struct List2 : Item<T, item1, Item<T, item2, End<T>>>
{};

} // namespace Meta