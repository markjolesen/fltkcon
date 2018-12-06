// spinner.cxx
//
// Spinner widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
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
#include <stdio.h>
#include <stdlib.h>
#include "spinner.h"

void
Fl_Spinner::sb_cb(Fl_Widget* w, Fl_Spinner* sb)
{
  double v;

  if (w == &(sb->input_))
  {
    v = atof((char*)sb->input_.value());

    if (v < sb->minimum_)
    {
      sb->value_ = sb->minimum_;
      sb->update();
    }

    else if (v > sb->maximum_)
    {
      sb->value_ = sb->maximum_;
      sb->update();
    }

    else sb->value_ = v;
  }

  else if (w == &(sb->up_button_))
  {
    v = sb->value_ + sb->step_;

    if (v > sb->maximum_)
    {
      if (sb->wrap_)
        v = sb->minimum_;

      else
        v = sb->maximum_;
    }

    sb->value_ = v;
    sb->update();
  }

  else if (w == &(sb->down_button_))
  {
    v = sb->value_ - sb->step_;

    if (v < sb->minimum_)
    {
      if (sb->wrap_)
        v = sb->maximum_;

      else
        v = sb->minimum_;
    }

    sb->value_ = v;
    sb->update();
  }

  sb->set_changed();
  sb->do_callback();
}

void
Fl_Spinner::update()
{
  char s[255];

  if (format_[0] == '%' && format_[1] == '.'
      && format_[2] == '*')
  {
    int c = 0;
    char temp[64], *sp = temp;
    sprintf(temp, "%.12f", step_);

    while (*sp) sp++;

    sp--;

    while (sp > temp && *sp == '0') sp--;

    while (sp > temp && (*sp >= '0' && *sp <= '9'))
    {
      sp--;
      c++;
    }

    sprintf(s, format_, c, value_);
  }

  else
  {
    sprintf(s, format_, value_);
  }

  input_.value((unsigned char*)s);
  input_.damage(Fl_Widget::FL_DAMAGE_ALL);
}

#if 0
#define FL_UP_ARROW_TX (unsigned char*)"@-42<"
#define FL_DOWN_ARROW_TX (unsigned char*)"@-42>"
#endif

#define FL_UP_ARROW_TX (unsigned char*)"+"
#define FL_DOWN_ARROW_TX (unsigned char*)"-"

Fl_Spinner::Fl_Spinner(int const X, int const Y, unsigned int const W,
                       unsigned int const H, const unsigned char* L)
  : Fl_Group(X, Y, W, H, L),
    input_(X, Y, W - 2, 1),
    up_button_(X + W - 2, Y, 1, 1, FL_UP_ARROW_TX),
    down_button_(X + W - 1, Y, 1, 1, FL_DOWN_ARROW_TX)
{
  end();

  value_   = 1.0;
  minimum_ = 1.0;
  maximum_ = 100.0;
  step_    = 1.0;
  wrap_    = 1;
  format_  = "%g";

  align(Fl_Label::FL_ALIGN_LEFT);

  input_.value((unsigned char*)"1");
  input_.type(Fl_Input_::FL_INT_INPUT);
  input_.when(FL_WHEN_ENTER_KEY | FL_WHEN_RELEASE);
  input_.callback((Fl_Callback*)sb_cb, this);

  up_button_.callback((Fl_Callback*)sb_cb, this);

  down_button_.callback((Fl_Callback*)sb_cb, this);
}

bool
Fl_Spinner::handle(
  enum Fl_Event const event)
{

  switch (event)
  {

    case FL_KEYDOWN:
    case FL_SHORTCUT:
      if (Fl::event_key() == FL_Up)
      {
        up_button_.do_callback();
        return 1;
      }

      else if (Fl::event_key() == FL_Down)
      {
        down_button_.do_callback();
        return 1;
      }

      else if (Fl::event_key() == FL_Left || Fl::event_key() == FL_Right)
      {
        input_.handle(event);
        return 1;
      }

      return 0;

    case FL_FOCUS:
      if (input_.take_focus()) return 1;

      return 0;
    default:
      break;
  }

  return Fl_Group::handle(event);
}

void
Fl_Spinner::resize(int const X, int const Y, unsigned int const W, unsigned int const H)
{
  Fl_Group::resize(X, Y, W, H);

  input_.resize(X, Y, W - H / 2 - 2, H);
  up_button_.resize(X + W - H / 2 - 2, Y, H / 2 + 2, H / 2);
  down_button_.resize(X + W - H / 2 - 2, Y + H - H / 2,
                      H / 2 + 2, H / 2);
}

void
Fl_Spinner::step(double s)
{
  step_ = s;

  if (step_ != (int)step_) input_.type(Fl_Input_::FL_FLOAT_INPUT);

  else input_.type(Fl_Input_::FL_INT_INPUT);

  update();
}

void
Fl_Spinner::type(unsigned char const v)
{
  if (v == Fl_Input_::FL_FLOAT_INPUT)
  {
    format("%.*f");
  }

  else
  {
    format("%.0f");
  }

  input_.type(v);
}

bool
Fl_Spinner::Fl_Spinner_Input::handle(
  enum Fl_Event const event)
{
  if (event == FL_KEYBOARD)
  {
    const int key = Fl::event_key();

    if (key == FL_Up || key == FL_Down)
    {
      Fl_Input::handle(FL_UNFOCUS);
      return 0;
    }
  }

  return Fl_Input::handle(event);
}
