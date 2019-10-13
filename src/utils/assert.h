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
      /* __PRETTY_FUNCTION__ is a normal char[] not a prepocessor macro */ \
      printf(__PRETTY_FUNCTION__); \
      printf(": assert(" #cond ") failed at " __FILE__ ":" S__LINE__ "\n"); \
      trap(); \
    } \
  }
