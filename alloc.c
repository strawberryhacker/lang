// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2025, Bjørn Brodtkorb. All rights reserved.

#include "alloc.h"
#include <sys/mman.h>

//------------------------------------------------------------------------------
#define PAGE_SIZE (16 * 1024)

//------------------------------------------------------------------------------
static void add_page(alloc_t* alloc)
{
  void* page = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  assert(page);
  array_append(&alloc->pages, page);
  alloc->off = 0;
}

//------------------------------------------------------------------------------
void alloc_init(alloc_t* alloc)
{
  add_page(alloc);
  alloc->off += sizeof(void*); // Prevent zero tag
}

//------------------------------------------------------------------------------
void* alloc(alloc_t* alloc, int size)
{
  uint32_t tag;
  void* data = alloc_tagged(alloc, size, &tag);
  (void)tag;
  return data;
}

//------------------------------------------------------------------------------
void* alloc_tagged(alloc_t* alloc, int size, uint32_t* tag)
{
  size = alignup(size, sizeof(void*));

  if (alloc->off + size > PAGE_SIZE)
    add_page(alloc);
  
  uint8_t* page = array_last(&alloc->pages);
  uint8_t* res = page + alloc->off;

  *tag = (alloc->pages.len - 1) * PAGE_SIZE + alloc->off;

  alloc->off += size;
  return res;
}

//------------------------------------------------------------------------------
void* alloc_get_tagged(alloc_t* alloc, uint32_t tag)
{
  int idx = tag / PAGE_SIZE;
  int off = tag % PAGE_SIZE;

  uint8_t* page = alloc->pages.data[idx];

  return page + off;
}