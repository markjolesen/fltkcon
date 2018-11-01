// fl_enums.h
//
// Enumerations for the Fast Light Tool Kit (FLTK).
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
#ifndef Fl_Enumerations_H
#define Fl_Enumerations_H

enum Fl_Event
{
  FL_NO_EVENT   = 0,
  FL_PUSH   = 1,
  FL_RELEASE    = 2,
  FL_ENTER    = 3,
  FL_LEAVE    = 4,
  FL_DRAG   = 5,
  FL_FOCUS    = 6,
  FL_UNFOCUS    = 7,
  FL_KEYDOWN    = 8,
  FL_KEYBOARD   = 8,
  FL_KEYUP    = 9,
  FL_CLOSE    = 10,
  FL_MOVE   = 11,
  FL_SHORTCUT   = 12,
  FL_DEACTIVATE   = 13,
  FL_ACTIVATE   = 14,
  FL_HIDE   = 15,
  FL_SHOW   = 16,
  FL_PASTE    = 17,
  FL_SELECTIONCLEAR = 18,
  FL_MOUSEWHEEL   = 19,
  FL_DND_ENTER    = 20,
  FL_DND_DRAG   = 21,
  FL_DND_LEAVE    = 22,
  FL_DND_RELEASE  = 23,
  FL_SCREEN_CONFIGURATION_CHANGED = 24,
  FL_FULLSCREEN         = 25,
  FL_ZOOM_GESTURE = 26
};

#define FL_Button 0xfee8
#define FL_BackSpace  0xff08
#define FL_Tab    0xff09
#define FL_Iso_Key  0xff0c
#define FL_Enter  0xff0d
#define FL_Pause  0xff13
#define FL_Scroll_Lock  0xff14
#define FL_Escape 0xff1b
#define FL_Kana         0xff2e
#define FL_Eisu         0xff2f
#define FL_Yen          0xff30
#define FL_JIS_Underscore 0xff31
#define FL_Home   0xff50
#define FL_Left   0xff51
#define FL_Up   0xff52
#define FL_Right  0xff53
#define FL_Down   0xff54
#define FL_Page_Up  0xff55
#define FL_Page_Down  0xff56
#define FL_End    0xff57
#define FL_Print  0xff61
#define FL_Insert 0xff63
#define FL_Menu   0xff67
#define FL_Help   0xff68
#define FL_Num_Lock 0xff7f
#define FL_KP   0xff80
#define FL_KP_Enter 0xff8d
#define FL_KP_Last  0xffbd
#define FL_F    0xffbd
#define FL_F_Last 0xffe0
#define FL_Shift_L  0xffe1
#define FL_Shift_R  0xffe2
#define FL_Control_L  0xffe3
#define FL_Control_R  0xffe4
#define FL_Caps_Lock  0xffe5
#define FL_Meta_L 0xffe7
#define FL_Meta_R 0xffe8
#define FL_Alt_L  0xffe9
#define FL_Alt_R  0xffea
#define FL_Delete 0xffff

#define FL_Volume_Down  0xEF11
#define FL_Volume_Mute  0xEF12
#define FL_Volume_Up    0xEF13
#define FL_Media_Play   0xEF14
#define FL_Media_Stop   0xEF15
#define FL_Media_Prev   0xEF16
#define FL_Media_Next   0xEF17
#define FL_Home_Page    0xEF18
#define FL_Mail         0xEF19
#define FL_Search       0xEF1B
#define FL_Back         0xEF26
#define FL_Forward      0xEF27
#define FL_Stop         0xEF28
#define FL_Refresh      0xEF29
#define FL_Sleep        0xEF2F
#define FL_Favorites    0xEF30

#define FL_LEFT_MOUSE 1
#define FL_MIDDLE_MOUSE 2
#define FL_RIGHT_MOUSE  3

#define FL_SHIFT  0x00010000
#define FL_CAPS_LOCK  0x00020000
#define FL_CTRL   0x00040000
#define FL_ALT    0x00080000
#define FL_NUM_LOCK 0x00100000
#define FL_META   0x00400000
#define FL_SCROLL_LOCK  0x00800000
#define FL_BUTTON1  0x01000000
#define FL_BUTTON2  0x02000000
#define FL_BUTTON3  0x04000000
#define FL_BUTTONS  0x7f000000
#define FL_BUTTON(n)  (0x00800000<<(n))

#define FL_KEY_MASK 0x0000ffff

#define FL_COMMAND FL_CTRL

typedef unsigned int Fl_Shortcut;

enum Fl_Boxtype
{
  FL_BOX_NONE = 0,
  FL_BOX_FRAME
};


#endif
