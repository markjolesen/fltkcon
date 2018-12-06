// drvgr.cxx
//
// Fl_Graphics_Driver class for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The FDOSTUI authors
// Copyright 2017-2018 The fltkal authors
// Copyright 2010-2018 by Bill Spitzak and others.
//
//                              FLTK License
//                            December 11, 2001
//
// The FLTK library and included programs are provided under the terms
// of the GNU Library General Public License (LGPL) with the following
// exceptions:
//
//     1. Modifications to the FLTK configure script, config
//        header file, and makefiles by themselves to support
//        a specific platform do not constitute a modified or
//        derivative work.
//
//       The authors do request that such modifications be
//       contributed to the FLTK project - send all contributions
//       through the "Software Trouble Report" on the following page:
//
//            http://www.fltk.org/str.php
//
//     2. Widgets that are subclassed from FLTK widgets do not
//        constitute a derivative work.
//
//     3. Static linking of applications and widgets to the
//        FLTK library does not constitute a derivative work
//        and does not require the author to provide source
//        code for the application or widget, use the shared
//        FLTK libraries, or link their applications or
//        widgets against a user-supplied version of FLTK.
//
//        If you link the application or widget to a modified
//        version of FLTK, then the changes to FLTK must be
//        provided under the terms of the LGPL in sections
//        1, 2, and 4.
//
//     4. You do not have to provide a copy of the FLTK license
//        with programs that are linked to the FLTK library, nor
//        do you have to identify the FLTK license in your
//        program or documentation as required by section 6
//        of the LGPL.
//
//        However, programs must still identify their use of FLTK.
//        The following example statement can be included in user
//        documentation to satisfy this requirement:
//
//            [program/widget] is based in part on the work of
//            the FLTK project (http://www.fltk.org).
//
//     This library is free software; you can redistribute it and/or
//     modify it under the terms of the GNU Library General Public
//     License as published by the Free Software Foundation; either
//     version 2 of the License, or (at your option) any later version.
//
//     This library is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//     Library General Public License for more details.
//
//     You should have received a copy of the GNU Library General Public
//     License along with FLTK.  If not, see <http://www.gnu.org/licenses/>.
//
#include <stdio.h> // remove
#include <string.h> // remove
#include "drvgr.h"
#include "fl_clip.h"
#include "fl_skin.h"
#include "screen.h"

extern "C" void mouse_show();
extern "C" void mouse_hide();

inline static enum foreground
_FG(enum Fl::foreground const fcolor)
{
  return static_cast<enum foreground>(fcolor);
}

inline static enum background
_BG(enum Fl::background const bcolor)
{
  return static_cast<enum background>(bcolor);
}

int Fl_Graphics_Driver::draw_offset_x = 0;
int Fl_Graphics_Driver::draw_offset_y = 0;

Fl_Graphics_Driver&
Fl_Graphics_Driver::instance()
{
  static Fl_Graphics_Driver driver;
  return driver;
}

Fl_Graphics_Driver::Fl_Graphics_Driver() :
  stack_top_(0)
{
  return;
}

bool
Fl_Graphics_Driver::clip_intersects(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y) const
{
  bool l_intersects = true;

  do
  {

    if (0 == stack_top_)
    {
      break;
    }

    struct region const* l_region = &stack_[(stack_top_ - 1)];

    if ((*l_region).m_is_empty)
    {
      break;
    }

    l_intersects = false;

    int l_pos_x = (i_pos_x + draw_offset_x);
    int l_pos_x2 = (l_pos_x + i_len_x);

    if (l_pos_x2 < (*l_region).m_pos_x)
    {
      break;
    }

    l_pos_x2 = ((*l_region).m_pos_x + (*l_region).m_len_x);

    if (l_pos_x > l_pos_x2)
    {
      break;
    }

    int l_pos_y = (i_pos_y + draw_offset_y);
    int l_pos_y2 = (l_pos_y + i_len_y);

    if (l_pos_y2 < (*l_region).m_pos_y)
    {
      break;
    }

    l_pos_y2 = ((*l_region).m_pos_y + (*l_region).m_len_y);

    if (l_pos_y > l_pos_y2)
    {
      break;
    }

    l_intersects = true;

  }
  while (0);

  return l_intersects;
}

