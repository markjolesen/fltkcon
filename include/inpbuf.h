/*
 inpbuf.h

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#if !defined(__inpbuf_h__)

#include <stddef.h>

struct inpbuf
{
  size_t m_length;
  size_t m_data_size;
  unsigned char* m_data;
  struct inpbuf* m_next;
  struct inpbuf* m_prev;
};

struct inplist
{
  struct inpbuf* m_head;
  struct inpbuf* m_tail;
  struct inpbuf* m_free;
  size_t m_size;
};

#ifdef __cplusplus
extern "C" {
#endif

extern struct inplist*
inplist_new();

extern void
inplist_free(
  struct inplist* const io_list);

extern void
inplist_clear(
  struct inplist* const io_list);

extern struct inpbuf*
inplist_append(
  struct inplist* const io_list);

extern struct inpbuf*
inplist_insert(
  struct inplist* const io_list,
  size_t const i_slot);

extern void
inplist_remove(
  struct inplist* const io_list,
  size_t const i_slot);

extern unsigned char*
inplist_as_string(
  struct inplist const* const i_list);

extern size_t
inplist_slot(
  struct inplist const* const i_list,
  struct inpbuf const* const i_buf);

extern struct inpbuf*
inplist_nth(
  struct inplist* const i_list,
  size_t const i_slot);

extern int
inpbuf_grow(
  struct inpbuf* const io_buf,
  size_t const i_growby);

extern int
inpbuf_ins(
  struct inpbuf* const io_buf,
  size_t const i_slot,
  unsigned char const i_chr);

extern int
inpbuf_ovw(
  struct inpbuf* const io_buf,
  size_t const i_slot,
  unsigned char const i_chr);

extern int
inpbuf_del(
  struct inpbuf* const io_buf,
  size_t const i_slot);

extern int
inpbuf_copy(
  struct inpbuf* const io_buf,
  unsigned char const* i_data,
  size_t const i_size);

#ifdef __cplusplus
}
#endif

#define __inpbuf_h__
#endif
