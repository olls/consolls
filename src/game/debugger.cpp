#include "debugger.h"

#include "socket.h"

#include "machine/disassembler.h"

#include "utils/types.h"
#include "utils/assert.h"

#include <SDL2/SDL.h>
#include "sdl-state.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include "imgui.h"
#include "imgui_sdl.h"
#include "imgui_memory_editor.h"
#pragma clang diagnostic pop


namespace Debugger
{

using Machine::MemoryAddress;


void
init(SDL_State::SDL_State& sdl_state, Options::Args const& args)
{
  if (args.debugger)
  {
    sdl_state = {};

    bool success = SDL_State::init(sdl_state, APP_NAME, 640, 480);
    assert(success);

    ImGui::CreateContext();
    ImGuiSDL::Initialize(sdl_state.sdl_renderer, 640, 480);

    SDL_Texture* texture = SDL_CreateTexture(sdl_state.sdl_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 100, 100);
    {
      SDL_SetRenderTarget(sdl_state.sdl_renderer, texture);
      SDL_SetRenderDrawColor(sdl_state.sdl_renderer, 255, 0, 255, 255);
      SDL_RenderClear(sdl_state.sdl_renderer);
      SDL_SetRenderTarget(sdl_state.sdl_renderer, nullptr);
    }

    Socket::init(args.client);
  }
}


void
destroy(SDL_State::SDL_State& sdl_state, Options::Args const& args)
{
  if (args.debugger)
  {
    ImGuiSDL::Deinitialize();

    SDL_DestroyRenderer(sdl_state.sdl_renderer);
    SDL_DestroyWindow(sdl_state.sdl_window);

    ImGui::DestroyContext();

    Socket::destroy();
  }
}


void
advance(SDL_State::SDL_State& sdl_state, Options::Args const& args, Machine::Machine& machine)
{
  if (args.debugger)
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
    const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

    // Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)

    io.DeltaTime = 1.0f / 60.0f;
    io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
    io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
    io.MouseWheel = static_cast<float>(wheel);

    ImGui::NewFrame();
    ImGui::ShowDemoWindow();

    SDL_SetRenderDrawColor(sdl_state.sdl_renderer, 114, 144, 154, 255);
    SDL_RenderClear(sdl_state.sdl_renderer);

    static MemoryEditor mem_edit;
    mem_edit.DrawWindow("Memory Editor", machine.memory.bytes, machine.memory.size);

    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());

    SDL_RenderPresent(sdl_state.sdl_renderer);

    Socket::advance(args.client, machine);
  }
}

} // namespace Debugger
