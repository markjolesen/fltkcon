// scroll.h
//
// Scroll header file for the Fast Light Tool Kit (FLTK).
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
//
#ifndef Fl_Scroll_H
#define Fl_Scroll_H

#include "group.h"
#include "scrlbar.h"

class Fl_Scroll : public Fl_Group
{

    int xposition_, yposition_;
    int oldx, oldy;
    static void hscrollbar_cb(Fl_Widget*, void*);
    static void scrollbar_cb(Fl_Widget*, void*);
    void fix_scrollbar_order();
    static void draw_clip(
      void* v,
      int const X,
      int const Y,
      unsigned int const W,
      unsigned int const H,
      enum Fl::foreground const fcolor,
      enum Fl::background const bcolor);

  protected:

    typedef struct
    {
      typedef struct
      {
        int x, y, w, h;
      } Fl_Region_XYWH;
      typedef struct
      {
        int l;
        int r;
        int t;
        int b;
      } Fl_Region_LRTB;
      typedef struct
      {
        int x, y, w, h;
        int pos;
        int size;
        int first;
        int total;
      } Fl_Scrollbar_Data;
      int scrollsize;
      Fl_Region_XYWH innerbox;
      Fl_Region_XYWH innerchild;
      Fl_Region_LRTB child;
      int hneeded;
      int vneeded;
      Fl_Scrollbar_Data hscroll;
      Fl_Scrollbar_Data vscroll;
    } ScrollInfo;
    void recalc_scrollbars(ScrollInfo& si);

  protected:

    void bbox(int&, int&, int&, int&);
    void draw();

  public:

    Fl_Scrollbar scrollbar;
    Fl_Scrollbar hscrollbar;

    void
    resize(
      int const x,
      int const y,
      unsigned int const w,
      unsigned int const h);

    bool
    handle(
      Fl_Event const);

    Fl_Scroll(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      unsigned char const* i_label = 0L);

    enum
    {
      HORIZONTAL = 1,
      VERTICAL = 2,
      BOTH = 3,
      ALWAYS_ON = 4,
      HORIZONTAL_ALWAYS = 5,
      VERTICAL_ALWAYS = 6,
      BOTH_ALWAYS = 7
    };

    int
    xposition() const
    {
      return xposition_;
    }
    int
    yposition() const
    {
      return yposition_;
    }
    void scroll_to(int, int);
    void clear();
};

#endif
