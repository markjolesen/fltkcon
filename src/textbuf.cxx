// textbuf.cxx
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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "textbuf.h"
#include "fl_ask.h"
#include "fl.h"
#include "flstring.h"
#include "fl_sys.h"
#include "fl_utf8.h"

#ifndef min

static int
max(int i1, int i2)
{
  return i1 >= i2 ? i1 : i2;
}

static int
min(int i1, int i2)
{
  return i1 <= i2 ? i1 : i2;
}

#endif


static unsigned char* undobuffer;
static int undobufferlength;
static Fl_Text_Buffer* undowidget;
static int undoat;
static int undocut;
static int undoinsert;
static int
undoyankcut;

static void
undobuffersize(int n)
{
  if (n > undobufferlength)
  {
    if (undobuffer)
    {
      do
      {
        undobufferlength *= 2;
      }
      while (undobufferlength < n);

      undobuffer = (unsigned char*) realloc(undobuffer, undobufferlength);
    }

    else
    {
      undobufferlength = n + 9;
      undobuffer = (unsigned char*) malloc(undobufferlength);
    }
  }
}

static void
def_transcoding_warning_action(Fl_Text_Buffer* text)
{
  fl_alert("%s", text->file_encoding_warning_message);
}

Fl_Text_Buffer::Fl_Text_Buffer(int requestedSize, int preferredGapSize)
{
  mLength = 0;
  mPreferredGapSize = preferredGapSize;
  mBuf = (unsigned char*) malloc(requestedSize + mPreferredGapSize);
  mGapStart = 0;
  mGapEnd = requestedSize + mPreferredGapSize;
  mTabDist = 8;
  mPrimary.mSelected = 0;
  mPrimary.mStart = mPrimary.mEnd = 0;
  mSecondary.mSelected = 0;
  mSecondary.mStart = mSecondary.mEnd = 0;
  mHighlight.mSelected = 0;
  mHighlight.mStart = mHighlight.mEnd = 0;
  mModifyProcs = NULL;
  mCbArgs = NULL;
  mNModifyProcs = 0;
  mNPredeleteProcs = 0;
  mPredeleteProcs = NULL;
  mPredeleteCbArgs = NULL;
  mCursorPosHint = 0;
  mCanUndo = 1;
  input_file_was_transcoded = 0;
  transcoding_warning_action = def_transcoding_warning_action;
}


Fl_Text_Buffer::~Fl_Text_Buffer()
{
  free(mBuf);

  if (mNModifyProcs != 0)
  {
    delete[]mModifyProcs;
    delete[]mCbArgs;
  }

  if (mNPredeleteProcs > 0)
  {
    delete[] mPredeleteProcs;
    delete[] mPredeleteCbArgs;
  }
}


unsigned char*
Fl_Text_Buffer::text() const
{
  unsigned char* t = (unsigned char*) malloc(mLength + 1);
  memcpy(t, mBuf, mGapStart);
  memcpy(t + mGapStart, mBuf + mGapEnd, mLength - mGapStart);
  t[mLength] = '\0';
  return t;
}

void
Fl_Text_Buffer::text(const unsigned char* t)
{
  IS_UTF8_ALIGNED(t)

  if (!t) t = (unsigned char*)"";

  call_predelete_callbacks(0, length());

  const unsigned char* deletedText = text();
  int deletedLength = mLength;
  free((void*) mBuf);

  int insertedLength = (int) strlen((char*)t);
  mBuf = (unsigned char*) malloc(insertedLength + mPreferredGapSize);
  mLength = insertedLength;
  mGapStart = insertedLength;
  mGapEnd = mGapStart + mPreferredGapSize;
  memcpy(mBuf, t, insertedLength);

  update_selections(0, deletedLength, 0);

  call_modify_callbacks(0, deletedLength, insertedLength, 0, deletedText);
  free((void*) deletedText);
}

unsigned char*
Fl_Text_Buffer::text_range(int start, int end) const
{
  IS_UTF8_ALIGNED2(this, (start))
  IS_UTF8_ALIGNED2(this, (end))

  unsigned char* s = NULL;

  if (start < 0 || start > mLength)
  {
    s = (unsigned char*) malloc(1);
    s[0] = '\0';
    return s;
  }

  if (end < start)
  {
    int temp = start;
    start = end;
    end = temp;
  }

  if (end > mLength)
    end = mLength;

  int copiedLength = end - start;
  s = (unsigned char*) malloc(copiedLength + 1);

  if (end <= mGapStart)
  {
    memcpy(s, mBuf + start, copiedLength);
  }

  else if (start >= mGapStart)
  {
    memcpy(s, mBuf + start + (mGapEnd - mGapStart), copiedLength);
  }

  else
  {
    int part1Length = mGapStart - start;
    memcpy(s, mBuf + start, part1Length);
    memcpy(s + part1Length, mBuf + mGapEnd, copiedLength - part1Length);
  }

  s[copiedLength] = '\0';
  return s;
}

unsigned int
Fl_Text_Buffer::char_at(int pos) const
{
  if (pos < 0 || pos >= mLength)
    return '\0';

  IS_UTF8_ALIGNED2(this, (pos))

  const char* src = (char*)address(pos);
  return fl_utf8decode(src, 0, 0);
}


