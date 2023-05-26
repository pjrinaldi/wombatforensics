/********************************************************************************
*                                                                               *
*                       S c r o l l   B a r   W i d g e t                       *
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
#ifndef FXSCROLLBAR_H
#define FXSCROLLBAR_H

#ifndef FXWINDOW_H
#include "FXWindow.h"
#endif

namespace FX {


/// ScrollBar styles
enum {
  SCROLLBAR_VERTICAL   = 0,             /// Vertically oriented
  SCROLLBAR_HORIZONTAL = 0x00020000,    /// Horizontally oriented
  SCROLLBAR_WHEELJUMP  = 0x00040000     /// Mouse wheel jumps instead of sliding smoothly
  };



/**
* The scroll bar is used when a document has a larger content than may be made
* visible.  The range is the total size of the document, the page size is the viewable
* space available for the document.  The size of the scrollbar thumb is adjusted to give
* feedback of the relative sizes of each.
* The scroll bar may be manipulated by the left mouse button (normal scrolling), by the
* middle mouse button (same as the left mouse only the scroll position can jump to the
* place where the click is made), or by the right mouse button (vernier- or fine-scrolling).
* Holding down the control key while scrolling with the left or middle mouse button also
* enables vernier-scrolling mode.  The vernier-scrolling mode is very useful for accurate
* positioning in large documents.
* Finally, if the mouse sports a wheel, the scroll bar can be manipulated by means
* of the mouse wheel as well.  Holding down the Control-key during wheel motion
* will cause the scrolling to go faster than normal.
* While moving the scroll bar, a message of type SEL_CHANGED will be sent to the
* target, and the message data will reflect the current position of type FXint.
* At the end of the interaction, the scroll bar will send a message of type
* SEL_COMMAND to notify the target of the final position.
*/
class FXAPI FXScrollBar : public FXWindow {
  FXDECLARE(FXScrollBar)
protected:
  FXint      range;           // Scrollable range
  FXint      page;            // Page size
  FXint      line;            // Line size
  FXint      pos;             // Position
  FXint      barsize;         // Bar size
  FXint      thumbsize;       // Thumb size
  FXint      thumbpos;        // Thumb position
  FXint      wheelLines;      // Scroll wheel lines
  FXColor    hiliteColor;     // Hightlight color
  FXColor    shadowColor;     // Shadow color
  FXColor    borderColor;     // Border color
  FXColor    arrowColor;      // Arrow color
  FXint      dragpoint;       // Point where grabbed
  FXuchar    mode;            // Current mode of control
protected:
  FXScrollBar();
  void drawButton(FXDCWindow& dc,FXint x,FXint y,FXint w,FXint h,FXbool down);
  void drawThumb(FXDCWindow& dc,FXint x,FXint y,FXint w,FXint h);
  void drawLeftArrow(FXDCWindow& dc,FXint x,FXint y,FXint w,FXint h,FXbool down);
  void drawRightArrow(FXDCWindow& dc,FXint x,FXint y,FXint w,FXint h,FXbool down);
  void drawUpArrow(FXDCWindow& dc,FXint x,FXint y,FXint w,FXint h,FXbool down);
  void drawDownArrow(FXDCWindow& dc,FXint x,FXint y,FXint w,FXint h,FXbool down);
protected:
  enum {
    MODE_NONE,
    MODE_INC,
    MODE_DEC,
    MODE_PAGE_INC,
    MODE_PAGE_DEC,
    MODE_DRAG,
    MODE_FINE_DRAG
    };
private:
  FXScrollBar(const FXScrollBar&);
  FXScrollBar &operator=(const FXScrollBar&);
public:
  long onPaint(FXObject*,FXSelector,void*);
  long onMotion(FXObject*,FXSelector,void*);
  long onMouseWheel(FXObject*,FXSelector,void*);
  long onLeftBtnPress(FXObject*,FXSelector,void*);
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
  long onMiddleBtnPress(FXObject*,FXSelector,void*);
  long onMiddleBtnRelease(FXObject*,FXSelector,void*);
  long onRightBtnPress(FXObject*,FXSelector,void*);
  long onRightBtnRelease(FXObject*,FXSelector,void*);
  long onUngrabbed(FXObject*,FXSelector,void*);
  long onTimeWheel(FXObject*,FXSelector,void*);
  long onAutoScroll(FXObject*,FXSelector,void*);
  long onCmdSetValue(FXObject*,FXSelector,void*);
  long onCmdSetIntValue(FXObject*,FXSelector,void*);
  long onCmdGetIntValue(FXObject*,FXSelector,void*);
  long onCmdSetLongValue(FXObject*,FXSelector,void*);
  long onCmdGetLongValue(FXObject*,FXSelector,void*);
  long onCmdSetIntRange(FXObject*,FXSelector,void*);
  long onCmdGetIntRange(FXObject*,FXSelector,void*);
public:
  enum{
    ID_AUTOSCROLL=FXWindow::ID_LAST,
    ID_TIMEWHEEL,
    ID_LAST
    };
public:

