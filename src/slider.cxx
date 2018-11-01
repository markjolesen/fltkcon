// slider.cxx
//
// Slider widget for the Fast Light Tool Kit (FLTK).
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
#include "slider.h"
#include "fl_caret.h"
#include "fl_draw.h"
#include "fl.h"
#include "fl_skin.h"
#include "flstring.h"
#include "slfill.h"
#include "slhzfill.h"
#include "slhz.h"
#include "slhznice.h"
#include "slnice.h"

Fl_Slider::Fl_Slider(int const X, int const Y, unsigned int const W,
                     unsigned int const H, const unsigned char* L)
  : Fl_Valuator(X, Y, W, H, L)
{
}

double
Fl_Slider::value2pct(
  double const i_value) const
{
  bool l_flip;
  bool l_hz;
  double l_min;
  double l_max;
  double l_pct;
  double l_tmp;
  double l_value;

  do
  {
    l_hz = horizontal();

    if (min == max)
    {
      l_pct = 0.;
      break;
    }

    l_flip = (max < min);

    if (!l_flip)
    {
      l_min = min;
      l_max = max;
    }

    else
    {
      l_min = max;
      l_max = min;
    }

    if (false == l_hz)
    {
      l_flip = !l_flip;
    }

    if (l_max > 0)
    {
      l_value = i_value;
    }

    else
    {
      l_flip = !l_flip;
      l_tmp = l_min;
      l_min = -l_max;
      l_max = -l_tmp;
      l_value = -i_value;
    }

    l_pct = (l_value - l_min) / (l_max - l_min);

    if (l_flip)
    {
      l_pct = 1 - l_pct;
    }

    if (l_pct > 1)
    {
      l_pct = 1;
    }

    else if (l_pct < 0)
    {
      l_pct = 0;
    }

  }
  while (0);

  return l_pct;
}

int
Fl_Slider::value2screen(
  double const i_value,
  int const i_len) const
{
  double l_pct;
  int l_pos;
  double l_value;

  l_pct = value2pct(i_value);
  l_value = l_pct * i_len;
  l_value = round(l_value);
  l_pos = static_cast<int>(l_value);

  return l_pos;
}

double
Fl_Slider::screen2value(
  int const i_pos,
  int const i_len) const
{
  bool l_flip;
  bool l_hz;
  double l_max;
  double l_min;
  double l_value;
  double l_pct;
  int l_pos;
  double l_tmp;

  do
  {

    l_hz = horizontal();

    if (i_len <= 0)
    {
      l_value = min;
      break;
    }

    if (i_pos == i_len - 1)
    {
      if (l_hz)
      {
        l_value = max;
      }

      else
      {
        l_value = min;
      }

      break;
    }

    l_pos = i_pos;

    l_flip = (max < min);

    if (!l_flip)
    {
      l_min = min;
      l_max = max;
    }

    else
    {
      l_min = max;
      l_max = min;
    }

    if (false == l_hz)
    {
      l_flip = !l_flip;
    }

    if (l_flip)
    {
      l_pos = i_len - l_pos;
    }

    l_pct = static_cast<double>(l_pos) / i_len;

    if (l_pct <= 0)
    {
      l_value = l_min;
      break;
    }

    if (l_pct >= 1)
    {
      l_value = l_max;
      break;
    }

    l_flip = (l_max <= 0);

    if (l_flip)
    {
      l_tmp = l_min;
      l_min = -l_max;
      l_max = -l_tmp;
      l_pct = 1 - l_pct;
    }

    l_value = l_pct * (l_max - l_min) + l_min;

    if (l_flip)
    {
      l_value = -l_value;
    }

  }
  while (0);

  return l_value;
}

void
Fl_Slider::bounds(double a, double b)
{
  if (minimum() != a || maximum() != b)
  {
    Fl_Valuator::bounds(a, b);
    damage(FL_DAMAGE_EXPOSE);
  }
}

int
Fl_Slider::scrollvalue(int pos, int size, int first, int total)
{
  step(1, 1);

  if (pos + size > first + total) total = pos + size - first;

  bounds(first, total - size + first);
  return value(pos);
}

void
Fl_Slider::draw(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y)
{
  unsigned int l_fillc;

  switch (type())
  {
    case FL_VERT_NICE_SLIDER:
      l_fillc = Fl::_skin_sym.vline;
      break;

    case FL_HOR_NICE_SLIDER:
      l_fillc = Fl::_skin_sym.hline;
      break;

    default:
      l_fillc = Fl::_skin_sym.stipple;
      break;
  }

  Fl::draw_fill(i_pos_x, i_pos_y, i_len_x, i_len_y,
                l_fillc, Fl::fcolor_white, Fl::bcolor_black);

  bool l_hz = horizontal();
  unsigned int l_len = (l_hz) ? i_len_x : i_len_y;
  int l_value = value2screen(value(), l_len);

  switch (type())
  {
    case FL_VERT_FILL_SLIDER:
      l_len = (i_len_y - l_value);

      if (l_len)
      {
        Fl::draw_fill(i_pos_x, i_pos_y, i_len_x, l_len,
                      0x20, Fl::fcolor_white, Fl::bcolor_red);
      }

      break;

    case FL_HOR_FILL_SLIDER:
      if (l_value)
      {
        Fl::draw_fill(i_pos_x, i_pos_y, l_value, i_len_y,
                      0x20, Fl::fcolor_white, Fl::bcolor_red);
      }

      break;

    default:
      int l_pos_x;
      int l_pos_y;

      if (l_hz)
      {
        if (l_value)
        {
          l_value--;
        }

        l_pos_x = l_value + i_pos_x;
        l_pos_y = i_pos_y + (i_len_y / 2);
      }

      else
      {
        if (0 == l_value)
        {
          l_value++;
        }

        l_pos_y = (l_len - l_value) + i_pos_y;
        l_pos_x = i_pos_x + (i_len_x / 2);
      }

      Fl::draw_putc(
        l_pos_x,
        l_pos_y,
        Fl::_skin_sym.thumb,
        1,
        Fl::fcolor_white,
        Fl::bcolor_black);
      break;
  }

  return;
}