char
Fl_Text_Buffer::byte_at(int pos) const
{
  if (pos < 0 || pos >= mLength)
    return '\0';

  const unsigned char* src = address(pos);
  return *src;
}


void
Fl_Text_Buffer::insert(int pos, const unsigned char* text)
{
  IS_UTF8_ALIGNED2(this, (pos))
  IS_UTF8_ALIGNED(text)

  if (!text || !*text)
    return;

  if (pos > mLength)
    pos = mLength;

  if (pos < 0)
    pos = 0;

  call_predelete_callbacks(pos, 0);

  int nInserted = insert_(pos, text);
  mCursorPosHint = pos + nInserted;
  IS_UTF8_ALIGNED2(this, (mCursorPosHint))
  call_modify_callbacks(pos, 0, nInserted, 0, NULL);
}


void
Fl_Text_Buffer::vprintf(const unsigned char* fmt, va_list ap)
{
  unsigned char buffer[1024];
  ::vsnprintf((char*)buffer, 1024, (char*)fmt, ap);
  buffer[1024 - 1] = 0;
  append(buffer);
}


void
Fl_Text_Buffer::printf(const unsigned char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  Fl_Text_Buffer::vprintf(fmt, ap);
  va_end(ap);
}


void
Fl_Text_Buffer::replace(int start, int end, const unsigned char* text)
{
  if (!text)
    return;

  if (start < 0)
    start = 0;

  if (end > mLength)
    end = mLength;

  IS_UTF8_ALIGNED2(this, (start))
  IS_UTF8_ALIGNED2(this, (end))
  IS_UTF8_ALIGNED(text)

  call_predelete_callbacks(start, end - start);
  const unsigned char* deletedText = text_range(start, end);
  remove_(start, end);
  int nInserted = insert_(start, text);
  mCursorPosHint = start + nInserted;
  call_modify_callbacks(start, end - start, nInserted, 0, deletedText);
  free((void*) deletedText);
}


void
Fl_Text_Buffer::remove(int start, int end)
{
  if (start > end)
  {
    int temp = start;
    start = end;
    end = temp;
  }

  if (start > mLength)
    start = mLength;

  if (start < 0)
    start = 0;

  if (end > mLength)
    end = mLength;

  if (end < 0)
    end = 0;

  IS_UTF8_ALIGNED2(this, (start))
  IS_UTF8_ALIGNED2(this, (end))

  if (start == end)
    return;

  call_predelete_callbacks(start, end - start);
  const unsigned char* deletedText = text_range(start, end);
  remove_(start, end);
  mCursorPosHint = start;
  call_modify_callbacks(start, end - start, 0, 0, deletedText);
  free((void*) deletedText);
}


void
Fl_Text_Buffer::copy(Fl_Text_Buffer* fromBuf, int fromStart,
                     int fromEnd, int toPos)
{
  IS_UTF8_ALIGNED2(fromBuf, fromStart)
  IS_UTF8_ALIGNED2(fromBuf, fromEnd)
  IS_UTF8_ALIGNED2(this, (toPos))

  int copiedLength = fromEnd - fromStart;

  if (copiedLength > mGapEnd - mGapStart)
    reallocate_with_gap(toPos, copiedLength + mPreferredGapSize);

  else if (toPos != mGapStart)
    move_gap(toPos);

  if (fromEnd <= fromBuf->mGapStart)
  {
    memcpy(&mBuf[toPos], &fromBuf->mBuf[fromStart], copiedLength);
  }

  else if (fromStart >= fromBuf->mGapStart)
  {
    memcpy(&mBuf[toPos],
           &fromBuf->mBuf[fromStart + (fromBuf->mGapEnd - fromBuf->mGapStart)],
           copiedLength);
  }

  else
  {
    int part1Length = fromBuf->mGapStart - fromStart;
    memcpy(&mBuf[toPos], &fromBuf->mBuf[fromStart], part1Length);
    memcpy(&mBuf[toPos + part1Length],
           &fromBuf->mBuf[fromBuf->mGapEnd], copiedLength - part1Length);
  }

  mGapStart += copiedLength;
  mLength += copiedLength;
  update_selections(toPos, 0, copiedLength);
}


int
Fl_Text_Buffer::undo(int* cursorPos)
{
  if (undowidget != this || (!undocut && !undoinsert && !mCanUndo))
    return 0;

  int ilen = undocut;
  int xlen = undoinsert;
  int b = undoat - xlen;

  if (xlen && undoyankcut && !ilen)
  {
    ilen = undoyankcut;
  }

  if (xlen && ilen)
  {
    undobuffersize(ilen + 1);
    undobuffer[ilen] = 0;
    unsigned char* tmp = (unsigned char*)strdup((char*)undobuffer);
    replace(b, undoat, tmp);

    if (cursorPos)
      *cursorPos = mCursorPosHint;

    free(tmp);
  }

  else if (xlen)
  {
    remove(b, undoat);

    if (cursorPos)
      *cursorPos = mCursorPosHint;
  }

  else if (ilen)
  {
    undobuffersize(ilen + 1);
    undobuffer[ilen] = 0;
    insert(undoat, undobuffer);

    if (cursorPos)
      *cursorPos = mCursorPosHint;

    undoyankcut = 0;
  }

  return 1;
}


