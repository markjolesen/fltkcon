// menu.cxx
//
// Menu code for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2015 by Bill Spitzak and others.
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
#include <stdio.h>
#include "menu_.h"
#include "drvwin.h"
#include "drvsys.h"
#include "fl.h"
#include "fl_clip.h"
#include "fl_draw.h"
#include "fl_rend.h"
#include "fl_skin.h"
#include "flstring.h"
#include "fl_utf8.h"
#include "winmenu.h"

int
Fl_Menu_Item::size() const
{
  const Fl_Menu_Item* m = this;
  int nest = 0;

  for (;;)
  {
    if (!m->text)
    {
      if (!nest) return (int) (m - this + 1);

      nest--;
    }

    else if (m->flags & FL_SUBMENU)
    {
      nest++;
    }

    m++;
  }
}

static const Fl_Menu_Item*
next_visible_or_not(const Fl_Menu_Item* m)
{
  int nest = 0;

  do
  {
    if (!m->text)
    {
      if (!nest) return m;

      nest--;
    }

    else if (m->flags & FL_SUBMENU)
    {
      nest++;
    }

    m++;
  }
  while (nest);

  return m;
}

const Fl_Menu_Item*
Fl_Menu_Item::next(int n) const
{
  if (n < 0) return 0;

  const Fl_Menu_Item* m = this;

  if (!m->visible()) n++;

  while (n)
  {
    m = next_visible_or_not(m);

    if (m->visible() || !m->text) n--;
  }

  return m;
}

static const Fl_Menu_* button = 0;
#define LEADING 4

class menutitle : public Fl_Menu_Window
{
    void draw();
  public:
    const Fl_Menu_Item* menu;
    menutitle(int X, int Y, int W, int H, const Fl_Menu_Item*);
};

class menuwindow : public Fl_Menu_Window
{
    void draw();

    void drawentry(
      Fl_Menu_Item const*,
      int const i,
      bool const erase);

    bool handle_part1(Fl_Event const);
    bool handle_part2(Fl_Event const e, int ret);
  public:
    menutitle* title;
    bool handle(Fl_Event const);
    int itemheight;
    int numitems;
    int selected;
    int drawn_selected;
    int shortcutWidth;
    const Fl_Menu_Item* menu;
    const Fl_Menu_Item** table;
    unsigned int offset_y;
    menuwindow(const Fl_Menu_Item* m, int X, int Y, int W, int H,
               const Fl_Menu_Item* picked, const Fl_Menu_Item* title,
               int menubar = 0, int menubar_title = 0, int right_edge = 0);
    ~menuwindow();
    void set_selected(int);
    int find_selected(int mx, int my);
    int titlex(int);
    void autoscroll(int);
    void position(int x, int y);
    int is_inside(int x, int y);
};

extern char fl_draw_shortcut;

int
Fl_Menu_Item::measure(int* hp, const Fl_Menu_* m) const
{
  Fl_Label l;
  l.value   = text;
  l.type    = (Fl_Label::Fl_LabelType)labeltype_;
  fl_draw_shortcut = 1;
  int w = 0;
  int h = 0;
  l.measure(w, hp ? *hp : h);
  fl_draw_shortcut = 0;

  if (flags & (FL_MENU_TOGGLE | FL_MENU_RADIO)) w += 4;

  return w;
}

