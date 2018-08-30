#pragma once

#include "sdl-state.h"
#include "frame-id.h"
#include "types.h"
#include <SDL2/SDL.h>


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

  enum Key
  {
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
    Tick, Minus, Plus, Tab, BraceOpen, BraceClose, Colon, Apostrophe, Hash, Comma, FullStop, BackSlash, ForwardSlash,
    Escape, Backspace, Enter, Space,
    Insert, Delete, Home, End, PageUp, PageDown,
    CtrlLeft, CtrlRight, ShiftLeft, ShiftRight, AltLeft, AltRight,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

    N_KEYS
  };

  KeyState keys[N_KEYS];
};


// Have to leave the anonymous enum in Input struct so that it can be used for indexing the keys array without casting.
//   But want the definitions at the top level Input namespace so we can use Input::Key_X instead of Input::Input::Key_X.
using Key = Input::Key;


void
init(Input& input);


bool
is_pressed(KeyState& key);


bool
up(Input& input, Key key, FrameID::FrameID frame_id);


bool
down(Input& input, Key key, FrameID::FrameID frame_id);


void
update(Input& input, FrameID::FrameID frame_id);

} // namespace Input