/*
 ttexted.cxx

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to FDOSTUI Library. This work is published
 from: United States.
*/
#include "win.h"
#include "texted.h"

int
main()
{
  Fl_Window w(1, 1, 78, 23, (unsigned char*)"Test Window");
  Fl_Text_Editor  ted(0, 0, 50, 20);
  Fl_Text_Buffer buffer(1024, 1024);
  ted.buffer(&buffer);
  w.show();
  Fl::run();
  return 0;
}