void
Fl_Text_Buffer::canUndo(char flag)
{
  mCanUndo = flag;

  if (!mCanUndo && undowidget == this)
    undowidget = 0;
}


void
Fl_Text_Buffer::tab_distance(int tabDist)
{
  call_predelete_callbacks(0, mLength);

  mTabDist = tabDist;

  const unsigned char* deletedText = text();
  call_modify_callbacks(0, mLength, mLength, 0, deletedText);
  free((void*) deletedText);
}


void
Fl_Text_Buffer::select(int start, int end)
{
  IS_UTF8_ALIGNED2(this, (start))
  IS_UTF8_ALIGNED2(this, (end))

  Fl_Text_Selection oldSelection = mPrimary;

  mPrimary.set(start, end);
  redisplay_selection(&oldSelection, &mPrimary);
}


void
Fl_Text_Buffer::unselect()
{
  Fl_Text_Selection oldSelection = mPrimary;

  mPrimary.mSelected = 0;
  redisplay_selection(&oldSelection, &mPrimary);
}


int
Fl_Text_Buffer::selection_position(int* start, int* end)
{
  return mPrimary.position(start, end);
}


unsigned char*
Fl_Text_Buffer::selection_text()
{
  return selection_text_(&mPrimary);
}


void
Fl_Text_Buffer::remove_selection()
{
  remove_selection_(&mPrimary);
}


void
Fl_Text_Buffer::replace_selection(const unsigned char* text)
{
  replace_selection_(&mPrimary, text);
}


void
Fl_Text_Buffer::secondary_select(int start, int end)
{
  Fl_Text_Selection oldSelection = mSecondary;

  mSecondary.set(start, end);
  redisplay_selection(&oldSelection, &mSecondary);
}


void
Fl_Text_Buffer::secondary_unselect()
{
  Fl_Text_Selection oldSelection = mSecondary;

  mSecondary.mSelected = 0;
  redisplay_selection(&oldSelection, &mSecondary);
}


int
Fl_Text_Buffer::secondary_selection_position(int* start, int* end)
{
  return mSecondary.position(start, end);
}


unsigned char*
Fl_Text_Buffer::secondary_selection_text()
{
  return selection_text_(&mSecondary);
}


void
Fl_Text_Buffer::remove_secondary_selection()
{
  remove_selection_(&mSecondary);
}


void
Fl_Text_Buffer::replace_secondary_selection(const unsigned char* text)
{
  replace_selection_(&mSecondary, text);
}


void
Fl_Text_Buffer::highlight(int start, int end)
{
  Fl_Text_Selection oldSelection = mHighlight;

  mHighlight.set(start, end);
  redisplay_selection(&oldSelection, &mHighlight);
}


void
Fl_Text_Buffer::unhighlight()
{
  Fl_Text_Selection oldSelection = mHighlight;

  mHighlight.mSelected = 0;
  redisplay_selection(&oldSelection, &mHighlight);
}


int
Fl_Text_Buffer::highlight_position(int* start, int* end)
{
  return mHighlight.position(start, end);
}


unsigned char*
Fl_Text_Buffer::highlight_text()
{
  return selection_text_(&mHighlight);
}


void
Fl_Text_Buffer::add_modify_callback(Fl_Text_Modify_Cb bufModifiedCB,
                                    void* cbArg)
{
  Fl_Text_Modify_Cb* newModifyProcs =
    new Fl_Text_Modify_Cb[mNModifyProcs + 1];
  void** newCBArgs = new void* [mNModifyProcs + 1];

  for (int i = 0; i < mNModifyProcs; i++)
  {
    newModifyProcs[i + 1] = mModifyProcs[i];
    newCBArgs[i + 1] = mCbArgs[i];
  }

  if (mNModifyProcs != 0)
  {
    delete[]mModifyProcs;
    delete[]mCbArgs;
  }

  newModifyProcs[0] = bufModifiedCB;
  newCBArgs[0] = cbArg;
  mNModifyProcs++;
  mModifyProcs = newModifyProcs;
  mCbArgs = newCBArgs;
}


void
Fl_Text_Buffer::remove_modify_callback(Fl_Text_Modify_Cb bufModifiedCB,
                                       void* cbArg)
{
  int i, toRemove = -1;

  for (i = 0; i < mNModifyProcs; i++)
  {
    if (mModifyProcs[i] == bufModifiedCB && mCbArgs[i] == cbArg)
    {
      toRemove = i;
      break;
    }
  }

  if (toRemove == -1)
  {
    Fl::error
    ("Fl_Text_Buffer::remove_modify_callback(): Can't find modify CB to remove");
    return;
  }

  mNModifyProcs--;

  if (mNModifyProcs == 0)
  {
    mNModifyProcs = 0;
    delete[]mModifyProcs;
    mModifyProcs = NULL;
    delete[]mCbArgs;
    mCbArgs = NULL;
    return;
  }

  Fl_Text_Modify_Cb* newModifyProcs = new Fl_Text_Modify_Cb[mNModifyProcs];
  void** newCBArgs = new void* [mNModifyProcs];

  for (i = 0; i < toRemove; i++)
  {
    newModifyProcs[i] = mModifyProcs[i];
    newCBArgs[i] = mCbArgs[i];
  }

  for (; i < mNModifyProcs; i++)
  {
    newModifyProcs[i] = mModifyProcs[i + 1];
    newCBArgs[i] = mCbArgs[i + 1];
  }

  delete[]mModifyProcs;
  delete[]mCbArgs;
  mModifyProcs = newModifyProcs;
  mCbArgs = newCBArgs;
}


