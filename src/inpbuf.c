/*
 inpbuf.c

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#include "inpbuf.h"
#include <stdlib.h>
#include <string.h>

#define INPBUF_GROWBY 128

static struct inpbuf*
inplist_alloc(
  struct inplist* const io_list)
{
  struct inpbuf* l_buf;

  if ((*io_list).m_free)
  {
    l_buf = (*io_list).m_free;
    (*io_list).m_free = (*io_list).m_free->m_next;
    (*l_buf).m_next = 0;
  }

  else
  {
    l_buf = (struct inpbuf*)calloc(1, sizeof(*l_buf));
  }

  return l_buf;
}


struct inplist*
inplist_new()
{
  struct inplist* l_list;
  l_list = (struct inplist*)calloc(1, sizeof(*l_list));
  return l_list;
}

void
inplist_free(
  struct inplist* const io_list)
{
  struct inpbuf* l_buf;
  struct inpbuf* l_next;

  l_buf = (*io_list).m_head;

  while (l_buf)
  {
    l_next = (*l_buf).m_next;
    free((*l_buf).m_data);
    free(l_buf);
    l_buf = l_next;
  };

  l_buf = (*io_list).m_free;

  while (l_buf)
  {
    l_next = (*l_buf).m_next;
    free((*l_buf).m_data);
    free(l_buf);
    l_buf = l_next;
  };

  free(io_list);

  return;
}

void
inplist_clear(
  struct inplist* const io_list)
{
  struct inpbuf* l_node;

  if ((*io_list).m_head)
  {
    (*io_list).m_tail->m_next = (*io_list).m_free;
    (*io_list).m_free = (*io_list).m_head;
    (*io_list).m_head = 0;
    (*io_list).m_tail = 0;
    (*io_list).m_size = 0;

    l_node = (*io_list).m_free;

    while (l_node)
    {
      (*l_node).m_length = 0;
      (*l_node).m_prev = 0;
      l_node = (*l_node).m_next;
    }
  }

  return;
}

struct inpbuf*
inplist_append(
  struct inplist* const io_list)
{
  struct inpbuf* l_buf;

  l_buf = inplist_alloc(io_list);

  if ((*io_list).m_tail)
  {
    (*l_buf).m_prev = (*io_list).m_tail;
    (*io_list).m_tail->m_next = l_buf;
    (*io_list).m_tail = l_buf;
    (*io_list).m_size++;
  }

  else
  {
    (*io_list).m_head = l_buf;
    (*io_list).m_tail = l_buf;
    (*io_list).m_size = 1;
  }

  return l_buf;
}

struct inpbuf*
inplist_insert(
  struct inplist* const io_list,
  size_t const i_slot)
{
  struct inpbuf* l_node;
  struct inpbuf* l_buf;
  size_t l_slot;

  l_node = 0;

  for (l_slot = (*io_list).m_size; l_slot < i_slot; l_slot++)
  {
    inplist_append(io_list);
  }

  l_node = (*io_list).m_head;

  for (l_slot = 0; l_slot < i_slot; l_slot++)
  {
    l_node = (*l_node).m_next;
  }

  if (l_node)
  {
    l_buf = inplist_alloc(io_list);

    (*l_buf).m_prev = (*l_node).m_prev;

    if ((*l_node).m_prev)
    {
      (*l_node).m_prev->m_next = l_buf;
    }

    (*l_node).m_prev = l_buf;
    (*l_buf).m_next = l_node;

    if ((*io_list).m_head == l_node)
    {
      (*io_list).m_head = l_buf;
    }

    if ((*io_list).m_tail == l_node)
    {
      (*io_list).m_tail = l_buf;
    }

    (*io_list).m_size++;
  }

  else
  {
    l_buf = inplist_append(io_list);
  }

  return l_buf;
}

void
inplist_remove(
  struct inplist* const io_list,
  size_t const i_slot)
{
  struct inpbuf* l_node;
  size_t l_slot;

  l_node = (*io_list).m_head;

  for (l_slot = 0; l_slot < i_slot; l_slot++)
  {
    l_node = (*l_node).m_next;
  }

  if (l_node)
  {

    if ((*l_node).m_prev)
    {
      (*l_node).m_prev->m_next = (*l_node).m_next;
    }

    if ((*l_node).m_next)
    {
      (*l_node).m_next->m_prev = (*l_node).m_prev;
    }

    if ((*io_list).m_head == l_node)
    {
      (*io_list).m_head = (*l_node).m_next;
    }

    if ((*io_list).m_tail == l_node)
    {
      (*io_list).m_tail = (*l_node).m_prev;
    }

    (*l_node).m_length = 0;
    (*l_node).m_next = (*io_list).m_free;
    (*l_node).m_prev = 0;
    (*io_list).m_free = l_node;
    (*io_list).m_size--;
  }

  return;
}

extern unsigned char*
inplist_as_string(
  struct inplist const* const i_list)
{
  struct inpbuf const* l_buf;
  unsigned char* l_str;
  unsigned char* l_tail;
  size_t l_size;

  l_str = 0;
  l_size = 0;

  for (l_buf = (*i_list).m_head; l_buf; l_buf = (*l_buf).m_next)
  {
    l_size += (1 + (*l_buf).m_length);
  };

  l_size += (1 + (*i_list).m_size);

  l_str = (unsigned char*)calloc(1, l_size);

  l_tail = l_str;

  for (l_buf = (*i_list).m_head; l_buf; l_buf = (*l_buf).m_next)
  {
    memcpy(l_tail, (*l_buf).m_data, (*l_buf).m_length);
    l_tail += (*l_buf).m_length;

    if ((*l_buf).m_next)
    {
      *l_tail++ = '\n';
    }
  };

  return l_str;
}

size_t
inplist_slot(
  struct inplist const* const i_list,
  struct inpbuf const* const i_buf)
{
  struct inpbuf* l_head;
  size_t l_slot;

  l_head = (*i_list).m_head;
  l_slot = 0;

  while (l_head)
  {
    if (i_buf == l_head)
    {
      break;
    }

    l_head = (*l_head).m_next;
    l_slot++;
  };

  return l_slot;
}

struct inpbuf*
inplist_nth(
  struct inplist* const i_list,
  size_t const i_slot)
{
  size_t l_slot;
  struct inpbuf* l_buf;

  l_buf = (*i_list).m_head;
  l_slot = 0;

  while (l_buf)
  {
    if (i_slot == l_slot)
    {
      break;
    }

    l_buf = (*l_buf).m_next;
    l_slot++;
  }

  return l_buf;
}

int
inpbuf_grow(
  struct inpbuf* const io_buf,
  size_t const i_growby)
{
  int l_exit;
  unsigned char* l_block;
  size_t l_size;

  do
  {

    l_size = ((*io_buf).m_length + i_growby);

    if ((*io_buf).m_data_size > l_size)
    {
      break;
    }

    l_size = 8 + (((INPBUF_GROWBY + l_size) >> 3) << 3);

    l_block = (unsigned char*)realloc((*io_buf).m_data, l_size);

    if (l_block)
    {
      (*io_buf).m_data = l_block;
      (*io_buf).m_data_size = l_size;
    }

  }
  while (0);

  l_exit = (i_growby <= (*io_buf).m_data_size);

  return l_exit;
}

int
inpbuf_ins(
  struct inpbuf* const io_buf,
  size_t const i_slot,
  unsigned char const i_chr)
{
  unsigned char* l_dst;
  unsigned char* l_src;
  size_t l_slot;
  int l_exit;

  do
  {

    if ((*io_buf).m_length < i_slot)
    {
      break;
    }

    l_exit = inpbuf_grow(io_buf, 1);

    if (0 == l_exit)
    {
      break;
    }

    if (0 == (*io_buf).m_length)
    {
      *(*io_buf).m_data = i_chr;
      (*io_buf).m_length++;
      break;
    }

    l_dst = (*io_buf).m_data + (*io_buf).m_length;
    l_src = (*io_buf).m_data + (*io_buf).m_length - 1;
    l_slot = (*io_buf).m_length;

    do
    {
      if (i_slot == l_slot)
      {
        *((*io_buf).m_data + l_slot) = i_chr;
        (*io_buf).m_length++;
        break;
      }

      *l_dst-- = *l_src--;
      l_slot--;
    }
    while (1);

  }
  while (0);

  return l_exit;
}

int
inpbuf_ovw(
  struct inpbuf* const io_buf,
  size_t const i_slot,
  unsigned char const i_chr)
{
  int l_exit;

  l_exit = 0;

  do
  {

    if ((*io_buf).m_length < i_slot)
    {
      break;
    }

    if ((*io_buf).m_length == i_slot)
    {
      l_exit = inpbuf_grow(io_buf, 1);

      if (0 == l_exit)
      {
        break;
      }

      (*io_buf).m_length++;
    }

    *((*io_buf).m_data + i_slot) = i_chr;

    l_exit = 1;

  }
  while (0);

  return l_exit;
}

int
inpbuf_del(
  struct inpbuf* const io_buf,
  size_t const i_slot)
{
  int l_exit;
  unsigned char* l_dst;
  unsigned char* l_src;
  size_t l_slot;

  l_exit = 0;

  do
  {

    if ((*io_buf).m_length <= i_slot)
    {
      break;
    }

    l_src = (*io_buf).m_data + i_slot + 1;
    l_dst = (*io_buf).m_data + i_slot;
    l_slot = i_slot;

    do
    {

      if (l_slot == (*io_buf).m_length)
      {
        break;
      }

      *l_dst++ = *l_src++;
      l_slot++;

    }
    while (1);

    (*io_buf).m_length--;
    l_exit = 1;

  }
  while (0);

  return l_exit;
}

extern int
inpbuf_copy(
  struct inpbuf* const io_buf,
  unsigned char const* i_data,
  size_t const i_size)
{
  int l_result;

  (*io_buf).m_length = 0;

  l_result = inpbuf_grow(io_buf, i_size);

  if (l_result)
  {
    memcpy((*io_buf).m_data, i_data, i_size);
    (*io_buf).m_length = i_size;
  }

  return l_result;
}
