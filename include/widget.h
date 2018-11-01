// widget.h
//
// Widget header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2016, 2018 by Bill Spitzak and others.
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
#ifndef Fl_Widget_H
#define Fl_Widget_H

#include <stddef.h>
#include "fl_color.h"
#include "fl_enums.h"
#include "fl_skin.h"

class Fl_Widget;
class Fl_Window;
class Fl_Group;

namespace Fl
{
  void focus(Fl_Widget*);
};

typedef void (Fl_Callback )(Fl_Widget*, void*);
typedef Fl_Callback* Fl_Callback_p;
typedef void (Fl_Callback0)(Fl_Widget*);
typedef void (Fl_Callback1)(Fl_Widget*, long);

struct Fl_Label
{

  enum Fl_LabelType
  {
    FL_NORMAL_LABEL = 0,
    FL_NO_LABEL
  };

  enum Fl_Align
  {
    FL_ALIGN_CENTER    = 0x0000,
    FL_ALIGN_TOP   = 0x0001,
    FL_ALIGN_BOTTOM    = 0x0002,
    FL_ALIGN_LEFT    = 0x0004,
    FL_ALIGN_RIGHT   = 0x0008,
    FL_ALIGN_INSIDE    = 0x0010,
    /* FL_ALIGN_TEXT_OVER_IMAGE = 0x0020,*/
    /* FL_ALIGN_IMAGE_OVER_TEXT = 0x0000, */
    FL_ALIGN_CLIP    = 0x0040,
    FL_ALIGN_WRAP    = 0x0080,
    /* FL_ALIGN_IMAGE_NEXT_TO_TEXT = 0x0100, */
    /* FL_ALIGN_TEXT_NEXT_TO_IMAGE = 0x0120,*/
    /* FL_ALIGN_IMAGE_BACKDROP  = 0x0200, */
    FL_ALIGN_TOP_LEFT  = (FL_ALIGN_TOP | FL_ALIGN_LEFT),
    FL_ALIGN_TOP_RIGHT = (FL_ALIGN_TOP | FL_ALIGN_RIGHT),
    FL_ALIGN_BOTTOM_LEFT = (FL_ALIGN_BOTTOM | FL_ALIGN_LEFT),
    FL_ALIGN_BOTTOM_RIGHT  = (FL_ALIGN_BOTTOM | FL_ALIGN_RIGHT),
    FL_ALIGN_LEFT_TOP  = 0x0007,
    FL_ALIGN_RIGHT_TOP = 0x000b,
    FL_ALIGN_LEFT_BOTTOM = 0x000d,
    FL_ALIGN_RIGHT_BOTTOM  = 0x000e,
    FL_ALIGN_NOWRAP    = 0x0000,
    FL_ALIGN_POSITION_MASK   = 0x000f
                               /* FL_ALIGN_IMAGE_MASK      = 0x0320 */
  };

  const unsigned char* value;
  Fl_Align align_;
  Fl_LabelType type;

  static void
  draw(
    int const i_pos_x,
    int const i_pos_y,
    unsigned int const i_len_x,
    unsigned int const i_len_y,
    unsigned char const* i_label,
    Fl_Align const i_align,
    enum Fl::foreground const i_fcolor,
    enum Fl::background const i_bcolor,
    enum Fl::foreground const i_hcolor);

  void
  draw(
    int const i_pos_x,
    int const i_pos_y,
    unsigned int const i_len_x,
    unsigned int const i_len_y,
    Fl_Align const i_align,
    enum Fl::foreground const i_fcolor,
    enum Fl::background const i_bcolor,
    enum Fl::foreground const i_hcolor) const;

  void
  measure(int& w, int& h) const;

};

class Fl_Widget
{

  public:

    enum Fl_When
    {
      FL_WHEN_NEVER   = 0,
      FL_WHEN_CHANGED = 1,
      FL_WHEN_NOT_CHANGED = 2,
      FL_WHEN_RELEASE = 4,
      FL_WHEN_RELEASE_ALWAYS = 6,
      FL_WHEN_ENTER_KEY = 8,
      FL_WHEN_ENTER_KEY_ALWAYS = 10,
      FL_WHEN_ENTER_KEY_CHANGED = 11
    };

    enum Fl_Damage
    {
      FL_DAMAGE_NONE     = 0x00,
      FL_DAMAGE_CHILD    = 0x01,
      FL_DAMAGE_EXPOSE   = 0x02,
      FL_DAMAGE_SCROLL   = 0x04,
      FL_DAMAGE_OVERLAY  = 0x08,
      FL_DAMAGE_USER1    = 0x10,
      FL_DAMAGE_USER2    = 0x20,
      FL_DAMAGE_ALL      = 0x80
    };

  protected:

