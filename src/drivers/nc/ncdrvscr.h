// ncdrvscr.h
//
// Curses Screen Driver for the Fast Light Tool Kit (FLTK)
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
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
#if !defined(FL_NC_SCREEN_DRIVER_H)

#include <time.h>
#include "drvscr.h"
#include "fl_enums.h"
#include "fl_timer.h"
#include "ncwm.h"

class Fl_NC_Screen_Driver : public Fl_Screen_Driver
{

  protected:

    enum status
    {
      STATUS_CLEAR = 0,
      STATUS_PUSHED,
      STATUS_STICKY
    };

    struct
    {
      enum status status;
      int x;
      int y;
    } state_;

    ncwm wm_;

    void
    event_key(
      Fl_Window& window,
      int const ascii,
      int const scan) const;

    void
    event_key(
      Fl_Window& window,
      int const key) const;

    void
    event_mouse(
      Fl_Window& window);

    int
    poll(Fl_Window& window);

  public:

    Fl_NC_Screen_Driver();

    virtual ~Fl_NC_Screen_Driver();

    virtual void init();

    virtual int x();

    virtual int y();

    virtual int w();

    virtual int h();

    virtual void screen_xywh(int& X, int& Y, int& W, int& H, int n);

    virtual void screen_work_area(int& X, int& Y, int& W, int& H, int n);

    virtual void beep(int type);

    virtual void flush();

    virtual double wait(double time_to_wait);

    virtual int ready();

    virtual void grab(Fl_Window* win);

    virtual void add_timeout(double time, Fl_Timeout_Handler cb, void* argp);

    virtual void repeat_timeout(double time, Fl_Timeout_Handler cb, void* argp);

    virtual int has_timeout(Fl_Timeout_Handler cb, void* argp);

    virtual void remove_timeout(Fl_Timeout_Handler cb, void* argp);

    virtual int compose(int& del);

};

#define FL_NC_SCREEN_DRIVER_H
#endif
