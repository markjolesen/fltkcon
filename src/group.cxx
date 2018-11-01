// group.cxx
//
// Group widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2018 by Bill Spitzak and others.
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
#include <stdio.h>
#include "group.h"
#include "drvwin.h"
#include "fl.h"
#include "fl_rect.h"
#include "fl_draw.h"
#include "fl_clip.h"
#include "win.h"

Fl_Group* Fl_Group::current_;

Fl_Widget* const*
Fl_Group::array() const
{
  return children_ <= 1 ? (Fl_Widget**)(&array_) : array_;
}

int
Fl_Group::find(const Fl_Widget* o) const
{
  Fl_Widget* const* a = array();
  int i;

  for (i = 0; i < children_; i++) if (*a++ == o) break;

  return i;
}

void
Fl_Group::begin()
{
  current_ = this;
}

void
Fl_Group::end()
{
  current_ = parent();
}

Fl_Group*
Fl_Group::current()
{
  return current_;
}

void
Fl_Group::current(Fl_Group* g)
{
  current_ = g;
}

extern Fl_Widget* fl_oldfocus;

static bool
send(Fl_Widget* o, Fl_Event const event)
{
  if (!o->as_window()) return o->handle(event);

  Fl_Event dndevent = event;

  switch ( event )
  {
    case FL_DND_ENTER:
    case FL_DND_DRAG:
      dndevent = (o->contains(Fl::belowmouse())) ? FL_DND_DRAG : FL_DND_ENTER;
  }

  int save_x = Fl::e_x;
  Fl::e_x -= o->x();
  int save_y = Fl::e_y;
  Fl::e_y -= o->y();
  int ret = o->handle(event);
  Fl::e_y = save_y;
  Fl::e_x = save_x;

  switch ( dndevent )
  {
    case FL_ENTER:
    case FL_DND_ENTER:

      if (!o->contains(Fl::belowmouse())) Fl::belowmouse(o);

      break;
  }

  return ret;
}

static int
navkey()
{
  if (Fl::event_state(FL_CTRL | FL_ALT | FL_META)) return 0;

  switch (Fl::event_key())
  {
    case 0: 
      break;

    case FL_Tab:
      if (!Fl::event_state(FL_SHIFT)) return FL_Right;

      return FL_Left;

    case FL_Right:
      return FL_Right;

    case FL_Left:
      return FL_Left;

    case FL_Up:
      return FL_Up;

    case FL_Down:
      return FL_Down;
  }

  return 0;
}

