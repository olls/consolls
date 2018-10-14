#pragma once

#include "sdl-state.h"
#include "frame-id.h"
#include "types.h"
#include <SDL2/SDL.h>


namespace Key_Hidden
{
// Need Key enum to be anonymous for it to be used as array indices.
// Typedef needed so that we can reference its type for function args...
// Namespace needed so all the Keys are not polluting the global scope!
// using Key_Hidden::Key needed to hide the namespace

typedef enum
{
  A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
  _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
  Tick, Minus, Plus, Tab, BraceOpen, BraceClose, Colon, Apostrophe, Hash, Comma, FullStop, BackSlash, ForwardSlash,
  Escape, Backspace, Enter, Space,
  Insert, Delete, Home, End, PageUp, PageDown,
  CtrlLeft, CtrlRight, ShiftLeft, ShiftRight, AltLeft, AltRight,
  F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

  N_KEYS
} Key;

} // namespace Key_Hidden

using Key_Hidden::Key;


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

  KeyState keys[Key::N_KEYS];
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