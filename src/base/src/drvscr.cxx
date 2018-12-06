// drvscr.cxx
//
// All screen related calls in a driver style class.
//
// Copyright 2018 The FDOSTUI authors
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

#include <stdio.h>
#include "drvwin.h"
#include "drvscr.h"
#include "box.h"
#include "fl.h"
#include "group.h"
#include "input.h"
#include "platform.h"
#include "win.h"

char Fl_Screen_Driver::bg_set = 0;
char Fl_Screen_Driver::bg2_set = 0;
char Fl_Screen_Driver::fg_set = 0;


Fl_Screen_Driver::Fl_Screen_Driver() :
  num_screens(-1), text_editor_extra_key_bindings(NULL)
{
}

Fl_Screen_Driver::~Fl_Screen_Driver()
{
}


void
Fl_Screen_Driver::display(const char*)
{
  // blank
}


int
Fl_Screen_Driver::visual(int)
{
  // blank
  return 1;
}


void
Fl_Screen_Driver::screen_xywh(int& X, int& Y, int& W, int& H, int mx, int my)
{
  screen_xywh(X, Y, W, H, screen_num(mx, my));
}


void
Fl_Screen_Driver::screen_work_area(int& X, int& Y, int& W, int& H, int mx,
                                   int my)
{
  screen_work_area(X, Y, W, H, screen_num(mx, my));
}


int
Fl_Screen_Driver::screen_count()
{
  if (num_screens < 0)
    init();

  return num_screens ? num_screens : 1;
}


void
Fl_Screen_Driver::screen_xywh(int& X, int& Y, int& W, int& H, int mx, int my,
                              int mw, int mh)
{
  screen_xywh(X, Y, W, H, screen_num(mx, my, mw, mh));
}


int
Fl_Screen_Driver::screen_num(int x, int y)
{
  int screen = 0;

  if (num_screens < 0) init();

  for (int i = 0; i < num_screens; i ++)
  {
    int sx, sy, sw, sh;
    screen_xywh(sx, sy, sw, sh, i);

    if ((x >= sx) && (x < (sx + sw)) && (y >= sy) && (y < (sy + sh)))
    {
      screen = i;
      break;
    }
  }

  return screen;
}


// Return the number of pixels common to the two rectangular areas
float
Fl_Screen_Driver::fl_intersection(int x1, int y1, int w1, int h1,
                                  int x2, int y2, int w2, int h2)
{
  if (x1 + w1 < x2 || x2 + w2 < x1 || y1 + h1 < y2 || y2 + h2 < y1)
    return 0.;

  int int_left = x1 > x2 ? x1 : x2;
  int int_right = x1 + w1 > x2 + w2 ? x2 + w2 : x1 + w1;
  int int_top = y1 > y2 ? y1 : y2;
  int int_bottom = y1 + h1 > y2 + h2 ? y2 + h2 : y1 + h1;
  return (float)(int_right - int_left) * (int_bottom - int_top);
}


int
Fl_Screen_Driver::screen_num(int x, int y, int w, int h)
{
  int best_screen = 0;
  float best_intersection = 0.;

  if (num_screens < 0) init();

  for (int i = 0; i < num_screens; i++)
  {
    int sx = 0, sy = 0, sw = 0, sh = 0;
    screen_xywh(sx, sy, sw, sh, i);
    float sintersection = fl_intersection(x, y, w, h, sx, sy, sw, sh);

    if (sintersection > best_intersection)
    {
      best_screen = i;
      best_intersection = sintersection;
    }
  }

  return best_screen;
}


const char*
Fl_Screen_Driver::get_system_scheme()
{
  return 0L;
}

/** The bullet character used by default by Fl_Secret_Input */
int Fl_Screen_Driver::secret_input_character = 0x2022;

void
Fl_Screen_Driver::compose_reset()
{
  Fl::compose_state = 0;
}

