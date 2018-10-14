#include "types.h"

#pragma once


namespace Options
{

struct Args
{
  bool debugger;
  bool client;
};


Args
get_args(s32 argc, char const *const argv[]);

} // namespace Options