void
Fl_Menu_Item::draw(int x, int y, int w, int h, const Fl_Menu_* m,
                   int selected) const
{
  unsigned char ind[5];
  unsigned char buf[81];
  Fl_Label l;
  l.value = text;
  l.type = (Fl_Label::Fl_LabelType)labeltype_;

  if (!fl_draw_shortcut) fl_draw_shortcut = 1;

  enum Fl::foreground fcolor = m->skin_.menu_fcolor;
  enum Fl::foreground hcolor = m->skin_.highlight_fcolor;

  if (active())
  {
    if (selected)
    {
      fcolor = m->skin_.highlight_fcolor;
    }
  }

  else
  {
    fcolor = m->skin_.disabled_fcolor;
    hcolor = m->skin_.disabled_fcolor;
  }

  if ((FL_MENU_TOGGLE | FL_MENU_RADIO) & flags)
  {
    ind[3] = ' ';
    ind[4] = 0;

    if (FL_MENU_RADIO & flags)
    {
      ind[0] = '(';
      ind[1] = ((value()) ? '*' : ' ');
      ind[2] = ')';
    }

    else
    {
      ind[0] = '[';
      ind[1] = ((value()) ? 'x' : ' ');
      ind[2] = ']';
    }

    snprintf((char*)buf, sizeof(buf), "%s%s", (char*)ind, text);
    l.value = buf;
  }

  l.draw(x, y, w, h, Fl_Label::FL_ALIGN_LEFT,
         fcolor, m->skin_.menu_bcolor, hcolor);

  fl_draw_shortcut = 0;

}

menutitle::menutitle(int X, int Y, int W, int H, const Fl_Menu_Item* L) :
  Fl_Menu_Window(X, Y, W, H, 0)
{
  end();
  type(FL_MENU_TITLE_WINDOW);
  set_modal();
  clear_border();
  set_menu_window();
  menu = L;
  Fl_Widget::skin_.normal_fcolor = Fl_Widget::skin_.highlight_fcolor;
}

menuwindow::menuwindow(const Fl_Menu_Item* m, int X, int Y, int Wp, int Hp,
                       const Fl_Menu_Item* picked, const Fl_Menu_Item* t,
                       int menubar, int menubar_title, int right_edge)
  : Fl_Menu_Window(X, Y, Wp, Hp, 0)
{
  offset_y = 0;
  type(FL_MENU_WINDOW);
  int scr_x, scr_y, scr_w, scr_h;
  int tx = X, ty = Y;

  Fl::screen_work_area(scr_x, scr_y, scr_w, scr_h);

  if (!right_edge || right_edge > scr_x + scr_w) right_edge = scr_x + scr_w;

  end();
  set_modal();
  set_menu_window();
  menu = m;

  if (m) m = m->first();

  drawn_selected = -1;
  selected = -1;

  numitems = 0;
  int j = 0;

  if (m)
  {
    Fl_Menu_Item const* m1;

    for (m1 = m; ; m1 = m1->next(), j++)
    {
      if (!m1->text) break;

      if (m1->visible())
      {
        numitems++;

        if (FL_MENU_DIVIDER & m1->flags)
        {
          numitems++;
        }

        if (picked)
        {
          if (m1 == picked)
          {
            selected = j;
            picked = 0;
          }

          else if (m1 > picked)
          {
            selected = j - 1;
            picked = 0;
            Wp = Hp = 0;
          }
        }
      }
    }

    if (numitems)
    {
      table = (const Fl_Menu_Item**)calloc(sizeof(Fl_Menu_Item*), numitems);

      for (j = 0, m1 = m; ; m1 = m1->next(), j++)
      {
        if (!m1->text) break;

        if (m1->visible())
        {
          table[j] = m1;

          if (FL_MENU_DIVIDER & m1->flags)
          {
            j++;
          }
        }
      }
    }
  }

  if (menubar)
  {
    itemheight = 0;
    title = 0;
    return;
  }

  itemheight = 1;

  int hotKeysw = 0;
  int hotModsw = 0;
  int Wtitle = 0;
  int Htitle = 0;

  if (t) Wtitle = t->measure(&Htitle, button) + 1;

  int W = 0;

  if (m) for (; m->text; m = m->next())
    {
      int hh;
      int w1 = m->measure(&hh, button);

      if (m->flags & (FL_SUBMENU | FL_SUBMENU_POINTER))
        w1 += 1;

      if (w1 > W) W = w1;

      if (m->shortcut_)
      {
        const char* k, *s = fl_shortcut_label(m->shortcut_, &k);

        if (fl_utf_nb_char((const unsigned char*)k, (int) strlen(k)) <= 4)
        {
          w1 = (int) (k - s);

          if (w1 > hotModsw) hotModsw = w1;

          w1 = strlen(k) + 1;

          if (w1 > hotKeysw) hotKeysw = w1;
        }

        else
        {
          w1 = strlen(s) + 1;

          if (w1 > (hotModsw + hotKeysw))
          {
            hotModsw = w1 - hotKeysw;
          }
        }
      }
    }

  shortcutWidth = hotKeysw;

  if (selected >= 0 && !Wp) X -= W / 2;

  int BW = 1;
  W += hotKeysw + hotModsw + 2 * BW /*+ 7*/;

  if (Wp > W) W = Wp;

  if (Wtitle > W) W = Wtitle;

  if (X < scr_x) X = scr_x;

  if (X > scr_x + scr_w - W) X = scr_x + scr_w - W;

  x(X);
  w(W);
  h_ = numitems;

  if (selected >= 0)
  {
    Y = Y + (Hp - itemheight) / 2 - selected * itemheight - BW;
  }

  else
  {
    Y = Y + Hp;

    if ((int)(Y + h()) > (int)(scr_y + scr_h) && (int)(Y - h()) >= (int)scr_y)
    {
      if (Hp > 1)
      {
        Y = Y - Hp - h();
      }

      else if (t)
      {
        Y = Y - itemheight - h();
      }

      else
      {
        Y = Y - h() + itemheight;
      }
    }
  }

  if (m) y(Y + 1);

  else
  {
    y(Y - 2);
    w(1);
    h(1);
  }

  if (scr_h <= (int)(y_ + h_))
  {
    y_ -= (y_ + h_ - scr_h + 1);
  }

  if (2 >= y_)
  {
    y_ = 3;
    h_ = scr_h - 4;
  }

  if (t)
  {
    if (menubar_title)
    {
      title = new menutitle(tx, ty - 1, Wtitle, 1, t);
    }

    else
    {
      int dy = 2;
      int ht = 1;
      title = new menutitle(X, Y - ht - dy, Wtitle, ht, t);
    }
  }

  else
  {
    title = 0;
  }


}

