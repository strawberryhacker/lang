// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2025, Bjørn Brodtkorb. All rights reserved.

#include "util.h"
#include "main.h"
#include "parse.h"
#include "ty.h"

//------------------------------------------------------------------------------
intern_t g_idents;
intern_t g_strings;

//------------------------------------------------------------------------------
int main(int argc, const char** argv)
{
  ty_init();

  intern_init(&g_idents);
  intern_init(&g_strings);

  printf("This is a test\n");
}