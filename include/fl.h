// fl.h
//
// Main header file for the Fast Light Tool Kit (FLTK).
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
#ifndef Fl_H
#  define Fl_H

#  ifdef check
#    undef check
#  endif

#  ifdef BSD
#    undef BSD
#  endif

#include <string.h>
#include "widget.h"

class Fl_Window;
class Fl_Screen_Driver;
class Fl_System_Driver;

extern const char* fl_local_alt;
extern const char* fl_local_ctrl;
extern const char* fl_local_meta;
extern const char* fl_local_shift;

typedef void (Fl_Label_Draw_F)(const Fl_Label* label, int x, int y, int w,
                               int h, enum Fl_Label::Fl_Align align);
typedef void (Fl_Label_Measure_F)(const Fl_Label* label, int& width,
                                  int& height);

typedef void (Fl_Box_Draw_F)(
  int const i_pos_x,
  int const i_pos_y,
  int const i_len_x,
  int const i_len_y,
  Fl::background const bcolor);

typedef void (*Fl_Timeout_Handler)(void* data);
typedef void (*Fl_Awake_Handler)(void* data);
typedef void (*Fl_Idle_Handler)(void* data);
typedef void (*Fl_Old_Idle_Handler)();
typedef bool (*Fl_Event_Handler)(Fl_Event const event);
typedef int (*Fl_System_Handler)(void* event, void* data);
typedef void (*Fl_Abort_Handler)(const char* format, ...);
typedef void (*Fl_Atclose_Handler)(Fl_Window* window, void* data);
typedef int (*Fl_Args_Handler)(int argc, char** argv, int& i);
typedef int (*Fl_Event_Dispatch)(int event, Fl_Window* w);
typedef void (*Fl_Clipboard_Notify_Handler)(int source, void* data);

namespace Fl
{

  extern int program_should_quit_;
  extern char const* const clipboard_plain_text;
  extern Fl_Screen_Driver* screen_driver();
  extern Fl_System_Driver* system_driver();
  void reset_marked_text();
  void insertion_point_location(int x, int y, int height);
  extern Fl_Event e_number;
  extern int e_x;
  extern int e_y;
  extern int e_x_root;
  extern int e_y_root;
  extern int e_dx;
  extern int e_dy;
  extern int e_state;
  extern int e_clicks;
  extern int e_is_click;
  extern int e_keysym;
  extern unsigned char* e_text;
  extern int e_length;
  extern void* e_clipboard_data;
  extern const unsigned char* e_clipboard_type;
  extern Fl_Event_Dispatch e_dispatch;
  extern Fl_Widget* belowmouse_;
  extern Fl_Widget* pushed_;
  extern Fl_Widget* focus_;
  extern int damage_;
  extern Fl_Widget* selection_owner_;
  extern Fl_Window* modal_;
  extern Fl_Window* grab_;
  extern int compose_state;

  void call_screen_init();

  inline void
  damage(int d)
  {
    damage_ = d;
  }

  typedef enum
  {
    OPTION_ARROW_FOCUS = 0,
    OPTION_VISIBLE_FOCUS,
    OPTION_DND_TEXT,
    OPTION_SHOW_TOOLTIPS,
    OPTION_FNFC_USES_GTK,
    OPTION_LAST
  } Fl_Option;

  extern unsigned char options_[OPTION_LAST];
  extern unsigned char options_read_;

  bool option(Fl_Option opt);
  void option(Fl_Option opt, bool val);
  extern void (*idle)();
  extern Fl_Awake_Handler* awake_ring_;
  extern void** awake_data_;
  extern int awake_ring_size_;
  extern int awake_ring_head_;
  extern int awake_ring_tail_;
  extern int e_original_keysym;
  extern int scrollbar_size_;
  int add_awake_handler_(Fl_Awake_Handler, void*);
  int get_awake_handler_(Fl_Awake_Handler&, void*&);

  int arg(int argc, char** argv, int& i);
  int args(int argc, char** argv, int& i, Fl_Args_Handler cb = 0);
  void args(int argc, char** argv);
  extern const char* const help;
  void display(const unsigned char*);
  int visual(int);
  int gl_visual(int, int* alist = 0);
  void own_colormap();
  void get_system_colors();

  int scrollbar_size();
  void scrollbar_size(int W);

  int wait();
  double wait(double time);
  int check();
  int ready();
  int run();

  inline int
  program_should_quit()
  {
    return program_should_quit_;
  }

