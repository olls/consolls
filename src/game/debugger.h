#pragma once

#include "loader/options.h"

#include "machine/machine.h"

#include "sdl-state.h"
#include "timer.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include "imgui.h"
#include "imgui_sdl.h"
#include "imgui_memory_editor.h"
#pragma clang diagnostic pop


namespace Debugger
{

struct Debugger
{
	SDL_State::SDL_State sdl_state;
  Timer::Timer timer;
	MemoryEditor mem_edit;
};


void
init(Debugger& debugger, Options::Args const& args);


void
destroy(Debugger& debugger, Options::Args const& args);


void
advance(Debugger& debugger, Options::Args const& args, Machine::Machine& machine);

} // namespace Debugger
