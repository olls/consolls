#pragma once

#include "texture.h"
#include "types.h"
#include <SDL2/SDL.h>


namespace SDL_State
{

struct SDL_State
{
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  SDL_Texture *sdl_texture;
};


bool
init(SDL_State& sdl_state, const char *title, u32 initial_width, u32 intial_height);


bool
set_render_texture(SDL_State& sdl_state, Texture::Texture& texture);


bool
render(SDL_State& sdl_state, Texture::Texture& texture);


bool
set_fullscreen(SDL_State& sdl_state, bool on);


bool
is_fullscreen(SDL_State& sdl_state);


void
destroy(SDL_State& sdl_state);

} // namespace SDL_State