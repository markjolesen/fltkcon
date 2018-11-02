// tmenubar.cxx
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
#include <stdlib.h>
#include "box.h"
#include "btnmenu.h"
#include "btntog.h"
#include "choice.h"
#include "fl_draw.h"
#include "fl.h"
#include "menubar.h"
#include "windbl.h"

// Globals
// Fl_Simple_Terminal *G_tty = 0;

void
window_cb(Fl_Widget* w, void*)
{
  /*  puts("window callback called");*/ // end of program, so stdout instead of G_tty
  ((Fl_Double_Window*)w)->hide();
}

void
quit_cb(Fl_Widget*, void*)
{
  exit(0);
}

Fl_Menu_Item hugemenu[100];

Fl_Menu_Item menutable[] =
{
  /*  {(unsigned char*)"foo",0,0,0,FL_MENU_INACTIVE}, */
  {(unsigned char*)"&File", 0, 0, 0, FL_SUBMENU},
  {(unsigned char*)"&Open", FL_ALT + 'o', 0, 0, FL_MENU_INACTIVE},
  {(unsigned char*)"&Close",  0,  0},
  {(unsigned char*)"&Quit", FL_ALT + 'q', quit_cb, 0, FL_MENU_DIVIDER},
  {(unsigned char*)"shortcut", 'a'},
  {(unsigned char*)"shortcut", FL_SHIFT + 'a'},
  {(unsigned char*)"shortcut", FL_CTRL + 'a'},
  {(unsigned char*)"shortcut", FL_CTRL + FL_SHIFT + 'a'},
  {(unsigned char*)"shortcut", FL_ALT + 'a'},
  {(unsigned char*)"shortcut", FL_ALT + FL_SHIFT + 'a'},
  {(unsigned char*)"shortcut", FL_ALT + FL_CTRL + 'a'},
  {(unsigned char*)"shortcut", FL_ALT + FL_SHIFT + FL_CTRL + 'a', 0, 0, FL_MENU_DIVIDER},
  {(unsigned char*)"shortcut", '\r'},
  {(unsigned char*)"shortcut", FL_CTRL + FL_Enter, 0, 0, FL_MENU_DIVIDER},
  {(unsigned char*)"shortcut", FL_F + 1},
  {(unsigned char*)"shortcut", FL_SHIFT + FL_F + 1},
  {(unsigned char*)"shortcut", FL_CTRL + FL_F + 1},
  {(unsigned char*)"shortcut", FL_SHIFT + FL_CTRL + FL_F + 1},
  {(unsigned char*)"shortcut", FL_ALT + FL_F + 1},
  {(unsigned char*)"shortcut", FL_ALT + FL_SHIFT + FL_F + 1},
  {(unsigned char*)"shortcut", FL_ALT + FL_CTRL + FL_F + 1},
  {(unsigned char*)"shortcut", FL_ALT + FL_SHIFT + FL_CTRL + FL_F + 1, 0, 0, FL_MENU_DIVIDER},
  {(unsigned char*)"&Submenus", FL_ALT + 'S', 0, (void*)"Submenu1", FL_SUBMENU},
  {(unsigned char*)"A very long menu item"},
  {(unsigned char*)"&submenu", FL_CTRL + 'S',  0, (void*)"submenu2", FL_SUBMENU},
  {(unsigned char*)"item 1"},
  {(unsigned char*)"item 2"},
  {(unsigned char*)"item 3"},
  {(unsigned char*)"item 4"},
  {0},
  {(unsigned char*)"after submenu"},
  {0},
  {0},
  {(unsigned char*)"&Edit", FL_F + 2, 0, 0, FL_SUBMENU},
  {(unsigned char*)"Undo",  FL_ALT + 'z', 0},
  {(unsigned char*)"Redo",  FL_ALT + 'r', 0, 0, FL_MENU_DIVIDER},
  {(unsigned char*)"Cut", FL_ALT + 'x', 0},
  {(unsigned char*)"Copy",  FL_ALT + 'c', 0},
  {(unsigned char*)"Paste", FL_ALT + 'v', 0},
  {(unsigned char*)"Inactive", FL_ALT + 'd', 0, 0, FL_MENU_INACTIVE},
  {(unsigned char*)"Clear", 0,  0, 0, FL_MENU_DIVIDER},
  {(unsigned char*)"Invisible", FL_ALT + 'e',  0, 0, FL_MENU_INVISIBLE},
  {(unsigned char*)"Preferences", 0, 0},
  {(unsigned char*)"Size",  0,  0},
  {0},
  {(unsigned char*)"&Checkbox", FL_F + 3, 0, 0, FL_SUBMENU},
  {(unsigned char*)"&Alpha",  FL_F + 2, 0, (void*)1, FL_MENU_TOGGLE},
  {(unsigned char*)"&Beta", 0,  0, (void*)2, FL_MENU_TOGGLE},
  {(unsigned char*)"&Gamma",  0,  0, (void*)3, FL_MENU_TOGGLE},
  {(unsigned char*)"&Delta",  0,  0, (void*)4, FL_MENU_TOGGLE | FL_MENU_VALUE},
  {(unsigned char*)"&Epsilon", 0,  0, (void*)5, FL_MENU_TOGGLE},
  {(unsigned char*)"&Pi", 0,  0, (void*)6, FL_MENU_TOGGLE},
  {(unsigned char*)"&Mu", 0,  0, (void*)7, FL_MENU_TOGGLE | FL_MENU_DIVIDER},
  {(unsigned char*)"Red", 0,  0, (void*)1, FL_MENU_TOGGLE},
  {(unsigned char*)"Black", 0,  0, (void*)1, FL_MENU_TOGGLE | FL_MENU_DIVIDER},
  {(unsigned char*)"00",  0,  0, (void*)1, FL_MENU_TOGGLE},
  {(unsigned char*)"000", 0,  0, (void*)1, FL_MENU_TOGGLE},
  {0},
  {(unsigned char*)"&Radio", 0, 0, 0, FL_SUBMENU},
  {(unsigned char*)"&Alpha",  0,  0, (void*)1, FL_MENU_RADIO},
  {(unsigned char*)"&Beta", 0,  0, (void*)2, FL_MENU_RADIO},
  {(unsigned char*)"&Gamma",  0,  0, (void*)3, FL_MENU_RADIO},
  {(unsigned char*)"&Delta",  0,  0, (void*)4, FL_MENU_RADIO | FL_MENU_VALUE},
  {(unsigned char*)"&Epsilon", 0,  0, (void*)5, FL_MENU_RADIO},
  {(unsigned char*)"&Pi", 0,  0, (void*)6, FL_MENU_RADIO},
  {(unsigned char*)"&Mu", 0,  0, (void*)7, FL_MENU_RADIO | FL_MENU_DIVIDER},
  {(unsigned char*)"Red", 0,  0, (void*)1, FL_MENU_RADIO},
  {(unsigned char*)"Black", 0,  0, (void*)1, FL_MENU_RADIO | FL_MENU_DIVIDER},
  {(unsigned char*)"00",  0,  0, (void*)1, FL_MENU_RADIO},
  {(unsigned char*)"000", 0,  0, (void*)1, FL_MENU_RADIO},
  {0},
  {(unsigned char*)"&Font", 0, 0, 0, FL_SUBMENU},
  {(unsigned char*)"Normal",  0, 0, 0, 0, 0},
  {(unsigned char*)"Bold",  0, 0, 0, 0, 0},
  {(unsigned char*)"Italic",  0, 0, 0, 0, 0},
  {(unsigned char*)"BoldItalic", 0, 0, 0, 0, 0},
  {(unsigned char*)"Small", 0, 0, 0, 0, 0},
  {(unsigned char*)"Emboss",  0, 0, 0, 0, 0},
  {(unsigned char*)"Engrave", 0, 0, 0, 0, 0},
  {(unsigned char*)"Shadow",  0, 0, 0, 0, 0},
  {(unsigned char*)"@->", 0, 0, 0, 0, 0},
  {0},
  {(unsigned char*)"&International", 0, 0, 0, FL_SUBMENU},
  {(unsigned char*)"Sharp Ess", 0x0000df},
  {(unsigned char*)"A Umlaut", 0x0000c4},
  {(unsigned char*)"a Umlaut", 0x0000e4},
  {(unsigned char*)"Euro currency", FL_COMMAND + 0x0020ac},
  {(unsigned char*)"the &\xc3\xbc Umlaut"},  // &uuml;
  {(unsigned char*)"the capital &\xc3\x9c"}, // &Uuml;
  {(unsigned char*)"convert \xc2\xa5 to &\xc2\xa3"}, // Yen to GBP
  {(unsigned char*)"convert \xc2\xa5 to &\xe2\x82\xac"}, // Yen to Euro
  {(unsigned char*)"Hangul character Sios &\xe3\x85\x85"},
  {(unsigned char*)"Hangul character Cieuc", 0x003148},
  {0},
  {(unsigned char*)"E&mpty", 0, 0, 0, FL_SUBMENU},
  {0},
  {(unsigned char*)"&Inactive", 0,  0, 0, FL_MENU_INACTIVE | FL_SUBMENU},
  {(unsigned char*)"A very long menu item"},
  {(unsigned char*)"A very long menu item"},
  {0},
  {(unsigned char*)"Invisible", 0, 0, 0, FL_MENU_INVISIBLE | FL_SUBMENU},
  {(unsigned char*)"A very long menu item"},
  {(unsigned char*)"A very long menu item"},
  {0},
  {(unsigned char*)"&Huge", 0, 0, (void*)hugemenu, FL_SUBMENU_POINTER},
  {(unsigned char*)"button", FL_F + 4, 0, 0, FL_MENU_TOGGLE},
  {0}
};