menuwindow::~menuwindow()
{
  hide();
  delete title;
}

void
menuwindow::position(int X, int Y)
{
  if (title)
  {
    title->position(X, title->y() + Y - y());
  }

  Fl_Menu_Window::position(X, Y);
}

void
menuwindow::autoscroll(int n)
{
  unsigned int old_offset = offset_y;

  do
  {

    if (n >= (int)offset_y && n < (int)(offset_y + h_ - 1))
    {
      break;
    }

    if (n < (int)offset_y)
    {
      for (; n < (int)offset_y; offset_y--);

      break;
    }

    for (; n > (int)(offset_y + h_ - 1); offset_y++);

  }
  while (0);

  if (old_offset != offset_y)
  {
    damage(Fl_Widget::FL_DAMAGE_ALL);
  }

  return;
}

void
menuwindow::drawentry(
  Fl_Menu_Item const* m,
  int const n,
  bool const erase)
{

  if (!m) return;

  int BW = 0;
  int xx = BW;
  int W = w();
  int ww = W - 2 * BW - 1;
  int yy = BW + n - offset_y;
  int hh = itemheight;

  m->draw(xx, yy, ww, hh, button, n == selected);

  if (m->submenu())
  {
    Fl::draw_putc((xx + ww - 2), yy, Fl::_skin_sym.right, 1,
                  Fl_Widget::skin_.menu_fcolor, Fl_Widget::skin_.menu_bcolor);
  }

  else if (m->shortcut_)
  {
    const char* k, *s = fl_shortcut_label(m->shortcut_, &k);

    if (fl_utf_nb_char((const unsigned char*)k, (int) strlen(k)) <= 4)
    {
      char* buf = (char*)malloc(k - s + 1);
      memcpy(buf, s, k - s);
      buf[k - s] = 0;
      fl_draw(
        (unsigned char const*)buf,
        xx,
        yy,
        ww - shortcutWidth,
        hh,
        Fl_Label::FL_ALIGN_RIGHT,
        Fl_Widget::skin_.menu_fcolor,
        Fl_Widget::skin_.menu_bcolor,
        Fl_Widget::skin_.highlight_fcolor);

      fl_draw(
        (unsigned char*)k,
        xx + ww - shortcutWidth,
        yy,
        shortcutWidth,
        hh,
        Fl_Label::FL_ALIGN_LEFT,
        Fl_Widget::skin_.menu_fcolor,
        Fl_Widget::skin_.menu_bcolor,
        Fl_Widget::skin_.highlight_fcolor);

      free(buf);
    }

    else
    {
      fl_draw(
        (unsigned char const*)s,
        xx,
        yy,
        ww - 4,
        hh,
        Fl_Label::FL_ALIGN_RIGHT,
        Fl_Widget::skin_.menu_fcolor,
        Fl_Widget::skin_.menu_bcolor,
        Fl_Widget::skin_.highlight_fcolor);
    }
  }

  if (m->flags & FL_MENU_DIVIDER)
  {
    if ((yy + 1) < (int)h_)
    {
      Fl::draw_fill(xx, yy + 1, ww + 1, 1, Fl::_skin_sym.hline,
                    Fl_Widget::skin_.menu_fcolor, Fl_Widget::skin_.menu_bcolor);
    }
  }

}

