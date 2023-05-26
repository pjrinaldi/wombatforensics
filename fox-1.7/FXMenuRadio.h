/********************************************************************************
*                                                                               *
*                         M e n u R a d i o   W i d g e t                       *
*                                                                               *
*********************************************************************************
* Copyright (C) 2002,2022 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef FXMENURADIO_H
#define FXMENURADIO_H

#ifndef FXMENUCOMMAND_H
#include "FXMenuCommand.h"
#endif

namespace FX {


/**
* The menu radio widget is used to invoke a command in the
* application from a menu.  Menu commands may reflect
* the state of the application by graying out, becoming hidden,
* or by a bullit.
* When activated, a menu radio sends a SEL_COMMAND to its target;
* the void* argument of the message contains the new state.
* A collection of menu radio widgets which belong to each other
* is supposed to be updated by a common SEL_UPDATE handler to
* properly maintain the state between them.
*/
class FXAPI FXMenuRadio : public FXMenuCommand {
  FXDECLARE(FXMenuRadio)
protected:
  FXColor radioColor;   // Color of the radio
  FXuchar check;        // State of menu
protected:
  FXMenuRadio();
private:
  FXMenuRadio(const FXMenuRadio&);
  FXMenuRadio &operator=(const FXMenuRadio&);
public:
  long onPaint(FXObject*,FXSelector,void*);
  long onButtonPress(FXObject*,FXSelector,void*);
  long onButtonRelease(FXObject*,FXSelector,void*);
  long onKeyPress(FXObject*,FXSelector,void*);
  long onKeyRelease(FXObject*,FXSelector,void*);
  long onHotKeyPress(FXObject*,FXSelector,void*);
  long onHotKeyRelease(FXObject*,FXSelector,void*);
  long onCheck(FXObject*,FXSelector,void*);
  long onUncheck(FXObject*,FXSelector,void*);
  long onUnknown(FXObject*,FXSelector,void*);
  long onCmdSetValue(FXObject*,FXSelector,void*);
  long onCmdSetIntValue(FXObject*,FXSelector,void*);
  long onCmdGetIntValue(FXObject*,FXSelector,void*);
  long onCmdAccel(FXObject*,FXSelector,void*);
public:

  /// Construct a menu radio
  FXMenuRadio(FXComposite* p,const FXString& text,FXObject* tgt=nullptr,FXSelector sel=0,FXuint opts=0);

  /// Return default width
  virtual FXint getDefaultWidth();

  /// Return default height
  virtual FXint getDefaultHeight();

  /// Set radio button state (true, false or maybe)
  void setCheck(FXuchar s=true);

  /// Get radio button state (true, false or maybe)
  FXuchar getCheck() const { return check; }

  /// Get the radio background color
  FXColor getRadioColor() const { return radioColor; }

  /// Set the radio background color
  void setRadioColor(FXColor clr);

  /// Save menu to a stream
  virtual void save(FXStream& store) const;

  /// Load menu from a stream
  virtual void load(FXStream& store);
  };

}

#endif
