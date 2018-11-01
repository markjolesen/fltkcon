// win.cxx
//
// Window widget class for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2016 by Bill Spitzak and others.
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
#include <stdlib.h>
#include "win.h"
#include "drvwin.h"
#include "fl.h"
#include "fl_clip.h"
#include "fl_draw.h"
#include "flstring.h"
#include "platform.h"

char Fl_Window::show_iconic_ = 0;

Fl_Window* Fl_Window::current_;

void
Fl_Window::_Fl_Window()
{
  type(FL_WINDOW);
  labeltype(Fl_Label::FL_NO_LABEL);
  i = 0;
  iconlabel_ = 0;
  size_range_set = 0;
  minw = maxw = minh = maxh = 0;
  callback((Fl_Callback*)default_callback);
}

Fl_Window::Fl_Window(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label) :
  Fl_Group(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label),
  pWindowDriver(Fl_Window_Driver::newWindowDriver(this)),
  skin_(Fl::_skin_win)
{
  _Fl_Window();
  set_flag(FORCE_POSITION);
}

Fl_Window::Fl_Window(int W, int H, const unsigned char* l) :
  Fl_Group((Fl_Group::current(0), 0), 0, W, H, l),
  pWindowDriver(Fl_Window_Driver::newWindowDriver(this))
{
  _Fl_Window();
  clear_visible();
}

Fl_Window::~Fl_Window()
{
  hide();

  delete pWindowDriver;
}


Fl_Window*
Fl_Widget::window() const
{
  for (Fl_Widget* o = parent(); o; o = o->parent())
    if (o->type() >= FL_WINDOW) return (Fl_Window*)o;

  return 0;
}

Fl_Window*
Fl_Widget::top_window() const
{
  const Fl_Widget* w = this;

  while (w->parent())
  {
    w = w->parent();
  }

  return const_cast<Fl_Widget*>(w)->as_window();
}

Fl_Window*
Fl_Widget::top_window_offset(int& xoff, int& yoff) const
{
  xoff = yoff = 0;
  const Fl_Widget* w = this;

  while (w && w->window())
  {
    xoff += w->x();
    yoff += w->y();
    w = w->window();
  }

  return const_cast<Fl_Widget*>(w)->as_window();
}

int
Fl_Window::x_root() const
{
  Fl_Window* p = window();

  if (p) return p->x_root() + x();

  return x();
}

int
Fl_Window::y_root() const
{
  Fl_Window* p = window();

  if (p) return p->y_root() + y();

  return y();
}

void
Fl_Window::label(const unsigned char* name)
{
  label(name, iconlabel());
}

void
Fl_Window::copy_label(const unsigned char* a)
{
  Fl_Widget::copy_label(a);
  label(label(), iconlabel());
}

void
Fl_Window::iconlabel(const unsigned char* iname)
{
  label(label(), iname);
}

void
Fl::default_atclose(Fl_Window* window, void* v)
{
  window->hide();
  Fl_Widget::default_callback(window, v); // put on Fl::read_queue()
}

void (*Fl::atclose)(Fl_Window*, void*) = default_atclose;

void
Fl_Window::default_callback(Fl_Window* win, void* v)
{
  Fl::atclose(win, v);
}

Fl_Window*
Fl_Window::current()
{
  return current_;
}

int
Fl_Window::decorated_w() const
{
  return pWindowDriver->decorated_w();
}


int
Fl_Window::decorated_h() const
{
  return pWindowDriver->decorated_h();
}

void
Fl_Window::flush()
{
  if (!shown()) return;

  make_current();
  draw();

  return;
}


void
Fl_Window::draw()
{

  pWindowDriver->draw_begin();

  if (damage() & ~FL_DAMAGE_CHILD)
  {
    Fl::draw_fill(0, 0, w_, h_,
                  skin_.fillc, skin_.fill_fcolor, skin_.fill_bcolor);
    draw_box(0, 0, w_, h_,
             Fl_Widget::skin_.box_fcolor, Fl_Widget::skin_.box_bcolor);
  }

  draw_children();

  pWindowDriver->draw_end();

  return;
}

void
Fl_Window::make_current()
{
  pWindowDriver->make_current();
  current_ = this;
}

void
Fl_Window::label(const unsigned char* name, const unsigned char* mininame)
{
  Fl_Widget::label(name);
  iconlabel_ = mininame;
  pWindowDriver->label(name, mininame);
}

void
Fl_Window::show()
{
  labeltype(Fl_Label::FL_NO_LABEL);
  pWindowDriver->show();
}

void
Fl_Window::resize(int X, int Y, int W, int H)
{
  pWindowDriver->resize(X, Y, W, H);
}

void
Fl_Window::hide()
{
  pWindowDriver->hide();
}

bool
Fl_Window::handle(enum Fl_Event const ev)
{
  if (parent())
  {
    switch (ev)
    {
      case FL_SHOW:
        if (!shown()) show();

        else
        {
          pWindowDriver->map();
        }

        break;

      case FL_HIDE:
        if (shown())
        {
          if (visible())
          {
            Fl_Widget* p = parent();

            for (; p->visible(); p = p->parent()) {}

            if (p->type() >= FL_WINDOW) break;
          }

          pWindowDriver->unmap();
        }

        break;
    }
  }

  return Fl_Group::handle(ev);
}

void
Fl_Window::size_range(int minw, int minh, int maxw, int maxh, int dw, int dh,
                      int aspect)
{
  this->minw   = minw;
  this->minh   = minh;
  this->maxw   = maxw;
  this->maxh   = maxh;
  this->dw     = dw;
  this->dh     = dh;
  this->aspect = aspect;
  pWindowDriver->size_range();
}
