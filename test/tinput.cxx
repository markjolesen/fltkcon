/*
 tinput.cxx

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#include "win.h"
#include "btn.h"
#include "fl_draw.h"
#include "fl_skin.h"
#include "input.h"

int
main()
{
  Fl_Window w(10, 5, 20, 20, (unsigned char*)"Test Window");
  Fl_Input  i(0, 0, 15, 1);
  w.show();
  Fl::run();
  return 0;
}