bool
Fl_Group::handle(Fl_Event const event)
{

  Fl_Widget* const* a = array();
  int i;
  Fl_Widget* o;

  switch (event)
  {

    case FL_FOCUS:
      switch (navkey())
      {
        default:
          if (savedfocus_ && savedfocus_->take_focus()) return 1;

        case FL_Right:
        case FL_Down:
          for (i = children(); i--;) if ((*a++)->take_focus()) return 1;

          break;

        case FL_Left:
        case FL_Up:
          for (i = children(); i--;) if (a[i]->take_focus()) return 1;

          break;
      }

      return 0;

    case FL_UNFOCUS:
      savedfocus_ = fl_oldfocus;
      return 0;

    case FL_KEYBOARD:
      return navigation(navkey());

    case FL_SHORTCUT:
      for (i = children(); i--;)
      {
        o = a[i];

        if (o->takesevents() && Fl::event_inside(o) && send(o, FL_SHORTCUT))
          return 1;
      }

      for (i = children(); i--;)
      {
        o = a[i];

        if (o->takesevents() && !Fl::event_inside(o) && send(o, FL_SHORTCUT))
          return 1;
      }

      if ((Fl::event_key() == FL_Enter
           || Fl::event_key() == FL_KP_Enter)) return navigation(FL_Down);

      return 0;

    case FL_ENTER:
    case FL_MOVE:
      for (i = children(); i--;)
      {
        o = a[i];

        if (o->visible() && Fl::event_inside(o))
        {
          if (o->contains(Fl::belowmouse()))
          {
            return send(o, FL_MOVE);
          }

          else
          {
            Fl::belowmouse(o);

            if (send(o, FL_ENTER)) return 1;
          }
        }
      }

      Fl::belowmouse(this);
      return 1;

    case FL_DND_ENTER:
    case FL_DND_DRAG:
      for (i = children(); i--;)
      {
        o = a[i];

        if (o->takesevents() && Fl::event_inside(o))
        {
          if (o->contains(Fl::belowmouse()))
          {
            return send(o, FL_DND_DRAG);
          }

          else if (send(o, FL_DND_ENTER))
          {
            if (!o->contains(Fl::belowmouse())) Fl::belowmouse(o);

            return 1;
          }
        }
      }

      Fl::belowmouse(this);
      return 0;

    case FL_PUSH:
      for (i = children(); i--;)
      {
        o = a[i];

        if (o->takesevents() && Fl::event_inside(o))
        {
          Fl_Widget_Tracker wp(o);

          if (send(o, FL_PUSH))
          {
            if (Fl::pushed() && wp.exists() && !o->contains(Fl::pushed())) Fl::pushed(o);

            return 1;
          }
        }
      }

      return 0;

    case FL_RELEASE:
    case FL_DRAG:
      o = Fl::pushed();

      if (o == this) return 0;

      else if (o) send(o, event);

      else
      {
        for (i = children(); i--;)
        {
          o = a[i];

          if (o->takesevents() && Fl::event_inside(o))
          {
            if (send(o, event)) return 1;
          }
        }
      }

      return 0;

    case FL_MOUSEWHEEL:
      for (i = children(); i--;)
      {
        o = a[i];

        if (o->takesevents() && Fl::event_inside(o) && send(o, FL_MOUSEWHEEL))
          return 1;
      }

      for (i = children(); i--;)
      {
        o = a[i];

        if (o->takesevents() && !Fl::event_inside(o) && send(o, FL_MOUSEWHEEL))
          return 1;
      }

      return 0;

    case FL_DEACTIVATE:
    case FL_ACTIVATE:
      for (i = children(); i--;)
      {
        o = *a++;

        if (o->active()) o->handle(event);
      }

      return 1;

    case FL_SHOW:
    case FL_HIDE:
      for (i = children(); i--;)
      {
        o = *a++;

        if (event == FL_HIDE && o == Fl::focus())
        {
          Fl_Event old_event = Fl::e_number;
          o->handle(Fl::e_number = FL_UNFOCUS);
          Fl::e_number = old_event;
          Fl::focus(0);
        }

        if (o->visible()) o->handle(event);
      }

      return 1;

    default:

      for (i = 0; i < children(); i ++)
        if (Fl::focus_ == a[i]) break;

      if (i >= children()) i = 0;

      if (children())
      {
        for (int j = i;;)
        {
          if (a[j]->takesevents()) if (send(a[j], event)) return 1;

          j++;

          if (j >= children()) j = 0;

          if (j == i) break;
        }
      }

      return 0;
  }
}

//void Fl_Group::focus(Fl_Widget *o) {Fl::focus(o); o->handle(FL_FOCUS);}

#if 0
const char*
nameof(Fl_Widget* o)
{
  if (!o) return "NULL";

  if (!o->label()) return "<no label>";

  return o->label();
}
#endif

int
Fl_Group::navigation(int const key)
{
  if (children() <= 1) return 0;

  int i;

  for (i = 0; ; i++)
  {
    if (i >= children_) return 0;

    if (array_[i]->contains(Fl::focus())) break;
  }

  Fl_Widget* previous = array_[i];

  for (;;)
  {
    switch (key)
    {
      case FL_Right:
      case FL_Down:
        i++;

        if (i >= children_)
        {
          if (parent()) return 0;

          i = 0;
        }

        break;

      case FL_Left:
      case FL_Up:
        if (i) i--;

        else
        {
          if (parent()) return 0;

          i = children_ - 1;
        }

        break;

      default:
        return 0;
    }

    Fl_Widget* o = array_[i];

    if (o == previous) return 0;

    switch (key)
    {
      case FL_Down:
      case FL_Up:

        if (o->x() >= previous->x() + previous->w() ||
            o->x() + o->w() <= previous->x()) continue;
    }

    if (o->take_focus()) return 1;
  }
}

Fl_Group::Fl_Group(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label) :
  Fl_Widget(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label)
{
  align(Fl_Label::FL_ALIGN_TOP);
  children_ = 0;
  array_ = 0;
  savedfocus_ = 0;
  resizable_ = this;
  bounds_ = 0; 
  sizes_ = 0; 
  begin();
}

