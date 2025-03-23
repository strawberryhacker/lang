// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2025, Bjørn Brodtkorb. All rights reserved.

#ifndef ALLOC_H
#define ALLOC_H

//------------------------------------------------------------------------------
#include "util.h"
#include "array.h"

//------------------------------------------------------------------------------
typedef struct
{
  array_t pages;
  int off;
} alloc_t;

//------------------------------------------------------------------------------
void alloc_init(alloc_t* alloc);
void* alloc(alloc_t* alloc, int size);
void* alloc_tagged(alloc_t* alloc, int size, uint32_t* tag);
void* alloc_get_tagged(alloc_t* alloc, uint32_t tag);

#endif
