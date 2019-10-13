#pragma once

#include "utils/types.h"

extern "C"
{
size_t strlen(char const *s) noexcept;
void *memset(void *s, int c, size_t n) noexcept;
void *memcpy(void *dest, void const *src, size_t n) noexcept;
int strcmp(const char *s1, const char *s2) noexcept;
}