void
Fl_Group::clear()
{
  savedfocus_ = 0;
  resizable_ = this;
  init_sizes();

  Fl_Widget* pushed = Fl::pushed();

  if (contains(pushed)) pushed = this; 

  Fl::pushed(this);

#define REVERSE_CHILDREN
#ifdef  REVERSE_CHILDREN

  if (children_ > 1)
  {
    Fl_Widget* temp;
    Fl_Widget** a = (Fl_Widget**)array();

    for (int i = 0, j = children_ - 1; i < children_ / 2; i++, j--)
    {
      temp = a[i];
      a[i] = a[j];
      a[j] = temp;
    }
  }

#endif // REVERSE_CHILDREN

  while (children_)
  {
    int idx = children_ - 1;
    Fl_Widget* w = child(idx);

    if (w->parent() == this) 
    {
      if (children_ > 2)
      {
        w->parent_ = 0;
        children_--;
      }

      else
      {
        remove(idx);
      }

      delete w;
    }

    else
    {
      remove(idx);
    }
  }

  if (pushed != this) Fl::pushed(pushed);

}

Fl_Group::~Fl_Group()
{
  clear();
}

void
Fl_Group::insert(Fl_Widget& o, int index)
{
  if (o.parent())
  {
    Fl_Group* g = o.parent();
    int n = g->find(o);

    if (g == this)
    {
      if (index > n) index--;

      if (index == n) return;
    }

    g->remove(n);
  }

  o.parent_ = this;

  if (children_ == 0)
  {
    array_ = (Fl_Widget**)&o;
  }

  else if (children_ == 1)
  {
    Fl_Widget* t = (Fl_Widget*)array_;
    array_ = (Fl_Widget**)malloc(2 * sizeof(Fl_Widget*));

    if (index)
    {
      array_[0] = t;
      array_[1] = &o;
    }

    else
    {
      array_[0] = &o;
      array_[1] = t;
    }
  }

  else
  {
    if (!(children_ & (children_ - 1)))
      array_ = (Fl_Widget**)realloc((void*)array_,
                                    2 * children_ * sizeof(Fl_Widget*));

    int j;

    for (j = children_; j > index; j--) array_[j] = array_[j - 1];

    array_[j] = &o;
  }

  children_++;
  init_sizes();
}

void
Fl_Group::add(Fl_Widget& o)
{
  insert(o, children_);
}

void
Fl_Group::remove(unsigned int const index)
{
  if (/*index < 0 ||*/ index >= children_) return;

  Fl_Widget& o = *child(index);

  if (&o == savedfocus_) savedfocus_ = 0;

  if (o.parent_ == this)
  {
    o.parent_ = 0;
  }

  children_--;

  if (children_ == 1)
  {
    Fl_Widget* t = array_[!index];
    free((void*)array_);
    array_ = (Fl_Widget**)t;
  }

  else if (children_ > 1)
  {
    for (unsigned int slot = index; slot < children_;
         slot++) array_[slot] = array_[slot + 1];
  }

  init_sizes();
}

void
Fl_Group::remove(Fl_Widget& o)
{
  if (!children_) return;

  int i = find(o);

  if (i < children_) remove(i);
}

void
Fl_Group::init_sizes()
{
  delete[] bounds_;
  bounds_ = 0;
  delete[] sizes_;
  sizes_ = 0;
}

Fl_Rect*
Fl_Group::bounds()
{
  if (!bounds_)
  {
    Fl_Rect* p = bounds_ = new Fl_Rect[children_ + 2];

    if (as_window())
      p[0] = Fl_Rect(w(), h()); // x = y = 0

    else
      p[0] = Fl_Rect(this);

    int left   = p->x(); 
    int top    = p->y();
    int right  = p->r();
    int bottom = p->b();
    p[1] = Fl_Rect(left, top, right - left, bottom - top);
    p += 2;
    Fl_Widget* const* a = array();

    for (int i = children_; i--;)
    {
      *p++ = Fl_Rect(*a++);
    }
  }

  return bounds_;
}

