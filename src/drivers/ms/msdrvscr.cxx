// msdrvscr.cxx
//
// MS Screen Driver for the Fast Light Tool Kit (FLTK)
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
#include <string.h>
#include "msdrvscr.h"
#include "msdrvwin.h"
#include "keycode.h"
#include "screen.h"
#include "fl_skin.h"
#include "fl_caret.h"
#include "mswm.h"

extern "C" HANDLE _screen_stdin;

extern void fl_fix_focus();

Fl_Screen_Driver*
Fl_Screen_Driver::newScreenDriver()
{
  return new Fl_MS_Screen_Driver();
}

Fl_MS_Screen_Driver::Fl_MS_Screen_Driver() :
  Fl_Screen_Driver(),
  state_(),
  wm_()
{
  int rc;

  state_.btn = 0;

  Fl::_skin_sym.ret = '+';
  Fl::_skin_sym.right = '>';
  Fl::_skin_sym.left = '<';

  screen_init();
  screen_fill(0x20, fcolor_white, bcolor_black);

  return;
}

Fl_MS_Screen_Driver::~Fl_MS_Screen_Driver()
{
  screen_deinit();
  return;
}

void
Fl_MS_Screen_Driver::init()
{

  num_screens = 1;

  return;
}

int
Fl_MS_Screen_Driver::x()
{
  return 0;
}

int
Fl_MS_Screen_Driver::y()
{
  return 0;
}

int
Fl_MS_Screen_Driver::w()
{
  return _video_cols;
}

int
Fl_MS_Screen_Driver::h()
{
  return _video_rows;
}

void
Fl_MS_Screen_Driver::screen_xywh(int& X, int& Y, int& W, int& H, int /*n*/)
{
  X = 0;
  Y = 0;
  W = _video_cols;
  H = _video_rows;
  return;
}

void
Fl_MS_Screen_Driver::screen_work_area(int& X, int& Y, int& W, int& H,
                                      int /*n*/)
{
  X = 0;
  Y = 0;
  W = _video_cols;
  H = _video_rows;
  return;
}

void
Fl_MS_Screen_Driver::beep(int type)
{
}

void
Fl_MS_Screen_Driver::flush()
{
}

void
Fl_MS_Screen_Driver::event_key(Fl_Window& window, KEY_EVENT_RECORD& event)
{
  unsigned char buf[2];
  int sym = 0;
  Fl_Window* w = &window;

  buf[0] = 0;
  buf[1] = 0;
  Fl::e_length = 0;

  if ((LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED) & event.dwControlKeyState)
  {
    Fl::e_state |= FL_ALT;
  }

  if ((LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED) & event.dwControlKeyState)
  {
    Fl::e_state |= FL_CTRL;
  }

  if (SHIFT_PRESSED & event.dwControlKeyState)
  {
    Fl::e_state |= FL_SHIFT;
  }

  switch (event.wVirtualKeyCode)
  {
    case VK_BACK:
      sym = FL_BackSpace;
      break;

    case VK_DELETE:
      sym = FL_Delete;
      break;

    case VK_DOWN:
      sym = FL_Down;
      break;

    case VK_END:
      sym = FL_End;
      break;

    case VK_ESCAPE:
      sym = FL_Escape;

      while (w->parent())
      {
        w = w->window();
      }

      break;

    case VK_HOME:
      sym = FL_Home;
      break;

    case VK_INSERT:
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

    case VK_LEFT:
      sym = FL_Left;
      break;

    case VK_NEXT:
      sym = FL_Page_Down;
      break;

    case VK_PRIOR:
      sym = FL_Page_Up;
      break;

    case VK_RETURN:
      sym = FL_Enter;
      break;

    case VK_RIGHT:
      sym = FL_Right;
      break;

    case VK_TAB:
      sym = FL_Tab;
      break;

    case VK_UP:
      sym = FL_Up;
      break;

    default:
      sym = event.uChar.AsciiChar;

      if (0x20 <= sym && 0x7f > sym)
      {
        buf[0] = sym;
        Fl::e_length = 1;
      }

      break;
  }

  if (sym)
  {
    Fl::e_text = buf;
    Fl::e_keysym = sym;
    Fl::e_number = FL_KEYBOARD;
    Fl::handle(FL_KEYBOARD, w);
  }

  return;
}

