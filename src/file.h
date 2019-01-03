#pragma once

#include "types.h"


namespace File
{

struct File
{
  s32 fd;

  char const * read_ptr;

  /// @brief Pointer to writeable file-data, this pointer is only set if `write` is set to true when
  ///          opening the file.
  char* write_ptr;

  s32 size;
};


bool
open(char const * filename, File* result, bool write = false, s32 trunc_to = -1);


bool
close(File* file, s32 trunc_to = -1);

} // namespace File