// inpfile.cxx
//
// File_Input header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2010 by Bill Spitzak and others.
// Original version Copyright 1998 by Curtis Edwards.
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
#include <stdio.h>
#include "inpfile.h"
#include "drvsys.h"
#include "fl.h"
#include "fl_draw.h"
#include "filename.h"
#include "flstring.h"
#include "win.h"

#define FL_DAMAGE_BAR 0x10

Fl_File_Input::Fl_File_Input(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label) :
  Fl_Input(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label)
{
  buttons_[0] = 0;
  ok_entry_   = 1;
  pressed_    = -1;
}

void
Fl_File_Input::draw_buttons()
{
  int i, X;
  int l_pos_x;
  unsigned int l_len_x;

  if (damage() & (FL_DAMAGE_BAR | FL_DAMAGE_ALL))
  {
    update_buttons();
  }

  Fl::draw_putc(
    x_,
    y_ - 1,
    '-',
    w_,
    Fl::fcolor_black,
    Fl::bcolor_light_gray);

  for (X = 0, i = 0; buttons_[i]; i ++)
  {
    if ((X + buttons_[i]) > cur_->m_offset_x)
    {
      if (X < cur_->m_offset_x)
      {
        l_pos_x = x_;
        l_len_x = X + buttons_[i] - cur_->m_offset_x;
      }

      else if ((X + buttons_[i] - cur_->m_offset_x) > w())
      {
        l_pos_x = x_ + X - cur_->m_offset_x;
        l_len_x = w_ - X + cur_->m_offset_x;
      }

      else
      {
        l_pos_x = x_ + X - cur_->m_offset_x;
        l_len_x = buttons_[i];
      }

      Fl::draw_putc(
        l_pos_x + 1,
        y_ - 1,
        '-',
        l_len_x - 1,
        Fl::fcolor_black,
        Fl::bcolor_light_gray);

      Fl::draw_putc(
        l_pos_x + l_len_x - 1,
        y_ - 1,
        '|',
        1,
        Fl::fcolor_black,
        Fl::bcolor_light_gray);

    }

    X += buttons_[i];
  }

  return;
}

void
Fl_File_Input::update_buttons()
{
  int   i;
  unsigned char const* start;
  unsigned char const* end;

  for (i = 0, start = value();
       start && i < (int)(sizeof(buttons_) / sizeof(buttons_[0]) - 1);
       start = end, i ++)
  {
    if ((end = Fl::system_driver()->next_dir_sep(start)) == NULL)
      break;

    end ++;

    buttons_[i] = (short)(end - start);

  }

  buttons_[i] = 0;

  return;
}


void
Fl_File_Input::value(const unsigned char* str, int len)
{
  damage(FL_DAMAGE_BAR);
  Fl_Input::value(str, len);
  return;
}


void
Fl_File_Input::value(const unsigned char* str)
{
  damage(FL_DAMAGE_BAR);
  Fl_Input::value(str);
  return;
}

void
Fl_File_Input::draw()
{

  if (damage() & (FL_DAMAGE_BAR | FL_DAMAGE_ALL)) draw_buttons();

  Fl_Input_::drawtext(x_, y_, w_, h_);

  return;
}

bool
Fl_File_Input::handle(enum Fl_Event const event)
{
  static char inButtonBar = 0;

  switch (event)
  {
    case FL_MOVE :
    case FL_ENTER :
      return 1;

    case FL_PUSH :
      inButtonBar = (Fl::event_y() < y());

    case FL_RELEASE :
    case FL_DRAG :
      if (inButtonBar)
        return handle_button(event);

      else
        return Fl_Input::handle(event);

    default :
    {
      Fl_Widget_Tracker wp(this);

      if (Fl_Input::handle(event))
      {
        if (wp.exists())
          damage(FL_DAMAGE_BAR);

        return 1;
      }
    }

    return 0;
  }
}


int
Fl_File_Input::handle_button(int event)
{
  int i, X;
  unsigned char* start;
  unsigned char const* end;
  unsigned char newvalue[FL_PATH_MAX];


  for (X = 0, i = 0; buttons_[i]; i ++)
  {
    X += buttons_[i];

    if (X > cur_->m_offset_x && Fl::event_x() < (x() + X - cur_->m_offset_x)) break;
  }

  if (event == FL_RELEASE) pressed_ = -1;

  else pressed_ = (short)i;

  window()->make_current();
  draw_buttons();

  if (!buttons_[i] || event != FL_RELEASE) return 1;

  strlcpy((char*)newvalue, (char*)value(), sizeof(newvalue));

  for (start = newvalue, end = start; start
       && i >= 0; start = const_cast<unsigned char*>(end), i --)
  {
    if ((end = Fl::system_driver()->next_dir_sep(start)) == NULL)
      break;

    end ++;
  }

  if (i < 0)
  {
    *start = '\0';
    value(newvalue, (int) (start - newvalue) );

    set_changed();

    if (when() & (FL_WHEN_CHANGED | FL_WHEN_RELEASE) ) do_callback();
  }

  return 1;
}