  inline void
  program_should_quit(int should_i)
  {
    program_should_quit_ = should_i;
  }

  Fl_Widget* readqueue();
  void add_timeout(double t, Fl_Timeout_Handler, void* = 0);
  void repeat_timeout(double t, Fl_Timeout_Handler, void* = 0);
  int  has_timeout(Fl_Timeout_Handler, void* = 0);
  void remove_timeout(Fl_Timeout_Handler, void* = 0);
  void add_check(Fl_Timeout_Handler, void* = 0);
  int  has_check(Fl_Timeout_Handler, void* = 0);
  void remove_check(Fl_Timeout_Handler, void* = 0);
  void run_checks();
  void add_idle(Fl_Idle_Handler cb, void* data = 0);
  int  has_idle(Fl_Idle_Handler cb, void* data = 0);
  void remove_idle(Fl_Idle_Handler cb, void* data = 0);

  inline int
  damage()
  {
    return damage_;
  }

  void redraw();
  void flush();
  extern void (*warning)(const char*, ...);
  extern void (*error)(const char*, ...);
  extern void (*fatal)(const char*, ...);
  Fl_Window* first_window();
  void first_window(Fl_Window*);
  Fl_Window* next_window(const Fl_Window*);

  inline Fl_Window*
  modal()
  {
    return modal_;
  }

  inline Fl_Window*
  grab()
  {
    return grab_;
  }

  void grab(Fl_Window*);

  inline Fl_Event
  event()
  {
    return e_number;
  }

  inline int
  event_x()
  {
    return e_x;
  }

  inline int
  event_y()
  {
    return e_y;
  }

  inline int
  event_x_root()
  {
    return e_x_root;
  }

  inline int
  event_y_root()
  {
    return e_y_root;
  }

  inline int
  event_dx()
  {
    return e_dx;
  }

  inline int
  event_dy()
  {
    return e_dy;
  }

  inline int
  event_clicks()
  {
    return e_clicks;
  }

  inline void
  event_clicks(int i)
  {
    e_clicks = i;
  }

  inline int
  event_is_click()
  {
    return e_is_click;
  }

  inline void
  event_is_click(int i)
  {
    e_is_click = i;
  }

  inline int
  event_button()
  {
    return e_keysym - FL_Button;
  }

  inline int
  event_state()
  {
    return e_state;
  }

  inline int
  event_state(int mask)
  {
    return e_state & mask;
  }

  inline int
  event_key()
  {
    return e_keysym;
  }

  inline int
  event_original_key()
  {
    return e_original_keysym;
  }

  int event_key(int key);
  int get_key(int key);

  inline const unsigned char*
  event_text()
  {
    return e_text;
  }

  inline int
  event_length()
  {
    return e_length;
  }

  inline void*
  event_clipboard()
  {
    return e_clipboard_data;
  }

  inline const unsigned char*
  event_clipboard_type()
  {
    return e_clipboard_type;
  }

  int compose(int& del);
  void compose_reset();
  int event_inside(int, int, int, int);
  int event_inside(const Fl_Widget*);
  int test_shortcut(Fl_Shortcut);
  bool handle(Fl_Event, Fl_Window*);
  bool handle_(Fl_Event, Fl_Window*);

  inline Fl_Widget*
  belowmouse()
  {
    return belowmouse_;
  }

  void belowmouse(Fl_Widget*);

  inline Fl_Widget*
  pushed()
  {
    return pushed_;
  }

  void pushed(Fl_Widget*);

  inline Fl_Widget*
  focus()
  {
    return focus_;
  }

  void focus(Fl_Widget*);
  void add_handler(Fl_Event_Handler h);
  void remove_handler(Fl_Event_Handler h);
  void add_system_handler(Fl_System_Handler h, void* data);
  void remove_system_handler(Fl_System_Handler h);
  void event_dispatch(Fl_Event_Dispatch d);
  Fl_Event_Dispatch event_dispatch();

  void copy(const unsigned char* stuff, int len, int destination = 0,
            const char* type = Fl::clipboard_plain_text);

  void paste(Fl_Widget& receiver, int source,
             const char* type = Fl::clipboard_plain_text);


  void add_clipboard_notify(Fl_Clipboard_Notify_Handler h, void* data = 0);
  void remove_clipboard_notify(Fl_Clipboard_Notify_Handler h);
  int clipboard_contains(const char* type);
  int dnd();

