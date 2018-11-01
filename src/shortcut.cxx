// shortcut.cxx
//
// Shortcut support routines for the Fast Light Tool Kit (FLTK).
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
#include <ctype.h>
#include <stdlib.h>
#include "drvsys.h"
#include "fl.h"
#include "fl_rend.h"
#include "flstring.h"
#include "fl_utf8.h"

int
Fl::test_shortcut(unsigned int shortcut)
{
  if (!shortcut) return 0;

  unsigned int v = shortcut & FL_KEY_MASK;

  if (((unsigned)fl_tolower(v)) != v)
  {
    shortcut |= FL_SHIFT;
  }

  int shift = Fl::event_state();

  if ((shortcut & shift) != (shortcut & 0x7fff0000)) return 0;

  int mismatch = (shortcut ^ shift) & 0x7fff0000;

  if (mismatch & (FL_META | FL_ALT | FL_CTRL)) return 0;

  unsigned int key = shortcut & FL_KEY_MASK;

  if (!(mismatch & (FL_SHIFT)) && key == (unsigned)Fl::event_key()) return 1;

  unsigned int firstChar = fl_utf8decode((char const*)Fl::event_text(),
                                         (char const*)(Fl::event_text() + Fl::event_length()), 0);

  if ( ! (FL_CAPS_LOCK & shift) && key == firstChar) return 1;

  if ((shift & FL_CTRL) && key >= 0x3f && key <= 0x5F
      && firstChar == (key ^ 0x40)) return 1;

  return 0;
}

const char*
fl_shortcut_label(unsigned int shortcut)
{
  return fl_shortcut_label(shortcut, 0L);
}

static char*
add_modifier_key(char* p, const char* end, const char* name)
{
  size_t ln = strlen(name);

  if (p + ln > end)
  {
    if (p + 4 <= end)
    {
      strcpy(p, "...");
      p += 3;
    }

    else
      return p;
  }

  else
  {
    strcpy(p, name);
    p += ln;
  }

  if (p[-1] == '\\')
    p--;

  else if (p[-1] == '+')
  {/*empty*/}
  else
    *p++ = '+';

  return p;
}

const char*
fl_shortcut_label(unsigned int shortcut, const char** eom)
{
  static char buf[80];
  char* p = buf;
  char* end = &buf[sizeof(buf) - 20];

  if (eom) *eom = p;

  if (!shortcut)
  {
    *p = 0;
    return buf;
  }

  unsigned int key = shortcut & FL_KEY_MASK;

  if (((unsigned)fl_tolower(key)) != key)
  {
    shortcut |= FL_SHIFT;
  }

  if (shortcut & FL_CTRL)
  {
    p = add_modifier_key(p, end, fl_local_ctrl);
  }

  if (shortcut & FL_ALT)
  {
    p = add_modifier_key(p, end, fl_local_alt);
  }

  if (shortcut & FL_SHIFT)
  {
    p = add_modifier_key(p, end, fl_local_shift);
  }

  if (shortcut & FL_META)
  {
    p = add_modifier_key(p, end, fl_local_meta);
  }

  if (eom) *eom = p;

  return Fl::system_driver()->shortcut_add_key_name(key, p, buf, eom);
}

unsigned int
fl_old_shortcut(const unsigned char* s)
{
  if (!s || !*s) return 0;

  if (s[1] == 0
      && strchr("@!", s[0])) return (unsigned int)s[0]; 

  unsigned int n = 0;

  if (*s == '#')
  {
    n |= FL_ALT;
    s++;
  }

  if (*s == '+')
  {
    n |= FL_SHIFT;
    s++;
  }

  if (*s == '^')
  {
    n |= FL_CTRL;
    s++;
  }

  if (*s == '!')
  {
    n |= FL_META;
    s++;
  }

  if (*s == '@')
  {
    n |= FL_COMMAND;
    s++;
  }

  if (*s && s[1]) return n | (int)strtol((char*)s, 0, 0);

  return n | *s;
}

unsigned int
Fl_Widget::label_shortcut(const unsigned char* t)
{
  if (!t) return 0;

  for (;;)
  {
    if (*t == 0) return 0;

    if (*t == '&')
    {
      unsigned int s = fl_utf8decode((char const*)t + 1, (char const*)0, 0);

      if (s == 0) return 0;

      else if (s == (unsigned int)'&') t++;

      else return s;
    }

    t++;
  }
}

int
Fl_Widget::test_shortcut(const unsigned char* t, const bool require_alt)
{
  static int extra_test = Fl::system_driver()->need_test_shortcut_extra();

  if (!t) return 0;

  if (require_alt && Fl::event_state(FL_ALT) == 0) return 0;

  unsigned int c = fl_utf8decode((char const*)Fl::event_text(),
                                 (char const*)(Fl::event_text() + Fl::event_length()), 0);

  if (extra_test && c && Fl::event_state(FL_ALT))
    c = Fl::event_key();

  if (!c) return 0;

  unsigned int ls = label_shortcut(t);

  if (c == ls)
    return 1;

  if (extra_test && Fl::event_state(FL_ALT) && c < 128 && isalpha(c)
      && (unsigned)toupper(c) == ls)
    return 1;

  return 0;
}

int
Fl_Widget::test_shortcut()
{
  if (!(flags()&SHORTCUT_LABEL)) return 0;

  return test_shortcut(label());
}

const char*
Fl_System_Driver::shortcut_add_key_name(unsigned key, char* p, char* buf,
                                        const char** eom)
{
  if (key >= FL_F && key <= FL_F_Last)
  {
    *p++ = 'F';

    if (key > FL_F + 9) *p++ = (char)((key - FL_F) / 10 + '0');

    *p++ = (char)((key - FL_F) % 10 + '0');
  }

  else
  {
    int a = 0;
    int b = key_table_size;

    while (a < b)
    {
      int c = (a + b) / 2;

      if (key_table[c].key == key)
      {
        if (p > buf)
        {
          strcpy(p, key_table[c].name);
          return buf;
        }

        else
        {
          const char* sp = key_table[c].name;

          if (eom) *eom = sp;

          return sp;
        }
      }

      if (key_table[c].key < key) a = c + 1;

      else b = c;
    }

    if (key >= FL_KP && key <= FL_KP_Last)
    {
      strcpy(p, "KP_");
      p += 3;
      *p++ = (unsigned char)(key & 127);
    }

    else
    {
      p += fl_utf8encode(fl_toupper(key), p);
    }
  }

  *p = 0;
  return buf;
}
