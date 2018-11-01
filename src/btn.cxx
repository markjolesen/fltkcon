// btn.cxx
//
// Button widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2014 by Bill Spitzak and others.
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
#include <stdio.h>
#include "btn.h"
#include "btnrad.h"
#include "btntog.h"
#include "fl.h"
#include "fl_draw.h"
#include "fl_caret.h"
#include "group.h"
#include "win.h"

Fl_Widget_Tracker* Fl_Button::key_release_tracker = 0;

bool
Fl_Button::value(bool const v)
{
  oldval_ = v;
  clear_changed();

  if (value_ != v)
  {
    value_ = v;

    redraw_label();

    return true;
  }

  return false;
}

void
Fl_Button::setonly()
{
  value(true);
  Fl_Group* g = parent();
  Fl_Widget* const* a = g->array();

  for (int i = g->children(); i--;)
  {
    Fl_Widget* o = *a++;

    if (o != this && o->type() == FL_RADIO_BUTTON) ((Fl_Button*)o)->value(false);
  }
}

void
Fl_Button::draw()
{

  if (type() == FL_HIDDEN_BUTTON) return;

  enum Fl::foreground l_fcolor;
  enum Fl::background l_bcolor;

  if (this != Fl::focus())
  {
    l_fcolor = skin_.normal_fcolor;
    l_bcolor = skin_.normal_bcolor;
  }

  else
  {
    l_fcolor = skin_.focused_fcolor;
    l_bcolor = skin_.focused_bcolor;
  }

  unsigned char ind[5];
  unsigned char text[81];

  ind[3] = ' ';
  ind[4] = 0;

  if (FL_RADIO_BUTTON == type())
  {
    ind[0] = '(';
    ind[1] = ((value_) ? '*' : ' ');
    ind[2] = ')';
  }

  else if (FL_TOGGLE_BUTTON == type())
  {
    ind[0] = '[';
    ind[1] = ((value_) ? 'x' : ' ');
    ind[2] = ']';
  }

  else
  {
    ind[0] = 0;
  }

  unsigned char const* l_value = label_.value;

  if (0 == l_value)
  {
    l_value = (unsigned char const*)"";
  }

  snprintf((char*)text, sizeof(text), "%s%s", (char*)ind, (char*)label_.value);

  Fl_Label::draw(x_, y_, w_, h_, text, label_.align_,
                 l_fcolor, l_bcolor, l_fcolor);

  return;
}

bool
Fl_Button::handle(Fl_Event const event)
{
  bool newval;

  switch (event)
  {
    case FL_ENTER:
    case FL_LEAVE:
      return true;

    case FL_PUSH:
      if (Fl::visible_focus() && handle(FL_FOCUS)) Fl::focus(this);

    case FL_DRAG:
      if (Fl::event_inside(this))
      {
        if (type() == FL_RADIO_BUTTON) newval = true;

        else newval = !oldval_;
      }

      else
      {
        clear_changed();
        newval = oldval_;
      }

      if (newval != value_)
      {
        value_ = newval;
        set_changed();
        redraw();

        if (when() & FL_WHEN_CHANGED) do_callback();
      }

      return true;

    case FL_RELEASE:
      if (value_ == oldval_)
      {
        if (when() & FL_WHEN_NOT_CHANGED) do_callback();

        return true;
      }

      set_changed();

      if (type() == FL_RADIO_BUTTON) setonly();

      else if (type() == FL_TOGGLE_BUTTON) oldval_ = value_;

      else
      {
        value(oldval_);
        set_changed();

        if (when() & FL_WHEN_CHANGED)
        {
          Fl_Widget_Tracker wp(this);
          do_callback();

          if (wp.deleted()) return true;
        }
      }

      if (when() & FL_WHEN_RELEASE) do_callback();

      return true;

    case FL_SHORTCUT:
      if (!(shortcut() ?
            Fl::test_shortcut(shortcut()) : test_shortcut())) return false;

      if (Fl::visible_focus() && handle(FL_FOCUS)) Fl::focus(this);

      goto triggered_by_keyboard;

    case FL_FOCUS :
      Fl::caret_hide();

    case FL_UNFOCUS :
      if (Fl::visible_focus())
      {
        int X = x() > 0 ? x() - 1 : 0;
        int Y = y() > 0 ? y() - 1 : 0;

        if (window()) window()->damage(FL_DAMAGE_ALL, X, Y, w() + 2, h() + 2);

        return true;
      }

      return false;

    case FL_KEYBOARD :
      if (Fl::focus() == this && Fl::event_key() == ' ' &&
          !(Fl::event_state() & (FL_SHIFT | FL_CTRL | FL_ALT | FL_META)))
      {
        set_changed();
      triggered_by_keyboard:
        Fl_Widget_Tracker wp(this);

        if (type() == FL_RADIO_BUTTON)
        {
          if (!value_)
          {
            setonly();

            if (when() & FL_WHEN_CHANGED) do_callback();
          }
        }

        else if (type() == FL_TOGGLE_BUTTON)
        {
          value(!value());

          if (when() & FL_WHEN_CHANGED) do_callback();
        }

        else
        {
          simulate_key_action();
        }

        if (wp.deleted()) return true;

        if (when() & FL_WHEN_RELEASE) do_callback();

        return true;
      }

    default:
      return false;
  }
}

void
Fl_Button::simulate_key_action()
{
  if (key_release_tracker)
  {
    Fl::remove_timeout(key_release_timeout, key_release_tracker);
    key_release_timeout(key_release_tracker);
  }

  value(true);
  redraw();
  key_release_tracker = new Fl_Widget_Tracker(this);
  Fl::add_timeout(0.15, key_release_timeout, key_release_tracker);
}

void
Fl_Button::key_release_timeout(void* d)
{
  Fl_Widget_Tracker* wt = (Fl_Widget_Tracker*)d;

  if (!wt)
    return;

  if (wt == key_release_tracker)
    key_release_tracker = 0L;

  Fl_Button* btn = (Fl_Button*)wt->widget();

  if (btn)
  {
    btn->value(false);
    btn->redraw();
  }

  delete wt;
}

Fl_Button::Fl_Button(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label) :
  Fl_Widget(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label),
  value_(false),
  oldval_(false),
  shortcut_(0)
{
  set_flag(SHORTCUT_LABEL);
}

Fl_Radio_Button::Fl_Radio_Button(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* label) :
  Fl_Button(i_pos_x, i_pos_y, i_len_x, i_len_y, label)
{
  type(FL_RADIO_BUTTON);
}

Fl_Toggle_Button::Fl_Toggle_Button(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* label) :
  Fl_Button(i_pos_x, i_pos_y, i_len_x, i_len_y, label)
{
  type(FL_TOGGLE_BUTTON);
}
