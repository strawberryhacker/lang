// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2025, Bjørn Brodtkorb. All rights reserved.

#ifndef TOK_H
#define TOK_H

//------------------------------------------------------------------------------
#include "util.h"

//------------------------------------------------------------------------------
enum
{
  TOK_NONE,
  TOK_EOF,
  TOK_LN,
  TOK_COMMENT,
  TOK_IDENT,
  TOK_STR,
  TOK_NUM,
  TOK_TY,

  TOK_INC,
  TOK_DEC,
  TOK_MUL,
  TOK_DIV,
  TOK_ADD,
  TOK_SUB,
  TOK_PAR_L,
  TOK_PAR_R,
  TOK_CURLY_L,
  TOK_CURLY_R,
  TOK_BRACK_L,
  TOK_BRACK_R,
  TOK_COMMA,
  TOK_DOT,
  TOK_2DOT,
  TOK_SEMI,
  TOK_COLON,
  TOK_2COLON,
  TOK_EQ,
  TOK_GE,
  TOK_GT,
  TOK_LE,
  TOK_LT,
  TOK_ASSIGN,
  TOK_ARROW,
  TOK_APOST,
  TOK_AND,
  TOK_OR,
  TOK_BITAND,
  TOK_BITOR,

  TOK_FN,
  TOK_STRUCT,
  TOK_UNION,
  TOK_FALSE,
  TOK_TRUE,
  TOK_IF,
  TOK_ELSE,
  TOK_FOR,
  TOK_CONST,
  TOK_WHILE,
  TOK_RETURN,
  TOK_CONTINUE,
  TOK_TYPEDEF,
  TOK_TYPEOF,
  TOK_BREAK,
  TOK_CAST,
  TOK_IMPORT,
  TOK_CNT,
};

//------------------------------------------------------------------------------
typedef struct
{
  int kind;
  uint16_t line;
  uint16_t off;
  union
  {
    uint32_t tag;
    uint32_t ty;
    int64_t num;
  };
} tok_t;

//------------------------------------------------------------------------------
extern tok_t g_tok;

//------------------------------------------------------------------------------
void tok_init(const char* path, const char* data);
void tok_parse();
void tok_skip(int kind);
bool tok_eat(int kind);
void tok_skip_lines();
bool tok_eat_lines();

#endif
