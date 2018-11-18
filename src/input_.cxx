// input_.cxx
//
// Common input widget routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2011, 2018 by Bill Spitzak and others.
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
//
#include <stdlib.h>
#include <ctype.h>
#include "input_.h"
#include "drvscr.h"
#include "fl.h"
#include "fl_ask.h"
#include "fl_caret.h"
#include "fl_clip.h"
#include "fl_draw.h"
#include "fl_rend.h"
#include "flstring.h"
#include "fl_utf8.h"
#include "win.h"

Fl_Input_::Fl_Input_(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label) :
  Fl_Widget(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label),
  cur_(inpcur_new(i_len_x, i_len_y))
{
  return;
}

Fl_Input_::~Fl_Input_()
{
  inpcur_free(cur_);
}

void
Fl_Input_::resize(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y)
{
  inpcur_resize(cur_, i_len_x, i_len_y);
  Fl_Widget::resize(i_pos_x, i_pos_y, i_len_x, i_len_y);
}

void
Fl_Input_::value(
  unsigned char const* i_buf,
  size_t const i_size)
{
  size_t l_size;
  struct inpbuf* l_buf;

  inplist_clear((*cur_).m_list);

  (*cur_).m_active = 0;
  (*cur_).m_caret_x = 0;
  (*cur_).m_slot_x = 0;
  (*cur_).m_sticky_x = 0;
  (*cur_).m_caret_y = 0;
  (*cur_).m_offset_x = 0;
  (*cur_).m_offset_y = 0;

  unsigned char const* l_head = i_buf;
  unsigned char const* l_tail = i_buf;
  size_t l_left = i_size;

  while (l_left)
  {
    if (0 == *l_tail)
    {
      break;
    }

    if ('\n' == *l_tail)
    {
      l_size = l_tail - l_head;
      l_buf = inplist_append((*cur_).m_list);
      inpbuf_copy(l_buf, l_head, l_size);
      l_head = l_tail + 1;
    }

    l_tail++;
    l_left--;
  }

  if (l_head != l_tail)
  {
    l_size = l_tail - l_head;
    l_buf = inplist_append((*cur_).m_list);
    inpbuf_copy(l_buf, l_head, l_size);
  }

  (*cur_).m_active = (*cur_).m_list->m_head;

  return;
}

void
Fl_Input_::drawtext(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int i_len_x,
  unsigned int i_len_y) const
{

  do
  {

    if (this == Fl::focus())
    {
      if (false == Fl::_caret_is_visible)
      {
        if (Fl::CARET_OVERWRITE == Fl::_caret_mode)
        {
          Fl::caret_block();
        }

        else
        {
          Fl::caret_underline();
        }
      }

      Fl::caret_set_position((x_ + (*cur_).m_caret_x), (y_ + (*cur_).m_caret_y));
    }

    Fl::draw_fill(i_pos_x, i_pos_y, i_len_x, i_len_y, 0x20,
                  Fl::fcolor_white, Fl::bcolor_black);

    Fl::clip_push(i_pos_x, i_pos_y, i_len_x, i_len_y);

    struct inpbuf* l_buf = inplist_nth((*cur_).m_list, (*cur_).m_offset_y);
    unsigned int l_line = 0;

    do
    {

      if (0 == l_buf)
      {
        break;
      }

      if (i_len_y <= l_line)
      {
        break;
      }

      if ((*cur_).m_offset_x < (*l_buf).m_length)
      {
        unsigned char const* l_data = &(*l_buf).m_data[(*cur_).m_offset_x];
        Fl::draw_puts(
          x_,
          y_ + l_line,
          l_data,
          (*l_buf).m_length - (*cur_).m_offset_x,
          Fl::fcolor_white,
          Fl::bcolor_black);
      }

      l_line++;
      l_buf = (*l_buf).m_next;

    }
    while (1);

    Fl::clip_pop();

  }
  while (0);

  return;
}

bool
Fl_Input_::handletext(
  enum Fl_Event const i_event,
  int const i_pos_x,
  int const i_pos_y,
  unsigned int i_len_x,
  unsigned int i_len_y)
{
  bool l_handled = true;

  switch (i_event)
  {
    case FL_FOCUS:
      damage(Fl_Widget::FL_DAMAGE_USER1);
      break;

    case FL_UNFOCUS:
    case FL_HIDE:
      break;

    default:
      l_handled = false;
      break;
  }

  return l_handled;
}
