// inpcho.cxx
//
// An input/chooser widget.
//            ______________  ____
//           |              || __ |
//           | input area   || \/ |
//           |______________||____|
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
// Copyright 2004 by Greg Ercolano.
//
#include <string.h>
#include "inpcho.h"
#include "fl_draw.h"

Fl_Input_Choice::InputMenuButton::InputMenuButton(int x, int y, int w, int h,
                                                  unsigned char const* l)
  : Fl_Menu_Button(x, y, w, h, l)
{
}

void
Fl_Input_Choice::InputMenuButton::draw()
{

  Fl::draw_putc(x_ + w_ - 1, y_, Fl::_skin_sym.down, 1,
                skin_.normal_fcolor, skin_.normal_bcolor);

  return;
}


void
Fl_Input_Choice::menu_cb(Fl_Widget*, void* data)
{
  Fl_Input_Choice* o = (Fl_Input_Choice*)data;
  Fl_Widget_Tracker wp(o);
  const Fl_Menu_Item* item = o->menubutton()->mvalue();

  if (item && item->flags & (FL_SUBMENU | FL_SUBMENU_POINTER))
    return;

  if (!strcmp((char const*)o->inp_->value(), (char const*)o->menu_->text()))
  {
    o->Fl_Widget::clear_changed();

    if (o->when() & FL_WHEN_NOT_CHANGED)
      o->do_callback();
  }

  else
  {
    o->inp_->value(o->menu_->text());
    o->inp_->set_changed();
    o->Fl_Widget::set_changed();

    if (o->when() & (FL_WHEN_CHANGED | FL_WHEN_RELEASE))
      o->do_callback();
  }

  if (wp.deleted()) return;

  if (o->callback() != default_callback)
  {
    o->Fl_Widget::clear_changed();
    o->inp_->clear_changed();
  }
}

void
Fl_Input_Choice::inp_cb(Fl_Widget*, void* data)
{
  Fl_Input_Choice* o = (Fl_Input_Choice*)data;
  Fl_Widget_Tracker wp(o);

  if (o->inp_->changed())
  {
    o->Fl_Widget::set_changed();

    if (o->when() & (FL_WHEN_CHANGED | FL_WHEN_RELEASE))
      o->do_callback();
  }

  else
  {
    o->Fl_Widget::clear_changed();

    if (o->when() & FL_WHEN_NOT_CHANGED)
      o->do_callback();
  }

  if (wp.deleted()) return;

  if (o->callback() != default_callback)
    o->Fl_Widget::clear_changed();
}

Fl_Input_Choice::Fl_Input_Choice (
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label) :
  Fl_Group(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label)
{
  align(Fl_Label::FL_ALIGN_LEFT);
  inp_ = new Fl_Input(inp_x(), inp_y(),
                      inp_w(), inp_h());
  inp_->callback(inp_cb, (void*)this);
  inp_->when(FL_WHEN_CHANGED | FL_WHEN_NOT_CHANGED);
  menu_ = new InputMenuButton(menu_x(), menu_y(),
                              menu_w(), menu_h());
  menu_->callback(menu_cb, (void*)this);
  end();
}

void
Fl_Input_Choice::resize(int X, int Y, int W, int H)
{
  Fl_Group::resize(X, Y, W, H);
  inp_->resize(inp_x(), inp_y(), inp_w(), inp_h());
  menu_->resize(menu_x(), menu_y(), menu_w(), menu_h());
}

void
Fl_Input_Choice::value(int val)
{
  menu_->value(val);
  inp_->value(menu_->text(val));
}

void
Fl_Input_Choice::set_changed()
{
  inp_->set_changed();
}

void
Fl_Input_Choice::clear_changed()
{
  inp_->clear_changed();
  Fl_Widget::clear_changed();
}

int
Fl_Input_Choice::update_menubutton()
{
  for ( int i = 0; i < menu_->size(); i++ )
  {
    const Fl_Menu_Item& item = menu_->menu()[i];

    if (item.flags & (FL_SUBMENU | FL_SUBMENU_POINTER)) continue;

    const unsigned char* name = menu_->text(i);

    if ( name && strcmp((char const*)name, (char const*)inp_->value()) == 0)
    {
      menu_->value(i);
      return 1;
    }
  }

  return 0;
}
