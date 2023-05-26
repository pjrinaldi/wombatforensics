/********************************************************************************
*                                                                               *
*                      P r o g r e s s   D i a l o g   B o x                    *
*                                                                               *
*********************************************************************************
* Copyright (C) 2001,2022 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef FXPROGRESSDIALOG_H
#define FXPROGRESSDIALOG_H

#ifndef FXDIALOGBOX_H
#include "FXDialogBox.h"
#endif

namespace FX {


/// Progress dialog options
enum {
  PROGRESSDIALOG_NOCANCEL = 0,                        /// Default is no cancel button
  PROGRESSDIALOG_CANCEL   = 0x02000000,               /// Enable the cancel button
  PROGRESSDIALOG_NORMAL   = (DECOR_TITLE|DECOR_BORDER)/// Normally with title and border
  };


class FXHorizontalSeparator;
class FXProgressBar;
class FXButton;
class FXLabel;

/**
* A Progress Dialog is a simple dialog which is used to
* keep a user informed of the progress of a lengthy operation
* in a program and that the program is in fact still working.
* The PROGRESSDIALOG_CANCEL option enables the display of the
* cancel button in the Progress Dialog.
*/
class FXAPI FXProgressDialog : public FXDialogBox {
  FXDECLARE(FXProgressDialog)
protected:
  FXProgressBar         *progress;    // Progress bar
  FXLabel               *message;     // Message
  FXHorizontalSeparator *separator;   // Separator
  FXButton              *cancel;      // Cancel button
  FXbool                 cancelled;   // User hit cancel
protected:
  FXProgressDialog();
private:
  FXProgressDialog(const FXProgressDialog&);
  FXProgressDialog &operator=(const FXProgressDialog&);
public:
  long onCmdSetValue(FXObject*,FXSelector,void*);
  long onCmdSetIntValue(FXObject*,FXSelector,void*);
  long onCmdGetIntValue(FXObject*,FXSelector,void*);
  long onCmdSetLongValue(FXObject*,FXSelector,void*);
  long onCmdGetLongValue(FXObject*,FXSelector,void*);
  long onCmdSetIntRange(FXObject*,FXSelector,void*);
  long onCmdGetIntRange(FXObject*,FXSelector,void*);
  long onCmdSetStringValue(FXObject*,FXSelector,void*);
  long onCmdGetStringValue(FXObject*,FXSelector,void*);
  long onCmdCancel(FXObject*,FXSelector,void*);
public:

  /// Construct input dialog box with given caption, icon, and prompt text
  FXProgressDialog(FXWindow* owner,const FXString& caption,const FXString& label,FXuint opts=PROGRESSDIALOG_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Change the progress message
  void setMessage(const FXString& msg);

  /// Get progress message
  FXString getMessage() const;

  /// Change style of the progress bar widget
  void setBarStyle(FXuint style);

  /// Get style of the progress bar widget
  FXuint getBarStyle() const;

  /// Change the amount of progress
  void setProgress(FXuint value);

  /// Get current progress
  FXuint getProgress() const;

  /// Set total amount of progress
  void setTotal(FXuint value);

  /// Return total amount of progrss
  FXuint getTotal() const;

  /// Increment progress by given amount
  void increment(FXuint value);

  /// Has operation been cancelled?
  FXbool isCancelled() const;

  /// Change cancelled flag
  void setCancelled(FXbool flg);

  /// Destroy
  virtual ~FXProgressDialog();
  };

}

#endif