bool
Fl_Graphics_Driver::clip_box(
  int& o_pos_x,
  int& o_pos_y,
  unsigned int& o_len_x,
  unsigned int& o_len_y,
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y) const
{
  struct region l_region;

  l_region.m_is_empty = false;
  l_region.m_pos_x = 0;
  l_region.m_pos_y = 0;
  l_region.m_len_x = _video_cols;
  l_region.m_len_y = _video_rows;

  o_pos_x = (i_pos_x + draw_offset_x);
  o_pos_y = (i_pos_y + draw_offset_y);
  o_len_x = i_len_x;
  o_len_y = i_len_y;

  do
  {

    if (0 == i_len_x || 0 == i_len_y)
    {
      break;
    }

    if (stack_top_)
    {
      struct region const* l_top = &stack_[(stack_top_ - 1)];

      if (false == (*l_top).m_is_empty)
      {
        l_region.m_pos_x = (*l_top).m_pos_x;
        l_region.m_pos_y = (*l_top).m_pos_y;
        l_region.m_len_x = (*l_top).m_len_x;
        l_region.m_len_y = (*l_top).m_len_y;
      }
    }

    int l_pos_x2 = (l_region.m_pos_x + l_region.m_len_x - 1);

    if (o_pos_x < l_region.m_pos_x)
    {
      int l_len_x = (i_len_x - (l_region.m_pos_x - o_pos_x));

      if (0 <= l_len_x)
      {
        o_len_x = static_cast<unsigned int>(l_len_x);
      }

      else
      {
        o_len_x = 0;
      }

      o_pos_x = l_region.m_pos_x;
    }

    else if (o_pos_x > l_pos_x2)
    {
      o_len_x = 0;
      o_pos_x = l_pos_x2;
    }

    if (o_len_x && ((int)(o_pos_x + o_len_x) > l_pos_x2))
    {
      o_len_x = 1 + (l_pos_x2 - o_pos_x);
    }

    int l_pos_y2 = (l_region.m_pos_y + l_region.m_len_y - 1);

    if (o_pos_y < l_region.m_pos_y)
    {
      int l_len_y = (i_len_y - (l_region.m_pos_y - o_pos_y));

      if (0 <= l_len_y)
      {
        o_len_y = static_cast<unsigned int>(l_len_y);
      }

      else
      {
        o_len_y = 0;
      }

      o_pos_y = l_region.m_pos_y;
    }

    else if (o_pos_y > l_pos_y2)
    {
      o_len_y = 0;
      o_pos_x = l_pos_x2;
    }

    if (o_len_y && ((int)(o_pos_y + o_len_y) > l_pos_y2))
    {
      o_len_y = 1 + (l_pos_y2 - o_pos_y);
    }

  }
  while (0);

  bool l_changed = (((i_pos_x + draw_offset_x) != o_pos_x) ||
                    ((i_pos_y + draw_offset_y) != o_pos_y) ||
                    (i_len_x != o_len_x) ||
                    (i_len_y != o_len_y));

  return l_changed;
}

void
Fl_Graphics_Driver::clip_pop()
{

  if (stack_top_)
  {
    stack_top_--;
  }

  return;
}

void
Fl_Graphics_Driver::clip_push(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y)
{

  do
  {

    if (stack_max <= stack_top_)
    {
      break;
    }

    int                                 l_pos_x;
    int                                 l_pos_y;
    unsigned int                        l_len_x;
    unsigned int                        l_len_y;

    clip_box(l_pos_x, l_pos_y, l_len_x, l_len_y,
             i_pos_x, i_pos_y, i_len_x, i_len_y);

    struct region* l_region = &stack_[stack_top_];

    (*l_region).m_is_empty = false;
    (*l_region).m_pos_x = l_pos_x;
    (*l_region).m_pos_y = l_pos_y;
    (*l_region).m_len_x = l_len_x;
    (*l_region).m_len_y = l_len_y;
    stack_top_++;

  }
  while (0);

  return;
}

void
Fl_Graphics_Driver::clip_push_none()
{

  do
  {

    if (stack_max <= stack_top_)
    {
      break;
    }

    struct region* l_region = &stack_[stack_top_];

    (*l_region).m_is_empty = true;

    (*l_region).m_pos_x = 0;

    (*l_region).m_pos_y = 0;

    (*l_region).m_len_x = 0;

    (*l_region).m_len_y = 0;

    stack_top_++;

  }
  while (0);

  return;
}

void
Fl_Graphics_Driver::clip_replace(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y)
{

  do
  {

    if (0 == stack_top_)
    {
      clip_push(i_pos_x, i_pos_y, i_len_x, i_len_y);
      break;
    }

    int                                 l_pos_x;
    int                                 l_pos_y;
    unsigned int                        l_len_x;
    unsigned int                        l_len_y;

    clip_box( l_pos_x, l_pos_y, l_len_x, l_len_y,
              i_pos_x, i_pos_y, i_len_y, i_len_x);

    struct region* l_region = &stack_[stack_top_];

    (*l_region).m_is_empty = false;
    (*l_region).m_pos_x = l_pos_x;
    (*l_region).m_pos_y = l_pos_y;
    (*l_region).m_len_x = l_len_x;
    (*l_region).m_len_y = l_len_y;

  }
  while (0);

  return;
}

