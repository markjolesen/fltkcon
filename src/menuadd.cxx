// menuadd.cxx
//
// Menu utilities for the Fast Light Tool Kit (FLTK).
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
//
#include <stdio.h>
#include <stdlib.h>
#include "menu_.h"
#include "flstring.h"

static Fl_Menu_Item* local_array = 0;
static int local_array_alloc = 0;
static int local_array_size = 0;
extern Fl_Menu_* fl_menu_array_owner;

static Fl_Menu_Item*
array_insert(
  Fl_Menu_Item* array,
  int size,
  int n,
  const unsigned char* text,
  int flags
)
{
  if (array == local_array && size >= local_array_alloc)
  {
    local_array_alloc = 2 * size;
    Fl_Menu_Item* newarray = new Fl_Menu_Item[local_array_alloc];
    memmove(newarray, array, size * sizeof(Fl_Menu_Item));
    delete[] local_array;
    local_array = array = newarray;
  }

  memmove(array + n + 1, array + n, sizeof(Fl_Menu_Item) * (size - n));
  Fl_Menu_Item* m = array + n;
  m->text = (unsigned char*)(text ? strdup((char const*)text) : 0);
  m->shortcut_ = 0;
  m->callback_ = 0;
  m->user_data_ = 0;
  m->flags = flags;
  m->labeltype_ = 0;
  return array;
}

static int
compare(const unsigned char* a, const unsigned char* b)
{
  for (;;)
  {
    int n = *a - *b;

    if (n)
    {
      if (*a == '&') a++;

      else if (*b == '&') b++;

      else return n;
    }

    else if (*a)
    {
      a++;
      b++;
    }

    else
    {
      return 0;
    }
  }
}

int
Fl_Menu_Item::add(
  const unsigned char* mytext,
  int sc,
  Fl_Callback* cb,
  void* data,
  int myflags
)
{
  return (insert(-1, mytext, sc, cb, data, myflags)); // -1: append
}

int
Fl_Menu_Item::insert(
  int index,
  const unsigned char* mytext,
  int sc,
  Fl_Callback* cb,
  void* data,
  int myflags
)
{
  Fl_Menu_Item* array = this;
  Fl_Menu_Item* m = this;
  const unsigned char* p;
  unsigned char* q;
  unsigned char buf[1024];

  int msize = array == local_array ? local_array_size : array->size();
  int flags1 = 0;
  const unsigned char* item;

  for (;;)
  {

    if (*mytext == '/')
    {
      item = mytext;
      break;
    }

    if (*mytext == '_')
    {
      mytext++;
      flags1 = FL_MENU_DIVIDER;
    }

    q = buf;

    for (p = mytext; *p && *p != '/'; *q++ = *p++) if (*p == '\\' && p[1]) p++;

    *q = 0;

    item = buf;

    if (*p != '/') break;

    index = -1;
    mytext = p + 1;

    for (; m->text; m = m->next())
      if (m->flags & FL_SUBMENU && !compare(item, m->text)) break;

    if (!m->text)
    {
      int n = (index == -1) ? (int) (m - array) : index;
      array = array_insert(array, msize, n, item, FL_SUBMENU | flags1);
      msize++;
      array = array_insert(array, msize, n + 1, 0, 0);
      msize++;
      m = array + n;
    }

    m++;
    flags1 = 0;
  }

  for (; m->text; m = m->next())
    if (!(m->flags & FL_SUBMENU) && !compare(m->text, item)) break;

  if (!m->text)
  {
    int n = (index == -1) ? (int) (m - array) : index;
    array = array_insert(array, msize, n, item, myflags | flags1);
    msize++;

    if (myflags & FL_SUBMENU)
    {
      array = array_insert(array, msize, n + 1, 0, 0);
      msize++;
    }

    m = array + n;
  }

  m->shortcut_ = sc;
  m->callback_ = cb;
  m->user_data_ = data;
  m->flags = myflags | flags1;

  if (array == local_array) local_array_size = msize;

  return (int) (m - array);
}

int
Fl_Menu_::add(const unsigned char* label, int shortcut, Fl_Callback* callback,
              void* userdata, int flags)
{
  return (insert(-1, label, shortcut, callback, userdata, flags));
}

int
Fl_Menu_::insert(
  int index,
  const unsigned char* label,
  int shortcut,
  Fl_Callback* callback,
  void* userdata,
  int flags
)
{
  if (this != fl_menu_array_owner)
  {
    if (fl_menu_array_owner)
    {
      Fl_Menu_* o = fl_menu_array_owner;
      int value_offset = (int) (o->value_ - local_array);
      int n = local_array_size;
      Fl_Menu_Item* newMenu = o->menu_ = new Fl_Menu_Item[n];
      memcpy(newMenu, local_array, n * sizeof(Fl_Menu_Item));

      if (o->value_) o->value_ = newMenu + value_offset;
    }

    if (menu_)
    {
      delete[] local_array;

      if (!alloc) copy(menu_); 

      local_array_alloc = local_array_size = size();
      local_array = menu_;
    }

    else
    {
      alloc = 2; 

      if (local_array)
      {
        menu_ = local_array;
      }

      else
      {
        local_array_alloc = 15;
        local_array = menu_ = new Fl_Menu_Item[local_array_alloc];
        memset(local_array, 0, sizeof(Fl_Menu_Item) * local_array_alloc);
      }

      memset(menu_, 0, sizeof(Fl_Menu_Item));
      local_array_size = 1;
    }

    fl_menu_array_owner = this;
  }

  int r = menu_->insert(index, label, shortcut, callback, userdata, flags);
  int value_offset = (int) (value_ - menu_);
  menu_ = local_array;

  if (value_) value_ = menu_ + value_offset;

  return r;
}

int
Fl_Menu_::add(const unsigned char* str)
{
  unsigned char buf[1024];
  int r = 0;

  while (*str)
  {
    int sc = 0;
    unsigned char* c;

    for (c = buf; c < (buf + sizeof(buf) - 2) && *str && *str != '|'; str++)
    {
      if (*str == '\t')
      {
        *c++ = 0;
        sc = fl_old_shortcut(str);
      }

      else *c++ = *str;
    }

    *c = 0;
    r = add(buf, sc, 0, 0, 0);

    if (*str) str++;
  }

  return r;
}

void
Fl_Menu_::replace(int i, const unsigned char* str)
{
  if (i < 0 || i >= size()) return;

  if (!alloc) copy(menu_);

  if (alloc > 1)
  {
    free((void*)menu_[i].text);
    str = (unsigned char*)strdup((char const*)str);
  }

  menu_[i].text = str;
}


void
Fl_Menu_::remove(int i)
{
  int n = size();

  if (i < 0 || i >= n) return;

  if (!alloc) copy(menu_);

  Fl_Menu_Item* item = menu_ + i;
  const Fl_Menu_Item* next_item = item->next();

  if (alloc > 1)
  {
    for (Fl_Menu_Item* m = item; m < next_item; m++)
      if (m->text) free((void*)(m->text));
  }

  memmove(item, next_item, (menu_ + n - next_item)*sizeof(Fl_Menu_Item));
}
