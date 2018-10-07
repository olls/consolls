#include "types.h"

#pragma once


namespace Options
{

struct Args
{
  bool debugger = false;
  bool client = false;
};


Args
get_args(s32 argc, char const *argv[]);

} // namespace Options