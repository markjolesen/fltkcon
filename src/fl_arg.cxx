// fl_arg.cxx
//
// Optional argument initialization code for the Fast Light Tool Kit (FLTK).
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
#include <ctype.h>
#include "drvwin.h"
#include "drvsys.h"
#include "fl.h"
#include "filename.h"
#include "fl_draw.h"
#include "flstring.h"
#include "win.h"

static int
fl_match(const char* a, const char* s, int atleast = 1)
{
  const char* b = s;

  while (*a && (*a == *b || tolower(*a) == *b))
  {
    a++;
    b++;
  }

  return !*a && b >= s + atleast;
}

static char arg_called;
static char return_i;
static const char* name;
static const char* geometry;
static const char* title;

int
Fl::arg(int argc, char** argv, int& i)
{
  arg_called = 1;
  const char* s = argv[i];

  if (!s)
  {
    i++;
    return 1;
  }

  if (s[0] != '-' || s[1] == '-' || !s[1])
  {
    return_i = 1;
    return 0;
  }

  s++; 

  if (fl_match(s, "iconic"))
  {
    Fl_Window::show_iconic_ = 1;
    i++;
    return 1;
  }

  else if (fl_match(s, "kbd"))
  {
    Fl::visible_focus(1);
    i++;
    return 1;
  }

  else if (fl_match(s, "nokbd", 3))
  {
    Fl::visible_focus(0);
    i++;
    return 1;
  }

  else if (fl_match(s, "dnd", 2))
  {
    Fl::dnd_text_ops(1);
    i++;
    return 1;
  }

  else if (fl_match(s, "nodnd", 3))
  {
    Fl::dnd_text_ops(0);
    i++;
    return 1;
  }

  else if (fl_match(s, "tooltips", 2))
  {
    i++;
    return 1;
  }

  else if (fl_match(s, "notooltips", 3))
  {
    i++;
    return 1;
  }

  else if (Fl::system_driver()->single_arg(s))
  {
    i++;
    return 1;
  }

  const char* v = argv[i + 1];

  if (i >= argc - 1 || !v)
    return 0; 

  if (fl_match(s, "geometry"))
  {

    int flags, gx, gy;
    unsigned int gw, gh;
    flags = Fl::system_driver()->XParseGeometry(v, &gx, &gy, &gw, &gh);

    if (!flags) return 0;

    geometry = v;

  }

  else if (fl_match(s, "display", 2))
  {
    Fl::system_driver()->display_arg(v);

  }

  else if (Fl::system_driver()->arg_and_value(s, v))
  {
  }
  else if (fl_match(s, "title", 2))
  {
    title = v;

  }

  else if (fl_match(s, "name", 2))
  {
    name = v;

  }

  else if (fl_match(s, "bg2", 3) || fl_match(s, "background2", 11))
  {
  }
  else if (fl_match(s, "bg", 2) || fl_match(s, "background", 10))
  {
  }
  else if (fl_match(s, "fg", 2) || fl_match(s, "foreground", 10))
  {
  }
  else if (fl_match(s, "scheme", 1))
  {
  }
  else
  {
    return 0;
  }

  i += 2;
  return 2;
}

int
Fl::args(int argc, char** argv, int& i, Fl_Args_Handler cb)
{
  arg_called = 1;
  i = 1;

  while (i < argc)
  {
    if (cb && cb(argc, argv, i)) continue;

    if (!arg(argc, argv, i)) return return_i ? i : 0;
  }

  return i;
}

void
Fl_Window::show(int argc, char** argv)
{
  if (argc && !arg_called) Fl::args(argc, argv);

#if 0
  Fl::get_system_colors();
#endif

  pWindowDriver->show_with_args_begin();

  static char beenhere;

  if (!beenhere)
  {
    if (geometry)
    {
      int fl = 0, gx = x(), gy = y();
      unsigned int gw = w(), gh = h();
      fl = Fl::system_driver()->XParseGeometry(geometry, &gx, &gy, &gw, &gh);

      if (fl & Fl_System_Driver::fl_XNegative) gx = Fl::w() - w() + gx;

      if (fl & Fl_System_Driver::fl_YNegative) gy = Fl::h() - h() + gy;

      Fl_Widget* r = resizable();

      if (!r) resizable(this);

      if (fl & (Fl_System_Driver::fl_XValue | Fl_System_Driver::fl_YValue))
        x(-1), resize(gx, gy, gw, gh);

      else
        size(gw, gh);

      resizable(r);
    }
  }

#if 0

  if (name)
  {
    xclass(name);
    name = 0;
  }

  else if (!xclass()) xclass(fl_filename_name(argv[0]));

  if (title)
  {
    label(title);
    title = 0;
  }

  else if (!label()) label(xclass());

#endif

  if (!beenhere)
  {
    beenhere = 1;
#if 0
    Fl::scheme(Fl::scheme()); // opens display!  May call Fl::fatal()
#endif
  }

  show();

  pWindowDriver->show_with_args_end(argc, argv);
}

static const char* const helpmsg =
  "options are:\n"
  " -bg2 color\n"
  " -bg color\n"
  " -di[splay] host:n.n\n"
  " -dn[d]\n"
  " -fg color\n"
  " -g[eometry] WxH+X+Y\n"
  " -i[conic]\n"
  " -k[bd]\n"
  " -na[me] classname\n"
  " -nod[nd]\n"
  " -nok[bd]\n"
  " -not[ooltips]\n"
  " -s[cheme] scheme\n"
  " -ti[tle] windowtitle\n"
  " -to[oltips]";

const char* const Fl::help = helpmsg + 13;

void
Fl::args(int argc, char** argv)
{
  int i;

  if (Fl::args(argc, argv, i) < argc) Fl::error(helpmsg);
}
