// fl_draw.h
//
// Portable drawing function header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
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
#if !defined(__fl_draw_h__)

#include "drvgr.h"

namespace Fl
{

  inline void
  draw_fill(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y,
    unsigned int const                  i_char,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
  {

    gr().draw_fill(
      i_pos_x,
      i_pos_y,
      i_len_x,
      i_len_y,
      i_char,
      i_fcolor,
      i_bcolor);

    return;
  }

  inline void
  draw_putc(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_char,
    unsigned int const                  i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
  {

    gr().draw_putc(
      i_pos_x,
      i_pos_y,
      i_char,
      i_repeat_count,
      i_fcolor,
      i_bcolor);

    return;
  }

  inline void
  draw_putvc(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_char,
    unsigned int const                  i_repeat_count,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
  {

    gr().draw_putvc(
      i_pos_x,
      i_pos_y,
      i_char,
      i_repeat_count,
      i_fcolor,
      i_bcolor);

    return;
  }

  inline void
  draw_puts(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned char const*                i_string,
    unsigned int const                  i_length,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
  {

    gr().draw_puts(
      i_pos_x,
      i_pos_y,
      i_string,
      i_length,
      i_fcolor,
      i_bcolor);

    return;
  }

  inline void
  draw_frame(
    int const                           i_pos_x,
    int const                           i_pos_y,
    unsigned int const                  i_len_x,
    unsigned int const                  i_len_y,
    enum foreground const               i_fcolor,
    enum background const               i_bcolor)
  {

    gr().draw_frame(
      i_pos_x,
      i_pos_y,
      i_len_x,
      i_len_y,
      i_fcolor,
      i_bcolor);

    return;
  }

};

#define __fl_draw_h__
#endif