void
Fl_Text_Buffer::add_predelete_callback(Fl_Text_Predelete_Cb bufPreDeleteCB,
                                       void* cbArg)
{
  Fl_Text_Predelete_Cb* newPreDeleteProcs =
    new Fl_Text_Predelete_Cb[mNPredeleteProcs + 1];
  void** newCBArgs = new void* [mNPredeleteProcs + 1];

  for (int i = 0; i < mNPredeleteProcs; i++)
  {
    newPreDeleteProcs[i + 1] = mPredeleteProcs[i];
    newCBArgs[i + 1] = mPredeleteCbArgs[i];
  }

  if (mNPredeleteProcs > 0)
  {
    delete[] mPredeleteProcs;
    delete[] mPredeleteCbArgs;
  }

  newPreDeleteProcs[0] = bufPreDeleteCB;
  newCBArgs[0] = cbArg;
  mNPredeleteProcs++;
  mPredeleteProcs = newPreDeleteProcs;
  mPredeleteCbArgs = newCBArgs;
}


void
Fl_Text_Buffer::remove_predelete_callback(Fl_Text_Predelete_Cb bufPreDeleteCB,
                                          void* cbArg)
{
  int i, toRemove = -1;

  for (i = 0; i < mNPredeleteProcs; i++)
  {
    if (mPredeleteProcs[i] == bufPreDeleteCB &&
        mPredeleteCbArgs[i] == cbArg)
    {
      toRemove = i;
      break;
    }
  }

  if (toRemove == -1)
  {
    Fl::error
    ("Fl_Text_Buffer::remove_predelete_callback(): Can't find pre-delete CB to remove");
    return;
  }

  mNPredeleteProcs--;

  if (mNPredeleteProcs == 0)
  {
    delete[]mPredeleteProcs;
    mPredeleteProcs = NULL;
    delete[]mPredeleteCbArgs;
    mPredeleteCbArgs = NULL;
    return;
  }

  Fl_Text_Predelete_Cb* newPreDeleteProcs = new
  Fl_Text_Predelete_Cb[mNPredeleteProcs];
  void** newCBArgs = new void* [mNPredeleteProcs];

  for (i = 0; i < toRemove; i++)
  {
    newPreDeleteProcs[i] = mPredeleteProcs[i];
    newCBArgs[i] = mPredeleteCbArgs[i];
  }

  for (; i < mNPredeleteProcs; i++)
  {
    newPreDeleteProcs[i] = mPredeleteProcs[i + 1];
    newCBArgs[i] = mPredeleteCbArgs[i + 1];
  }

  delete[] mPredeleteProcs;
  delete[] mPredeleteCbArgs;
  mPredeleteProcs = newPreDeleteProcs;
  mPredeleteCbArgs = newCBArgs;
}


unsigned char*
Fl_Text_Buffer::line_text(int pos) const
{
  return text_range(line_start(pos), line_end(pos));
}


int
Fl_Text_Buffer::line_start(int pos) const
{
  if (!findchar_backward(pos, '\n', &pos))
    return 0;

  return pos + 1;
}


int
Fl_Text_Buffer::line_end(int pos) const
{
  if (!findchar_forward(pos, '\n', &pos))
    pos = mLength;

  return pos;
}


bool
Fl_Text_Buffer::is_word_separator(int pos) const
{
  int c = char_at(pos);

  if (c < 128)
  {
    return !(isalnum(c) || c == '_');
  }

  return (c == 0xA0 || (c >= 0x3000 && c <= 0x301F));
}


int
Fl_Text_Buffer::word_start(int pos) const
{
  while (pos > 0 && !is_word_separator(pos))
  {
    pos = prev_char(pos);
  }

  if (is_word_separator(pos))
    pos = next_char(pos);

  return pos;
}


int
Fl_Text_Buffer::word_end(int pos) const
{
  while (pos < length() && !is_word_separator(pos))
  {
    pos = next_char(pos);
  }

  return pos;
}


int
Fl_Text_Buffer::count_displayed_characters(int lineStartPos,
                                           int targetPos) const
{
  IS_UTF8_ALIGNED2(this, (lineStartPos))
  IS_UTF8_ALIGNED2(this, (targetPos))

  int charCount = 0;

  int pos = lineStartPos;

  while (pos < targetPos)
  {
    pos = next_char(pos);
    charCount++;
  }

  return charCount;
}


int
Fl_Text_Buffer::skip_displayed_characters(int lineStartPos, int nChars)
{
  IS_UTF8_ALIGNED2(this, (lineStartPos))

  int pos = lineStartPos;

  for (int charCount = 0; charCount < nChars && pos < mLength; charCount++)
  {
    unsigned int c = char_at(pos);

    if (c == '\n')
      return pos;

    pos = next_char(pos);
  }

  return pos;
}