void
Fl_Graphics_Driver::draw_fill(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned int const i_char,
  enum Fl::foreground const i_fcolor,
  enum Fl::background const i_bcolor) const
{
  int l_pos_x;
  int l_pos_y;
  unsigned int l_len_x;
  unsigned int l_len_y;

  clip_box(l_pos_x, l_pos_y, l_len_x, l_len_y,
           i_pos_x, i_pos_y, i_len_x, i_len_y);

  if (l_len_x && l_len_y)
  {
    ::screen_fill_area(l_pos_x, l_pos_y, l_len_x, l_len_y,
                       i_char, _FG(i_fcolor), _BG(i_bcolor));
  }

  return;
}

void
Fl_Graphics_Driver::draw_putc(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_char,
  unsigned int const i_repeat_count,
  enum Fl::foreground const i_fcolor,
  enum Fl::background const i_bcolor) const
{
  int l_pos_x;
  int l_pos_y;
  unsigned int l_len_x;
  unsigned int l_len_y;

  clip_box(l_pos_x, l_pos_y, l_len_x, l_len_y,
           i_pos_x, i_pos_y, i_repeat_count, 1);

  if (l_len_x && l_len_y)
  {
    ::screen_putc(l_pos_x, l_pos_y, i_char, l_len_x, _FG(i_fcolor),
                  _BG(i_bcolor));
  }

  return;
}

void
Fl_Graphics_Driver::draw_putvc(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_char,
  unsigned int const i_repeat_count,
  enum Fl::foreground const i_fcolor,
  enum Fl::background const i_bcolor) const
{
  int l_pos_x;
  int l_pos_y;
  unsigned int l_len_x;
  unsigned int l_len_y;

  clip_box(l_pos_x, l_pos_y, l_len_x, l_len_y,
           i_pos_x, i_pos_y, 1, i_repeat_count);

  if (l_len_x && l_len_y)
  {
    screen_putvc(l_pos_x, l_pos_y, i_char, l_len_y,
                 _FG(i_fcolor), _BG(i_bcolor));
  }

  return;
}

void
Fl_Graphics_Driver::draw_puts(
  int const i_pos_x,
  int const i_pos_y,
  unsigned char const* i_string,
  unsigned int const i_length,
  enum Fl::foreground const i_fcolor,
  enum Fl::background const i_bcolor) const
{
  int l_pos_x;
  int l_pos_y;
  unsigned int l_len_x;
  unsigned int l_len_y;
  unsigned int l_offset = 0;

  clip_box(l_pos_x, l_pos_y, l_len_x, l_len_y,
           i_pos_x, i_pos_y, i_length, 1);

  if (l_len_x && l_len_y)
  {
    if (0 > i_pos_x)
    {
      l_offset = (i_length - l_len_x);
    }

    ::screen_puts(l_pos_x, l_pos_y, i_string + l_offset, l_len_x,
                  _FG(i_fcolor), _BG(i_bcolor));
  }

}

void
Fl_Graphics_Driver::draw_frame(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  enum Fl::foreground const i_fcolor,
  enum Fl::background const i_bcolor) const
{
  unsigned int l_right;
  unsigned int l_bottom;

  l_right = (i_pos_x + i_len_x - 1);
  l_bottom = (i_pos_y + i_len_y - 1);

  draw_putc(i_pos_x, i_pos_y, Fl::_skin_sym.hline, i_len_x, i_fcolor, i_bcolor);

  draw_putc(i_pos_x, l_bottom, Fl::_skin_sym.hline, i_len_x, i_fcolor, i_bcolor);

  draw_putvc(i_pos_x, i_pos_y, Fl::_skin_sym.vline, i_len_y, i_fcolor, i_bcolor);

  draw_putc(i_pos_x, i_pos_y, Fl::_skin_sym.top_left, 1, i_fcolor, i_bcolor);

  draw_putc(i_pos_x, l_bottom, Fl::_skin_sym.bottom_left, 1, i_fcolor, i_bcolor);

  draw_putvc(l_right, i_pos_y, Fl::_skin_sym.vline, i_len_y, i_fcolor, i_bcolor);

  draw_putc(l_right, i_pos_y, Fl::_skin_sym.top_right, 1, i_fcolor, i_bcolor);

  draw_putc(l_right, l_bottom, Fl::_skin_sym.bottom_right, 1, i_fcolor, i_bcolor);

  return;
}

void
Fl_Graphics_Driver::flip_to_offscreen(bool i_copy) const
{
  screen_push(i_copy);
}

void
Fl_Graphics_Driver::flip_to_onscreen() const
{
  screen_pop();
}

void
Fl_Graphics_Driver::mouse_hide()
{
#if !defined(__CURSES__)
  ::mouse_hide();
#endif
  return;
}

void
Fl_Graphics_Driver::mouse_show()
{
#if !defined(__CURSES__)
  ::mouse_show();
#endif
  return;
}