void
Fl_MS_Screen_Driver::event_mouse(Fl_Window& window, MOUSE_EVENT_RECORD& event)
{

  Fl::e_is_click = 0;
  Fl::e_clicks = 0;
  Fl::e_x_root = event.dwMousePosition.X;
  Fl::e_y_root = event.dwMousePosition.Y;
  Fl::e_x = event.dwMousePosition.X - window.x();
  Fl::e_y = event.dwMousePosition.Y - window.y();

  do
  {

    if ((0 == event.dwButtonState) && state_.btn)
    {
      Fl::handle(FL_RELEASE, &window);
      break;
    }

    if (FROM_LEFT_1ST_BUTTON_PRESSED & event.dwButtonState)
    {
      Fl::e_state |= FL_BUTTON1;
      Fl::e_keysym = (FL_Button | FL_LEFT_MOUSE);
      Fl::e_clicks = 1;
    }

    if (RIGHTMOST_BUTTON_PRESSED & event.dwButtonState)
    {
      Fl::e_state |= FL_BUTTON2;
      Fl::e_keysym = (FL_Button | FL_RIGHT_MOUSE);
      Fl::e_clicks = 1;
    }

    if (DOUBLE_CLICK & event.dwEventFlags)
    {
      Fl::e_clicks = 2;
    }

    if (Fl::e_clicks)
    {
      if ((0 == state_.btn) &&
          (FL_WINDOW == window.type() || FL_DOUBLE_WINDOW == window.type()))
      {
        wm::hit_type what = wm_.hit(window, event.dwMousePosition.X,
                                    event.dwMousePosition.Y);

        if (wm::HIT_NONE != what && wm::HIT_WINDOW != what)
        {
          wm_.handle_mouse(window, what, event.dwMousePosition.X,
                           event.dwMousePosition.Y);
          break;
        }
      }

      Fl::e_is_click = 1;
      Fl::handle(FL_PUSH, &window);
      break;
    }

    if (0 == (MOUSE_MOVED & event.dwEventFlags))
    {
      break;
    }

    if (state_.btn)
    {
      Fl::handle(FL_DRAG, &window);
      break;
    }

    Fl::handle(FL_MOVE, &window);

  }
  while (0);

  state_.btn = event.dwButtonState;

  return;
}

void
Fl_MS_Screen_Driver::poll(Fl_Window& window)
{
  int rc;
  INPUT_RECORD record;
  DWORD count;

  Fl::e_length = 1;
  Fl::e_state = 0;

  rc = ReadConsoleInputA(_screen_stdin, &record, 1, &count);

  if (rc && count)
  {
    switch (record.EventType)
    {
      case KEY_EVENT:
        if (record.Event.KeyEvent.bKeyDown)
        {
          for (unsigned int count = 0; record.Event.KeyEvent.wRepeatCount > count;
               count++)
          {
            event_key(window, record.Event.KeyEvent);
          }
        }

        break;

      case MOUSE_EVENT:
        event_mouse(window, record.Event.MouseEvent);
        break;

      case WINDOW_BUFFER_SIZE_EVENT:
        break;

      default:
        break;
    }
  }

  return;
}

double
Fl_MS_Screen_Driver::wait(double time_to_wait)
{

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

    poll(*window);

  }
  while (0);

  return 1;
}

int
Fl_MS_Screen_Driver::ready()
{
  return 1;
}

void
Fl_MS_Screen_Driver::grab(Fl_Window* win)
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
Fl_MS_Screen_Driver::add_timeout(double time, Fl_Timeout_Handler cb,
                                 void* argp)
{
}

void
Fl_MS_Screen_Driver::repeat_timeout(double time, Fl_Timeout_Handler cb,
                                    void* argp)
{
}

int
Fl_MS_Screen_Driver::has_timeout(Fl_Timeout_Handler cb, void* argp)
{
  return 0;
}

void
Fl_MS_Screen_Driver::remove_timeout(Fl_Timeout_Handler cb, void* argp)
{
}

int
Fl_MS_Screen_Driver::compose(int& del)
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
