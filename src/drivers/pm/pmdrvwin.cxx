// pmdrvwin.cxx
//
// Protected Mode Window handling code for the Fast Light Tool Kit (FLTK).
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
#include "pmdrvwin.h"
#include "drvgr.h"
#include "fl_draw.h"
#include "platform.h"
#include "mouse.h"
#include <string.h>

Window fl_window = 0;

Fl_Window_Driver*
Fl_Window_Driver::newWindowDriver(Fl_Window* w)
{
  return new Fl_PM_Window_Driver(w);
}

Fl_PM_Window_Driver::Fl_PM_Window_Driver(Fl_Window* w) :
  Fl_Window_Driver(w),
  block_(block_new())
{
  return;
}

Fl_PM_Window_Driver::~Fl_PM_Window_Driver()
{
  block_free(block_);
  return;
}

int
Fl_PM_Window_Driver::decorated_w()
{
  return w();
}

void
Fl_PM_Window_Driver::draw_begin()
{

  mouse_hide();

  if (Fl_Widget::FL_DAMAGE_ALL & pWindow->damage())
  {

    if (pWindow->get_border())
    {
      Fl::draw_frame(
        (pWindow->x() - 1),
        (pWindow->y() - 1),
        (pWindow->w() + 2),
        (pWindow->h() + 2),
        pWindow->Fl_Widget::skin_.box_fcolor,
        pWindow->Fl_Widget::skin_.box_bcolor);
    }

    unsigned char const* label = pWindow->label();

    if (label)
    {
      unsigned int len = (unsigned int)strlen((char const*)label);
      int center = (((pWindow->w() + 2) / 2) - (len / 2));
      Fl::draw_puts(
        (pWindow->x() + center - 1),
        (pWindow->y() - 1),
        label,
        len,
        pWindow->Fl_Widget::skin_.normal_fcolor,
        pWindow->Fl_Widget::skin_.normal_bcolor);
    }
  }

  Fl_Graphics_Driver::draw_offset_x = pWindow->x();
  Fl_Graphics_Driver::draw_offset_y = pWindow->y();

  if (FL_CHILD_WINDOW == pWindow->type())
  {
    Fl_Group* p = pWindow->parent();
    Fl_Window* w = p->as_window();

    while (w)
    {
      Fl_Graphics_Driver::draw_offset_x += w->x();
      Fl_Graphics_Driver::draw_offset_y += w->y();
      p = w->parent();

      if (p)
      {
        w = p->as_window();
      }

      else
      {
        break;
      }
    }
  }

  return;
}

void
Fl_PM_Window_Driver::draw_end()
{

  Fl_Graphics_Driver::draw_offset_x = 0;
  Fl_Graphics_Driver::draw_offset_y = 0;

  mouse_show();

  return;
}

int
Fl_PM_Window_Driver::decorated_h()
{
  return h();
}

Fl_X*
Fl_PM_Window_Driver::makeWindow()
{
  Fl_Group::current(0);
  Fl_X* x = new Fl_X;
  i(x);
  x->w = pWindow;
  x->next = Fl_X::first;
  Fl_X::first = x;

  pWindow->set_visible();
  pWindow->redraw();
  Fl_Event old_event = Fl::e_number;
  pWindow->handle(Fl::e_number = FL_SHOW);
  Fl::e_number = old_event;

  return x;
}

void
Fl_PM_Window_Driver::take_focus()
{
  if (!shown())
  {
    pWindow->show();
  }
}

void
Fl_PM_Window_Driver::show()
{
  if (!shown())
  {
    fl_open_display();
    makeWindow();
  }
}

void
Fl_PM_Window_Driver::hide()
{
  hide_common();
}

void
Fl_PM_Window_Driver::erase_menu()
{

  mouse_hide();
  block_write(block_);
  mouse_show();

  return;
}

void
Fl_PM_Window_Driver::show_menu()
{

  Fl_Window_Driver::show_menu();

  int x = pWindow->x();
  int y = pWindow->y();
  int w = pWindow->w();
  int h = pWindow->h();

  if (pWindow->get_border())
  {
    x--;
    y--;
    w += 2;
    h += 2;
  }

  mouse_hide();
  block_read(block_, x, y, w, h);
  mouse_show();

  return;
}

void
Fl_PM_Window_Driver::resize(int X, int Y, int W, int H)
{

  pWindow->Fl_Group::resize(X, Y, W, H);

  return;
}

int
Fl_PM_Window_Driver::scrollto(int src_x, int src_y, int src_w, int src_h,
                              int dest_x, int dest_y, void (*draw_area)(void*, int, int, int, int),
                              void* data)
{
  return 0;
}
