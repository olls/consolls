#include "game.h"

#include "sdl-state.h"
#include "debugger.h"

#include "machine/machine-serialisation.h"
#include "machine/basolls.h"
#include "machine/disassembler.h"
#include "machine/instructions.h"

#include "utils/assert.h"


namespace Game
{

bool
advance(State* const state, SDL_State::SDL_State& sdl_state)
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
        texture &= Texture::allocate<Palette::Colour>(state->texture, state->machine.memory.sb_width, state->machine.memory.sb_height);
        if (texture)
        {
          texture &= SDL_State::set_render_texture(sdl_state, state->texture);
          if (!texture)
          {
            Texture::destroy(state->texture);
          }
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
  success &= SDL_State::init(sdl_state, APP_NAME, 640, 480);

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
    Timer::init(state.input_update, (u32)(1000000.0f*.2f));

    state.frame_id = 0;

    bool running = true;
    while (running)
    {
      Debugger::advance(args, state.machine);

      running &= advance(&state, sdl_state);

      Clock::regulate(state.clock);
    }
  }

  if (state.texture.pixels)
  {
    Texture::destroy(state.texture);
  }

  Debugger::destroy(args);

  SDL_State::destroy(sdl_state);
  return success;
}

} // namespace Game
