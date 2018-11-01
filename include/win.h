// win.h
//
// Window header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2016 by Bill Spitzak and others.
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
#ifndef Fl_Window_H
#define Fl_Window_H

#include <stdlib.h>
#include "fl.h"
#include "group.h"
#include "fl_skin.h"

#define FL_WINDOW 0xF0
#define FL_DOUBLE_WINDOW 0xF1
#define FL_CHILD_WINDOW         (FL_DOUBLE_WINDOW + 1)
#define FL_MENU_WINDOW          (FL_DOUBLE_WINDOW + 2)
#define FL_MENU_TITLE_WINDOW    (FL_DOUBLE_WINDOW + 3)
#define FL_TOOLTIP_WINDOW       (FL_DOUBLE_WINDOW + 4)

class Fl_X;
class Fl_Window_Driver;
class Fl_Double_Window;

class Fl_Window : public Fl_Group
{
    friend int Fl::arg(int argc, char** argv, int& i);
    friend class Fl_X;
    friend class Fl_Window_Driver;
  private:
    static char show_iconic_;

    Fl_X* i;
    Fl_Window_Driver* pWindowDriver;

    const unsigned char* iconlabel_;
    int minw, minh, maxw, maxh;
    int dw, dh, aspect;
    unsigned char size_range_set;

    void _Fl_Window();

    Fl_Window(const Fl_Window&);
    Fl_Window& operator=(const Fl_Window&);

  protected:

    static Fl_Window* current_;
    void draw();
    virtual void flush();

    void
    force_position(int force)
    {
      if (force) set_flag(FORCE_POSITION);

      else clear_flag(FORCE_POSITION);
    }

    int
    force_position() const
    {
      return ((flags() & FORCE_POSITION) ? 1 : 0);
    }

    void free_icons();

  public:

    struct Fl::skin_box skin_;

    Fl_Window(int const w, int const h, const unsigned char* title = 0);

    Fl_Window(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      const unsigned char* i_label = 0L);

    virtual ~Fl_Window();

    virtual bool handle(enum Fl_Event const);

    virtual void resize(int X, int Y, int W, int H);

    void set_border(int b);

    void
    clear_border()
    {
      set_flag(NOBORDER);
    }

    unsigned int
    get_border() const
    {
      return !(flags() & NOBORDER);
    }

    void
    set_override()
    {
      set_flag(NOBORDER | OVERRIDE);
    }

    unsigned int
    override() const
    {
      return flags()&OVERRIDE;
    }

    void
    set_modal()
    {
      set_flag(MODAL);
    }

    unsigned int
    modal() const
    {
      return flags() & MODAL;
    }

    void
    set_non_modal()
    {
      set_flag(NON_MODAL);
    }

    unsigned int
    non_modal() const
    {
      return flags() & (NON_MODAL | MODAL);
    }

    void
    clear_modal_states()
    {
      clear_flag(NON_MODAL | MODAL);
    }

    void
    set_menu_window()
    {
      set_flag(MENU_WINDOW);
    }

    unsigned int
    menu_window() const
    {
      return flags() & MENU_WINDOW;
    }

    void
    set_tooltip_window()
    {
      set_flag(TOOLTIP_WINDOW);
      clear_flag(MENU_WINDOW);
    }

    unsigned int
    tooltip_window() const
    {
      return flags() & TOOLTIP_WINDOW;
    }

    void hotspot(int x, int y, int offscreen = 0);
    void hotspot(const Fl_Widget*, int offscreen = 0);

    void
    hotspot(const Fl_Widget& p, int offscreen = 0)
    {
      hotspot(&p, offscreen);
    }

    void size_range(int minw, int minh, int maxw = 0, int maxh = 0, int dw = 0,
                    int dh = 0, int aspect = 0);

    const unsigned char*
    label() const
    {
      return Fl_Widget::label();
    }

    const unsigned char*
    iconlabel() const
    {
      return iconlabel_;
    }

    void label(const unsigned char*);
    void iconlabel(const unsigned char*);
    void label(const unsigned char* label, const unsigned char* iconlabel);
    void copy_label(const unsigned char* a);

    int
    shown()
    {
      return i != 0;
    }

    virtual void
    show();

    virtual void
    hide();

    void
    show(
      int argc, char** argv);

    void iconize();

    int x_root() const ;
    int y_root() const ;

    static Fl_Window* current();
    void make_current();

    static void default_callback(Fl_Window*, void* v);

    int decorated_w() const;

    int decorated_h() const;

    virtual Fl_Window*
    as_window()
    {
      return this;
    }

    virtual class Fl_Overlay_Window*
      as_overlay_window()
    {
        return 0L;
    }

    virtual class Fl_Double_Window*
      as_double_window()
    {
        return 0L;
    }

    int is_shaped();
    int screen_num();
};

#endif
