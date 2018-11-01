// textdsp.h
//
// Header file for Fl_Text_Display class.
//
// Copyright 2018 The fltkcon authors
// Copyright 2017-2018 The fltkal authors
// Copyright 2001-2017 by Bill Spitzak and others.
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
#ifndef FL_TEXT_DISPLAY_H
#define FL_TEXT_DISPLAY_H

#include "fl.h"
#include "fl_draw.h"
#include "group.h"
#include "widget.h"
#include "scrlbar.h"
#include "textbuf.h"

class Fl_Text_Display: public Fl_Group
{

  public:

    enum
    {
      NORMAL_CURSOR,
      CARET_CURSOR,
      DIM_CURSOR,
      BLOCK_CURSOR,
      HEAVY_CURSOR,
      SIMPLE_CURSOR
    };

    enum
    {
      CURSOR_POS,
      CHARACTER_POS
    };

    enum
    {
      DRAG_NONE = -2,
      DRAG_START_DND = -1,
      DRAG_CHAR = 0,
      DRAG_WORD = 1,
      DRAG_LINE = 2
    };

    enum
    {
      WRAP_NONE,
      WRAP_AT_COLUMN,
      WRAP_AT_PIXEL,
      WRAP_AT_BOUNDS
    };

    friend void fl_text_drag_me(int pos, Fl_Text_Display* d);

    typedef void (*Unfinished_Style_Cb)(int, void*);

    struct Fl::skin_editor skin_;

    Fl_Text_Display(
      int const i_pos_x,
      int const i_pos_y,
      unsigned int const i_len_x,
      unsigned int const i_len_y,
      const unsigned char* i_label = 0L);

    ~Fl_Text_Display();

    virtual bool
    handle(
      enum Fl_Event const event);

    void buffer(Fl_Text_Buffer* buf);

    void
    buffer(Fl_Text_Buffer& buf)
    {
      buffer(&buf);
    }

    Fl_Text_Buffer*
    buffer() const
    {
      return mBuffer;
    }

    void redisplay_range(int start, int end);
    void scrollto(int topLineNum, int horizOffset);
    void insert(const unsigned char* text);
    void overstrike(const unsigned char* text);
    void insert_position(int newPos);

    int
    insert_position() const
    {
      return mCursorPos;
    }
    int position_to_xy(int pos, int* x, int* y) const;

    int in_selection(int x, int y) const;
    void show_insert_position();

    int move_right();
    int move_left();
    int move_up();
    int move_down();
    int count_lines(int start, int end, bool start_pos_is_line_start) const;
    int line_start(int pos) const;
    int line_end(int startPos, bool startPosIsLineStart) const;
    int skip_lines(int startPos, int nLines, bool startPosIsLineStart);
    int rewind_lines(int startPos, int nLines);
    void next_word(void);
    void previous_word(void);

    void show_cursor(int b = 1);

    void
    hide_cursor()
    {
      show_cursor(0);
    }

    void cursor_style(int style);
    int
    cursor_style() const
    {
      return mCursorStyle;
    }

    enum Fl::foreground
    cursor_color() const
    {
      return mCursor_color;
    }

    void
    cursor_color(enum Fl::foreground const n)
    {
      mCursor_color = n;
    }

    int
    scrollbar_width() const
    {
      return scrollbar_width_ ? scrollbar_width_ : Fl::scrollbar_size();
    }

    void
    scrollbar_width(int width)
    {
      Fl::scrollbar_size(width);
      scrollbar_width_ = 0;
    }

    int
    scrollbar_size() const
    {
      return (scrollbar_width_);
    }

    void
    scrollbar_size(int newSize)
    {
      scrollbar_width_ = newSize;
    }

    enum Fl_Label::Fl_Align
    scrollbar_align() const
    {
      return scrollbar_align_;
    }

    void
    scrollbar_align(enum Fl_Label::Fl_Align const a)
    {
      scrollbar_align_ = a;
    }

    int
    word_start(int pos) const
    {
      return buffer()->word_start(pos);
    }

    int
    word_end(int pos) const
    {
      return buffer()->word_end(pos);
    }


    int position_style(int lineStartPos, int lineLen, int lineIndex) const;

    int
    shortcut() const
    {
      return shortcut_;
    }

    void
    shortcut(int s)
    {
      shortcut_ = s;
    }

    enum Fl::foreground
    textcolor() const
    {
      return textcolor_;
    }

    void
    textcolor(enum Fl::foreground const n)
    {
      textcolor_ = n;
    }

    int wrapped_column(int row, int column) const;
    int wrapped_row(int row) const;
    void wrap_mode(int wrap, int wrap_margin);

    void
    margin(
      unsigned int const i_left,
      unsigned int const i_right,
      unsigned int const i_top,
      unsigned int const i_bottom)
    {
      mMargin.left = i_left;
      mMargin.right = i_right;
      mMargin.top = i_top;
      mMargin.bottom = i_bottom;
    }

    virtual void recalc_display();
    virtual void resize(int X, int Y, int W, int H);

    double x_to_col(double x) const;

    double col_to_x(double col) const;

    void linenumber_width(int width);

    int linenumber_width() const;

    void linenumber_align(enum Fl_Label::Fl_Align val);

    enum Fl_Label::Fl_Align linenumber_align() const;

    void linenumber_format(const unsigned char* val);

