// texted.h
//
// Header file for Fl_Text_Editor class.
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 2001-2010 by Bill Spitzak and others.
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
// Original code Copyright Mark Edel.  Permission to distribute under
// the LGPL for the FLTK library granted by Mark Edel.
//
#ifndef FL_TEXT_EDITOR_H
#define FL_TEXT_EDITOR_H

#include "textdsp.h"

#define FL_TEXT_EDITOR_ANY_STATE  (-1L)

class Fl_Text_Editor : public Fl_Text_Display
{
  public:

    typedef int (*Key_Func)(int key, Fl_Text_Editor* editor);

    struct Key_Binding
    {
      int          key;
      int          state;
      Key_Func     function;
      Key_Binding* next;
    };

    Fl_Text_Editor(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      unsigned char const* i_label = 0L);

    ~Fl_Text_Editor()
    {
      remove_all_key_bindings();
    }

    virtual bool
    handle(
      enum Fl_Event const event);

    void
    insert_mode(int b)
    {
      insert_mode_ = b;
    }
    int
    insert_mode()
    {
      return insert_mode_;
    }
    void tab_nav(int val);
    int tab_nav() const;
    void add_key_binding(int key, int state, Key_Func f, Key_Binding** list);
    void
    add_key_binding(int key, int state, Key_Func f)
    {
      add_key_binding(key, state, f, &key_bindings);
    }
    void remove_key_binding(int key, int state, Key_Binding** list);
    void
    remove_key_binding(int key, int state)
    {
      remove_key_binding(key, state, &key_bindings);
    }
    void remove_all_key_bindings(Key_Binding** list);
    void
    remove_all_key_bindings()
    {
      remove_all_key_bindings(&key_bindings);
    }
    void add_default_key_bindings(Key_Binding** list);
    Key_Func bound_key_function(int key, int state, Key_Binding* list) const;
    Key_Func
    bound_key_function(int key, int state) const
    {
      return bound_key_function(key, state, key_bindings);
    }
    void
    default_key_function(Key_Func f)
    {
      default_key_function_ = f;
    }

    static int kf_default(int c, Fl_Text_Editor* e);
    static int kf_ignore(int c, Fl_Text_Editor* e);
    static int kf_backspace(int c, Fl_Text_Editor* e);
    static int kf_enter(int c, Fl_Text_Editor* e);
    static int kf_move(int c, Fl_Text_Editor* e);
    static int kf_shift_move(int c, Fl_Text_Editor* e);
    static int kf_ctrl_move(int c, Fl_Text_Editor* e);
    static int kf_c_s_move(int c, Fl_Text_Editor* e);
    static int kf_meta_move(int c, Fl_Text_Editor* e);
    static int kf_m_s_move(int c, Fl_Text_Editor* e);
    static int kf_home(int, Fl_Text_Editor* e);
    static int kf_end(int c, Fl_Text_Editor* e);
    static int kf_left(int c, Fl_Text_Editor* e);
    static int kf_up(int c, Fl_Text_Editor* e);
    static int kf_right(int c, Fl_Text_Editor* e);
    static int kf_down(int c, Fl_Text_Editor* e);
    static int kf_page_up(int c, Fl_Text_Editor* e);
    static int kf_page_down(int c, Fl_Text_Editor* e);
    static int kf_insert(int c, Fl_Text_Editor* e);
    static int kf_delete(int c, Fl_Text_Editor* e);
    static int kf_copy(int c, Fl_Text_Editor* e);
    static int kf_cut(int c, Fl_Text_Editor* e);
    static int kf_paste(int c, Fl_Text_Editor* e);
    static int kf_select_all(int c, Fl_Text_Editor* e);
    static int kf_undo(int c, Fl_Text_Editor* e);

  protected:
    int handle_key();
    void maybe_do_callback();

#ifndef FL_DOXYGEN
    int insert_mode_;
    Key_Binding* key_bindings;
#endif

    static Key_Binding* global_key_bindings;

#ifndef FL_DOXYGEN
    Key_Func default_key_function_;
#endif
};

#endif
