// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2025, Bjørn Brodtkorb. All rights reserved.

#ifndef UTIL_H
#define UTIL_H

//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdnoreturn.h>

//------------------------------------------------------------------------------
static inline int alignup(int val, int align)
{
  return (val + align - 1) / align * align;
}

#endif
