#include "game.h"

#include "debugger.h"
#include "basolls.h"
#include "sdl-state.h"
#include "palette.h"
#include "instructions.h"
#include "clock.h"
#include "assert.h"


namespace Game
{

static bool blit = false;

bool
advance(State *const state, SDL_State::SDL_State& sdl_state)
{
  bool success = true;

  FrameID::update(state->frame_id);

  if (Timer::check(state->input_update) ||
      Machine::consume_signal_register(state->machine, Machine::Reserved::PollInput))
  {
    Input::update(state->input, state->frame_id);

    if (state->input.quit)
    {
      success = false;
    }

    if (Input::up<Key::F11>(state->input, state->frame_id))
    {
      SDL_State::toggle_fullscreen(sdl_state);
    }
  }

  Machine::advance(state->machine);

  if (Machine::consume_signal_register(state->machine, Machine::Reserved::Blit))
  {
    blit = true;
    bool texture = true;
    if (!state->texture.pixels)
    {
      texture &= Machine::allocate_screen_buffer_texture(state->machine.memory, state->texture);
      if (texture)
      {
        texture &= SDL_State::set_render_texture(sdl_state, state->texture);
      }
    }

    if (!texture)
    {
      state->texture.pixels = 0;
      success &= false;
    }
    else
    {
      Machine::output_screen_buffer(state->machine, state->texture);
      success &= SDL_State::render(sdl_state, state->texture);
    }
  }
  else
  {
    blit = false;
  }

  return success;
}


bool
run(Options::Args args)
{
  bool success = true;

  State state = {};

  SDL_State::SDL_State sdl_state = {};
  success &= SDL_State::init(sdl_state, APP_NAME, 640, 480, SDL_PIXELFORMAT_RGBX8888);

  success &= Basolls::load_os(state.machine);

  Debugger::init(args);

  if (success)
  {
    Input::init(state.input);
    Timer::init(state.input_update, 1000000*.2);

    state.frame_id = 0;

    bool running = true;
    while (running)
    {
      Debugger::advance(args, state.machine, blit);

      running &= advance(&state, sdl_state);

      Clock::regulate(state.clock);
    }
  }

  Debugger::destroy(args);

  Clock::print_report(state.clock);

  SDL_State::destroy(sdl_state);
  return success;
}

} // namespace Game