void
menutitle::draw()
{
  Fl_Window_Driver::driver(this)->draw_begin();
  menu->draw(0, 0, w(), h(), button, 2);
  Fl_Window_Driver::driver(this)->draw_end();
}

void
menuwindow::draw()
{
  const Fl_Menu_Item* m;

  Fl_Window_Driver::driver(this)->draw_begin();

  if (damage() != FL_DAMAGE_CHILD)
  {
    Fl::draw_fill(0, 0, w(), h(), 0x20,
                  Fl_Widget::skin_.menu_fcolor, Fl_Widget::skin_.menu_bcolor);

    if (menu)
    {
      unsigned int count = 0;
      int slot = offset_y;

      do
      {
        if (h_ <= count)
        {
          break;
        }

        if (numitems <= slot)
        {
          break;
        }

        m = table[slot];

        if (m)
        {
          drawentry(m, slot, 0);
        }

        count++;
        slot++;
      }
      while (1);
    }
  }

  else if (damage() & FL_DAMAGE_CHILD && selected != drawn_selected)
  {
    if (-1 != drawn_selected)
    {
      m = table[drawn_selected];

      if (m)
      {
        drawentry(m, drawn_selected, 1);
      }
    }

    if (-1 != selected)
    {
      m = table[selected];

      if (m)
      {
        drawentry(m, selected, 1);
      }
    }
  }

  drawn_selected = selected;
  Fl_Window_Driver::driver(this)->draw_end();

  return;
}

void
menuwindow::set_selected(int n)
{
  if (n != selected)
  {
    selected = n;
    damage(FL_DAMAGE_CHILD);
  }
}

int
menuwindow::find_selected(int mx, int my)
{

  if (!menu || !menu->text) return -1;

  mx -= x();
  my -= y();

  if (my < 0 || my >= (int)h()) return -1;

  if (!itemheight)
  {
    int xx = 1;
    int n = 0;
    const Fl_Menu_Item* m = menu ? menu->first() : 0;

    for (; ; m = m->next(), n++)
    {
      if (!m->text) return -1;

      xx += m->measure(0, button) + 1;

      if (xx > mx) break;
    }

    return n;
  }

  if (mx < 0 || mx > (int)w_) return -1;

  if (my < 0 || my >= numitems) return -1;

  return (my + offset_y);
}

int
menuwindow::titlex(int n)
{
  const Fl_Menu_Item* m;
  int xx = 1;

  for (m = menu->first(); n--; m = m->next()) xx += m->measure(0, button) + 1;

  return xx;
}

