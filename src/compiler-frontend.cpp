#include "compolls.h"
#include "machine.h"
#include "file.h"
#include "assert.h"
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

    File::File file = {};
    if (!File::open(filename, &file))
    {
      success &= false;
      printf("Couldn't open file: %s\n", filename);
    }
    else
    {
      String::String file_string = { file.read_ptr, (u32)file.size };

      Machine::Machine machine = {};
      Machine::MemoryAddress addr = Machine::Reserved::UserStart;

      // Compolls::compile(S("add(123, [u8](u16 a, u8 stride){MemoryAddress a = [u8](u16 a, u8 stride){}}) MemoryAddress a = [u8](u16 a, u8 stride){}"), machine, addr);
      // Compolls::compile(S("add(1, 2)"), machine, addr);

      Basolls::Subroutine<void> result = {};
      if (!Compolls::compile(file_string, machine, addr, result))
      {
        success &= false;
        printf("\n\nCompilation error!\n");
      }
      else
      {
        printf("\n\nCompiled successfully!\n");
      }
    }
  }

  return success == true ? 0 : 1;
}