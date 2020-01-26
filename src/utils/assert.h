#pragma once

#include "utils/debug.h"

#define trap() __asm__ __volatile__ ("int $3\n\t")

#define S(x) #x
#define S_(x) S(x)
#define S_LINE S_(__LINE__)

#define assert(cond) \
  do \
  { \
    if (!(cond)) \
    { \
      /* __PRETTY_FUNCTION__ is a normal char[] not a prepocessor macro */ \
      printf(__PRETTY_FUNCTION__); \
      printf(": assert(" #cond ") failed at " __FILE__ ":" S_LINE "\n"); \
      trap(); \
    } \
  } \
  while (0)
