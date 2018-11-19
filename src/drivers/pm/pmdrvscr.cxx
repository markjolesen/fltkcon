// pmdrvscr.cxx
//
// Protected Mode Screen Driver for the Fast Light Tool Kit (FLTK)
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
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
#include "pmdrvscr.h"
#include "pmdrvwin.h"
#include "mouse.h"
#include "keycode.h"
#include "keyboard.h"
#include "screen.h"
#include "fl_skin.h"
#include "fl_caret.h"
#include "pmwm.h"

extern void fl_fix_focus();

Fl_Screen_Driver*
Fl_Screen_Driver::newScreenDriver()
{
  return new Fl_PM_Screen_Driver();
}

Fl_PM_Screen_Driver::Fl_PM_Screen_Driver() :
  Fl_Screen_Driver(),
  wm_()
{
  prev_.m_curs_col = 0;
  prev_.m_curs_row = 0;
  prev_.m_btn_state = 0;
  screen_init();
  Fl::caret_hide();
  screen_fill(0x20, fcolor_white, bcolor_black);
  mouse_init();
  mouse_show();
  return;
}

Fl_PM_Screen_Driver::~Fl_PM_Screen_Driver()
{
  mouse_deinit();
  screen_deinit();
  return;
}

void
Fl_PM_Screen_Driver::init()
{

  num_screens = 1;

  return;
}

int
Fl_PM_Screen_Driver::x()
{
  return 0;
}

int
Fl_PM_Screen_Driver::y()
{
  return 0;
}

int
Fl_PM_Screen_Driver::w()
{
  return _video_cols;
}

int
Fl_PM_Screen_Driver::h()
{
  return _video_rows;
}

void
Fl_PM_Screen_Driver::screen_xywh(int& X, int& Y, int& W, int& H, int /*n*/)
{
  X = 0;
  Y = 0;
  W = _video_cols;
  H = _video_rows;
  return;
}

void
Fl_PM_Screen_Driver::screen_work_area(int& X, int& Y, int& W, int& H,
                                      int /*n*/)
{
  X = 0;
  Y = 0;
  W = _video_cols;
  H = _video_rows;
  return;
}

void
Fl_PM_Screen_Driver::beep(int type)
{
}

void
Fl_PM_Screen_Driver::flush()
{
}

