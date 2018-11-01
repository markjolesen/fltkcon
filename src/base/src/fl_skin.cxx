/*
 skin.cxx

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#include "fl_skin.h"

struct Fl::skin_symbols Fl::_skin_sym =
{
  /* thumb */ 0xdb,
  /* stipple */ 0xb1,
  /* ret */ 0x4,
  /* up */ 0x1e,
  /* down */ 0x1f,
  /* right */ 0x10,
  /* left */ 0x11,
  /* lequal */0xf3,
  /* gequal */0xf2,
  /* hline */ 0xc4,
  /* vline */ 0xb3,
  /* top_left */ 0xda,
  /* top_right */ 0xbf,
  /* bottom_left */ 0xc0,
  /* bottom_right */ 0xd9
};

struct Fl::skin_widget Fl::_skin_widget =
{
  /* normal_bcolor */ Fl::bcolor_black,
  /* normal_fcolor */ Fl::fcolor_white,
  /* focused_bcolor */ Fl::bcolor_black,
  /* focused_fcolor */ Fl::fcolor_blue,
  /* highlight_fcolor */ Fl::fcolor_yellow,
  /* disabled_fcolor */ Fl::fcolor_red,
  /* menu_bcolor */ Fl::bcolor_light_gray,
  /* menu_fcolor */ Fl::fcolor_black,
  /* box_bcolor */ Fl::bcolor_black,
  /* box_fcolor */ Fl::fcolor_white
};

struct Fl::skin_box Fl::_skin_win =
{
  /* fillc */ 0x20,
  /* fill_bcolor */ Fl::bcolor_black,
  /* fill_fcolor */ Fl::fcolor_white
};

struct Fl::skin_box Fl::_skin_box =
{
  /* fillc */ 0x20,
  /* fill_bcolor */ Fl::bcolor_black,
  /* fill_fcolor */ Fl::fcolor_white
};

struct Fl::skin_editor Fl::_skin_editor =
{
  /* margin_fillc */ 0x20,
  /* margin_bcolor */ Fl::bcolor_light_gray,
  /* margin_fcolor */ Fl::fcolor_black,
  /* lineno_bcolor */ Fl::bcolor_light_gray,
  /* lineno_fcolor */ Fl::fcolor_black
};
