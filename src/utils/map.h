#pragma once

#include "utils/array.h"
#include "utils/types.h"


namespace Map
{

template <typename Key_t, typename Value_t>
struct Pair
{
  using Key = Key_t;
  using Value = Value_t;

  Key key;
  Value value;
};

template <typename Key, typename Value>
using Map = Array::Array<Pair<Key, Value>>;


template <typename Key, typename Value>
Value *
find(Map<Key, Value> const& map, Key key)
{
  using PairT = Pair<Key, Value>;

  Value* result = NULL;

  Array::FindFirstFunc<PairT> find_func = [](PairT const& pair, void* user){ return pair.key == *(Key*)user; };
  s32 index = Array::find_first(map, find_func, &key);
  if (index != -1)
  {
    result = &(Array::get(map, index)->value);
  }

  return result;
}

} // namespace Map