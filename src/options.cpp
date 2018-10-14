#include "options.h"
#include <cstring>


namespace Options
{

Args
get_args(s32 argc, char const *const argv[])
{
  Args args = {};
  s32 i = 1;

  while (i < argc)
  {
    args.debugger &= (strcmp(argv[i], "--debugger") == 0);
    args.client &= (strcmp(argv[i], "--client") == 0);

    ++i;
  }

  return args;
}

} // namespace Options