int
menuwindow::is_inside(int mx, int my)
{
  if ( mx < x_root() || mx >= (int)(x_root() + w()) ||
       my < y_root() || my >= (int)(y_root() + h()))
  {
    return 0;
  }

  if (itemheight == 0 && find_selected(mx, my) == -1)
  {
    return 0;
  }

  return 1;
}

#define INITIAL_STATE 0
#define PUSH_STATE 1
#define DONE_STATE 2
#define MENU_PUSH_STATE 3

struct menustate
{
  const Fl_Menu_Item* current_item;
  int menu_number;
  int item_number;
  menuwindow* p[20];
  int nummenus;
  int menubar;
  int state;
  menuwindow* fakemenu;
  int is_inside(int mx, int my);
};
static menustate* p = 0;

int
menustate::is_inside(int mx, int my)
{
  int i;

  for (i = nummenus - 1; i >= 0; i--)
  {
    if (p[i]->is_inside(mx, my))
      return 1;
  }

  return 0;
}

static inline void
setitem(const Fl_Menu_Item* i, int m, int n)
{
  p->current_item = i;
  p->menu_number = m;
  p->item_number = n;
}

static void
setitem(int m, int n)
{
  menustate& pp = *p;
  pp.current_item = (n >= 0) ? pp.p[m]->menu->next(n) : 0;
  pp.menu_number = m;
  pp.item_number = n;
}

static int
forward(int menu)
{
  menustate& pp = *p;

  if (menu == -1)
    menu = 0;

  menuwindow& m = *(pp.p[menu]);
  int item = (menu == pp.menu_number) ? pp.item_number : m.selected;

  while (++item < m.numitems)
  {
    const Fl_Menu_Item* m1 = m.table[item];

    if (m1)
    {
      if (m1->activevisible())
      {
        setitem(m1, menu, item);
        return 1;
      }
    }
  }

  return 0;
}

static int
backward(int menu)
{
  menustate& pp = *p;
  menuwindow& m = *(pp.p[menu]);
  int item = (menu == pp.menu_number) ? pp.item_number : m.selected;

  if (item < 0) item = m.numitems;

  while (--item >= 0)
  {
    const Fl_Menu_Item* m1 = m.table[item];

    if (m1)
    {
      if (m1->activevisible())
      {
        setitem(m1, menu, item);
        return 1;
      }
    }
  }

  if (m.offset_y)
  {
    m.offset_y = 0;
    pp.current_item = 0;
    pp.item_number = 0;
    m.damage(Fl_Widget::FL_DAMAGE_ALL);
    return 1;
  }

  return 0;
}

bool
menuwindow::handle(Fl_Event const e)
{
  static int use_part2 = Fl::system_driver()->need_menu_handle_part2();
  int ret = handle_part1(e);

  if (use_part2) ret = handle_part2(e, ret);

  return ret;
}

bool
menuwindow::handle_part2(Fl_Event const e, int ret)
{
  menustate& pp = *p;

  if (pp.state == DONE_STATE)
  {
    hide();

    if (pp.fakemenu)
    {
      pp.fakemenu->hide();

      if (pp.fakemenu->title)
        pp.fakemenu->title->hide();
    }

    int i = pp.nummenus;

    while (i > 0)
    {
      menuwindow* mw = pp.p[--i];

      if (mw)
      {
        mw->hide();

        if (mw->title)
          mw->title->hide();
      }
    }
  }

  return ret;
}