int
Fl_Text_Buffer::count_lines(int startPos, int endPos) const
{
  IS_UTF8_ALIGNED2(this, (startPos))
  IS_UTF8_ALIGNED2(this, (endPos))

  int gapLen = mGapEnd - mGapStart;
  int lineCount = 0;

  int pos = startPos;

  while (pos < mGapStart)
  {
    if (pos == endPos)
      return lineCount;

    if (mBuf[pos++] == '\n')
      lineCount++;
  }

  while (pos < mLength)
  {
    if (pos == endPos)
      return lineCount;

    if (mBuf[pos++ + gapLen] == '\n')
      lineCount++;
  }

  return lineCount;
}


int
Fl_Text_Buffer::skip_lines(int startPos, int nLines)
{
  IS_UTF8_ALIGNED2(this, (startPos))

  if (nLines == 0)
    return startPos;

  int gapLen = mGapEnd - mGapStart;
  int pos = startPos;
  int lineCount = 0;

  while (pos < mGapStart)
  {
    if (mBuf[pos++] == '\n')
    {
      lineCount++;

      if (lineCount == nLines)
      {
        IS_UTF8_ALIGNED2(this, (pos))
        return pos;
      }
    }
  }

  while (pos < mLength)
  {
    if (mBuf[pos++ + gapLen] == '\n')
    {
      lineCount++;

      if (lineCount >= nLines)
      {
        IS_UTF8_ALIGNED2(this, (pos))
        return pos;
      }
    }
  }

  IS_UTF8_ALIGNED2(this, (pos))
  return pos;
}


int
Fl_Text_Buffer::rewind_lines(int startPos, int nLines)
{
  IS_UTF8_ALIGNED2(this, (startPos))

  int pos = startPos - 1;

  if (pos <= 0)
    return 0;

  int gapLen = mGapEnd - mGapStart;
  int lineCount = -1;

  while (pos >= mGapStart)
  {
    if (mBuf[pos + gapLen] == '\n')
    {
      if (++lineCount >= nLines)
      {
        IS_UTF8_ALIGNED2(this, (pos + 1))
        return pos + 1;
      }
    }

    pos--;
  }

  while (pos >= 0)
  {
    if (mBuf[pos] == '\n')
    {
      if (++lineCount >= nLines)
      {
        IS_UTF8_ALIGNED2(this, (pos + 1))
        return pos + 1;
      }
    }

    pos--;
  }

  return 0;
}


int
Fl_Text_Buffer::search_forward(int startPos, const unsigned char* searchString,
                               int* foundPos, int matchCase) const
{
  IS_UTF8_ALIGNED2(this, (startPos))
  IS_UTF8_ALIGNED(searchString)

  if (!searchString)
    return 0;

  int bp;
  const unsigned char* sp;

  if (matchCase)
  {
    while (startPos < length())
    {
      bp = startPos;
      sp = searchString;

      for (;;)
      {
        char c = *sp;

        if (!c)
        {
          *foundPos = startPos;
          return 1;
        }

        int l = fl_utf8len1(c);

        if (memcmp(sp, address(bp), l))
          break;

        sp += l;
        bp += l;
      }

      startPos = next_char(startPos);
    }
  }

  else
  {
    while (startPos < length())
    {
      bp = startPos;
      sp = searchString;

      for (;;)
      {
        if (!*sp)
        {
          *foundPos = startPos;
          return 1;
        }

        int l;
        unsigned int b = char_at(bp);
        unsigned int s = fl_utf8decode((char*)sp, 0, &l);

        if (fl_tolower(b) != fl_tolower(s))
          break;

        sp += l;
        bp = next_char(bp);
      }

      startPos = next_char(startPos);
    }
  }

  return 0;
}

int
Fl_Text_Buffer::search_backward(int startPos, const unsigned char* searchString,
                                int* foundPos, int matchCase) const
{
  IS_UTF8_ALIGNED2(this, (startPos))
  IS_UTF8_ALIGNED(searchString)

  if (!searchString)
    return 0;

  int bp;
  const unsigned char* sp;

  if (matchCase)
  {
    while (startPos >= 0)
    {
      bp = startPos;
      sp = searchString;

      for (;;)
      {
        char c = *sp;

        if (!c)
        {
          *foundPos = startPos;
          return 1;
        }

        int l = fl_utf8len1(c);

        if (memcmp(sp, address(bp), l))
          break;

        sp += l;
        bp += l;
      }

      startPos = prev_char(startPos);
    }
  }

  else
  {
    while (startPos >= 0)
    {
      bp = startPos;
      sp = searchString;

      for (;;)
      {
        if (!*sp)
        {
          *foundPos = startPos;
          return 1;
        }

        int l;
        unsigned int b = char_at(bp);
        unsigned int s = fl_utf8decode((char*)sp, 0, &l);

        if (fl_tolower(b) != fl_tolower(s))
          break;

        sp += l;
        bp = next_char(bp);
      }

      startPos = prev_char(startPos);
    }
  }

  return 0;
}



int
Fl_Text_Buffer::insert_(int pos, const unsigned char* text)
{
  if (!text || !*text)
    return 0;

  int insertedLength = (int) strlen((char*)text);

  if (insertedLength > mGapEnd - mGapStart)
    reallocate_with_gap(pos, insertedLength + mPreferredGapSize);

  else if (pos != mGapStart)
    move_gap(pos);

  memcpy(&mBuf[pos], text, insertedLength);
  mGapStart += insertedLength;
  mLength += insertedLength;
  update_selections(pos, 0, insertedLength);

  if (mCanUndo)
  {
    if (undowidget == this && undoat == pos && undoinsert)
    {
      undoinsert += insertedLength;
    }

    else
    {
      undoinsert = insertedLength;
      undoyankcut = (undoat == pos) ? undocut : 0;
    }

    undoat = pos + insertedLength;
    undocut = 0;
    undowidget = this;
  }

  return insertedLength;
}


