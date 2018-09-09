#include "game.h"

#include "sdl-state.h"
#include "palette.h"
#include "instructions.h"
#include "assert.h"

#define ARRAY_LEN(a) (sizeof(a)/sizeof((a)[0]))


namespace Game
{

void
load_program(Machine::Machine& machine)
{
  Machine::MemoryAddress addr = Machine::Reserved::UserStart;

  Machine::MemoryAddress vars = 0x100;
  Machine::MemoryAddress stride = vars; vars += 2;
  Machine::MemoryAddress colour = vars; vars += 1;
  Machine::MemoryAddress colour_a = vars; vars += 2;
  Machine::MemoryAddress counter = vars; vars += 2;
  Machine::MemoryAddress pixel_pos = vars; vars += 2;
  Machine::MemoryAddress offset = vars; vars += 2;

  Machine::advance_addr<Instructions::Code>(machine, addr) = Instructions::Code::SET_W;
  Machine::advance_addr<Instructions::SET<u16>>(machine, addr) = {
    .addr = stride,
    .value = 71
  };

  Machine::advance_addr<Instructions::Code>(machine, addr) = Instructions::Code::SET;
  Machine::advance_addr<Instructions::SET<u8>>(machine, addr) = {
    .addr = colour,
    .value = Palette::Red
  };

  Machine::advance_addr<Instructions::Code>(machine, addr) = Instructions::Code::SET_W;
  Machine::advance_addr<Instructions::SET<u16>>(machine, addr) = {
    .addr = colour_a,
    .value = colour
  };

  Machine::advance_addr<Instructions::Code>(machine, addr) = Instructions::Code::SET_W;
  Machine::advance_addr<Instructions::SET<u16>>(machine, addr) = {
    .addr = offset,
    .value = Machine::Reserved::ScreenBuffer
  };

  Machine::advance_addr<Instructions::Code>(machine, addr) = Instructions::Code::SET_W;
  Machine::advance_addr<Instructions::SET<u16>>(machine, addr) = {
    .addr = counter,
    .value = 0
  };

  Machine::MemoryAddress loop = addr;

  Machine::advance_addr<Instructions::Code>(machine, addr) = Instructions::Code::ADD_W;
  Machine::advance_addr<Instructions::ADD>(machine, addr) = {
    .a = counter,
    .b = offset,
    .result = pixel_pos
  };

  Machine::advance_addr<Instructions::Code>(machine, addr) = Instructions::Code::COPY;
  Machine::advance_addr<Instructions::COPY>(machine, addr) = {
    .from = colour_a,
    .to = pixel_pos
  };

  Machine::advance_addr<Instructions::Code>(machine, addr) = Instructions::Code::ADD_W;
  Machine::advance_addr<Instructions::ADD>(machine, addr) = {
    .a = counter,
    .b = stride,
    .result = counter
  };

  Machine::advance_addr<Instructions::Code>(machine, addr) = Instructions::Code::CMP_W;
  Machine::advance_addr<Instructions::CMP>(machine, addr) = {
    .a = offset,
    .b = counter,
    .addr = loop
  };

  Machine::advance_addr<Instructions::Code>(machine, addr) = Instructions::Code::SUB_W;
  Machine::advance_addr<Instructions::SUB>(machine, addr) = {
    .a = counter,
    .b = offset,
    .result = counter
  };

  Machine::advance_addr<Instructions::Code>(machine, addr) = Instructions::Code::JUMP;
  Machine::advance_addr<Instructions::JUMP>(machine, addr) = {
    .addr = loop
  };

  Machine::set<Machine::MemoryAddress>(machine, Machine::Reserved::NI, Machine::Reserved::UserStart);
}


bool
advance(State *state, SDL_State::SDL_State& sdl_state)
{
  bool success = true;

  FrameID::update(state->frame_id);
  Input::update(state->input, state->frame_id);

  if (state->input.quit)
  {
    success = false;
  }

  if (Input::up<Key::F11>(state->input, state->frame_id))
  {
    SDL_State::toggle_fullscreen(sdl_state);
  }

  Machine::advance(state->machine);

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
    success &= false;
  }
  else
  {
    Machine::output_screen_buffer(state->machine, state->texture);
    assert(SDL_State::render(sdl_state, state->texture));
  }

  return success;
}


bool
run()
{
  bool success = true;

  State state = {};

  SDL_State::SDL_State sdl_state = {};
  success &= SDL_State::init(sdl_state, APP_NAME, 640, 480, SDL_PIXELFORMAT_RGBX8888);

  if (success)
  {
    Input::init(state.input);

    state.frame_id = 0;

    load_program(state.machine);

    bool running = true;
    while (running)
    {
      running &= advance(&state, sdl_state);
    }
  }

  SDL_State::destroy(sdl_state);
  return success;
}

} // namespace Game