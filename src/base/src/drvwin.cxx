// drvwin.cxx
//
// A base class for platform specific window handling code
// for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The FDOSTUI authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2017 by Bill Spitzak and others.
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
#include "drvwin.h"
#include "fl_draw.h"
#include "fl.h"
#include "platform.h"

extern void fl_throw_focus(Fl_Widget* o);


/**
 Create a new Window Driver.

 This calls should be derived into a new class that manages desktop windows
 on the target platform.
 */
Fl_Window_Driver::Fl_Window_Driver(Fl_Window* win) :
  pWindow(win)
{
  wait_for_expose_value = 0;
}


Fl_Window_Driver::~Fl_Window_Driver()
{
}

int
Fl_Window_Driver::minw()
{
  return pWindow->minw;
}
int
Fl_Window_Driver::minh()
{
  return pWindow->minh;
}
int
Fl_Window_Driver::maxw()
{
  return pWindow->maxw;
}
int
Fl_Window_Driver::maxh()
{
  return pWindow->maxh;
}
int
Fl_Window_Driver::dw()
{
  return pWindow->dw;
}
int
Fl_Window_Driver::dh()
{
  return pWindow->dh;
}
int
Fl_Window_Driver::aspect()
{
  return pWindow->aspect;
}
int
Fl_Window_Driver::force_position()
{
  return pWindow->force_position();
}
void
Fl_Window_Driver::force_position(int c)
{
  pWindow->force_position(c);
}
void
Fl_Window_Driver::x(int X)
{
  pWindow->x(X);
}
void
Fl_Window_Driver::y(int Y)
{
  pWindow->y(Y);
}
void
Fl_Window_Driver::current(Fl_Window* c)
{
  pWindow->current_ = c;
}



unsigned char
Fl_Window_Driver::size_range_set()
{
  return pWindow->size_range_set;
}

void
Fl_Window_Driver::flush_Fl_Window()
{
  pWindow->Fl_Window::flush();
}

void
Fl_Window_Driver::flush_menu()
{
  pWindow->Fl_Window::flush();
}

/**
 Draw the window content.
 A new driver can add code before or after drawing an individua window.
 */
void
Fl_Window_Driver::draw()
{
  pWindow->draw();
}

/**
 Prepare this window for rendering.
 A new driver may prepare bitmaps and clipping areas for calls to the
 Graphics driver.
 */
void
Fl_Window_Driver::make_current() { }

/**
 Make the window visble and raise it to the top.
 */
void
Fl_Window_Driver::show() { }

void
Fl_Window_Driver::show_menu()
{
  pWindow->Fl_Window::show();
}

/**
 Change the window title.
 A new drive should provide an interface to change the title of the window
 in the title bar.
 */
void
Fl_Window_Driver::label(const unsigned char* name,
                        const unsigned char* mininame) {}

void
Fl_Window_Driver::take_focus()
{
  // nothing to do
}


void
Fl_Window_Driver::flush_double()
{
  flush_Fl_Window();
}

void
Fl_Window_Driver::draw_begin()
{
  // nothing to do
}


void
Fl_Window_Driver::draw_end()
{
  // nothing to do
}

void
Fl_Window_Driver::destroy_double_buffer()
{
  // fl_delete_offscreen(other_xid);
  // other_xid = 0;
}

// This function is available for use by platform-specific, Fl_Window_Driver-derived classes
int
Fl_Window_Driver::hide_common()
{
  pWindow->clear_visible();

  if (!shown()) return 1;

  // remove from the list of windows:
  Fl_X* ip = Fl_X::i(pWindow);
  Fl_X** pp = &Fl_X::first;

  for (; *pp != ip; pp = &(*pp)->next) if (!*pp) return 1;

  *pp = ip->next;

  pWindow->i = 0;

  // recursively remove any subwindows:
  for (Fl_X* wi = Fl_X::first; wi;)
  {
    Fl_Window* W = wi->w;

    if (W->window() == pWindow)
    {
      W->hide();
      W->set_visible();
      wi = Fl_X::first;
    }

    else wi = wi->next;
  }

  if (pWindow == Fl::modal_)   // we are closing the modal window, find next one:
  {
    Fl_Window* W;

    for (W = Fl::first_window(); W; W = Fl::next_window(W))
      if (W->modal()) break;

    Fl::modal_ = W;
  }

  // Make sure no events are sent to this window:
  fl_throw_focus(pWindow);
  pWindow->handle(FL_HIDE);
  return 0;
}


void
Fl_Window_Driver::use_border()
{
  if (shown())
  {
    pWindow->hide(); // hide and then show to reflect the new state of the window border
    pWindow->show();
  }
}

void
Fl_Window_Driver::size_range()
{
  pWindow->size_range_set = 1;
}

void
Fl_Window_Driver::flush()
{
  pWindow->flush();
}

void
Fl_Window_Driver::wait_for_expose()
{
  if (!shown()) return;

  Fl_X* i = Fl_X::i(pWindow);

  while (!i || wait_for_expose_value)
  {
    Fl::wait();
  }
}
