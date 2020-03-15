#include "sdl-events.h"

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_events.h>


namespace SDL_Events
{

bool
advance(Input::Input& input, FrameID::FrameID frame_id)
{
  bool quit = false;
  
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      case (SDL_QUIT):
      {
        quit = true;
      } break;
      case (SDL_KEYDOWN):
      case (SDL_KEYUP):
      {
        for (Key key_index = Key::START;
             key_index < Key::COUNT;
             ++key_index)
        {
          Input::update_key_state(input.keys[key_index], &event, frame_id);
        }
      } break;
    }
  }
  
  return quit;
}

}  // namespace SDL_Events
