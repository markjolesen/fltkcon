// rect.h
//
// Fl_Rect header file for the Fast Light Tool Kit (FLTK).
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
#ifndef Fl_Rect_H
#define Fl_Rect_H

#include "widget.h"

class Fl_Rect
{

    int x_;
    int y_;
    int w_;
    int h_;

  public:

    Fl_Rect()
      : x_(0), y_(0), w_(0), h_(0) {}

    Fl_Rect(int W, int H)
      : x_(0), y_(0), w_(W), h_(H) {}

    Fl_Rect(int X, int Y, int W, int H)
      : x_(X), y_(Y), w_(W), h_(H) {}

    Fl_Rect (const Fl_Widget& widget)
      : x_(widget.x()), y_(widget.y()), w_(widget.w()), h_(widget.h()) {}

    Fl_Rect (const Fl_Widget* const widget)
      : x_(widget->x()), y_(widget->y()), w_(widget->w()), h_(widget->h()) {}

    int
    x() const
    {
      return x_;
    }
    int
    y() const
    {
      return y_;
    }
    int
    w() const
    {
      return w_;
    }
    int
    h() const
    {
      return h_;
    }

    int
    r() const
    {
      return x_ + w_;
    }
    int
    b() const
    {
      return y_ + h_;
    }

    void
    x(int X)
    {
      x_ = X;
    }
    void
    y(int Y)
    {
      y_ = Y;
    }
    void
    w(int W)
    {
      w_ = W;
    }
    void
    h(int H)
    {
      h_ = H;
    }

};

#endif // Fl_Rect_H
