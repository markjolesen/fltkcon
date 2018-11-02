/*
 talign.cxx

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#include "box.h"
#include "btnk.h"
#include "btnradro.h"
#include "windbl.h"

Fl_Check_Button* btn_center;
Fl_Check_Button* btn_inside;
Fl_Radio_Round_Button* btn_top;
Fl_Radio_Round_Button* btn_bottom;
Fl_Radio_Round_Button* btn_left;
Fl_Radio_Round_Button* btn_right;
Fl_Box* box;

void
btn_cb(Fl_Widget*, void*)
{
  unsigned int align = 0;

  if (btn_center->value())
  {
    align |= Fl_Label::FL_ALIGN_CENTER;
  }

  if (btn_inside->value())
  {
    align |= Fl_Label::FL_ALIGN_INSIDE;
  }

  if (btn_top->value())
  {
    align |= Fl_Label::FL_ALIGN_TOP;
  }

  if (btn_bottom->value())
  {
    align |= Fl_Label::FL_ALIGN_BOTTOM;
  }

  if (btn_left->value())
  {
    align |= Fl_Label::FL_ALIGN_LEFT;
  }

  if (btn_right->value())
  {
    align |= Fl_Label::FL_ALIGN_RIGHT;
  }

  box->align(align);
  box->damage(Fl_Widget::FL_DAMAGE_ALL);

  return;
}

int
main(int argc, char** argv)
{
  Fl_Double_Window* w = new Fl_Double_Window(1, 1, 78, 23,
                                             (unsigned char*)"Valuator classes, showing values for type()");
  Fl_Group* g;

  w->begin();

  btn_center = new Fl_Check_Button(0, 0, 15, 1, (unsigned char*)"CENTER");
  btn_center->callback(btn_cb, 0);
  btn_inside = new Fl_Check_Button(0, 1, 15, 1, (unsigned char*)"INSIDE");
  btn_inside->callback(btn_cb, 0);

  Fl_Group* g1 = new Fl_Group(0, 2, 30, 2);

  btn_top = new Fl_Radio_Round_Button(0, 2, 15, 1, (unsigned char*)"TOP");
  btn_top->callback(btn_cb, 0);

  btn_bottom = new Fl_Radio_Round_Button(0, 3, 15, 1, (unsigned char*)"BOTTOM");
  btn_bottom->callback(btn_cb, 0);

  g1->end();

  Fl_Group* g2 = new Fl_Group(0, 4, 30, 2);

  btn_left = new Fl_Radio_Round_Button(0, 4, 15, 1, (unsigned char*)"LEFT");
  btn_left->callback(btn_cb, 0);

  btn_right = new Fl_Radio_Round_Button(0, 5, 15, 1, (unsigned char*)"RIGHT");
  btn_right->callback(btn_cb, 0);
  g2->end();

  box = new Fl_Box(25, 5, 30, 15, (unsigned char*)"This is text");

  w->end();

  w->show(argc, argv);
  return Fl::run();
}
