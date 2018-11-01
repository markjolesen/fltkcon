/*
 BLOCK.C

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#include "block.h"
#include "screen.h"
#include <stdlib.h>
#include <stddef.h>
#if defined(__WATCOMC__)
#include <malloc.h>
#endif

extern struct block*
block_new()
{
  struct block* l_block;

  l_block = calloc(1, sizeof(*l_block));

  return l_block;
}

extern void
block_free(
  struct block* const o_block)
{

  free((*o_block).m_data);
  free(o_block);

  return;
}

static unsigned int
block_length(
  unsigned int const i_len_x,
  unsigned int const i_len_y)
{
  unsigned int l_len_x;
  unsigned int l_size;

  l_len_x = i_len_x;
#if defined(__CURSES__)
  l_len_x++;
#endif

  l_size = l_len_x * i_len_y * sizeof(screen_block_t);
#if defined(__CURSES__)
  l_size += sizeof(screen_block_t);
#endif

  return l_size;
}

static void
block_screen_read(
  struct block* const io_block)
{
  unsigned int l_len_x;
  screen_block_t* l_next;
  int l_pos_y;
  unsigned int l_row;

  l_len_x = (*io_block).m_len_x;
#if defined(__CURSES__)
  l_len_x++;
#endif

  l_next = (*io_block).m_data;

  for (l_row = 0; (*io_block).m_len_y > l_row; l_row++)
  {
    l_pos_y = (*io_block).m_pos_y + l_row;

    if ((int)_video_rows <= l_pos_y)
    {
      break;
    }

    l_next = screen_read(l_next, l_len_x, (*io_block).m_pos_x, l_pos_y);
  }

  return;
}

extern void
block_read(
  struct block* const io_block,
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y)
{
  unsigned int l_size;
  void* l_block;

  l_size = block_length(i_len_x, i_len_y);

  if ((*io_block).m_alloc_size < l_size)
  {
    l_block = realloc((*io_block).m_data, l_size);
    (*io_block).m_data = l_block;
    (*io_block).m_alloc_size = l_size;
  }

  (*io_block).m_pos_x = i_pos_x;
  (*io_block).m_pos_y = i_pos_y;
  (*io_block).m_len_x = i_len_x;
  (*io_block).m_len_y = i_len_y;

  block_screen_read(io_block);

  return;
}

extern void
block_write(
  struct block const* const i_block)
{
  unsigned int                        l_len_x;
  screen_block_t const __FAR*         l_next;
  int                                 l_pos_y;
  unsigned int                        l_row;

  l_len_x = (*i_block).m_len_x;
#if defined(__CURSES__)
  l_len_x++;
#endif

  l_next = (*i_block).m_data;

  for (l_row = 0; (*i_block).m_len_y > l_row; l_row++)
  {
    l_pos_y = (*i_block).m_pos_y + l_row;

    if ((int)_video_rows <= l_pos_y)
    {
      break;
    }

    l_next = screen_write((*i_block).m_pos_x, l_pos_y, l_next, l_len_x);
  }

  return;
}
