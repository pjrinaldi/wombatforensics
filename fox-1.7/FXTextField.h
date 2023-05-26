/********************************************************************************
*                                                                               *
*                         T e x t   F i e l d   W i d g e t                     *
*                                                                               *
*********************************************************************************
* Copyright (C) 1997,2022 by Jeroen van der Zijp.   All Rights Reserved.        *
*********************************************************************************
* This library is free software; you can redistribute it and/or modify          *
* it under the terms of the GNU Lesser General Public License as published by   *
* the Free Software Foundation; either version 3 of the License, or             *
* (at your option) any later version.                                           *
*                                                                               *
* This library is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 *
* GNU Lesser General Public License for more details.                           *
*                                                                               *
* You should have received a copy of the GNU Lesser General Public License      *
* along with this program.  If not, see <http://www.gnu.org/licenses/>          *
********************************************************************************/
#ifndef FXTEXTFIELD_H
#define FXTEXTFIELD_H

#ifndef FXFRAME_H
#include "FXFrame.h"
#endif

namespace FX {


/// Textfield styles
enum {
  TEXTFIELD_PASSWD     = 0x00080000,    /// Password mode
  TEXTFIELD_INTEGER    = 0x00100000,    /// Integer mode
  TEXTFIELD_REAL       = 0x00200000,    /// Real mode
  TEXTFIELD_READONLY   = 0x00400000,    /// NOT editable
  TEXTFIELD_ENTER_ONLY = 0x00800000,    /// Only callback when enter hit
  TEXTFIELD_LIMITED    = 0x01000000,    /// Limit entry to given number of columns
  TEXTFIELD_OVERSTRIKE = 0x02000000,    /// Overstrike mode
  TEXTFIELD_AUTOGRAY   = 0x04000000,    /// Automatically gray out text field when not updated
  TEXTFIELD_AUTOHIDE   = 0x08000000,    /// Automatically hide text field when not updated
  TEXTFIELD_NORMAL     = FRAME_SUNKEN|FRAME_THICK
  };


/**
* A text field is a single-line text entry widget.
* The text field widget supports clipboard for cut-and-paste
* operations.
* Text input may be constrained to a certain format; the built-in
* capabilities support integer and real number entry constraints;
* additional constraints on the input may be implemented by intercepting
* the SEL_VERIFY message; a custom handler should examine the tentative
* input string passed as type const FXchar* in the message data, and return
* a value of "0" if the new input is accepted.
* During text entry, the text field sends a SEL_CHANGED message to its target,
* with the message data set to the current text value of type const FXchar*.
* When the text is accepted by hitting ENTER, the SEL_COMMAND message is sent.
* The text field also sends SEL_COMMAND when the focus moves to another control.
* TEXTFIELD_ENTER_ONLY can be passed to suppress this feature. Typically, this
* flag is used in dialogs that close when ENTER is hit in a text field.
*/
class FXAPI FXTextField : public FXFrame {
  FXDECLARE(FXTextField)
protected:
  FXString      contents;       // Edited text
  FXFont       *font;           // Text font
  const FXchar *delimiters;     // Set of delimiters
  FXString      clipped;        // Clipped text
  FXString      help;           // Help string
  FXString      tip;            // Tooltip
  FXColor       textColor;      // Text color
  FXColor       selbackColor;   // Selected background color
  FXColor       seltextColor;   // Selected text color
  FXColor       cursorColor;    // Color of the Cursor
  FXuint        blink;          // Next cursor blink state
  FXint         cursor;         // Cursor position
  FXint         anchor;         // Anchor position
  FXint         columns;        // Number of columns visible
  FXint         shift;          // Shift amount
protected:
  FXTextField();
  FXint index(FXint x) const;
  FXint coord(FXint i) const;
  FXint leftWord(FXint pos) const;
  FXint rightWord(FXint pos) const;
  FXint wordStart(FXint pos) const;
  FXint wordEnd(FXint pos) const;
  void drawCursor(FXuint state);
  void paintCursor(FXDCWindow& dc) const;
  void eraseCursor(FXDCWindow& dc) const;
  void drawTextRange(FXDCWindow& dc,FXint fm,FXint to) const;
  void drawTextFragment(FXDCWindow& dc,FXint x,FXint y,FXint fm,FXint to) const;
  void drawPWDTextFragment(FXDCWindow& dc,FXint x,FXint y,FXint fm,FXint to) const;
  void enterText(const FXString& text,FXbool notify);
  FXbool deletePendingSelection(FXbool notify);
private:
  FXTextField(const FXTextField&);
  FXTextField& operator=(const FXTextField&);
public:
  long onPaint(FXObject*,FXSelector,void*);
  long onUpdate(FXObject*,FXSelector,void*);
  long onBlink(FXObject*,FXSelector,void*);
  long onFocusIn(FXObject*,FXSelector,void*);
  long onFocusOut(FXObject*,FXSelector,void*);
  long onFocusSelf(FXObject*,FXSelector,void*);
  long onMotion(FXObject*,FXSelector,void*);
  long onAutoScroll(FXObject*,FXSelector,void*);
  long onLeftBtnPress(FXObject*,FXSelector,void*);
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
  long onMiddleBtnPress(FXObject*,FXSelector,void*);
  long onMiddleBtnRelease(FXObject*,FXSelector,void*);
  long onKeyPress(FXObject*,FXSelector,void*);
  long onKeyRelease(FXObject*,FXSelector,void*);
  long onVerify(FXObject*,FXSelector,void*);
  long onSelectionLost(FXObject*,FXSelector,void*);
  long onSelectionGained(FXObject*,FXSelector,void*);
  long onSelectionRequest(FXObject*,FXSelector,void* ptr);
  long onClipboardLost(FXObject*,FXSelector,void*);
  long onClipboardGained(FXObject*,FXSelector,void*);
  long onClipboardRequest(FXObject*,FXSelector,void*);
  long onCmdSetTip(FXObject*,FXSelector,void*);
  long onCmdGetTip(FXObject*,FXSelector,void*);
  long onCmdSetHelp(FXObject*,FXSelector,void*);
  long onCmdGetHelp(FXObject*,FXSelector,void*);
  long onQueryTip(FXObject*,FXSelector,void*);
  long onQueryHelp(FXObject*,FXSelector,void*);
  long onUpdIsEditable(FXObject*,FXSelector,void*);
  long onUpdHaveSelection(FXObject*,FXSelector,void*);
  long onUpdHaveEditableSelection(FXObject*,FXSelector,void*);

