// fl_draw.cxx
//
// Label drawing code for the Fast Light Tool Kit (FLTK).
//
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

// Implementation of fl_draw(const char*,int,int,int,int,Fl_Align)
// Used to draw all the labels and text, this routine:
// Word wraps the labels to fit into their bounding box.
// Breaks them into lines at the newlines.
// Expands all unprintable characters to ^X or \nnn notation
// Aligns them against the inside of the box.

#include "fl_rend.h"
#include "fl_utf8.h"
#include "fl.h"
#include "fl_clip.h"
#include "flstring.h"
#include <ctype.h>
#include <math.h>

inline int
fl_height()
{
  return 1;
}
inline int
fl_descent()
{
  return 0;
}
#define fl_draw_symbol(a,b,c,d,e,f)


char fl_draw_shortcut;  // set by fl_labeltypes.cxx

static unsigned char* underline_at;

/* If called with maxbuf==0, use an internally allocated buffer and enlarge it as needed.
 Otherwise, use buf as buffer but don't go beyond its length of maxbuf.
 */
static const unsigned char*
expand_text_(const unsigned char* from, unsigned char*& buf, int maxbuf,
             double maxw, int& n,
             double& width, int wrap, int draw_symbols)
{
  unsigned char* e = buf + (maxbuf - 4);
  underline_at = 0;
  double w = 0;
  static int l_local_buff = 500;
  static unsigned char* local_buf = (unsigned char*)malloc(
                                      l_local_buff); // initial buffer allocation

  if (maxbuf == 0)
  {
    buf = local_buf;
    e = buf + l_local_buff - 4;
  }

  unsigned char* o = buf;
  unsigned char* word_end = o;
  unsigned const char* word_start = from;

  const unsigned char* p = from;

  for (;; p++)
  {

    int c = *p & 255;

    if (!c || c == ' ' || c == '\n')
    {
      // test for word-wrap:
      if (word_start < p && wrap)
      {
        double newwidth = w + fl_width(word_end, (int) (o - word_end) );

        if (word_end > buf && int(newwidth) > maxw)   // break before this word
        {
          o = word_end;
          p = word_start;
          break;
        }

        word_end = o;
        w = newwidth;
      }

      if (!c) break;

      else if (c == '\n')
      {
        p++;
        break;
      }

      word_start = p + 1;
    }

    if (o > e)
    {
      if (maxbuf) break; // don't overflow buffer

      l_local_buff += (o - e) + 200; // enlarge buffer
      buf = (unsigned char*)realloc(local_buf, l_local_buff);
      e = buf + l_local_buff - 4; // update pointers to buffer content
      o = buf + (o - local_buf);
      word_end = buf + (word_end - local_buf);
      local_buf = buf;
    }

    if (c == '\t')
    {
      for (c = fl_utf_nb_char((unsigned char*)buf, (int) (o - buf) ) % 8; c < 8
           && o < e; c++)
        *o++ = ' ';
    }

    else if (c == '&' && fl_draw_shortcut && *(p + 1))
    {
      if (*(p + 1) == '&')
      {
        p++;
        *o++ = '&';
      }

      else if (fl_draw_shortcut != 2) underline_at = o;
    }

    else if (c < ' ' || c == 127)     // ^X
    {
      *o++ = '^';
      *o++ = c ^ 0x40;
    }

    else if (c == '@' && draw_symbols)     // Symbol???
    {
      if (p[1] && p[1] != '@')  break;

      *o++ = c;

      if (p[1]) p++;
    }

    else
    {
      *o++ = c;
    }
  }

  width = w + fl_width(word_end, (int) (o - word_end));
  *o = 0;
  n = (int) (o - buf);
  return p;
}

/**
 Copy \p from to \p buf, replacing control characters with ^X.

 Stop at a newline or if \p maxbuf characters written to buffer.
 Also word-wrap if width exceeds maxw.
 Returns a pointer to the start of the next line of characters.
 Sets n to the number of characters put into the buffer.
 Sets width to the width of the string in the \ref drawing_fl_font "current font".
 */
