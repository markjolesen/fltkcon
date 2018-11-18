// textdsp.cxx
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 2001-2018 by Bill Spitzak and others.
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
// Original code Copyright Mark Edel.  Permission to distribute under
// the LGPL for the FLTK library granted by Mark Edel.
//
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include "textdsp.h"
#include "textbuf.h"
#include "drvscr.h"
#include "fl.h"
#include "fl_caret.h"
#include "fl_clip.h"
#include "fl_color.h"
#include "fl_draw.h"
#include "fl_rend.h"
#include "flstring.h"
#include "fl_utf8.h"
#include "platform.h"
#include "win.h"

#undef min
#undef max

#define LINENUM_LEFT_OF_VSCROLL
#define TOP_MARGIN 0 /* 1 */
#define BOTTOM_MARGIN 0 /* 1 */
#define LEFT_MARGIN 0 /* 3 */
#define RIGHT_MARGIN 0 /* 3 */

#define NO_HINT -1

#define FILL_MASK         0x0100
#define SECONDARY_MASK    0x0200
#define PRIMARY_MASK      0x0400
#define HIGHLIGHT_MASK    0x0800
#define BG_ONLY_MASK      0x1000
#define TEXT_ONLY_MASK    0x2000
#define STYLE_LOOKUP_MASK   0xff

#define MAX_DISP_LINE_LEN 1000

static int max( int i1, int i2 );
static int min( int i1, int i2 );
static int countlines( const unsigned char* string );

static int scroll_direction = 0;
static int scroll_amount = 0;
static int scroll_y = 0;
static int scroll_x = 0;

#define TMPFONTWIDTH 6

Fl_Text_Display::Fl_Text_Display(
  int const X,
  int const Y,
  unsigned int const W,
  unsigned int const H,
  const unsigned char* l) :
  Fl_Group(X, Y, W, H, l),
  skin_(Fl::_skin_editor)
{

  mMargin.left = LEFT_MARGIN;
  mMargin.right = RIGHT_MARGIN;
  mMargin.top = TOP_MARGIN;
  mMargin.bottom = BOTTOM_MARGIN;
  damage_range1_start = damage_range1_end = -1;
  damage_range2_start = damage_range2_end = -1;
  mCursorPos = 0;
  mCursorOn = 0;
  mCursorOldY = -100;
  mCursorToHint = NO_HINT;
  mCursorStyle = NORMAL_CURSOR;
  mCursorPreferredXPos = -1;
  mNVisibleLines = 1;
  mNBufferLines = 0;
  mBuffer = NULL;
  mStyleBuffer = NULL;
  mFirstChar = 0;
  mLastChar = 0;
  mContinuousWrap = 0;
  mWrapMarginPix = 0;
  mLineStarts = new int[mNVisibleLines];
  {
    for (int i = 1; i < mNVisibleLines; i++) mLineStarts[i] = -1;
  }
  mLineStarts[0] = 0;
  mTopLineNum = 1;
  mAbsTopLineNum = 1;
  mNeedAbsTopLineNum = 0;
  mHorizOffset = 0;
  mTopLineNumHint = 1;
  mHorizOffsetHint = 0;
  mNStyles = 0;
  mUnfinishedStyle = 0;
  mUnfinishedHighlightCB = 0;
  mHighlightCBArg = 0;
  mSuppressResync = 0;
  mNLinesDeleted = 0;
  mModifyingTabDistance = 0;
  mColumnScale = 0;
  mCursor_color = Fl::fcolor_white;

  mHScrollBar = new Fl_Scrollbar(0, 0, 1, 1);
  mHScrollBar->callback((Fl_Callback*)h_scrollbar_cb, this);
  mHScrollBar->type(FL_HORIZONTAL);

  mVScrollBar = new Fl_Scrollbar(0, 0, 1, 1);
  mVScrollBar->callback((Fl_Callback*)v_scrollbar_cb, this);

  scrollbar_width_ = 1;
  scrollbar_align_ = Fl_Label::FL_ALIGN_BOTTOM_RIGHT;

  dragPos = 0;
  dragType = DRAG_CHAR;
  dragging = 0;
  display_insert_position_hint = 0;

  text_area.x = 0;
  text_area.y = 0;
  text_area.w = 0;
  text_area.h = 0;

  shortcut_ = 0;
  textcolor_ = Fl::fcolor_white;
  mLineNumLeft = 0;
  mLineNumWidth = 0;

  linenumber_align_   = Fl_Label::FL_ALIGN_RIGHT;
  linenumber_format_  = (unsigned char*)strdup("%d");

  set_flag(SHORTCUT_LABEL);
  clear_flag(NEEDS_KEYBOARD);

  end();
}

Fl_Text_Display::~Fl_Text_Display()
{
  if (scroll_direction)
  {
    Fl::remove_timeout(scroll_timer_cb, this);
    scroll_direction = 0;
  }

  if (mBuffer)
  {
    mBuffer->remove_modify_callback(buffer_modified_cb, this);
    mBuffer->remove_predelete_callback(buffer_predelete_cb, this);
  }

  if (mLineStarts) delete[] mLineStarts;

  if (linenumber_format_)
  {
    free((void*)linenumber_format_);
    linenumber_format_ = 0;
  }
}

void
Fl_Text_Display::linenumber_width(int width)
{
  if (width < 0) return;

  mLineNumWidth = width;
  recalc_display();
}

int
Fl_Text_Display::linenumber_width() const
{
  return mLineNumWidth;
}

void
Fl_Text_Display::linenumber_align(Fl_Label::Fl_Align val)
{
  linenumber_align_ = val;
}

Fl_Label::Fl_Align
Fl_Text_Display::linenumber_align() const
{
  return linenumber_align_;
}

void
Fl_Text_Display::linenumber_format(const unsigned char* val)
{
  if ( linenumber_format_ ) free((void*)linenumber_format_);

  linenumber_format_ = (val ? (unsigned char*)strdup((char*)val) : 0);
}

const unsigned char*
Fl_Text_Display::linenumber_format() const
{
  return linenumber_format_;
}

void
Fl_Text_Display::buffer( Fl_Text_Buffer* buf )
{
  if ( buf == mBuffer) return;

  if ( mBuffer != 0 )
  {
    unsigned char* deletedText = mBuffer->text();
    buffer_modified_cb( 0, 0, mBuffer->length(), 0, deletedText, this );
    free(deletedText);
    mNBufferLines = 0;
    mBuffer->remove_modify_callback( buffer_modified_cb, this );
    mBuffer->remove_predelete_callback( buffer_predelete_cb, this );
  }

  mBuffer = buf;

  if (mBuffer)
  {
    mBuffer->add_modify_callback( buffer_modified_cb, this );
    mBuffer->add_predelete_callback( buffer_predelete_cb, this );

    buffer_modified_cb( 0, buf->length(), 0, 0, 0, this );
  }

  recalc_display();
}

int
Fl_Text_Display::longest_vline() const
{
  int longest = 0;

  for (int i = 0; i < mNVisibleLines; i++)
    longest = max(longest, measure_vline(i));

  return longest;
}

void
Fl_Text_Display::resize(int X, int Y, int W, int H)
{

  Fl_Widget::resize(X, Y, W, H);
  mColumnScale =
    0;
  recalc_display();
}

