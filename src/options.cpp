#include "options.h"
#include <string.h>
#include <stdio.h>


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

    if ((strcmp(argv[i], "--file") == 0) ||
        (strcmp(argv[i], "-f") == 0))
    {
      ++i;
      if (i < argc)
      {
        args.file = argv[i];
      }
      else
      {
        printf("No filename provided after --file/-f argument; ignoring.\n");
      }
    }

    ++i;
  }

  return args;
}

} // namespace Options