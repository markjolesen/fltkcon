// wm.cxx
//
// Window manager for the Fast Light Tool Kit (FLTK)
//
// Copyright 2018 The fltkcon authors
// Copyright 2018 The fltkal authors
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
#include "wm.h"
#include "fl.h"
#include "platform.h"

wm::wm()
{
}

wm::~wm()
{
}

wm::hit_type
wm::hit(Fl_Window& window, int const x, int const y) const
{
  hit_type what = HIT_NONE;

  do
  {

    int top = window.y() - 1;
    int left = window.x() - 1;
    int right = window.x() + window.w();
    int bottom = window.y() + window.h();

    if (x == left)
    {
      if (y == top)
      {
        what = HIT_NORTH_WEST;
      }

      else if (y == bottom)
      {
        what = HIT_SOUTH_WEST;
      }

      else if (y > top && y < bottom)
      {
        what = HIT_WEST;
      }

      break;
    }

    if (x == right)
    {
      if (y == top)
      {
        what = HIT_NORTH_EAST;
      }

      else if (y == bottom)
      {
        what = HIT_SOUTH_EAST;
      }

      else if (y > top && y < bottom)
      {
        what = HIT_EAST;
      }

      break;
    }

    what = HIT_WINDOW;

    if ((x >= left) && (x <= right))
    {
      if (y == top)
      {
        what = HIT_MOVE;
      }

      else if (y == bottom)
      {
        what = HIT_SOUTH;
      }
    }

  }
  while (0);

  return what;
}

bool
wm::handle_mouse(Fl_Window& window, hit_type const what, int const x,
                int const y) const
{
  bool handled = false;

  do
  {

    if (!(FL_WINDOW == window.type() || FL_DOUBLE_WINDOW == window.type()))
    {
      break;
    }

    if (HIT_WINDOW == what)
    {
      break;
    }

    if (HIT_NONE != what)
    {
      if (window.modal() && HIT_MOVE != what)
      {
        break;
      }

      handle_push(window, what);
      handled = true;
      break;
    }

    if (window.modal())
    {
      break;
    }

    Fl_X* i;

    for (Fl_X** pp = &Fl_X::first; (i = *pp); pp = &i->next)
    {
      Fl_Window* wi = i->w;

      if (wi != &window)
      {
        hit_type what2 =  hit((*wi), x, y);

        if (what2)
        {
          *pp = i->next;
          i->next = Fl_X::first;
          Fl_X::first = i;
          wi->take_focus();
          break;
        }
      }
    }

  }
  while (0);

  return handled;
}
