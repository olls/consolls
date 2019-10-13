#pragma once

#include "utils/types.h"


namespace Options
{

struct Args
{
  bool debugger;
  bool step;
  bool client;
  char const* file;
};


Args
get_args(s32 argc, char const *const argv[]);

} // namespace Options