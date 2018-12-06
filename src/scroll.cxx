// scroll.cxx
//
// Scroll widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 1998-2017 by Bill Spitzak and others.
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
#include "scroll.h"
#include "drvgr.h"
#include "fl.h"
#include "fl_clip.h"
#include "fl_draw.h"
#include "fl_rend.h"

void
Fl_Scroll::clear()
{
  remove(scrollbar);
  remove(hscrollbar);
  Fl_Group::clear();
  add(hscrollbar);
  add(scrollbar);
}

void
Fl_Scroll::fix_scrollbar_order()
{
  Fl_Widget** a = (Fl_Widget**)array();

  if (a[children() - 1] != &scrollbar)
  {
    int i, j;

    for (i = j = 0; j < children(); j++)
      if (a[j] != &hscrollbar && a[j] != &scrollbar) a[i++] = a[j];

    a[i++] = &hscrollbar;
    a[i++] = &scrollbar;
  }
}

void
Fl_Scroll::draw_clip(
  void* v,
  int const X,
  int const Y,
  unsigned int const W,
  unsigned int const H,
  enum Fl::foreground const fcolor,
  enum Fl::background const bcolor)
{

  Fl::draw_fill(X, Y, W, H, 0x20, fcolor, bcolor);
  Fl::draw_frame(X, Y, W, H, fcolor, bcolor);

  Fl::clip_push(X + 1, Y + 1, W - 2, H - 2);

  Fl_Scroll* s = (Fl_Scroll*)v;

  Fl_Widget* const* a = s->array();

  for (int i = s->children() - 2; i--;)
  {
    Fl_Widget& o = **a++;
    s->draw_child(o);
    s->draw_outside_label(o);
  }

  Fl::clip_pop();

  return;
}

void
Fl_Scroll::recalc_scrollbars(ScrollInfo& si)
{

  si.innerbox.x = x() + 1;
  si.innerbox.y = y() + 1;
  si.innerbox.w = w() - 2;
  si.innerbox.h = h() - 2;

  si.child.l = si.innerbox.x;
  si.child.r = si.innerbox.x;
  si.child.b = si.innerbox.y;
  si.child.t = si.innerbox.y;
  int first = 1;
  Fl_Widget* const* a = array();

  for (int i = children(); i--;)
  {
    Fl_Widget* o = *a++;

    if ( o == &scrollbar || o == &hscrollbar ) continue;

    if ( first )
    {
      first = 0;
      si.child.l = o->x();
      si.child.r = o->x() + o->w();
      si.child.b = o->y() + o->h();
      si.child.t = o->y();
    }

    else
    {
      if (o->x() < si.child.l) si.child.l = o->x();

      if (o->y() < si.child.t) si.child.t = o->y();

      if ((int)(o->x() + o->w()) > si.child.r) si.child.r = o->x() + o->w();

      if ((int)(o->y() + o->h()) > si.child.b) si.child.b = o->y() + o->h();
    }
  }

  {
    int X = si.innerbox.x;
    int Y = si.innerbox.y;
    int W = si.innerbox.w;
    int H = si.innerbox.h;

    si.scrollsize = 1;
    si.vneeded = 0;
    si.hneeded = 0;

    if (type() & VERTICAL)
    {
      if ((type() & ALWAYS_ON) || si.child.t < Y || si.child.b > Y + H)
      {
        si.vneeded = 1;
        W -= si.scrollsize;

        if (scrollbar.align() & Fl_Label::FL_ALIGN_LEFT) X += si.scrollsize;
      }
    }

    if (type() & HORIZONTAL)
    {
      if ((type() & ALWAYS_ON) || si.child.l < X || si.child.r > X + W)
      {
        si.hneeded = 1;
        H -= si.scrollsize;

        if (scrollbar.align() & Fl_Label::FL_ALIGN_TOP) Y += si.scrollsize;

        if (!si.vneeded && (type() & VERTICAL))
        {
          if ((type() & ALWAYS_ON) || si.child.t < Y || si.child.b > Y + H)
          {
            si.vneeded = 1;
            W -= si.scrollsize;

            if (scrollbar.align() & Fl_Label::FL_ALIGN_LEFT) X += si.scrollsize;
          }
        }
      }
    }

    si.innerchild.x = X;
    si.innerchild.y = Y;
    si.innerchild.w = W;
    si.innerchild.h = H;
  }

  si.hscroll.x = si.innerchild.x;
  si.hscroll.y = (scrollbar.align() & Fl_Label::FL_ALIGN_TOP)
                 ? si.innerbox.y
                 : si.innerbox.y + si.innerbox.h - si.scrollsize;
  si.hscroll.w = si.innerchild.w;
  si.hscroll.h = si.scrollsize;

  si.vscroll.x = (scrollbar.align() & Fl_Label::FL_ALIGN_LEFT)
                 ? si.innerbox.x
                 : si.innerbox.x + si.innerbox.w - si.scrollsize;
  si.vscroll.y = si.innerchild.y;
  si.vscroll.w = si.scrollsize;
  si.vscroll.h = si.innerchild.h;

  si.hscroll.pos = si.innerchild.x - si.child.l;
  si.hscroll.size = si.innerchild.w;
  si.hscroll.first = 0;
  si.hscroll.total = si.child.r - si.child.l;

  if ( si.hscroll.pos < 0 )
  {
    si.hscroll.total += (-si.hscroll.pos);
    si.hscroll.first = si.hscroll.pos;
  }

  si.vscroll.pos = si.innerchild.y - si.child.t;
  si.vscroll.size = si.innerchild.h;
  si.vscroll.first = 0;
  si.vscroll.total = si.child.b - si.child.t;

  if ( si.vscroll.pos < 0 )
  {
    si.vscroll.total += (-si.vscroll.pos);
    si.vscroll.first = si.vscroll.pos;
  }

}