  // Value access
  long onCmdSetValue(FXObject*,FXSelector,void*);
  long onCmdSetIntValue(FXObject*,FXSelector,void*);
  long onCmdSetLongValue(FXObject*,FXSelector,void*);
  long onCmdSetRealValue(FXObject*,FXSelector,void*);
  long onCmdSetStringValue(FXObject*,FXSelector,void*);
  long onCmdGetIntValue(FXObject*,FXSelector,void*);
  long onCmdGetLongValue(FXObject*,FXSelector,void*);
  long onCmdGetRealValue(FXObject*,FXSelector,void*);
  long onCmdGetStringValue(FXObject*,FXSelector,void*);

  // Cursor movement
  long onCmdCursorHome(FXObject*,FXSelector,void*);
  long onCmdCursorEnd(FXObject*,FXSelector,void*);
  long onCmdCursorRight(FXObject*,FXSelector,void*);
  long onCmdCursorLeft(FXObject*,FXSelector,void*);
  long onCmdCursorWordLeft(FXObject*,FXSelector,void*);
  long onCmdCursorWordRight(FXObject*,FXSelector,void*);

  // Cursor drag movement
  long onCmdCursorShiftHome(FXObject*,FXSelector,void*);
  long onCmdCursorShiftEnd(FXObject*,FXSelector,void*);
  long onCmdCursorShiftRight(FXObject*,FXSelector,void*);
  long onCmdCursorShiftLeft(FXObject*,FXSelector,void*);
  long onCmdCursorShiftWordLeft(FXObject*,FXSelector,void*);
  long onCmdCursorShiftWordRight(FXObject*,FXSelector,void*);

  // Inserting
  long onCmdInsertString(FXObject*,FXSelector,void*);
  long onIMEStart(FXObject*,FXSelector,void*);

