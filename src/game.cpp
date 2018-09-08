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

  #define Instruction (Machine::advance_addr<Instructions::Code>(machine, addr))
  #define Wide(a) (Machine::advance_addr<u16>(machine, addr) = (a))
  #define Value(v) (Machine::advance_addr<u8>(machine, addr) = (v))

  Machine::MemoryAddress vars = 0x100;
  Machine::MemoryAddress stride = vars; vars += 2;
  Machine::MemoryAddress colour = vars; vars += 1;
  Machine::MemoryAddress colour_a = vars; vars += 2;
  Machine::MemoryAddress counter = vars; vars += 2;
  Machine::MemoryAddress pixel_pos = vars; vars += 2;
  Machine::MemoryAddress offset = vars; vars += 2;

  Instruction = Instructions::Code::SET_W;
    Wide(stride);
    Wide(71);

  Instruction = Instructions::Code::SET;
    Wide(colour);
    Value(Palette::Red);

  Instruction = Instructions::Code::SET_W;
    Wide(colour_a);
    Wide(colour);

  Instruction = Instructions::Code::SET_W;
    Wide(offset);
    Wide(Machine::Reserved::ScreenBuffer);

  Instruction = Instructions::Code::SET_W;
    Wide(counter);
    Wide(0);

  Machine::MemoryAddress loop = addr;

  Instruction = Instructions::Code::ADD_W;
    Wide(counter);
    Wide(offset);
    Wide(pixel_pos);

  Instruction = Instructions::Code::COPY;
    Wide(colour_a);
    Wide(pixel_pos);

  Instruction = Instructions::Code::ADD_W;
    Wide(counter);
    Wide(stride);
    Wide(counter);

  Instruction = Instructions::Code::CMP_W;
    Wide(offset);
    Wide(counter);
    Wide(loop);

  Instruction = Instructions::Code::SUB_W;
    Wide(counter);
    Wide(offset);
    Wide(counter);

  Instruction = Instructions::Code::JUMP;
    Wide(loop);

  #undef Instruction
  #undef Address
  #undef Value

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