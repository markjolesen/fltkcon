// menu_.h
//
// Menu base class header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2018 by Bill Spitzak and others.
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
#ifndef Fl_Menu__H
#define Fl_Menu__H

#include "widget.h"
#include "menuitem.h"

class Fl_Menu_ : public Fl_Widget
{

    Fl_Menu_Item* menu_;
    const Fl_Menu_Item* value_;

  protected:

    unsigned char alloc;

    int item_pathname_(char* name, int namelen, const Fl_Menu_Item* finditem,
                       const Fl_Menu_Item* menu = 0) const;
  public:
    Fl_Menu_(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      unsigned char const* i_label = 0L);

    ~Fl_Menu_();

    int item_pathname(char* name, int namelen,
                      const Fl_Menu_Item* finditem = 0) const;
    const Fl_Menu_Item* picked(const Fl_Menu_Item*);
    const Fl_Menu_Item* find_item(const unsigned char* name);
    const Fl_Menu_Item* find_item(Fl_Callback*);
    int find_index(const unsigned char* name) const;
    int find_index(const Fl_Menu_Item* item) const;
    int find_index(Fl_Callback* cb) const;

    const Fl_Menu_Item*
    test_shortcut()
    {
      return picked(menu()->test_shortcut());
    }
    void global();

    const Fl_Menu_Item*
    menu() const
    {
      return menu_;
    }
    void menu(const Fl_Menu_Item* m);
    void copy(const Fl_Menu_Item* m, void* user_data = 0);
    int insert(int index, const unsigned char*, int shortcut, Fl_Callback*,
               void* = 0,
               int = 0);
    int  add(const unsigned char*, int shortcut, Fl_Callback*, void* = 0,
             int = 0);
    int
    add(const unsigned char* a, const unsigned char* b, Fl_Callback* c, void* d = 0,
        int e = 0)
    {
      return add(a, fl_old_shortcut(b), c, d, e);
    }
    int
    insert(int index, const unsigned char* a, const unsigned char* b,
           Fl_Callback* c, void* d = 0,
           int e = 0)
    {
      return insert(index, a, fl_old_shortcut(b), c, d, e);
    }
    int  add(const unsigned char*);
    int  size() const ;
    void
    size(int W, int H)
    {
      Fl_Widget::size(W, H);
    }
    void clear();
    int clear_submenu(int index);
    void replace(int, const unsigned char*);
    void remove(int);
    void
    shortcut(int i, int s)
    {
      menu_[i].shortcut(s);
    }
    void
    mode(int i, int fl)
    {
      menu_[i].flags = fl;
    }
    int
    mode(int i) const
    {
      return menu_[i].flags;
    }

    const Fl_Menu_Item*
    mvalue() const
    {
      return value_;
    }
    int
    value() const
    {
      return value_ ? (int)(value_ - menu_) : -1;
    }
    int value(const Fl_Menu_Item*);
    int
    value(int i)
    {
      return value(menu_ + i);
    }
    const unsigned char*
    text() const
    {
      return value_ ? value_->text : 0;
    }
    const unsigned char*
    text(int i) const
    {
      return menu_[i].text;
    }

    void setonly(Fl_Menu_Item* item);
};

#endif
