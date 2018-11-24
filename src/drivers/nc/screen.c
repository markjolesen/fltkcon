/*
 screen.c

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "screen.h"
#include <stdio.h>

short int                               mouse_initialized = 0;
int                                     _video_has_color = 1;
unsigned int                            _video_cols = 80;
unsigned int                            _video_rows = 25;

struct screen_pair_map
{
  enum foreground                       m_fcolor;
  struct screen_map_bcolor
  {
    int                                 m_pair;
    enum background                     m_bcolor;
  }                                     m_bkgnd[8];
};

static struct screen_pair_map const g_pair_map[8] =
{
  {
    fcolor_black /*0*/,
    { {1, bcolor_black/*0*/},
      {2, bcolor_red/*1*/},
      {3, bcolor_green/*2*/},
      {4, COLOR_YELLOW/*bcolor_yellow 3*/},
      {5, bcolor_blue/*4*/},
      {6, bcolor_magenta/*5*/},
      {7, bcolor_cyan/*6*/},
      {8, bcolor_light_gray/*WHITE 7*/}
    }
  },
  {
    fcolor_red /*1*/,
    { {9, bcolor_black/*0*/},
      {10, bcolor_red/*1*/},
      {11, bcolor_green/*2*/},
      {12, COLOR_YELLOW/*bcolor_yellow 3*/},
      {13, bcolor_blue/*4*/},
      {14, bcolor_magenta/*5*/},
      {15, bcolor_cyan/*6*/},
      {16, bcolor_light_gray/*WHITE 7*/}
    }
  },
  {
    fcolor_green /*2*/,
    { {17, bcolor_black/*0*/},
      {18, bcolor_red/*1*/},
      {19, bcolor_green/*2*/},
      {20, COLOR_YELLOW/*bcolor_yellow 3*/},
      {21, bcolor_blue/*4*/},
      {22, bcolor_magenta/*5*/},
      {23, bcolor_cyan/*6*/},
      {24, bcolor_light_gray/*WHITE 7*/}
    }
  },
  {
    fcolor_yellow /*3*/,
    { {25, bcolor_black/*0*/},
      {26, bcolor_red/*1*/},
      {27, bcolor_green/*2*/},
      {28, COLOR_YELLOW/*bcolor_yellow 3*/},
      {29, bcolor_blue/*4*/},
      {30, bcolor_magenta/*5*/},
      {31, bcolor_cyan/*6*/},
      {32, bcolor_light_gray/*WHITE 7*/}
    }
  },
  {
    fcolor_blue /*4*/,
    { {33, bcolor_black/*0*/},
      {34, bcolor_red/*1*/},
      {35, bcolor_green/*2*/},
      {36, COLOR_YELLOW/*bcolor_yellow 3*/},
      {37, bcolor_blue/*4*/},
      {38, bcolor_magenta/*5*/},
      {39, bcolor_cyan/*6*/},
      {40, bcolor_light_gray/*WHITE 7*/}
    }
  },
  {
    fcolor_magenta /*5*/,
    { {41, bcolor_black/*0*/},
      {42, bcolor_red/*1*/},
      {43, bcolor_green/*2*/},
      {44, COLOR_YELLOW/*bcolor_yellow 3*/},
      {45, bcolor_blue/*4*/},
      {46, bcolor_magenta/*5*/},
      {47, bcolor_cyan/*6*/},
      {48, bcolor_light_gray/*WHITE 7*/}
    }
  },
  {
    fcolor_cyan /*6*/,
    { {49, bcolor_black/*0*/},
      {50, bcolor_red/*1*/},
      {51, bcolor_green/*2*/},
      {52, COLOR_YELLOW/*bcolor_yellow 3*/},
      {53, bcolor_blue/*4*/},
      {54, bcolor_magenta/*5*/},
      {55, bcolor_cyan/*6*/},
      {56, bcolor_light_gray/*WHITE 7*/}
    }
  },
  {
    fcolor_white /*7*/,
    { {57, bcolor_black/*0*/},
      {58, bcolor_red/*1*/},
      {59, bcolor_green/*2*/},
      {60, COLOR_YELLOW/*bcolor_yellow 3*/},
      {61, bcolor_blue/*4*/},
      {62, bcolor_magenta/*5*/},
      {63, bcolor_cyan/*6*/},
      {64, bcolor_light_gray/*WHITE 7*/}
    }
  }
};

