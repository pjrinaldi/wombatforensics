/********************************************************************************
*                                                                               *
*                       D r a g   C o r n e r   W i d g e t                     *
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
#ifndef FXDRAGCORNER_H
#define FXDRAGCORNER_H

#ifndef FXWINDOW_H
#include "FXWindow.h"
#endif

namespace FX {


/**
* A drag corner widget may be placed in the bottom right corner
* so as to allow the window to be resized more easily.
*/
class FXAPI FXDragCorner : public FXWindow {
  FXDECLARE(FXDragCorner)
protected:
  FXColor hiliteColor;
  FXColor shadowColor;
  FXint   oldw;
  FXint   oldh;
  FXint   xoff;
  FXint   yoff;
  FXbool  ewmh;
protected:
  FXDragCorner();
private:
  FXDragCorner(const FXDragCorner&);
  FXDragCorner &operator=(const FXDragCorner&);
public:
  long onPaint(FXObject*,FXSelector,void*);
  long onLeftBtnPress(FXObject*,FXSelector,void*);
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
  long onMotion(FXObject*,FXSelector,void*);
public:

  /// Construct a drag corner
  FXDragCorner(FXComposite* p);

  /// Get default width
  virtual FXint getDefaultWidth();

  /// Get default height
  virtual FXint getDefaultHeight();

  /// Create all of the server-side resources for this window
  virtual void create();

  /// Change highlight color
  void setHiliteColor(FXColor clr);

  /// Return current highlight color
  FXColor getHiliteColor() const { return hiliteColor; }

  /// Change shadow color
  void setShadowColor(FXColor clr);

  /// Return current shadow color
  FXColor getShadowColor() const { return shadowColor; }

  /// Save drag corner to a stream
  virtual void save(FXStream& store) const;

  /// Load drag corner from a stream
  virtual void load(FXStream& store);
  };

}

#endif
