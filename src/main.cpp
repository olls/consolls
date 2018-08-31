#include "game.h"
#include "assert.h"


s32
main(s32 argc, char const *argv[])
{
  bool success = true;

  bool (*game_run)() = NULL;

#if LIVE_RELOAD

#else
  game_run = &(Game::run);
#endif

  assert(game_run);
  success &= game_run();

  return success ? 0 : 1;
}