void
Fl_Group::resize(
  int const X,
  int const Y,
  unsigned int const W,
  unsigned int const H)
{

  int dx = X - x();
  int dy = Y - y();
  int dw = W - w();
  int dh = H - h();

  Fl_Rect* p = bounds(); 

  Fl_Widget::resize(X, Y, W, H);

  /*
    if ((!resizable() || (dw==0 && dh==0 )) && !Fl_Window_Driver::is_a_rescale()) {

      if (!as_window()) {
        Fl_Widget*const* a = array();
        for (int i=children_; i--;) {
    Fl_Widget* o = *a++;
    o->resize(o->x() + dx, o->y() + dy, o->w(), o->h());
        }
      }

    } else */if (children_)
  {

    dx = X - p->x();
    dw = W - p->w();
    dy = Y - p->y();
    dh = H - p->h();

    if (as_window())
      dx = dy = 0;

    p++;

    int RL = p->x();
    int RR = RL + p->w();
    int RT = p->y();
    int RB = RT + p->h();
    p++;

    Fl_Widget* const* a = array();

    for (int i = children_; i--; p++)
    {

      Fl_Widget* o = *a++;
      int L = p->x();
      int R = L + p->w();
      int T = p->y();
      int B = T + p->h();

#if 0 // old widget resizing code: used up to FLTK 1.3.x, deactivated 29 Mar 2018
      // FIXME: This should be removed before the release of FLTK 1.4.0

      if (L >= RR) L += dw;

      else if (L > RL) L = RL + ((L - RL) * (RR + dw - RL) + (RR - RL) / 2) /
                             (RR - RL);

      if (R >= RR) R += dw;

      else if (R > RL) R = RL + ((R - RL) * (RR + dw - RL) + (RR - RL) / 2) /
                             (RR - RL);

      if (T >= RB) T += dh;

      else if (T > RT) T = RT + ((T - RT) * (RB + dh - RT) + (RB - RT) / 2) /
                             (RB - RT);

      if (B >= RB) B += dh;

      else if (B > RT) B = RT + ((B - RT) * (RB + dh - RT) + (RB - RT) / 2) /
                             (RB - RT);

#else // much simpler code from Francois Ostiguy: since FLTK 1.4.0

      if (L >= RR) L += dw;

      else if (L > RL) L += dw * (L - RL) / (RR - RL);

      if (R >= RR) R += dw;

      else if (R > RL) R += dw * (R - RL) / (RR - RL);

      if (T >= RB) T += dh;

      else if (T > RT) T += dh * (T - RT) / (RB - RT);

      if (B >= RB) B += dh;

      else if (B > RT) B += dh * (B - RT) / (RB - RT);

#endif // old / new (1.4.0++) widget resizing code

      o->resize(L + dx, T + dy, R - L, B - T);
    }
  }
}

void
Fl_Group::draw_children()
{
  Fl_Widget* const* a = array();

  if (clip_children())
  {
    Fl::clip_push(x(), y(), w(), h());
  }

  if (damage() & ~FL_DAMAGE_CHILD)
  {
    for (int i = children_; i--;)
    {
      Fl_Widget& o = **a++;
      draw_child(o);
      draw_outside_label(o);
    }
  }

  else
  {
    for (int i = children_; i--;) update_child(**a++);
  }

  if (clip_children()) Fl::clip_pop();
}

void
Fl_Group::draw()
{
  if (damage() & ~FL_DAMAGE_CHILD)
  {
    draw_box();
    draw_label();
  }

  draw_children();
}

void
Fl_Group::update_child(Fl_Widget& widget) const
{
  if (widget.damage() && widget.visible() && widget.type() < FL_WINDOW &&
      Fl::clip_intersects(widget.x(), widget.y(), widget.w(), widget.h()))
  {
    widget.draw();
    widget.clear_damage();
  }
}

void
Fl_Group::draw_child(Fl_Widget& widget) const
{
  if (widget.visible() && widget.type() < FL_WINDOW &&
      Fl::clip_intersects(widget.x(), widget.y(), widget.w(), widget.h()))
  {
    widget.clear_damage(FL_DAMAGE_ALL);
    widget.draw();
    widget.clear_damage();
  }
}

extern char fl_draw_shortcut;