void
Fl_Text_Buffer::remove_(int start, int end)
{

  if (mCanUndo)
  {
    if (undowidget == this && undoat == end && undocut)
    {
      undobuffersize(undocut + end - start + 1);
      memmove(undobuffer + end - start, undobuffer, undocut);
      undocut += end - start;
    }

    else
    {
      undocut = end - start;
      undobuffersize(undocut);
    }

    undoat = start;
    undoinsert = 0;
    undoyankcut = 0;
    undowidget = this;
  }

  if (start > mGapStart)
  {
    if (mCanUndo)
      memcpy(undobuffer, mBuf + (mGapEnd - mGapStart) + start,
             end - start);

    move_gap(start);
  }

  else if (end < mGapStart)
  {
    if (mCanUndo)
      memcpy(undobuffer, mBuf + start, end - start);

    move_gap(end);
  }

  else
  {
    int prelen = mGapStart - start;

    if (mCanUndo)
    {
      memcpy(undobuffer, mBuf + start, prelen);
      memcpy(undobuffer + prelen, mBuf + mGapEnd, end - start - prelen);
    }
  }

  mGapEnd += end - mGapStart;
  mGapStart -= mGapStart - start;

  mLength -= end - start;

  update_selections(start, end - start, 0);
}


void
Fl_Text_Selection::set(int startpos, int endpos)
{
  mSelected = (startpos != endpos);
  mStart = min(startpos, endpos);
  mEnd = max(startpos, endpos);
}


int
Fl_Text_Selection::position(int* startpos, int* endpos) const
{
  if (!mSelected)
  {
    *startpos = 0;
    *endpos = 0;
    return 0;
  }

  *startpos = mStart;
  *endpos = mEnd;
  return 1;
}


int
Fl_Text_Selection::includes(int pos) const
{
  return (selected() && pos >= start() && pos < end() );
}


unsigned char*
Fl_Text_Buffer::selection_text_(Fl_Text_Selection* sel) const
{
  int start, end;

  if (!sel->position(&start, &end))
  {
    unsigned char* s = (unsigned char*) malloc(1);
    *s = '\0';
    return s;
  }

  return text_range(start, end);
}

void
Fl_Text_Buffer::remove_selection_(Fl_Text_Selection* sel)
{
  int start, end;

  if (!sel->position(&start, &end))
    return;

  remove(start, end);
}


void
Fl_Text_Buffer::replace_selection_(Fl_Text_Selection* sel,
                                   const unsigned char* text)
{
  Fl_Text_Selection oldSelection = *sel;

  int start, end;

  if (!sel->position(&start, &end))
    return;

  replace(start, end, text);

  sel->mSelected = 0;
  redisplay_selection(&oldSelection, sel);
}


void
Fl_Text_Buffer::call_modify_callbacks(int pos, int nDeleted,
                                      int nInserted, int nRestyled,
                                      const unsigned char* deletedText) const
{
  IS_UTF8_ALIGNED2(this, pos)

  for (int i = 0; i < mNModifyProcs; i++)
    (*mModifyProcs[i]) (pos, nInserted, nDeleted, nRestyled,
                        deletedText, mCbArgs[i]);
}


void
Fl_Text_Buffer::call_predelete_callbacks(int pos, int nDeleted) const
{
  for (int i = 0; i < mNPredeleteProcs; i++)
    (*mPredeleteProcs[i]) (pos, nDeleted, mPredeleteCbArgs[i]);
}


void
Fl_Text_Buffer::redisplay_selection(Fl_Text_Selection*
                                    oldSelection,
                                    Fl_Text_Selection*
                                    newSelection) const
{
  int oldStart, oldEnd, newStart, newEnd, ch1Start, ch1End, ch2Start,
      ch2End;

  oldStart = oldSelection->mStart;
  newStart = newSelection->mStart;
  oldEnd = oldSelection->mEnd;
  newEnd = newSelection->mEnd;

  if (!oldSelection->mSelected && !newSelection->mSelected)
    return;

  if (!oldSelection->mSelected)
  {
    call_modify_callbacks(newStart, 0, 0, newEnd - newStart, NULL);
    return;
  }

  if (!newSelection->mSelected)
  {
    call_modify_callbacks(oldStart, 0, 0, oldEnd - oldStart, NULL);
    return;
  }

  if (oldEnd < newStart || newEnd < oldStart)
  {
    call_modify_callbacks(oldStart, 0, 0, oldEnd - oldStart, NULL);
    call_modify_callbacks(newStart, 0, 0, newEnd - newStart, NULL);
    return;
  }

  ch1Start = min(oldStart, newStart);
  ch2End = max(oldEnd, newEnd);
  ch1End = max(oldStart, newStart);
  ch2Start = min(oldEnd, newEnd);

  if (ch1Start != ch1End)
    call_modify_callbacks(ch1Start, 0, 0, ch1End - ch1Start, NULL);

  if (ch2Start != ch2End)
    call_modify_callbacks(ch2Start, 0, 0, ch2End - ch2Start, NULL);
}


