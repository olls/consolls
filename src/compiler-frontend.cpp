#include "compolls.h"
#include "machine.h"
#include "string.h"
#include "types.h"


s32
main(s32 argc, char const * argv[])
{
  bool success = true;

  if (argc != 2)
  {
    success &= false;
    printf("Argument 1 must be compolls source filename.\n");
  }
  else
  {
    char const * filename = argv[1];

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