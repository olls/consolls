#pragma once

#include "loader/options.h"

#include "machine/machine.h"

#include "utils/assert.h"

#include "sdl-state.h"
#include "timer.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#define IM_ASSERT assert
#include "imgui.h"
#include "imgui_sdl.h"
#include "imgui_memory_editor.h"
#pragma clang diagnostic pop


namespace Debugger
{

struct Debugger
{
  Timer::Timer timer;
  MemoryEditor mem_edit;
};


void
init(Debugger& debugger, SDL_Renderer* renderer, Options::Args const& args);


void
destroy(Debugger& debugger, Options::Args const& args);


bool
advance(Debugger& debugger, SDL_Renderer* renderer, Options::Args const& args, Machine::Machine& machine, SDL_Texture* machine_texture, bool& stepping);

} // namespace Debugger
