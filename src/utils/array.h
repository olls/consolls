#pragma once

#include "utils/types.h"
#include "utils/allocate.h"
#include "utils/assert.h"

#define OVERRIDE_ARRAY_ALLOCATE_FUNC Allocate::allocate<u8>
#define OVERRIDE_ARRAY_FREE_FUNC Allocate::unallocate
#define OVERRIDE_ARRAY_ASSERT_FUNC assert

#include "array_impl.h"