  // Manipulation Selection
  long onCmdCutSel(FXObject*,FXSelector,void*);
  long onCmdCopySel(FXObject*,FXSelector,void*);
  long onCmdPasteSel(FXObject*,FXSelector,void*);
  long onCmdDeleteSel(FXObject*,FXSelector,void*);
  long onCmdPasteMiddle(FXObject*,FXSelector,void*);
  long onCmdSelectAll(FXObject*,FXSelector,void*);
  long onCmdDeselectAll(FXObject*,FXSelector,void*);

  // Deletion
  long onCmdBackspace(FXObject*,FXSelector,void*);
  long onCmdBackspaceWord(FXObject*,FXSelector,void*);
  long onCmdBackspaceBol(FXObject*,FXSelector,void*);
  long onCmdDeleteChar(FXObject*,FXSelector,void*);
  long onCmdDeleteWord(FXObject*,FXSelector,void*);
  long onCmdDeleteEol(FXObject*,FXSelector,void*);
  long onCmdDeleteAll(FXObject*,FXSelector,void*);

  // Control commands
  long onCmdToggleEditable(FXObject*,FXSelector,void*);
  long onUpdToggleEditable(FXObject*,FXSelector,void*);
  long onCmdToggleOverstrike(FXObject*,FXSelector,void*);
  long onUpdToggleOverstrike(FXObject*,FXSelector,void*);
public:

  /// Default text delimiters
  static const FXchar textDelimiters[];

public:
  enum{
    ID_CURSOR_HOME=FXFrame::ID_LAST,
    ID_CURSOR_END,
    ID_CURSOR_RIGHT,
    ID_CURSOR_LEFT,
    ID_CURSOR_WORD_LEFT,
    ID_CURSOR_WORD_RIGHT,
    ID_CURSOR_SHIFT_HOME,
    ID_CURSOR_SHIFT_END,
    ID_CURSOR_SHIFT_LEFT,
    ID_CURSOR_SHIFT_RIGHT,
    ID_CURSOR_SHIFT_WORD_LEFT,
    ID_CURSOR_SHIFT_WORD_RIGHT,
    ID_SELECT_ALL,
    ID_DESELECT_ALL,
    ID_CUT_SEL,
    ID_COPY_SEL,
    ID_DELETE_SEL,
    ID_PASTE_SEL,
    ID_PASTE_MIDDLE,
    ID_INSERT_STRING,
    ID_BACKSPACE,
    ID_BACKSPACE_WORD,
    ID_BACKSPACE_BOL,
    ID_DELETE_CHAR,
    ID_DELETE_WORD,
    ID_DELETE_EOL,
    ID_DELETE_ALL,
    ID_TOGGLE_EDITABLE,
    ID_TOGGLE_OVERSTRIKE,
    ID_BLINK,
    ID_AUTOSCROLL,
    ID_LAST
    };
public:

  /// Construct text field wide enough to display ncols columns
  FXTextField(FXComposite* p,FXint ncols,FXObject* tgt=nullptr,FXSelector sel=0,FXuint opts=TEXTFIELD_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);

  /// Create server-side resources
  virtual void create();

  /// Perform layout
  virtual void layout();

  /// Enable text field
  virtual void enable();

  /// Disable text field
  virtual void disable();

  /// Return default width
  virtual FXint getDefaultWidth();

  /// Return default height
  virtual FXint getDefaultHeight();

  /// Yes, text field may receive focus
  virtual FXbool canFocus() const;

  /// Move the focus to this window
  virtual void setFocus();

  /// Remove the focus from this window
  virtual void killFocus();

  /// Set editable mode
  void setEditable(FXbool edit=true);

  /// Return true if text field may be edited
  FXbool isEditable() const;

  /// Set overstrike mode
  void setOverstrike(FXbool over=true);

  /// Return true if overstrike mode in effect
  FXbool isOverstrike() const;

  /// Change the text and move cursor to end
  void setText(const FXString& text,FXbool notify=false);

  /// Replace range of bytes with text
  void replaceText(FXint pos,FXint m,const FXString& text,FXbool notify=false);

