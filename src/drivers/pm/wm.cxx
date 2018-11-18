// wm.cxx
//
// Window manager for the Fast Light Tool Kit (FLTK)
//
// Copyright 2018 The fltkcon authors
// Copyright 2018 The fltkal authors
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
#include "wm.h"
#include "block.h"
#include "fl.h"
#include "fl_draw.h"
#include "mouse.h"
#include "platform.h"
#include "pmdrvwin.h"

wm::wm()
{
}

wm::~wm()
{
}

wm::hit_type
wm::hit(Fl_Window& window, int const x, int const y) const
{
  hit_type what = HIT_NONE;

  do
  {

    int top = window.y() - 1;
    int left = window.x() - 1;
    int right = window.x() + window.w();
    int bottom = window.y() + window.h();

    if (x == left)
    {
      if (y == top)
      {
        what = HIT_NORTH_WEST;
      }

      else if (y == bottom)
      {
        what = HIT_SOUTH_WEST;
      }

      else if (y > top && y < bottom)
      {
        what = HIT_WEST;
      }

      break;
    }

    if (x == right)
    {
      if (y == top)
      {
        what = HIT_NORTH_EAST;
      }

      else if (y == bottom)
      {
        what = HIT_SOUTH_EAST;
      }

      else if (y > top && y < bottom)
      {
        what = HIT_EAST;
      }

      break;
    }

    what = HIT_WINDOW;

    if ((x >= left) && (x <= right))
    {
      if (y == top)
      {
        what = HIT_MOVE;
      }

      else if (y == bottom)
      {
        what = HIT_SOUTH;
      }
    }

  }
  while (0);

  return what;
}

bool
wm::handle_push(Fl_Window& window, hit_type const what, int const x,
                int const y) const
{
  bool handled = false;

  do
  {

    if (!(FL_WINDOW == window.type() || FL_DOUBLE_WINDOW == window.type()))
    {
      break;
    }

    if (HIT_WINDOW == what)
    {
      break;
    }

    if (HIT_NONE != what)
    {
      if (window.modal() && HIT_MOVE != what)
      {
        break;
      }

      handle_push(window, what);
      handled = true;
      break;
    }

    if (window.modal())
    {
      break;
    }

    Fl_X* i;

    for (Fl_X** pp = &Fl_X::first; (i = *pp); pp = &i->next)
    {
      Fl_Window* wi = i->w;

      if (wi != &window)
      {
        hit_type what2 =  hit((*wi), x, y);

        if (what2)
        {
          *pp = i->next;
          i->next = Fl_X::first;
          Fl_X::first = i;
          wi->take_focus();
          break;
        }
      }
    }

  }
  while (0);

  return handled;
}

static inline void
resize_east(int& left, int& top, int& width, int& height, int const delta_x,
            int const delta_y)
{
  width += delta_x;

  if (Fl_PM_Window_Driver::window_min_width > width)
  {
    width = Fl_PM_Window_Driver::window_min_width;
  }
}

static inline void
resize_west(int& left, int& top, int& width, int& height, int const delta_x,
            int const delta_y)
{
  width -= delta_x;

  if (Fl_PM_Window_Driver::window_min_width < width)
  {
    left += delta_x;
  }

  else
  {
    width += delta_x;
  }
}

static inline void
resize_north(int& left, int& top, int& width, int& height, int const delta_x,
             int const delta_y)
{
  height -= delta_y;

  if (Fl_PM_Window_Driver::window_min_height < height)
  {
    top += delta_y;
  }

  else
  {
    height += delta_y;
  }
}

static inline void
resize_south(int& left, int& top, int& width, int& height, int const delta_x,
             int const delta_y)
{
  height += delta_y;

  if (Fl_PM_Window_Driver::window_min_height > height)
  {
    height = Fl_PM_Window_Driver::window_min_height;
  }
}

static inline void
resize_north_east(int& left, int& top, int& width, int& height,
                  int const delta_x, int const delta_y)
{
  resize_north(left, top, width, height, delta_x, delta_y);
  resize_east(left, top, width, height, delta_x, delta_y);
}