    const unsigned char* linenumber_format() const;

  protected:
    virtual void draw();
    void draw_text(int X, int Y, int W, int H);
    void draw_range(int start, int end);
    void draw_cursor(int, int);

    void draw_string(int style, int x, int y, int toX, const unsigned char* string,
                     int nChars) const;

    void draw_vline(int visLineNum, int leftClip, int rightClip,
                    int leftCharIndex, int rightCharIndex);

    int find_x(const unsigned char* s, int len, int style, int x) const;

    enum
    {
      DRAW_LINE,
      FIND_INDEX,
      FIND_INDEX_FROM_ZERO,
      GET_WIDTH,
      FIND_CURSOR_INDEX
    };

    int handle_vline(int mode,
                     int lineStart, int lineLen, int leftChar, int rightChar,
                     int topClip, int bottomClip,
                     int leftClip, int rightClip) const;

    void draw_line_numbers(bool clearAll);

    void clear_rect(int style, int x, int y, int width, int height) const;
    void display_insert();

    void offset_line_starts(int newTopLineNum);

    void calc_line_starts(int startLine, int endLine);

    void update_line_starts(int pos, int charsInserted, int charsDeleted,
                            int linesInserted, int linesDeleted, int* scrolled);

    void calc_last_char();

    int position_to_line( int pos, int* lineNum ) const;
    double string_width(const unsigned char* string, int length, int style) const;

    static void scroll_timer_cb(void*);

    static void buffer_predelete_cb(int pos, int nDeleted, void* cbArg);
    static void buffer_modified_cb(int pos, int nInserted, int nDeleted,
                                   int nRestyled, const unsigned char* deletedText,
                                   void* cbArg);

    static void h_scrollbar_cb(Fl_Scrollbar* w, Fl_Text_Display* d);
    static void v_scrollbar_cb( Fl_Scrollbar* w, Fl_Text_Display* d);
    void update_v_scrollbar();
    void update_h_scrollbar();
    int measure_vline(int visLineNum) const;
    int longest_vline() const;
    int empty_vlines() const;
    int vline_length(int visLineNum) const;
    int xy_to_position(int x, int y, int PosType = CHARACTER_POS) const;

    void xy_to_rowcol(int x, int y, int* row, int* column,
                      int PosType = CHARACTER_POS) const;
    void maintain_absolute_top_line_number(int state);
    int get_absolute_top_line_number() const;
    void absolute_top_line_number(int oldFirstChar);
    int maintaining_absolute_top_line_number() const;
    void reset_absolute_top_line_number();
    int position_to_linecol(int pos, int* lineNum, int* column) const;
    int scroll_(int topLineNum, int horizOffset);

    void extend_range_for_styles(int* start, int* end);

    void find_wrap_range(const unsigned char* deletedText, int pos, int nInserted,
                         int nDeleted, int* modRangeStart, int* modRangeEnd,
                         int* linesInserted, int* linesDeleted);
    void measure_deleted_lines(int pos, int nDeleted);
    void wrapped_line_counter(Fl_Text_Buffer* buf, int startPos, int maxPos,
                              int maxLines, bool startPosIsLineStart,
                              int styleBufOffset, int* retPos, int* retLines,
                              int* retLineStart, int* retLineEnd,
                              bool countLastLineMissingNewLine = true) const;
    void find_line_end(int pos, bool start_pos_is_line_start, int* lineEnd,
                       int* nextLineStart) const;
    double measure_proportional_character(const unsigned char* s, int colNum,
                                          int pos) const;
    int wrap_uses_character(int lineEndPos) const;

    int damage_range1_start, damage_range1_end;
    int damage_range2_start, damage_range2_end;
    int mCursorPos;
    int mCursorOn;
    int mCursorOldY;
    int mCursorToHint;
    int mCursorStyle;
    int mCursorPreferredXPos;
    int mNVisibleLines;
    int mNBufferLines;
    Fl_Text_Buffer* mBuffer;
    Fl_Text_Buffer* mStyleBuffer;
    int mFirstChar, mLastChar;
    int mContinuousWrap;
    int mWrapMarginPix;
    struct
    {
      unsigned int left;
      unsigned int right;
      unsigned int top;
      unsigned int bottom;
    } mMargin;
    int* mLineStarts;
    int mTopLineNum;
    int mAbsTopLineNum;
    int mNeedAbsTopLineNum;
    int mHorizOffset;
    int mTopLineNumHint;
    int mHorizOffsetHint;
    int mNStyles;
    char mUnfinishedStyle;
    Unfinished_Style_Cb mUnfinishedHighlightCB;
    void* mHighlightCBArg;

    int mSuppressResync;
    int mNLinesDeleted;
    int mModifyingTabDistance;
    mutable double mColumnScale;
    enum Fl::foreground mCursor_color;

    Fl_Scrollbar* mHScrollBar;
    Fl_Scrollbar* mVScrollBar;
    int scrollbar_width_;
    enum Fl_Label::Fl_Align scrollbar_align_;
    int dragPos, dragType, dragging;
    int display_insert_position_hint;

    struct
    {
      int x, y, w, h;
    } text_area;

    int shortcut_;

    enum Fl::foreground textcolor_;

    int mLineNumLeft, mLineNumWidth;

    enum Fl_Label::Fl_Align    linenumber_align_;
    const unsigned char* linenumber_format_;
};

#endif