bool
menuwindow::handle_part1(Fl_Event const e)
{
  menustate& pp = *p;

  switch (e)
  {
    case FL_KEYBOARD:
      switch (Fl::event_key())
      {
        case FL_BackSpace:
        BACKTAB:
          if (!backward(pp.menu_number))
          {
            pp.item_number = -1;
            backward(pp.menu_number);
          }

          return 1;

        case FL_Up:
          if (pp.menubar && pp.menu_number == 0)
          {
          }
          else if (backward(pp.menu_number))
          {
          }
          else if (pp.menubar && pp.menu_number == 1)
          {
            setitem(0, pp.p[0]->selected);
          }

          return 1;

        case FL_Tab:
          if (Fl::event_shift()) goto BACKTAB;

        case FL_Down:
          if (pp.menu_number || !pp.menubar)
          {
            if (!forward(pp.menu_number) && Fl::event_key() == FL_Tab)
            {
              pp.item_number = -1;
              forward(pp.menu_number);
            }
          }

          else if (pp.menu_number < pp.nummenus - 1)
          {
            forward(pp.menu_number + 1);
          }

          return 1;

        case FL_Right:
          if (pp.menubar && (pp.menu_number <= 0 || (pp.menu_number == 1
                                                     && pp.nummenus == 2)))
            forward(0);

          else if (pp.menu_number < pp.nummenus - 1) forward(pp.menu_number + 1);

          return 1;

        case FL_Left:
          if (pp.menubar && pp.menu_number <= 1) backward(0);

          else if (pp.menu_number > 0)
            setitem(pp.menu_number - 1, pp.p[pp.menu_number - 1]->selected);

          return 1;

        case FL_Enter:
        case FL_KP_Enter:
        case ' ':
          pp.state = DONE_STATE;
          return 1;

        case FL_Escape:
          setitem(0, -1, 0);
          pp.state = DONE_STATE;
          return 1;
      }

      break;

    case FL_SHORTCUT:
    {
      for (int mymenu = pp.nummenus; mymenu--;)
      {
        menuwindow& mw = *(pp.p[mymenu]);
        int item;
        const Fl_Menu_Item* m = mw.menu->find_shortcut(&item);

        if (m)
        {
          setitem(m, mymenu, item);

          if (!m->submenu()) pp.state = DONE_STATE;

          return 1;
        }
      }
    }
    break;

    case FL_MOVE:
    {
      static int use_part1_extra =
        Fl::system_driver()->need_menu_handle_part1_extra();

      if (use_part1_extra && pp.state == DONE_STATE)
      {
        return 1;
      }
    }

    case FL_ENTER:
    case FL_PUSH:
    case FL_DRAG:
    {
      int mx = Fl::event_x_root();
      int my = Fl::event_y_root();
      int item = 0;
      int mymenu = pp.nummenus - 1;

      if ((!pp.menubar || mymenu) && !pp.is_inside(mx, my))
      {
        setitem(0, -1, 0);

        if (e == FL_PUSH)
          pp.state = DONE_STATE;

        return 1;
      }

      for (mymenu = pp.nummenus - 1; ; mymenu--)
      {
        item = pp.p[mymenu]->find_selected(mx, my);

        if (item >= 0)
          break;

        if (mymenu <= 0)
        {
          if (pp.menu_number == -1 && e == FL_PUSH)
          {
            pp.state = DONE_STATE;
            return 1;
          }

          if (pp.current_item && pp.menu_number == 0 && !pp.current_item->submenu())
          {
            if (e == FL_PUSH)
              pp.state = DONE_STATE;

            setitem(0, -1, 0);
            return 1;
          }

          return 0;
        }
      }

      menuwindow* mw = pp.p[mymenu];

      if (mw->itemheight && (1 < mw->numitems))
      {
        if (pp.item_number == item)
        {
          if (item)
          {
            unsigned slot = (item - mw->offset_y);

            if (0 == slot)
            {
              do
              {
                item--;

                if (0 == item || mw->table[item])
                {
                  break;
                }
              }
              while (1);
            }

            else if (slot >= (mw->h_ - 1))
            {
              while (mw->numitems > (item + 1))
              {
                item++;

                if (mw->table[item])
                {
                  break;
                }
              }
            }
          }
        }

        setitem(mw->table[item], mymenu, item);
      }

      else
      {
        setitem(mymenu, item);
      }

      if (mw->table[item])
      {
        if (e == FL_PUSH)
        {
          if (pp.current_item && pp.current_item->submenu()
              && item != pp.p[mymenu]->selected
              && !pp.current_item->callback_)
            pp.state = MENU_PUSH_STATE;

          else
            pp.state = PUSH_STATE;
        }
      }
    }

    return 1;

    case FL_RELEASE:

      if (   !Fl::event_is_click()
             || pp.state == PUSH_STATE
             || (pp.menubar && pp.current_item && !pp.current_item->submenu()) // button
         )
      {
        if (!pp.current_item || pp.current_item->activevisible())
          pp.state = DONE_STATE;
      }

      return 1;
    default:
      break;
  }

  return Fl_Window::handle(e);
}

