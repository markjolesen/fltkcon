// filename.h
//
// Filename header file for the Fast Light Tool Kit (FLTK).
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
#ifndef FL_FILENAME_H
#  define FL_FILENAME_H

#  define FL_PATH_MAX 2048
const char* fl_filename_name(const char* filename);
const char* fl_filename_ext(const char* buf);
char* fl_filename_setext(char* to, int tolen, const char* ext);
int fl_filename_expand(char* to, int tolen, const char* from);
int fl_filename_absolute(char* to, int tolen, const char* from);
int fl_filename_relative(char* to, int tolen, const char* from);
int fl_filename_match(const char* name, const char* pattern);
int fl_filename_isdir(const char* name);

#  if defined(__cplusplus) && !defined(FL_DOXYGEN)
inline char*
fl_filename_setext(char* to, const char* ext)
{
  return fl_filename_setext(to, FL_PATH_MAX, ext);
}
inline int
fl_filename_expand(char* to, const char* from)
{
  return fl_filename_expand(to, FL_PATH_MAX, from);
}
inline int
fl_filename_absolute(char* to, const char* from)
{
  return fl_filename_absolute(to, FL_PATH_MAX, from);
}
int fl_filename_relative(char* to, int tolen, const char* from,
                         const char* cwd);
inline int
fl_filename_relative(char* to, const char* from)
{
  return fl_filename_relative(to, FL_PATH_MAX, from);
}
#  endif /* __cplusplus */

#  if defined (__cplusplus)
extern "C" {
#  endif /* __cplusplus */

#  if !defined(FL_DOXYGEN)
int fl_alphasort(struct dirent**, struct dirent**);
int fl_casealphasort(struct dirent**, struct dirent**);
int fl_casenumericsort(struct dirent**, struct dirent**);
int fl_numericsort(struct dirent**, struct dirent**);
#  endif

typedef int (Fl_File_Sort_F)(struct dirent**,
                             struct dirent**);   /**< File sorting function. \see fl_filename_list() */

#  if defined(__cplusplus)
}

int fl_filename_list(const char* d, struct dirent*** l,
                     Fl_File_Sort_F* s = fl_numericsort);

void fl_filename_free_list(struct dirent*** l, int n);

int fl_open_uri(const char* uri, char* msg = (char*)0, int msglen = 0);

void fl_decode_uri(char* uri);

#  endif /* __cplusplus */

#endif /* FL_FILENAME_H */
