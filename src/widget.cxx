// widget.cxx
//
// Base widget class for the Fast Light Tool Kit (FLTK).
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
#include <stdlib.h>
#include "widget.h"
#include "fl.h"
#include "fl_draw.h"
#include "flstring.h"
#include "group.h"
#include "win.h"

const int QUEUE_SIZE = 20;

static Fl_Widget* obj_queue[QUEUE_SIZE];
static int obj_head, obj_tail;

void
Fl_Widget::default_callback(Fl_Widget* widget, void* /*v*/)
{
  obj_queue[obj_head++] = widget;

  if (obj_head >= QUEUE_SIZE) obj_head = 0;

  if (obj_head == obj_tail)
  {
    obj_tail++;

    if (obj_tail >= QUEUE_SIZE) obj_tail = 0;
  }
}

Fl_Widget*
Fl::readqueue()
{
  if (obj_tail == obj_head) return 0;

  Fl_Widget* widget = obj_queue[obj_tail++];

  if (obj_tail >= QUEUE_SIZE) obj_tail = 0;

  return widget;
}

static void
cleanup_readqueue(Fl_Widget* w)
{

  if (obj_tail == obj_head) return;

  int old_head = obj_head;
  int entry = obj_tail;
  obj_head = obj_tail;

  for (;;)
  {
    Fl_Widget* o = obj_queue[entry++];

    if (entry >= QUEUE_SIZE) entry = 0;

    if (o != w)
    {
      obj_queue[obj_head++] = o;

      if (obj_head >= QUEUE_SIZE) obj_head = 0;
    }

    if (entry == old_head) break;
  }

  return;
}

bool
Fl_Widget::handle(enum Fl_Event const)
{
  return false;
}

Fl_Widget::Fl_Widget(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label)
{

  skin_ = Fl::_skin_widget;

  x_ = i_pos_x;
  y_ = i_pos_y;
  w_ = i_len_x;
  h_ = i_len_y;

  label_.value   = i_label;
  label_.type  = Fl_Label::FL_NORMAL_LABEL;
  label_.align_  = Fl_Label::FL_ALIGN_CENTER;
  callback_  = default_callback;
  user_data_   = 0;
  type_    = 0;
  flags_   = VISIBLE_FOCUS;
  damage_  = FL_DAMAGE_NONE;
  when_    = FL_WHEN_RELEASE;
  box_ = false;

  parent_ = 0;

  if (Fl_Group::current()) Fl_Group::current()->add(this);

  return;
}

void
Fl_Widget::resize(
  int const x,
  int const y,
  unsigned int const w,
  unsigned int const h)
{
  x_ = x;
  y_ = y;
  w_ = w;
  h_ = h;
}

int
Fl_Widget::damage_resize(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y)
{
  if (x() == i_pos_x && y() == i_pos_y && w() == i_len_x
      && h() == i_len_y) return 0;

  resize(i_pos_x, i_pos_y, i_len_x, i_len_y);
  redraw();
  return 1;
}

int
Fl_Widget::take_focus()
{
  if (!takesevents()) return 0;

  if (!visible_focus()) return 0;

  if (!handle(FL_FOCUS)) return 0;

  if (contains(Fl::focus())) return 1;

  Fl::focus(this);
  return 1;
}

extern void fl_throw_focus(Fl_Widget*);

Fl_Widget::~Fl_Widget()
{
  Fl::clear_widget_pointer(this);

  if (flags() & COPIED_LABEL) free((void*)(label_.value));

  if (parent_) parent_->remove(this);

  parent_ = 0; 
  fl_throw_focus(this);

  if (callback_ == default_callback) cleanup_readqueue(this);
}

void
Fl_Widget::activate()
{
  if (!active())
  {
    clear_flag(INACTIVE);

    if (active_r())
    {
      redraw();
      redraw_label();
      handle(FL_ACTIVATE);

      if (inside(Fl::focus())) Fl::focus()->take_focus();
    }
  }
}

void
Fl_Widget::deactivate()
{
  if (active_r())
  {
    set_flag(INACTIVE);
    redraw();
    redraw_label();
    handle(FL_DEACTIVATE);
    fl_throw_focus(this);
  }

  else
  {
    set_flag(INACTIVE);
  }
}

bool
Fl_Widget::active_r() const
{
  for (const Fl_Widget* o = this; o; o = o->parent())
    if (!o->active()) return false;

  return true;
}

void
Fl_Widget::show()
{
  if (!visible())
  {
    clear_flag(INVISIBLE);

    if (visible_r())
    {
      redraw();
      redraw_label();
      handle(FL_SHOW);

      if (inside(Fl::focus())) Fl::focus()->take_focus();
    }
  }
}

void
Fl_Widget::hide()
{
  if (visible_r())
  {
    set_flag(INVISIBLE);

    for (Fl_Widget* p = parent(); p; p = p->parent())
      if (!p->parent())
      {
        p->redraw();
        break;
      }

    handle(FL_HIDE);
    fl_throw_focus(this);
  }

  else
  {
    set_flag(INVISIBLE);
  }
}

int
Fl_Widget::visible_r() const
{
  for (const Fl_Widget* o = this; o; o = o->parent())
    if (!o->visible()) return 0;

  return 1;
}

int
Fl_Widget::contains(const Fl_Widget* o) const
{
  for (; o; o = o->parent_) if (o == this) return 1;

  return 0;
}


void
Fl_Widget::label(const unsigned char* a)
{
  if (flags() & COPIED_LABEL)
  {
    if (label_.value == a)
      return;

    free((void*)(label_.value));
    clear_flag(COPIED_LABEL);
  }

  label_.value = a;
  redraw_label();
}


void
Fl_Widget::copy_label(const unsigned char* a)
{
  if ((flags() & COPIED_LABEL) && (label_.value == a))
    return;

  if (a)
  {
    label((unsigned char*)strdup((char const*)a));
    set_flag(COPIED_LABEL);
  }

  else
  {
    label(0);
  }
}

void
Fl_Widget::do_callback(Fl_Widget* widget, void* arg)
{
  if (!callback_) return;

  Fl_Widget_Tracker wp(this);
  callback_(widget, arg);

  if (wp.deleted()) return;

  if (callback_ != default_callback)
    clear_changed();
}

void
Fl_Widget::draw_box() const
{

  draw_box(x_, y_, w_, h_, skin_.box_fcolor, skin_.box_bcolor);

  return;
}

void
Fl_Widget::draw_box(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  enum Fl::foreground const i_fcolor,
  enum Fl::background const i_bcolor) const
{

  if (box_)
  {
    Fl::draw_frame(i_pos_x, i_pos_y, i_len_x, i_len_y, i_fcolor, i_bcolor);
  }

  return;
}
