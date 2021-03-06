#include "sdl-state.h"

#include "utils/assert.h"
#include "utils/debug.h"

#include <SDL2/SDL.h>


namespace SDL_State
{

bool
init(SDL_State& sdl_state, char const* title, s32 initial_width, s32 initial_height)
{
  bool success = true;

  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
  {
    printf("Failed to init SDL: %s\n", SDL_GetError());
    success &= false;
  }
  else
  {
    sdl_state.sdl_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, initial_width, initial_height, SDL_WINDOW_RESIZABLE);

    if (sdl_state.sdl_window == NULL)
    {
      printf("Failed to create SDL_Window: %s\n", SDL_GetError());
      success &= false;
    }
    else
    {
      sdl_state.sdl_renderer = SDL_CreateRenderer(sdl_state.sdl_window, -1, SDL_RENDERER_PRESENTVSYNC);
      if (sdl_state.sdl_renderer == NULL)
      {
        printf("Failed to create SDL_Renderer: %s\n", SDL_GetError());
        success &= false;
      }
    }

    success &= SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
  }

  return success;
}


bool
set_render_texture(SDL_State& sdl_state, Game::TextureType const& texture)
{
  bool success = true;

  // TODO: Will need to do more work here to support different formats.
  u32 const pixel_format = SDL_PIXELFORMAT_RGBX8888;
  assert(sizeof(Game::TextureType::Pixel) == SDL_BYTESPERPIXEL(pixel_format));

  if (0 != SDL_RenderSetLogicalSize(sdl_state.sdl_renderer, texture.width, texture.height))
  {
    printf("Failed to set renderer logical size: %s\n", SDL_GetError());
    success &= false;
  }

  if (sdl_state.sdl_texture != NULL)
  {
    SDL_DestroyTexture(sdl_state.sdl_texture);
    sdl_state.sdl_texture = NULL;
  }

  sdl_state.sdl_texture = SDL_CreateTexture(sdl_state.sdl_renderer, pixel_format, SDL_TEXTUREACCESS_STREAMING, texture.width, texture.height);
  if (sdl_state.sdl_texture == NULL)
  {
    printf("Failed to create SDL_Texture: %s\n", SDL_GetError());
    success &= false;
  }

  return success;
}


bool
render(SDL_State const& sdl_state, Game::TextureType const& texture)
{
  bool success = true;

  success &= 0 == SDL_UpdateTexture(sdl_state.sdl_texture, NULL, texture.pixels, texture.width * (s32)sizeof(Game::TextureType::Pixel));
  success &= 0 == SDL_SetRenderDrawColor(sdl_state.sdl_renderer, 0x00, 0x00, 0x00, 0xff);
  success &= 0 == SDL_RenderClear(sdl_state.sdl_renderer);
  success &= 0 == SDL_RenderCopy(sdl_state.sdl_renderer, sdl_state.sdl_texture, NULL, NULL);
  if (success)
  {
    SDL_RenderPresent(sdl_state.sdl_renderer);
  }

  return success;
}


bool
set_fullscreen(SDL_State const& sdl_state, bool on)
{
  bool success = true;
  success &= 0 == SDL_SetWindowFullscreen(sdl_state.sdl_window, on ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
  return success;
}


bool
is_fullscreen(SDL_State const& sdl_state)
{
  bool result;
  result = SDL_WINDOW_FULLSCREEN_DESKTOP & SDL_GetWindowFlags(sdl_state.sdl_window);
  return result;
}


void
destroy(SDL_State& sdl_state)
{
  SDL_DestroyWindow(sdl_state.sdl_window);  sdl_state.sdl_window = NULL;
  SDL_DestroyTexture(sdl_state.sdl_texture);  sdl_state.sdl_texture = NULL;
  SDL_DestroyRenderer(sdl_state.sdl_renderer);  sdl_state.sdl_renderer = NULL;
  SDL_Quit();
}

} // namespace SDL_State
