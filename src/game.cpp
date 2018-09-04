#include "game.h"

#include "sdl-state.h"
#include "assert.h"


namespace Game
{

bool
advance(State *state, SDL_State::SDL_State& sdl_state)
{
  bool success = true;

  FrameID::update(state->frame_id);
  Input::update(state->input, state->frame_id);

  if (state->input.quit)
  {
    success = false;
  }

  if (Input::up<Key::F11>(state->input, state->frame_id))
  {
    SDL_State::toggle_fullscreen(sdl_state);
  }

  Machine::advance(state->machine);

  u8* pixels = Machine::get_ptr<u8>(state->machine.memory, Machine::Reserved::ScreenBuffer);

  // Convert and copy pixels into state.texture

  static int n = 0;
  n += 67;
  Texture::set_pixel(state->texture, (n/state->texture.height)%state->texture.width, n%state->texture.height, (0xffd0043+n*437347%57342)*9);

  assert(SDL_State::render(sdl_state, state->texture));

  return success;
}


bool
run()
{
  bool success = true;

  State state = {};

  SDL_State::SDL_State sdl_state = {};
  success &= SDL_State::init(sdl_state, APP_NAME, 640, 480, SDL_PIXELFORMAT_RGBX8888);
  if (success)
  {
    success &= Texture::allocate(state.texture, 128, 128);
    if (success)
    {
      success &= SDL_State::set_render_texture(sdl_state, state.texture);
    }
  }

  if (success)
  {
    Input::init(state.input);

    state.frame_id = 0;

    bool running = true;
    while (running)
    {
      running &= advance(&state, sdl_state);
    }
  }

  SDL_State::destroy(sdl_state);
  return success;
}

} // namespace Game