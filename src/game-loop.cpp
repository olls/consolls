#include "game-loop.h"

#include "texture.h"
#include "input.h"
#include "assert.h"


namespace GameLoop
{

bool
game_loop(SDL_State::SDL_State& sdl_state)
{
  bool success = true;

  const s32 width = 128;
  const s32 height = 128;

  Texture::Texture texture = {};
  success &= Texture::init(texture, width, height);
  SDL_State::set_render_texture(sdl_state, texture);

  Input::Input input = {};
  Input::init(input);

  FrameID::FrameID frame_id = 0;

  bool running = true;
  while (running)
  {
    FrameID::update(frame_id);
    Input::update(input, frame_id);

    if (input.quit)
    {
      running = false;
    }
    if (Input::up<Key::F11>(input, frame_id))
    {
      SDL_State::set_fullscreen(sdl_state, !SDL_State::is_fullscreen(sdl_state));
    }

    static int n = 0;
    n += 67;
    Texture::set_pixel(texture, (n/texture.height)%texture.width, n%texture.height, (0xffd0043+n*437347%57342)*9);

    assert(SDL_State::render(sdl_state, texture));
  }

  Texture::destroy(texture);
  return success;
}

} // namespace GameLoop