  /// Append text at the end
  void appendText(const FXString& text,FXbool notify=false);

  /// Insert text at position
  void insertText(FXint pos,const FXString& text,FXbool notify=false);

  /// Remove range of text
  void removeText(FXint pos,FXint m,FXbool notify=false);

  /// Remove all text
  void clearText(FXbool notify=false);

  /// Get entire text
  FXString getText() const { return contents; }

  /// Select all text
  FXbool selectAll();

  /// Select len characters starting at given position pos
  FXbool setSelection(FXint pos,FXint len);

  /// Extend the selection from the anchor to the given position
  FXbool extendSelection(FXint pos);

  /// Unselect the text
  FXbool killSelection();

  /// Get selected text
  FXString getSelectedText() const;

  /// Copy primary selection to clipboard
  FXbool copySelection();

  /// Cut primary selection to clipboard
  FXbool cutSelection(FXbool notify=false);

  /// Delete primary selection
  FXbool deleteSelection(FXbool notify=false);

  /// Paste primary selection
  FXbool pasteSelection(FXbool notify=false);

  /// Paste clipboard
  FXbool pasteClipboard(FXbool notify=false);

  /// Return true if position pos is selected
  FXbool isPosSelected(FXint pos) const;

  /// Return true if position is fully visible
  FXbool isPosVisible(FXint pos) const;

  /// Scroll text to make the given position visible
  void makePositionVisible(FXint pos);

   /// Set cursor position
  void setCursorPos(FXint pos);

  /// Return cursor position
  FXint getCursorPos() const { return cursor; }

  /// Move cursor to position, and scroll into view
  void moveCursor(FXint pos);

  /// Move cursor to position, and extent the selection to this point
  void moveCursorAndSelect(FXint pos);

  /// Change anchor position
  void setAnchorPos(FXint pos);

  /// Return anchor position
  FXint getAnchorPos() const { return anchor; }

  /// Change text style
  void setTextStyle(FXuint style);

  /// Return text style
  FXuint getTextStyle() const;

  /// Set the text font
  void setFont(FXFont* fnt);

  /// Get the text font
  FXFont* getFont() const { return font; }

  /// Change number of visible columns
  void setNumColumns(FXint cols);

  /// Return number of visible columns
  FXint getNumColumns() const { return columns; }

  /// Change word delimiters
  void setDelimiters(const FXchar* delims=textDelimiters){ delimiters=delims; }

  /// Return word delimiters
  const FXchar* getDelimiters() const { return delimiters; }

  /**
  * Change text justification mode. The justify mode is a combination of
  * horizontal justification (JUSTIFY_LEFT, JUSTIFY_RIGHT, or JUSTIFY_CENTER_X),
  * and vertical justification (JUSTIFY_TOP, JUSTIFY_BOTTOM, JUSTIFY_CENTER_Y).
  * Note that JUSTIFY_CENTER_X can not be set from the constructor since by
  * default text fields are left-justified.
  */
  void setJustify(FXuint mode);

  /// Return text justification mode
  FXuint getJustify() const;

  /// Change text color
  void setTextColor(FXColor clr);

  /// Return text color
  FXColor getTextColor() const { return textColor; }

  /// Change selected background color
  void setSelBackColor(FXColor clr);

  /// Return selected background color
  FXColor getSelBackColor() const { return selbackColor; }

  /// Change selected text color
  void setSelTextColor(FXColor clr);

  /// Return selected text color
  FXColor getSelTextColor() const { return seltextColor; }

  /// Changes the cursor color
  void setCursorColor(FXColor clr);

  /// Return the cursor color
  FXColor getCursorColor() const { return cursorColor; }

  /// Set the status line help text for this label
  void setHelpText(const FXString& text){ help=text; }

  /// Get the status line help text for this label
  const FXString& getHelpText() const { return help; }

  /// Set the tool tip message for this text field
  void setTipText(const FXString& text){ tip=text; }

  /// Get the tool tip message for this text field
  const FXString& getTipText() const { return tip; }

  /// Save text field to a stream
  virtual void save(FXStream& store) const;

  /// Load text field from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXTextField();
  };

}

#endif
