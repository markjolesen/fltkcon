//
// Fl_Scroll test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2010 by Bill Spitzak and others.
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
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "fl.h"
#include "windbl.h"
#include "scroll.h"
#include "btn.h"
#include "btnlt.h"
#include "choice.h"
#include "box.h"
#include "fl_draw.h"

class Drawing : public Fl_Widget
{
    void draw();
  public:
    Drawing(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      unsigned char const* i_label = 0L) :
      Fl_Widget(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label)
    {
      align(Fl_Label::FL_ALIGN_TOP);
    }
};

void
Drawing::draw()
{
  static unsigned char* const fltkcon[4] =
  {
    (unsigned char*)" ___ _  _____ _  _____ ___  _  _ ",
    (unsigned char*)"| __| ||_   _| |/ / __/ _ \\| \\| |",
    (unsigned char*)"| _|| |__| | | ' < (_| (_) | .` |",
    (unsigned char*)"|_| |____|_| |_|\\_\\___\\___/|_|\\_|"
  };

  Fl_Widget* p = parent();

  Fl::draw_fill(x_, y_, w_, h_, 0x20, Fl::fcolor_white, Fl::bcolor_blue);

  for (unsigned slot = 0; 4 > slot; slot++)
  {
    Fl::draw_puts(
      x_,
      (y_ + slot),
      fltkcon[slot],
      strlen((char*)fltkcon[slot]),
      Fl::fcolor_white,
      Fl::bcolor_blue);
  }

  return;
}

Fl_Scroll* thescroll;

void
type_cb(Fl_Widget*, void* v)
{
  thescroll->type((unsigned char)((ptrdiff_t)v));
  thescroll->redraw();
}

Fl_Menu_Item choices[] =
{
  {(unsigned char*)"0", 0, type_cb, (void*)0},
  {(unsigned char*)"HORIZONTAL", 0, type_cb, (void*)Fl_Scroll::HORIZONTAL},
  {(unsigned char*)"VERTICAL", 0, type_cb, (void*)Fl_Scroll::VERTICAL},
  {(unsigned char*)"BOTH", 0, type_cb, (void*)Fl_Scroll::BOTH},
  {(unsigned char*)"HORIZONTAL_ALWAYS", 0, type_cb, (void*)Fl_Scroll::HORIZONTAL_ALWAYS},
  {(unsigned char*)"VERTICAL_ALWAYS", 0, type_cb, (void*)Fl_Scroll::VERTICAL_ALWAYS},
  {(unsigned char*)"BOTH_ALWAYS", 0, type_cb, (void*)Fl_Scroll::BOTH_ALWAYS},
  {0}
};

void
align_cb(Fl_Widget*, void* v)
{
  thescroll->scrollbar.align((unsigned char)((ptrdiff_t)v));
  thescroll->redraw();
}

Fl_Menu_Item align_choices[] =
{
  {(unsigned char*)"left+top", 0, align_cb, (void*)(Fl_Label::FL_ALIGN_LEFT + Fl_Label::FL_ALIGN_TOP)},
  {(unsigned char*)"left+bottom", 0, align_cb, (void*)(Fl_Label::FL_ALIGN_LEFT + Fl_Label::FL_ALIGN_BOTTOM)},
  {(unsigned char*)"right+top", 0, align_cb, (void*)(Fl_Label::FL_ALIGN_RIGHT + Fl_Label::FL_ALIGN_TOP)},
  {(unsigned char*)"right+bottom", 0, align_cb, (void*)(Fl_Label::FL_ALIGN_RIGHT + Fl_Label::FL_ALIGN_BOTTOM)},
  {0}
};

int
main(int argc, char** argv)
{
    static enum Fl::background bgcolor[]=
    {
	    Fl::bcolor_black,
	    Fl::bcolor_blue,
	    Fl::bcolor_green,
	    Fl::bcolor_cyan,
	    Fl::bcolor_red,
	    Fl::bcolor_magenta,
	    Fl::bcolor_light_gray
    };

  Fl_Window window(1, 1, 35, 15);

  Fl_Scroll flscroll(0, 0, 34, 14);
  thescroll = &flscroll;

  int n = 0;
  int y;

  for (y = 1; y < 16; y++) for (int x = 1; x < 6; x++)
    {
      unsigned char buf[20];
      sprintf((char*)buf, "%d", n++);
      Fl_Button* b = new Fl_Button((x - 1) * 12, y, 10, 1);
      b->copy_label(buf);
      b->skin_.normal_fcolor = Fl::fcolor_white;
      b->skin_.normal_bcolor = bgcolor[x];
      b->skin_.focused_fcolor = Fl::fcolor_black;
      b->skin_.focused_bcolor = Fl::bcolor_light_gray;
    }

  Drawing drawing(1, y, 34, 5, 0);

  for (y += 5; y < 25; y++) for (int x = 1; x < 6; x++)
    {
      unsigned char buf[20];
      sprintf((char*)buf, "%d", n++);
      Fl_Button* b = new Fl_Button((x - 1) * 12, y, 10, 1);
      b->copy_label(buf);
      b->skin_.normal_fcolor = Fl::fcolor_white;
      b->skin_.normal_bcolor = bgcolor[x];
      b->skin_.focused_fcolor = Fl::fcolor_black;
      b->skin_.focused_bcolor = Fl::bcolor_light_gray;
    }

  flscroll.end();

  /*
    window.resizable(flscroll);

    Fl_Box box(0,300,5*75,window.h()-300); // gray area below the flscroll
    box.box(FL_FLAT_BOX);

    Fl_Light_Button but1(150, 310, 200, 25, "box");

    Fl_Choice choice(150, 335, 200, 25, "type():");
    choice.menu(choices);
    choice.value(3);

    Fl_Choice achoice(150, 360, 200, 25, "flscrollbar.align():");
    achoice.menu(align_choices);
    achoice.value(3);
  */


  window.end();
  window.show(argc, argv);

  return Fl::run();
}
