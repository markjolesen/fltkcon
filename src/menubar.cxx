// menubar.cxx
//
// Menu bar widget for the Fast Light Tool Kit (FLTK).
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
#include "menubar.h"
#include "fl.h"
#include "fl_draw.h"
#include "fl_rend.h"

void
Fl_Menu_Bar::draw()
{

  Fl::draw_fill(x_, y_, w_, h_, 0x20, skin_.menu_fcolor, skin_.menu_bcolor);

  if (!menu() || !menu()->text) return;

  const Fl_Menu_Item* m;
  int X = (x_ + 1);

  for (m = menu()->first(); m->text; m = m->next())
  {
    int W = m->measure(0, this) + 1;
    m->draw(X, y(), W, h(), this);
    X += W;
  }

  return;
}

bool
Fl_Menu_Bar::handle(
  enum Fl_Event const event)
{
  const Fl_Menu_Item* v;

  if (menu() && menu()->text) switch (event)
    {
      case FL_ENTER:
      case FL_LEAVE:
        return 1;

      case FL_PUSH:
        v = 0;
      J1:
        v = menu()->pulldown(x(), y(), w(), h(), v, this, 0, 1);
        picked(v);
        return 1;

      case FL_SHORTCUT:
        if (visible_r())
        {
          v = menu()->find_shortcut(0, true);

          if (v && v->submenu()) goto J1;
        }

        return test_shortcut() != 0;
    }

  return 0;
}

Fl_Menu_Bar::Fl_Menu_Bar(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label) :
  Fl_Menu_(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label)
{
}