void
Fl_Group::draw_outside_label(const Fl_Widget& widget) const
{
  if (!widget.visible()) return;

  if (!(widget.align() & 15)
      || (widget.align() & Fl_Label::FL_ALIGN_INSIDE)) return;

  Fl_Label::Fl_Align a = widget.align();
  int X = widget.x();
  int Y = widget.y();
  int W = widget.w();
  int H = widget.h();
  int wx, wy;

  if (const_cast<Fl_Group*>(this)->as_window())
  {
    wx = wy = 0;
  }

  else
  {
    wx = x();
    wy = y();
  }

  if ( (a & Fl_Label::FL_ALIGN_POSITION_MASK) == Fl_Label::FL_ALIGN_LEFT_TOP )
  {
    a = static_cast<Fl_Label::Fl_Align>((a & (~Fl_Label::FL_ALIGN_POSITION_MASK) ) |
                                        Fl_Label::FL_ALIGN_TOP_RIGHT);
    X = wx;
    W = widget.x() - X - 3;
  }

  else if ( (a & Fl_Label::FL_ALIGN_POSITION_MASK) ==
            Fl_Label::FL_ALIGN_LEFT_BOTTOM )
  {
    a = static_cast<Fl_Label::Fl_Align>((a & (~Fl_Label::FL_ALIGN_POSITION_MASK) ) |
                                        Fl_Label::FL_ALIGN_BOTTOM_RIGHT);
    X = wx;
    W = widget.x() - X - 3;
  }

  else if ( (a & Fl_Label::FL_ALIGN_POSITION_MASK) ==
            Fl_Label::FL_ALIGN_RIGHT_TOP )
  {
    a = static_cast<Fl_Label::Fl_Align>((a & (~Fl_Label::FL_ALIGN_POSITION_MASK) ) |
                                        Fl_Label::FL_ALIGN_TOP_LEFT);
    X = X + W + 3;
    W = wx + this->w() - X;
  }

  else if ( (a & Fl_Label::FL_ALIGN_POSITION_MASK) ==
            Fl_Label::FL_ALIGN_RIGHT_BOTTOM )
  {
    a = static_cast<Fl_Label::Fl_Align>((a & (~Fl_Label::FL_ALIGN_POSITION_MASK) ) |
                                        Fl_Label::FL_ALIGN_BOTTOM_LEFT);
    X = X + W + 3;
    W = wx + this->w() - X;
  }

  else if (a & Fl_Label::FL_ALIGN_TOP)
  {
    a = static_cast<Fl_Label::Fl_Align>(a ^ Fl_Label::FL_ALIGN_TOP);
    a = static_cast<Fl_Label::Fl_Align>(a | Fl_Label::FL_ALIGN_BOTTOM);
    Y = wy;
    H = widget.y() - Y;
    Y++;
  }

  else if (a & Fl_Label::FL_ALIGN_BOTTOM)
  {
    a = static_cast<Fl_Label::Fl_Align>(a ^ Fl_Label::FL_ALIGN_BOTTOM);
    a = static_cast<Fl_Label::Fl_Align>(a | Fl_Label::FL_ALIGN_TOP);
    Y = Y + H;
    H = wy + h() - Y;
    Y--;
  }

  else if (a & Fl_Label::FL_ALIGN_LEFT)
  {
    a = static_cast<Fl_Label::Fl_Align>(a ^ Fl_Label::FL_ALIGN_LEFT);
    a = static_cast<Fl_Label::Fl_Align>(a | Fl_Label::FL_ALIGN_RIGHT);
    X = wx;
    W = widget.x() - X - 3;
    X += 2;
  }

  else if (a & Fl_Label::FL_ALIGN_RIGHT)
  {
    a = static_cast<Fl_Label::Fl_Align>(a ^ Fl_Label::FL_ALIGN_RIGHT);
    a = static_cast<Fl_Label::Fl_Align>(a | Fl_Label::FL_ALIGN_LEFT);
    X = X + W + 3;
    W = wx + this->w() - X;
    X -= 2;
  }

  enum Fl::foreground l_fcolor;
  enum Fl::background l_bcolor;

  if (&widget != Fl::focus())
  {
    l_fcolor = skin_.normal_fcolor;
    l_bcolor = skin_.normal_bcolor;
  }

  else
  {
    l_fcolor = skin_.focused_fcolor;
    l_bcolor = skin_.focused_bcolor;
  }

  widget.draw_label(X, Y, W, H, a, l_fcolor, l_bcolor, l_fcolor);

}
