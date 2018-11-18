/*
 inpcur.c

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#include "inpcur.h"
#include <stdlib.h>
#include <string.h>

struct inpcur*
inpcur_new(
  unsigned int const i_len_x,
  unsigned int const i_len_y)
{
  struct inpcur* l_cur;
  l_cur = (struct inpcur*)calloc(1, sizeof(*l_cur));
  (*l_cur).m_list = inplist_new();
  (*l_cur).m_active = inplist_append((*l_cur).m_list);
  (*l_cur).m_len_x = i_len_x;
  (*l_cur).m_len_y = i_len_y;
  return l_cur;
}

void
inpcur_free(
  struct inpcur* const io_cur)
{
  inplist_free((*io_cur).m_list);
  return;
}

enum inpmod
inpcur_resize(
  struct inpcur* const io_cur,
  unsigned int const i_len_x,
  unsigned int const i_len_y)
{
  unsigned int l_mod;

  l_mod = MOD_NONE;

  //FIXME

  return l_mod;
}

enum inpmod
inpcur_home(
  struct inpcur* const io_cur)
{
  unsigned int l_mod;

  l_mod = MOD_NONE;

  if ((*io_cur).m_slot_x)
  {
    if ((*io_cur).m_caret_x)
    {
      (*io_cur).m_caret_x = 0;
      l_mod |= MOD_CURSOR;
    }

    (*io_cur).m_slot_x = 0;

    if ((*io_cur).m_offset_x)
    {
      (*io_cur).m_offset_x = 0;
      l_mod |= MOD_SCROLL;
    }
  }

  (*io_cur).m_sticky_x = (*io_cur).m_slot_x;

  return l_mod;
}

enum inpmod
inpcur_end(
  struct inpcur* const io_cur)
{
  unsigned int l_mod;

  l_mod = MOD_NONE;

  while ((*io_cur).m_active->m_length > (*io_cur).m_slot_x)
  {
    (*io_cur).m_slot_x++;

    if ((*io_cur).m_len_x > (*io_cur).m_caret_x)
    {
      (*io_cur).m_caret_x++;
      l_mod |= MOD_CURSOR;
    }

    else
    {
      (*io_cur).m_offset_x++;
      l_mod |= MOD_SCROLL;
    }
  }

  (*io_cur).m_sticky_x = (*io_cur).m_slot_x;

  return l_mod;
}

enum inpmod
inpcur_left(
  struct inpcur* const io_cur)
{
  unsigned int l_mod;

  l_mod = MOD_NONE;

  if (0 == (*io_cur).m_caret_x && (*io_cur).m_offset_x)
  {
    (*io_cur).m_offset_x--;
    l_mod |= MOD_SCROLL;
  }

  if ((*io_cur).m_caret_x)
  {
    l_mod |= MOD_CURSOR;
    (*io_cur).m_caret_x--;
  }

  if ((*io_cur).m_slot_x)
  {
    (*io_cur).m_slot_x--;
  }

  (*io_cur).m_sticky_x = (*io_cur).m_slot_x;

  return l_mod;
}

enum inpmod
inpcur_right(
  struct inpcur* const io_cur)
{
  unsigned int l_mod;
  int l_overflow;

  l_mod = MOD_NONE;
  l_overflow = 1;

  if ((*io_cur).m_active->m_length > (*io_cur).m_slot_x)
  {
    if ((*io_cur).m_len_x && (((*io_cur).m_len_x - 1) > (*io_cur).m_caret_x))
    {
      if ((*io_cur).m_active->m_length > (*io_cur).m_caret_x)
      {
        l_mod |= MOD_CURSOR;
        (*io_cur).m_caret_x++;
        l_overflow = 0;
      }
    }

    if (l_overflow)
    {
      (*io_cur).m_offset_x++;
      l_mod |= MOD_SCROLL;
    }

    (*io_cur).m_slot_x++;
  }

  (*io_cur).m_sticky_x = (*io_cur).m_slot_x;

  return l_mod;
}

static enum inpmod
inpcur_sticky(
  struct inpcur* const io_cur)
{
  unsigned int l_mod;
  int l_overflow;

  l_mod = MOD_NONE;

  (*io_cur).m_slot_x = 0;
  (*io_cur).m_caret_x = 0;
  (*io_cur).m_offset_x = 0;

  do
  {

    if ((*io_cur).m_sticky_x <= (*io_cur).m_slot_x)
    {
      break;
    }

    if ((*io_cur).m_active->m_length <= (*io_cur).m_slot_x)
    {
      break;
    }

    l_overflow = 1;

    if ((*io_cur).m_len_x && (((*io_cur).m_len_x - 1) > (*io_cur).m_caret_x))
    {
      if ((*io_cur).m_active->m_length > (*io_cur).m_caret_x)
      {
        l_mod |= MOD_CURSOR;
        (*io_cur).m_caret_x++;
        l_overflow = 0;
      }
    }

    if (l_overflow)
    {
      (*io_cur).m_offset_x++;
      l_mod |= MOD_SCROLL;
    }

    (*io_cur).m_slot_x++;

  }
  while (1);

  return l_mod;
}

extern enum inpmod
inpcur_up(
  struct inpcur* const io_cur)
{
  unsigned int l_mod;

  l_mod = MOD_NONE;

  if ((*io_cur).m_active->m_prev)
  {
    (*io_cur).m_active = (*io_cur).m_active->m_prev;

    if ((*io_cur).m_caret_y)
    {
      (*io_cur).m_caret_y--;
      l_mod = MOD_CURSOR;
    }

    else
    {
      (*io_cur).m_offset_y--;
      l_mod = MOD_SCROLL;
    }

    l_mod |= inpcur_sticky(io_cur);
  }

  return l_mod;
}

extern enum inpmod
inpcur_down(
  struct inpcur* const io_cur)
{
  unsigned int l_mod;

  l_mod = MOD_NONE;

  if ((*io_cur).m_active->m_next)
  {
    (*io_cur).m_active = (*io_cur).m_active->m_next;

    if ((*io_cur).m_len_y && ((*io_cur).m_len_y - 1) > (*io_cur).m_caret_y)
    {
      (*io_cur).m_caret_y++;
      l_mod = MOD_CURSOR;
    }

    else
    {
      (*io_cur).m_offset_y++;
      l_mod = MOD_SCROLL;
    }

    l_mod |= inpcur_sticky(io_cur);
  }

  return l_mod;
}

enum inpmod
inpcur_ins(
  struct inpcur* const io_cur,
  unsigned char const i_chr)
{
  unsigned int l_mod;
  int l_result;

  l_mod = MOD_NONE;

  l_result = inpbuf_ins((*io_cur).m_active, (*io_cur).m_slot_x, i_chr);

  if (l_result)
  {
    l_mod = inpcur_right(io_cur);
  }

  return l_mod;
}

enum inpmod
inpcur_ovw(
  struct inpcur* const io_cur,
  unsigned char const i_chr)
{
  unsigned int l_mod;
  int l_result;

  l_mod = MOD_NONE;

  l_result = inpbuf_ovw((*io_cur).m_active, (*io_cur).m_slot_x, i_chr);

  if (l_result)
  {
    l_mod = inpcur_right(io_cur);
  }

  return l_mod;
}

enum inpmod
inpcur_del(
  struct inpcur* const io_cur)
{
  unsigned int l_mod;
  int l_result;
  struct inpbuf* l_buf;
  size_t l_size;
  size_t l_slot;

  l_mod = MOD_NONE;

  if ((*io_cur).m_slot_x < (*io_cur).m_active->m_length)
  {
    l_result = inpbuf_del((*io_cur).m_active, (*io_cur).m_slot_x);

    if (l_result)
    {
      l_mod = MOD_LINE;
    }
  }

  else
  {
    l_buf = (*io_cur).m_active->m_next;

    if (l_buf)
    {
      l_size = ((*l_buf).m_length + (*io_cur).m_active->m_length);
      l_result = inpbuf_grow((*io_cur).m_active, (*l_buf).m_length);

      if (l_result)
      {
        if ((*l_buf).m_length)
        {
          memcpy(
            (*io_cur).m_active->m_data + (*io_cur).m_active->m_length,
            (*l_buf).m_data,
            (*l_buf).m_length);
          (*io_cur).m_active->m_length += (*l_buf).m_length;
        }

        l_slot = inplist_slot((*io_cur).m_list, l_buf);
        inplist_remove((*io_cur).m_list, l_slot);
        l_mod = MOD_EXPOSE;
      }
    }
  }

  return l_mod;
}
