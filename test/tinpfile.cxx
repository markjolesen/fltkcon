/*
 talign.cxx

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#include "inpfile.h"
#include "windbl.h"

int
main(int argc, char** argv)
{
  Fl_Double_Window* w = new Fl_Double_Window(1, 1, 78, 23,
                                             (unsigned char*)"File Input");

  Fl_File_Input inp(3, 3, 70, 3);

  w->show(argc, argv);
  return Fl::run();
}