void
Fl_Scroll::bbox(int& X, int& Y, int& W, int& H)
{
  X = x();
  Y = y();
  W = w();
  H = h();

  if (scrollbar.visible())
  {
    W -= scrollbar.w();

    if (scrollbar.align() & Fl_Label::FL_ALIGN_LEFT) X += scrollbar.w();
  }

  if (hscrollbar.visible())
  {
    H -= hscrollbar.h();

    if (scrollbar.align() & Fl_Label::FL_ALIGN_TOP) Y += hscrollbar.h();
  }
}

void
Fl_Scroll::draw()
{
  fix_scrollbar_order();
  int X, Y, W, H;
  bbox(X, Y, W, H);

  unsigned char d = damage();

  if (d & FL_DAMAGE_ALL)
  {
    draw_clip(this, X, Y, W, H, skin_.normal_fcolor, skin_.normal_bcolor);
  }

  else
  {
    if (d & FL_DAMAGE_SCROLL)
    {
      fl_scroll(X, Y, W, H, oldx - xposition_, oldy - yposition_, draw_clip, this,
                skin_);

      Fl_Widget* const* a = array();
      int L, R, T, B;
      L = 999999;
      R = 0;
      T = 999999;
      B = 0;

      for (int i = children() - 2; i--; a++)
      {
        if ((*a)->x() < L) L = (*a)->x();

        if ((int)((*a)->x() + (*a)->w()) > R) R = (*a)->x() + (*a)->w();

        if ((*a)->y() < T) T = (*a)->y();

        if ((int)((*a)->y() + (*a)->h()) > B) B = (*a)->y() + (*a)->h();
      }

      if (L > X) draw_clip(this, X, Y, L - X, H, skin_.normal_fcolor,
                             skin_.normal_bcolor);

      if (R < (X + W)) draw_clip(this, R, Y, X + W - R, H, skin_.normal_fcolor,
                                   skin_.normal_bcolor);

      if (T > Y) draw_clip(this, X, Y, W, T - Y, skin_.normal_fcolor,
                             skin_.normal_bcolor);

      if (B < (Y + H)) draw_clip(this, X, B, W, Y + H - B, skin_.normal_fcolor,
                                   skin_.normal_bcolor);
    }

    if (d & FL_DAMAGE_CHILD)
    {
      Fl::clip_push(X, Y, W, H);
      Fl_Widget* const* a = array();

      for (int i = children() - 2; i--;) update_child(**a++);

      Fl::clip_pop();
    }
  }

  {
    ScrollInfo si;
    recalc_scrollbars(si);

    if (si.vneeded && !scrollbar.visible())
    {
      scrollbar.set_visible();
      d = FL_DAMAGE_ALL;
    }

    else if (!si.vneeded && scrollbar.visible())
    {
      scrollbar.clear_visible();
      draw_clip(this, si.vscroll.x, si.vscroll.y, si.vscroll.w, si.vscroll.h,
                skin_.normal_fcolor, skin_.normal_bcolor);
      d = FL_DAMAGE_ALL;
    }

    if (si.hneeded && !hscrollbar.visible())
    {
      hscrollbar.set_visible();
      d = FL_DAMAGE_ALL;
    }

    else if (!si.hneeded && hscrollbar.visible())
    {
      hscrollbar.clear_visible();
      draw_clip(this, si.hscroll.x, si.hscroll.y, si.hscroll.w, si.hscroll.h,
                skin_.normal_fcolor, skin_.normal_bcolor);
      d = FL_DAMAGE_ALL;
    }

    else if ((int)hscrollbar.h() != si.scrollsize || (int)scrollbar.w() != si.scrollsize )
    {
      d = FL_DAMAGE_ALL;
    }

    scrollbar.resize(si.vscroll.x, si.vscroll.y, si.vscroll.w, si.vscroll.h);
    oldy = yposition_ = si.vscroll.pos;
    scrollbar.value(si.vscroll.pos, si.vscroll.size, si.vscroll.first,
                    si.vscroll.total);

    hscrollbar.resize(si.hscroll.x, si.hscroll.y, si.hscroll.w, si.hscroll.h);
    oldx = xposition_ = si.hscroll.pos;
    hscrollbar.value(si.hscroll.pos, si.hscroll.size, si.hscroll.first,
                     si.hscroll.total);
  }

  if (d & FL_DAMAGE_ALL)
  {
    draw_child(scrollbar);
    draw_child(hscrollbar);
#if 0

    if (scrollbar.visible() && hscrollbar.visible())
    {
      Fl::draw_putc(
        scrollbar.x(),
        hscrollbar.y(),
        0x20,
        1,
        skin_.normal_fcolor,
        skin_.normal_bcolor);
    }

#endif
  }

  else
  {
    update_child(scrollbar);
    update_child(hscrollbar);
  }
}

