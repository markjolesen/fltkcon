// inpvalue.cxx
//
// Value input widget for the Fast Light Tool Kit (FLTK).
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
#include <stdlib.h>
#include <math.h>
#include "inpvalue.h"
#include "fl.h"
#include "group.h"

void
Fl_Value_Input::input_cb(Fl_Widget*, void* v)
{
  Fl_Value_Input& t = *(Fl_Value_Input*)v;
  double nv;

  if ((t.step() - floor(t.step())) > 0.0
      || t.step() == 0.0) nv = strtod((char const*)t.input.value(), 0);

  else nv = strtol((char const*)t.input.value(), 0, 0);

  if (nv != t.value() || t.when() & FL_WHEN_NOT_CHANGED)
  {
    t.set_value(nv);
    t.set_changed();

    if (t.when()) t.do_callback();
  }
}

void
Fl_Value_Input::draw()
{
  if (damage() & ~FL_DAMAGE_CHILD) input.clear_damage(FL_DAMAGE_ALL);

  Fl_Widget* i = &input;
  i->draw();
  input.clear_damage();
}

void
Fl_Value_Input::resize(int X, int Y, int W, int H)
{
  Fl_Valuator::resize(X, Y, W, H);
  input.resize(X, Y, W, H);
}

void
Fl_Value_Input::value_damage()
{
  unsigned char buf[128];
  format(buf);
  input.value(buf);
//_mjo
#if 0
  input.mark(input.position()); // turn off selection highlight
#endif
}

bool
Fl_Value_Input::handle(enum Fl_Event const event)
{
  double v;
  int delta;
  int mx = Fl::event_x_root();
  static int ix, drag;
  input.when(when());

  switch (event)
  {
    case FL_PUSH:
      if (!step()) goto DEFAULT;

      ix = mx;
      drag = Fl::event_button();
      handle_push();
      return 1;

    case FL_DRAG:
      if (!step()) goto DEFAULT;

      delta = mx - ix;

      if (delta > 5) delta -= 5;

      else if (delta < -5) delta += 5;

      else delta = 0;

      switch (drag)
      {
        case 3:
          v = increment(previous_value(), delta * 100);
          break;

        case 2:
          v = increment(previous_value(), delta * 10);
          break;

        default:
          v = increment(previous_value(), delta);
          break;
      }

      v = round(v);
      handle_drag(soft() ? softclamp(v) : clamp(v));;
      return 1;

    case FL_RELEASE:
      if (!step()) goto DEFAULT;

      if (value() != previous_value() || !Fl::event_is_click())
        handle_release();

      else
      {
        Fl_Widget_Tracker wp(&input);
        input.handle(FL_PUSH);

        if (wp.exists())
          input.handle(FL_RELEASE);
      }

      return 1;

    case FL_FOCUS:
      return input.take_focus();

    case FL_SHORTCUT:
      return input.handle(event);

    default:
    DEFAULT:
      input.type(((step() - floor(step())) > 0.0
                  || step() == 0.0) ? Fl_Input_::FL_FLOAT_INPUT : Fl_Input_::FL_INT_INPUT);
      return input.handle(event);
  }
}

Fl_Value_Input::Fl_Value_Input(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label) :
  Fl_Valuator(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label),
  input(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label)
{
  soft_ = 0;

  if (input.parent())
    input.parent()->remove(input);

  input.parent((Fl_Group*)this);
  input.callback(input_cb, this);
  input.when(FL_WHEN_CHANGED);
  align(Fl_Label::FL_ALIGN_LEFT);
  value_damage();
  set_flag(SHORTCUT_LABEL);
}

Fl_Value_Input::~Fl_Value_Input()
{

  if (input.parent() == (Fl_Group*)this)
    input.parent(0);
}
