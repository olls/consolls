#pragma once

#include "utils/array.h"
#include "utils/types.h"
#include "utils/pair.h"


namespace Map
{

template <typename Key, typename Value>
using Map = Array::Array<Pair<Key, Value>>;


template <typename Key, typename Value>
s32
find_index(Map<Key, Value> const& map, Key key)
{
  using PairT = Pair<Key, Value>;

  Array::FindFirstFunc<PairT> find_func = [](PairT const& pair, void* user){ return pair.key == *(Key*)user; };
  s32 index = Array::find_first(map, find_func, &key);

  return index;
}


template <typename Key, typename Value>
Value const*
find(Map<Key, Value> const& map, Key key)
{
  using PairT = Pair<Key, Value>;

  Value const* result = NULL;

  Array::FindFirstFunc<PairT> find_func = [](PairT const& pair, void* user){ return pair.key == *(Key*)user; };
  s32 index = Array::find_first(map, find_func, &key);
  if (index != -1)
  {
    result = &(Array::get(map, index)->value);
  }

  return result;
}

template <typename Key, typename Value>
Value*
find(Map<Key, Value>& map, Key key)
{
  Value* result = NULL;

  s32 index = find_index(map, key);
  if (index != -1)
  {
    result = &(Array::get(map, index)->value);
  }

  return result;
}


// NOTE:  Untested!
template <typename Key, typename Value>
Value&
ensure(Map<Key, Value> const& map, Key key, Value value)
{
  Value* result = find(map, key);
  if (result)
  {
    result->value = value;
  }
  else
  {
    result = Array::add(map, {key, value});
  }
  assert(value != NULL);
  return *value;
}


template <typename Key, typename Value>
bool
erase(Map<Key, Value>& map, Key key)
{
  using PairT = Pair<Key, Value>;

  Array::FindFirstFunc<PairT> find_func = [](PairT const& pair, void* user){ return pair.key == *(Key*)user; };
  s32 index = Array::find_first(map, find_func, &key);
  if (index != -1)
  {
    Array::remove(map, index);
    return true;
  }
  else
  {
    return false;
  }
}

} // namespace Map
