// input_.h
//
// Input base class header file for the Fast Light Tool Kit (FLTK).
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
#ifndef Fl_Input__H
#define Fl_Input__H

#include "widget.h"
#include "inpcur.h"

class Fl_Input_ : public Fl_Widget
{

  public:

    enum Fl_InputType
    {
      FL_NORMAL_INPUT = 0,
      FL_FLOAT_INPUT = 1,
      FL_INT_INPUT = 2,
      FL_HIDDEN_INPUT = 3,
      FL_MULTILINE_INPUT = 4,
      FL_SECRET_INPUT = 5,
      FL_INPUT_TYPE = 7,
      FL_INPUT_READONLY = 8,
      FL_NORMAL_OUTPUT =  (FL_NORMAL_INPUT | FL_INPUT_READONLY),
      FL_MULTILINE_OUTPUT = (FL_MULTILINE_INPUT | FL_INPUT_READONLY)
                            /*
                                    FL_INPUT_WRAP= 16,
                                    FL_MULTILINE_INPUT_WRAP= (FL_MULTILINE_INPUT | FL_INPUT_WRAP),
                                    FL_MULTILINE_OUTPUT_WRAP= (FL_MULTILINE_INPUT | FL_INPUT_READONLY | FL_INPUT_WRAP)
                            */
    };

    Fl_Input_(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      unsigned char const* i_label = 0L);

    virtual ~Fl_Input_();

    virtual
    void resize(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y);

    void
    value(
      unsigned char const* i_buf,
      size_t const i_size = static_cast<size_t>(-1));

    int
    shortcut() const
    {
      return shortcut_;
    }

    void
    shortcut(int const s)
    {
      shortcut_ = s;
    }

    unsigned char*
    value() const
    {
      return inplist_as_string((*cur_).m_list);
    }

    int
    input_type() const
    {
      return type() & FL_INPUT_TYPE;
    }

    void
    input_type(int const t)
    {
      type((unsigned char)(t | readonly()));
    }

    bool
    readonly() const
    {
      return type() & FL_INPUT_READONLY;
    }

    void
    readonly(bool const b)
    {
      if (b) type((unsigned char)(type() | FL_INPUT_READONLY));

      else type((unsigned char)(type() & ~FL_INPUT_READONLY));
    }

    void
    tab_nav(bool const val)
    {
      tab_nav_ = val;
    }

    bool
    tab_nav() const
    {
      return tab_nav_;
    }

  protected:

    int shortcut_;
    bool tab_nav_;
    enum Fl_InputType type_;
    struct inpcur* cur_;

    void
    drawtext(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int i_len_x,
      unsigned int i_len_y) const;

    bool
    handletext(
      enum Fl_Event const event,
      int const i_pos_x,
      int const i_pos_y,
      unsigned int i_len_x,
      unsigned int i_len_y);

};

#endif
