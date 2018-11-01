// menu_.cxx
//
// Common menu code for the Fast Light Tool Kit (FLTK).
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
#include "fl.h"
#include "flstring.h"

#define SAFE_STRCAT(s) { len += (int) strlen(s); if ( len >= namelen ) { *name='\0'; return(-2); } else strcat(name,(s)); }

int
Fl_Menu_::item_pathname(char* name, int namelen,
                        const Fl_Menu_Item* finditem) const
{
  name[0] = '\0';
  return item_pathname_(name, namelen, finditem, menu_);
}

int
Fl_Menu_::item_pathname_(char* name,
                         int namelen,
                         const Fl_Menu_Item* finditem,
                         const Fl_Menu_Item* menu) const
{
  int len = 0;
  int level = 0;
  finditem = finditem ? finditem : mvalue();
  menu = menu ? menu : this->menu();

  for ( int t = 0; t < size(); t++ )
  {
    const Fl_Menu_Item* m = menu + t;

    if (m->submenu())
    {
      if (m->flags & FL_SUBMENU_POINTER)
      {
        int slen = strlen(name);
        const Fl_Menu_Item* submenu = (const Fl_Menu_Item*)m->user_data();

        if (m->label())
        {
          if (*name) SAFE_STRCAT("/");

          SAFE_STRCAT((char*)m->label());
        }

        if (item_pathname_(name, len, finditem, submenu) == 0)
          return 0;

        name[slen] = 0;
      }

      else
      {
        ++level;

        if (*name) SAFE_STRCAT("/");

        if (m->label()) SAFE_STRCAT((char*)m->label());

        if (m == finditem) return (0);
      }
    }

    else
    {
      if (m->label())
      {
        if ( m == finditem )
        {
          SAFE_STRCAT("/");
          SAFE_STRCAT((char*)m->label());
          return (0);
        }
      }

      else
      {
        if ( --level < 0 )
        {
          *name = '\0';
          return -1;
        }

        char* ss = strrchr(name, '/');

        if ( ss )
        {
          *ss = 0;
          len = (int) strlen(name);
        }

        else
        {
          name[0] = '\0';
          len = 0;
        }

        continue;
      }
    }
  }

  *name = '\0';
  return (-1);
}

const Fl_Menu_Item*
Fl_Menu_::find_item(const unsigned char* pathname)
{
  int i = find_index(pathname);
  return ( (i == -1) ? 0 : (const Fl_Menu_Item*)(menu_ + i));
}

int
Fl_Menu_::find_index(const Fl_Menu_Item* item) const
{
  Fl_Menu_Item* max = menu_ + size();

  if (item < menu_ || item >= max) return (-1);

  return (int) (item - menu_);
}

int
Fl_Menu_::find_index(Fl_Callback* cb) const
{
  for ( int t = 0; t < size(); t++ )
    if (menu_[t].callback_ == cb)
      return (t);

  return (-1);
}

int
Fl_Menu_::find_index(const unsigned char* pathname) const
{
  unsigned char menupath[1024] = "";

  for ( int t = 0; t < size(); t++ )
  {
    Fl_Menu_Item* m = menu_ + t;

    if (m->flags & FL_SUBMENU)
    {
      if (menupath[0]) strlcat((char*)menupath, "/", sizeof(menupath));

      strlcat((char*)menupath, (char const*)m->label(), sizeof(menupath));

      if (!strcmp((char*)menupath, (char const*)pathname)) return (t);
    }

    else
    {
      if (!m->label())
      {
        unsigned char* ss = (unsigned char*)strrchr((char const*)menupath, '/');

        if ( ss ) *ss = 0;

        else menupath[0] = '\0';

        continue;
      }

      unsigned char itempath[1024];
      strcpy((char*)itempath, (char const*)menupath);

      if (itempath[0]) strlcat((char*)itempath, "/", sizeof(itempath));

      strlcat((char*)itempath, (char const*)m->label(), sizeof(itempath));

      if (!strcmp((char const*)itempath, (char const*)pathname)) return (t);
    }
  }

  return (-1);
}

const Fl_Menu_Item*
Fl_Menu_::find_item(Fl_Callback* cb)
{
  for ( int t = 0; t < size(); t++ )
  {
    const Fl_Menu_Item* m = menu_ + t;

    if (m->callback_ == cb)
    {
      return m;
    }
  }

  return (const Fl_Menu_Item*)0;
}

int
Fl_Menu_::value(const Fl_Menu_Item* m)
{
  clear_changed();

  if (value_ != m)
  {
    value_ = m;
    return 1;
  }

  return 0;
}

