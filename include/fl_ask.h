// fl_ask.h
//
// Standard dialog header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2011 by Bill Spitzak and others.
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
#ifndef fl_ask_H
#  define fl_ask_H

#include "fl_enums.h"

class Fl_Widget;

/** Different system beeps available.
 \sa fl_beep(int) */
enum Fl_Beep
{
  FL_BEEP_DEFAULT = 0,  ///< Default beep.
  FL_BEEP_MESSAGE,  ///< Message beep.
  FL_BEEP_ERROR,  ///< Error beep.
  FL_BEEP_QUESTION,  ///< Question beep.
  FL_BEEP_PASSWORD,  ///< Password beep.
  FL_BEEP_NOTIFICATION  ///< Notification beep.
};

#  ifdef __GNUC__
/* the GNUC-specific attribute appearing below in prototypes with a variable list of arguments
 helps detection of mismatches between format string and argument list at compilation time */
#    define __fl_attr(x) __attribute__ (x)
#  else
#    define __fl_attr(x)
#  endif // __GNUC__

void fl_beep(int type = FL_BEEP_DEFAULT);
void fl_message(const char*, ...) __fl_attr((__format__ (__printf__, 1, 2)));
void fl_alert(const char*, ...) __fl_attr((__format__ (__printf__, 1, 2)));
int fl_choice(const char* q, const unsigned char* b0, const unsigned char* b1,
              const unsigned char* b2,
              ...) __fl_attr((__format__ (__printf__, 1, 5)));
const char* fl_input(const char* label, const char* deflt = 0,
                     ...) __fl_attr((__format__ (__printf__, 1, 3)));

unsigned char* fl_password(
  const char* label,
  const unsigned char* deflt = 0,
  ...) __fl_attr((__format__ (__printf__, 1, 3)));

Fl_Widget* fl_message_icon();

void fl_message_hotspot(int enable);
int fl_message_hotspot(void);

void fl_message_title(const unsigned char* title);
void fl_message_title_default(const unsigned char* title);

extern const unsigned char* fl_no;
extern const unsigned char* fl_yes;
extern const unsigned char* fl_ok;
extern const unsigned char* fl_cancel;
extern const unsigned char* fl_close;
#endif // !fl_ask_H