void
Fl_PM_Screen_Driver::event_key(
  Fl_Window& window,
  unsigned char const scan,
  unsigned char const ascii) const
{
  unsigned char buf[2];
  int sym = 0;
  Fl_Window* w = &window;

  char kb0 = *(char*)0x0417;
  char kb1 = *(char*)0x0418;

  if (0x3 & kb0)
  {
    Fl::e_state |= FL_SHIFT;
  }

  if (0x4 & kb0)
  {
    Fl::e_state |= FL_CTRL;
  }

  if (0x8 & kb0)
  {
    Fl::e_state |= FL_ALT;
  }

  if (0x1 & kb1)
  {
    Fl::e_state |= FL_CTRL;
  }

  if (0x2 & kb1)
  {
    Fl::e_state |= FL_ALT;
  }

  buf[0] = 0;
  buf[1] = 0;
  Fl::e_text = buf;
  Fl::e_length = 0;

  if (ascii)
  {
    switch (ascii)
    {
      case ASCII_BS:
        sym = FL_BackSpace;
        break;

      case ASCII_TAB:
        sym = FL_Tab;
        break;

      case ASCII_ENTER:
        sym = FL_Enter;
        break;

      case ASCII_ESC:
        sym = FL_Escape;

        while (w->parent())
        {
          w = w->window();
        }

        break;

      default:
        sym = ascii;
        break;
    }
  }

  else
  {
    switch (scan)
    {
#if 0

      case SCAN_F1:
      case SCAN_F2:
      case SCAN_F3:
      case SCAN_F4:
      case SCAN_F5:
      case SCAN_F6:
      case SCAN_F7:
      case SCAN_F8:
      case SCAN_F9:
      case SCAN_F10:
#endif
      case SCAN_HOME:
        sym = FL_Home;
        break;

      case SCAN_UP:
        sym = FL_Up;
        break;

      case SCAN_PGUP:
        sym = FL_Page_Up;
        break;

      case SCAN_LEFT:
        sym = FL_Left;
        break;

      case SCAN_RIGHT:
        sym = FL_Right;
        break;

      case SCAN_END:
        sym = FL_End;
        break;

      case SCAN_DOWN:
        sym = FL_Down;
        break;

      case SCAN_PGDN:
        sym = FL_Page_Down;
        break;

      case SCAN_INS:
        sym = FL_Insert;
        Fl::_caret_mode = static_cast<enum Fl::caret_mode>(!static_cast<int>
                                                           (Fl::_caret_mode));

        if (Fl::_caret_is_visible)
        {
          if (Fl::CARET_OVERWRITE == Fl::_caret_mode)
          {
            Fl::caret_block();
          }

          else
          {
            Fl::caret_underline();
          }
        }

        else
        {
          Fl::caret_hide();
        }

        break;

      case SCAN_DEL:
        sym = FL_Delete;
        break;

      case SCAN_SHIFT_TAB:
        sym = FL_Tab;
        Fl::e_state |= FL_SHIFT;
        break;
#if 0

      case SCAN_SHIFT_F1:
      case SCAN_SHIFT_F2:
      case SCAN_SHIFT_F3:
      case SCAN_SHIFT_F4:
      case SCAN_SHIFT_F5:
      case SCAN_SHIFT_F6:
      case SCAN_SHIFT_F7:
      case SCAN_SHIFT_F8:
      case SCAN_SHIFT_F9:
      case SCAN_SHIFT_F10:
#endif
      case SCAN_ALT_1:
        sym = '1';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_2:
        sym = '2';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_3:
        sym = '3';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_4:
        sym = '4';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_5:
        sym = '5';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_6:
        sym = '6';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_7:
        sym = '7';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_8:
        sym = '8';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_9:
        sym = '9';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_0:
        sym = '0';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_MINUS:
        sym = '-';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_PLUS:
        sym = '+';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_Q:
        sym = 'q';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_W:
        sym = 'w';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_E:
        sym = 'e';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_R:
        sym = 'r';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_T:
        sym = 't';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_Y:
        sym = 'y';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_U:
        sym = 'u';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_I:
        sym = 'i';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_O:
        sym = 'o';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_P:
        sym = 'p';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_A:
        sym = 'a';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_S:
        sym = 's';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_D:
        sym = 'd';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_F:
        sym = 'f';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_G:
        sym = 'g';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_H:
        sym = 'h';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_J:
        sym = 'j';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_K:
        sym = 'k';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_L:
        sym = 'l';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_Z:
        sym = 'z';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_X:
        sym = 'x';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_C:
        sym = 'c';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_V:
        sym = 'v';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_B:
        sym = 'r';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_N:
        sym = 'n';
        Fl::e_state |= FL_ALT;
        break;

      case SCAN_ALT_M:
        sym = 'm';
        Fl::e_state |= FL_ALT;
        break;
#if 0

      case SCAN_ALT_F1:
      case SCAN_ALT_F2:
      case SCAN_ALT_F3:
      case SCAN_ALT_F4:
      case SCAN_ALT_F5:
      case SCAN_ALT_F6:
      case SCAN_ALT_F7:
      case SCAN_ALT_F8:
      case SCAN_ALT_F9:
      case SCAN_ALT_F10:
      case SCAN_CTL_F1:
      case SCAN_CTL_F2:
      case SCAN_CTL_F3:
      case SCAN_CTL_F4:
      case SCAN_CTL_F5:
      case SCAN_CTL_F6:
      case SCAN_CTL_F7:
      case SCAN_CTL_F8:
      case SCAN_CTL_F9:
      case SCAN_CTL_F10:
#endif
      case SCAN_CTL_HOME:
        sym = FL_Home;
        Fl::e_state |= FL_CTRL;
        break;

      case SCAN_CTL_PGUP:
        sym = FL_Page_Up;
        Fl::e_state |= FL_CTRL;
        break;

      case SCAN_CTL_LEFT:
        sym = FL_Left;
        Fl::e_state |= FL_CTRL;
        break;

      case SCAN_CTL_RIGHT:
        sym = FL_Right;
        Fl::e_state |= FL_CTRL;
        break;

      case SCAN_CTL_END:
        sym = FL_End;
        Fl::e_state |= FL_CTRL;
        break;

      case SCAN_CTL_PGDN:
        sym = FL_Page_Down;
        Fl::e_state |= FL_CTRL;
        break;
    }
  }

  if (0x20 <= sym && 0x7f > sym)
  {
    buf[0] = sym;
    Fl::e_length = 1;
  }

  Fl::e_keysym = sym;
  Fl::e_number = FL_KEYBOARD;
  Fl::handle(FL_KEYBOARD, w);

  return;
}

void
Fl_PM_Screen_Driver::event_mouse(
  Fl_Window& window,
  struct mouse_event const& mouse)
{

  Fl::e_x_root = mouse.m_curs_col;
  Fl::e_y_root = mouse.m_curs_row;
  Fl::e_x = mouse.m_curs_col - window.x();
  Fl::e_y = mouse.m_curs_row - window.y();

  int movement = (mouse.m_curs_col - prev_.m_curs_col) +
                 (mouse.m_curs_row - prev_.m_curs_row);

  if (1 & mouse.m_btn_state)
  {
    Fl::e_state |= FL_BUTTON1;
    Fl::e_keysym = (FL_Button | FL_LEFT_MOUSE);
  }

  if (2 & mouse.m_btn_state)
  {
    Fl::e_state |= FL_BUTTON2;
    Fl::e_keysym = (FL_Button | FL_RIGHT_MOUSE);
  }

