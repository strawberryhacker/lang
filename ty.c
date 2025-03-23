// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2025, Bjørn Brodtkorb. All rights reserved.

#include "ty.h"
#include "array.h"
#include "alloc.h"

//------------------------------------------------------------------------------
array_t g_types;
alloc_t g_alloc;

//------------------------------------------------------------------------------
static ty_int_t* ty_int(int size, bool sign)
{
  ty_int_t* ty = alloc(&g_alloc, sizeof(ty_int_t));
  ty->base.kind = TY_INT;
  ty->base.size = size;
  ty->base.align = size;
  ty->sign = sign;
  return ty;
}

//------------------------------------------------------------------------------
static ty_t* ty_bool()
{
  ty_t* ty = alloc(&g_alloc, sizeof(ty_t));
  ty->kind = TY_BOOL;
  ty->size = 0;
  ty->align = 0;
  return ty;
}

//------------------------------------------------------------------------------
void ty_init()
{
  array_init(&g_types, 1024);

  g_types.data[TY_BOOL] = ty_bool();
  g_types.data[TY_INT] = ty_int(0, true);
  g_types.data[TY_IDX_U8] = ty_int(1, false);
  g_types.data[TY_IDX_U16] = ty_int(2, false);
  g_types.data[TY_IDX_U32] = ty_int(4, false);
  g_types.data[TY_IDX_U64] = ty_int(8, false);
  g_types.data[TY_IDX_S8] = ty_int(1, true);
  g_types.data[TY_IDX_S16] = ty_int(2, true);
  g_types.data[TY_IDX_S32] = ty_int(4, true);
  g_types.data[TY_IDX_S64] = ty_int(8, true);

  g_types.len = TY_IDX_CNT;
}