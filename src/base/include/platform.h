// platform.h
//
// Platform header file for the Fast Light Tool Kit (FLTK).
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
#if !defined(FL_PLATFORM_H) && !defined(FL_DOXYGEN)
#  define FL_PLATFORM_H

#if defined(FL_LIBRARY) || defined(FL_INTERNALS)

#include "win.h"
#include "fl_rect.h"

typedef int Window;
typedef Fl_Rect Fl_Region;

class Fl_X
{
  public:
    Window xid;
    Fl_Window* w;
    Fl_Region region;
    Fl_X* next;
    static Fl_X* first;
    static Fl_X*
    i(const Fl_Window* w)
    {
      return (Fl_X*)w->i;
    }
};

inline Window
fl_xid(const Fl_Window* w)
{
  Fl_X* xTemp = Fl_X::i(w);
  return xTemp ? xTemp->xid : 0;
}
#else
extern Window fl_xid_(const Fl_Window* w);
#  define fl_xid(w) fl_xid_(w)
#endif // FL_LIBRARY || FL_INTERNALS

extern Fl_Window* fl_find(Window xid);
extern void fl_open_display();
extern void fl_close_display();
extern Window fl_window;
extern int fl_parse_color(const char* p, unsigned char& r, unsigned char& g,
                          unsigned char& b);
extern void fl_open_callback(void (*)(const char*));

#endif // !FL_PLATFORM_H
