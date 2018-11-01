// input.h
//
// Input header file for the Fast Light Tool Kit (FLTK).
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
#ifndef Fl_Input_H
#define Fl_Input_H

#include "input_.h"

class Fl_Input : public Fl_Input_
{

    friend class Fl_Screen_Driver;

    void
    handle_mouse(
      bool keepmark = false);

    void
    handle_keyboard_compose(
      int const i_del);

    bool
    handle_keyboard();

    int
    handle_key();

  protected:

    enum Fl_Widget::Fl_Damage
    home();

    enum Fl_Widget::Fl_Damage
    end();

    enum Fl_Widget::Fl_Damage
    left();

    enum Fl_Widget::Fl_Damage
    right();

    enum Fl_Widget::Fl_Damage
    up();

    enum Fl_Widget::Fl_Damage
    down();

    enum Fl_Widget::Fl_Damage
    ins(
      unsigned char const* i_chr,
      size_t const i_size);

    enum Fl_Widget::Fl_Damage
    ovw(
      unsigned char const* i_chr,
      size_t const i_size);

    enum Fl_Widget::Fl_Damage
    del();

    enum Fl_Widget::Fl_Damage
    bs();

    enum Fl_Widget::Fl_Damage
    nl();

    virtual void
    draw();

  public:

    virtual bool
    handle(
      enum Fl_Event const event);

    Fl_Input(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      const unsigned char* i_label = 0L);

};

#endif
