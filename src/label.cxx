// labeltyp.cxx
//
// Label drawing routines for the Fast Light Tool Kit (FLTK).
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
#include "widget.h"
#include "fl.h"
#include "fl_rend.h"
#include "group.h"

void
fl_normal_measure(const Fl_Label* o, int& W, int& H)
{
  fl_measure(o->value, W, H);
}

#if 0
#define MAX_LABELTYPE 16

static Fl_Label_Draw_F* table[MAX_LABELTYPE] =
{
  fl_normal_label,
  fl_no_label,
  fl_normal_label,  // _FL_SHADOW_LABEL,
  fl_normal_label,  // _FL_ENGRAVED_LABEL,
  fl_normal_label,  // _FL_EMBOSSED_LABEL,
  fl_no_label,    // _FL_MULTI_LABEL,
  fl_no_label,    // _FL_ICON_LABEL,
  // FL_FREE_LABELTYPE+n:
  fl_no_label, fl_no_label, fl_no_label,
  fl_no_label, fl_no_label, fl_no_label,
  fl_no_label, fl_no_label, fl_no_label
};

static Fl_Label_Measure_F* measure[MAX_LABELTYPE];

/** Sets the functions to call to draw and measure a specific labeltype. */
void
Fl::set_labeltype(Fl_Label::Fl_Labeltype t, Fl_Label_Draw_F* f,
                  Fl_Label_Measure_F* m)
{
  table[t] = f;
  measure[t] = m;
}

#endif

////////////////////////////////////////////////////////////////

void
Fl_Label::draw(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  Fl_Align const i_align,
  enum Fl::foreground const i_fcolor,
  enum Fl::background const i_bcolor,
  enum Fl::foreground const i_hcolor) const
{
  if (FL_NO_LABEL != type && value && *value)
  {
    Fl_Label::draw(i_pos_x, i_pos_y, i_len_x, i_len_y, value,
                   i_align, i_fcolor, i_bcolor, i_hcolor);
  }

  return;
}

static void
Fl_Label::draw(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label,
  Fl_Align const i_align,
  enum Fl::foreground const i_fcolor,
  enum Fl::background const i_bcolor,
  enum Fl::foreground const i_hcolor)
{
  if (i_label && *i_label)
  {
    fl_draw(i_label, i_pos_x, i_pos_y, i_len_x, i_len_y,
            i_align, i_fcolor, i_bcolor, i_hcolor);
  }
}

void
Fl_Label::measure(int& W, int& H) const
{
  if (!value)
  {
    W = H = 0;
    return;
  }

  fl_normal_measure(this, W, H);
}

void
Fl_Widget::draw_label() const
{
  int X = x_;
  int W = w_;

  if (W > 11 && align() & (Fl_Label::FL_ALIGN_LEFT | Fl_Label::FL_ALIGN_RIGHT))
  {
    X += 1;
    W -= 2;
  }

  draw_label(X, y_, W, h_);

  return;
}

void
Fl_Widget::draw_label(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y) const
{
  if ((align() & 15) && !(align() & Fl_Label::FL_ALIGN_INSIDE)) return;

  enum Fl::foreground l_fcolor;
  enum Fl::background l_bcolor;

  if (this != Fl::focus())
  {
    l_fcolor = skin_.normal_fcolor;
    l_bcolor = skin_.normal_bcolor;
  }

  else
  {
    l_fcolor = skin_.focused_fcolor;
    l_bcolor = skin_.focused_bcolor;
  }

  draw_label(i_pos_x, i_pos_y, i_len_x, i_len_y,
             align(), l_fcolor, l_bcolor, l_fcolor);

  return;
}

void
Fl_Widget::draw_label(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  enum Fl_Label::Fl_Align const i_align,
  enum Fl::foreground const i_fcolor,
  enum Fl::background const i_bcolor,
  enum Fl::foreground const i_hcolor) const
{
  if (label_.value && *label_.value)
  {
    if (flags()&SHORTCUT_LABEL) fl_draw_shortcut = 1;

    Fl_Label::draw(i_pos_x, i_pos_y, i_len_x, i_len_y,
                   label_.value, i_align, i_fcolor, i_bcolor, i_hcolor);
    fl_draw_shortcut = 0;
  }
}
