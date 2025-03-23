// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2025, Bjørn Brodtkorb. All rights reserved.

#include "array.h"

//------------------------------------------------------------------------------
void array_init(array_t* arr, int size)
{
  arr->data = malloc(size * sizeof(*arr->data));
  arr->size = size;
  arr->len = 0;
}

//------------------------------------------------------------------------------
void array_clear(array_t* arr)
{
  arr->len = 0;
}

//------------------------------------------------------------------------------
void array_append(array_t* arr, void* item)
{
  if (arr->len == arr->size)
  {
    arr->size = arr->size ? 2 * arr->size : 8;
    arr->data = realloc(arr->data, arr->size * sizeof(*arr->data));
  }

  arr->data[arr->len++] = item;
}

//------------------------------------------------------------------------------
void* array_last(array_t* arr)
{
  assert(arr->len);
  return arr->data[arr->len - 1];
}