    enum Fl_Flags
    {
      INACTIVE        = 1 << 0,
      INVISIBLE       = 1 << 1,
      OUTPUT          = 1 << 2,
      NOBORDER        = 1 << 3,
      FORCE_POSITION  = 1 << 4,
      NON_MODAL       = 1 << 5,
      SHORTCUT_LABEL  = 1 << 6,
      CHANGED         = 1 << 7,
      OVERRIDE        = 1 << 8,
      VISIBLE_FOCUS   = 1 << 9,
      COPIED_LABEL    = 1 << 10,
      CLIP_CHILDREN   = 1 << 11,
      MENU_WINDOW     = 1 << 12,
      TOOLTIP_WINDOW  = 1 << 13,
      MODAL           = 1 << 14,
      NO_OVERLAY      = 1 << 15,
      GROUP_RELATIVE  = 1 << 16,
      COPIED_TOOLTIP  = 1 << 17,
      FULLSCREEN      = 1 << 18,
      MAC_USE_ACCENTS_MENU = 1 << 19,
      NEEDS_KEYBOARD  = 1 << 20,
      USERFLAG3       = 1 << 29,
      USERFLAG2       = 1 << 30,
      USERFLAG1       = 1 << 31
    };

  private:

    friend class Fl_Group;
    friend void Fl::focus(Fl_Widget*);

  protected:

    Fl_Group* parent_;
    Fl_Callback* callback_;
    void* user_data_;
    int x_, y_;
    unsigned int w_, h_;
    Fl_Label label_;
    Fl_Flags flags_;
    unsigned char type_;
    Fl_Damage damage_;
    Fl_When when_;
    bool box_;

    void
    draw_box() const;

    void
    draw_box(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      enum Fl::foreground const i_fcolor,
      enum Fl::background const i_bcolor) const;

  private:

    Fl_Widget(const Fl_Widget&);
    Fl_Widget& operator=(const Fl_Widget&);

  protected:

    Fl_Widget(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      unsigned char const* i_label = 0L);

    void
    x(int const v)
    {
      x_ = v;
    }

    void
    y(int const v)
    {
      y_ = v;
    }

    void
    w(int const v)
    {
      w_ = v;
    }

    void
    h(int const v)
    {
      h_ = v;
    }

    unsigned int
    flags() const
    {
      return flags_;
    }

    void
    set_flag(unsigned int const c)
    {
      unsigned int bits = (static_cast<unsigned int>(flags_) | c);
      flags_ = static_cast<Fl_Flags>(bits);
    }

    void
    clear_flag(unsigned int const c)
    {
      unsigned bits = (static_cast<unsigned int>(flags_) & (~c));
      flags_ = static_cast<Fl_Flags>(bits);
    }

  public:

    struct Fl::skin_widget skin_;

    virtual ~Fl_Widget();

    virtual void draw() = 0;

    virtual bool
    handle(
      enum Fl_Event const event);

    int
    is_label_copied() const
    {
      return ((flags_ & COPIED_LABEL) ? 1 : 0);
    }

    Fl_Group*
    parent() const
    {
      return parent_;
    }

    void
    parent(Fl_Group* p)
    {
      parent_ = p;
    }

    unsigned char
    type() const
    {
      return type_;
    }

    void
    type(unsigned char const t)
    {
      type_ = t;
    }

    int
    x() const
    {
      return x_;
    }

    int
    y() const
    {
      return y_;
    }

    int
    w() const
    {
      return w_;
    }

    int
    h() const
    {
      return h_;
    }

    virtual
    void resize(
      int const x,
      int const y,
      unsigned int const w,
      unsigned int const h);

    int
    damage_resize(
      int const x,
      int const y,
      unsigned int const w,
      unsigned int const h);

    void
    position(int const X, int const Y)
    {
      resize(X, Y, w_, h_);
    }

    void
    size(int const W, int const H)
    {
      resize(x_, y_, W, H);
    }

    enum Fl_Label::Fl_Align
    align() const
    {
      return label_.align_;
    }

    void
    align(enum Fl_Label::Fl_Align const alignment)
    {
      label_.align_ = alignment;
    }

    void
    align(int const alignment)
    {
      label_.align_ = static_cast<Fl_Label::Fl_Align>(alignment);
    }

    bool
    box() const
    {
      return box_;
    }

    void
    box(
      bool const i_has_box)
    {
      box_ = i_has_box;
      return;
    }

    const unsigned char*
    label() const
    {
      return label_.value;
    }

    void label(const unsigned char* text);

    void copy_label(const unsigned char* new_label);

    void
    label(Fl_Label::Fl_LabelType const a, const unsigned char* b)
    {
      label_.type = a;
      label_.value = b;
    }

    Fl_Label::Fl_LabelType
    labeltype() const
    {
      return label_.type;
    }