  /// Construct scroll bar
  FXScrollBar(FXComposite* p,FXObject* tgt=nullptr,FXSelector sel=0,FXuint opts=SCROLLBAR_VERTICAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Return default width
  virtual FXint getDefaultWidth();

  /// Return default height
  virtual FXint getDefaultHeight();

  /// Perform layout
  virtual void layout();

  /**
  * Set content size range.  The range must be at least 1,
  * but may be smaller than the viewable page size.
  */
  void setRange(FXint r,FXbool notify=false);

  /// Return content size range
  FXint getRange() const { return range; }

  /**
  * Set the viewable page size. The page size must be at least 1,
  * but may be larger than the range.
  */
  void setPage(FXint p,FXbool notify=false);

  /// Return viewable page size
  FXint getPage() const { return page; }

  /**
  * Change scroll position.  The position is always greater or equal
  * to 0, up to the range less the page size.  If the range is less
  * than the page size, the position will simply be equal to zero.
  */
  void setPosition(FXint p,FXbool notify=false);

  /// Return current scroll position
  FXint getPosition() const { return pos; }

  /// Set scoll increment for line
  void setLine(FXint l);

  /// Return line increment
  FXint getLine() const { return line; }

  /// Change mouse wheel scroll lines
  void setWheelLines(FXint lines){ wheelLines=lines; }

  /// Return current wheel lines
  FXint getWheelLines() const { return wheelLines; }

  /// Change highlight color
  void setHiliteColor(FXColor clr);

  /// Return highlight color
  FXColor getHiliteColor() const { return hiliteColor; }

  /// Change the shadow color
  void setShadowColor(FXColor clr);

  /// Return the shadow color
  FXColor getShadowColor() const { return shadowColor; }

  /// Change the border color
  void setBorderColor(FXColor clr);

  /// Return the border color
  FXColor getBorderColor() const { return borderColor; }

  /// Change the arrow color
  void setArrowColor(FXColor clr);

  /// Return the arrow color
  FXColor getArrowColor() const { return arrowColor; }

  /// Change the scrollbar style
  void setScrollBarStyle(FXuint style);

  /// Return the scrollbar style
  FXuint getScrollBarStyle() const;

  /// Change the bar size
  void setBarSize(FXint size);

  /// Return the bar size
  FXint getBarSize() const { return barsize; }

  /// Save to stream
  virtual void save(FXStream& store) const;

  /// Load from stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXScrollBar();
  };


/// Corner between scroll bars
class FXAPI FXScrollCorner : public FXWindow {
  FXDECLARE(FXScrollCorner)
protected:
  FXScrollCorner();
private:
  FXScrollCorner(const FXScrollCorner&);
  FXScrollCorner &operator=(const FXScrollCorner&);
public:
  long onPaint(FXObject*,FXSelector,void*);
public:

  /// Constructor
  FXScrollCorner(FXComposite* p);

  /// Can not be enabled
  virtual void enable();

  /// Can not be disabled
  virtual void disable();
  };

}

#endif
