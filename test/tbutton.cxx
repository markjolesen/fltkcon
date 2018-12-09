/*
 tbutton.cxx

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#include "windbl.h"
#include "btn.h"
#include "btnk.h"
#include "btnlt.h"
#include "btnmenu.h"
#include "btnrad.h"
#include "btnradlt.h"
#include "btnradro.h"
#include "btnret.h"
#include "btnro.h"
#include "btnrpt.h"
#include "btntog.h"

int
main(int argc, char** argv)
{

  Fl_Window window(1, 1, 35, 15);

  {
    Fl_Button* o = new Fl_Button(1, 1, 25, 1,
                                 reinterpret_cast<unsigned char const*>("Button"));
    o->align(Fl_Label::FL_ALIGN_LEFT | Fl_Label::FL_ALIGN_INSIDE);
  }
  {
    Fl_Check_Button* o = new Fl_Check_Button(1, 2, 25, 1,
                                             reinterpret_cast<unsigned char const*>("Check Button"));
    o->align(Fl_Label::FL_ALIGN_LEFT | Fl_Label::FL_ALIGN_INSIDE);
  }
  {
    Fl_Light_Button* o = new Fl_Light_Button(1, 3, 25, 1,
                                             reinterpret_cast<unsigned char const*>("Light Button"));
    o->align(Fl_Label::FL_ALIGN_LEFT | Fl_Label::FL_ALIGN_INSIDE);
  }
  {
    static Fl_Menu_Item items[] =
    {
      {(unsigned char*)"Item 1"},
      {(unsigned char*)"Item 2"},
      {(unsigned char*)"Item 3"},
      {0}
    };
    Fl_Menu_Button* o = new Fl_Menu_Button(1, 4, 25, 1,
                                           reinterpret_cast<unsigned char const*>("Button Menu"));
    o->menu(items);
    o->align(Fl_Label::FL_ALIGN_LEFT | Fl_Label::FL_ALIGN_INSIDE);
  }
  {
    Fl_Radio_Button* o = new Fl_Radio_Button(1, 5, 25, 1,
                                             reinterpret_cast<unsigned char const*>("Radio Button"));
    o->align(Fl_Label::FL_ALIGN_LEFT | Fl_Label::FL_ALIGN_INSIDE);
  }
  {
    Fl_Radio_Light_Button* o = new Fl_Radio_Light_Button(1, 6, 25, 1,
                                                         reinterpret_cast<unsigned char const*>("Radio Light Button"));
    o->align(Fl_Label::FL_ALIGN_LEFT | Fl_Label::FL_ALIGN_INSIDE);
  }
  {
    Fl_Radio_Round_Button* o = new Fl_Radio_Round_Button(1, 7, 25, 1,
                                                         reinterpret_cast<unsigned char const*>("Radio Round Button"));
    o->align(Fl_Label::FL_ALIGN_LEFT | Fl_Label::FL_ALIGN_INSIDE);
  }
  {
    Fl_Return_Button* o = new Fl_Return_Button(1, 8, 25, 1,
                                               reinterpret_cast<unsigned char const*>("Return Button"));
    o->align(Fl_Label::FL_ALIGN_LEFT | Fl_Label::FL_ALIGN_INSIDE);
  }
  {
    Fl_Round_Button* o = new Fl_Round_Button(1, 9, 25, 1,
                                             reinterpret_cast<unsigned char const*>("Round Button"));
    o->align(Fl_Label::FL_ALIGN_LEFT | Fl_Label::FL_ALIGN_INSIDE);
  }
  {
    Fl_Repeat_Button* o = new Fl_Repeat_Button(1, 10, 25, 1,
                                               reinterpret_cast<unsigned char const*>("Repeat Button"));
    o->align(Fl_Label::FL_ALIGN_LEFT | Fl_Label::FL_ALIGN_INSIDE);
  }
  {
    Fl_Toggle_Button* o = new Fl_Toggle_Button(1, 11, 25, 1,
                                               reinterpret_cast<unsigned char const*>("Toggle Button"));
    o->align(Fl_Label::FL_ALIGN_LEFT | Fl_Label::FL_ALIGN_INSIDE);
  }

  window.end();
  window.show(argc, argv);

  return Fl::run();
}
