#pragma once

#include "sdl-state.h"
#include "types.h"


namespace GameLoop
{

bool
game_loop(SDL_State::SDL_State& sdl_state);

} // namespace GameLoop