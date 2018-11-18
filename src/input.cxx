// input.cxx
//
// Input widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2016, 2018 by Bill Spitzak and others.
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

// This is the "user interface", it decodes user actions into what to
// do to the text.  See also Fl_Input_.cxx, where the text is actually
// manipulated (and some ui, in particular the mouse, is done...).
// In theory you can replace this code with another subclass to change
// the keybindings.

#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "drvsys.h"
#include "drvscr.h"
#include "fl.h"
#include "fl_ask.h"
#include "fl_caret.h"
#include "fl_draw.h"
#include "flstring.h"
#include "inpfloat.h"
#include "inphid.h"
#include "inpint.h"
#include "inpmulti.h"
#include "outmulti.h"
#include "output.h"
#include "platform.h"
#include "win.h"

Fl_Input::Fl_Input(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  const unsigned char* i_label) :
  Fl_Input_(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label)
{
  return;
}

void
Fl_Input::draw()
{

  if (Fl_Input_::FL_HIDDEN_INPUT != input_type())
  {
    Fl_Input_::drawtext(x_, y_, w_, h_);
  }

  return;
}

void
Fl_Input::handle_keyboard_compose(
  int const i_del)
{
  unsigned int l_damage = Fl_Widget::FL_DAMAGE_NONE;

  do
  {

    if (Fl_Input_::FL_FLOAT_INPUT == input_type() ||
        Fl_Input_::FL_INT_INPUT == input_type())
    {
      Fl::compose_reset();

      static char const* legal_fp_chars = ".eE+-";
      unsigned char l_ascii = Fl::event_text()[0];
      unsigned int l_ip = 0;
      unsigned char l_chr0 = 0;
      unsigned char l_chr1 = 0;

      if ((*cur_).m_active->m_length)
      {
        l_ip = (*cur_).m_slot_x;
        l_chr0 = (*cur_).m_active->m_data[0];

        if (1 < (*cur_).m_active->m_length)
        {
          l_chr1 = (*cur_).m_active->m_data[1];
        }
      }

      if (   (!l_ip && (l_ascii == '+' || l_ascii == '-'))
             || (l_ascii >= '0' && l_ascii <= '9')
             || (l_ip == 1 && l_chr0 == '0' && (l_ascii == 'x' || l_ascii == 'X'))
             || (l_ip > 1 && l_chr0 == '0' && (l_chr1 == 'x' || l_chr1 == 'X')
                 && ((l_ascii >= 'A' && l_ascii <= 'F') || (l_ascii >= 'a' && l_ascii <= 'f')))
             || (input_type() == Fl_Input_::FL_FLOAT_INPUT && l_ascii
                 && strchr(legal_fp_chars, l_ascii)))
      {
        if (readonly())
        {
          fl_beep();
        }

        else
        {
          if (Fl::CARET_OVERWRITE == Fl::_caret_mode)
          {
            l_damage = ovw(&l_ascii, 1);
          }

          else
          {
            l_damage = ins(&l_ascii, 1);
          }
        }
      }

      break;
    }

    if (i_del || Fl::event_length())
    {
      if (readonly())
      {
        fl_beep();
      }

      else
      {
        if (Fl::CARET_OVERWRITE == Fl::_caret_mode)
        {
          l_damage = ovw(Fl::event_text(), Fl::event_length());
        }

        else
        {
          l_damage = ins(Fl::event_text(), Fl::event_length());
        }
      }
    }

  }
  while (0);

  damage(l_damage);

  return;
}

bool
Fl_Input::handle_keyboard()
{
  bool l_handled = true;

  do
  {
    int l_del;
    bool l_compose = Fl::compose(l_del);

    if (l_compose)
    {
      handle_keyboard_compose(l_del);
      break;
    }

    unsigned int mods = Fl::event_state() & (FL_META | FL_CTRL | FL_ALT);
    unsigned int shift = Fl::event_state() & FL_SHIFT;
    unsigned int multiline = (input_type() == FL_MULTILINE_INPUT) ? 1 : 0;

    if (FL_Enter == Fl::event_key() &&
        multiline &&
        Fl_Input_::FL_FLOAT_INPUT != input_type() &&
        Fl_Input_::FL_INT_INPUT != input_type())
    {
      nl();
      break;
    }

    l_handled = Fl::screen_driver()->input_widget_handle_key(
                  Fl::event_key(), mods, shift, this);

  }
  while (0);

  return l_handled;
}

bool
Fl_Input::handle(
  enum Fl_Event const i_event)
{
  bool l_handled = false;

  switch (i_event)
  {
    case FL_KEYBOARD:
      l_handled = handle_keyboard();
      break;
  }

