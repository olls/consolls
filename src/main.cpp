#include "game.h"
#include "options.h"
#include "assert.h"


s32
main(s32 argc, char const *argv[])
{
  bool success = true;

  bool (*game_run)(Options::Args args) = NULL;

#if LIVE_RELOAD

#else
  game_run = &(Game::run);
#endif

  assert(game_run);
  success &= game_run(Options::get_args(argc, argv));

  return success ? 0 : 1;
}