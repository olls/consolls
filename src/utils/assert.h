#pragma once

#include "utils/debug.h"

#define trap() __asm__ __volatile__ ("int $3\n\t")

#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

#define assert(cond) \
  { \
    if (!(cond)) \
    { \
      printf("assert(" #cond ") failed at " __FILE__ ":" S__LINE__ "\n"); \
      trap(); \
    } \
  }

namespace Assert
{

} // namespace Assert