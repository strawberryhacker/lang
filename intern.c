// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2025, Bjørn Brodtkorb. All rights reserved.

#include "intern.h"

//------------------------------------------------------------------------------
typedef struct
{
  int len;
  char str[];
} entry_t;

//------------------------------------------------------------------------------
static uint32_t str_hash(const char* str, int len)
{
  uint32_t hash = 0x811c9dc5;
  for (int i = 0; i < len; i++)
    hash = (hash ^ str[i]) * 0x1000193;
  return hash;
}

//------------------------------------------------------------------------------
static void expand(intern_t* intern, int size)
{
  uint32_t* tags = calloc(size, sizeof(uint32_t));

  // Rehash
  for (int i = 0; i < intern->size; i++)
  {
    uint32_t tag = intern->tags[i];
    if (!tag)
      continue;
    
    entry_t* ent = alloc_get_tagged(&intern->alloc, tag);
    uint32_t idx = str_hash(ent->str, ent->len) % size;

    // Find first free slot
    while (tags[idx])
      idx = (idx + 1) % size;
    
    tags[idx] = tag;
  }

  free(intern->tags);
  intern->tags = tags;
  intern->size = size;
}

//------------------------------------------------------------------------------
void intern_init(intern_t* intern)
{
  alloc_init(&intern->alloc);
  intern->size = 4096;
  intern->tags = calloc(intern->size, sizeof(uint32_t));
  intern->cnt = 0;
}

//------------------------------------------------------------------------------
uint32_t intern_add(intern_t* intern, const char* str, int len)
{
  uint32_t tag;
  uint32_t idx = str_hash(str, len) % intern->size;

  // Search for match
  for (;;)
  {
    tag = intern->tags[idx];
    if (!tag)
      break;

    entry_t* ent = alloc_get_tagged(&intern->alloc, tag);
    if (ent->len == len && !memcmp(ent->str, str, len))
      return tag;
    
    idx = (idx + 1) % intern->size;
  }

  // No match
  entry_t* ent = alloc_tagged(&intern->alloc, sizeof(entry_t) + len + 1, &tag);
  memcpy(ent->str, str, len);
  ent->str[len] = 0;
  ent->len = len;

  intern->tags[idx] = tag;
  intern->cnt++;

  if (intern->cnt > 2 * intern->size / 3)
    expand(intern, 2 * intern->size);
  
  return tag;
}

//------------------------------------------------------------------------------
const char* intern_get(intern_t* intern, uint32_t tag)
{
  entry_t* ent = alloc_get_tagged(&intern->alloc, tag);
  return ent->str;
}