    void
    labeltype(Fl_Label::Fl_LabelType const a)
    {
      label_.type = a;
    }

    Fl_Callback_p
    callback() const
    {
      return callback_;
    }

    void
    callback(Fl_Callback* cb, void* p)
    {
      callback_ = cb;
      user_data_ = p;
    }

    void
    callback(Fl_Callback* cb)
    {
      callback_ = cb;
    }

    void
    callback(Fl_Callback0* cb)
    {
      callback_ = (Fl_Callback*)cb;
    }

    void
    callback(Fl_Callback1* cb, long p = 0)
    {
      callback_ = (Fl_Callback*)cb;
      user_data_ = (void*)(ptrdiff_t)p;
    }

    void*
    user_data() const
    {
      return user_data_;
    }

    void
    user_data(void* v)
    {
      user_data_ = v;
    }

    long
    argument() const
    {
      return (long)(ptrdiff_t)user_data_;
    }

    void
    argument(long v)
    {
      user_data_ = (void*)(ptrdiff_t)v;
    }

    Fl_When
    when() const
    {
      return (Fl_When)when_;
    }

    void
    when(unsigned char const i)
    {
      when_ = static_cast<Fl_When>(i);
    }

    unsigned int
    visible() const
    {
      return !(flags_ & INVISIBLE);
    }

    int visible_r() const;

    virtual void show();

    virtual void hide();

    void
    set_visible()
    {
      unsigned bits = (static_cast<unsigned int>(flags_) & (~INVISIBLE));
      flags_ = static_cast<Fl_Flags>(bits);
    }

    void
    clear_visible()
    {
      set_flag(INVISIBLE);
    }

    unsigned int
    active() const
    {
      return !(flags_ & INACTIVE);
    }

    bool active_r() const;

    void activate();

    void deactivate();

    unsigned int
    output() const
    {
      return (flags_ & OUTPUT);
    }

    void
    set_output()
    {
      set_flag(OUTPUT);
    }

    void
    clear_output()
    {
      clear_flag(OUTPUT);
    }

    unsigned int
    takesevents() const
    {
      return !(flags_ & (INACTIVE | INVISIBLE | OUTPUT));
    }

    unsigned int
    changed() const
    {
      return flags_ & CHANGED;
    }

    void
    set_changed()
    {
      set_flag(CHANGED);
    }

    void
    clear_changed()
    {
      clear_flag(CHANGED);
    }

    void
    clear_active()
    {
      set_flag(INACTIVE);
    }

    void
    set_active()
    {
      clear_flag(INACTIVE);
    }

    int take_focus();

    void
    set_visible_focus()
    {
      set_flag(VISIBLE_FOCUS);
    }

    void
    clear_visible_focus()
    {
      clear_flag(VISIBLE_FOCUS);
    }

    void
    visible_focus(bool const v)
    {
      if (v)
      {
        set_visible_focus();
      }

      else
      {
        clear_visible_focus();
      }
    }

    unsigned int
    visible_focus()
    {
      return flags_ & VISIBLE_FOCUS;
    }

    static void default_callback(Fl_Widget* widget, void* data);

    void
    do_callback()
    {
      do_callback(this, user_data_);
    }

    void
    do_callback(Fl_Widget* widget, long arg)
    {
      do_callback(widget, (void*)(ptrdiff_t)arg);
    }

    void do_callback(Fl_Widget* widget, void* arg = 0);

    int test_shortcut();

    static unsigned int label_shortcut(const unsigned char* t);

    static int test_shortcut(const unsigned char*, const bool require_alt = false);

    int contains(const Fl_Widget* w) const ;

    int
    inside(const Fl_Widget* wgt) const
    {
      return wgt ? wgt->contains(this) : 0;
    }

    void redraw();

    void redraw_label();

    void draw_label() const;

    void
    draw_label(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y) const;

    void draw_label(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      enum Fl_Label::Fl_Align const i_align,
      enum Fl::foreground const i_fcolor,
      enum Fl::background const i_bcolor,
      enum Fl::foreground const i_hcolor) const;

    unsigned char
    damage() const
    {
      return damage_;
    }

    void
    clear_damage(unsigned char const c = 0)
    {
      damage_ = static_cast<Fl_Damage>(c);
    }

    void damage(unsigned char const c);

    void damage(unsigned char const c, int const x, int const y,
                unsigned int const w, unsigned int const h);

    Fl_Window* window() const ;

    Fl_Window* top_window() const;

    Fl_Window* top_window_offset(int& xoff, int& yoff) const;

    virtual Fl_Group*
    as_group()
    {
      return 0;
    }

    virtual Fl_Window*
    as_window()
    {
      return 0;
    }

    int
    use_accents_menu()
    {
      return flags() & MAC_USE_ACCENTS_MENU;
    }

};

#define FL_RESERVED_TYPE 100

#endif
