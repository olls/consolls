#pragma once

#include "sdl-state.h"
#include "frame-id.h"
#include "enum-array.h"
#include "types.h"
#include <SDL2/SDL.h>


enum class Key
{
  A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
  _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
  Tick, Minus, Plus, Tab, BraceOpen, BraceClose, Colon, Apostrophe, Hash, Comma, FullStop, BackSlash, ForwardSlash,
  Escape, Backspace, Enter, Space,
  Insert, Delete, Home, End, PageUp, PageDown,
  CtrlLeft, CtrlRight, ShiftLeft, ShiftRight, AltLeft, AltRight,
  F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

  COUNT,
  START = A
};


inline
Key
operator++(Key& key)
{
  key = (Key)((u32)key + 1);
  assert(key <= Key::COUNT);
  return key;
}


namespace Input
{

struct KeyState
{
  SDL_Keycode sdl_key_code;
  FrameID::FrameID up_at;
  FrameID::FrameID down_at;
};


struct Input
{
  bool quit;

  EnumArray::EnumArray<Key, KeyState> keys;
};


void
init(Input& input);


template <Key key>
bool
is_pressed(Input const & input)
{
  bool result = (input.keys[key].up_at < input.keys[key].down_at);
  return result;
}


template <Key key>
bool
up(Input const & input, FrameID::FrameID frame_id)
{
  bool result = (input.keys[key].up_at == frame_id);
  return result;
}


template <Key key>
bool
down(Input const & input, FrameID::FrameID frame_id)
{
  bool result = (input.keys[key].down_at == frame_id);
  return result;
}


void
update(Input& input, FrameID::FrameID frame_id);

} // namespace Input