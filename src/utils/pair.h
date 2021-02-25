#pragma once

template <typename Key_t, typename Value_t>
struct Pair
{
  using Key = Key_t;
  using Value = Value_t;

  Key key;
  Value value;
};
