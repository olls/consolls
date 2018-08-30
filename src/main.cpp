#include "sdl-state.h"
#include "game-loop.h"
#include "types.h"


s32
main(s32 argc, char const *argv[])
{
  bool success = true;

  SDL_State::SDL_State sdl_state = {};
  success &= SDL_State::init(sdl_state, APP_NAME, 640, 480);

  if (success)
  {
    success &= GameLoop::game_loop(sdl_state);
  }

  SDL_State::destroy(sdl_state);
  return success ? 0 : 1;
}