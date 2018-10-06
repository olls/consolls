#include "options.h"
#include <cstring>


namespace Options
{

Args
get_args(s32 argc, char const *argv[])
{
  Args args = {};
  s32 i = 1;

  while (i < argc)
  {
    if (strcmp(argv[i], "--debugger") == 0)
    {
      args.debugger = true;
    }

    if (strcmp(argv[i], "--client") == 0)
    {
      args.client = true;
    }

    ++i;
  }

  return args;
}

} // namespace Options