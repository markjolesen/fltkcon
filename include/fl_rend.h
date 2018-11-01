// fl_rend.h
//
// Portable drawing function header file for the Fast Light Tool Kit (FLTK).
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
#ifndef fl_draw_H
#define fl_draw_H

#include "widget.h"
#include "fl_color.h"
#include "fl_draw.h"
#include "fl_enums.h"

class Fl_Window;

extern char fl_draw_shortcut;

typedef unsigned Fl_Color;
typedef unsigned Fl_Cursor;

inline void
fl_color(Fl_Color c) {/*fl_graphics_driver->color(c);*/ }
inline void
fl_color(int c)
{
  fl_color((Fl_Color)c);
}
inline Fl_Color
fl_color()
{
  return 0;/*fl_graphics_driver->color();*/
}

inline void
fl_focus_rect(int x, int y, int w, int h) { /*fl_graphics_driver->focus_rect(x, y, w, h); */}
inline void
fl_rect(int x, int y, int w, int h, Fl_Color c)
{
  fl_color(c);
  fl_rect(x, y, w, h, c);
}
inline void
fl_rectf(int x, int y, int w, int h) { /*fl_graphics_driver->rectf(x,y,w,h); */}
inline void
fl_rectf(int x, int y, int w, int h, Fl_Color c)
{
  fl_color(c);
  fl_rectf(x, y, w, h);
}

void fl_rectf(int x, int y, int w, int h, unsigned char r, unsigned char g,
              unsigned char b);

double fl_width(const unsigned char* txt);

inline double
fl_width(const unsigned char* txt, int n)
{
  return n; /*fl_graphics_driver->width(txt, n);*/
}

inline double
fl_width(unsigned int c)
{
  return 1;/*fl_graphics_driver->width(c);*/
}
void fl_text_extents(const char*, int& dx, int& dy, int& w, int& h);
inline void
fl_text_extents(const char* t, int n, int& dx, int& dy, int& w, int& h)
{
  dx = 0, dy = 0;
  w = n;
  h = 1;/*fl_graphics_driver->text_extents(t, n, dx, dy, w, h);*/
}

const char* fl_latin1_to_local(const char* t, int n = -1);
const char* fl_local_to_latin1(const char* t, int n = -1);
const char* fl_mac_roman_to_local(const char* t, int n = -1);
const char* fl_local_to_mac_roman(const char* t, int n = -1);

void fl_draw(const unsigned char* str, int x, int y);

void fl_draw(int angle, const unsigned char* str, int x, int y);

inline void
fl_draw(
  const unsigned char* str,
  unsigned int const n,
  int const x,
  int const y,
  enum Fl::foreground const fcolor,
  enum Fl::background const bcolor)
{
  Fl::draw_puts(x, y, str, n, fcolor, bcolor);
}

inline void
fl_draw(int angle, const unsigned char* str, int n, int x,
        int y) {/*fl_graphics_driver->draw(angle,str,n,x,y); */}

inline void
fl_rtl_draw(const char* str, int n, int x,
            int y) {/*fl_graphics_driver->rtl_draw(str,n,x,y); */}

void fl_measure(const unsigned char* str, int& x, int& y,
                int draw_symbols = 1);

void fl_draw(
  const unsigned char* str,
  int const x,
  int const y,
  unsigned int const w,
  unsigned int const h,
  enum Fl_Label::Fl_Align const align,
  enum Fl::foreground const fcolor,
  enum Fl::background const bcolor,
  enum Fl::foreground const hcolor,
  int const draw_symbols = 1);

void fl_draw(
  const unsigned char* str,
  int const x,
  int const y,
  unsigned int const w,
  unsigned int const h,
  enum Fl_Label::Fl_Align const align,
  enum Fl::foreground const fcolor,
  enum Fl::background const bcolor,
  enum Fl::foreground const hcolor,
  void (*callthis)( const char* str, unsigned int const n, int const x,
                    int const y, enum Fl::foreground const fcolor,
                    enum Fl::background const bcolor),
  int const draw_symbols = 1);

void fl_scroll(int X, int Y, int W, int H, int dx, int dy,
               void (*draw_area)(void*, int, int, int, int), void* data);
const char* fl_shortcut_label(unsigned int shortcut);
const char* fl_shortcut_label(unsigned int shortcut, const char** eom);
unsigned int fl_old_shortcut(const char* s);
void fl_overlay_rect(int x, int y, int w, int h);
void fl_overlay_clear();
void fl_cursor(Fl_Cursor);
void fl_cursor(Fl_Cursor, Fl_Color fg, Fl_Color bg = 255);
const unsigned char* fl_expand_text(const unsigned char* from,
                                    unsigned char* buf, int maxbuf,
                                    double maxw, int& n, double& width,
                                    int wrap, int draw_symbols = 0);

void fl_set_status(int X, int Y, int W, int H);


int fl_draw_symbol(const unsigned char* label, int x, int y, int w, int h,
                   Fl_Color);
int fl_add_symbol(const unsigned char* name, void (*drawit)(Fl_Color),
                  int scalable);

#endif