const Fl_Menu_Item*
Fl_Menu_::picked(const Fl_Menu_Item* v)
{
  if (v)
  {
    if (v->radio())
    {
      if (!v->value())
      {
        set_changed();
        setonly((Fl_Menu_Item*)v);
      }

      redraw();
    }

    else if (v->flags & FL_MENU_TOGGLE)
    {
      set_changed();
      ((Fl_Menu_Item*)v)->flags ^= FL_MENU_VALUE;
      redraw();
    }

    else if (v != value_)
    {
      set_changed();
    }

    value_ = v;

    if (when() & (FL_WHEN_CHANGED | FL_WHEN_RELEASE))
    {
      if (changed() || when()&FL_WHEN_NOT_CHANGED)
      {
        if (value_ && value_->callback_) value_->do_callback((Fl_Widget*)this);

        else do_callback();
      }
    }
  }

  return v;
}

static Fl_Menu_Item*
first_submenu_item(Fl_Menu_Item* item, Fl_Menu_Item* start)
{
  Fl_Menu_Item* m = start;
  int nest = 0;

  while (1)
  {
    if (!m->text)
    {
      if (!nest) return NULL;

      nest--;
    }

    else
    {
      if (m == item) return start;

      if (m->flags & FL_SUBMENU_POINTER)
      {
        Fl_Menu_Item* first = first_submenu_item(item, (Fl_Menu_Item*)m->user_data());

        if (first) return first;
      }

      else if (m->flags & FL_SUBMENU)
      {
        nest++;
      }
    }

    m++;
  }
}


Fl_Menu_::setonly(Fl_Menu_Item* item)
{
  Fl_Menu_Item* first = first_submenu_item(item, menu_);

  if (!first) return;

  item->flags |= FL_MENU_RADIO | FL_MENU_VALUE;
  Fl_Menu_Item* j;

  for (j = item; ; )
  {
    if (j->flags & FL_MENU_DIVIDER) break; 

    j++;

    if (!j->text || !j->radio()) break;

    j->clear();
  }

  for (j = item - 1; j >= first; j--)
  {
    if (!j->text || (j->flags & FL_MENU_DIVIDER) || !j->radio()) break;

    j->clear();
  }
}

void
Fl_Menu_Item::setonly()
{
  flags |= FL_MENU_RADIO | FL_MENU_VALUE;
  Fl_Menu_Item* j;

  for (j = this; ; )
  {
    if (j->flags & FL_MENU_DIVIDER) break;

    j++;

    if (!j->text || !j->radio()) break;

    j->clear();
  }

  for (j = this - 1; ; j--)
  {
    if (!j->text || (j->flags & FL_MENU_DIVIDER) || !j->radio()) break;

    j->clear();
  }
}

Fl_Menu_::Fl_Menu_(
  int const i_pos_x,
  int const i_pos_y,
  unsigned int const i_len_x,
  unsigned int const i_len_y,
  unsigned char const* i_label) :
  Fl_Widget(i_pos_x, i_pos_y, i_len_x, i_len_y, i_label)
{
  set_flag(SHORTCUT_LABEL);
  when(FL_WHEN_RELEASE_ALWAYS);
  value_ = menu_ = 0;
  alloc = 0;
}

int
Fl_Menu_::size() const
{
  if (!menu_) return 0;

  return menu_->size();
}

void
Fl_Menu_::menu(const Fl_Menu_Item* m)
{
  clear();
  value_ = menu_ = (Fl_Menu_Item*)m;
}

void
Fl_Menu_::copy(const Fl_Menu_Item* m, void* ud)
{
  int n = m->size();
  Fl_Menu_Item* newMenu = new Fl_Menu_Item[n];
  memcpy(newMenu, m, n * sizeof(Fl_Menu_Item));
  menu(newMenu);
  alloc = 1;

  if (ud) for (; n--;)
    {
      if (newMenu->callback_) newMenu->user_data_ = ud;

      newMenu++;
    }
}

Fl_Menu_::~Fl_Menu_()
{
  clear();
}

Fl_Menu_* fl_menu_array_owner = 0;

void
Fl_Menu_::clear()
{
  if (alloc)
  {
    if (alloc > 1) for (int i = size(); i--;)
        if (menu_[i].text) free((void*)menu_[i].text);

    if (this == fl_menu_array_owner)
      fl_menu_array_owner = 0;

    else
      delete[] menu_;

    alloc = 0;
  }

  menu_ = 0;
  value_ = 0;
}

int
Fl_Menu_::clear_submenu(int index)
{
  if ( index < 0 || index >= size() ) return (-1);

  if ( ! (menu_[index].flags & FL_SUBMENU) ) return (-1);

  ++index;

  while ( index < size() )
  {
    if ( menu_[index].text == 0 ) break;

    remove(index);
  }

  return (0);
}
