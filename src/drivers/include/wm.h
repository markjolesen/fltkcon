// wm.h
//
// Window manager for the Fast Light Tool Kit (FLTK)
//
// Copyright 2018 The fltkcon authors
// Copyright 2018 The fltkal authors
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
#if !defined(__WM_H__)

#include "drvwin.h"
#include "win.h"

class wm
{

  public:

    wm();

    virtual ~wm();

    enum hit_type
    {
      HIT_NONE = 0,
      HIT_WINDOW,
      HIT_MOVE,
      HIT_EAST,
      HIT_WEST,
      HIT_NORTH_EAST,
      HIT_NORTH_WEST,
      HIT_SOUTH,
      HIT_SOUTH_EAST,
      HIT_SOUTH_WEST
    };

    hit_type
    hit(Fl_Window& window, int const x, int const y) const;

    bool
    handle_mouse(Fl_Window& window, hit_type const what, int const x,
                int const y) const;

    static void
    resize_east(int& left, int& top, unsigned int& width, unsigned int& height,
                int const delta_x, int const delta_y);

    static void
    resize_west(int& left, int& top, unsigned int& width, unsigned int& height,
                int const delta_x, int const delta_y);

    static void
    resize_north(int& left, int& top, unsigned int& width, unsigned int& height,
                 int const delta_x, int const delta_y);

    static void
    resize_south(int& left, int& top, unsigned int& width, unsigned int& height,
                 int const delta_x, int const delta_y);

    static void
    resize_north_east(int& left, int& top, unsigned int& width,
                      unsigned int& height, int const delta_x, int const delta_y);

    static void
    resize_north_west(int& left, int& top, unsigned int& width,
                      unsigned int& height, int const delta_x, int const delta_y);
    static void
    resize_south_east(int& left, int& top, unsigned int& width,
                      unsigned int& height, int const delta_x, int const delta_y);

    static void
    resize_south_west(int& left, int& top, unsigned int& width,
                      unsigned int& height, int const delta_x, int const delta_y);

  protected:

    virtual void
    handle_push(Fl_Window& window, hit_type const what) const = 0;

  private:

    wm(wm const&);

    wm& operator=(wm const&);

};

inline void
wm::resize_east(int& left, int& top, unsigned int& width, unsigned int& height,
                int const delta_x, int const delta_y)
{
  width += delta_x;

  if (Fl_Window_Driver::window_min_width > width)
  {
    width = Fl_Window_Driver::window_min_width;
  }
}

inline void
wm::resize_west(int& left, int& top, unsigned int& width, unsigned int& height,
                int const delta_x, int const delta_y)
{
  width -= delta_x;

  if (Fl_Window_Driver::window_min_width < width)
  {
    left += delta_x;
  }

  else
  {
    width += delta_x;
  }
}

inline void
wm::resize_north(int& left, int& top, unsigned int& width, unsigned int& height,
                 int const delta_x, int const delta_y)
{
  height -= delta_y;

  if (Fl_Window_Driver::window_min_height < height)
  {
    top += delta_y;
  }

  else
  {
    height += delta_y;
  }
}

inline void
wm::resize_south(int& left, int& top, unsigned int& width, unsigned int& height,
                 int const delta_x, int const delta_y)
{
  height += delta_y;

  if (Fl_Window_Driver::window_min_height > height)
  {
    height = Fl_Window_Driver::window_min_height;
  }
}

inline void
wm::resize_north_east(int& left, int& top, unsigned int& width,
                      unsigned int& height, int const delta_x, int const delta_y)
{
  resize_north(left, top, width, height, delta_x, delta_y);
  resize_east(left, top, width, height, delta_x, delta_y);
}

inline void
wm::resize_north_west(int& left, int& top, unsigned int& width,
                      unsigned int& height, int const delta_x, int const delta_y)
{
  resize_north(left, top, width, height, delta_x, delta_y);
  resize_west(left, top, width, height, delta_x, delta_y);
}

inline void
wm::resize_south_east(int& left, int& top, unsigned int& width,
                      unsigned int& height, int const delta_x, int const delta_y)
{
  resize_south(left, top, width, height, delta_x, delta_y);
  resize_east(left, top, width, height, delta_x, delta_y);
}

inline void
wm::resize_south_west(int& left, int& top, unsigned int& width,
                      unsigned int& height, int const delta_x, int const delta_y)
{
  resize_south(left, top, width, height, delta_x, delta_y);
  resize_west(left, top, width, height, delta_x, delta_y);
}

#define __WM_H__
#endif
