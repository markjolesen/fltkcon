// fl_watch.h
//
// Ticker class for the Fast Light Tool Kit (FLTK)
//
// Copyright 2018 The fltkcon authors
// Copyright 2018 The fltkal authors
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
#if !defined(__FL_WATCH_H__)

#include "fl_ticks.h"

class fl_watch
{

  public:

    fl_watch();

    virtual ~fl_watch();

    bool expired() const;

    void set(double const seconds);

    void reset();

    void elapse(Fl::ticks_t const& elapsed);

  protected:

    bool expired_;
    Fl::ticks_t expiry_;
    Fl::ticks_t current_;

  private:

    fl_watch(fl_watch const&);

    fl_watch& operator=(fl_watch const&);

};

inline
fl_watch::fl_watch() :
  expired_(true),
  expiry_(),
  current_()
{
  return;
}

inline
fl_watch::~fl_watch()
{
  return;
}

inline void
fl_watch::set(double const seconds)
{
  if (seconds > 0)
  {
    expired_ = false;
    Fl::ticks_convert(expiry_, seconds);
  }

  else
  {
    expired_ = true;
    Fl::ticks_convert(expiry_, 0);
  }

  current_ = expiry_;
}

inline void
fl_watch::reset()
{
  current_ = expiry_;
  expired_ = false;
}

inline bool
fl_watch::expired() const
{
  return expired_;
}

inline void
fl_watch::elapse(Fl::ticks_t const& elapsed)
{

  if (false == expired_)
  {
    expired_ = Fl::ticks_elapse(current_, elapsed);
  }

  return;
}

#define __FL_WATCH_H__
#endif