const Fl_Menu_Item*
Fl_Menu_Item::pulldown(
  int X, int Y, int W, int H,
  const Fl_Menu_Item* initial_item,
  const Fl_Menu_* pbutton,
  const Fl_Menu_Item* t,
  int menubar) const
{
  Fl_Group::current(0);

  button = pbutton;

  if (pbutton && pbutton->window())
  {
    for (Fl_Window* w = pbutton->window(); w; w = w->window())
    {
      X += w->x();
      Y += w->y();
    }
  }

  else
  {
    X += Fl::event_x_root() - Fl::event_x();
    Y += Fl::event_y_root() - Fl::event_y();
  }

  menuwindow mw(this, X, Y, W, H, initial_item, t, menubar);
  Fl::grab(mw);
  menustate pp;
  p = &pp;
  pp.p[0] = &mw;
  pp.nummenus = 1;
  pp.menubar = menubar;
  pp.state = INITIAL_STATE;
  pp.fakemenu = 0;

  if (initial_item && mw.selected >= 0)
  {
    setitem(0, mw.selected);
    goto STARTUP;
  }

  pp.current_item = 0;
  pp.menu_number = 0;
  pp.item_number = -1;

  if (menubar)
  {
    if (!mw.handle(FL_DRAG))
    {
      Fl::grab(0);
      return 0;
    }
  }

  initial_item = pp.current_item;

  if (initial_item) goto STARTUP;

  for (;;)
  {

    {
      for (int k = menubar; k < pp.nummenus; k++)
      {
        if (!pp.p[k]->shown())
        {
          if (pp.p[k]->title) pp.p[k]->title->show();

          pp.p[k]->show();
        }
      }
    }

    {
      const Fl_Menu_Item* oldi = pp.current_item;
      Fl::wait();

      if (pp.state == DONE_STATE) break;

      if (pp.current_item == oldi) continue;
    }

    if (pp.fakemenu)
    {
      delete pp.fakemenu;
      pp.fakemenu = 0;
    }

    if (!pp.current_item)
    {
      pp.p[pp.nummenus - 1]->set_selected(-1);
      continue;
    }

    if (pp.fakemenu)
    {
      delete pp.fakemenu;
      pp.fakemenu = 0;
    }

    initial_item = 0;
    pp.p[pp.menu_number]->autoscroll(pp.item_number);

  STARTUP:
    menuwindow& cw = *pp.p[pp.menu_number];
    const Fl_Menu_Item* m = pp.current_item;

    if (!m->activevisible())
    {
      cw.set_selected(-1);
      initial_item = 0;
      continue;
    }

    cw.set_selected(pp.item_number);

    if (m == initial_item) initial_item = 0;

    if (m->submenu())
    {
      const Fl_Menu_Item* title = m;
      const Fl_Menu_Item* menutable;

      if (m->flags & FL_SUBMENU) menutable = m + 1;

      else menutable = (Fl_Menu_Item*)(m)->user_data_;

      int nX, nY;

      if (!pp.menu_number && pp.menubar)
      {
        nX = cw.x() + cw.titlex(pp.item_number);
        nY = cw.y() + cw.h();
        initial_item = 0;
      }

      else
      {
        nX = cw.x() + cw.w();
        nY = cw.y() + pp.item_number * cw.itemheight;
        title = 0;
      }

      if (initial_item)
      {
        menuwindow* n = new menuwindow(menutable, X, Y, W, H, initial_item, title, 0, 0,
                                       cw.x());
        pp.p[pp.nummenus++] = n;

        if (n->selected >= 0)
        {
          int dy = n->y() - nY;
          int dx = n->x() - nX;
          int waX, waY, waW, waH;
          Fl::screen_work_area(waX, waY, waW, waH, X, Y);

          for (int menu = 0; menu <= pp.menu_number; menu++)
          {
            menuwindow* tt = pp.p[menu];
            int nx = tt->x() + dx;

            if (nx < waX)
            {
              nx = waX;
              dx = -tt->x() + waX;
            }

            int ny = tt->y() + dy;

            if (ny < waY)
            {
              ny = waY;
              dy = -tt->y() + waY;
            }

            tt->position(nx, ny);
          }

          setitem(pp.nummenus - 1, n->selected);
          goto STARTUP;
        }
      }

      else if (pp.nummenus > pp.menu_number + 1 &&
               pp.p[pp.menu_number + 1]->menu == menutable)
      {
        while (pp.nummenus > pp.menu_number + 2) delete pp.p[--pp.nummenus];

        pp.p[pp.nummenus - 1]->set_selected(-1);
      }

      else
      {
        while (pp.nummenus > pp.menu_number + 1) delete pp.p[--pp.nummenus];

        pp.p[pp.nummenus++] = new menuwindow(menutable, nX, nY,
                                             title ? 1 : 0, 0, 0, title, 0, menubar,
                                             (title ? 0 : cw.x()) );
      }
    }

    else
    {
      while (pp.nummenus > pp.menu_number + 1) delete pp.p[--pp.nummenus];

      if (!pp.menu_number && pp.menubar)
      {
        pp.fakemenu = new menuwindow(0,
                                     cw.x() + cw.titlex(pp.item_number),
                                     cw.y() + cw.h(), 0, 0,
                                     0, m, 0, 1);
        pp.fakemenu->title->show();
      }
    }
  }

  const Fl_Menu_Item* m = pp.current_item;
  delete pp.fakemenu;

  while (pp.nummenus > 1) delete pp.p[--pp.nummenus];

  mw.hide();
  Fl::grab(0);
  return m;
}