void
Fl_Scroll::resize(int const X, int const Y, unsigned int const W,
                  unsigned int const H)
{
  int dx = X - x(), dy = Y - y();
  int dw = W - w(), dh = H - h();
  Fl_Widget::resize(X, Y, W, H);
  fix_scrollbar_order();
  Fl_Widget* const* a = array();

  for (int i = children() - 2; i--;)
  {
    Fl_Widget* o = *a++;
    o->position(o->x() + dx, o->y() + dy);
  }

  if (dw == 0 && dh == 0)
  {
    char pad = ( scrollbar.visible() && hscrollbar.visible() );
    char al = ( (scrollbar.align() & Fl_Label::FL_ALIGN_LEFT) != 0 );
    char at = ( (scrollbar.align() & Fl_Label::FL_ALIGN_TOP)  != 0 );
    scrollbar.position(al ? X : X + W - 2, (at && pad) ? Y : Y + 1);
    hscrollbar.position((al && pad) ? X : X + 1, at ? Y : Y + H - 2);
  }

  else
  {
    redraw();
  }
}

void
Fl_Scroll::scroll_to(int X, int Y)
{
  int dx = xposition_ - X;
  int dy = yposition_ - Y;

  if (!dx && !dy) return;

  xposition_ = X;
  yposition_ = Y;
  Fl_Widget* const* a = array();

  for (int i = children(); i--;)
  {
    Fl_Widget* o = *a++;

    if (o == &hscrollbar || o == &scrollbar) continue;

    o->position(o->x() + dx, o->y() + dy);
  }

  if (parent() == (Fl_Group*)window()) damage(FL_DAMAGE_ALL);

  else damage(FL_DAMAGE_SCROLL);
}

void
Fl_Scroll::hscrollbar_cb(Fl_Widget* o, void*)
{
  Fl_Scroll* s = (Fl_Scroll*)(o->parent());
  s->scroll_to(int(((Fl_Scrollbar*)o)->value()), s->yposition());
}

void
Fl_Scroll::scrollbar_cb(Fl_Widget* o, void*)
{
  Fl_Scroll* s = (Fl_Scroll*)(o->parent());
  s->scroll_to(s->xposition(), int(((Fl_Scrollbar*)o)->value()));
}

Fl_Scroll::Fl_Scroll(int const X, int const Y, unsigned int const W,
                     unsigned int const H, const unsigned char* L)
  : Fl_Group(X, Y, W, H, L),
    scrollbar(X + W - 2, Y + 1, 1, H - 3),
    hscrollbar(X + 1, Y + H - 2, W - 3, 1)
{
  type(BOTH);
  xposition_ = oldx = 0;
  yposition_ = oldy = 0;
  hscrollbar.type(FL_HORIZONTAL);
  hscrollbar.callback(hscrollbar_cb);
  scrollbar.callback(scrollbar_cb);
}

bool
Fl_Scroll::handle(Fl_Event const event)
{
  fix_scrollbar_order();
  return Fl_Group::handle(event);
}
