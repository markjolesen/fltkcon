/*
 fl_caret.cxx

 License CC0 PUBLIC DOMAIN

 To the extent possible under law, Mark J. Olesen has waived all copyright
 and related or neighboring rights to fltkcon Library. This work is published
 from: United States.
*/
#include "fl_caret.h"

bool Fl::_caret_is_visible = false;
bool Fl::_caret_can_hide = true;
enum Fl::caret_mode Fl::_caret_mode = Fl::CARET_INSERT;
