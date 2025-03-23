// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2025, Bjørn Brodtkorb. All rights reserved.

#include "error.h"

//------------------------------------------------------------------------------
noreturn void _error(const char* path, uint16_t line, uint16_t off, const char* fmt, va_list va)
{
  printf("%s:%d:%d: \033[31merror:\033[0m ", path, line, off);
  vprintf(fmt, va);
  printf("\n");
  exit(0);
}

//------------------------------------------------------------------------------
void _warn(const char* path, uint16_t line, uint16_t off, const char* fmt, va_list va)
{
  printf("%s:%d:%d: \033[33mwarning:\033[0m ", path, line, off);
  vprintf(fmt, va);
  printf("\n");
  exit(0);
}