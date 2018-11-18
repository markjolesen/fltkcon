// scrlbar.cxx
//
// Scroll bar widget for the Fast Light Tool Kit (FLTK).
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
#include "scrlbar.h"
#include "fl.h"
#include "fl_draw.h"
#include "flstring.h"

#define INITIALREPEAT .5
#define REPEAT .05

void
Fl_Scrollbar::increment_cb()
{
  char inv = maximum() < minimum();
  int ls = inv ? -linesize_ : linesize_;
  int i;

  switch (pushed_)
  {
    case 1:
      i = -ls;
      break;

    default:
      i =  ls;
      break;

    case 5:
      i = -(int((maximum() - minimum())));

      if (inv)
      {
        if (i < -ls) i = -ls;
      }

      else
      {
        if (i > -ls) i = -ls; // err
      }

      break;

    case 6:
      i = -(int((maximum() - minimum())));

      if (inv)
      {
        if (i > ls) i = ls;
      }

      else
      {
        if (i < ls) i = ls; // err
      }

      break;
  }

  handle_drag(clamp(value() + i));
}

void
Fl_Scrollbar::timeout_cb(void* v)
{
  Fl_Scrollbar* s = (Fl_Scrollbar*)v;
  s->increment_cb();
  Fl::add_timeout(REPEAT, timeout_cb, s);
}

int
Fl_Scrollbar::handle(int event)
{
  int area;
  int X = x();
  int Y = y();
  int W = w();
  int H = h();

  if (horizontal())
  {
    if (W >= 3 * H)
    {
      X += H;
      W -= 2 * H;
    }
  }

  else
  {
    if (H >= 3 * W)
    {
      Y += W;
      H -= 2 * W;
    }
  }

  int relx;
  int ww;

  if (horizontal())
  {
    relx = Fl::event_x() - X;
    ww = W;
  }

  else
  {
    relx = Fl::event_y() - Y;
    ww = H;
  }

  if (relx < 0) area = 1;

  else if (relx >= ww) area = 2;

  else
  {
    int S = 0;
    int T = (horizontal() ? H : W) / 2 + 1;

    if (type() == FL_VERT_NICE_SLIDER || type() == FL_HOR_NICE_SLIDER) T += 4;

    if (S < T) S = T;

    double val =
      (maximum() - minimum()) ? (value() - minimum()) / (maximum() - minimum()) : 0.5;
    int sliderx;

    if (val >= 1.0) sliderx = ww - S;

    else if (val <= 0.0) sliderx = 0;

    else sliderx = int(val * (ww - S) + .5);

    if (Fl::event_button() == FL_MIDDLE_MOUSE) area = 8;

    else if (relx < sliderx) area = 5;

    else if (relx >= sliderx + S) area = 6;

    else area = 8;
  }

  switch (event)
  {
    case FL_ENTER:
    case FL_LEAVE:
      return 1;

    case FL_RELEASE:
      damage(FL_DAMAGE_ALL);

      if (pushed_)
      {
        Fl::remove_timeout(timeout_cb, this);
        pushed_ = 0;
      }

      handle_release();
      return 1;

    case FL_PUSH:
      if (pushed_) return 1;

      if (area != 8) pushed_ = area;

      if (pushed_)
      {
        handle_push();
        Fl::add_timeout(INITIALREPEAT, timeout_cb, this);
        increment_cb();
        damage(FL_DAMAGE_ALL);
        return 1;
      }

      return Fl_Slider::handle(event, X, Y, W, H);

    case FL_DRAG:
      if (pushed_) return 1;

      return Fl_Slider::handle(event, X, Y, W, H);

    case FL_MOUSEWHEEL :
      if (horizontal())
      {
        if (Fl::e_dx == 0) return 0;

        int ls = maximum() >= minimum() ? linesize_ : -linesize_;
        handle_drag(clamp(value() + ls * Fl::e_dx));
        return 1;
      }

      else
      {
        if (Fl::e_dy == 0) return 0;

        int ls = maximum() >= minimum() ? linesize_ : -linesize_;
        handle_drag(clamp(value() + ls * Fl::e_dy));
        return 1;
      }

    case FL_SHORTCUT:
    case FL_KEYBOARD:
    {
      int v = value();
      int ls = maximum() >= minimum() ? linesize_ : -linesize_;

      if (horizontal())
      {
        switch (Fl::event_key())
        {
          case FL_Left:
            v -= ls;
            break;

          case FL_Right:
            v += ls;
            break;

          default:
            return 0;
        }
      }

      else
      {
        switch (Fl::event_key())
        {
          case FL_Up:
            v -= ls;
            break;

          case FL_Down:
            v += ls;
            break;

          case FL_Page_Up:
            v -= int((maximum() - minimum()));
            v += ls;
            break;

          case FL_Page_Down:
            v += int((maximum() - minimum()));
            v -= ls;
            break;

          case FL_Home:
            v = int(minimum());
            break;

          case FL_End:
            v = int(maximum());
            break;

          default:
            return 0;
        }
      }

      v = int(clamp(v));

      if (v != value())
      {
        Fl_Slider::value(v);
        value_damage();
        set_changed();
        do_callback();
      }

      return 1;
    }
  }

  return 0;
}

void
Fl_Scrollbar::draw()
{
  bool l_hz = horizontal();
  int l_pos_x;
  int l_pos_y;
  unsigned int l_len_x;
  unsigned int l_len_y;

  if (l_hz)
  {
    l_pos_x = x_ + 1;
    l_pos_y = y_;
    l_len_x = w_ - 2;
    l_len_y = h_;
  }

  else
  {
    l_pos_x = x_;
    l_pos_y = y_ + 1;
    l_len_x = w_;
    l_len_y = h_ - 2;
  }

  Fl_Slider::draw(l_pos_x, l_pos_y, l_len_x, l_len_y);

  if (l_hz)
  {
    Fl::draw_putc(
      x_,
      y_,
      Fl::_skin_sym.left,
      1,
      Fl::fcolor_white,
      Fl::bcolor_black);

    l_pos_x = (x_ + w_ - 1);

    Fl::draw_putc(
      l_pos_x,
      y_,
      Fl::_skin_sym.right,
      1,
      Fl::fcolor_white,
      Fl::bcolor_black);
  }

  else
  {
    l_pos_x = (x_ + (w_ / 2));

    Fl::draw_putc(
      l_pos_x,
      y_,
      Fl::_skin_sym.up,
      1,
      Fl::fcolor_white,
      Fl::bcolor_black);

    l_pos_y = (y_ + (h_ - 1));

    Fl::draw_putc(
      x_,
      l_pos_y,
      Fl::_skin_sym.down,
      1,
      Fl::fcolor_white,
      Fl::bcolor_black);
  }

}

Fl_Scrollbar::Fl_Scrollbar(int const X, int const Y, unsigned int const W,
                           unsigned int const H, const unsigned char* L)
  : Fl_Slider(X, Y, W, H, L)
{
  linesize_ = 16;
  pushed_ = 0;
  step(1);
}

Fl_Scrollbar::~Fl_Scrollbar()
{
  if (pushed_)
    Fl::remove_timeout(timeout_cb, this);
}
