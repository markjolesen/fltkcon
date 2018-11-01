/*
 fl_color.h

 (color bits taken from FDOSTUI SCREEN.H)

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#ifndef __fl_color_h__

#if defined(__NT__) && !defined(__CURSES__)
#    ifdef _WIN32_WINNT
#    undef _WIN32_WINNT
#    endif
#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif

#ifdef __CURSES__
#include <curses.h>
#endif

#if (defined(__DOS__) || defined(__DJGPP__)) && !defined(__CURSES__)

namespace Fl
{

  enum foreground
  {
    fcolor_black                        = 0,
    fcolor_blue                         = 1,
    fcolor_green                        = 2,
    fcolor_cyan                         = 3,
    fcolor_red                          = 4,
    fcolor_magenta                      = 5,
    /* fcolor_brown                     = 6, */
    /* fcolor_light_gray                = 7, */
    /* fcolor_gray                      = 8, */
    /* fcolor_light_blue                = 9, */
    /* fcolor_light_green               = 10, */
    /* fcolor_light_cyan                = 11, */
    /* fcolor_light_red                 = 12, */
    /* fcolor_light_magenta             = 13, */
    fcolor_yellow                       = 14,
    fcolor_white                        = 15
  };

};

#endif /* (defined(__DOS__) || defined(__DJGPP__)) && !defined(__CURSES__) */

#if defined(__NT__) && !defined(__CURSES__)

#define FCOLOR_BLACK                    (0)
#define FCOLOR_RED                      (FOREGROUND_RED)
#define FCOLOR_GREEN                    (FOREGROUND_GREEN)
#define FCOLOR_BLUE                     (FOREGROUND_BLUE)
#define FCOLOR_YELLOW                   (FOREGROUND_RED   | FOREGROUND_GREEN)
#define FCOLOR_MAGENTA                  (FOREGROUND_RED   | FOREGROUND_BLUE)
#define FCOLOR_WHITE                    (FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FCOLOR_CYAN                     (FOREGROUND_GREEN | FOREGROUND_BLUE)

#define FCOLOR_LIGHT_BLACK              (FCOLOR_BLACK   | FOREGROUND_INTENSITY)
#define FCOLOR_LIGHT_RED                (FCOLOR_RED     | FOREGROUND_INTENSITY)
#define FCOLOR_LIGHT_GREEN              (FCOLOR_GREEN   | FOREGROUND_INTENSITY)
#define FCOLOR_LIGHT_BLUE               (FCOLOR_BLUE    | FOREGROUND_INTENSITY)
#define FCOLOR_LIGHT_YELLOW             (FCOLOR_YELLOW  | FOREGROUND_INTENSITY)
#define FCOLOR_LIGHT_MAGENTA            (FCOLOR_MAGENTA | FOREGROUND_INTENSITY)
#define FCOLOR_LIGHT_WHITE              (FCOLOR_WHITE   | FOREGROUND_INTENSITY)
#define FCOLOR_LIGHT_CYAN               (FCOLOR_CYAN    | FOREGROUND_INTENSITY)

namespace Fl
{

  enum foreground
  {
    fcolor_black                        = FCOLOR_BLACK,
    fcolor_blue                         = FCOLOR_BLUE,
    fcolor_green                        = FCOLOR_GREEN,
    fcolor_cyan                         = FCOLOR_CYAN,
    fcolor_red                          = FCOLOR_RED,
    fcolor_magenta                      = FCOLOR_MAGENTA,
    fcolor_yellow                       = FCOLOR_YELLOW,
    fcolor_white                        = FCOLOR_WHITE
  };

};

#endif /* #if defined(__NT__) && !defined(__CURSES__) */

#if defined(__CURSES__)

namespace Fl
{

  enum foreground
  {
    fcolor_black                        = COLOR_BLACK,
    fcolor_blue                         = COLOR_BLUE,
    fcolor_green                        = COLOR_GREEN,
    fcolor_cyan                         = COLOR_CYAN,
    fcolor_red                          = COLOR_RED,
    fcolor_magenta                      = COLOR_MAGENTA,
    fcolor_yellow                       = COLOR_YELLOW,
    fcolor_white                        = COLOR_WHITE
  };

};

#endif /* #if defined(__CURSES__) */

#if (defined(__DOS__) || defined(__DJGPP__)) && !defined(__CURSES__)

namespace Fl
{

  enum background
  {
    bcolor_black                        = 0,
    bcolor_blue                         = 1,
    bcolor_green                        = 2,
    bcolor_cyan                         = 3,
    bcolor_red                          = 4,
    bcolor_magenta                      = 5,
    /* bcolor_brown                     = 6, */
    bcolor_light_gray                   = 7
  };

};

#endif /* (defined(__DOS__) || defined(__DJGPP__)) && !defined(__CURSES__) */

#if defined(__NT__) && !defined(__CURSES__)

#define BCOLOR_BLACK                    (0)
#define BCOLOR_RED                      (BACKGROUND_RED)
#define BCOLOR_GREEN                    (BACKGROUND_GREEN)
#define BCOLOR_BLUE                     (BACKGROUND_BLUE)
#define BCOLOR_YELLOW                   (BACKGROUND_RED   | BACKGROUND_GREEN)
#define BCOLOR_MAGENTA                  (BACKGROUND_RED   | BACKGROUND_BLUE)
#define BCOLOR_WHITE                    (BACKGROUND_RED   | BACKGROUND_GREEN | BACKGROUND_BLUE)
#define BCOLOR_CYAN                     (BACKGROUND_GREEN | BACKGROUND_BLUE)

#define BCOLOR_LIGHT_BLACK              (BCOLOR_BLACK   | BACKGROUND_INTENSITY)
#define BCOLOR_LIGHT_RED                (BCOLOR_RED     | BACKGROUND_INTENSITY)
#define BCOLOR_LIGHT_GREEN              (BCOLOR_GREEN   | BACKGROUND_INTENSITY)
#define BCOLOR_LIGHT_BLUE               (BCOLOR_BLUE    | BACKGROUND_INTENSITY)
#define BCOLOR_LIGHT_YELLOW             (BCOLOR_YELLOW  | BACKGROUND_INTENSITY)
#define BCOLOR_LIGHT_MAGENTA            (BCOLOR_MAGENTA | BACKGROUND_INTENSITY)
#define BCOLOR_LIGHT_WHITE              (BCOLOR_WHITE   | BACKGROUND_INTENSITY)
#define BCOLOR_LIGHT_CYAN               (BCOLOR_CYAN    | BACKGROUND_INTENSITY)

namespace Fl
{

  enum background
  {
    bcolor_black                        = BCOLOR_BLACK,
    bcolor_blue                         = BCOLOR_BLUE,
    bcolor_green                        = BCOLOR_GREEN,
    bcolor_cyan                         = BCOLOR_CYAN,
    bcolor_red                          = BCOLOR_RED,
    bcolor_magenta                      = BCOLOR_MAGENTA,
    bcolor_light_gray                   = BCOLOR_WHITE
  };

};

#endif /* #if defined(__NT__) && !defined(__CURSES__) */

#if defined(__CURSES__)

namespace Fl
{

  enum background
  {
    bcolor_black                        = COLOR_BLACK,
    bcolor_blue                         = COLOR_BLUE,
    bcolor_green                        = COLOR_GREEN,
    bcolor_cyan                         = COLOR_CYAN,
    bcolor_red                          = COLOR_RED,
    bcolor_magenta                      = COLOR_MAGENTA,
    bcolor_light_gray                   = COLOR_WHITE
  };

};

#endif /* #if defined(__CURSES__) */

#define __fl_color_h__
#endif
