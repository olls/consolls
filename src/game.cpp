#include "game.h"

namespace Game
{

bool
run()
{
  bool success = true;

  SDL_State::SDL_State sdl_state = {};
  success &= SDL_State::init(sdl_state, APP_NAME, 640, 480);

  if (success)
  {
    success &= GameLoop::game_loop(sdl_state);
  }

  SDL_State::destroy(sdl_state);
  return success;
}

} // namespace Game