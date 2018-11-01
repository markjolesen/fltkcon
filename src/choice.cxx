// choice.cxx
//
// Choice widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2015 by Bill Spitzak and others.
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
#include "choice.h"
#include "fl.h"
#include "fl_clip.h"
#include "fl_draw.h"
#include "fl_rend.h"
#include "flstring.h"

void
Fl_Choice::draw()
{

  Fl::draw_fill(x_, y_, w_, h_, 0x20, skin_.menu_fcolor, skin_.menu_bcolor);

  if (mvalue())
  {
    Fl_Menu_Item m = *mvalue();

    if (active_r()) m.activate();

    else m.deactivate();

    Fl::clip_push(x_, y_, w_, h_);

    fl_draw_shortcut = 2;
    m.draw(x_, y_, w_, h_, this, Fl::focus() == this);
    fl_draw_shortcut = 0;

    Fl::clip_pop();
  }

  Fl::draw_putc(x_ + w_ - 1, y_, Fl::_skin_sym.down, 1,
                skin_.normal_fcolor, skin_.normal_bcolor);

  draw_label();
}

Fl_Choice::Fl_Choice(int const X, int const Y, unsigned int const W,
                     unsigned int const H, const unsigned char* L)
  : Fl_Menu_(X, Y, W, H, L)
{
  align(Fl_Label::FL_ALIGN_LEFT);
  when(FL_WHEN_RELEASE);
}

int
Fl_Choice::value(const Fl_Menu_Item* v)
{
  if (!Fl_Menu_::value(v)) return 0;

  redraw();
  return 1;
}

int
Fl_Choice::value(int v)
{
  if (v == -1) return value((const Fl_Menu_Item*)0);

  if (v < 0 || v >= (size() - 1)) return 0;

  if (!Fl_Menu_::value(v)) return 0;

  redraw();
  return 1;
}

bool
Fl_Choice::handle(enum Fl_Event const e)
{
  if (!menu() || !menu()->text) return 0;

  const Fl_Menu_Item* v;

  switch (e)
  {
    case FL_ENTER:
    case FL_LEAVE:
      return 1;

    case FL_KEYBOARD:
      if (Fl::event_key() != ' ' ||
          (Fl::event_state() & (FL_SHIFT | FL_CTRL | FL_ALT | FL_META))) return 0;

    case FL_PUSH:
      if (Fl::visible_focus()) Fl::focus(this);

    J1:
      v = menu()->pulldown(x(), y(), w(), h(), mvalue(), this);

      if (!v || v->submenu()) return 1;

      if (v != mvalue()) redraw();

      picked(v);
      return 1;

    case FL_SHORTCUT:
      if (Fl_Widget::test_shortcut()) goto J1;

      v = menu()->test_shortcut();

      if (!v) return 0;

      if (v != mvalue()) redraw();

      picked(v);
      return 1;

    case FL_FOCUS:
    case FL_UNFOCUS:
      if (Fl::visible_focus())
      {
        redraw();
        return 1;
      }

      else return 0;

    default:
      return 0;
  }
}
