// drvwin.h
//
// A base class for platform specific window handling code
// for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 2010-2017 by Bill Spitzak and others.
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
#ifndef FL_WINDOW_DRIVER_H
#define FL_WINDOW_DRIVER_H

#include <stdlib.h>
#include "win.h"

class Fl_X;

/**
 \brief A base class for platform specific window handling code.

 This class is only for internal use by the FLTK library.

 When porting FLTK to a new platform, many mothods in this class provide
 a minimal default implementation. Some methods must be overridden to make
 sure that the Graphics Driver will draw into the bitmap associated with
 this window.
*/
class Fl_Window_Driver
{
    friend class Fl_Window;

  protected:
    Fl_Window* pWindow;
    void flush_Fl_Window(); // accessor to protected Fl_Window::flush()
  public:

    enum
    {
      window_min_height = 5,
      window_min_width = 5
    };

    Fl_Window_Driver(Fl_Window*);
    virtual ~Fl_Window_Driver();
    static Fl_Window_Driver* newWindowDriver(Fl_Window*);
    int wait_for_expose_value;
    // Fl_Offscreen other_xid; // offscreen bitmap (overlay and double-buffered windows)

    // --- frequently used accessors to public window data
    /** returns the x coordinate of the window. */
    int
    x() const
    {
      return pWindow->x();
    }
    /** returns the y coordinate of the window. */
    int
    y() const
    {
      return pWindow->y();
    }
    /** returns the width of the window. */
    int
    w() const
    {
      return pWindow->w();
    }
    /** returns the height of the window. */
    int
    h() const
    {
      return pWindow->h();
    }
    /** returns whether the window has a border. */
    int
    get_border() const
    {
      return pWindow->get_border();
    }
    /** returns whether the window itself is visible(). */
    int
    visible() const
    {
      return pWindow->visible();
    }
    /** returns whether the window and all its parents are visible(). */
    int
    visible_r() const
    {
      return pWindow->visible_r();
    }
    /** returns whether the window is shown(). */
    int
    shown() const
    {
      return pWindow->shown();
    }
    /** returns the parent of the window. */
    Fl_Group*
    parent() const
    {
      return pWindow->parent();
    }

    // --- accessors to private window data
    int minw();
    int minh();
    int maxw();
    int maxh();
    int dw();
    int dh();
    int aspect();
    unsigned char size_range_set();
    int force_position();
    void force_position(int c);
    void x(int X);
    void y(int Y);
    void current(Fl_Window* c);
    char
    show_iconic()
    {
      return Fl_Window::show_iconic_;
    }
    void
    show_iconic(char c)
    {
      Fl_Window::show_iconic_ = c;
    }
    void
    i(Fl_X* x)
    {
      pWindow->i = x;
    }

    // --- window data
    virtual int
    decorated_w()
    {
      return w();  // default, should be overidden by driver
    }
    virtual int
    decorated_h()
    {
      return h();
    }

    // --- window management
    virtual void take_focus();
    virtual void flush(); // the default implementation may be enough
    virtual void flush_double();
///    virtual void flush_overlay();
    virtual void flush_menu();
    virtual void
    erase_menu() {}
    /** Usable for platform-specific code executed before the platform-independent part of Fl_Window::draw() */
    virtual void draw_begin();
    /** Usable for platform-specific code executed after the platform-independent part of Fl_Window::draw() */
    virtual void draw_end();
    void draw();
    virtual void make_current();
    virtual void label(const unsigned char* name, const unsigned char* mininame);

    virtual Fl_X*
    makeWindow()
    {
      return 0;
    }
    virtual void wait_for_expose();
    virtual void destroy_double_buffer();
    virtual void show();
    virtual void show_menu();
    virtual void
    resize(int X, int Y, int W, int H) {}
    virtual void
    hide() {}
    int hide_common();
    virtual void
    map() {}
    virtual void
    unmap() {}
    virtual void
    fullscreen_on() {}
    virtual void
    fullscreen_off(int X, int Y, int W, int H) {}
    virtual void use_border();
    virtual void size_range();
    virtual void
    iconize() {}
    virtual void
    decoration_sizes(int* top, int* left,  int* right, int* bottom)
    {
      *top = *left = *right = *bottom = 0;
    }
    virtual void
    show_with_args_begin() {}
    virtual void
    show_with_args_end(int argc, char** argv) {}
//    virtual int can_do_overlay();
//    virtual void redraw_overlay();

    // --- window printing/drawing helper
    virtual int
    scrollto(
      int const X,
      int const Y,
      unsigned int const W,
      unsigned int const H,
      int const dx,
      int const dy,
      void (*draw_area)(
        void*,
        int const,
        int const,
        unsigned int const,
        unsigned int const,
        enum Fl::foreground const fcolor,
        enum Fl::background const bcolor),
      void* data,
      struct Fl::skin_widget const& skin)
    {
      return 0;
    }

    static inline Fl_Window_Driver*
    driver(const Fl_Window* win)
    {
      return win->pWindowDriver;
    }
};

#endif // FL_WINDOW_DRIVER_H