static int
screen_map_get_pair(
  enum foreground const               i_fcolor,
  enum background const               i_bcolor)
{
  struct screen_map_bcolor const*     l_back;
  struct screen_pair_map const*       l_map;
  int                                 l_pair;

  l_map = &g_pair_map[i_fcolor];
  l_back = &l_map->m_bkgnd[i_bcolor];
  l_pair = (*l_back).m_pair;

  return l_pair;
}

static void
screen_init_pairs()
{
  struct screen_pair_map const*       l_map;
  int                                 l_slot_back;
  int                                 l_slot_fore;

  for (l_slot_fore = 0; 8 > l_slot_fore; l_slot_fore++)
  {
    l_map = &g_pair_map[l_slot_fore];

    for (l_slot_back = 0; 8 > l_slot_back; l_slot_back++)
    {
      init_pair(
        l_map->m_bkgnd[l_slot_back].m_pair,
        l_map->m_fcolor,
        l_map->m_bkgnd[l_slot_back].m_bcolor);

    }
  }

  return;
}

int
screen_init()
{
  int                                 l_exit;
  int                                 l_rc;

  l_exit = 0;

  initscr(); /* aborts on error */

  _video_cols = COLS;
  _video_rows = LINES;
  _video_has_color = has_colors();

  if (_video_has_color)
  {
    start_color();
    screen_init_pairs();
  }

  cbreak();
  noecho();
  nonl();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  meta(stdscr, FALSE);

#if defined(NCURSES_MOUSE_VERSION)
  mouse_initialized = 1;
  mousemask(
    (BUTTON1_PRESSED |
     BUTTON3_PRESSED |
     BUTTON1_RELEASED |
     BUTTON3_RELEASED |
     REPORT_MOUSE_POSITION), 0);

  /* appears to fix mouse presses (doesn't queue/try to combine) */
  mouseinterval(0);

  printf("\033[?1000h"); /* button pressed/released */
  /*printf("\033[?1002h");*/ /* button motion event */
  printf("\033[?1003h"); /* button motion highlight mode */
  fflush(stdout);
#endif

  return l_exit;
}

void
screen_deinit()
{

  endwin();

  return;
}

extern void
screen_fill(
  screen_char_t const                 i_char,
  enum foreground const               i_fcolor,
  enum background const               i_bcolor)
{
  int                                 l_pair;

  do
  {

    if (0 != i_char && 0x20 != i_char)
    {
      screen_fill_area(0, 0, _video_cols, _video_rows, i_char, i_fcolor, i_bcolor);
      break;
    }

    if (_video_has_color)
    {
      l_pair = screen_map_get_pair(i_fcolor, i_bcolor);
      bkgd(COLOR_PAIR(l_pair));
    }

    clear();

  }
  while (0);

  return;
}

extern void
screen_fill_area(
  int const                           i_col,
  int const                           i_row,
  unsigned int const                  i_columns,
  unsigned int const                  i_rows,
  screen_char_t const                 i_char,
  enum foreground const               i_fcolor,
  enum background const               i_bcolor)
{
  screen_char_t                       l_char;
  int                                 l_col;
  int                                 l_col_count;
  unsigned int                        l_columns;
  int                                 l_pair;
  unsigned int                        l_pos_x;
  unsigned int                        l_pos_y;
  int                                 l_row;
  int                                 l_row_count;
  unsigned int                        l_rows;
  int                                 l_scratch;

  l_col = i_col;
  l_columns = i_columns;
  l_row = i_row;
  l_rows = i_rows;

