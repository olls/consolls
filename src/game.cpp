#include "game.h"

#include "machine-serialisation.h"
#include "debugger.h"
#include "compolls.h"
#include "basolls.h"
#include "disassembler.h"
#include "sdl-state.h"
#include "palette.h"
#include "instructions.h"
#include "clock.h"
#include "assert.h"


namespace Game
{

bool
advance(State *const state, SDL_State::SDL_State& sdl_state)
{
  bool success = true;
  bool advance_machine = !state->stepping;

  FrameID::update(state->frame_id);

  if (Timer::check(state->input_update) ||
      (advance_machine && Machine::consume_signal_register(state->machine, Machine::Reserved::PollInput)))
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

    if (state->step_mode)
    {
      if (Input::up<Key::P>(state->input, state->frame_id))
      {
        state->stepping = !state->stepping;
      }
      if (state->stepping && Input::up<Key::N>(state->input, state->frame_id))
      {
        advance_machine = true;
      }
    }
  }

  if (state->stepping && (state->frame_id == 1 || advance_machine))
  {
    // Display the next instruction

    Machine::MemoryAddress ni = Machine::get<Machine::MemoryAddress>(state->machine, Machine::Reserved::NI);
    Disassembler::disassemble_instruction(state->machine, ni);
    // Disassembler::disassemble(state->machine, Machine::Reserved::UserStart, Machine::Reserved::UserEnd);
  }

  if (advance_machine)
  {
    Machine::advance(state->machine);

    if (Machine::consume_signal_register(state->machine, Machine::Reserved::Blit))
    {
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
  }

  return success;
}


bool
run(Options::Args args)
{
  bool success = true;

  State state = {};
  state.step_mode = args.step;
  state.stepping = state.step_mode;

  SDL_State::SDL_State sdl_state = {};
  success &= SDL_State::init(sdl_state, APP_NAME, 640, 480, SDL_PIXELFORMAT_RGBX8888);

  Machine::set(state.machine, Machine::Reserved::ScreenBuffer, (Palette::Yellow|Palette::Brown<<4), state.machine.memory.screen_buffer_size);
  Machine::set<u8>(state.machine, Machine::Reserved::Blit, true);

  if (args.file != NULL)
  {
    success &= MachineSerialisation::deserialise(args.file, state.machine);
  }

  // success &= Basolls::load_os(state.machine);

  Debugger::init(args);

  if (success)
  {
    Input::init(state.input);
    Timer::init(state.input_update, 1000000*.2);

    state.frame_id = 0;

    bool running = true;
    while (running)
    {
      Debugger::advance(args, state.machine);

      running &= advance(&state, sdl_state);

      Clock::regulate(state.clock);
    }
  }

  Debugger::destroy(args);

  SDL_State::destroy(sdl_state);
  return success;
}

} // namespace Game