  inline Fl_Widget*
  selection_owner()
  {
    return Fl::selection_owner_;
  }

  void selection_owner(Fl_Widget*);
  void selection(Fl_Widget& owner, const unsigned char*, int len);
  void paste(Fl_Widget& receiver);
  int x();
  int y();
  int w();
  int h();

  int screen_count();
  void screen_xywh(int& X, int& Y, int& W, int& H);
  void screen_xywh(int& X, int& Y, int& W, int& H, int mx, int my);
  void screen_xywh(int& X, int& Y, int& W, int& H, int n);
  void screen_xywh(int& X, int& Y, int& W, int& H, int mx, int my, int mw,
                   int mh);
  int screen_num(int x, int y);
  int screen_num(int x, int y, int w, int h);
  void screen_dpi(float& h, float& v, int n = 0);
  void screen_work_area(int& X, int& Y, int& W, int& H, int mx, int my);
  void screen_work_area(int& X, int& Y, int& W, int& H, int n);
  void screen_work_area(int& X, int& Y, int& W, int& H);
  float screen_scale(int n);
  void set_labeltype(Fl_Label::Fl_LabelType, Fl_Label_Draw_F*,
                     Fl_Label_Measure_F*);
  void set_labeltype(Fl_Label::Fl_LabelType, Fl_Label::Fl_LabelType from);
  Fl_Box_Draw_F* get_boxtype(Fl_Boxtype);
  void set_boxtype(Fl_Boxtype, Fl_Box_Draw_F*, unsigned char, unsigned char,
                   unsigned char, unsigned char);
  void set_boxtype(Fl_Boxtype, Fl_Boxtype from);
  int box_dx(Fl_Boxtype);
  int box_dy(Fl_Boxtype);
  int box_dw(Fl_Boxtype);
  int box_dh(Fl_Boxtype);

  int draw_box_active();

  inline void
  set_abort(Fl_Abort_Handler f)
  {
    fatal = f;
  }

  extern void (*atclose)(Fl_Window*, void*);
  void default_atclose(Fl_Window*, void*);

  inline void
  set_atclose(Fl_Atclose_Handler f)
  {
    atclose = f;
  }

  inline int
  event_shift()
  {
    return e_state & FL_SHIFT;
  }

  inline int
  event_ctrl()
  {
    return e_state & FL_CTRL;
  }

  inline int
  event_command()
  {
    return e_state & FL_COMMAND;
  }

  inline int
  event_alt()
  {
    return e_state & FL_ALT;
  }

  inline int
  event_buttons()
  {
    return e_state & 0x7f000000;
  }

  inline int
  event_button1()
  {
    return e_state & FL_BUTTON1;
  }

  inline int
  event_button2()
  {
    return e_state & FL_BUTTON2;
  }

  inline int
  event_button3()
  {
    return e_state & FL_BUTTON3;
  }

  inline void
  set_idle(Fl_Old_Idle_Handler cb)
  {
    idle = cb;
  }

  inline void
  grab(Fl_Window& win)
  {
    grab(&win);
  }

  inline void
  release()
  {
    grab(0);
  }

  inline void
  visible_focus(int v)
  {
    option(OPTION_VISIBLE_FOCUS, (v != 0));
  }

  inline int
  visible_focus()
  {
    return option(OPTION_VISIBLE_FOCUS);
  }

  inline void
  dnd_text_ops(int v)
  {
    option(OPTION_DND_TEXT, (v != 0));
  }

  inline int
  dnd_text_ops()
  {
    return option(OPTION_DND_TEXT);
  }

  int lock();
  void unlock();
  void awake(void* message = 0);
  int awake(Fl_Awake_Handler cb, void* message = 0);
  void* thread_message();
  void delete_widget(Fl_Widget* w);
  void do_widget_deletion();
  void watch_widget_pointer(Fl_Widget*& w);
  void release_widget_pointer(Fl_Widget*& w);
  void clear_widget_pointer(Fl_Widget const* w);
};

class Fl_Widget_Tracker
{

    Fl_Widget* wp_;

  public:

    Fl_Widget_Tracker(Fl_Widget* wi);
    ~Fl_Widget_Tracker();

    Fl_Widget*
    widget()
    {
      return wp_;
    }
    int
    deleted()
    {
      return wp_ == 0;
    }
    int
    exists()
    {
      return wp_ != 0;
    }

};

#endif // !Fl_H
