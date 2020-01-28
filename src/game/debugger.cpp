#include "debugger.h"

#include "machine/disassembler.h"

#include "utils/types.h"
#include "utils/assert.h"

#include "sdl-state.h"

#include <SDL2/SDL.h>


namespace Debugger
{

using Machine::MemoryAddress;


void
init(Debugger& debugger, SDL_Renderer* renderer, Options::Args const& args)
{
  if (args.debugger)
  {
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, 640, 480);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 100, 100);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);

    Timer::init(debugger.timer, (u32)(1000000.0f/60.0f));
  }
}


void
destroy(Debugger& debugger, Options::Args const& args)
{
  if (args.debugger)
  {
    ImGuiSDL::Deinitialize();
    ImGui::DestroyContext();
  }
}


bool
advance(Debugger& debugger, SDL_Renderer* renderer, Options::Args const& args, Machine::Machine& machine, SDL_Texture* machine_texture, bool& stepping)
{
  bool step = false;

  if (args.debugger && Timer::check(debugger.timer))
  {
    MemoryAddress ni = Machine::get<MemoryAddress>(machine, Machine::Reserved::NI);
    // Disassembler::disassemble_instruction(machine, ni);

    ImGuiIO& io = ImGui::GetIO();

    int wheel = 0;

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_WINDOWEVENT)
      {
        if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
          io.DisplaySize.x = static_cast<float>(e.window.data1);
          io.DisplaySize.y = static_cast<float>(e.window.data2);
        }
      }
      else if (e.type == SDL_MOUSEWHEEL)
      {
        wheel = e.wheel.y;
      }
    }

    int mouseX, mouseY;
    u32 const buttons = SDL_GetMouseState(&mouseX, &mouseY);

    io.DeltaTime = 1.0f / 60.0f;
    io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
    io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
    io.MouseWheel = static_cast<float>(wheel);

    ImGui::NewFrame();
    ImGui::ShowDemoWindow();

    SDL_SetRenderDrawColor(renderer, 114, 144, 154, 255);
    SDL_RenderClear(renderer);

    debugger.mem_edit.DrawWindow("Memory Editor", machine.memory.bytes, machine.memory.size);

    if (ImGui::Begin("Stepper"))
    {
      ImGui::Checkbox("Stepping", &stepping);
      if (stepping)
      {
        step = ImGui::Button("Step");
      }
      float const dim = ImGui::GetWindowContentRegionWidth();
      ImGui::Image(machine_texture, ImVec2(dim, dim));
    }
    ImGui::End();

    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());

    SDL_RenderPresent(renderer);
  }

  return step;
}

} // namespace Debugger
