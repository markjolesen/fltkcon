// tvaluato.cxx
//
// Copyright 2018 The fltkcon authors
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
#include "box.h"
#include "counters.h"
#include "fl.h"
#include "inpvalue.h"
#include "outvalue.h"
#include "scrlbar.h"
#include "slider.h"
#include "slvalue.h"
#include "spinner.h"
#include "windbl.h"

int
main(int argc, char** argv)
{
  Fl_Double_Window* w = new Fl_Double_Window(1, 1, 78, 23,
                                             (unsigned char*)"Valuator classes");

  w->begin();

  {
    Fl_Box* o = new Fl_Box(0, 1, 30, 14, (unsigned char*)"Fl_Slider");
    o->align(Fl_Label::FL_ALIGN_TOP);
  }
  {
    Fl_Slider* o = new Fl_Slider(2, 3, 1, 10);
    o->type(Fl_Slider::FL_VERT_SLIDER);
  }
  {
    Fl_Slider* o = new Fl_Slider(4, 3, 1, 10);
    o->type(Fl_Slider::FL_VERT_NICE_SLIDER);
  }
  {
    Fl_Slider* o = new Fl_Slider(6, 3, 1, 10);
    o->type(Fl_Slider::FL_VERT_FILL_SLIDER);
  }
  {
    Fl_Slider* o = new Fl_Slider(8, 3, 20, 1);
    o->type(Fl_Slider::FL_HOR_SLIDER);
  }
  {
    Fl_Slider* o = new Fl_Slider(8, 5, 20, 1);
    o->type(Fl_Slider::FL_HOR_NICE_SLIDER);
  }
  {
    Fl_Slider* o = new Fl_Slider(8, 7, 20, 1);
    o->type(Fl_Slider::FL_HOR_FILL_SLIDER);
  }
  {
    Fl_Box* o = new Fl_Box(30, 1, 26, 14, (unsigned char*)"Fl_Scrollbar");
    o->align(Fl_Label::FL_ALIGN_TOP);
  }
  {
    Fl_Scrollbar* o = new Fl_Scrollbar(32, 3, 1, 10);
    o->type(Fl_Slider::FL_VERT_SLIDER);
    o->maximum(100);
  }
  {
    Fl_Scrollbar* o = new Fl_Scrollbar(34, 3, 20, 1);
    o->type(Fl_Slider::FL_HOR_SLIDER);
    o->maximum(100);
  }
  {
    Fl_Box* o = new Fl_Box(56, 1, 22, 14);
    o->align(Fl_Label::FL_ALIGN_TOP);
  }
  {
    Fl_Value_Input* o = new Fl_Value_Input(58, 3, 19, 1,
                                           (unsigned char*)"Fl_Value_Input");
    o->align(Fl_Label::FL_ALIGN_TOP);
  }
  {
    Fl_Value_Output* o = new Fl_Value_Output(58, 6, 19, 1,
                                             (unsigned char*)"Fl_Value_Output");
    o->align(Fl_Label::FL_ALIGN_TOP);
  }
  {
    Fl_Box* o = new Fl_Box(0, 16, 30, 7, (unsigned char*)"Fl_Counter");
    o->align(Fl_Label::FL_ALIGN_TOP);
  }
  {
    Fl_Counter* o = new Fl_Counter(5, 18, 19, 1);
  }
  {
    Fl_Simple_Counter* o = new Fl_Simple_Counter(5, 20, 19, 1);
  }
  {
    Fl_Box* o = new Fl_Box(30, 16, 26, 7, (unsigned char*)"Fl_Spinner");
    o->align(Fl_Label::FL_ALIGN_TOP);
  }
  {
    Fl_Spinner* o = new Fl_Spinner(36, 18, 14, 1);
  }
  {
    Fl_Spinner* o = new Fl_Spinner(36, 20, 14, 1);
    o->type(Fl_Input_::FL_FLOAT_INPUT);
  }
  {
    Fl_Box* o = new Fl_Box(56, 16, 21, 7, (unsigned char*)"Fl_Value_Slider");
    o->align(Fl_Label::FL_ALIGN_TOP);
  }
  {
    Fl_Value_Slider* o = new Fl_Value_Slider(58, 17, 1, 5);
    o->type(FL_VERTICAL);
  }
  {
    Fl_Value_Slider* o = new Fl_Value_Slider(61, 21, 14, 1);
    o->type(FL_HORIZONTAL);
  }

  w->end();

  w->show(argc, argv);
  return Fl::run();
}