const unsigned char*
fl_expand_text(const unsigned char* from, unsigned char* buf, int maxbuf,
               double maxw, int& n,
               double& width, int wrap, int draw_symbols)
{
  return expand_text_(from,  buf, maxbuf, maxw,  n, width,  wrap,  draw_symbols);
}

/**
  The same as fl_draw(const char*,int,int,int,int,Fl_Align,Fl_Image*,int) with
  the addition of the \p callthis parameter, which is a pointer to a text drawing
  function such as fl_draw(const char*, int, int, int) to do the real work
*/
void
fl_draw(
  unsigned char const* str,
  int const x,
  int const y,
  unsigned int const w,
  unsigned int const h,
  enum Fl_Label::Fl_Align const align,
  enum Fl::foreground const fcolor,
  enum Fl::background const bcolor,
  enum Fl::foreground const hcolor,
  void (*callthis)( const unsigned char* str, unsigned int const n, int const x,
                    int const y, enum Fl::foreground const fcolor,
                    enum Fl::background const bcolor),
  int const draw_symbols)
{
  unsigned char* linebuf = NULL;
  unsigned const char* p;
  unsigned const char* e;
  int buflen;
  unsigned char symbol[2][255], *symptr;
  int symwidth[2], symoffset, symtotal;

  // count how many lines and put the last one into the buffer:
  int lines;
  double width;

  symbol[0][0] = '\0';
  symwidth[0]  = 0;

  symbol[1][0] = '\0';
  symwidth[1]  = 0;

  if (draw_symbols)
  {
    if (str && str[0] == '@' && str[1] && str[1] != '@')
    {
      // Start with a symbol...
      for (symptr = symbol[0];
           *str && !isspace(*str) && symptr < (symbol[0] + sizeof(symbol[0]) - 1);
           *symptr++ = *str++) {/*empty*/}

      *symptr = '\0';

      if (isspace(*str)) str++;

      symwidth[0] = (w < h ? w : h);
    }

    if (str && (p = (unsigned char*)strrchr((char*)str, '@')) != NULL
        && p > (str + 1) && p[-1] != '@')
    {
      strlcpy((char*)symbol[1], (char*)p, sizeof(symbol[1]));
      symwidth[1] = (w < h ? w : h);
    }
  }

  symtotal = symwidth[0] + symwidth[1];

  int strw = 0;
  int strh;

  if (str)
  {
    for (p = str, lines = 0; p;)
    {
      e = expand_text_(p, linebuf, 0, w - symtotal, buflen, width,
                       align & Fl_Label::FL_ALIGN_WRAP, draw_symbols);

      if (strw < width) strw = (int)width;

      lines++;

      if (!*e || (*e == '@' && e[1] != '@' && draw_symbols)) break;

      p = e;
    }
  }

  else lines = 0;

  if ((symwidth[0] || symwidth[1]) && lines)
  {
    if (symwidth[0]) symwidth[0] = lines;

    if (symwidth[1]) symwidth[1] = lines;
  }

  symtotal = symwidth[0] + symwidth[1];
  strh = lines;

  // figure out vertical position of the first line:
  int xpos;
  int ypos;

  symoffset = 0;

  if (align & Fl_Label::FL_ALIGN_BOTTOM) ypos = y + h - 2 - (lines - 1);

  else if (align & Fl_Label::FL_ALIGN_TOP) ypos = y + 1;

  else ypos = y + (h - lines) / 2;

  // now draw all the lines:
  if (str)
  {
    for (p = str; ; ypos++)
    {
      if (lines > 1) e = expand_text_(p, linebuf, 0, w - symtotal, buflen,
                                        width, align & Fl_Label::FL_ALIGN_WRAP, draw_symbols);

      else e = (unsigned char*)"";

      if (width > symoffset) symoffset = (int)(width + 0.5);

      if (align & Fl_Label::FL_ALIGN_LEFT) xpos = x + symwidth[0];

      else if (align & Fl_Label::FL_ALIGN_RIGHT) xpos = x + w - (int)(
                                                            width + .5) - symwidth[1];

      else xpos = x + (w - (int)(width + .5) - symtotal) / 2 + symwidth[0];

      callthis(linebuf, buflen, xpos, ypos, fcolor, bcolor);

      if (underline_at && underline_at >= linebuf
          && underline_at < (linebuf + buflen))
      {
        callthis(
          underline_at,
          1,
          xpos + (underline_at - linebuf),
          ypos,
          hcolor,
          bcolor);
      }

      if (!*e || (*e == '@' && e[1] != '@')) break;

      p = e;
    }
  }

  if (symwidth[0])
  {
    // draw to the left
    if (align & Fl_Label::FL_ALIGN_LEFT) xpos = x;

    else if (align & Fl_Label::FL_ALIGN_RIGHT) xpos = x + w - symtotal - symoffset;

    else xpos = x + (w - symoffset - symtotal) / 2;

    if (align & Fl_Label::FL_ALIGN_BOTTOM) ypos = y + h - symwidth[0];

    else if (align & Fl_Label::FL_ALIGN_TOP) ypos = y;

    else ypos = y + (h - symwidth[0]) / 2;

    fl_draw_symbol(symbol[0], xpos, ypos, symwidth[0], symwidth[0], fl_color());
  }

  if (symwidth[1])
  {
    // draw to the right
    if (align & Fl_Label::FL_ALIGN_LEFT) xpos = x + symoffset + symwidth[0];

    else if (align & Fl_Label::FL_ALIGN_RIGHT) xpos = x + w - symwidth[1];

    else xpos = x + (w - symoffset - symtotal) / 2 + symoffset + symwidth[0];

    if (align & Fl_Label::FL_ALIGN_BOTTOM) ypos = y + h - symwidth[1];

    else if (align & Fl_Label::FL_ALIGN_TOP) ypos = y;

    else ypos = y + (h - symwidth[1]) / 2;

    fl_draw_symbol(symbol[1], xpos, ypos, symwidth[1], symwidth[1], fl_color());
  }

  return;
}