  do
  {

    if (0 == i_rows)
    {
      break;
    }

    if (0 > i_col)
    {
      l_scratch = l_columns - (0 - i_col);

      if (0 >= l_scratch)
      {
        break;
      }

      l_columns = l_scratch;
      l_col = 0;
    }

    if (l_col > _video_cols)
    {
      break;
    }

    if (l_col + l_columns > _video_cols)
    {
      l_columns = _video_cols - l_col;
    }

    if (0 == l_columns)
    {
      break;
    }

    if (0 > l_row)
    {
      l_scratch = l_rows - (0 - l_row);

      if (0 >= l_scratch)
      {
        break;
      }

      l_rows = l_scratch;
      l_row = 0;
    }

    l_char = (i_char) ? i_char : 0x20;

    if (_video_has_color)
    {
      l_pair = screen_map_get_pair(i_fcolor, i_bcolor);
      attron(COLOR_PAIR(l_pair));
    }

    l_pos_x = l_col;
    l_pos_y = l_row;
    l_row_count = 0;

    do
    {

      if (l_rows <= l_row_count)
      {
        break;
      }

      if (_video_rows <= l_pos_y)
      {
        break;
      }

      l_pos_x = l_col;
      l_col_count = 0;

      do
      {

        if (l_columns <= l_col_count)
        {
          break;
        }

        mvaddch(l_pos_y, l_pos_x, l_char);

        l_pos_x++;
        l_col_count++;

      }
      while (1);

      l_pos_y++;
      l_row_count++;

    }
    while (1);

  }
  while (0);

  return;
}

extern void
screen_putc(
  int const                           i_col,
  int const                           i_row,
  screen_char_t const                 i_char,
  unsigned int const                  i_repeat_count,
  enum foreground const               i_fcolor,
  enum background const               i_bcolor)
{

  screen_fill_area(i_col, i_row, i_repeat_count, 1, i_char, i_fcolor, i_bcolor);

  return;
}

extern void
screen_putvc(
  int const                           i_col,
  int const                           i_row,
  screen_char_t const                 i_char,
  unsigned int const                  i_repeat_count,
  enum foreground const               i_fcolor,
  enum background const               i_bcolor)
{

  screen_fill_area(i_col, i_row, 1, i_repeat_count, i_char, i_fcolor, i_bcolor);

  return;
}

extern void
screen_puts(
  int const                           i_col,
  int const                           i_row,
  unsigned char const*                i_string,
  unsigned int const                  i_length,
  enum foreground const               i_fcolor,
  enum background const               i_bcolor)
{
  int                                 l_col;
  unsigned int                        l_length;
  unsigned char const*                l_ptr;
  int                                 l_pair;

  l_col = i_col;
  l_length = i_length;
  l_ptr = i_string;

  do
  {

    if (0 == i_length)
    {
      break;
    }

    if (0 > i_row)
    {
      break;
    }

    if (_video_rows <= i_row)
    {
      break;
    }

    if (_video_cols <= i_col)
    {
      break;
    }

    if (0 > l_col)
    {
      do
      {
        l_ptr++;
        l_length--;
        l_col++;

        if (0 == l_length)
        {
          break;
        }

        if (0 == l_col)
        {
          break;
        }

      }
      while (1);
    }

    if (0 == l_length)
    {
      break;
    }

    if (_video_cols <= l_col + l_length)
    {
      l_length = _video_cols - l_col;
    }

    if (_video_has_color)
    {
      l_pair = screen_map_get_pair(i_fcolor, i_bcolor);
      attron(COLOR_PAIR(l_pair));
    }

    mvaddnstr(i_row, i_col, (char*)l_ptr, l_length);

  }
  while (0);

  return;
}

extern void
screen_set_color(
  int const                           i_col,
  int const                           i_row,
  unsigned int const                  i_repeat_count,
  enum foreground const               i_fcolor,
  enum background const               i_bcolor)
{
  int                                 l_col;
  unsigned int                        l_columns;
  int                                 l_count;
  int                                 l_pair;

