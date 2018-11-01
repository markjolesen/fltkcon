// winscr.cxx
//
// Window support for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 1998-2015 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     http://www.fltk.org/COPYING.php
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//
#include "win.h"
#include "drvwin.h"

void
Fl_Window::set_border(int b)
{
  if (b)
  {
    if (get_border()) return;

    clear_flag(NOBORDER);
  }

  else
  {
    if (!get_border()) return;

    set_flag(NOBORDER);
  }

  pWindowDriver->use_border();
}
