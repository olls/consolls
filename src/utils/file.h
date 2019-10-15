#pragma once

#include "utils/types.h"


namespace File
{

struct File
{
  size_t size;

  char const * read_ptr;

  /// @brief Pointer to writeable file-data, this pointer is only set if `write` is set to true when
  ///          opening the file.
  char* write_ptr;

  s32 fd;
};


bool
open(char const * filename, File* result, bool write, bool trunc, size_t trunc_to);

inline
bool
open(char const * filename, File *result, bool write, size_t trunc_to)
{
  return open(filename, result, write, true, trunc_to);
}

inline
bool
open(char const * filename, File *result, bool write)
{
  return open(filename, result, write, false, 0);
}

inline
bool
open(char const * filename, File *result, size_t trunc_to)
{
  return open(filename, result, false, true, trunc_to);
}

inline
bool
open(char const * filename, File *result)
{
  return open(filename, result, false, false, 0);
}


bool
close(File* file, bool trunc, size_t trunc_to);

inline
bool
close(File* file, size_t trunc_to)
{
  return close(file, true, trunc_to);
}

inline
bool
close(File* file)
{
  return close(file, false, 0);
}

} // namespace File
