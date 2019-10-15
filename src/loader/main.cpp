#include "options.h"

#if !LIVE_RELOAD
#include "game/game.h"
#endif

#include "utils/assert.h"
#include "utils/types.h"


s32
main(s32 argc, char const *const argv[])
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
