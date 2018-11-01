// group.h
//
// Group header file for the Fast Light Tool Kit (FLTK).
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
#ifndef Fl_Group_H
#define Fl_Group_H

#include "widget.h"

class Fl_Rect;

class Fl_Group : public Fl_Widget
{

    Fl_Widget** array_;
    Fl_Widget* savedfocus_;
    Fl_Widget* resizable_;
    int children_;
    Fl_Rect* bounds_;
    int* sizes_;

    int navigation(int const);
    static Fl_Group* current_;

    Fl_Group(const Fl_Group&);
    Fl_Group& operator=(const Fl_Group&);

  protected:
    void draw();
    void draw_child(Fl_Widget& widget) const;
    void draw_children();
    void draw_outside_label(const Fl_Widget& widget) const ;
    void update_child(Fl_Widget& widget) const;
    Fl_Rect* bounds();

  public:

    bool handle(Fl_Event const);
    void begin();
    void end();
    static Fl_Group* current();
    static void current(Fl_Group* g);

    int
    children() const
    {
      return children_;
    }

    Fl_Widget*
    child(unsigned int const n) const
    {
      return array()[n];
    }

    int find(const Fl_Widget*) const;

    int
    find(const Fl_Widget& o) const
    {
      return find(&o);
    }

    Fl_Widget* const* array() const;

    void
    resize(
      int const x,
      int const y,
      unsigned int const w,
      unsigned int const h);

    void
    resizable(Fl_Widget& o)
    {
      resizable_ = &o;
    }
    void
    resizable(Fl_Widget* o)
    {
      resizable_ = o;
    }
    Fl_Widget*
    resizable() const
    {
      return resizable_;
    }
    void
    add_resizable(Fl_Widget& o)
    {
      resizable_ = &o;
      add(o);
    }

    Fl_Group(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      unsigned char const* i_label = 0L);

    virtual ~Fl_Group();

    void add(Fl_Widget&);

    void
    add(Fl_Widget* o)
    {
      add(*o);
    }

    void insert(Fl_Widget&, int i);

    void
    insert(Fl_Widget& o, Fl_Widget* before)
    {
      insert(o, find(before));
    }

    void remove(unsigned int const index);

    void remove(Fl_Widget&);

    void
    remove(Fl_Widget* o)
    {
      remove(*o);
    }

    void clear();

    void init_sizes();

    void
    clip_children(bool const c)
    {
      if (c) set_flag(CLIP_CHILDREN);

      else clear_flag(CLIP_CHILDREN);
    }

    unsigned int
    clip_children()
    {
      return (flags() & CLIP_CHILDREN) != 0;
    }

    virtual Fl_Group*
    as_group()
    {
      return this;
    }
};

class Fl_End
{
  public:
    Fl_End()
    {
      Fl_Group::current()->end();
    }
};

#endif
