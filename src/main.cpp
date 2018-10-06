#include "game.h"
#include "assert.h"
#include <cstring>


s32
main(s32 argc, char const *argv[])
{
  bool success = true;

  bool (*game_run)(bool debugger) = NULL;

#if LIVE_RELOAD

#else
  game_run = &(Game::run);
#endif

  bool debugger = false;

  if (argc > 1 && strcmp(argv[1], "--debugger") == 0)
  {
    debugger = true;
  }

  assert(game_run);
  success &= game_run(debugger);

  return success ? 0 : 1;
}