bool
Fl_Screen_Driver::input_widget_handle_key(
  int key, unsigned mods, unsigned shift, Fl_Input* input)
{

  switch (key)
  {
    case FL_Delete:
    {
      // int selected = false; // __mjo (input->position() != input->mark()) ? 1 : 0;

//      if (mods == 0 && shift && selected)
//        return input->kf_copy_cut();    // Shift-Delete with selection (WP,NP,WOW,GE,KE,OF)

//      if (mods == 0 && shift && !selected)
//        return input->kf_delete_char_right(); // Shift-Delete no selection (WP,NP,WOW,GE,KE,!OF)

      if (mods == 0)          return
          input->del(); // Delete         (Standard)

//      if (mods == FL_CTRL)    return
//          input->kf_delete_word_right(); // Ctrl-Delete    (WP,!NP,WOW,GE,KE,!OF)

      return false;             // ignore other combos, pass to parent
    }

    case FL_Left:
      if (mods == 0)          return
          input->left();  // Left           (WP,NP,WOW,GE,KE,OF)

//      if (mods == FL_CTRL)    return
//          input->kf_move_word_left();  // Ctrl-Left      (WP,NP,WOW,GE,KE,!OF)

//      if (mods == FL_META)    return
//          input->kf_move_char_left();  // Meta-Left      (WP,NP,?WOW,GE,KE)

      return false;             // ignore other combos, pass to parent

    case FL_Right:
      if (mods == 0)          return
          input->right(); // Right          (WP,NP,WOW,GE,KE,OF)

//      if (mods == FL_CTRL)    return
//          input->kf_move_word_right(); // Ctrl-Right     (WP,NP,WOW,GE,KE,!OF)

//      if (mods == FL_META)    return
//          input->kf_move_char_right(); // Meta-Right     (WP,NP,?WOW,GE,KE,!OF)

      return false;             // ignore other combos, pass to parent

    case FL_Up:
      if (mods == 0)          return
          input->up(); // Up             (WP,NP,WOW,GE,KE,OF)

#if 0

      if (mods == FL_CTRL)    return
          input->kf_move_up_and_sol(); // Ctrl-Up        (WP,!NP,WOW,GE,!KE,OF)

#endif
      return 0;             // ignore other combos, pass to parent

    case FL_Down:
      if (mods == 0)          return
          input->down(); // Dn             (WP,NP,WOW,GE,KE,OF)

#if 0

      if (mods == FL_CTRL)    return
          input->kf_move_down_and_eol(); // Ctrl-Down      (WP,!NP,WOW,GE,!KE,OF)

#endif
      return 0;             // ignore other combos, pass to parent

#if 0

    case FL_Page_Up:

      // Fl_Input has no scroll control, so instead we move the cursor by one page
      if (mods == 0)          return
          input->kf_page_up(); // PageUp         (WP,NP,WOW,GE,KE)

      if (mods == FL_CTRL)    return
          input->kf_page_up(); // Ctrl-PageUp    (!WP,!NP,!WOW,!GE,KE,OF)

      if (mods == FL_ALT)     return
          input->kf_page_up(); // Alt-PageUp     (!WP,!NP,!WOW,!GE,KE,OF)

      return 0;             // ignore other combos, pass to parent

    case FL_Page_Down:
      if (mods == 0)          return
          input->kf_page_down(); // PageDn         (WP,NP,WOW,GE,KE)

      if (mods == FL_CTRL)    return
          input->kf_page_down(); // Ctrl-PageDn    (!WP,!NP,!WOW,!GE,KE,OF)

      if (mods == FL_ALT)     return
          input->kf_page_down(); // Alt-PageDn     (!WP,!NP,!WOW,!GE,KE,OF)

      return 0;             // ignore other combos, pass to parent
#endif

    case FL_Home:
      if (mods == 0)          return
          input->home();  // Home           (WP,NP,WOW,GE,KE,OF)

//      if (mods == FL_CTRL)    return
//          input->kf_top();   // Ctrl-Home      (WP,NP,WOW,GE,KE,OF)

      return 0;             // ignore other combos, pass to parent

    case FL_End:
      if (mods == 0)          return
          input->end();  // End            (WP,NP,WOW,GE,KE,OF)

//      if (mods == FL_CTRL)    return
//          input->kf_bottom();    // Ctrl-End       (WP,NP,WOW,GE,KE,OF)

      return false;             // ignore other combos, pass to parent

    case FL_BackSpace:
      if (mods == 0)          return
          input->bs(); // Backspace      (WP,NP,WOW,GE,KE,OF)

#if 0

      if (mods == FL_CTRL)    return
          input->kf_delete_word_left(); // Ctrl-Backspace (WP,!NP,WOW,GE,KE,!OF)

      return 0;
#endif
      // ignore other combos, pass to parent
  }

  return false;
}

#if 0
static void
del_transient_window(void* data)
{
  Fl_Window* win = (Fl_Window*)data;
  Fl::delete_widget(win);
}
#endif

void
Fl_Screen_Driver::open_display()
{
  open_display_platform();
  static bool been_here = false;

  if (!been_here)
  {
    been_here = true;
    // int scount = screen_count(); // keep here
  }
}


// simulation of XParseColor:
int
Fl_Screen_Driver::parse_color(const char* p, unsigned char& r, unsigned char& g,
                              unsigned char& b)
{
  if (*p == '#') p++;

  size_t n = strlen(p);
  size_t m = n / 3;
  const char* pattern = 0;

  switch (m)
  {
    case 1:
      pattern = "%1x%1x%1x";
      break;

    case 2:
      pattern = "%2x%2x%2x";
      break;

    case 3:
      pattern = "%3x%3x%3x";
      break;

    case 4:
      pattern = "%4x%4x%4x";
      break;

    default:
      return 0;
  }

  int R, G, B;

  if (sscanf(p, pattern, &R, &G, &B) != 3) return 0;

  switch (m)
  {
    case 1:
      R *= 0x11;
      G *= 0x11;
      B *= 0x11;
      break;

    case 3:
      R >>= 4;
      G >>= 4;
      B >>= 4;
      break;

    case 4:
      R >>= 8;
      G >>= 8;
      B >>= 8;
      break;
  }

  r = (unsigned char)R;
  g = (unsigned char)G;
  b = (unsigned char)B;
  return 1;
}