void
Fl_Text_Display::recalc_display()
{
  if (!buffer()) return;

  unsigned int hscrollbarvisible = mHScrollBar->visible();
  unsigned int vscrollbarvisible = mVScrollBar->visible();
  int scrollsize = scrollbar_width_ ? scrollbar_width_ : Fl::scrollbar_size();

  int oldTAWidth = text_area.w;

  int X = x();
  int Y = y();
  int W = w();
  int H = h();

  text_area.x = X + mMargin.left + mLineNumWidth;
  text_area.y = Y + mMargin.top;
  text_area.w = W - mMargin.left - mMargin.right - mLineNumWidth;
  text_area.h = H - mMargin.top - mMargin.bottom;

  int i;
#if 0

  for (i = 0, mMaxsize = fl_height(textfont(), textsize()); i < mNStyles; i++)
    mMaxsize = max(mMaxsize, fl_height(mStyleTable[i].font, mStyleTable[i].size));

#endif

  // try without scrollbars first
  mVScrollBar->clear_visible();
  mHScrollBar->clear_visible();

#if (1) // optimization (experimental - seems to work well)

  oldTAWidth = -1; // force _first_ calculation in loop (STR #3412)

  if (mContinuousWrap && !mWrapMarginPix)
  {

    // int nvlines = (text_area.h + mMaxsize - 1) / mMaxsize;
    int nvlines = text_area.h;
    int nlines = buffer()->count_lines(0, buffer()->length());

    if (nvlines < 1) nvlines = 1;

    if (nlines >= nvlines - 1)
    {
      mVScrollBar->set_visible(); // we need a vertical scrollbar
      text_area.w -= scrollsize;
    }
  }

#endif // optimization

  for (int again = 1; again;)
  {
    again = 0;

#ifdef DEBUG2
    printf("*** again ... text_area.w = %d, oldTAWidth = %d, diff = %d\n",
           text_area.w, oldTAWidth, text_area.w - oldTAWidth);
#endif // DEBUG2

    if (mContinuousWrap && !mWrapMarginPix && text_area.w != oldTAWidth)
    {

      int oldFirstChar = mFirstChar;
      mNBufferLines = count_lines(0, buffer()->length(), true);
      mFirstChar = line_start(mFirstChar);
      mTopLineNum = count_lines(0, mFirstChar, true) + 1;
      absolute_top_line_number(oldFirstChar);
#ifdef DEBUG2
      printf("    mNBufferLines=%d\n", mNBufferLines);
#endif // DEBUG2

    }

    oldTAWidth = text_area.w;

    // int nvlines = (text_area.h + mMaxsize - 1) / mMaxsize;
    int nvlines = text_area.h;

    if (nvlines < 1) nvlines = 1;

    if (mNVisibleLines != nvlines)
    {
      mNVisibleLines = nvlines;

      if (mLineStarts) delete[] mLineStarts;

      mLineStarts = new int [mNVisibleLines];
    }

    calc_line_starts(0, mNVisibleLines);
    calc_last_char();

    if (scrollsize)
    {

      if (!mVScrollBar->visible() &&
          scrollbar_align() & (Fl_Label::FL_ALIGN_LEFT | Fl_Label::FL_ALIGN_RIGHT) &&
          mNBufferLines >= mNVisibleLines - (mContinuousWrap ? 0 : 1))
      {
        mVScrollBar->set_visible();
        text_area.w -= scrollsize;
        again = 1;
      }

      if (!mHScrollBar->visible() &&
          scrollbar_align() & (Fl_Label::FL_ALIGN_TOP | Fl_Label::FL_ALIGN_BOTTOM) &&
          (mVScrollBar->visible() || longest_vline() > text_area.w))
      {
        char wrap_at_bounds = mContinuousWrap && (mWrapMarginPix < text_area.w);

        if (!wrap_at_bounds)
        {
          mHScrollBar->set_visible();
          text_area.h -= scrollsize;
          again = 1;
        }
      }
    }
  } // (... again ...)

  text_area.x = X + mLineNumWidth + mMargin.left;

  if (mVScrollBar->visible() && scrollbar_align() & Fl_Label::FL_ALIGN_LEFT)
    text_area.x += scrollsize;

  text_area.y = Y + mMargin.top;

  if (mHScrollBar->visible() &&
      scrollbar_align() & Fl_Label::FL_ALIGN_TOP)
    text_area.y += scrollsize;

  if (mVScrollBar->visible())
  {
    if (scrollbar_align() & Fl_Label::FL_ALIGN_LEFT)
    {
#ifdef LINENUM_LEFT_OF_VSCROLL
      mVScrollBar->resize(text_area.x - mMargin.left - scrollsize,
#else
      mVScrollBar->resize(X,
#endif
                          text_area.y - mMargin.top,
                          scrollsize,
                          text_area.h + mMargin.top + mMargin.bottom);
    }

    else
    {
      mVScrollBar->resize(X + W - scrollsize,
                          text_area.y - mMargin.top,
                          scrollsize,
                          text_area.h + mMargin.top + mMargin.bottom);
    }
  }

  if (mHScrollBar->visible())
  {
    if (scrollbar_align() & Fl_Label::FL_ALIGN_TOP)
    {
      mHScrollBar->resize(text_area.x - mMargin.left,
                          Y,
                          text_area.w + mMargin.left + mMargin.right,
                          scrollsize);
    }

    else
    {
      mHScrollBar->resize(text_area.x - mMargin.left,
                          Y + H - scrollsize,
                          text_area.w + mMargin.left + mMargin.right,
                          scrollsize);
    }
  }


  if (mTopLineNumHint != mTopLineNum || mHorizOffsetHint != mHorizOffset)
    scroll_(mTopLineNumHint, mHorizOffsetHint);

  if (mNBufferLines < mNVisibleLines || mBuffer == NULL
      || mBuffer->length() == 0)
  {
    scroll_(1, mHorizOffset);
  }

  else
  {
    while (   mNVisibleLines >= 2
              && (mLineStarts[mNVisibleLines - 2] == -1)
              && scroll_(mTopLineNum - 1, mHorizOffset))
    { }
  }

  if (display_insert_position_hint)
    display_insert();

  int maxhoffset = max(0, longest_vline() - text_area.w);

  if (mHorizOffset > maxhoffset)
    scroll_(mTopLineNumHint, maxhoffset);

  mTopLineNumHint = mTopLineNum;
  mHorizOffsetHint = mHorizOffset;
  display_insert_position_hint = 0;

  if (mContinuousWrap ||
      hscrollbarvisible != mHScrollBar->visible() ||
      vscrollbarvisible != mVScrollBar->visible())
    redraw();

  update_v_scrollbar();
  update_h_scrollbar();
}


void
Fl_Text_Display::draw_text( int left, int top, int width, int height )
{
  int firstLine, lastLine, line;

// firstLine = ( top - text_area.y - fontHeight + 1 ) / fontHeight;
  firstLine = ( top - text_area.y );
// lastLine = ( top + height - text_area.y ) / fontHeight + 1;
  lastLine = ( top + height - text_area.y ) ;

  Fl::clip_push( left, top, width, height );

  for ( line = firstLine; line <= lastLine; line++ )
    draw_vline( line, left, left + width, 0, INT_MAX );

  Fl::clip_pop();

  return;
}


void
Fl_Text_Display::redisplay_range(int startpos, int endpos)
{
  IS_UTF8_ALIGNED2(buffer(), startpos)
  IS_UTF8_ALIGNED2(buffer(), endpos)

  if (damage_range1_start == -1 && damage_range1_end == -1)
  {
    damage_range1_start = startpos;
    damage_range1_end = endpos;
  }

  else if ((startpos >= damage_range1_start && startpos <= damage_range1_end) ||
           (endpos >= damage_range1_start && endpos <= damage_range1_end))
  {
    damage_range1_start = min(damage_range1_start, startpos);
    damage_range1_end = max(damage_range1_end, endpos);
  }

  else if (damage_range2_start == -1 && damage_range2_end == -1)
  {
    damage_range2_start = startpos;
    damage_range2_end = endpos;
  }

  else
  {
    damage_range2_start = min(damage_range2_start, startpos);
    damage_range2_end = max(damage_range2_end, endpos);
  }

  damage(Fl_Widget::FL_DAMAGE_SCROLL);
}



void
Fl_Text_Display::draw_range(int startpos, int endpos)
{
  startpos = buffer()->utf8_align(startpos);
  endpos = buffer()->utf8_align(endpos);

  int i, startLine, lastLine, startIndex, endIndex;

  if ( endpos < mFirstChar || ( startpos > mLastChar && !empty_vlines() ) )
    return;

  if ( startpos < 0 ) startpos = 0;

  if ( startpos > mBuffer->length() ) startpos = mBuffer->length();

  if ( endpos < 0 ) endpos = 0;

  if ( endpos > mBuffer->length() ) endpos = mBuffer->length();

  if ( startpos < mFirstChar )
    startpos = mFirstChar;

  if ( !position_to_line( startpos, &startLine ) )
    startLine = mNVisibleLines - 1;

  if ( endpos >= mLastChar )
  {
    lastLine = mNVisibleLines - 1;
  }

  else
  {
    if ( !position_to_line( endpos, &lastLine ) )
    {
      lastLine = mNVisibleLines - 1;
    }
  }

  startIndex = mLineStarts[ startLine ] == -1 ? 0 : startpos -
               mLineStarts[ startLine ];

  if ( endpos >= mLastChar )
    endIndex = INT_MAX;

  else if ( mLineStarts[ lastLine ] == -1 )
    endIndex = 0;

  else
    endIndex = endpos - mLineStarts[ lastLine ];

  if ( startLine == lastLine )
  {
    draw_vline( startLine, 0, INT_MAX, startIndex, endIndex );
    return;
  }

  draw_vline( startLine, 0, INT_MAX, startIndex, INT_MAX );

  for ( i = startLine + 1; i < lastLine; i++ )
    draw_vline( i, 0, INT_MAX, 0, INT_MAX );

  draw_vline( lastLine, 0, INT_MAX, 0, endIndex );

}



void
Fl_Text_Display::insert_position( int newPos )
{
  IS_UTF8_ALIGNED2(buffer(), newPos)

  if ( newPos == mCursorPos ) return;

  if ( newPos < 0 ) newPos = 0;

  if ( newPos > mBuffer->length() ) newPos = mBuffer->length();

  mCursorPreferredXPos = -1;

  redisplay_range(buffer()->prev_char_clipped(mCursorPos),
                  buffer()->next_char(mCursorPos));

  mCursorPos = newPos;

  redisplay_range(buffer()->prev_char_clipped(mCursorPos),
                  buffer()->next_char(mCursorPos));
}


void
Fl_Text_Display::show_cursor(int b)
{
  mCursorOn = b;

  if (!buffer()) return;

  redisplay_range(buffer()->prev_char_clipped(mCursorPos),
                  buffer()->next_char(mCursorPos));

  return;
}

void
Fl_Text_Display::cursor_style(int style)
{
  mCursorStyle = style;

  if (mCursorOn) show_cursor();
}


void
Fl_Text_Display::wrap_mode(int wrap, int wrapMargin)
{
  switch (wrap)
  {
    case WRAP_NONE:
      mWrapMarginPix = 0;
      mContinuousWrap = 0;
      break;

    case WRAP_AT_COLUMN:
    default:
      mWrapMarginPix = int(col_to_x(wrapMargin));
      mContinuousWrap = 1;
      break;

    case WRAP_AT_PIXEL:
      mWrapMarginPix = wrapMargin;
      mContinuousWrap = 1;
      break;

    case WRAP_AT_BOUNDS:
      mWrapMarginPix = 0;
      mContinuousWrap = 1;
      break;
  }

  if (buffer())
  {
    mNBufferLines = count_lines(0, buffer()->length(), true);

    mFirstChar = line_start(mFirstChar);
    mTopLineNum = count_lines(0, mFirstChar, true) + 1;

    reset_absolute_top_line_number();

    calc_line_starts(0, mNVisibleLines);
    calc_last_char();
  }

  else
  {
    mNBufferLines  = 0;
    mFirstChar     = 0;
    mTopLineNum    = 1;
    mAbsTopLineNum = 1;
  }

  recalc_display();
}


void
Fl_Text_Display::insert(const unsigned char* text)
{
  IS_UTF8_ALIGNED2(buffer(), mCursorPos)
  IS_UTF8_ALIGNED(text)

  int pos = mCursorPos;

  mCursorToHint = (int) (pos + strlen( (char*)text ));
  mBuffer->insert( pos, text );
  mCursorToHint = NO_HINT;
}


void
Fl_Text_Display::overstrike(const unsigned char* text)
{
  IS_UTF8_ALIGNED2(buffer(), mCursorPos)
  IS_UTF8_ALIGNED(text)

  int startPos = mCursorPos;
  Fl_Text_Buffer* buf = mBuffer;
  int lineStart = buf->line_start( startPos );
  int textLen = (int) strlen( (char*)text );
  int i, p, endPos, indent, startIndent, endIndent;
  const unsigned char* c;
  unsigned int ch;
  unsigned char* paddedText = NULL;

  startIndent = mBuffer->count_displayed_characters( lineStart, startPos );
  indent = startIndent;

  for ( c = text; *c != '\0'; c += fl_utf8len1(*c) )
    indent++;

  endIndent = indent;

  indent = startIndent;

  for ( p = startPos; ; p = buf->next_char(p) )
  {
    if ( p == buf->length() )
      break;

    ch = buf->char_at( p );

    if ( ch == '\n' )
      break;

    indent++;

    if ( indent == endIndent )
    {
      p = buf->next_char(p);
      break;
    }

    else if ( indent > endIndent )
    {
      if ( ch != '\t' )
      {
        p = buf->next_char(p);
        paddedText = new unsigned char [ textLen + FL_TEXT_MAX_EXP_CHAR_LEN + 1 ];
        strcpy( (char*)paddedText, (char*)text );

        for ( i = 0; i < indent - endIndent; i++ )
          paddedText[ textLen + i ] = ' ';

        paddedText[ textLen + i ] = '\0';
      }

      break;
    }
  }

  endPos = p;

  mCursorToHint = startPos + textLen;
  buf->replace( startPos, endPos, paddedText == NULL ? text : paddedText );
  mCursorToHint = NO_HINT;

  if ( paddedText != NULL )
    delete [] paddedText;
}


int
Fl_Text_Display::position_to_xy( int pos, int* X, int* Y ) const
{
  IS_UTF8_ALIGNED2(buffer(), pos)

  int lineStartPos;
  int visLineNum;

  if ((pos < mFirstChar) ||
      (pos > mLastChar && !empty_vlines()) ||
      (pos > buffer()->length()) )      // STR #3231
  {
    return (*X = *Y = 0);
  }

  if (!position_to_line(pos, &visLineNum) || visLineNum < 0
      || visLineNum > mNBufferLines)
  {
    return (*X = *Y = 0);
  }

  *Y = text_area.y + visLineNum;

  lineStartPos = mLineStarts[visLineNum];

  if ( lineStartPos == -1 )
  {
    *X = text_area.x - mHorizOffset;
    return 1;
  }

  *X = text_area.x + handle_vline(GET_WIDTH, lineStartPos, pos - lineStartPos, 0,
                                  0, 0, 0, 0, 0) - mHorizOffset;
  return 1;
}


int
Fl_Text_Display::position_to_linecol( int pos, int* lineNum,
                                      int* column ) const
{
  IS_UTF8_ALIGNED2(buffer(), pos)

  int retVal;

  if (mContinuousWrap)
  {
    if (!maintaining_absolute_top_line_number() || pos < mFirstChar
        || pos > mLastChar)
      return 0;

    *lineNum = mAbsTopLineNum + buffer()->count_lines(mFirstChar, pos);
    *column = buffer()->count_displayed_characters(buffer()->line_start(pos), pos);
    return 1;
  }

  retVal = position_to_line( pos, lineNum );

  if ( retVal )
  {
    *column = mBuffer->count_displayed_characters( mLineStarts[ *lineNum ], pos );
    *lineNum += mTopLineNum;
  }

  return retVal;
}


int
Fl_Text_Display::in_selection( int X, int Y ) const
{
  int pos = xy_to_position( X, Y, CHARACTER_POS );
  IS_UTF8_ALIGNED2(buffer(), pos)
  Fl_Text_Buffer* buf = mBuffer;
  return buf->primary_selection()->includes(pos);
}


int
Fl_Text_Display::wrapped_column(int row, int column) const
{
  int lineStart, dispLineStart;

  if (!mContinuousWrap || row < 0 || row > mNVisibleLines)
    return column;

  dispLineStart = mLineStarts[row];

  if (dispLineStart == -1)
    return column;

  lineStart = buffer()->line_start(dispLineStart);
  return column + buffer()->count_displayed_characters(lineStart, dispLineStart);
}



int
Fl_Text_Display::wrapped_row(int row) const
{
  if (!mContinuousWrap || row < 0 || row > mNVisibleLines)
    return row;

  return buffer()->count_lines(mFirstChar, mLineStarts[row]);
}



void
Fl_Text_Display::display_insert()
{
  int hOffset, topLine, X, Y;
  hOffset = mHorizOffset;
  topLine = mTopLineNum;

  if (insert_position() < mFirstChar)
  {
    topLine -= count_lines(insert_position(), mFirstChar, false);
  }

  else if (mNVisibleLines >= 2 && mLineStarts[mNVisibleLines - 2] != -1)
  {
    int lastChar = line_end(mLineStarts[mNVisibleLines - 2], true);

    if (insert_position() >= lastChar)
      topLine += count_lines(lastChar - (wrap_uses_character(mLastChar) ? 0 : 1),
                             insert_position(), false);
  }

  if (!position_to_xy( mCursorPos, &X, &Y ))
  {
    scroll_(topLine, hOffset);

    if (!position_to_xy( mCursorPos, &X, &Y ))
    {
#ifdef DEBUG
      printf ("*** display_insert/position_to_xy # GIVE UP !\n");
      fflush(stdout);
#endif // DEBUG
      return;
    }
  }

  if (X > text_area.x + text_area.w)
    hOffset += X - (text_area.x + text_area.w);

  else if (X < text_area.x)
    hOffset += X - text_area.x;

  if (topLine != mTopLineNum || hOffset != mHorizOffset)
    scroll_(topLine, hOffset);
}


void
Fl_Text_Display::show_insert_position()
{
  display_insert_position_hint = 1;
  recalc_display();   // resize(x(), y(), w(), h());
}


int
Fl_Text_Display::move_right()
{
  if ( mCursorPos >= mBuffer->length() )
    return 0;

  int p = insert_position();
  int q = buffer()->next_char(p);
  insert_position(q);
  return 1;
}



int
Fl_Text_Display::move_left()
{
  if ( mCursorPos <= 0 )
    return 0;

  int p = insert_position();
  int q = buffer()->prev_char_clipped(p);
  insert_position(q);
  return 1;
}



int
Fl_Text_Display::move_up()
{
  int lineStartPos, xPos, prevLineStartPos, newPos, visLineNum;

  if ( position_to_line( mCursorPos, &visLineNum ) )
    lineStartPos = mLineStarts[ visLineNum ];

  else
  {
    lineStartPos = line_start( mCursorPos );
    visLineNum = -1;
  }

  if ( lineStartPos == 0 )
    return 0;

  if (mCursorPreferredXPos >= 0)
    xPos = mCursorPreferredXPos;

  else
    xPos = handle_vline(GET_WIDTH, lineStartPos, mCursorPos - lineStartPos,
                        0, 0, 0, 0, 0, INT_MAX);

  if ( visLineNum != -1 && visLineNum != 0 )
    prevLineStartPos = mLineStarts[ visLineNum - 1 ];

  else
    prevLineStartPos = rewind_lines( lineStartPos, 1 );

  int lineEnd = line_end(prevLineStartPos, true);
  newPos = handle_vline(FIND_INDEX_FROM_ZERO, prevLineStartPos,
                        lineEnd - prevLineStartPos,
                        0, 0, 0, 0, 0, xPos);

  insert_position( newPos );

  mCursorPreferredXPos = xPos;
  return 1;
}



int
Fl_Text_Display::move_down()
{
  int lineStartPos, xPos, newPos, visLineNum;

  if ( mCursorPos == mBuffer->length() )
    return 0;

  if ( position_to_line( mCursorPos, &visLineNum ) )
    lineStartPos = mLineStarts[ visLineNum ];

  else
  {
    lineStartPos = line_start( mCursorPos );
    visLineNum = -1;
  }

  if (mCursorPreferredXPos >= 0)
  {
    xPos = mCursorPreferredXPos;
  }

  else
  {
    xPos = handle_vline(GET_WIDTH, lineStartPos, mCursorPos - lineStartPos,
                        0, 0, 0, 0, 0, INT_MAX);
  }

  int nextLineStartPos = skip_lines( lineStartPos, 1, true );
  int lineEnd = line_end(nextLineStartPos, true);
  newPos = handle_vline(FIND_INDEX_FROM_ZERO, nextLineStartPos,
                        lineEnd - nextLineStartPos,
                        0, 0, 0, 0, 0, xPos);

  insert_position( newPos );
  mCursorPreferredXPos = xPos;
  return 1;
}



int
Fl_Text_Display::count_lines(int startPos, int endPos,
                             bool startPosIsLineStart) const
{
  IS_UTF8_ALIGNED2(buffer(), startPos)
  IS_UTF8_ALIGNED2(buffer(), endPos)

  int retLines, retPos, retLineStart, retLineEnd;

#ifdef DEBUG
  printf("Fl_Text_Display::count_lines(startPos=%d, endPos=%d, startPosIsLineStart=%d\n",
         startPos, endPos, startPosIsLineStart);
#endif // DEBUG

  if (!mContinuousWrap)
    return buffer()->count_lines(startPos, endPos);

  wrapped_line_counter(buffer(), startPos, endPos, INT_MAX,
                       startPosIsLineStart, 0, &retPos, &retLines, &retLineStart,
                       &retLineEnd);

#ifdef DEBUG
  printf("   # after WLC: retPos=%d, retLines=%d, retLineStart=%d, retLineEnd=%d\n",
         retPos, retLines, retLineStart, retLineEnd);
#endif // DEBUG

  return retLines;
}



int
Fl_Text_Display::skip_lines(int startPos, int nLines,
                            bool startPosIsLineStart)
{
  IS_UTF8_ALIGNED2(buffer(), startPos)

  int retLines, retPos, retLineStart, retLineEnd;

  if (!mContinuousWrap)
    return buffer()->skip_lines(startPos, nLines);

  if (nLines == 0)
    return startPos;

  wrapped_line_counter(buffer(), startPos, buffer()->length(),
                       nLines, startPosIsLineStart, 0,
                       &retPos, &retLines, &retLineStart, &retLineEnd);
  IS_UTF8_ALIGNED2(buffer(), retPos)
  return retPos;
}



int
Fl_Text_Display::line_end(int startPos, bool startPosIsLineStart) const
{
  IS_UTF8_ALIGNED2(buffer(), startPos)

  int retLines, retPos, retLineStart, retLineEnd;

  if (!mContinuousWrap)
    return buffer()->line_end(startPos);

  if (startPos == buffer()->length())
    return startPos;

  wrapped_line_counter(buffer(), startPos, buffer()->length(), 1,
                       startPosIsLineStart, 0, &retPos, &retLines, &retLineStart,
                       &retLineEnd);

  IS_UTF8_ALIGNED2(buffer(), retLineEnd)
  return retLineEnd;
}



int
Fl_Text_Display::line_start(int pos) const
{
  IS_UTF8_ALIGNED2(buffer(), pos)

  int retLines, retPos, retLineStart, retLineEnd;

  if (!mContinuousWrap)
    return buffer()->line_start(pos);

  wrapped_line_counter(buffer(), buffer()->line_start(pos), pos, INT_MAX, true, 0,
                       &retPos, &retLines, &retLineStart, &retLineEnd);

  IS_UTF8_ALIGNED2(buffer(), retLineStart)
  return retLineStart;
}



int
Fl_Text_Display::rewind_lines(int startPos, int nLines)
{
  IS_UTF8_ALIGNED2(buffer(), startPos)

  Fl_Text_Buffer* buf = buffer();
  int pos, lineStart, retLines, retPos, retLineStart, retLineEnd;

  if (!mContinuousWrap)
    return buf->rewind_lines(startPos, nLines);

  pos = startPos;

  for (;;)
  {
    lineStart = buf->line_start(pos);
    wrapped_line_counter(buf, lineStart, pos, INT_MAX, true, 0,
                         &retPos, &retLines, &retLineStart, &retLineEnd, false);

    if (retLines > nLines)
      return skip_lines(lineStart, retLines - nLines, true);

    nLines -= retLines;
    pos = lineStart - 1;

    if (pos < 0)
      return 0;

    nLines -= 1;
  }
}



void
Fl_Text_Display::next_word()
{
  int pos = insert_position();

  while (pos < buffer()->length() && !buffer()->is_word_separator(pos))
  {
    pos = buffer()->next_char(pos);
  }

  while (pos < buffer()->length() && buffer()->is_word_separator(pos))
  {
    pos = buffer()->next_char(pos);
  }

  insert_position( pos );
}



void
Fl_Text_Display::previous_word()
{
  int pos = insert_position();

  if (pos == 0) return;

  pos = buffer()->prev_char(pos);

  while (pos && buffer()->is_word_separator(pos))
  {
    pos = buffer()->prev_char(pos);
  }

  while (pos && !buffer()->is_word_separator(pos))
  {
    pos = buffer()->prev_char(pos);
  }

  if (buffer()->is_word_separator(pos))
  {
    pos = buffer()->next_char(pos);
  }

  insert_position( pos );
}



void
Fl_Text_Display::buffer_predelete_cb(int pos, int nDeleted, void* cbArg)
{
  Fl_Text_Display* textD = (Fl_Text_Display*)cbArg;

  if (textD->mContinuousWrap)
  {
    IS_UTF8_ALIGNED2(textD->buffer(), pos)
    textD->measure_deleted_lines(pos, nDeleted);
  }

  else
  {
    textD->mSuppressResync = 0;
  }
}



void
Fl_Text_Display::buffer_modified_cb( int pos, int nInserted, int nDeleted,
                                     int nRestyled, const unsigned char* deletedText, void* cbArg )
{
  int linesInserted, linesDeleted, startDispPos, endDispPos;
  Fl_Text_Display* textD = ( Fl_Text_Display* ) cbArg;
  Fl_Text_Buffer* buf = textD->mBuffer;
  int oldFirstChar = textD->mFirstChar;
  int scrolled, origCursorPos = textD->mCursorPos;
  int wrapModStart = 0, wrapModEnd = 0;

  IS_UTF8_ALIGNED2(buf, pos)
  IS_UTF8_ALIGNED2(buf, oldFirstChar)

  if ( nInserted != 0 || nDeleted != 0 )
    textD->mCursorPreferredXPos = -1;

  if (textD->mContinuousWrap)
  {
    textD->find_wrap_range(deletedText, pos, nInserted, nDeleted,
                           &wrapModStart, &wrapModEnd, &linesInserted, &linesDeleted);
  }

  else
  {
    linesInserted = nInserted == 0 ? 0 : buf->count_lines( pos, pos + nInserted );
    linesDeleted = nDeleted == 0 ? 0 : countlines( deletedText );
  }

  if ( nInserted != 0 || nDeleted != 0 )
  {
    if (textD->mContinuousWrap)
    {
      textD->update_line_starts( wrapModStart, wrapModEnd - wrapModStart,
                                 nDeleted + pos - wrapModStart + (wrapModEnd - (pos + nInserted)),
                                 linesInserted, linesDeleted, &scrolled );
    }

    else
    {
      textD->update_line_starts( pos, nInserted, nDeleted, linesInserted,
                                 linesDeleted, &scrolled );
    }
  }

  else
    scrolled = 0;

  if (textD->maintaining_absolute_top_line_number() &&
      (nInserted != 0 || nDeleted != 0))
  {
    if (deletedText && (pos + nDeleted < oldFirstChar))
      textD->mAbsTopLineNum += buf->count_lines(pos, pos + nInserted) -
                               countlines(deletedText);

    else if (pos < oldFirstChar)
      textD->reset_absolute_top_line_number();
  }

  textD->mNBufferLines += linesInserted - linesDeleted;

  if ( textD->mCursorToHint != NO_HINT )
  {
    textD->mCursorPos = textD->mCursorToHint;
    textD->mCursorToHint = NO_HINT;
  }

  else if ( textD->mCursorPos > pos )
  {
    if ( textD->mCursorPos < pos + nDeleted )
      textD->mCursorPos = pos;

    else
      textD->mCursorPos += nInserted - nDeleted;
  }

  textD->resize(textD->x(), textD->y(), textD->w(), textD->h());

  if (!textD->visible_r()) return;

  if ( scrolled )
  {
    textD->damage(Fl_Widget::FL_DAMAGE_EXPOSE);

    if ( textD->mStyleBuffer )
      textD->mStyleBuffer->primary_selection()->selected(0);

    return;
  }

  startDispPos = textD->mContinuousWrap ? wrapModStart : pos;
  IS_UTF8_ALIGNED2(buf, startDispPos)

  if ( origCursorPos == startDispPos && textD->mCursorPos != startDispPos )
    startDispPos = min( startDispPos, buf->prev_char_clipped(origCursorPos) );

  IS_UTF8_ALIGNED2(buf, startDispPos)

  if ( linesInserted == linesDeleted )
  {
    if ( nInserted == 0 && nDeleted == 0 )
      endDispPos = pos + nRestyled;

    else
    {
      if (textD->mContinuousWrap)
        endDispPos = wrapModEnd;

      else
        endDispPos = buf->next_char(buf->line_end( pos + nInserted ));

    }

    if (linesInserted > 1)
    {
      textD->damage(Fl_Widget::FL_DAMAGE_EXPOSE);
    }
  }

  else
  {
    endDispPos = buf->next_char(textD->mLastChar);
  }

  IS_UTF8_ALIGNED2(buf, startDispPos)
  IS_UTF8_ALIGNED2(buf, endDispPos)

  if ( textD->mStyleBuffer )
    textD->extend_range_for_styles( &startDispPos, &endDispPos );

  IS_UTF8_ALIGNED2(buf, startDispPos)
  IS_UTF8_ALIGNED2(buf, endDispPos)

  textD->redisplay_range( startDispPos, endDispPos );
}


void
Fl_Text_Display::maintain_absolute_top_line_number(int state)
{
  mNeedAbsTopLineNum = state;
  reset_absolute_top_line_number();
}



int
Fl_Text_Display::get_absolute_top_line_number() const
{
  if (!mContinuousWrap)
    return mTopLineNum;

  if (maintaining_absolute_top_line_number())
    return mAbsTopLineNum;

  return 0;
}



void
Fl_Text_Display::absolute_top_line_number(int oldFirstChar)
{
  if (maintaining_absolute_top_line_number())
  {
    if (mFirstChar < oldFirstChar)
      mAbsTopLineNum -= buffer()->count_lines(mFirstChar, oldFirstChar);

    else
      mAbsTopLineNum += buffer()->count_lines(oldFirstChar, mFirstChar);
  }
}



int
Fl_Text_Display::maintaining_absolute_top_line_number() const
{
  return mContinuousWrap &&
         (mLineNumWidth != 0 || mNeedAbsTopLineNum);
}



void
Fl_Text_Display::reset_absolute_top_line_number()
{
  mAbsTopLineNum = 1;
  absolute_top_line_number(0);
}



int
Fl_Text_Display::position_to_line( int pos, int* lineNum ) const
{
  IS_UTF8_ALIGNED2(buffer(), pos)

  int i;

  *lineNum = 0;

  if ( pos < mFirstChar ) return 0;

  if ( pos > mLastChar )
  {
    if ( empty_vlines() )
    {
      if ( mLastChar < mBuffer->length() )
      {
        if ( !position_to_line( mLastChar, lineNum ) )
        {
          Fl::error("Fl_Text_Display::position_to_line(): Consistency check ptvl failed");
          return 0;
        }

        return ++( *lineNum ) <= mNVisibleLines - 1;
      }

      else
      {
        position_to_line( buffer()->prev_char_clipped(mLastChar), lineNum );
        return 1;
      }
    }

    return 0;
  }

  for ( i = mNVisibleLines - 1; i >= 0; i-- )
  {
    if ( mLineStarts[ i ] != -1 && pos >= mLineStarts[ i ] )
    {
      *lineNum = i;
      return 1;
    }
  }

  return 0;
}


int
Fl_Text_Display::handle_vline(
  int mode,
  int lineStartPos, int lineLen, int leftChar, int rightChar,
  int Y, int bottomClip,
  int leftClip, int rightClip) const
{
  IS_UTF8_ALIGNED2(buffer(), lineStartPos)

  int i, X, startIndex, style, charStyle;
  unsigned char* lineStr;
  double startX;

  if ( lineStartPos == -1 )
  {
    lineStr = NULL;
  }

  else
  {
    lineStr = mBuffer->text_range( lineStartPos, lineStartPos + lineLen );
  }

  int cursor_pos = 0;

  if (mode == FIND_CURSOR_INDEX)
  {
    mode = FIND_INDEX;
    cursor_pos = 1;
  }

  if (mode == GET_WIDTH)
  {
    X = 0;
  }

  else if (mode == FIND_INDEX_FROM_ZERO)
  {
    X = 0;
    mode = FIND_INDEX;
  }

  else
  {
    X = text_area.x - mHorizOffset;
  }

  startX = X;
  startIndex = 0;

  if (!lineStr)
  {

    if (mode == DRAW_LINE)
    {
      style = position_style(lineStartPos, lineLen, -1);
      draw_string( style | BG_ONLY_MASK, text_area.x, Y, text_area.x + text_area.w,
                   (unsigned char*)lineStr, lineLen );
    }

    if (mode == FIND_INDEX)
    {
      IS_UTF8_ALIGNED2(buffer(), lineStartPos)
      return lineStartPos;
    }

    return 0;
  }

  char currChar = 0, prevChar = 0;
  style = position_style(lineStartPos, lineLen, 0);

  for (i = 0; i < lineLen; )
  {
    currChar = lineStr[i];
    int len = fl_utf8len1(currChar);

    if (len <= 0) len = 1;

    charStyle = position_style(lineStartPos, lineLen, i);

    if (charStyle != style || currChar == '\t' || prevChar == '\t')
    {
      double w = 0;

      if (prevChar == '\t')
      {
        double tab = col_to_x(mBuffer->tab_distance());
        double xAbs = (mode == GET_WIDTH) ? startX : startX + mHorizOffset -
                      text_area.x;
        w = ((int(xAbs / tab) + 1) * tab) - xAbs;

        if (mode == DRAW_LINE)
          draw_string( style | BG_ONLY_MASK, startX, Y, startX + w, 0, 0 );

        if (mode == FIND_INDEX && startX + w > rightClip)
        {
          free(lineStr);

          if (cursor_pos && (startX + w / 2 < rightClip))
            return lineStartPos + startIndex + len;

          return lineStartPos + startIndex;
        }
      }

      else
      {
        w = string_width( lineStr + startIndex, i - startIndex, style );

        if (mode == DRAW_LINE)
          draw_string( style, startX, Y, startX + w, (unsigned char*)lineStr + startIndex,
                       i - startIndex );

        if (mode == FIND_INDEX && startX + w > rightClip)
        {
          int di = find_x(lineStr + startIndex, i - startIndex, style,
                          -(rightClip - startX));
          free(lineStr);
          IS_UTF8_ALIGNED2(buffer(), (lineStartPos + startIndex + di))
          return lineStartPos + startIndex + di;
        }
      }

      style = charStyle;
      startX += w;
      startIndex = i;
    }

    i += len;
    prevChar = currChar;
  }

  int w = 0;

  if (currChar == '\t')
  {
    double tab = col_to_x(mBuffer->tab_distance());
    double xAbs = (mode == GET_WIDTH) ? startX : startX + mHorizOffset -
                  text_area.x;
    w = ((int(xAbs / tab) + 1) * tab) - xAbs;

    if (mode == DRAW_LINE)
      draw_string( style | BG_ONLY_MASK, startX, Y, startX + w, 0, 0 );

    if (mode == FIND_INDEX)
    {
      free(lineStr);

      if (cursor_pos)
        return lineStartPos + startIndex + ( rightClip - startX > w / 2 ? 1 : 0 );

      return lineStartPos + startIndex + ( rightClip - startX > w ? 1 : 0 );
    }
  }

  else
  {
    w = string_width( lineStr + startIndex, i - startIndex, style );

    if (mode == DRAW_LINE)
      draw_string( style, startX, Y, startX + w, (unsigned char*)lineStr + startIndex,
                   i - startIndex );

    if (mode == FIND_INDEX)
    {
      int di = find_x(lineStr + startIndex, i - startIndex, style,
                      -(rightClip - startX));
      free(lineStr);
      IS_UTF8_ALIGNED2(buffer(), (lineStartPos + startIndex + di))
      return lineStartPos + startIndex + di;
    }
  }

  if (mode == GET_WIDTH)
  {
    free(lineStr);
    return startX + w;
  }

  startX += w;
  style = position_style(lineStartPos, lineLen, i);

  if (mode == DRAW_LINE)
    draw_string( style | BG_ONLY_MASK, startX, Y, text_area.x + text_area.w,
                 (unsigned char*)lineStr, lineLen );

  free(lineStr);
  IS_UTF8_ALIGNED2(buffer(), (lineStartPos + lineLen))
  return lineStartPos + lineLen;
}


int
Fl_Text_Display::find_x(const unsigned char* s, int len, int style, int x) const
{
  IS_UTF8_ALIGNED(s)

  int cursor_pos = x < 0;
  x = x < 0 ? -x : x;

  int i = 0;
  int last_w = 0;

  while (i < len)
  {
    int cl = fl_utf8len1(s[i]);
    int w = int( string_width(s, i + cl, style) );

    if (w > x)
    {
      if (cursor_pos && (w - x < x - last_w)) return i + cl;

      return i;
    }

    last_w = w;
    i += cl;
  }

  return len;
}



void
Fl_Text_Display::draw_vline(int visLineNum, int leftClip, int rightClip,
                            int leftCharIndex, int rightCharIndex)
{
  int Y, lineStartPos, lineLen;

  if ( visLineNum < 0 || visLineNum >= mNVisibleLines )
    return;

  Y = text_area.y + visLineNum;

  lineStartPos = mLineStarts[ visLineNum ];

  if ( lineStartPos == -1 )
  {
    lineLen = 0;
  }

  else
  {
    lineLen = vline_length( visLineNum );
  }

  leftClip = max( text_area.x, leftClip );
  rightClip = min( rightClip, text_area.x + text_area.w );

  handle_vline(DRAW_LINE,
               lineStartPos, lineLen, leftCharIndex, rightCharIndex,
               Y, (Y + 1), leftClip, rightClip);
  return;
}



void
Fl_Text_Display::draw_string(int style,
                             int X, int Y, int toX,
                             const unsigned char* string, int nChars) const
{

  IS_UTF8_ALIGNED(string)

  if ( style & FILL_MASK )
  {
    if (style & TEXT_ONLY_MASK) return;

    clear_rect( style, X, Y, toX - X, 1 );
    return;
  }

  int fsize = 1;
  enum Fl::foreground fcolor = Fl_Widget::skin_.normal_fcolor;
  enum Fl::background bcolor = Fl_Widget::skin_.normal_bcolor;

  if (style & HIGHLIGHT_MASK)
  {
    fcolor = Fl_Widget::skin_.highlight_fcolor;
  }

  if ( !active_r() )
  {
    fcolor = Fl_Widget::skin_.disabled_fcolor;
  }

  if (!(style & TEXT_ONLY_MASK))
  {
    Fl::draw_fill(X, Y, w_, 1, 0x20, fcolor, bcolor);
  }

  if (!(style & BG_ONLY_MASK))
  {
    fl_draw(string, nChars, X, Y, fcolor, bcolor);
  }

  return;
}

void
Fl_Text_Display::clear_rect(int style,
                            int X, int Y,
                            int width, int height) const
{
  if ( width == 0 )
    return;

  enum Fl::foreground fcolor = Fl_Widget::skin_.normal_fcolor;
  enum Fl::background bcolor = Fl_Widget::skin_.normal_bcolor;

  if (style & HIGHLIGHT_MASK)
  {
    fcolor = Fl_Widget::skin_.highlight_fcolor;
  }

  Fl::draw_fill(X, Y, width, height, 0x20, fcolor, bcolor);

  return;
}

void
Fl_Text_Display::draw_cursor( int X, int Y )
{

  if (false == Fl::_caret_is_visible)
  {
    if (Fl::CARET_OVERWRITE == Fl::_caret_mode)
    {
      Fl::caret_block();
    }

    else
    {
      Fl::caret_underline();
    }
  }

  Fl::caret_set_position(X, Y);

  return;
}

int
Fl_Text_Display::position_style( int lineStartPos, int lineLen,
                                 int lineIndex) const
{
  IS_UTF8_ALIGNED2(buffer(), lineStartPos)

  Fl_Text_Buffer* buf = mBuffer;
  Fl_Text_Buffer* styleBuf = mStyleBuffer;
  int pos, style = 0;

  if ( lineStartPos == -1 || buf == NULL )
    return FILL_MASK;

  pos = lineStartPos + min( lineIndex, lineLen );

  if ( lineIndex >= lineLen )
    style = FILL_MASK;

  else if ( styleBuf != NULL )
  {
    style = ( unsigned char ) styleBuf->byte_at( pos );

    if (style == mUnfinishedStyle && mUnfinishedHighlightCB)
    {
      (mUnfinishedHighlightCB)( pos, mHighlightCBArg);
      style = (unsigned char) styleBuf->byte_at( pos);
    }
  }

  if (buf->primary_selection()->includes(pos))
    style |= PRIMARY_MASK;

  if (buf->highlight_selection()->includes(pos))
    style |= HIGHLIGHT_MASK;

  if (buf->secondary_selection()->includes(pos))
    style |= SECONDARY_MASK;

  return style;
}


double
Fl_Text_Display::string_width( const unsigned char* string, int length,
                               int style ) const
{
  IS_UTF8_ALIGNED(string)
  return length;
}



int
Fl_Text_Display::xy_to_position( int X, int Y, int posType ) const
{
  int lineStart, lineLen;
  int visLineNum;

  visLineNum = ( Y - text_area.y );

  if ( visLineNum < 0 )
    return mFirstChar;

  if ( visLineNum >= mNVisibleLines )
    visLineNum = mNVisibleLines - 1;

  lineStart = mLineStarts[ visLineNum ];

  if ( lineStart == -1 )
    return mBuffer->length();

  lineLen = vline_length( visLineNum );

  int mode = (posType == CURSOR_POS) ? FIND_CURSOR_INDEX : FIND_INDEX;
  return handle_vline(mode,
                      lineStart, lineLen, 0, 0,
                      0, 0,
                      text_area.x, X);
}



void
Fl_Text_Display::xy_to_rowcol( int X, int Y, int* row,
                               int* column, int posType ) const
{
  int fontWidth = TMPFONTWIDTH;

  *row = ( Y - text_area.y );

  if ( *row < 0 ) *row = 0;

  if ( *row >= mNVisibleLines ) *row = mNVisibleLines - 1;

  *column = ( ( X - text_area.x ) + mHorizOffset +
              ( posType == CURSOR_POS ? fontWidth / 2 : 0 ) ) / fontWidth;

  if ( *column < 0 ) * column = 0;
}



void
Fl_Text_Display::offset_line_starts( int newTopLineNum )
{
  int oldTopLineNum = mTopLineNum;
  int oldFirstChar = mFirstChar;
  int lineDelta = newTopLineNum - oldTopLineNum;
  int nVisLines = mNVisibleLines;
  int* lineStarts = mLineStarts;
  int i, lastLineNum;
  Fl_Text_Buffer* buf = mBuffer;

  if ( lineDelta == 0 )
    return;

  lastLineNum = oldTopLineNum + nVisLines - 1;

  if ( newTopLineNum < oldTopLineNum && newTopLineNum < -lineDelta )
  {
    mFirstChar = skip_lines( 0, newTopLineNum - 1, true );
  }

  else if ( newTopLineNum < oldTopLineNum )
  {
    mFirstChar = rewind_lines( mFirstChar, -lineDelta );
  }

  else if ( newTopLineNum < lastLineNum )
  {
    mFirstChar = lineStarts[ newTopLineNum - oldTopLineNum ];
  }

  else if ( newTopLineNum - lastLineNum < mNBufferLines - newTopLineNum )
  {
    mFirstChar = skip_lines( lineStarts[ nVisLines - 1 ],
                             newTopLineNum - lastLineNum, true );
  }

  else
  {
    mFirstChar = rewind_lines( buf->length(), mNBufferLines - newTopLineNum + 1 );
  }

  if ( lineDelta < 0 && -lineDelta < nVisLines )
  {
    for ( i = nVisLines - 1; i >= -lineDelta; i-- )
      lineStarts[ i ] = lineStarts[ i + lineDelta ];

    calc_line_starts( 0, -lineDelta );
  }

  else if ( lineDelta > 0 && lineDelta < nVisLines )
  {
    for ( i = 0; i < nVisLines - lineDelta; i++ )
      lineStarts[ i ] = lineStarts[ i + lineDelta ];

    calc_line_starts( nVisLines - lineDelta, nVisLines - 1 );
  }

  else
    calc_line_starts( 0, nVisLines );

  calc_last_char();
  mTopLineNum = newTopLineNum;

  absolute_top_line_number(oldFirstChar);
}



void
Fl_Text_Display::update_line_starts(int pos, int charsInserted,
                                    int charsDeleted, int linesInserted,
                                    int linesDeleted, int* scrolled )
{
  IS_UTF8_ALIGNED2(buffer(), pos)

  int* lineStarts = mLineStarts;
  int i, lineOfPos, lineOfEnd, nVisLines = mNVisibleLines;
  int charDelta = charsInserted - charsDeleted;
  int lineDelta = linesInserted - linesDeleted;

  if ( pos + charsDeleted < mFirstChar )
  {
    mTopLineNum += lineDelta;

    for ( i = 0; i < nVisLines && lineStarts[i] != -1; i++ )
      lineStarts[ i ] += charDelta;

    mFirstChar += charDelta;
    mLastChar += charDelta;
    *scrolled = 0;
    return;
  }

  if ( pos < mFirstChar )
  {
    if ( position_to_line( pos + charsDeleted, &lineOfEnd ) &&
         ++lineOfEnd < nVisLines && lineStarts[ lineOfEnd ] != -1 )
    {
      mTopLineNum = max( 1, mTopLineNum + lineDelta );
      mFirstChar = rewind_lines(lineStarts[ lineOfEnd ] + charDelta, lineOfEnd );
    }

    else
    {
      if ( mTopLineNum > mNBufferLines + lineDelta )
      {
        mTopLineNum = 1;
        mFirstChar = 0;
      }

      else
        mFirstChar = skip_lines( 0, mTopLineNum - 1, true );
    }

    calc_line_starts( 0, nVisLines - 1 );
    calc_last_char();
    *scrolled = 1;
    return;
  }

  if ( pos <= mLastChar )
  {
    position_to_line( pos, &lineOfPos );

    if ( lineDelta == 0 )
    {
      for ( i = lineOfPos + 1; i < nVisLines && lineStarts[ i ] != -1; i++ )
        lineStarts[ i ] += charDelta;
    }

    else if ( lineDelta > 0 )
    {
      for ( i = nVisLines - 1; i >= lineOfPos + lineDelta + 1; i-- )
        lineStarts[ i ] = lineStarts[ i - lineDelta ] +
                          ( lineStarts[ i - lineDelta ] == -1 ? 0 : charDelta );
    }

    else /* (lineDelta < 0) */
    {
      for ( i = max( 0, lineOfPos + 1 ); i < nVisLines + lineDelta; i++ )
        lineStarts[ i ] = lineStarts[ i - lineDelta ] +
                          ( lineStarts[ i - lineDelta ] == -1 ? 0 : charDelta );
    }

    if ( linesInserted >= 0 )
      calc_line_starts( lineOfPos + 1, lineOfPos + linesInserted );

    if ( lineDelta < 0 )
      calc_line_starts( nVisLines + lineDelta, nVisLines );

    calc_last_char();
    *scrolled = 0;
    return;
  }

  if ( empty_vlines() )
  {
    position_to_line( pos, &lineOfPos );
    calc_line_starts( lineOfPos, lineOfPos + linesInserted );
    calc_last_char();
    *scrolled = 0;
    return;
  }

  *scrolled = 0;
}


void
Fl_Text_Display::calc_line_starts( int startLine, int endLine )
{
  int startPos, bufLen = mBuffer->length();
  int line, lineEnd, nextLineStart, nVis = mNVisibleLines;
  int* lineStarts = mLineStarts;

  if ( endLine < 0 ) endLine = 0;

  if ( endLine >= nVis ) endLine = nVis - 1;

  if ( startLine < 0 ) startLine = 0;

  if ( startLine >= nVis ) startLine = nVis - 1;

  if ( startLine > endLine )
    return;

  if ( startLine == 0 )
  {
    lineStarts[ 0 ] = mFirstChar;
    startLine = 1;
  }

  startPos = lineStarts[ startLine - 1 ];

  if ( startPos == -1 )
  {
    for ( line = startLine; line <= endLine; line++ )
      lineStarts[ line ] = -1;

    return;
  }

  for ( line = startLine; line <= endLine; line++ )
  {
    find_line_end(startPos, true, &lineEnd, &nextLineStart);
    startPos = nextLineStart;

    if ( startPos >= bufLen )
    {
      if ( line == 0 || ( lineStarts[ line - 1 ] != bufLen &&
                          lineEnd != nextLineStart ) )
      {
        lineStarts[ line ] = bufLen;
        line++;
      }

      break;
    }

    lineStarts[ line ] = startPos;
  }

  for ( ; line <= endLine; line++ )
    lineStarts[ line ] = -1;
}


void
Fl_Text_Display::calc_last_char()
{
  int i;

  for (i = mNVisibleLines - 1; i >= 0 && mLineStarts[i] == -1; i--) ;

  mLastChar = i < 0 ? 0 : line_end(mLineStarts[i], true);
}


void
Fl_Text_Display::scrollto(int topLineNum, int horizOffset)
{
  mTopLineNumHint = topLineNum;
  mHorizOffsetHint = horizOffset;
  recalc_display();
}


int
Fl_Text_Display::scroll_(int topLineNum, int horizOffset)
{
  if (topLineNum > mNBufferLines + 3 - mNVisibleLines)
    topLineNum = mNBufferLines + 3 - mNVisibleLines;

  if (topLineNum < 1) topLineNum = 1;

  if (horizOffset > longest_vline() - text_area.w)
    horizOffset = longest_vline() - text_area.w;

  if (horizOffset < 0) horizOffset = 0;

  if (mHorizOffset == horizOffset && mTopLineNum == topLineNum)
    return 0;

  offset_line_starts(topLineNum);

  mHorizOffset = horizOffset;

  damage(Fl_Widget::FL_DAMAGE_EXPOSE);
  return 1;
}


void
Fl_Text_Display::update_v_scrollbar()
{
  mVScrollBar->value(mTopLineNum, mNVisibleLines, 1,
                     mNBufferLines + 1 + (mContinuousWrap ? 0 : 1));
  mVScrollBar->linesize(3);
}


void
Fl_Text_Display::update_h_scrollbar()
{
  int sliderMax = max(longest_vline(), text_area.w + mHorizOffset);
  mHScrollBar->value( mHorizOffset, text_area.w, 0, sliderMax );
}


void
Fl_Text_Display::v_scrollbar_cb(Fl_Scrollbar* b, Fl_Text_Display* textD)
{
  if (b->value() == textD->mTopLineNum) return;

  textD->scrollto(b->value(), textD->mHorizOffset);
}


void
Fl_Text_Display::h_scrollbar_cb(Fl_Scrollbar* b, Fl_Text_Display* textD)
{
  if (b->value() == textD->mHorizOffset) return;

  textD->scrollto(textD->mTopLineNum, b->value());
}


void
Fl_Text_Display::draw_line_numbers(bool /*clearAll*/)
{
  int Y, line, visLine, lineStart;
  char lineNumString[16];
  int lineHeight = 1;
  int isactive = active_r() ? 1 : 0;

  if (mLineNumWidth <= 0 || !visible_r())
    return;

  int hscroll_h = mHScrollBar->visible() ? mHScrollBar->h() : 0;
  int xoff = 0;
  int yoff = text_area.y - y();

#ifndef LINENUM_LEFT_OF_VSCROLL
  int vscroll_w = mVScrollBar->visible() ? mVScrollBar->w() : 0;

  if (scrollbar_align()&FL_ALIGN_LEFT)
    xoff += vscroll_w;

#endif

//_mjo
#if 0

  Fl_Color fgcolor = isactive ? linenumber_fgcolor() : fl_inactive(
                       linenumber_fgcolor());
  Fl_Color bgcolor = isactive ? linenumber_bgcolor() : fl_inactive(
                       linenumber_bgcolor());
  Fl::clip_push(x() + xoff,
                y() + yoff,
                mLineNumWidth,
                h() - Fl::box_dw(box()) - hscroll_h);
  {
    // Set background color for line number area -- LZA / STR# 2621
    // Erase background
    fl_color(bgcolor);
    fl_rectf(x() + xoff, y(), mLineNumWidth, h());

    // Draw separator line
    //fl_color(180,180,180);
    //fl_line(x()+mLineNumWidth-1, y(), x()+mLineNumWidth-1, y()+h());

    // Draw line number text
    Y = y() + yoff;
    line = get_absolute_top_line_number();

    // set font color for line numbers
    fl_color(fgcolor);

    for (visLine = 0; visLine < mNVisibleLines; visLine++)
    {
      lineStart = mLineStarts[visLine];

      if (lineStart != -1 && (lineStart == 0
                              || buffer()->char_at(lineStart - 1) == '\n'))
      {
        sprintf(lineNumString, linenumber_format(), line);
        int xx = x() + xoff + 3,
            yy = Y,
            ww = mLineNumWidth - (3 * 2),
            hh = lineHeight;
        fl_draw(lineNumString, xx, yy, ww, hh, linenumber_align(), 0, 0);
        //DEBUG fl_rect(xx, yy, ww, hh);
        line++;
      }

      else
      {
        if (visLine == 0) line++;
      }

      Y += lineHeight;
    }
  }
  Fl::clip_pop();

#endif
}

static int
max( int i1, int i2 )
{
  return i1 >= i2 ? i1 : i2;
}

static int
min( int i1, int i2 )
{
  return i1 <= i2 ? i1 : i2;
}


static int
countlines( const unsigned char* string )
{
  IS_UTF8_ALIGNED(string)

  const unsigned char* c;
  int lineCount = 0;

  if (!string) return 0;

  for ( c = string; *c != '\0'; c++ )
    if ( *c == '\n' ) lineCount++;

  return lineCount;
}


int
Fl_Text_Display::measure_vline( int visLineNum ) const
{
  int lineLen = vline_length( visLineNum );
  int lineStartPos = mLineStarts[ visLineNum ];

  if (lineStartPos < 0 || lineLen == 0) return 0;

  return handle_vline(GET_WIDTH, lineStartPos, lineLen, 0, 0, 0, 0, 0, 0);
}


int
Fl_Text_Display::empty_vlines() const
{
  return (mNVisibleLines > 0) && (mLineStarts[ mNVisibleLines - 1 ] == -1);
}


int
Fl_Text_Display::vline_length( int visLineNum ) const
{
  int nextLineStart, lineStartPos;

  if (visLineNum < 0 || visLineNum >= mNVisibleLines)
    return (0);

  lineStartPos = mLineStarts[ visLineNum ];

  if ( lineStartPos == -1 )
    return 0;

  if ( visLineNum + 1 >= mNVisibleLines )
    return mLastChar - lineStartPos;

  nextLineStart = mLineStarts[ visLineNum + 1 ];

  if ( nextLineStart == -1 )
    return mLastChar - lineStartPos;

  int nextLineStartMinus1 = buffer()->prev_char(nextLineStart);

  if (wrap_uses_character(nextLineStartMinus1))
    return nextLineStartMinus1 - lineStartPos;

  return nextLineStart - lineStartPos;
}


void
Fl_Text_Display::find_wrap_range(const unsigned char* deletedText, int pos,
                                 int nInserted, int nDeleted,
                                 int* modRangeStart, int* modRangeEnd,
                                 int* linesInserted, int* linesDeleted)
{
  IS_UTF8_ALIGNED(deletedText)
  IS_UTF8_ALIGNED2(buffer(), pos)

  int length, retPos, retLines, retLineStart, retLineEnd;
  Fl_Text_Buffer* deletedTextBuf, *buf = buffer();
  int nVisLines = mNVisibleLines;
  int* lineStarts = mLineStarts;
  int countFrom, countTo, lineStart, adjLineStart, i;
  int visLineNum = 0, nLines = 0;

  if (pos >= mFirstChar && pos <= mLastChar)
  {
    for (i = nVisLines - 1; i > 0; i--)
    {
      if (lineStarts[i] != -1 && pos >= lineStarts[i])
      {
        break;
      }
    }

    if (i > 0)
    {
      countFrom = lineStarts[i - 1];
      visLineNum = i - 1;
    }

    else
    {
      countFrom = buf->line_start(pos);
    }
  }

  else
  {
    countFrom = buf->line_start(pos);
  }

  IS_UTF8_ALIGNED2(buf, countFrom)

  lineStart = countFrom;
  *modRangeStart = countFrom;

  for (;;)
  {

    wrapped_line_counter(buf, lineStart, buf->length(), 1, true, 0,
                         &retPos, &retLines, &retLineStart, &retLineEnd);

    if (retPos >= buf->length())
    {
      countTo = buf->length();
      *modRangeEnd = countTo;

      if (retPos != retLineEnd)
        nLines++;

      break;
    }

    else
    {
      lineStart = retPos;
    }

    nLines++;

    if (lineStart > pos + nInserted
        && buf->char_at(buf->prev_char(lineStart)) == '\n')
    {
      countTo = lineStart;
      *modRangeEnd = lineStart;
      break;
    }

    if (mSuppressResync)
      continue;

    if (lineStart <= pos)
    {
      while (visLineNum < nVisLines && lineStarts[visLineNum] < lineStart)
        visLineNum++;

      if (visLineNum < nVisLines && lineStarts[visLineNum] == lineStart)
      {
        countFrom = lineStart;
        nLines = 0;

        if (visLineNum + 1 < nVisLines && lineStarts[visLineNum + 1] != -1)
          *modRangeStart = min(pos, buf->prev_char(lineStarts[visLineNum + 1]));

        else
          *modRangeStart = countFrom;
      }

      else
        *modRangeStart = min(*modRangeStart, buf->prev_char(lineStart));
    }

    else if (lineStart > pos + nInserted)
    {
      adjLineStart = lineStart - nInserted + nDeleted;

      while (visLineNum < nVisLines && lineStarts[visLineNum] < adjLineStart)
        visLineNum++;

      if (visLineNum < nVisLines && lineStarts[visLineNum] != -1 &&
          lineStarts[visLineNum] == adjLineStart)
      {
        countTo = line_end(lineStart, true);
        *modRangeEnd = lineStart;
        break;
      }
    }
  }

  *linesInserted = nLines;


  if (mSuppressResync)
  {
    *linesDeleted = mNLinesDeleted;
    mSuppressResync = 0;
    return;
  }

  length = (pos - countFrom) + nDeleted + (countTo - (pos + nInserted));
  deletedTextBuf = new Fl_Text_Buffer(length);
  deletedTextBuf->copy(buffer(), countFrom, pos, 0);

  if (nDeleted != 0)
    deletedTextBuf->insert(pos - countFrom, deletedText);

  deletedTextBuf->copy(buffer(), pos + nInserted, countTo,
                       pos - countFrom + nDeleted);
  wrapped_line_counter(deletedTextBuf, 0, length, INT_MAX, true, countFrom,
                       &retPos, &retLines, &retLineStart, &retLineEnd, false);
  delete deletedTextBuf;
  *linesDeleted = retLines;
  mSuppressResync = 0;
}


void
Fl_Text_Display::measure_deleted_lines(int pos, int nDeleted)
{
  IS_UTF8_ALIGNED2(buffer(), pos)

  int retPos, retLines, retLineStart, retLineEnd;
  Fl_Text_Buffer* buf = buffer();
  int nVisLines = mNVisibleLines;
  int* lineStarts = mLineStarts;
  int countFrom, lineStart;
  int nLines = 0, i;

  if (pos >= mFirstChar && pos <= mLastChar)
  {
    for (i = nVisLines - 1; i > 0; i--)
      if (lineStarts[i] != -1 && pos >= lineStarts[i])
        break;

    if (i > 0)
    {
      countFrom = lineStarts[i - 1];
    }

    else
      countFrom = buf->line_start(pos);
  }

  else
    countFrom = buf->line_start(pos);

  lineStart = countFrom;

  for (;;)
  {
    wrapped_line_counter(buf, lineStart, buf->length(), 1, true, 0,
                         &retPos, &retLines, &retLineStart, &retLineEnd);

    if (retPos >= buf->length())
    {
      if (retPos != retLineEnd)
        nLines++;

      break;
    }

    else
      lineStart = retPos;

    nLines++;

    if (lineStart > pos + nDeleted && buf->char_at(lineStart - 1) == '\n')
    {
      break;
    }

  }

  mNLinesDeleted = nLines;
  mSuppressResync = 1;
}


void
Fl_Text_Display::wrapped_line_counter(Fl_Text_Buffer* buf, int startPos,
                                      int maxPos, int maxLines, bool startPosIsLineStart, int styleBufOffset,
                                      int* retPos, int* retLines, int* retLineStart, int* retLineEnd,
                                      bool countLastLineMissingNewLine) const
{
  IS_UTF8_ALIGNED2(buf, startPos)
  IS_UTF8_ALIGNED2(buf, maxPos)

  int lineStart, newLineStart = 0, b, p, colNum, wrapMarginPix;
  int i, foundBreak;
  double width;
  int nLines = 0;
  unsigned int c;

  if (mWrapMarginPix != 0)
  {
    wrapMarginPix = mWrapMarginPix;
  }

  else
  {
    wrapMarginPix = text_area.w;
  }

  if (startPosIsLineStart)
    lineStart = startPos;

  else
    lineStart = line_start(startPos);

  colNum = 0;
  width = 0;

  for (p = lineStart; p < buf->length(); p = buf->next_char(p))
  {
    c = buf->char_at(p);  // UCS-4

    if (c == '\n')
    {
      if (p >= maxPos)
      {
        *retPos = maxPos;
        *retLines = nLines;
        *retLineStart = lineStart;
        *retLineEnd = maxPos;
        return;
      }

      nLines++;
      int p1 = buf->next_char(p);

      if (nLines >= maxLines)
      {
        *retPos = p1;
        *retLines = nLines;
        *retLineStart = p1;
        *retLineEnd = p;
        return;
      }

      lineStart = p1;
      colNum = 0;
      width = 0;
    }

    else
    {
      const unsigned char* s = buf->address(p);
      colNum++;
      width += measure_proportional_character(s, (int)width, p + styleBufOffset);
    }

    if (width > wrapMarginPix)
    {
      foundBreak = false;

      for (b = p; b >= lineStart; b = buf->prev_char(b))
      {
        c = buf->char_at(b);

        if (c == '\t' || c == ' ')
        {
          newLineStart = buf->next_char(b);
          colNum = 0;
          width = 0;
          int iMax = buf->next_char(p);

          for (i = buf->next_char(b); i < iMax; i = buf->next_char(i))
          {
            width += measure_proportional_character(buf->address(i), (int)width,
                                                    i + styleBufOffset);
            colNum++;
          }

          foundBreak = true;
          break;
        }
      }

      if (b < lineStart) b = lineStart;

      if (!foundBreak)
      {
        newLineStart = max(p, buf->next_char(lineStart));
        colNum++;

        if (b >= buf->length())
        {
          width = 0;
        }

        else
        {
          const unsigned char* s = buf->address(b);
          width = measure_proportional_character(s, 0, p + styleBufOffset);
        }
      }

      if (p >= maxPos)
      {
        *retPos = maxPos;
        *retLines = maxPos < newLineStart ? nLines : nLines + 1;
        *retLineStart = maxPos < newLineStart ? lineStart : newLineStart;
        *retLineEnd = maxPos;
        return;
      }

      nLines++;

      if (nLines >= maxLines)
      {
        *retPos = foundBreak ? buf->next_char(b) : max(p, buf->next_char(lineStart));
        *retLines = nLines;
        *retLineStart = lineStart;
        *retLineEnd = foundBreak ? b : p;
        return;
      }

      lineStart = newLineStart;
    }
  }

  *retPos = buf->length();
  *retLines = nLines;

  if (countLastLineMissingNewLine && colNum > 0)
    *retLines = buf->next_char(*retLines);

  *retLineStart = lineStart;
  *retLineEnd = buf->length();
}


double
Fl_Text_Display::measure_proportional_character(const unsigned char* s,
                                                int xPix,
                                                int pos) const
{
  IS_UTF8_ALIGNED(s)

  if (*s == '\t')
  {
    int tab = (int)col_to_x(mBuffer->tab_distance());
    return (((xPix / tab) + 1) * tab) - xPix;
  }

  int charLen = fl_utf8len1(*s), style = 0;

  if (mStyleBuffer)
  {
    style = mStyleBuffer->byte_at(pos);
  }

  return string_width(s, charLen, style);
}


void
Fl_Text_Display::find_line_end(int startPos, bool startPosIsLineStart,
                               int* lineEnd, int* nextLineStart) const
{
  IS_UTF8_ALIGNED2(buffer(), startPos)

  int retLines, retLineStart;

  if (!mContinuousWrap)
  {
    int le = buffer()->line_end(startPos);
    int ls = buffer()->next_char(le);
    *lineEnd = le;
    *nextLineStart = min(buffer()->length(), ls);
    return;
  }

  wrapped_line_counter(buffer(), startPos, buffer()->length(),
                       1, startPosIsLineStart, 0, nextLineStart, &retLines,
                       &retLineStart, lineEnd);
}


int
Fl_Text_Display::wrap_uses_character(int lineEndPos) const
{
  IS_UTF8_ALIGNED2(buffer(), lineEndPos)

  unsigned int c;

  if (!mContinuousWrap || lineEndPos == buffer()->length())
    return 1;

  c = buffer()->char_at(lineEndPos);
  return c == '\n' || ((c == '\t' || c == ' ') &&
                       lineEndPos + 1 < buffer()->length());
}


void
Fl_Text_Display::extend_range_for_styles( int* startpos, int* endpos )
{
  IS_UTF8_ALIGNED2(buffer(), (*startpos))
  IS_UTF8_ALIGNED2(buffer(), (*endpos))

  Fl_Text_Selection* sel = mStyleBuffer->primary_selection();
  int extended = 0;

  if ( sel->selected() )
  {
    if ( sel->start() < *startpos )
    {
      *startpos = sel->start();
      *startpos = buffer()->utf8_align(*startpos);
      IS_UTF8_ALIGNED2(buffer(), (*startpos))
      extended = 1;
    }

    if ( sel->end() > *endpos )
    {
      *endpos = sel->end();
      *endpos = buffer()->utf8_align(*endpos);
      IS_UTF8_ALIGNED2(buffer(), (*endpos))
      extended = 1;
    }
  }

  if ( extended )
    *endpos = mBuffer->line_end( *endpos ) + 1;

  IS_UTF8_ALIGNED2(buffer(), (*endpos))
}


void
Fl_Text_Display::draw(void)
{


  if (!buffer())
  {
    return;
  }

  enum Fl::foreground fcolor = Fl_Widget::skin_.normal_fcolor;
  enum Fl::background bcolor = Fl_Widget::skin_.normal_bcolor;

  Fl::clip_push(x(), y(), w(), h());

  if ( !active_r() )
  {
    fcolor = Fl_Widget::skin_.disabled_fcolor;
  }

  int scrollsize = scrollbar_width_ ? scrollbar_width_ : Fl::scrollbar_size();

  if (damage() & Fl_Widget::FL_DAMAGE_ALL)
  {
    recalc_display();
    int W = w(), H = h();

    if (mHScrollBar->visible())
      W -= scrollsize;

    if (mVScrollBar->visible())
      H -= scrollsize;

    Fl::draw_fill(text_area.x - mMargin.left, text_area.y - mMargin.top,
                  mMargin.left, text_area.h + mMargin.top + mMargin.bottom,
                  skin_.margin_fillc, skin_.margin_fcolor, skin_.margin_bcolor);

    Fl::draw_fill(text_area.x + text_area.w, text_area.y - mMargin.top,
                  mMargin.right, text_area.h + mMargin.top + mMargin.bottom,
                  skin_.margin_fillc, skin_.margin_fcolor, skin_.margin_bcolor);

    Fl::draw_fill(text_area.x, text_area.y - mMargin.top,
                  text_area.w, mMargin.top,
                  skin_.margin_fillc, skin_.margin_fcolor, skin_.margin_bcolor);

    Fl::draw_fill(text_area.x, text_area.y + text_area.h,
                  text_area.w, mMargin.bottom,
                  skin_.margin_fillc, skin_.margin_fcolor, skin_.margin_bcolor);

#if 0

    if (mVScrollBar->visible() && mHScrollBar->visible())
      Fl::draw_fill(mVScrollBar->x(), mHScrollBar->y(),
                    mVScrollBar->w(), mHScrollBar->h(),
                    FL_GRAY);

#endif

  }

  else if (damage() & (Fl_Widget::FL_DAMAGE_SCROLL |
                       Fl_Widget::FL_DAMAGE_EXPOSE))
  {
    Fl::clip_push(text_area.x - mMargin.left,
                  text_area.y,
                  text_area.w + mMargin.left + mMargin.right,
                  text_area.h);

    Fl::draw_fill(text_area.x - mMargin.left, mCursorOldY,
                  mMargin.left, 1,
                  skin_.margin_fillc, skin_.margin_fcolor, skin_.margin_bcolor);

    Fl::draw_fill(text_area.x + text_area.w, mCursorOldY,
                  mMargin.right, 1,
                  skin_.margin_fillc, skin_.margin_fcolor, skin_.margin_bcolor);

    Fl::clip_pop();
  }

  if (damage() & (Fl_Widget::FL_DAMAGE_ALL | Fl_Widget::FL_DAMAGE_CHILD))
  {
    mVScrollBar->damage(Fl_Widget::FL_DAMAGE_ALL);
    mHScrollBar->damage(Fl_Widget::FL_DAMAGE_ALL);
  }

  update_child(*mVScrollBar);
  update_child(*mHScrollBar);

  if (damage() & (Fl_Widget::FL_DAMAGE_ALL | Fl_Widget::FL_DAMAGE_EXPOSE))
  {
    int X, Y;
    unsigned int W, H;

    if (Fl::clip_box(X, Y, W, H, text_area.x, text_area.y, text_area.w,
                     text_area.h))
    {
      draw_text(X, Y, W, H);
    }

    else
    {
      draw_text(text_area.x, text_area.y, text_area.w, text_area.h);
    }
  }

  else if (damage() & Fl_Widget::FL_DAMAGE_SCROLL)
  {
    Fl::clip_push(text_area.x, text_area.y, text_area.w, text_area.h);

    draw_range(damage_range1_start, damage_range1_end);

    if (damage_range2_end != -1)
    {
      draw_range(damage_range2_start, damage_range2_end);
    }

    damage_range1_start = damage_range1_end = -1;
    damage_range2_start = damage_range2_end = -1;
    Fl::clip_pop();
  }

  int start, end;
  int has_selection = buffer()->selection_position(&start, &end);

  if (damage() & (Fl_Widget::FL_DAMAGE_ALL | Fl_Widget::FL_DAMAGE_SCROLL |
                  Fl_Widget::FL_DAMAGE_EXPOSE)
      && (
        (Fl::screen_driver()->has_marked_text() && Fl::compose_state) ||
        (!has_selection) || mCursorPos < start || mCursorPos > end) &&
      mCursorOn && Fl::focus() == (Fl_Widget*)this )
  {
    int X = 0, Y = 0;

    if (position_to_xy(mCursorPos, &X, &Y))
    {
      draw_cursor(X, Y);
      mCursorOldY = Y;
    }
  }

  draw_line_numbers(true);

  Fl::clip_pop();

}

void
fl_text_drag_me(int pos, Fl_Text_Display* d)
{
  if (d->dragType == Fl_Text_Display::DRAG_CHAR)
  {
    if (pos >= d->dragPos)
    {
      d->buffer()->select(d->dragPos, pos);
    }

    else
    {
      d->buffer()->select(pos, d->dragPos);
    }

    d->insert_position(pos);
  }

  else if (d->dragType == Fl_Text_Display::DRAG_WORD)
  {
    if (pos >= d->dragPos)
    {
      d->insert_position(d->word_end(pos));
      d->buffer()->select(d->word_start(d->dragPos), d->word_end(pos));
    }

    else
    {
      d->insert_position(d->word_start(pos));
      d->buffer()->select(d->word_start(pos), d->word_end(d->dragPos));
    }
  }

  else if (d->dragType == Fl_Text_Display::DRAG_LINE)
  {
    if (pos >= d->dragPos)
    {
      d->insert_position(d->buffer()->line_end(pos) + 1);
      d->buffer()->select(d->buffer()->line_start(d->dragPos),
                          d->buffer()->line_end(pos) + 1);
    }

    else
    {
      d->insert_position(d->buffer()->line_start(pos));
      d->buffer()->select(d->buffer()->line_start(pos),
                          d->buffer()->line_end(d->dragPos) + 1);
    }
  }
}


void
Fl_Text_Display::scroll_timer_cb(void* user_data)
{
  Fl_Text_Display* w = (Fl_Text_Display*)user_data;
  int pos;

  switch (scroll_direction)
  {
    case 1: // mouse is to the right, scroll left
      w->scrollto(w->mTopLineNum, w->mHorizOffset + scroll_amount);
      pos = w->xy_to_position(w->text_area.x + w->text_area.w, scroll_y, CURSOR_POS);
      break;

    case 2: // mouse is to the left, scroll right
      w->scrollto(w->mTopLineNum, w->mHorizOffset + scroll_amount);
      pos = w->xy_to_position(w->text_area.x, scroll_y, CURSOR_POS);
      break;

    case 3: // mouse is above, scroll down
      w->scrollto(w->mTopLineNum + scroll_amount, w->mHorizOffset);
      pos = w->xy_to_position(scroll_x, w->text_area.y, CURSOR_POS);
      break;

    case 4: // mouse is below, scroll up
      w->scrollto(w->mTopLineNum + scroll_amount, w->mHorizOffset);
      pos = w->xy_to_position(scroll_x, w->text_area.y + w->text_area.h, CURSOR_POS);
      break;

    default:
      return;
  }

  fl_text_drag_me(pos, w);
  Fl::repeat_timeout(.1, scroll_timer_cb, user_data);
}


bool
Fl_Text_Display::handle(
  enum Fl_Event const event)
{

  if (!buffer()) return 0;

  if (!Fl::event_inside(text_area.x, text_area.y, text_area.w, text_area.h) &&
      !dragging && event != FL_LEAVE && event != FL_ENTER &&
      event != FL_MOVE && event != FL_FOCUS && event != FL_UNFOCUS &&
      event != FL_KEYBOARD && event != FL_KEYUP)
  {
    return Fl_Group::handle(event);
  }

  switch (event)
  {
    case FL_ENTER:
    case FL_MOVE:
      if (active_r())
      {
        return 1;
      }

      else
      {
        return 0;
      }

    case FL_LEAVE:
    case FL_HIDE:
      if (active_r() && window())
      {
        return 1;
      }

      else
      {
        return 0;
      }

    case FL_PUSH:
    {
      if (Fl::focus() != this)
      {
        Fl::focus(this);
        handle(FL_FOCUS);
      }

      if (Fl_Group::handle(event)) return 1;

      if (Fl::event_state()&FL_SHIFT) return handle(FL_DRAG);

      dragging = 1;
      int pos = xy_to_position(Fl::event_x(), Fl::event_y(), CURSOR_POS);
      dragPos = pos;

      if (buffer()->primary_selection()->includes(pos))
      {
        dragType = DRAG_START_DND;
        return 1;
      }

      dragType = Fl::event_clicks();

      if (dragType == DRAG_CHAR)
      {
        buffer()->unselect();
      }

      else if (dragType == DRAG_WORD)
      {
        buffer()->select(word_start(pos), word_end(pos));
        dragPos = word_start(pos);
      }

      if (buffer()->primary_selection()->selected())
        insert_position(buffer()->primary_selection()->end());

      else
        insert_position(pos);

      show_insert_position();
      return 1;
    }

    case FL_DRAG:
    {
      if (dragType == DRAG_NONE)
        return 1;

      if (dragType == DRAG_START_DND)
      {
        if (!Fl::event_is_click() && Fl::dnd_text_ops())
        {
          const unsigned char* copy = buffer()->selection_text();
          Fl::screen_driver()->dnd(1);
          free((void*)copy);
        }

        return 1;
      }

      int X = Fl::event_x(), Y = Fl::event_y(), pos = insert_position();

      if (Y < text_area.y)
      {
        scroll_x = X;
        scroll_amount = (Y - text_area.y) / 5 - 1;

        if (!scroll_direction)
        {
          Fl::add_timeout(.01, scroll_timer_cb, this);
        }

        scroll_direction = 3;
      }

      else if (Y >= text_area.y + text_area.h)
      {
        scroll_x = X;
        scroll_amount = (Y - text_area.y - text_area.h) / 5 + 1;

        if (!scroll_direction)
        {
          Fl::add_timeout(.01, scroll_timer_cb, this);
        }

        scroll_direction = 4;
      }

      else if (X < text_area.x)
      {
        scroll_y = Y;
        scroll_amount = (X - text_area.x) / 2 - 1;

        if (!scroll_direction)
        {
          Fl::add_timeout(.01, scroll_timer_cb, this);
        }

        scroll_direction = 2;
      }

      else if (X >= text_area.x + text_area.w)
      {
        scroll_y = Y;
        scroll_amount = (X - text_area.x - text_area.w) / 2 + 1;

        if (!scroll_direction)
        {
          Fl::add_timeout(.01, scroll_timer_cb, this);
        }

        scroll_direction = 1;
      }

      else
      {
        if (scroll_direction)
        {
          Fl::remove_timeout(scroll_timer_cb, this);
          scroll_direction = 0;
        }

        pos = xy_to_position(X, Y, CURSOR_POS);
        pos = buffer()->next_char(pos);
      }

      fl_text_drag_me(pos, this);
      return 1;
    }

    case FL_RELEASE:
    {
      if (Fl::event_is_click() && (! Fl::event_clicks()) &&
          buffer()->primary_selection()->includes(dragPos)
          && !(Fl::event_state()&FL_SHIFT) )
      {
        buffer()->unselect();
        insert_position(dragPos);
        return 1;
      }

      else if (Fl::event_clicks() == DRAG_LINE
               && Fl::event_button() == FL_LEFT_MOUSE)
      {
        buffer()->select(buffer()->line_start(dragPos),
                         buffer()->next_char(buffer()->line_end(dragPos)));
        dragPos = line_start(dragPos);
        dragType = DRAG_CHAR;
      }

      else
      {
        dragging = 0;

        if (scroll_direction)
        {
          Fl::remove_timeout(scroll_timer_cb, this);
          scroll_direction = 0;
        }

        dragType = DRAG_CHAR;
      }

      const unsigned char* copy = buffer()->selection_text();

      if (*copy) Fl::copy(copy, (int) strlen((char*)copy), 0);

      free((void*)copy);
      return 1;
    }

    case FL_MOUSEWHEEL:
      if (Fl::event_dy()) return mVScrollBar->handle(event);

      else return mHScrollBar->handle(event);

    case FL_UNFOCUS:
    case FL_FOCUS:
      if (buffer()->selected())
      {
        int start, end;

        if (buffer()->selection_position(&start, &end))
          redisplay_range(start, end);
      }

      if (buffer()->secondary_selected())
      {
        int start, end;

        if (buffer()->secondary_selection_position(&start, &end))
          redisplay_range(start, end);
      }

      if (buffer()->highlight())
      {
        int start, end;

        if (buffer()->highlight_position(&start, &end))
          redisplay_range(start, end);
      }

      return 1;

    case FL_KEYBOARD:

      if ((Fl::event_state() & (FL_CTRL | FL_COMMAND)) && Fl::event_key() == 'c')
      {
        if (!buffer()->selected()) return 1;

        const unsigned char* copy = buffer()->selection_text();

        if (*copy) Fl::copy(copy, (int) strlen((char*)copy), 1);

        free((void*)copy);
        return 1;
      }

      if ((Fl::event_state() & (FL_CTRL | FL_COMMAND)) && Fl::event_key() == 'a')
      {
        buffer()->select(0, buffer()->length());
        const unsigned char* copy = buffer()->selection_text();

        if (*copy) Fl::copy(copy, (int) strlen((char*)copy), 0);

        free((void*)copy);
        return 1;
      }

      if (mVScrollBar->handle(event)) return 1;

      if (mHScrollBar->handle(event)) return 1;

      break;

    case FL_SHORTCUT:
      if (!(shortcut() ? Fl::test_shortcut(shortcut()) : test_shortcut()))
        return 0;

      if (Fl::visible_focus() && handle(FL_FOCUS))
      {
        Fl::focus(this);
        return 1;
      }

      break;

  }

  return 0;
}


double
Fl_Text_Display::x_to_col(double y) const
{
  if (!mColumnScale)
  {
    mColumnScale = string_width((unsigned char*)"Mitg", 4, 'A') / 4.0;
  }

  return (y / mColumnScale) + 0.5;
}


double
Fl_Text_Display::col_to_x(double col) const
{
  if (!mColumnScale)
  {
    x_to_col(0);
  }

  return col * mColumnScale;
}
