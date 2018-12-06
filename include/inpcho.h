// inpcho.h
//
// An input/chooser widget.
//            ______________  ____
//           |              || __ |
//           | input area   || \/ |
//           |______________||____|
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2017 by Bill Spitzak and others.
// Copyright 2004 by Greg Ercolano.
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
#ifndef Fl_Input_Choice_H
#define Fl_Input_Choice_H

#include "fl.h"
#include "group.h"
#include "input.h"
#include "btnmenu.h"

class Fl_Input_Choice : public Fl_Group
{

    class InputMenuButton : public Fl_Menu_Button
    {
        void draw();
      public:
        InputMenuButton(int X, int Y, int W, int H, const unsigned char* L = 0);
    };

    Fl_Input* inp_;
    InputMenuButton* menu_;

    static void menu_cb(Fl_Widget*, void* data);

    static void inp_cb(Fl_Widget*, void* data);

    inline int
    inp_x()
    {
      return x();
    }
    inline int
    inp_y()
    {
      return y();
    }
    inline int
    inp_w()
    {
      return w();
    }
    inline int
    inp_h()
    {
      return h();
    }

    inline int
    menu_x()
    {
      return (x() + w());
    }
    inline int
    menu_y()
    {
      return y();
    }
    inline int
    menu_w()
    {
      return (20);
    }
    inline int
    menu_h()
    {
      return h();
    }

  public:

    Fl_Input_Choice(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      unsigned char const* i_label = 0L);

    virtual
    void resize(
      int const x,
      int const y,
      unsigned int const w,
      unsigned int const h);

    void
    add(const unsigned char* s)
    {
      menu_->add(s);
    }

    int
    changed() const
    {
      return inp_->changed() | Fl_Widget::changed();
    }

    void clear_changed();

    void set_changed();

    void
    clear()
    {
      menu_->clear();
    }

    const Fl_Menu_Item*
    menu()
    {
      return (menu_->menu());
    }

    void
    menu(const Fl_Menu_Item* m)
    {
      menu_->menu(m);
    }

    const unsigned char*
    value() const
    {
      return (inp_->value());
    }

    void
    value(const unsigned char* val)
    {
      inp_->value(val);
    }

    void value(int val);

    int update_menubutton();

    Fl_Menu_Button*
    menubutton()
    {
      return menu_;
    }

    Fl_Input*
    input()
    {
      return inp_;
    }
};

#endif // !Fl_Input_Choice_H