  if (false == l_handled)
  {
    l_handled = Fl_Input_::handletext(i_event, x_, y_, w_, h_);
  }

  return l_handled;
}

enum Fl_Widget::Fl_Damage
Fl_Input::home()
{
  enum Fl_Widget::Fl_Damage l_damage =
    static_cast<Fl_Widget::Fl_Damage>(inpcur_home(cur_));
  damage(l_damage);
  return l_damage;
}

enum Fl_Widget::Fl_Damage
Fl_Input::end()
{
  enum Fl_Widget::Fl_Damage l_damage =
    static_cast<Fl_Widget::Fl_Damage>(inpcur_end(cur_));
  damage(l_damage);
  return l_damage;
}

enum Fl_Widget::Fl_Damage
Fl_Input::left()
{
  enum Fl_Widget::Fl_Damage l_damage =
    static_cast<Fl_Widget::Fl_Damage>(inpcur_left(cur_));
  damage(l_damage);
  return l_damage;
}

enum Fl_Widget::Fl_Damage
Fl_Input::right()
{
  enum Fl_Widget::Fl_Damage l_damage =
    static_cast<Fl_Widget::Fl_Damage>(inpcur_right(cur_));
  damage(l_damage);
  return l_damage;
}

enum Fl_Widget::Fl_Damage
Fl_Input::up()
{
  enum Fl_Widget::Fl_Damage l_damage =
    static_cast<Fl_Widget::Fl_Damage>(inpcur_up(cur_));
  damage(l_damage);
  return l_damage;
}

enum Fl_Widget::Fl_Damage
Fl_Input::down()
{
  enum Fl_Widget::Fl_Damage l_damage =
    static_cast<Fl_Widget::Fl_Damage>(inpcur_down(cur_));
  damage(l_damage);
  return l_damage;
}

enum Fl_Widget::Fl_Damage
Fl_Input::ins(
  unsigned char const* i_chr,
  size_t const i_size)
{
  unsigned int l_damage = Fl_Widget::FL_DAMAGE_NONE;

  for (size_t l_slot = 0; i_size > l_slot; l_slot++)
  {
    l_damage |= static_cast<Fl_Widget::Fl_Damage>(inpcur_ins(cur_, i_chr[l_slot]));
  }

  damage(l_damage);
  return static_cast<Fl_Widget::Fl_Damage>(l_damage);
}

enum Fl_Widget::Fl_Damage
Fl_Input::ovw(
  unsigned char const* i_chr,
  size_t const i_size)
{
  unsigned int l_damage = Fl_Widget::FL_DAMAGE_NONE;

  for (size_t l_slot = 0; i_size > l_slot; l_slot++)
  {
    l_damage |= static_cast<Fl_Widget::Fl_Damage>(inpcur_ovw(cur_, i_chr[l_slot]));
  }

  damage(l_damage);
  return static_cast<Fl_Widget::Fl_Damage>(l_damage);
}

enum Fl_Widget::Fl_Damage
Fl_Input::del()
{
  enum Fl_Widget::Fl_Damage l_damage =
    static_cast<Fl_Widget::Fl_Damage>(inpcur_del(cur_));
  damage(l_damage);
  return l_damage;
}

enum Fl_Widget::Fl_Damage
Fl_Input::bs()
{
  unsigned int l_damage = left();

  if (l_damage)
  {
    l_damage |= del();
    damage(l_damage);
  }

  return static_cast<Fl_Widget::Fl_Damage>(l_damage);
}

enum Fl_Widget::Fl_Damage
Fl_Input::nl()
{
  unsigned int l_damage = Fl_Widget::FL_DAMAGE_NONE;
  size_t l_slot = inplist_slot((*cur_).m_list, (*cur_).m_active);
  struct inpbuf* l_buf = inplist_insert((*cur_).m_list, (1 + l_slot));
  unsigned int l_len = (*cur_).m_active->m_length - (*cur_).m_slot_x;

  if (l_len)
  {
    unsigned char const* l_ptr = (*cur_).m_active->m_data + (*cur_).m_slot_x;
    l_damage |= inpbuf_grow(l_buf, l_len);
    (*l_buf).m_length = l_len;
    memcpy((*l_buf).m_data, l_ptr, l_len);
    (*cur_).m_active->m_length = (*cur_).m_slot_x;
  }

  (*cur_).m_caret_x = 0;
  (*cur_).m_slot_x = 0;
  (*cur_).m_sticky_x = 0;
  (*cur_).m_offset_x = 0;
  l_damage |= (Fl_Widget::FL_DAMAGE_USER1 | down());

  damage(l_damage);

  return static_cast<Fl_Widget::Fl_Damage>(l_damage);
}
