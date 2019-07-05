#include "compolls.h"
#include "machine.h"
#include "string.h"
#include "types.h"


s32
main(s32 argc, char const * argv[])
{
  bool success = true;

  char const* program = NULL;
  char const* filename = NULL;
  char const* output_filename = NULL;

  u32 arg_index = 0;
  if (arg_index < argc)
  {
    program = argv[arg_index];
    arg_index += 1;
  }
  else
  {
    printf("Not enough arguments passed in.\n");
    success &= false;
  }

  if (arg_index < argc)
  {
    filename = argv[arg_index];
    arg_index += 1;
  }
  else
  {
    success &= false;
    printf("Argument 1 must be Compolls source filename.\n");
  }

  for (;
       arg_index < argc;
       ++arg_index)
  {
    if (strcmp(argv[arg_index], "-o") == 0)
    {
      if (arg_index + 1 < argc)
      {
        arg_index += 1;
        output_filename = argv[arg_index];
      }
      else
      {
        printf("-o specified without no filename following\n");
        success &= false;
      }
    }
  }

  if (success)
  {
    Machine::Machine machine = {};
    Machine::MemoryAddress addr = Machine::Reserved::UserStart;

    Machine::MemoryAddress result;
    String::String error_msg = {};
    if (!Compolls::compile_file(filename, machine, addr, result, &error_msg))
    {
      success &= false;
      printf("\n\nCompilation error!\n");
    }
    else
    {
      printf("\n\nCompiled successfully!\n");
    }
    printf("%.*s", print_s(error_msg));
  }

  return success == true ? 0 : 1;
}