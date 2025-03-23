// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2025, Bjørn Brodtkorb. All rights reserved.

#ifndef ARRAY_H
#define ARRAY_H

//------------------------------------------------------------------------------
#include "util.h"

//------------------------------------------------------------------------------
typedef struct
{
  void** data;
  int size;
  int len;
} array_t;

//------------------------------------------------------------------------------
void array_init(array_t* arr, int size);
void array_clear(array_t* arr);
void array_append(array_t* arr, void* item);
void* array_last(array_t* arr);

#endif
