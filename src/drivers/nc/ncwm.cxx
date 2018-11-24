// ncwm.cxx
//
// Window manager for the Fast Light Tool Kit (FLTK)
//
// Copyright 2018 The fltkcon authors
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
#include "ncwm.h"
#include "block.h"
#include "fl.h"
#include "fl_draw.h"
#include "platform.h"

ncwm::ncwm() :
  wm()
{
  return;
}

ncwm::~ncwm()
{
  return;
}

void
ncwm::handle_push(Fl_Window& window, hit_type const what) const
{
  struct block* blk;
  int left = window.x() - 1;
  int top = window.y() - 1;
  unsigned int width = window.w() + 2;
  unsigned int height = window.h() + 2;
  int left_orig = left;
  int top_orig = top;
  unsigned int width_orig = width;
  unsigned int height_orig = height;
  int rc;

  blk = block_new();
  block_read_frame(blk, left, top, width, height);
  Fl::draw_frame(left, top, width, height, Fl::fcolor_red, Fl::bcolor_light_gray);
  refresh();

  do
  {

    nodelay(stdscr, 1);
    int key = getch();

    if (KEY_MOUSE != key)
    {
      continue;
    }

    MEVENT mouse;

    int rc = getmouse(&mouse);

	if (ERR == rc)
	{
		mouse.bstate= BUTTON1_RELEASED;
	}

    if ((BUTTON1_RELEASED | BUTTON3_RELEASED) & mouse.bstate)
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
        block_write_frame(blk);
      }

      break;
    }

    int delta_x = (mouse.x - Fl::e_x_root);
    int delta_y = (mouse.y - Fl::e_y_root);
    int movement = (abs(delta_x) + abs(delta_y));

    Fl::e_x_root = mouse.x;
    Fl::e_y_root = mouse.y;

    if (movement)
    {
      int left1 = left;
      int top1 = top;
      unsigned int width1 = width;
      unsigned int height1 = height;

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
        block_write_frame(blk);
        block_read_frame(blk, left, top, width, height);
        Fl::draw_frame(left, top, width, height, Fl::fcolor_red, Fl::bcolor_light_gray);
	refresh();
      }
    }

  }
  while (1);

  block_free(blk);

  return;
}
