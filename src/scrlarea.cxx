// scrlarea.cxx
//
// Scrolling routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2016 by Bill Spitzak and others.
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
#include "drvwin.h"
#include "fl_rend.h"

void
fl_scroll(
  int const X,
  int const Y,
  unsigned int const W,
  unsigned int const H,
  int const dx,
  int const dy,
  void (*draw_area)(
    void*,
    int const,
    int const,
    unsigned int const,
    unsigned int const,
    enum Fl::foreground const fcolor,
    enum Fl::background const bcolor),
  void* data,
  struct Fl::skin_widget const& skin)
{
  if (!dx && !dy) return;

  if (dx <= -(int)W || dx >= (int)W || dy <= -(int)H || dy >= (int)H)
  {
    draw_area(data, X, Y, W, H, skin.normal_fcolor, skin.normal_bcolor);
    return;
  }

  int src_x, src_w, dest_x, clip_x, clip_w;

  if (dx > 0)
  {
    src_x = X;
    dest_x = X + dx;
    src_w = W - dx;
    clip_x = X;
    clip_w = dx;
  }

  else
  {
    src_x = X - dx;
    dest_x = X;
    src_w = W + dx;
    clip_x = X + src_w;
    clip_w = W - src_w;
  }

  int src_y, src_h, dest_y, clip_y, clip_h;

  if (dy > 0)
  {
    src_y = Y;
    dest_y = Y + dy;
    src_h = H - dy;
    clip_y = Y;
    clip_h = dy;
  }

  else
  {
    src_y = Y - dy;
    dest_y = Y;
    src_h = H + dy;
    clip_y = Y + src_h;
    clip_h = H - src_h;
  }

  int retval = Fl_Window_Driver::driver(Fl_Window::current())->scrollto(
                 src_x,
                 src_y,
                 src_w,
                 src_h,
                 dest_x,
                 dest_y,
                 draw_area,
                 data,
                 skin);

  if (retval)
  {
    draw_area(data, X, Y, W, H, skin.normal_fcolor, skin.normal_bcolor);
    return;
  }

  if (dx) draw_area(
      data,
      clip_x,
      dest_y,
      clip_w,
      src_h,
      skin.normal_fcolor,
      skin.normal_bcolor);

  if (dy) draw_area(
      data,
      X,
      clip_y,
      W,
      clip_h,
      skin.normal_fcolor,
      skin.normal_bcolor);

  return;
}