Fl_Menu_Item pulldown[] =
{
  {(unsigned char*)"Red", FL_ALT + 'r'},
  {(unsigned char*)"Green", FL_ALT + 'g'},
  {(unsigned char*)"Blue",  FL_ALT + 'b'},
  {(unsigned char*)"Strange", FL_ALT + 's', 0, 0, FL_MENU_INACTIVE},
  {(unsigned char*)"&Charm",  FL_ALT + 'c'},
  {(unsigned char*)"Truth", FL_ALT + 't'},
  {(unsigned char*)"Beauty",  FL_ALT + 'b'},
  {0}
};

int
main(int argc, char** argv)
{

  for (int i = 0; i < 99; i++)
  {
    char buf[100];
    sprintf(buf, "item %d", i);
    hugemenu[i].text = (unsigned char*)strdup(buf);
  }

  Fl_Double_Window window(1, 1, 78, 23);
  window.callback(window_cb);

  Fl_Menu_Bar menubar(0, 0, 78, 1);
  menubar.menu(menutable);

  Fl_Menu_Button mb1(10, 10, 12, 1, (unsigned char*)"&menubutton");
  mb1.menu(pulldown);

  Fl_Choice ch(50, 10, 12, 1, (unsigned char*)"&choice:");
  ch.menu(pulldown);

#if 0

  Fl_Menu_Button mb(0, 0, WIDTH, 400, (unsigned char*)"&popup");
  mb.type(Fl_Menu_Button::POPUP3);
  mb.menu(menutable);
  mb.remove(1); // delete the "File" submenu
  Fl_Box b(200, 200, 200, 100,
           (unsigned char*)"Press right button\nfor a pop-up menu");
  window.resizable(&mb);
#endif
  window.end();
  window.show(argc, argv);

  return Fl::run();
}
