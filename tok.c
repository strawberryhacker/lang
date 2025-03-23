// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2025, Bjørn Brodtkorb. All rights reserved.

#include "tok.h"
#include "intern.h"
#include "ty.h"
#include "error.h"
#include "main.h"

//------------------------------------------------------------------------------
static const char* g_cur;
static const char* g_path;

static uint16_t g_line;
static uint16_t g_off;

tok_t g_tok;

//------------------------------------------------------------------------------
noreturn void error(const char* fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  _error(g_path, g_tok.line, g_tok.off, fmt, va);
}

//------------------------------------------------------------------------------
static bool blank(char c)
{
  return c == ' ' || c == '\t' || c == '\r';
}

//------------------------------------------------------------------------------
static bool digit(char c)
{
  return c >= '0' && c <= '9';
}

//------------------------------------------------------------------------------
static bool letter(char c)
{
  if (c == '_')
    return true;
  
  c |= 0x20;

  return c >= 'a' && c <= 'z';
}

//------------------------------------------------------------------------------
static char next()
{
  assert(g_cur[0]);

  if (g_cur[0] == '\n')
  {
    g_line++;
    g_off = 0;
  }
  else
    g_off++;

  g_cur++;
  return g_cur[0];
}

//------------------------------------------------------------------------------
static char skip(int n)
{
  g_cur += n;
  g_off += n;
  return g_cur[0];
}

//------------------------------------------------------------------------------
static void tok_num()
{
  int64_t num = 0;

  for (char c = g_cur[0]; digit(c); c = next())
    num = 10 * num + c - '0';
  
  g_tok.kind = TOK_NUM;
  g_tok.num = num;
}

//------------------------------------------------------------------------------
static bool tok_keyword(const char* str, const char* keyword, int len, int kind)
{
  if (memcmp(str, keyword, len))
    return false;
  
  g_tok.kind = kind;
  return true;
}

//------------------------------------------------------------------------------
static bool tok_type(const char* str, const char* type, int len, uint32_t ty)
{
  if (memcmp(str, type, len))
    return false;

  g_tok.kind = TOK_TY;
  g_tok.ty = ty;
  return true;
}

//------------------------------------------------------------------------------
static void tok_ident()
{
  const char* ident = g_cur;
  for (char c = g_cur[0]; digit(c) || letter(c); c = next());
  int len = g_cur - ident;

  switch (len) {
    case 2:
    {
      if (tok_keyword(ident, "if", 2, TOK_IF))
        return;
      if (tok_keyword(ident, "fn", 2, TOK_FN))
        return;
      if (tok_type(ident, "u8", 2, TY_IDX_U8))
        return;
      if (tok_type(ident, "s8", 2, TY_IDX_S8))
        return;
    } break;

    case 3:
    {
      if (tok_keyword(ident, "for", 3, TOK_FOR)) return;

      if (ident[0] == 'u') {
        if (tok_type(ident, "u16", 3, TY_IDX_U16))
          return;
        if (tok_type(ident, "u32", 3, TY_IDX_U32))
          return;
        if (tok_type(ident, "u64", 3, TY_IDX_U64))
          return;
      }
      
      if (ident[0] == 's') {
        if (tok_type(ident, "s16", 3, TY_IDX_S16))
          return;
        if (tok_type(ident, "s32", 3, TY_IDX_S32))
          return;
        if (tok_type(ident, "s64", 3, TY_IDX_S64))
          return;
        if (tok_type(ident, "int", 3, TY_IDX_INT))
          return;
      }
    } break;

    case 4:
    {
      if (tok_keyword(ident, "else", 4, TOK_ELSE))
        return;
      if (tok_keyword(ident, "cast", 4, TOK_CAST))
        return;
      if (tok_keyword(ident, "true", 4, TOK_TRUE))
        return;
      if (tok_type(ident, "bool", 4, TY_IDX_BOOL))
        return;
    } break;

    case 5:
    {
      if (tok_keyword(ident, "union", 5, TOK_UNION))
        return;
      if (tok_keyword(ident, "false", 5, TOK_FALSE))
        return;
      if (tok_keyword(ident, "const", 5, TOK_CONST))
        return;
      if (tok_keyword(ident, "while", 5, TOK_WHILE))
        return;
      if (tok_keyword(ident, "break", 5, TOK_BREAK))
        return;
    } break;

    case 6:
    {
      if (tok_keyword(ident, "return", 6, TOK_RETURN))
        return;
      if (tok_keyword(ident, "struct", 6, TOK_STRUCT))
        return;
      if (tok_keyword(ident, "import", 6, TOK_IMPORT))
        return;
      if (tok_keyword(ident, "typeof", 6, TOK_TYPEOF))
        return;
    } break;

    case 7:
    {
      if (tok_keyword(ident, "typedef", 7, TOK_TYPEDEF))
        return;
    } break;

    case 8:
    {
      if (tok_keyword(ident, "continue", 8, TOK_CONTINUE))
        return;
    } break;
  }

  g_tok.tag = intern_add(&g_idents, ident, len);
  g_tok.kind = TOK_IDENT;
}

//------------------------------------------------------------------------------
static void tok_str()
{
  next(); // Open quote
  const char* str = g_cur;

  char c;
  for (c = g_cur[0]; c && c != '"'; c = next());

  if (c == 0)
    error("unterminated string");

  int len = g_cur - str;
  next(); // End quote

  g_tok.tag = intern_add(&g_strings, str, len);
  g_tok.kind = TOK_STR;
}