  do
  {

    if (3 & mouse.m_btn_state)
    {
      if (prev_.m_btn_state)
      {
        if (movement)
        {
          Fl::handle(FL_DRAG, &window);
        }
      }

      else
      {
        if (Fl::e_is_click == Fl::e_keysym)
        {
          Fl::e_clicks++;
        }

        else
        {
          Fl::e_clicks = 0;
          Fl::e_is_click = Fl::e_keysym;
        }

        Fl::handle(FL_PUSH, &window);
      }

      break;
    }

    if (movement)
    {
      Fl::handle(FL_MOVE, &window);
      break;
    }

  }
  while (0);

  return;
}

int
Fl_PM_Screen_Driver::wait_mouse(Fl_Window& window)
{
  struct mouse_event mouse;
  int rc;

  do
  {

    rc = mouse_get_event(&mouse);

    if (0 == rc)
    {
      break;
    }

    mouse.m_curs_col = (mouse.m_curs_col >> 3);
    mouse.m_curs_row = (mouse.m_curs_row >> 3);

    if (0 == prev_.m_btn_state)
    {
      if (mouse.m_btn_state &&
          (FL_WINDOW == window.type() ||
           FL_DOUBLE_WINDOW == window.type()))
      {
        wm::hit_type what = wm_.hit(window, mouse.m_curs_col, mouse.m_curs_row);

        if (wm::HIT_NONE != what && wm::HIT_WINDOW != what)
        {
          wm_.handle_mouse(window, what, mouse.m_curs_col, mouse.m_curs_row);
          break;
        }
      }
    }

    event_mouse(window, mouse);

    if (prev_.m_btn_state)
    {
      if (1 & prev_.m_btn_state)
      {
        Fl::e_state |= FL_BUTTON1;
        Fl::e_keysym = (FL_Button | FL_LEFT_MOUSE);
      }

      if (2 & prev_.m_btn_state)
      {
        Fl::e_state |= FL_BUTTON2;
        Fl::e_keysym = (FL_Button | FL_RIGHT_MOUSE);
      }

      Fl::handle(FL_RELEASE, &window);
    }

    prev_.m_curs_col = mouse.m_curs_col;
    prev_.m_curs_row = mouse.m_curs_row;
    prev_.m_btn_state = mouse.m_btn_state;

  }
  while (0);

  return rc;
}

double
Fl_PM_Screen_Driver::wait(double time_to_wait)
{
  int rc;
  char buf[2];

  do
  {

    Fl::run_checks();

    static int in_idle = 0;

    if (Fl::idle && !in_idle)
    {
      in_idle = 1;
      Fl::idle();
      in_idle = 0;
    }

    Fl::flush();
    Fl::e_state = 0;

    Fl_Window* window = 0;

    if (0 == Fl::grab_)
    {
      if (0 == Fl::focus_)
      {
        window = Fl::first_window();
      }

      else
      {
        window = Fl::focus_->as_window();

        if (0 == window)
        {
          window = Fl::focus_->window();
        }
      }
    }

    else
    {
      window = Fl::grab_;
    }

    if (0 == window)
    {
      break;
    }

    if (0 == Fl::focus_)
    {
      window->take_focus();
    }

    rc = keyboard_query();

    if (rc)
    {
      unsigned char ascii;
      unsigned char scan;
      keyboard_read(&scan, &ascii);
      event_key((*window), scan, ascii);
      break;
    }

    rc = wait_mouse((*window));

    if (rc)
    {
      break;
    }

  }
  while (0);

  return 1;
}

int
Fl_PM_Screen_Driver::ready()
{
  return 1;
}

void
Fl_PM_Screen_Driver::grab(Fl_Window* win)
{
  if (win)
  {
    Fl::grab_ = win;
  }

  else
  {
    Fl::grab_ = 0;
    fl_fix_focus();
  }

  return;
}

void
Fl_PM_Screen_Driver::add_timeout(double time, Fl_Timeout_Handler cb,
                                 void* argp)
{
}

void
Fl_PM_Screen_Driver::repeat_timeout(double time, Fl_Timeout_Handler cb,
                                    void* argp)
{
}

int
Fl_PM_Screen_Driver::has_timeout(Fl_Timeout_Handler cb, void* argp)
{
  return 0;
}

void
Fl_PM_Screen_Driver::remove_timeout(Fl_Timeout_Handler cb, void* argp)
{
}

int
Fl_PM_Screen_Driver::compose(int& del)
{
  int condition;
  unsigned char ascii = (unsigned char)Fl::e_text[0];
  condition = (Fl::e_state & (FL_ALT | FL_META | FL_CTRL)) && !(ascii & 128) ;

  if (condition)
  {
    del = 0;    // this stuff is to be treated as a function key
    return 0;
  }

  del = Fl::compose_state;
  Fl::compose_state = 0;

  // Only insert non-control characters:
  if ((!Fl::compose_state) && !(ascii & ~31 && ascii != 127))
  {
    return 0;
  }

  return 1;
}
