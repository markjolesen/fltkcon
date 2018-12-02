/*
 fl_caret.h

 ripped from FDOSTUI cursor.h

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#ifndef __fl_caret_h__

#include "drvgr.h"

#if defined(__CURSES__)
#include <curses.h>
#endif

namespace Fl
{

  enum caret_mode
  {
    CARET_OVERWRITE                     = 0,
    CARET_INSERT                        = 1
  };

  extern bool                           _caret_can_hide;
  extern bool                           _caret_is_visible;
  extern enum caret_mode                _caret_mode;

  extern void
  caret_set_position(
    unsigned char const                 i_column,
    unsigned char const                 i_row);

  extern void
  caret_hide();

  extern void
  caret_underline();

  extern void
  caret_block();

#if defined(__DOS__) && !defined(__CURSES__)

#pragma aux __caret_set_position =\
    "xor bh, bh"\
    "mov ah, 02h"\
    "int 010h"\
    modify [AH BH]\
    parm [DL] [DH];

#pragma aux __caret_set_type= \
    "mov ah, 01h"\
    "int 010h" \
    modify [ AH ] \
    parm [CH] [CL] ;

#if defined(__FLAT__)

#pragma aux __caret_get_position=\
    "push ebx"\
    "push eax"\
    "xor bh,bh"\
    "mov ah, 03h"\
    "int 010h"\
    "pop ebx"\
    "mov [byte ptr ebx], ch"\
    "pop ebx"\
    "mov [byte ptr ebx], cl"\
    "mov [byte ptr esi], dh"\
    "mov [byte ptr edi], dl"\
    modify [ AH ] \
    parm [AX] [BX] [SI] [DI];

#else

#pragma aux __caret_get_position=\
    "push bx"\
    "push ax"\
    "xor bh,bh"\
    "mov ah, 03h"\
    "int 010h"\
    "pop bx"\
    "mov [byte ptr bx], ch"\
    "pop bx"\
    "mov [byte ptr bx], cl"\
    "mov [byte ptr ds:si], dh"\
    "mov [byte ptr ds:di], dl"\
    modify [ AH ] \
    parm [AX] [BX] [SI] [DI];

#endif /* #ifdef __FLAT__ */

#define CARET_MASK_HIDE                0x20

  inline void
  caret_hide()
  {
    __caret_set_type(CARET_MASK_HIDE, 0x0);
    _caret_is_visible = false;
  }

  inline void
  caret_underline()
  {
    __caret_set_type(0x06, 0x07);
    _caret_mode = CARET_INSERT;
    _caret_is_visible = true;
  }

  inline void
  caret_block()
  {
    __caret_set_type(0x00, 0x07);
    _caret_mode = CARET_OVERWRITE;
    _caret_is_visible = true;
  }

  inline void
  caret_set_position(
    unsigned char const                 i_column,
    unsigned char const                 i_row)
  {
    __caret_set_position(
      (Fl_Graphics_Driver::draw_offset_x + i_column),
      (Fl_Graphics_Driver::draw_offset_y + i_row));
  }

#endif /* #if defined(__DOS__) && !defined(__CURSES__) */

#if defined(__CURSES__)

  inline void
  caret_set_position(
    unsigned char const                   i_column,
    unsigned char const                   i_row)
  {

    move(
      (Fl_Graphics_Driver::draw_offset_y + i_row),
      (Fl_Graphics_Driver::draw_offset_x + i_column));

    return;
  }

  inline void
  caret_hide()
  {

    curs_set(0);
    _caret_is_visible = false;

    return;
  }

  inline void
  caret_underline()
  {

    curs_set(1);
    _caret_mode = CARET_INSERT;
    _caret_is_visible = true;

    return;
  }

  inline void
  caret_block()
  {

    curs_set(2);
    _caret_mode = CARET_OVERWRITE;
    _caret_is_visible = true;

    return;
  }

#endif /* #if defined(__CURSES__) */

#if defined(__NT__)

  extern "C" HANDLE                       _screen_active;

  inline void
  caret_set_position(
    unsigned char const                 i_column,
    unsigned char const                 i_row)
  {
    COORD                               l_coord;

    l_coord.X = (Fl_Graphics_Driver::draw_offset_x + i_column);
    l_coord.Y = (Fl_Graphics_Driver::draw_offset_y + i_row);
    SetConsoleCursorPosition(_screen_active, l_coord);

    return;
  }

  inline void
  caret_hide()
  {
    CONSOLE_CURSOR_INFO                 l_info;

    GetConsoleCursorInfo(_screen_active, &l_info);
    l_info.bVisible = 0;
    SetConsoleCursorInfo(_screen_active, &l_info);
    _caret_is_visible = false;

    return;
  }

  inline void
  caret_underline()
  {
    CONSOLE_CURSOR_INFO                 l_info;

    l_info.dwSize = 15;
    l_info.bVisible = 1;
    SetConsoleCursorInfo(_screen_active, &l_info);
    _caret_mode = CARET_INSERT;
    _caret_is_visible = true;

    return;
  }

  inline void
  caret_block()
  {
    CONSOLE_CURSOR_INFO                 l_info;

    l_info.dwSize = 100;
    l_info.bVisible = 1;
    SetConsoleCursorInfo(_screen_active, &l_info);
    _caret_mode = CARET_OVERWRITE;
    _caret_is_visible = true;

    return;
  }

#endif /* #if defined(__NT__) */

}

#define __fl_caret_h__
#endif
