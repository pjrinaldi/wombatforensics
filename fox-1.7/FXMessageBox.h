/********************************************************************************
*                                                                               *
*                         M e s s a g e   B o x e s                             *
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
#ifndef FXMESSAGEBOX_H
#define FXMESSAGEBOX_H

#ifndef FXDIALOGBOX_H
#include "FXDialogBox.h"
#endif

namespace FX {


// Message box buttons
enum {
  MBOX_OK                         = 0x10000000, /// Message box has a only an OK button
  MBOX_OK_CANCEL                  = 0x20000000, /// Message box has OK and CANCEL buttons
  MBOX_YES_NO                     = 0x30000000, /// Message box has YES and NO buttons
  MBOX_YES_NO_CANCEL              = 0x40000000, /// Message box has YES, NO, and CANCEL buttons
  MBOX_QUIT_CANCEL                = 0x50000000, /// Message box has QUIT and CANCEL buttons
  MBOX_QUIT_SAVE_CANCEL           = 0x60000000, /// Message box has QUIT, SAVE, and CANCEL buttons
  MBOX_SKIP_SKIPALL_CANCEL        = 0x70000000, /// Message box has SKIP, SKIP ALL, and CANCEL buttons
  MBOX_SAVE_CANCEL_DONTSAVE       = 0x80000000, /// Message box has DON'T SAVE,CANCEL and SAVE buttons
  MBOX_YES_YESALL_NO_NOALL_CANCEL = 0xA0000000  /// Message box has YES, YES ALL, NO, NO ALL, and CANCEL buttons
  };


// Return values
enum {
  MBOX_CLICKED_YES      = 1,            /// The YES button was clicked
  MBOX_CLICKED_NO       = 2,            /// The NO button was clicked
  MBOX_CLICKED_OK       = 3,            /// The OK button was clicked
  MBOX_CLICKED_CANCEL   = 4,            /// The CANCEL button was clicked
  MBOX_CLICKED_QUIT     = 5,            /// The QUIT button was clicked
  MBOX_CLICKED_SAVE     = 6,            /// The SAVE button was clicked
  MBOX_CLICKED_SKIP     = 7,            /// The SKIP button was clicked
  MBOX_CLICKED_SKIPALL  = 8,            /// The SKIP ALL button was clicked
  MBOX_CLICKED_YESALL   = 9,            /// The YES ALL button was clicked
  MBOX_CLICKED_NOALL    = 10            /// The NO ALL button was clicked
  };


/**
* A Message Box is a convenience class which provides a dialog for
* very simple common yes/no type interactions with the user.
* The message box has an optional icon, a title string, and the question
* which is presented to the user.  It also has up to three buttons which
* furnish standard responses to the question.
* Message boxes are usually ran modally: the question must be answered
* before the program may continue.
*/
class FXAPI FXMessageBox : public FXDialogBox {
  FXDECLARE(FXMessageBox)
protected:
  FXMessageBox(){}
private:
  FXMessageBox(const FXMessageBox&);
  FXMessageBox &operator=(const FXMessageBox&);
  void initialize(const FXString& text,FXIcon* ic,FXuint whichbuttons);
public:
  long onCmdClicked(FXObject*,FXSelector,void*);
  long onCmdCancel(FXObject*,FXSelector,void*);
public:
  enum{
    ID_CLICKED_YES=FXDialogBox::ID_LAST,
    ID_CLICKED_NO,
    ID_CLICKED_OK,
    ID_CLICKED_CANCEL,
    ID_CLICKED_QUIT,
    ID_CLICKED_SAVE,
    ID_CLICKED_SKIP,
    ID_CLICKED_SKIPALL,
    ID_CLICKED_YESALL,
    ID_CLICKED_NOALL,
    ID_LAST
    };
public:

  /// Construct message box with given caption, icon, and message text
  FXMessageBox(FXWindow* owner,const FXString& caption,const FXString& text,FXIcon* ic=nullptr,FXuint opts=0,FXint x=0,FXint y=0);

  /// Construct free floating message box with given caption, icon, and message text
  FXMessageBox(FXApp* app,const FXString& caption,const FXString& text,FXIcon* ic=nullptr,FXuint opts=0,FXint x=0,FXint y=0);

  /**
  * Show a modal error message.
  * The text message may contain printf-tyle formatting commands.
  */
  static FXuint error(FXWindow* owner,FXuint opts,const char* caption,const char* message,...) FX_PRINTF(4,5) ;

  /**
  * Show modal error message, in free floating window.
  */
  static FXuint error(FXApp* app,FXuint opts,const char* caption,const char* message,...) FX_PRINTF(4,5) ;

  /**
  * Show a modal warning message
  * The text message may contain printf-tyle formatting commands.
  */
  static FXuint warning(FXWindow* owner,FXuint opts,const char* caption,const char* message,...) FX_PRINTF(4,5) ;

  /**
  * Show modal warning message, in free floating window.
  */
  static FXuint warning(FXApp* app,FXuint opts,const char* caption,const char* message,...) FX_PRINTF(4,5) ;

  /**
  * Show a modal question dialog
  * The text message may contain printf-tyle formatting commands.
  */
  static FXuint question(FXWindow* owner,FXuint opts,const char* caption,const char* message,...) FX_PRINTF(4,5) ;

  /**
  * Show modal question message, in free floating window.
  */
  static FXuint question(FXApp* app,FXuint opts,const char* caption,const char* message,...) FX_PRINTF(4,5) ;

  /**
  * Show a modal information dialog
  * The text message may contain printf-tyle formatting commands.
  */
  static FXuint information(FXWindow* owner,FXuint opts,const char* caption,const char* message,...) FX_PRINTF(4,5) ;

  /**
  * Show modal information message, in free floating window.
  */
  static FXuint information(FXApp* app,FXuint opts,const char* caption,const char* message,...) FX_PRINTF(4,5) ;

  };

}

#endif
