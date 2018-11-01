// menuitem.h
//
// Menu item header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Menu_Item_H
#define Fl_Menu_Item_H

#include "widget.h"

enum
{
  FL_MENU_INACTIVE = 1,
  FL_MENU_TOGGLE = 2,
  FL_MENU_VALUE = 4,
  FL_MENU_RADIO = 8,
  FL_MENU_INVISIBLE = 0x10,
  FL_SUBMENU_POINTER = 0x20,
  FL_SUBMENU = 0x40,
  FL_MENU_DIVIDER = 0x80,
  FL_MENU_HORIZONTAL = 0x100
};

extern Fl_Shortcut fl_old_shortcut(const unsigned char*);

class Fl_Menu_;

struct Fl_Menu_Item
{
  const unsigned char* text;
  int shortcut_;
  Fl_Callback* callback_;
  void* user_data_;
  int flags;
  unsigned char labeltype_;

  const Fl_Menu_Item* next(int = 1) const;

  Fl_Menu_Item*
  next(int i = 1)
  {
    return (Fl_Menu_Item*)(((const Fl_Menu_Item*)this)->next(i));
  }

  const Fl_Menu_Item*
  first() const
  {
    return next(0);
  }

  Fl_Menu_Item*
  first()
  {
    return next(0);
  }

  const unsigned char*
  label() const
  {
    return text;
  }

  void
  label(const unsigned char* a)
  {
    text = a;
  }

  void
  label(Fl_Label::Fl_LabelType const a, const unsigned char* b)
  {
    labeltype_ = a;
    text = b;
  }

  Fl_Label::Fl_LabelType
  labeltype() const
  {
    return (Fl_Label::Fl_LabelType)labeltype_;
  }

  void
  labeltype(Fl_Label::Fl_LabelType const a)
  {
    labeltype_ = a;
  }
  Fl_Callback_p
  callback() const
  {
    return callback_;
  }

  void
  callback(Fl_Callback* c, void* p)
  {
    callback_ = c;
    user_data_ = p;
  }

  void
  callback(Fl_Callback* c)
  {
    callback_ = c;
  }

  void
  callback(Fl_Callback0* c)
  {
    callback_ = (Fl_Callback*)c;
  }

  void
  callback(Fl_Callback1* c, long p = 0)
  {
    callback_ = (Fl_Callback*)c;
    user_data_ = (void*)(ptrdiff_t)p;
  }

  void*
  user_data() const
  {
    return user_data_;
  }
  void
  user_data(void* v)
  {
    user_data_ = v;
  }
  long
  argument() const
  {
    return (long)(ptrdiff_t)user_data_;
  }
  void
  argument(long v)
  {
    user_data_ = (void*)(ptrdiff_t)v;
  }

  int
  shortcut() const
  {
    return shortcut_;
  }

  void
  shortcut(int s)
  {
    shortcut_ = s;
  }
  int
  submenu() const
  {
    return flags & (FL_SUBMENU | FL_SUBMENU_POINTER);
  }
  int
  checkbox() const
  {
    return flags & FL_MENU_TOGGLE;
  }
  int
  radio() const
  {
    return flags & FL_MENU_RADIO;
  }
  int
  value() const
  {
    return flags & FL_MENU_VALUE;
  }
  void
  set()
  {
    flags |= FL_MENU_VALUE;
  }

  void
  clear()
  {
    flags &= ~FL_MENU_VALUE;
  }

  void setonly();

  int
  visible() const
  {
    return !(flags & FL_MENU_INVISIBLE);
  }

  void
  show()
  {
    flags &= ~FL_MENU_INVISIBLE;
  }

  void
  hide()
  {
    flags |= FL_MENU_INVISIBLE;
  }

  int
  active() const
  {
    return !(flags & FL_MENU_INACTIVE);
  }

  void
  activate()
  {
    flags &= ~FL_MENU_INACTIVE;
  }
  void
  deactivate()
  {
    flags |= FL_MENU_INACTIVE;
  }
  int
  activevisible() const
  {
    return !(flags & (FL_MENU_INACTIVE | FL_MENU_INVISIBLE));
  }

  int measure(int* h, const Fl_Menu_*) const;
  void draw(int x, int y, int w, int h, const Fl_Menu_*, int t = 0) const;

  const Fl_Menu_Item* popup(
    int X, int Y,
    const unsigned char* title = 0,
    const Fl_Menu_Item* picked = 0,
    const Fl_Menu_* = 0) const;
  const Fl_Menu_Item* pulldown(
    int X, int Y, int W, int H,
    const Fl_Menu_Item* picked = 0,
    const Fl_Menu_* = 0,
    const Fl_Menu_Item* title = 0,
    int menubar = 0) const;
  const Fl_Menu_Item* test_shortcut() const;
  const Fl_Menu_Item* find_shortcut(int* ip = 0,
                                    const bool require_alt = false) const;

  void
  do_callback(Fl_Widget* o) const
  {
    callback_(o, user_data_);
  }

  void
  do_callback(Fl_Widget* o, void* arg) const
  {
    callback_(o, arg);
  }

  void
  do_callback(Fl_Widget* o, long arg) const
  {
    callback_(o, (void*)(ptrdiff_t)arg);
  }

  int
  checked() const
  {
    return flags & FL_MENU_VALUE;
  }

  void
  check()
  {
    flags |= FL_MENU_VALUE;
  }

  void
  uncheck()
  {
    flags &= ~FL_MENU_VALUE;
  }

  int insert(int, const unsigned char*, int, Fl_Callback*, void* = 0, int = 0);
  int add(const unsigned char*, int shortcut, Fl_Callback*, void* = 0, int = 0);

  int
  add(const unsigned char* a, const unsigned char* b, Fl_Callback* c,
      void* d = 0, int e = 0)
  {
    return add(a, fl_old_shortcut(b), c, d, e);
  }

  int size() const ;
};

typedef Fl_Menu_Item Fl_Menu;

enum
{
  FL_PUP_NONE = 0,
  FL_PUP_GREY = FL_MENU_INACTIVE,
  FL_PUP_GRAY = FL_MENU_INACTIVE,
  FL_MENU_BOX = FL_MENU_TOGGLE,
  FL_PUP_BOX  = FL_MENU_TOGGLE,
  FL_MENU_CHECK = FL_MENU_VALUE,
  FL_PUP_CHECK  = FL_MENU_VALUE,
  FL_PUP_RADIO  = FL_MENU_RADIO,
  FL_PUP_INVISIBLE = FL_MENU_INVISIBLE,
  FL_PUP_SUBMENU = FL_SUBMENU_POINTER
};

#endif