void
Fl_Text_Buffer::move_gap(int pos)
{
  int gapLen = mGapEnd - mGapStart;

  if (pos > mGapStart)
    memmove(&mBuf[mGapStart], &mBuf[mGapEnd], pos - mGapStart);

  else
    memmove(&mBuf[pos + gapLen], &mBuf[pos], mGapStart - pos);

  mGapEnd += pos - mGapStart;
  mGapStart += pos - mGapStart;
}


void
Fl_Text_Buffer::reallocate_with_gap(int newGapStart, int newGapLen)
{
  unsigned char* newBuf = (unsigned char*) malloc(mLength + newGapLen);
  int newGapEnd = newGapStart + newGapLen;

  if (newGapStart <= mGapStart)
  {
    memcpy(newBuf, mBuf, newGapStart);
    memcpy(&newBuf[newGapEnd], &mBuf[newGapStart],
           mGapStart - newGapStart);
    memcpy(&newBuf[newGapEnd + mGapStart - newGapStart],
           &mBuf[mGapEnd], mLength - mGapStart);
  }

  else
  {
    memcpy(newBuf, mBuf, mGapStart);
    memcpy(&newBuf[mGapStart], &mBuf[mGapEnd], newGapStart - mGapStart);
    memcpy(&newBuf[newGapEnd],
           &mBuf[mGapEnd + newGapStart - mGapStart],
           mLength - newGapStart);
  }

  free((void*) mBuf);
  mBuf = newBuf;
  mGapStart = newGapStart;
  mGapEnd = newGapEnd;
}


void
Fl_Text_Buffer::update_selections(int pos, int nDeleted,
                                  int nInserted)
{
  mPrimary.update(pos, nDeleted, nInserted);
  mSecondary.update(pos, nDeleted, nInserted);
  mHighlight.update(pos, nDeleted, nInserted);
}


void
Fl_Text_Selection::update(int pos, int nDeleted, int nInserted)
{
  if (!mSelected || pos > mEnd)
    return;

  if (pos + nDeleted <= mStart)
  {
    mStart += nInserted - nDeleted;
    mEnd += nInserted - nDeleted;
  }

  else if (pos <= mStart && pos + nDeleted >= mEnd)
  {
    mStart = pos;
    mEnd = pos;
    mSelected = 0;
  }

  else if (pos <= mStart && pos + nDeleted < mEnd)
  {
    mStart = pos;
    mEnd = nInserted + mEnd - nDeleted;
  }

  else if (pos < mEnd)
  {
    mEnd += nInserted - nDeleted;

    if (mEnd <= mStart)
      mSelected = 0;
  }
}


int
Fl_Text_Buffer::findchar_forward(int startPos, unsigned searchChar,
                                 int* foundPos) const
{
  if (startPos >= mLength)
  {
    *foundPos = mLength;
    return 0;
  }

  if (startPos < 0)
    startPos = 0;

  for ( ; startPos < mLength; startPos = next_char(startPos))
  {
    if (searchChar == char_at(startPos))
    {
      *foundPos = startPos;
      return 1;
    }
  }

  *foundPos = mLength;
  return 0;
}


int
Fl_Text_Buffer::findchar_backward(int startPos, unsigned int searchChar,
                                  int* foundPos) const
{
  if (startPos <= 0)
  {
    *foundPos = 0;
    return 0;
  }

  if (startPos > mLength)
    startPos = mLength;

  for (startPos = prev_char(startPos); startPos >= 0;
       startPos = prev_char(startPos))
  {
    if (searchChar == char_at(startPos))
    {
      *foundPos = startPos;
      return 1;
    }
  }

  *foundPos = 0;
  return 0;
}

#ifdef EXAMPLE_ENCODING

unsigned
cp1252toucs(unsigned char*& p)
{
  static unsigned cp1252[32] =
  {
    0x20ac, 0x0081, 0x201a, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021,
    0x02c6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008d, 0x017d, 0x008f,
    0x0090, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
    0x02dc, 0x2122, 0x0161, 0x203a, 0x0153, 0x009d, 0x017e, 0x0178
  };
  unsigned char uc = *(unsigned char*)p;
  p++;
  return (uc < 0x80 || uc >= 0xa0 ? uc : cp1252[uc - 0x80]);
}

unsigned
utf16toucs(unsigned char*& p)
{
  union
  {
#if WORDS_BIGENDIAN
    struct
    {
      unsigned char a, b;
    } chars;
#else
    struct
    {
      unsigned char b, a;
    } chars;
#endif
    U16 short_val;
  } u;
  u.chars.a = *(unsigned char*)p++;
  u.chars.b = *(unsigned char*)p++;
  return u.short_val;
}

