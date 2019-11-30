#pragma once

#include "loader/options.h"

#include "machine/machine.h"

#include "sdl-state.h"


namespace Debugger
{

void
init(SDL_State::SDL_State& sdl_state, Options::Args const& args);


void
destroy(SDL_State::SDL_State& sdl_state, Options::Args const& args);


void
advance(SDL_State::SDL_State& sdl_state, Options::Args const& args, Machine::Machine& machine);

} // namespace Debugger
