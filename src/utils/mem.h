#pragma once

#include "utils/types.h"

extern "C"
{
size_t strlen(char const* s) noexcept;
void* memset(void* s, int c, size_t n) noexcept;
void* memcpy(void* dest, void const* src, size_t n) noexcept;
int strcmp(char const* s1, char const* s2) noexcept;
}
