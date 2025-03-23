// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2025, Bjørn Brodtkorb. All rights reserved.

#ifndef INTERN_H
#define INTERN_H

//------------------------------------------------------------------------------
#include "util.h"
#include "alloc.h"

//------------------------------------------------------------------------------
typedef struct
{
  alloc_t alloc;
  uint32_t* tags;
  int size;
  int cnt;
} intern_t;

//------------------------------------------------------------------------------
void intern_init(intern_t* intern);
uint32_t intern_add(intern_t* intern, const char* str, int len);
const char* intern_get(intern_t* intern, uint32_t tag);

#endif
