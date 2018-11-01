/*
 inpcur.h

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#if !defined(__inpcur_h__)

#include "inpbuf.h"

enum inpmod
{
  MOD_NONE = 0x00,
  MOD_EXPOSE = 0x02,
  MOD_SCROLL = 0x04,
  MOD_CURSOR = 0x10,
  MOD_LINE = 0x20
};

struct inpcur
{
  struct inplist* m_list;
  struct inpbuf* m_active;
  unsigned int m_caret_x;
  unsigned int m_slot_x;
  unsigned int m_sticky_x;
  unsigned int m_caret_y;
  unsigned int m_offset_x;
  unsigned int m_offset_y;
  unsigned int m_len_x;
  unsigned int m_len_y;
};

#ifdef __cplusplus
extern "C" {
#endif

extern struct inpcur*
inpcur_new(
  unsigned int const i_len_x,
  unsigned int const i_len_y);

extern void
inpcur_free(
  struct inpcur* const i_cur);

extern enum inpmod
inpcur_resize(
  struct inpcur* const io_cur,
  unsigned int const i_len_x,
  unsigned int const i_len_y);

extern enum inpmod
inpcur_home(
  struct inpcur* const io_cur);

extern enum inpmod
inpcur_end(
  struct inpcur* const io_cur);

extern enum inpmod
inpcur_left(
  struct inpcur* const io_cur);

extern enum inpmod
inpcur_right(
  struct inpcur* const io_cur);

extern enum inpmod
inpcur_up(
  struct inpcur* const io_cur);

extern enum inpmod
inpcur_down(
  struct inpcur* const io_cur);

extern enum inpmod
inpcur_ins(
  struct inpcur* const io_cur,
  unsigned char const i_chr);

extern enum inpmod
inpcur_ovw(
  struct inpcur* const io_cur,
  unsigned char const i_chr);

extern enum inpmod
inpcur_del(
  struct inpcur* const io_cur);

#ifdef __cplusplus
}
#endif

#define __inpcur_h__
#endif
