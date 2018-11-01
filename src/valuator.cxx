// valuator.cxx
//
// Valuator widget for the Fast Light Tool Kit (FLTK).
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
#include <math.h>
#include "valuator.h"
#include "fl.h"
#include "flstring.h"

#if defined(__DOS__)
extern "C" double rint(double);
#endif

Fl_Valuator::Fl_Valuator(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label) :
  Fl_Widget(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label)
{
  align(Fl_Label::FL_ALIGN_BOTTOM);
  when(FL_WHEN_CHANGED);
  value_ = 0;
  previous_value_ = 1;
  min = 0;
  max = 1;
  A = 0.0;
  B = 1;
}

const double epsilon = 4.66e-10;

void
Fl_Valuator::step(double s)
{
  if (s < 0) s = -s;

  A = rint(s);
  B = 1;

  while (fabs(s - A / B) > epsilon && B <= (0x7fffffff / 10))
  {
    B *= 10;
    A = rint(s * B);
  }
}

void
Fl_Valuator::precision(int digits)
{
  if (digits > 9) digits = 9;

  else if (digits < 0) digits = 0;

  A = 1.0;

  for (B = 1; digits--;) B *= 10;
}

void
Fl_Valuator::value_damage()
{
  damage(FL_DAMAGE_EXPOSE);
}

int
Fl_Valuator::value(double v)
{
  clear_changed();

  if (v == value_) return 0;

  value_ = v;
  value_damage();
  return 1;
}

double
Fl_Valuator::softclamp(double v)
{
  int which = (min <= max);
  double p = previous_value_;

  if ((v < min) == which && p != min && (p < min) != which) return min;

  else if ((v > max) == which && p != max && (p > max) != which) return max;

  else return v;
}

void
Fl_Valuator::handle_drag(double v)
{
  if (v != value_)
  {
    value_ = v;
    value_damage();
    set_changed();

    if (when() & FL_WHEN_CHANGED) do_callback();
  }
}

void
Fl_Valuator::handle_release()
{
  if (when()&FL_WHEN_RELEASE)
  {
    clear_changed();

    if (value_ != previous_value_ || when() & FL_WHEN_NOT_CHANGED)
    {
      do_callback();
    }
  }
}

double
Fl_Valuator::round(double v) const
{
  if (A) return rint(v * B / A) * A / B;

  else return v;
}

double
Fl_Valuator::clamp(double v)
{
  if ((v < min) == (min <= max)) return min;

  else if ((v > max) == (min <= max)) return max;

  else return v;
}

double
Fl_Valuator::increment(double v, int n)
{
  if (!A) return v + n * (max - min) / 100;

  if (min > max) n = -n;

  return (rint(v * B / A) + n) * A / B;
}

int
Fl_Valuator::format(unsigned char* buffer)
{
  double v = value();

  if (!A || !B) return snprintf((char*)buffer, 128, "%g", v);

  int i, c = 0;
  char temp[32];
  snprintf(temp, sizeof(temp), "%.12f", A / B);

  for (i = (int) strlen(temp) - 1; i > 0; i--)
  {
    if (temp[i] != '0') break;
  }

  for (; i > 0; i--, c++)
  {
    if (!isdigit(temp[i])) break;
  }

  return snprintf((char*)buffer, 128, "%.*f", c, v);
}
