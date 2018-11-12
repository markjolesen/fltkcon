/*
 BLOCK.H
 
 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#ifndef __block_h__

#ifdef __cplusplus
extern "C" {
#endif

struct block
{
  void* m_data;
  unsigned int m_alloc_size;
  int m_pos_x;
  int m_pos_y;
  unsigned int m_len_x;
  unsigned int m_len_y;
};

extern struct block*
block_new();

extern void
block_free(
  struct block* const o_block);

extern void
block_read(
  struct block* const io_block,
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y);

extern void
block_read_frame(
  struct block* const io_block,
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y);

extern void
block_write(
  struct block const* const i_block);

extern void
block_write_frame(
  struct block const* const i_block);

#ifdef __cplusplus
}
#endif

#define __block_h__
#endif
