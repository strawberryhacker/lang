// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2025, Bjørn Brodtkorb. All rights reserved.

#ifndef ERROR_H
#define ERROR_H

//------------------------------------------------------------------------------
#include "util.h"

//------------------------------------------------------------------------------
noreturn void _error(const char* path, uint16_t line, uint16_t off, const char* fmt, va_list va);
void _warn(const char* path, uint16_t line, uint16_t off, const char* fmt, va_list va);

#endif