static inline void
resize_north_west(int& left, int& top, int& width, int& height,
                  int const delta_x, int const delta_y)
{
  resize_north(left, top, width, height, delta_x, delta_y);
  resize_west(left, top, width, height, delta_x, delta_y);
}

static inline void
resize_south_east(int& left, int& top, int& width, int& height,
                  int const delta_x, int const delta_y)
{
  resize_south(left, top, width, height, delta_x, delta_y);
  resize_east(left, top, width, height, delta_x, delta_y);
}

static inline void
resize_south_west(int& left, int& top, int& width, int& height,
                  int const delta_x, int const delta_y)
{
  resize_south(left, top, width, height, delta_x, delta_y);
  resize_west(left, top, width, height, delta_x, delta_y);
}

void
wm::handle_push(Fl_Window& window, hit_type const what) const
{
  struct block* blk;
  int left = window.x() - 1;
  int top = window.y() - 1;
  int width = window.w() + 2;
  int height = window.h() + 2;
  int left_orig = left;
  int top_orig = top;
  int width_orig = width;
  int height_orig = height;
  int rc;

  blk = block_new();
  mouse_hide();
  block_read_frame(blk, left, top, width, height);
  Fl::draw_frame(left, top, width, height, Fl::fcolor_red, Fl::bcolor_light_gray);
  mouse_show();

  do
  {

    struct mouse_event mouse;

    rc = mouse_get_event(&mouse);

    if (0 == rc)
    {
      continue;
    }

    mouse.m_curs_col = (mouse.m_curs_col >> 3);
    mouse.m_curs_row = (mouse.m_curs_row >> 3);

    if (0 == mouse.m_btn_state)
    {
      if (left_orig != left ||
          top_orig != top ||
          width_orig != width ||
          height_orig != height)
      {
        window.resize(left + 1, top + 1, width - 2, height - 2);
        Fl::redraw();
        Fl::flush();
      }

      else
      {
        mouse_hide();
        block_write_frame(blk);
        mouse_show();
      }

      break;
    }

    int delta_x = (mouse.m_curs_col - Fl::e_x_root);
    int delta_y = (mouse.m_curs_row - Fl::e_y_root);
    int movement = (abs(delta_x) + abs(delta_y));

    Fl::e_x_root = mouse.m_curs_col;
    Fl::e_y_root = mouse.m_curs_row;

    if (movement)
    {
      int left1 = left;
      int top1 = top;
      int width1 = width;
      int height1 = height;

      switch (what)
      {
        case HIT_MOVE:
        {
          top += delta_y;
          left += delta_x;
          break;
        }

        case HIT_EAST:
        {
          resize_east(left, top, width, height, delta_x, delta_y);
          break;
        }

        case HIT_WEST:
        {
          resize_west(left, top, width, height, delta_x, delta_y);
          break;
        }

        case HIT_SOUTH:
        {
          resize_south(left, top, width, height, delta_x, delta_y);
          break;
        }

        case HIT_NORTH_EAST:
        {
          resize_north_east(left, top, width, height, delta_x, delta_y);
          break;
        }

        case HIT_NORTH_WEST:
        {
          resize_north_west(left, top, width, height, delta_x, delta_y);
          break;
        }

        case HIT_SOUTH_EAST:
        {
          resize_south_east(left, top, width, height, delta_x, delta_y);
          break;
        }

        case HIT_SOUTH_WEST:
        {
          resize_south_west(left, top, width, height, delta_x, delta_y);
          break;
        }

        default:
          break;
      }

      if (left1 != left ||
          top1 != top ||
          width1 != width ||
          height1 != height)
      {
        mouse_hide();
        block_write_frame(blk);
        block_read_frame(blk, left, top, width, height);
        Fl::draw_frame(left, top, width, height, Fl::fcolor_red, Fl::bcolor_light_gray);
        mouse_show();
      }
    }

  }
  while (1);

  block_free(blk);

  return;
}
