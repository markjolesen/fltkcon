// counter.cxx
//
// Counter widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2010 by Bill Spitzak and others.
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
#include "counter.h"
#include "counters.h"
#include "fl.h"
#include "fl_caret.h"
#include "fl_draw.h"
#include "fl_rend.h"
#include "fl_skin.h"

void
Fl_Counter::draw()
{
  int xx[5], ww[5];

  if (type() == FL_NORMAL_COUNTER)
  {
    int W = w() * 15 / 100;
    xx[1] = x();
    ww[1] = W;
    xx[2] = x() + 1 * W;
    ww[2] = W;
    xx[0] = x() + 2 * W;
    ww[0] = w() - 4 * W;
    xx[3] = x() + w() - 2 * W;
    ww[3] = W;
    xx[4] = x() + w() - 1 * W;
    ww[4] = W;
  }

  else
  {
    int W = w() * 20 / 100;
    xx[1] = 0;
    ww[1] = 0;
    xx[2] = x();
    ww[2] = W;
    xx[0] = x() + W;
    ww[0] = w() - 2 * W;
    xx[3] = x() + w() - 1 * W;
    ww[3] = W;
    xx[4] = 0;
    ww[4] = 0;
  }

  Fl::draw_fill(
    xx[2] + 1,
    y_,
    ww[0],
    h_,
    0x20,
    skin_.normal_fcolor,
    skin_.normal_bcolor);

  unsigned char str[128];
  format(str);
  fl_draw(str, xx[0], y(), ww[0], h(), Fl_Label::FL_ALIGN_CENTER,
          skin_.normal_fcolor, skin_.normal_bcolor, skin_.normal_fcolor);

  if (!(damage()&FL_DAMAGE_ALL)) return;

  if (type() == FL_NORMAL_COUNTER)
  {
    Fl::draw_putc(
      xx[1],
      y_,
      Fl::_skin_sym.lequal,
      1,
      skin_.normal_fcolor,
      skin_.normal_bcolor);
  }

  Fl::draw_putc(
    xx[2],
    y_,
    Fl::_skin_sym.left,
    1,
    skin_.normal_fcolor,
    skin_.normal_bcolor);

  Fl::draw_putc(
    xx[3],
    y_,
    Fl::_skin_sym.right,
    1,
    skin_.normal_fcolor,
    skin_.normal_bcolor);

  if (type() == FL_NORMAL_COUNTER)
  {
    Fl::draw_putc(
      xx[4],
      y_,
      Fl::_skin_sym.gequal,
      1,
      skin_.normal_fcolor,
      skin_.normal_bcolor);
  }
}

void
Fl_Counter::increment_cb()
{
  if (!mouseobj) return;

  double v = value();

  switch (mouseobj)
  {
    case 1:
      v -= lstep_;
      break;

    case 2:
      v = increment(v, -1);
      break;

    case 3:
      v = increment(v, 1);
      break;

    case 4:
      v += lstep_;
      break;
  }

  handle_drag(clamp(round(v)));
}

#define INITIALREPEAT .5
#define REPEAT .1

void
Fl_Counter::repeat_callback(void* v)
{
  Fl_Counter* b = (Fl_Counter*)v;

  if (b->mouseobj)
  {
    Fl::add_timeout(REPEAT, repeat_callback, b);
    b->increment_cb();
  }
}

int
Fl_Counter::calc_mouseobj()
{
  if (type() == FL_NORMAL_COUNTER)
  {
    int W = w() * 15 / 100;

    if (Fl::event_inside(x(), y(), W, h())) return 1;

    if (Fl::event_inside(x() + W, y(), W, h())) return 2;

    if (Fl::event_inside(x() + w() - 2 * W, y(), W, h())) return 3;

    if (Fl::event_inside(x() + w() - W, y(), W, h())) return 4;
  }

  else
  {
    int W = w() * 20 / 100;

    if (Fl::event_inside(x(), y(), W, h())) return 2;

    if (Fl::event_inside(x() + w() - W, y(), W, h())) return 3;
  }

  return -1;
}

bool
Fl_Counter::handle(
  enum Fl_Event const event)
{
  int i;

  switch (event)
  {
    case FL_RELEASE:
      if (mouseobj)
      {
        Fl::remove_timeout(repeat_callback, this);
        mouseobj = 0;
        redraw();
      }

      handle_release();
      return 1;

    case FL_PUSH:
      if (Fl::visible_focus()) Fl::focus(this);

      {
        Fl_Widget_Tracker wp(this);
        handle_push();

        if (wp.deleted()) return 1;
      }

    case FL_DRAG:
      i = calc_mouseobj();

      if (i != mouseobj)
      {
        Fl::remove_timeout(repeat_callback, this);
        mouseobj = (unsigned char)i;

        if (i) Fl::add_timeout(INITIALREPEAT, repeat_callback, this);

        Fl_Widget_Tracker wp(this);
        increment_cb();

        if (wp.deleted()) return 1;

        redraw();
      }

      return 1;

    case FL_KEYBOARD :
      switch (Fl::event_key())
      {
        case FL_Left:
          handle_drag(clamp(increment(value(), -1)));
          return 1;

        case FL_Right:
          handle_drag(clamp(increment(value(), 1)));
          return 1;

        default:
          return 0;
      }

    case FL_FOCUS :
      Fl::caret_hide();

    case FL_UNFOCUS :
      if (Fl::visible_focus())
      {
        redraw();
        return 1;
      }

      else return 0;

    case FL_ENTER :
    case FL_LEAVE :
      return 1;

    default:
      return 0;
  }
}

Fl_Counter::~Fl_Counter()
{
  Fl::remove_timeout(repeat_callback, this);
}

Fl_Counter::Fl_Counter(int const X, int const Y, unsigned int const W,
                       unsigned int const H, const unsigned char* L)
  : Fl_Valuator(X, Y, W, H, L)
{
  align(Fl_Label::FL_ALIGN_BOTTOM);
  bounds(-1000000.0, 1000000.0);
  Fl_Valuator::step(1, 10);
  lstep_ = 1.0;
  mouseobj = 0;
}


Fl_Simple_Counter::Fl_Simple_Counter(int const X, int const Y,
                                     unsigned int const W, unsigned int const H, const unsigned char* L)
  : Fl_Counter(X, Y, W, H, L)
{
  type(FL_SIMPLE_COUNTER);
}
