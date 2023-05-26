/********************************************************************************
*                                                                               *
*                D i r e c t o r y   S e l e c t i o n   D i a l o g            *
*                                                                               *
*********************************************************************************
* Copyright (C) 2000,2022 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef FXDIRDIALOG_H
#define FXDIRDIALOG_H

#ifndef FXDIALOGBOX_H
#include "FXDialogBox.h"
#endif

namespace FX {


class FXFileAssociations;
class FXDirSelector;


/**
* A Directory Dialog provides a way to select a directory.  In function,
* the directory selection dialog is very similar to the file dialog, except that
* the Directory Dialog displays a tree-structured view of the file system, and
* thereby makes up and down navigation through the file system significantly easier.
*/
class FXAPI FXDirDialog : public FXDialogBox {
  FXDECLARE(FXDirDialog)
protected:
  FXDirSelector *dirbox;          // Directory selection widget
protected:
  static const FXchar sectionName[];
protected:
  FXDirDialog(){}
  void initdialog();
private:
  FXDirDialog(const FXDirDialog&);
  FXDirDialog &operator=(const FXDirDialog&);
public:

  /// Construct Directory Dialog box
  FXDirDialog(FXWindow* owner,const FXString& name,FXuint opts=0,FXint x=0,FXint y=0,FXint w=400,FXint h=300);

  /// Construct free-floating Directory Dialog box
  FXDirDialog(FXApp* a,const FXString& name,FXuint opts=0,FXint x=0,FXint y=0,FXint w=400,FXint h=300);

  /// Hide this window
  virtual void hide();

  /// Change directory
  void setDirectory(const FXString& path);

  /// Return directory
  FXString getDirectory() const;

  /// Change wildcard matching pattern
  void setPattern(const FXString& ptrn);

  /// Return wildcard pattern
  FXString getPattern() const;

  /// Return wildcard matching mode
  FXuint getMatchMode() const;

  /// Change wildcard matching mode (see FXPath)
  void setMatchMode(FXuint mode);

  /// Return true if showing files as well as directories
  FXbool showFiles() const;

  /// Show or hide normal files
  void showFiles(FXbool showing);

  /// Return true if showing hidden files
  FXbool showHiddenFiles() const;

  /// Show or hide hidden files
  void showHiddenFiles(FXbool showing);

  /// Change directory list style
  void setDirBoxStyle(FXuint style);

  /// Return directory list style
  FXuint getDirBoxStyle() const;

  /// Change file associations; delete old ones if owned
  void setAssociations(FXFileAssociations* assoc,FXbool owned=false);

  /// Return file associations
  FXFileAssociations* getAssociations() const;

  /// Open directory name
  static FXString getOpenDirectory(FXWindow* owner,const FXString& caption,const FXString& path);

  /// Save to stream
  virtual void save(FXStream& store) const;

  /// Load from stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXDirDialog();
  };

}

#endif
