// btnmenu.cxx
//
// Menu button widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2010 by Bill Spitzak and others.
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
#include "fl.h"
#include "btnmenu.h"
#include "fl_draw.h"
#include "fl_caret.h"

static Fl_Menu_Button* pressed_menu_button_ = 0;

void
Fl_Menu_Button::draw()
{
  if (type()) return;

  int H = 1;
  int X = x() + w();
  int Y = y() + (h() - H) / 2;

  draw_label(x(), y(), X - x() + 2, h());
  Fl::draw_putc(x_ + w_, y_, Fl::_skin_sym.down, 1, skin_.normal_fcolor,
                skin_.normal_bcolor);

  return;
}

const Fl_Menu_Item*
Fl_Menu_Button::popup()
{
  const Fl_Menu_Item* m;
  pressed_menu_button_ = this;
  redraw();
  Fl_Widget_Tracker mb(this);

  if (type())
  {
    m = menu()->popup(Fl::event_x(), Fl::event_y(), label(), mvalue(), this);
  }

  else
  {
    m = menu()->pulldown(x(), y(), w(), h(), 0, this);
  }

  picked(m);
  pressed_menu_button_ = 0;

  if (mb.exists()) redraw();

  return m;
}

bool
Fl_Menu_Button::handle(
  enum Fl_Event const e)
{
  if (!menu() || !menu()->text) return 0;

  switch (e)
  {
    case FL_ENTER:
    case FL_LEAVE:
      return (!type()) ? 1 : 0;

    case FL_PUSH:
      if (type())
      {
        if (!(type() & (1 << (Fl::event_button() - 1)))) return 0;
      }

      if (Fl::visible_focus()) Fl::focus(this);

      popup();
      return 1;

    case FL_KEYBOARD:
      if (Fl::event_key() == ' ' &&
          !(Fl::event_state() & (FL_SHIFT | FL_CTRL | FL_ALT | FL_META)))
      {
        popup();
        return 1;
      }

      else return 0;

    case FL_SHORTCUT:
      if (Fl_Widget::test_shortcut())
      {
        popup();
        return 1;
      }

      return test_shortcut() != 0;

    case FL_FOCUS:
      Fl::caret_hide();

    case FL_UNFOCUS:
      if (Fl::visible_focus())
      {
        redraw();
        return 1;
      }

    default:
      return 0;
  }

  return 0;
}

Fl_Menu_Button::Fl_Menu_Button(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label) :
  Fl_Menu_(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label)
{
}