void
Fl_Slider::draw()
{

  draw(x_, y_, w_, h_);

  return;
}

int
Fl_Slider::handle(int event, int X, int Y, int W, int H)
{
  switch (event)
  {
    case FL_PUSH:
    {
      Fl_Widget_Tracker wp(this);

      if (!Fl::event_inside(X, Y, W, H)) return 0;

      handle_push();

      if (wp.deleted()) return 1;
    }

    case FL_DRAG:
    {

      double val;

      if (minimum() == maximum())
        val = 0.5;

      else
      {
        val = (value() - minimum()) / (maximum() - minimum());

        if (val > 1.0) val = 1.0;

        else if (val < 0.0) val = 0.0;
      }

      int ww = (horizontal() ? W : H);
      int mx = (horizontal() ? Fl::event_x() - X : Fl::event_y() - Y);
      int S;
      static int offcenter;

      if (type() == FL_HOR_FILL_SLIDER || type() == FL_VERT_FILL_SLIDER)
      {

        S = 0;

        if (event == FL_PUSH)
        {
          int xx = int(val * ww + .5);
          offcenter = mx - xx;

          if (offcenter < -10 || offcenter > 10) offcenter = 0;

          else return 1;
        }

      }

      else
      {
        S = 0;

        if (S >= ww) return 0;

        int T = (horizontal() ? H : W) / 2 + 1;

        if (type() == FL_VERT_NICE_SLIDER || type() == FL_HOR_NICE_SLIDER) T += 4;

        if (S < T) S = T;

        if (event == FL_PUSH)
        {
          int xx = int(val * (ww - S) + .5);
          offcenter = mx - xx;

          if (offcenter < 0) offcenter = 0;

          else if (offcenter > S) offcenter = S;

          else return 1;
        }
      }

      int xx = mx - offcenter;
      double v = 0;
      char tryAgain = 1;

      while (tryAgain)
      {
        tryAgain = 0;

        if (xx < 0)
        {
          xx = 0;
          offcenter = mx;

          if (offcenter < 0) offcenter = 0;
        }

        else if (xx > (ww - S))
        {
          xx = ww - S;
          offcenter = mx - xx;

          if (offcenter > S) offcenter = S;
        }

        v = round(xx * (maximum() - minimum()) / (ww - S) + minimum());

        if (event == FL_PUSH && v == value())
        {
          offcenter = S / 2;
          event = FL_DRAG;
          tryAgain = 1;
        }
      }

      handle_drag(clamp(v));
    }

    return 1;

    case FL_RELEASE:
      handle_release();
      return 1;

    case FL_KEYBOARD:
    {
      Fl_Widget_Tracker wp(this);

      switch (Fl::event_key())
      {
        case FL_Up:
          if (horizontal()) return 0;

          handle_push();

          if (wp.deleted()) return 1;

          handle_drag(clamp(increment(value(), -1)));

          if (wp.deleted()) return 1;

          handle_release();
          return 1;

        case FL_Down:
          if (horizontal()) return 0;

          handle_push();

          if (wp.deleted()) return 1;

          handle_drag(clamp(increment(value(), 1)));

          if (wp.deleted()) return 1;

          handle_release();
          return 1;

        case FL_Left:
          if (!horizontal()) return 0;

          handle_push();

          if (wp.deleted()) return 1;

          handle_drag(clamp(increment(value(), -1)));

          if (wp.deleted()) return 1;

          handle_release();
          return 1;

        case FL_Right:
          if (!horizontal()) return 0;

          handle_push();

          if (wp.deleted()) return 1;

          handle_drag(clamp(increment(value(), 1)));

          if (wp.deleted()) return 1;

          handle_release();
          return 1;

        default:
          return 0;
      }
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

bool
Fl_Slider::handle(
  enum Fl_Event const event)
{
  if (event == FL_PUSH && Fl::visible_focus())
  {
    Fl::focus(this);
    redraw();
  }

  return handle(event, x(), y(), w(), h());
}

Fl_Fill_Slider::Fl_Fill_Slider(int const X, int const Y, unsigned int const W,
                               unsigned int const H, const unsigned char* L)
  : Fl_Slider(X, Y, W, H, L)
{
  type(FL_VERT_FILL_SLIDER);
}

Fl_Hor_Slider::Fl_Hor_Slider(int const X, int const Y, unsigned int const W,
                             unsigned int const H, const unsigned char* L)
  : Fl_Slider(X, Y, W, H, L)
{
  type(FL_HOR_SLIDER);
}

Fl_Hor_Fill_Slider::Fl_Hor_Fill_Slider(int const X, int const Y,
                                       unsigned int const W, unsigned int const H, 
                                       const unsigned char* L)
  : Fl_Slider(X, Y, W, H, L)
{
  type(FL_HOR_FILL_SLIDER);
}

Fl_Hor_Nice_Slider::Fl_Hor_Nice_Slider(int const X, int const Y,
                                       unsigned int const W, unsigned int const H, 
                                       const unsigned char* L)
  : Fl_Slider(X, Y, W, H, L)
{
  type(FL_HOR_NICE_SLIDER);
}

Fl_Nice_Slider::Fl_Nice_Slider(int const X, int const Y, unsigned int const W,
                               unsigned int const H, const unsigned char* L)
  : Fl_Slider(X, Y, W, H, L)
{
  type(FL_VERT_NICE_SLIDER);
}
