// slvalue.cxx
//
// Value slider widget for the Fast Light Tool Kit (FLTK).
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
#include "slvalue.h"
#include "slhzval.h"
#include "fl.h"
#include "fl_draw.h"
#include "fl_rend.h"

Fl_Value_Slider::Fl_Value_Slider(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label) :
  Fl_Slider(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label)
{
  step(1, 100);
}

void
Fl_Value_Slider::draw()
{

  int sxx = x(), syy = y(), sww = w(), shh = h();
  int bxx = x(), byy = y(), bww = 5, bhh = 1;

  if (horizontal())
  {
    sxx += 5;
    sww -= 5;
  }

  else
  {
    syy += 1;
    shh -= 1;
  }

  Fl_Slider::draw(sxx, syy, sww, shh);

  unsigned char buf[128];
  format(buf);
  fl_draw(buf, bxx, byy, bww, bhh, Fl_Label::FL_ALIGN_CLIP,
          skin_.normal_fcolor, skin_.normal_bcolor, skin_.normal_fcolor);

  return;
}

int
Fl_Value_Slider::handle(int event)
{
  if (event == FL_PUSH && Fl::visible_focus())
  {
    Fl::focus(this);
    redraw();
  }

  int sxx = x(), syy = y(), sww = w(), shh = h();

  if (horizontal())
  {
    sxx += 35;
    sww -= 35;
  }

  else
  {
    syy += 25;
    shh -= 25;
  }

  return Fl_Slider::handle(event, sxx, syy, sww, shh);
}


Fl_Hor_Value_Slider::Fl_Hor_Value_Slider(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label) :
  Fl_Value_Slider(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label)
{
  type(FL_HOR_SLIDER);
}