//------------------------------------------------------------------------------
static bool tok_line_comment()
{
  for (char c = g_cur[0]; c && c != '\n'; c = next());
  g_tok.kind = TOK_COMMENT;
  return true; // Dummy return
}

//------------------------------------------------------------------------------
static bool tok_block_comment()
{
  int nest = 1;

  next();
  next();

  for (char c = g_cur[0]; c && nest; c = next())
  {
    if (c == '/' && g_cur[1] == '*')
    {
      next();
      nest++;
    }
    else if (c == '*' && g_cur[1] == '/')
    {
      next();
      nest--;
    }
  }

  if (nest)
    error("unterminated block comment");

  g_tok.kind = TOK_COMMENT;
  return true; // Dummy return
}

//------------------------------------------------------------------------------
static bool tok_punct(int kind, int len)
{
  skip(len);
  g_tok.kind = kind;
  return true; // Dummy return
}

//------------------------------------------------------------------------------
static bool tok_rest()
{
  switch (g_cur[0]) {
    case '*':
      return tok_punct(TOK_MUL, 1);

    case '/':
      if (g_cur[1] == '*')
        return tok_block_comment();  
      else if (g_cur[1] == '/')
        return tok_line_comment();
      else
        return tok_punct(TOK_DIV, 1);

    case '+':
      if (g_cur[1] == '+')
        return tok_punct(TOK_INC, 2);
      else
        return tok_punct(TOK_ADD, 1);

    case '-':
      if (g_cur[1] == '-')
        return tok_punct(TOK_DEC, 2);
      else if (g_cur[1] == '>')
        return tok_punct(TOK_ARROW, 2);
      else
        return tok_punct(TOK_SUB, 1);

    case '(':
      return tok_punct(TOK_PAR_L, 1);

    case ')':
      return tok_punct(TOK_PAR_R, 1);

    case '{':
      return tok_punct(TOK_CURLY_L, 1);

    case '}':
      return tok_punct(TOK_CURLY_R, 1);

    case '[':
      return tok_punct(TOK_BRACK_L, 1);

    case ']':
      return tok_punct(TOK_BRACK_R, 1);

    case ',':
      return tok_punct(TOK_COMMA, 1);

    case ';':
      return tok_punct(TOK_SEMI, 1);

    case '.':
      if (g_cur[1] == '.')
        return tok_punct(TOK_2DOT, 2);
      else
        return tok_punct(TOK_DOT, 1);

    case ':':
      if (g_cur[1] == ':')
        return tok_punct(TOK_2COLON, 2);
      else
        return tok_punct(TOK_COLON, 1);

    case '=':
      if (g_cur[1] == '=')
        return tok_punct(TOK_EQ, 2);
      else
        return tok_punct(TOK_ASSIGN, 1);

    case '>':
      if (g_cur[1] == '=')
        return tok_punct(TOK_GE, 2);
      else
        return tok_punct(TOK_GT, 1);

    case '<':
      if (g_cur[1] == '=')
        return tok_punct(TOK_LE, 2);
      else
        return tok_punct(TOK_LT, 1);

    case '`':
      return tok_punct(TOK_APOST, 1);

    case '|':
      if (g_cur[1] == '|')
        return tok_punct(TOK_OR, 2);
      else
        return tok_punct(TOK_BITOR, 1);

    case '&':
      if (g_cur[1] == '&')
        return tok_punct(TOK_AND, 2);
      else
        return tok_punct(TOK_BITAND, 1);

    default:
      return false;
  }
}

//------------------------------------------------------------------------------
void tok_init(const char* path, const char* data)
{
  g_cur = data;
  g_path = path;
  tok_parse();
}

//------------------------------------------------------------------------------
void tok_parse()
{
  char c;
  for (c = g_cur[0]; blank(c); c = next());

  g_tok.line = g_line;
  g_tok.off = g_off;

  if (c == 0)
  {
    g_tok.kind = TOK_EOF;
  }
  else if (c == '\n')
  {
    g_tok.kind = TOK_LN;
    next();
  }
  else if (c == '"')
  {
    tok_str();
  }
  else if (digit(c))
  {
    tok_num();
  }
  else if (letter(c))
  {
    tok_ident();
  }
  else
  {
    if (!tok_rest())
      error("invalid input");
  }
}

//------------------------------------------------------------------------------
void tok_skip(int kind)
{
  static char* tok_str[TOK_CNT] =
  {
    [TOK_IDENT] = "identifier",
  };

  if (g_tok.kind != kind)
  {
    char* str = tok_str[kind];
    assert(str);
    error("expecting %s", str);
  }

  tok_parse();
}

//------------------------------------------------------------------------------
bool tok_eat(int kind)
{
  if (g_tok.kind != kind)
    return false;

  tok_parse();
  return true;
}

//------------------------------------------------------------------------------
void tok_skip_lines()
{
  if (g_tok.kind != TOK_LN)
    error("expecting newline");

  while (tok_eat(TOK_LN));
}

//------------------------------------------------------------------------------
bool tok_eat_lines()
{
  if (g_tok.kind != TOK_LN)
    return false;

  while (tok_eat(TOK_LN));
  return true;
}
