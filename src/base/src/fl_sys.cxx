// fl_sys.cxx
//
// (bits taken from fl_utf8.cxx)
//
// A base class for platform specific system calls
// for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The FDOSTUI authors
// Copyright 2010-2018 by Bill Spitzak and others.
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
#include "drvsys.h"
#include "fl_sys.h"

#if (defined(__DOS__) || defined(__NT__))
char const Fl::_directory_separator = '\\';
#else
char const Fl::_directory_separator = '/';
#endif

int
Fl::chmod(const char* f, int mode)
{
  return Fl::system_driver()->chmod(f, mode);
}

int
Fl::access(const char* f, int mode)
{
  return Fl::system_driver()->access(f, mode);
}

int
Fl::stat(const char* f, struct stat* b)
{
  return Fl::system_driver()->stat(f, b);
}

char*
Fl::getcwd(char* buf, int len)
{
  if (buf == NULL)
  {
    buf = (char*)malloc(len + 1);
  }

  return Fl::system_driver()->getcwd(buf, len);
}

int
Fl::chdir(const char* path)
{
  if (!path)
    return -1;

  return Fl::system_driver()->chdir(path);
}

FILE*
Fl::fopen(const char* f, const char* mode)
{
  return Fl::system_driver()->fopen(f, mode);
}

int
Fl::system(const char* f)
{
  return Fl::system_driver()->system(f);
}

int
Fl::execvp(const char* file, char* const* argv)
{
  return Fl::system_driver()->execvp(file, argv);
}

int
Fl::open(const char* fname, int oflags, ...)
{
  int pmode;
  va_list ap;
  va_start(ap, oflags);
  pmode = va_arg (ap, int);
  va_end(ap);
  return Fl::system_driver()->open(fname, oflags, pmode);
}

int
Fl::open_binary(const char* fname, int binary, int oflags, ...)
{
  int pmode;
  va_list ap;
  va_start(ap, oflags);
  pmode = va_arg (ap, int);
  va_end(ap);
  return Fl::system_driver()->open_ext(fname, binary, oflags, pmode);
}

int
Fl::unlink(const char* fname)
{
  return Fl::system_driver()->unlink(fname);
}

int
Fl::rmdir(const char* f)
{
  return Fl::system_driver()->rmdir(f);
}

char*
Fl::getenv(const char* v)
{
  return Fl::system_driver()->getenv(v);
}

int
Fl::mkdir(const char* f, int mode)
{
  return Fl::system_driver()->mkdir(f, mode);
}

int
Fl::rename(const char* f, const char* t)
{
  return Fl::system_driver()->rename(f, t);
}

void
Fl::make_path_for_file(const char* path)
{
  const char* s = strrchr( path, Fl::_directory_separator );

  if ( !s ) return;

  size_t len =  (s - path);
  char* p = (char*)malloc( len + 1 );
  memcpy( p, path, len );
  p[len] = 0;
  Fl::make_path( p );
  free( p );
}

char
Fl::make_path(const char* path)
{
  if (Fl::access(path, 0))
  {
    const char* s = strrchr( path, Fl::_directory_separator);

    if ( !s ) return 0;

    size_t len = (size_t) (s - path);
    char* p = (char*)malloc( len + 1 );
    memcpy( p, path, len );
    p[len] = 0;
    Fl::make_path( p );
    free( p );
    Fl::mkdir(path, 0700);
  }

  return 1;
}