  l_col = i_col;
  l_columns = i_repeat_count;

  do
  {

    if (0 == l_columns)
    {
      break;
    }

    if (0 > i_row)
    {
      break;
    }

    if (_video_rows <= i_row)
    {
      break;
    }

    if (_video_cols <= i_col)
    {
      break;
    }

    if (0 > i_col)
    {
      l_count = i_repeat_count - (0 - i_col);

      if (0 >= l_count)
      {
        break;
      }

      l_columns = l_count;
      l_col = 0;
    }

    if (_video_has_color)
    {
      l_pair = screen_map_get_pair(i_fcolor, i_bcolor);
    }

    mvchgat(i_row, l_col, l_columns, A_NORMAL, l_pair, 0);

  }
  while (0);

  return;
}

extern screen_block_t
screen_getc(
  int const                           i_col,
  int const                           i_row)
{
  screen_block_t                      l_info;

  l_info = mvinch(i_row, i_col);

  return l_info;
}

extern screen_block_t*
screen_read(
  screen_block_t*                     o_block,
  unsigned int const                  i_columns,
  int const                           i_col,
  int const                           i_row)
{
  unsigned int                        l_left;
  int                                 l_col;
  screen_block_t*                     l_next;

  l_col = i_col;
  l_left = i_columns;
  l_next = o_block;

  if (0 <= i_row && _video_rows > i_row)
  {
    while (l_left && 0 > l_col)
    {
      l_col++;
      l_left--;
    }

    for (; l_left && _video_cols > l_col; l_col++, l_left--)
    {
      *l_next++ = mvinch(i_row, l_col);
    }
  }

  return l_next;
}

extern screen_block_t*
screen_readv(
  screen_block_t*                     o_block,
  unsigned int const                  i_rows,
  int const                           i_col,
  int const                           i_row)
{
  unsigned int                        l_left;
  screen_block_t*                     l_next;
  int                                 l_row;

  l_row = i_row;
  l_left = i_rows;
  l_next = o_block;

  if (0 <= i_col && _video_cols > i_col)
  {
    while (l_left && 0 > i_row)
    {
      l_row++;
      l_left--;
    }

    for (; l_left && _video_rows > l_row; l_row++, l_left--)
    {
      *l_next++ = mvinch(l_row, i_col);
    }

  }

  return l_next;
}

extern screen_block_t const*
screen_write(
  int const                           i_col,
  int const                           i_row,
  screen_block_t const*               i_block,
  unsigned int const                  i_columns)
{
  unsigned int                        l_left;
  int                                 l_col;
  screen_block_t const*               l_next;

  l_col = i_col;
  l_left = i_columns;
  l_next = i_block;

  if (0 <= i_row && _video_rows > i_row)
  {
    while (l_left && 0 > l_col)
    {
      l_col++;
      l_left--;
    }

    for (; l_left && _video_cols > l_col; l_col++, l_left--)
    {
      mvaddch(i_row, l_col, *l_next++);
    }
  }

  return l_next;
}

extern screen_block_t const*
screen_writev(
  int const                           i_col,
  int const                           i_row,
  screen_block_t const*               i_block,
  unsigned int const                  i_rows)
{
  unsigned int                        l_left;
  screen_block_t const*               l_next;
  int                                 l_row;

  l_row = i_row;
  l_left = i_rows;
  l_next = i_block;

  if (0 <= i_col && _video_cols > i_col)
  {
    while (l_left && 0 > i_row)
    {
      l_row++;
      l_left--;
    }

    for (; l_left && _video_rows > l_row; l_row++, l_left--)
    {
      mvaddch(l_row, i_col, *l_next++);
    }

  }

  return l_next;
}

extern void
screen_push(
  int const                           i_copy)
{
  return;
}

extern void
screen_pop()
{
  refresh();
  return;
}
