/*
 thello.cxx

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#include "widget.h"
#include "win.h"
#include "btn.h"

static void
quit_cb(Fl_Widget*, void* userdata)
{
  Fl_Window* window = reinterpret_cast<Fl_Window*>(userdata);
  window->hide();
  return;
}


int
main(int argc, char** argv)
{

  Fl_Window window(1, 1, 20, 5,
                   reinterpret_cast<unsigned char const*>("Hello Example"));

  Fl_Button* btn = new Fl_Button(2, 2, 15, 1,
                                 reinterpret_cast<unsigned char const*>("Hello"));
  btn->when((Fl_Widget::FL_WHEN_CHANGED |
             Fl_Widget::FL_WHEN_ENTER_KEY_ALWAYS |
             Fl_Widget::FL_WHEN_RELEASE));

  btn->callback(quit_cb, &window);

  window.end();
  window.show(argc, argv);

  return Fl::run();
}
