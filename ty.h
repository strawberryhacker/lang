// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2025, Bjørn Brodtkorb. All rights reserved.

#ifndef TY_H
#define TY_H

//------------------------------------------------------------------------------
#include "util.h"

//------------------------------------------------------------------------------
enum
{
  TY_IDX_BOOL,
  TY_IDX_INT,
  TY_IDX_U8,
  TY_IDX_U16,
  TY_IDX_U32,
  TY_IDX_U64,
  TY_IDX_S8,
  TY_IDX_S16,
  TY_IDX_S32,
  TY_IDX_S64,
  TY_IDX_CNT,
};

enum
{
  TY_INT,
  TY_BOOL,
};

//------------------------------------------------------------------------------
typedef struct
{
  int kind;
  int size;
  int align;
} ty_t;

typedef struct
{
  ty_t base;
  bool sign;
} ty_int_t;

//------------------------------------------------------------------------------
void ty_init();

#endif