/**
  Fancy string drawing function which is used to draw all the labels.

  The string is formatted and aligned inside the passed box.
  Handles '\\t' and '\\n', expands all other control characters to '^X',
  and aligns inside or against the edges of the box.
  See Fl_Widget::align() for values of \p align. The value FL_ALIGN_INSIDE
  is ignored, as this function always prints inside the box.
  If \p img is provided and is not \p NULL, the image is drawn above or
  below the text as specified by the \p align value.
  The \p draw_symbols argument specifies whether or not to look for symbol
  names starting with the '\@' character'
*/
void
fl_draw(
  const unsigned char* str,
  int const x,
  int const y,
  unsigned int const w,
  unsigned int const h,
  enum Fl_Label::Fl_Align const align,
  enum Fl::foreground const fcolor,
  enum Fl::background const bcolor,
  enum Fl::foreground const hcolor,
  int const draw_symbols)
{
  if (!str || !*str) return;

  if (w && h && !Fl::clip_intersects(x, y, w, h)
      && (align & Fl_Label::FL_ALIGN_INSIDE)) return;

  if (align & Fl_Label::FL_ALIGN_CLIP)
    Fl::clip_push(x, y, w, h);

  fl_draw(str, x, y, w, h, align, fcolor, bcolor, hcolor, fl_draw, draw_symbols);

  if (align & Fl_Label::FL_ALIGN_CLIP)
    Fl::clip_pop();
}

