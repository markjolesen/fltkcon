// drvgr.h
//
// Graphics Driver for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
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
//
#ifndef FL_GRAPHICS_DRIVER_H
#define FL_GRAPHICS_DRIVER_H

#include "fl_color.h"

class Fl_Graphics_Driver
{

  private:

    struct region
    {
      bool                                  m_is_empty;
      int                                   m_pos_x;
      int                                   m_pos_y;
      unsigned int                          m_len_x;
      unsigned int                          m_len_y;
    };

    enum
    {
      stack_max = 10
    };

    unsigned int                     stack_top_;
    struct region                    stack_[stack_max];

    Fl_Graphics_Driver();

    Fl_Graphics_Driver(Fl_Graphics_Driver const&);

    Fl_Graphics_Driver& operator=(Fl_Graphics_Driver const&);

  public:

    static int draw_offset_x;
    static int draw_offset_y;

    static Fl_Graphics_Driver&
    instance();

    bool
    clip_intersects(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y) const;

    bool
    clip_box(
      int& o_pos_x,
      int& o_pos_y,
      unsigned int& o_len_x,
      unsigned int& o_len_y,
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y) const;

    void
    clip_pop();

    void
    clip_push(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y);

    void
    clip_push_none();

    void
    clip_replace(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y);

    void
    draw_fill(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      unsigned int const i_char,
      enum Fl::foreground const i_fcolor,
      enum Fl::background const i_bcolor) const;

    void
    draw_putc(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_char,
      unsigned int const i_repeat_count,
      enum Fl::foreground const i_fcolor,
      enum Fl::background const i_bcolor) const;

    void
    draw_putvc(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_char,
      unsigned int const i_repeat_count,
      enum Fl::foreground const i_fcolor,
      enum Fl::background const i_bcolor) const;

    void
    draw_puts(
      int const i_pos_x,
      int const i_pos_y,
      unsigned char const* i_string,
      unsigned int const i_length,
      enum Fl::foreground const i_fcolor,
      enum Fl::background const i_bcolor) const;

    void
    draw_frame(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      enum Fl::foreground const i_fcolor,
      enum Fl::background const i_bcolor) const;

    void
    flip_to_offscreen(bool i_copy) const;

    void
    flip_to_onscreen() const;

    void
    mouse_hide();

    void
    mouse_show();

};

namespace Fl
{

  inline Fl_Graphics_Driver&
  gr()
  {
    return Fl_Graphics_Driver::instance();
  }


}

#endif // FL_GRAPHICS_DRIVER_H
