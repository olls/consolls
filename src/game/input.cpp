#include "input.h"

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_events.h>


namespace Input
{

void
init(Input& input)
{
  input.keys[Key::A].sdl_key_code = SDLK_a;
  input.keys[Key::B].sdl_key_code = SDLK_b;
  input.keys[Key::C].sdl_key_code = SDLK_c;
  input.keys[Key::D].sdl_key_code = SDLK_d;
  input.keys[Key::E].sdl_key_code = SDLK_e;
  input.keys[Key::F].sdl_key_code = SDLK_f;
  input.keys[Key::G].sdl_key_code = SDLK_g;
  input.keys[Key::H].sdl_key_code = SDLK_h;
  input.keys[Key::I].sdl_key_code = SDLK_i;
  input.keys[Key::J].sdl_key_code = SDLK_j;
  input.keys[Key::K].sdl_key_code = SDLK_k;
  input.keys[Key::L].sdl_key_code = SDLK_l;
  input.keys[Key::M].sdl_key_code = SDLK_m;

  input.keys[Key::N].sdl_key_code = SDLK_n;
  input.keys[Key::O].sdl_key_code = SDLK_o;
  input.keys[Key::P].sdl_key_code = SDLK_p;
  input.keys[Key::Q].sdl_key_code = SDLK_q;
  input.keys[Key::R].sdl_key_code = SDLK_r;
  input.keys[Key::S].sdl_key_code = SDLK_s;
  input.keys[Key::T].sdl_key_code = SDLK_t;
  input.keys[Key::U].sdl_key_code = SDLK_u;
  input.keys[Key::V].sdl_key_code = SDLK_v;
  input.keys[Key::W].sdl_key_code = SDLK_w;
  input.keys[Key::X].sdl_key_code = SDLK_x;
  input.keys[Key::Y].sdl_key_code = SDLK_y;
  input.keys[Key::Z].sdl_key_code = SDLK_z;

  input.keys[Key::_0].sdl_key_code = SDLK_0;
  input.keys[Key::_1].sdl_key_code = SDLK_1;
  input.keys[Key::_2].sdl_key_code = SDLK_2;
  input.keys[Key::_3].sdl_key_code = SDLK_3;
  input.keys[Key::_4].sdl_key_code = SDLK_4;
  input.keys[Key::_5].sdl_key_code = SDLK_5;
  input.keys[Key::_6].sdl_key_code = SDLK_6;
  input.keys[Key::_7].sdl_key_code = SDLK_7;
  input.keys[Key::_8].sdl_key_code = SDLK_8;
  input.keys[Key::_9].sdl_key_code = SDLK_9;

  input.keys[Key::Tick      ].sdl_key_code = SDLK_BACKQUOTE;
  input.keys[Key::Minus     ].sdl_key_code = SDLK_MINUS;
  input.keys[Key::Plus      ].sdl_key_code = SDLK_PLUS;
  input.keys[Key::Tab       ].sdl_key_code = SDLK_TAB;
  input.keys[Key::BraceOpen ].sdl_key_code = SDLK_LEFTBRACKET;
  input.keys[Key::BraceClose].sdl_key_code = SDLK_RIGHTBRACKET;
  input.keys[Key::Colon     ].sdl_key_code = SDLK_COLON;

  input.keys[Key::Apostrophe  ].sdl_key_code = SDLK_QUOTE;
  input.keys[Key::Hash        ].sdl_key_code = SDLK_HASH;
  input.keys[Key::Comma       ].sdl_key_code = SDLK_COMMA;
  input.keys[Key::FullStop    ].sdl_key_code = SDLK_PERIOD;
  input.keys[Key::BackSlash   ].sdl_key_code = SDLK_BACKSLASH;
  input.keys[Key::ForwardSlash].sdl_key_code = SDLK_SLASH;

  input.keys[Key::Escape   ].sdl_key_code = SDLK_ESCAPE;
  input.keys[Key::Backspace].sdl_key_code = SDLK_BACKSPACE;
  input.keys[Key::Enter    ].sdl_key_code = SDLK_RETURN;
  input.keys[Key::Space    ].sdl_key_code = SDLK_SPACE;

  input.keys[Key::Insert].sdl_key_code = SDLK_INSERT;
  input.keys[Key::Delete].sdl_key_code = SDLK_DELETE;

  input.keys[Key::Home].sdl_key_code = SDLK_HOME;
  input.keys[Key::End ].sdl_key_code = SDLK_END;

  input.keys[Key::PageUp  ].sdl_key_code = SDLK_PAGEUP;
  input.keys[Key::PageDown].sdl_key_code = SDLK_PAGEDOWN;

  input.keys[Key::CtrlLeft ].sdl_key_code = SDLK_LCTRL;
  input.keys[Key::CtrlRight].sdl_key_code = SDLK_RCTRL;

  input.keys[Key::ShiftLeft ].sdl_key_code = SDLK_LSHIFT;
  input.keys[Key::ShiftRight].sdl_key_code = SDLK_RSHIFT;

  input.keys[Key::AltLeft ].sdl_key_code = SDLK_LALT;
  input.keys[Key::AltRight].sdl_key_code = SDLK_RALT;

  input.keys[Key::F1 ].sdl_key_code = SDLK_F1;
  input.keys[Key::F2 ].sdl_key_code = SDLK_F2;
  input.keys[Key::F3 ].sdl_key_code = SDLK_F3;
  input.keys[Key::F4 ].sdl_key_code = SDLK_F4;
  input.keys[Key::F5 ].sdl_key_code = SDLK_F5;
  input.keys[Key::F6 ].sdl_key_code = SDLK_F6;
  input.keys[Key::F7 ].sdl_key_code = SDLK_F7;
  input.keys[Key::F8 ].sdl_key_code = SDLK_F8;
  input.keys[Key::F9 ].sdl_key_code = SDLK_F9;
  input.keys[Key::F10].sdl_key_code = SDLK_F10;
  input.keys[Key::F11].sdl_key_code = SDLK_F11;
  input.keys[Key::F12].sdl_key_code = SDLK_F12;
}


void
update_key_state(KeyState& key, SDL_Event const* key_event, FrameID::FrameID frame_id)
{
  if (key_event->key.keysym.sym == key.sdl_key_code)
  {
    if (key_event->type == SDL_KEYDOWN)
    {
      key.down_at = frame_id;
    }
    if (key_event->type == SDL_KEYUP)
    {
      key.up_at = frame_id;
    }
  }
}

} // namespace Input
