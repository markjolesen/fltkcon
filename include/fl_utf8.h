// fl_utf8.h
//
// Author: Jean-Marc Lienher ( http://oksid.ch )
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 2000-2010 by O'ksi'D.
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
// Copyright 2016-2017 by Bill Spitzak and others.
//

/* Merged in some functionality from the fltk-2 version. IMM.
 * The following code is an attempt to merge the functions incorporated in FLTK2
 * with the functions provided in OksiD's fltk-1.1.6-utf8 port
 */

/**
  \file fl_utf8.h
  \brief header for Unicode and UTF-8 character handling
*/

#ifndef _HAVE_FL_UTF8_HDR_
#define _HAVE_FL_UTF8_HDR_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

int fl_utf8bytes(unsigned ucs);

int fl_utf8len(char c);

int fl_utf8len1(char c);

int fl_utf_nb_char(const unsigned char* buf, int len);

unsigned fl_utf8decode(const char* p, const char* end, int* len);

int fl_utf8encode(unsigned ucs, char* buf);

const char* fl_utf8fwd(const char* p, const char* start, const char* end);

const char* fl_utf8back(const char* p, const char* start, const char* end);

unsigned fl_ucs_to_Utf16(const unsigned ucs, unsigned short* dst,
                         const unsigned dstlen);

unsigned fl_utf8toUtf16(const char* src, unsigned srclen, unsigned short* dst,
                        unsigned dstlen);

unsigned fl_utf8towc(const char* src, unsigned srclen, wchar_t* dst,
                     unsigned dstlen);

unsigned fl_utf8fromwc(char* dst, unsigned dstlen, const wchar_t* src,
                       unsigned srclen);

unsigned fl_utf8toa (const char* src, unsigned srclen, char* dst,
                     unsigned dstlen);

unsigned fl_utf8froma (char* dst, unsigned dstlen, const char* src,
                       unsigned srclen);

int fl_utf8locale(void);

int fl_utf8test(const char* src, unsigned len);

int fl_wcwidth_(unsigned int ucs);

int fl_wcwidth(const char* src);

unsigned int fl_nonspacing(unsigned int ucs);

unsigned fl_utf8to_mb(const char* src, unsigned srclen, char* dst,
                      unsigned dstlen);
char* fl_utf2mbcs(const char* src);

unsigned fl_utf8from_mb(char* dst, unsigned dstlen, const char* src,
                        unsigned srclen);

int fl_utf_strncasecmp(const char* s1, const char* s2, int n);
int fl_utf_strcasecmp(const char* s1, const char* s2);
int fl_tolower(unsigned int ucs);
int fl_toupper(unsigned int ucs);
int fl_utf_tolower(const unsigned char* str, int len, char* buf);
int fl_utf_toupper(const unsigned char* str, int len, char* buf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HAVE_FL_UTF8_HDR_ */