const Fl_Menu_Item*
Fl_Menu_Item::popup(
  int X, int Y,
  const unsigned char* title,
  const Fl_Menu_Item* picked,
  const Fl_Menu_* button
) const
{
  static Fl_Menu_Item dummy;
  dummy.text = title;
  return pulldown(X, Y, 0, 0, picked, button, title ? &dummy : 0);
}

const Fl_Menu_Item*
Fl_Menu_Item::find_shortcut(int* ip, const bool require_alt) const
{
  const Fl_Menu_Item* m = this;

  if (m) for (int ii = 0; m->text; m = next_visible_or_not(m), ii++)
    {
      if (m->active())
      {
        if (Fl::test_shortcut(m->shortcut_)
            || Fl_Widget::test_shortcut(m->text, require_alt))
        {
          if (ip) *ip = ii;

          return m;
        }
      }
    }

  return 0;
}

const Fl_Menu_Item*
Fl_Menu_Item::test_shortcut() const
{
  const Fl_Menu_Item* m = this;
  const Fl_Menu_Item* ret = 0;

  if (m) for (; m->text; m = next_visible_or_not(m))
    {
      if (m->active())
      {
        if (Fl::test_shortcut(m->shortcut_)) return m;

        if (!ret && m->submenu())
        {
          const Fl_Menu_Item* s =
            (m->flags & FL_SUBMENU) ? m + 1 : (const Fl_Menu_Item*)m->user_data_;
          ret = s->test_shortcut();
        }
      }
    }

  return ret;
}