static int
general_input_filter(unsigned char* buffer, int buflen,
                     unsigned char* line, int sline, unsigned char*& endline,
                     unsigned (*p_trf)(unsigned char*&),
                     FILE* fp)
{
  unsigned char* p, *q, multibyte[5];
  int lq, r, offset;
  p = line;
  q = buffer;

  while (q < buffer + buflen)
  {
    if (p >= endline)
    {
      r = fread(line, 1, sline, fp);
      endline = line + r;

      if (r == 0) return q - buffer;

      p = line;
    }

    if (q + 4 /*max width of UTF-8 char*/ > buffer + buflen)
    {
      memmove(line, p, endline - p);
      endline -= (p - line);
      return q - buffer;
    }

    lq = fl_utf8encode( p_trf(p), multibyte );
    memcpy(q, multibyte, lq);
    q += lq;
  }

  memmove(line, p, endline - p);
  endline -= (p - line);
  return q - buffer;
}
#endif // EXAMPLE_ENCODING

static int
utf8_input_filter(unsigned char*
                  buffer,
                  int buflen,
                  unsigned char* line,
                  int sline,
                  unsigned char*& endline,
                  FILE* fp,
                  int* input_was_changed)
{
  unsigned char* p, *q, multibyte[5];
  int l, lp, lq, r;
  unsigned u;
  p = line;
  q = buffer;

  while (q < buffer + buflen)
  {
    if (p >= endline)
    {
      r = (int) fread(line, 1, sline,
                      fp);
      endline = line + r;

      if (r == 0) return (int) (q - buffer);

      p = line;
    }

    l = fl_utf8len1(*p);

    if (p + l > endline)
    {
      memmove(line, p, endline - p);
      endline -= (p - line);
      r = (int) fread(endline, 1, sline - (endline - line), fp);
      endline += r;
      p = line;

      if (endline - line < l) break;
    }

    while ( l > 0)
    {
      u = fl_utf8decode((char*)p, (char*)p + l, &lp);
      lq = fl_utf8encode(u, (char*)multibyte);

      if (lp != l || lq != l) *input_was_changed = true;

      if (q + lq > buffer + buflen)
      {
        memmove(line, p, endline - p);
        endline -= (p - line);
        return (int) (q - buffer); 
      }

      memcpy(q, multibyte, lq);
      q += lq;
      p += lp;
      l -= lp;
    }
  }

  memmove(line, p, endline - p);
  endline -= (p - line);
  return (int) (q - buffer);
}

const unsigned char* Fl_Text_Buffer::file_encoding_warning_message =
  (unsigned char*)
  "Displayed text contains the UTF-8 transcoding\n"
  "of the input file which was not UTF-8 encoded.\n"
  "Some changes may have occurred.";

int
Fl_Text_Buffer::insertfile(const unsigned char* file, int pos, int buflen)
{
  FILE* fp;

  if (!(fp = Fl::fopen((char*)file, "r")))
    return 1;

  unsigned char* buffer = new unsigned char[buflen + 1];
  unsigned char* endline, line[100];
  int l;
  input_file_was_transcoded = false;
  endline = line;

  while (true)
  {
#ifdef EXAMPLE_ENCODING
    // example of 16-bit encoding: UTF-16
    l = general_input_filter(buffer, buflen,
                             line, sizeof(line), endline,
                             utf16toucs, // use cp1252toucs to read CP1252-encoded files
                             fp);
    input_file_was_transcoded = true;
#else
    l = utf8_input_filter(buffer, buflen, line, sizeof(line), endline,
                          fp, &input_file_was_transcoded);
#endif

    if (l == 0) break;

    buffer[l] = 0;
    insert(pos, buffer);
    pos += l;
  }

  int e = ferror(fp) ? 2 : 0;
  fclose(fp);
  delete[]buffer;

  if ( (!e) && input_file_was_transcoded && transcoding_warning_action)
  {
    transcoding_warning_action(this);
  }

  return e;
}


int
Fl_Text_Buffer::outputfile(const unsigned char* file,
                           int start, int end,
                           int buflen)
{
  FILE* fp;

  if (!(fp = Fl::fopen((char*)file, "w")))
    return 1;

  for (int n; (n = min(end - start, buflen)); start += n)
  {
    const unsigned char* p = text_range(start, start + n);
    int r = (int) fwrite(p, 1, n, fp);
    free((void*) p);

    if (r != n)
      break;
  }

  int e = ferror(fp) ? 2 : 0;
  fclose(fp);
  return e;
}


int
Fl_Text_Buffer::prev_char_clipped(int pos) const
{
  if (pos <= 0)
    return 0;

  IS_UTF8_ALIGNED2(this, (pos))

  char c;

  do
  {
    pos--;

    if (pos == 0)
      return 0;

    c = byte_at(pos);
  }
  while ( (c & 0xc0) == 0x80);

  IS_UTF8_ALIGNED2(this, (pos))
  return pos;
}


int
Fl_Text_Buffer::prev_char(int pos) const
{
  if (pos == 0) return -1;

  return prev_char_clipped(pos);
}


int
Fl_Text_Buffer::next_char(int pos) const
{
  IS_UTF8_ALIGNED2(this, (pos))
  int n = fl_utf8len1(byte_at(pos));
  pos += n;

  if (pos >= mLength)
    return mLength;

  IS_UTF8_ALIGNED2(this, (pos))
  return pos;
}


int
Fl_Text_Buffer::next_char_clipped(int pos) const
{
  return next_char(pos);
}

int
Fl_Text_Buffer::utf8_align(int pos) const
{
  char c = byte_at(pos);

  while ( (c & 0xc0) == 0x80)
  {
    pos--;
    c = byte_at(pos);
  }

  return pos;
}
