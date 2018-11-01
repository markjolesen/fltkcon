// textbuf.h
//
// Header file for Fl_Text_Buffer class.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//
#ifndef FL_TEXT_BUFFER_H
#define FL_TEXT_BUFFER_H

#include <stdarg.h>

#undef ASSERT_UTF8

#ifdef ASSERT_UTF8
# include <assert.h>
# define IS_UTF8_ALIGNED(a) if (a && *a) assert(fl_utf8len(*(a))>0);
# define IS_UTF8_ALIGNED2(a, b) if (b>=0 && b<a->length()) assert(fl_utf8len(a->byte_at(b))>0);
#else
# define IS_UTF8_ALIGNED(a)
# define IS_UTF8_ALIGNED2(a, b)
#endif
#define FL_TEXT_MAX_EXP_CHAR_LEN 20

class Fl_Text_Selection
{
    friend class Fl_Text_Buffer;

  public:

    void set(int startpos, int endpos);

    void update(int pos, int nDeleted, int nInserted);

    int
    start() const
    {
      return mSelected ? mStart : 0;
    }

    int
    end() const
    {
      return mSelected ? mEnd : 0;
    }

    bool
    selected() const
    {
      return mSelected;
    }

    void
    selected(bool b)
    {
      mSelected = b;
    }

    int
    length() const
    {
      return mSelected ? mEnd - mStart : 0;
    }

    int includes(int pos) const;

    int position(int* startpos, int* endpos) const;

  protected:

    int mStart;
    int mEnd;
    bool mSelected;
};


typedef void (*Fl_Text_Modify_Cb)(int pos, int nInserted, int nDeleted,
                                  int nRestyled, const unsigned char* deletedText,
                                  void* cbArg);


typedef void (*Fl_Text_Predelete_Cb)(int pos, int nDeleted, void* cbArg);


class Fl_Text_Buffer
{
  public:

    Fl_Text_Buffer(int requestedSize = 0, int preferredGapSize = 1024);

    ~Fl_Text_Buffer();

    int
    length() const
    {
      return mLength;
    }

    unsigned char* text() const;

    void text(const unsigned char* text);

    unsigned char* text_range(int start, int end) const;

    unsigned int char_at(int pos) const;

    char byte_at(int pos) const;

    const unsigned char*
    address(int pos) const
    {
      return (pos < mGapStart) ? mBuf + pos : mBuf + pos + mGapEnd - mGapStart;
    }

    unsigned char*
    address(int pos)
    {
      return (pos < mGapStart) ? mBuf + pos : mBuf + pos + mGapEnd - mGapStart;
    }

    void insert(int pos, const unsigned char* text);

    void
    append(const unsigned char* t)
    {
      insert(length(), t);
    }

    void vprintf(const unsigned char* fmt, va_list ap);
    void printf(const unsigned char* fmt, ...);

    void remove(int start, int end);

    void replace(int start, int end, const unsigned char* text);

    void copy(Fl_Text_Buffer* fromBuf, int fromStart, int fromEnd, int toPos);

    int undo(int* cp = 0);

    void canUndo(char flag = 1);

    int insertfile(const unsigned char* file, int pos, int buflen = 128 * 1024);

    int
    appendfile(const unsigned char* file, int buflen = 128 * 1024)
    {
      return insertfile(file, length(), buflen);
    }

    int
    loadfile(const unsigned char* file, int buflen = 128 * 1024)
    {
      select(0, length());
      remove_selection();
      return appendfile(file, buflen);
    }

    int outputfile(const unsigned char* file, int start, int end,
                   int buflen = 128 * 1024);

    int
    savefile(const unsigned char* file, int buflen = 128 * 1024)
    {
      return outputfile(file, 0, length(), buflen);
    }

    int
    tab_distance() const
    {
      return mTabDist;
    }

    void tab_distance(int tabDist);

    void select(int start, int end);

    int
    selected() const
    {
      return mPrimary.selected();
    }

    void unselect();

    int selection_position(int* start, int* end);

    unsigned char* selection_text();

    void remove_selection();

    void replace_selection(const unsigned char* text);

    void secondary_select(int start, int end);

    int
    secondary_selected()
    {
      return mSecondary.selected();
    }