/**
  Measure how wide and tall the string will be when printed by the
  fl_draw() function with \p align parameter. If the incoming \p w
  is non-zero it will wrap to that width.

  The \ref drawing_fl_font "current font" is used to do the width/height
  calculations, so unless its value is known at the time fl_measure() is
  called, it is advised to first set the current font with fl_font().
  With event-driven GUI programming you can never be sure which
  widget was exposed and redrawn last, nor which font it used.
  If you have not called fl_font() explicitly in your own code,
  the width and height may be set to unexpected values, even zero!

  \b Note: In the general use case, it's a common error to forget to set
  \p w to 0 before calling fl_measure() when wrap behavior isn't needed.

  \param[in] str nul-terminated string
  \param[out] w,h width and height of string in current font
  \param[in] draw_symbols non-zero to enable @@symbol handling [default=1]

  \code
  // Example: Common use case for fl_measure()
  const char *s = "This is a test";
  int wi=0, hi=0;              // initialize to zero before calling fl_measure()
  fl_font(FL_HELVETICA, 14);   // set current font face/size to be used for measuring
  fl_measure(s, wi, hi);       // returns pixel width/height of string in current font
  \endcode
*/
void
fl_measure(const unsigned char* str, int& w, int& h, int draw_symbols)
{
  if (!str || !*str)
  {
    w = 0;
    h = 0;
    return;
  }

  h = fl_height();
  unsigned char* linebuf = NULL;
  const unsigned char* p;
  const unsigned char* e;
  int buflen;
  int lines;
  double width = 0;
  int W = 0;
  int symwidth[2], symtotal;

  symwidth[0] = 0;  // size of symbol at beginning of string (if any)
  symwidth[1] = 0;  // size of symbol at end of string (if any)

  if (draw_symbols)
  {
    // Symbol at beginning of string?
    const unsigned char* sym2 = (str[0] == '@'
                                 && str[1] == '@') ? str + 2 : str; // sym2 check will skip leading @@

    if (str[0] == '@' && str[1] != '@')
    {
      while (*str && !isspace(*str))
      {
        ++str;  // skip over symbol
      }

      if (isspace(*str)) ++str;       // skip over trailing space

      sym2 = str;         // sym2 check will skip leading symbol
      symwidth[0] = h;
    }

    // Symbol at end of string?
    if ((p = (unsigned char*)strchr((char const*)sym2, '@')) != NULL
        && p[1] != '@')
    {
      symwidth[1] = h;
    }
  }

  symtotal = symwidth[0] + symwidth[1];

  for (p = str, lines = 0; p;)
  {
//    e = expand(p, linebuf, w - symtotal, buflen, width, w != 0, draw_symbols);
    e = expand_text_(p, linebuf, 0, w - symtotal, buflen, width,
                     w != 0, draw_symbols);

    if ((int)ceil(width) > W) W = (int)ceil(width);

    lines++;

    if (!*e || (*e == '@' && e[1] != '@' && draw_symbols)) break;

    p = e;
  }

  if ((symwidth[0] || symwidth[1]) && lines)
  {
    if (symwidth[0]) symwidth[0] = lines * fl_height();

    if (symwidth[1]) symwidth[1] = lines * fl_height();
  }

  symtotal = symwidth[0] + symwidth[1];

  w = W + symtotal;
  h = lines * h;
}

/**
  This function returns the actual height of the specified \p font
  and \p size. Normally the font height should always be 'size',
  but with the advent of XFT, there are (currently) complexities
  that seem to only be solved by asking the font what its actual
  font height is. (See STR#2115)

  This function was originally undocumented in 1.1.x, and was used
  only by Fl_Text_Display. We're now documenting it in 1.3.x so that
  apps that need precise height info can get it with this function.

  \returns the height of the font in pixels.

  \todo  In the future, when the XFT issues are resolved, this function
         should simply return the 'size' value.
*/
int
fl_height(int font, int size)
{
  return fl_height();
  /*
      if ( font == fl_font() && size == fl_size() ) return(fl_height());
      int tf = fl_font(), ts = fl_size();   // save
      fl_font(font,size);
      int height = fl_height();
      fl_font(tf,ts);                       // restore
      return(height);
  */
}
