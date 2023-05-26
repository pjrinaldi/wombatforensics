/********************************************************************************
*                                                                               *
*                            C h o i c e   B o x                                *
*                                                                               *
*********************************************************************************
* Copyright (C) 2004,2022 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef FXCHOICEBOX_H
#define FXCHOICEBOX_H

#ifndef FXDIALOGBOX_H
#include "FXDialogBox.h"
#endif

namespace FX {


class FXList;


/**
* The Choice Box provides a dialog panel to select one item out of a list
* of choices.  The choices are provided as a list of text strings.
* When the dialog closes, the index of the selected choice is returned,
* while a -1 is returned if the dialog was canceled,
*/
class FXAPI FXChoiceBox : public FXDialogBox {
  FXDECLARE(FXChoiceBox)
protected:
  FXList *list;
protected:
  FXChoiceBox(){}
private:
  FXChoiceBox(const FXChoiceBox&);
  FXChoiceBox &operator=(const FXChoiceBox&);
  void initialize(const FXString& text,FXIcon* icon);
public:
  long onCmdClicked(FXObject*,FXSelector,void*);
  long onCmdCancel(FXObject*,FXSelector,void*);
public:
  enum{
    ID_CLICKED=FXDialogBox::ID_LAST,
    ID_LAST
    };
public:

  /// Construct choice box with given caption, icon, message text, and with choices from array of strings
  FXChoiceBox(FXWindow* owner,const FXString& caption,const FXString& text,FXIcon* icon,const FXchar** choices,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Construct choice box with given caption, icon, message text, and with choices from newline separated strings
  FXChoiceBox(FXWindow* owner,const FXString& caption,const FXString& text,FXIcon* icon,const FXString& choices,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Construct free floating choice box with given caption, icon, message text, and with choices from array of strings
  FXChoiceBox(FXApp* app,const FXString& caption,const FXString& text,FXIcon* icon,const FXchar** choices,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Construct free floating choice box with given caption, icon, message text, and with choices from newline separated strings
  FXChoiceBox(FXApp* app,const FXString& caption,const FXString& text,FXIcon* icon,const FXString& choices,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Run modal invocation of the dialog
  virtual FXuint execute(FXuint placement=PLACEMENT_CURSOR);

  /**
  * Show a modal choice dialog.
  * Prompt the user using a dialog with given caption, icon,
  * message text, and with choices from newline array of strings.
  * The return value is -1 if cancelled, or the given choice.
  */
  static FXint ask(FXWindow* owner,FXuint opts,const FXString& caption,const FXString& text,FXIcon* icon,const FXchar** choices);

  /**
  * Show a modal choice dialog.
  * Prompt the user using a dialog with given caption, icon,
  * message text, and with choices from newline separated strings.
  * The return value is -1 if cancelled, or the given choice
  */
  static FXint ask(FXWindow* owner,FXuint opts,const FXString& caption,const FXString& text,FXIcon* icon,const FXString& choices);

  /**
  * Show modal choice message, in free floating window.
  * Prompt the user using a dialog with given caption, icon,
  * message text, and with choices from newline array of strings.
  * The return value is -1 if cancelled, or the given choice
  */
  static FXint ask(FXApp* app,FXuint opts,const FXString& caption,const FXString& text,FXIcon* icon,const FXchar** choices);

  /**
  * Show modal choice message, in free floating window.
  * Prompt the user using a dialog with given caption, icon,
  * message text, and with choices from newline separated strings.
  * The return value is -1 if cancelled, or the given choice
  */
  static FXint ask(FXApp* app,FXuint opts,const FXString& caption,const FXString& text,FXIcon* icon,const FXString& choices);


  /// Save choice box to a stream
  virtual void save(FXStream& store) const;

  /// Load choice box from a stream
  virtual void load(FXStream& store);

  /// Destroy choice box
  virtual ~FXChoiceBox();
  };

}

#endif