    void secondary_unselect();

    int secondary_selection_position(int* start, int* end);

    unsigned char* secondary_selection_text();

    void remove_secondary_selection();

    void replace_secondary_selection(const unsigned char* text);

    void highlight(int start, int end);

    int
    highlight()
    {
      return mHighlight.selected();
    }

    void unhighlight();

    int highlight_position(int* start, int* end);

    unsigned char* highlight_text();

    void add_modify_callback(Fl_Text_Modify_Cb bufModifiedCB, void* cbArg);

    void remove_modify_callback(Fl_Text_Modify_Cb bufModifiedCB, void* cbArg);

    void
    call_modify_callbacks()
    {
      call_modify_callbacks(0, 0, 0, 0, 0);
    }

    void add_predelete_callback(Fl_Text_Predelete_Cb bufPredelCB, void* cbArg);

    void remove_predelete_callback(Fl_Text_Predelete_Cb predelCB, void* cbArg);

    void
    call_predelete_callbacks()
    {
      call_predelete_callbacks(0, 0);
    }

    unsigned char* line_text(int pos) const;

    int line_start(int pos) const;

    int line_end(int pos) const;

    int word_start(int pos) const;

    int word_end(int pos) const;

    int count_displayed_characters(int lineStartPos, int targetPos) const;

    int skip_displayed_characters(int lineStartPos, int nChars);

    int count_lines(int startPos, int endPos) const;

    int skip_lines(int startPos, int nLines);

    int rewind_lines(int startPos, int nLines);

    int findchar_forward(int startPos, unsigned searchChar, int* foundPos) const;

    int findchar_backward(int startPos, unsigned int searchChar,
                          int* foundPos) const;

    int search_forward(int startPos, const unsigned char* searchString,
                       int* foundPos,
                       int matchCase = 0) const;

    int search_backward(int startPos, const unsigned char* searchString,
                        int* foundPos,
                        int matchCase = 0) const;

    const Fl_Text_Selection*
    primary_selection() const
    {
      return &mPrimary;
    }

    Fl_Text_Selection*
    primary_selection()
    {
      return &mPrimary;
    }

    const Fl_Text_Selection*
    secondary_selection() const
    {
      return &mSecondary;
    }

    const Fl_Text_Selection*
    highlight_selection() const
    {
      return &mHighlight;
    }

    int prev_char(int ix) const;
    int prev_char_clipped(int ix) const;

    int next_char(int ix) const;
    int next_char_clipped(int ix) const;

    int utf8_align(int) const;

    int input_file_was_transcoded;

    static const unsigned char* file_encoding_warning_message;

    void (*transcoding_warning_action)(Fl_Text_Buffer*);
    bool is_word_separator(int pos) const;

  protected:

    void call_modify_callbacks(int pos, int nDeleted, int nInserted,
                               int nRestyled, const unsigned char* deletedText) const;

    void call_predelete_callbacks(int pos, int nDeleted) const;

    int insert_(int pos, const unsigned char* text);

    void remove_(int start, int end);

    void redisplay_selection(Fl_Text_Selection* oldSelection,
                             Fl_Text_Selection* newSelection) const;

    void move_gap(int pos);

    void reallocate_with_gap(int newGapStart, int newGapLen);

    unsigned char* selection_text_(Fl_Text_Selection* sel) const;

    void remove_selection_(Fl_Text_Selection* sel);

    void replace_selection_(Fl_Text_Selection* sel, const unsigned char* text);

    void update_selections(int pos, int nDeleted, int nInserted);

    Fl_Text_Selection mPrimary;
    Fl_Text_Selection mSecondary;
    Fl_Text_Selection mHighlight;
    int mLength;
    unsigned char* mBuf;
    int mGapStart;
    int mGapEnd;
    int mTabDist;
    int mNModifyProcs;
    Fl_Text_Modify_Cb* mModifyProcs;
    void** mCbArgs;
    int mNPredeleteProcs;
    Fl_Text_Predelete_Cb* mPredeleteProcs;
    void** mPredeleteCbArgs;
    int mCursorPosHint;
    char mCanUndo;
    int mPreferredGapSize;
};

#endif
