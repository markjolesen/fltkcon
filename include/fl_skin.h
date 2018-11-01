/*
 fl_skin.h

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#if !defined(__skin_h__)

#include "fl_color.h"

namespace Fl
{

  struct skin_symbols
  {
    unsigned int thumb;
    unsigned int stipple;
    unsigned int ret;
    unsigned int up;
    unsigned int down;
    unsigned int right;
    unsigned int left;
    unsigned int lequal;
    unsigned int gequal;
    unsigned int hline;
    unsigned int vline;
    unsigned int top_left;
    unsigned int top_right;
    unsigned int bottom_left;
    unsigned int bottom_right;
  };

  struct skin_widget
  {
    enum Fl::background normal_bcolor;
    enum Fl::foreground normal_fcolor;
    enum Fl::background focused_bcolor;
    enum Fl::foreground focused_fcolor;
    enum Fl::foreground highlight_fcolor;
    enum Fl::foreground disabled_fcolor;
    enum Fl::background menu_bcolor;
    enum Fl::foreground menu_fcolor;
    enum Fl::background box_bcolor;
    enum Fl::foreground box_fcolor;
  };

  struct skin_box
  {
    unsigned int fillc;
    enum Fl::background fill_bcolor;
    enum Fl::foreground fill_fcolor;
  };

  struct skin_editor
  {
    unsigned int margin_fillc;
    enum Fl::background margin_bcolor;
    enum Fl::foreground margin_fcolor;
    enum Fl::background lineno_bcolor;
    enum Fl::foreground lineno_fcolor;
  };

  extern struct skin_symbols _skin_sym;
  extern struct skin_widget _skin_widget;
  extern struct skin_box _skin_win;
  extern struct skin_box _skin_box;
  extern struct skin_editor _skin_editor;

};

#define __skin_h__
#endif
