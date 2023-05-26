/********************************************************************************
*                                                                               *
*                  F i l e   S e l e c t i o n   W i d g e t                    *
*                                                                               *
*********************************************************************************
* Copyright (C) 1998,2022 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef FXFILESELECTOR_H
#define FXFILESELECTOR_H

#ifndef FXPACKER_H
#include "FXPacker.h"
#endif

namespace FX {


class FXFileAssociations;
class FXFileList;
class FXTextField;
class FXComboBox;
class FXDirBox;
class FXButton;
class FXMenuButton;
class FXIcon;
class FXMenuPane;
class FXCheckButton;
class FXMatrix;
class FXIconSource;
class FXHorizontalFrame;


/// File selection modes
enum {
  SELECTFILE_ANY,             /// A single file, existing or not (to save to)
  SELECTFILE_EXISTING,        /// An existing file (to load)
  SELECTFILE_MULTIPLE,        /// Multiple existing files
  SELECTFILE_MULTIPLE_ALL,    /// Multiple existing files or directories, but not '.' and '..'
  SELECTFILE_DIRECTORY        /// Existing directory, including '.' or '..'
  };


/// File selection widget
class FXAPI FXFileSelector : public FXPacker {
  FXDECLARE(FXFileSelector)
protected:
  FXFileList        *filebox;           // File list widget
  FXTextField       *filename;          // File name entry field
  FXComboBox        *filefilter;        // Combobox for pattern list
  FXMenuPane        *bookmarkmenu;      // Menu for bookmarks
  FXHorizontalFrame *navbuttons;        // Navigation buttons
  FXHorizontalFrame *fileboxframe;      // Frame around file list
  FXMatrix          *entryblock;        // Entry block
  FXCheckButton     *readonly;          // Open file as read only
  FXDirBox          *dirbox;            // Directory hierarchy list
  FXButton          *accept;            // Accept button
  FXButton          *cancel;            // Cancel button
  FXIcon            *updiricon;         // Up directory icon
  FXIcon            *listicon;          // List mode icon
  FXIcon            *detailicon;        // Detail mode icon
  FXIcon            *iconsicon;         // Icon mode icon
  FXIcon            *homeicon;          // Go home icon
  FXIcon            *workicon;          // Go home icon
  FXIcon            *shownicon;         // Files shown icon
  FXIcon            *hiddenicon;        // Files hidden icon
  FXIcon            *bookmarkicon;      // Book mark icon
  FXIcon            *bookaddicon;       // Book add icon
  FXIcon            *bookdelicon;       // Book delete icon
  FXIcon            *bookclricon;       // Book clear icon
  FXIcon            *sortingicon;       // Sorting icon
  FXIcon            *newicon;           // New directory icon
  FXIcon            *renameicon;        // Rename file icon
  FXIcon            *copyicon;          // Copy file icon
  FXIcon            *moveicon;          // Rename file icon
  FXIcon            *linkicon;          // Link file icon
  FXIcon            *deleteicon;        // Delete file icon
  FXRecentFiles      bookmarks;         // Bookmarked places
  FXuint             selectmode;        // Select mode
  FXbool             navigable;         // May navigate
protected:
  FXFileSelector(){}
  static FXint countFilenames(const FXString& string);
  static FXString decodeFilename(const FXString& string,FXint n=0);
  static FXString encodeFilename(const FXString& string);
private:
  FXFileSelector(const FXFileSelector&);
  FXFileSelector &operator=(const FXFileSelector&);
public:
  long onCmdAccept(FXObject*,FXSelector,void*);
  long onCmdFilter(FXObject*,FXSelector,void*);
  long onCmdItemDblClicked(FXObject*,FXSelector,void*);
  long onCmdItemSelected(FXObject*,FXSelector,void*);
  long onCmdItemDeselected(FXObject*,FXSelector,void*);
  long onCmdDirectoryUp(FXObject*,FXSelector,void*);
  long onUpdDirectoryUp(FXObject*,FXSelector,void*);
  long onUpdDirTree(FXObject*,FXSelector,void*);
  long onCmdDirTree(FXObject*,FXSelector,void*);
  long onCmdHome(FXObject*,FXSelector,void*);
  long onCmdWork(FXObject*,FXSelector,void*);
  long onCmdBookmark(FXObject*,FXSelector,void*);
  long onCmdUnBookmark(FXObject*,FXSelector,void*);
  long onCmdVisit(FXObject*,FXSelector,void*);
  long onCmdNew(FXObject*,FXSelector,void*);
  long onUpdNew(FXObject*,FXSelector,void*);
  long onCmdRename(FXObject*,FXSelector,void*);
  long onCmdCopy(FXObject*,FXSelector,void*);
  long onCmdMove(FXObject*,FXSelector,void*);
  long onCmdLink(FXObject*,FXSelector,void*);
  long onCmdRemove(FXObject*,FXSelector,void*);
  long onUpdSelected(FXObject*,FXSelector,void*);
  long onPopupMenu(FXObject*,FXSelector,void*);
  long onCmdImageSize(FXObject*,FXSelector,void*);
  long onUpdImageSize(FXObject*,FXSelector,void*);
  long onUpdNavigable(FXObject*,FXSelector,void*);
public:
  enum {
    ID_FILEFILTER=FXPacker::ID_LAST,
    ID_ACCEPT,
    ID_FILELIST,
    ID_DIRECTORY_UP,
    ID_DIRTREE,
    ID_MINI_SIZE,
    ID_NORMAL_SIZE,
    ID_MEDIUM_SIZE,
    ID_GIANT_SIZE,
    ID_HOME,
    ID_WORK,
    ID_BOOKMARK,
    ID_UNBOOKMARK,
    ID_BOOKMENU,
    ID_VISIT,
    ID_NEW,
    ID_RENAME,
    ID_COPY,
    ID_MOVE,
    ID_LINK,
    ID_REMOVE,
    ID_LAST
    };
public:

  /// Constructor
  FXFileSelector(FXComposite *p,FXObject* tgt=nullptr,FXSelector sel=0,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Create server-side resources
  virtual void create();

  /// Return a pointer to the "Accept" button
  FXButton *acceptButton() const { return accept; }

  /// Return a pointer to the "Cancel" button
  FXButton *cancelButton() const { return cancel; }

  /// Change file name
  void setFilename(const FXString& path);

  /// Return file name, if any
  FXString getFilename() const;

  /**
  * Return array of strings containing the selected file names, terminated
  * by an empty string.  This string array must be freed using delete [].
  * If no files were selected, a NULL is returned.
  */
  FXString* getFilenames() const;

  /// Change directory
  void setDirectory(const FXString& path);

  /// Return directory
  FXString getDirectory() const;

  /// Change file selection mode; the default is SELECTFILE_ANY
  void setSelectMode(FXuint mode);

  /// Return file selection mode
  FXuint getSelectMode() const { return selectmode; }

  /// Change file pattern
  void setPattern(const FXString& ptrn);

  /// Return file pattern
  FXString getPattern() const;

  /// Change wildcard matching mode (see FXPath)
  void setMatchMode(FXuint mode);

  /// Return wildcard matching mode
  FXuint getMatchMode() const;

  /**
  * Change the list of file patterns shown in the file dialog.
  * Each pattern comprises an optional name, followed by a pattern in
  * parentheses.  The patterns are separated by newlines.
  * For example,
  *
  *  "*\n*.cpp,*.cc\n*.hpp,*.hh,*.h"
  *
  * and
  *
  *  "All Files (*)\nC++ Sources (*.cpp,*.cc)\nC++ Headers (*.hpp,*.hh,*.h)"
  *
  * will set the same three patterns, but the former shows no pattern names.
  */
  void setPatternList(const FXString& patterns);

  /// Return list of patterns
  FXString getPatternList() const;

  /**
  * After setting the list of patterns, this call will
  * initially select pattern n as the active one.
  */
  void setCurrentPattern(FXint patno);

  /// Return current pattern number
  FXint getCurrentPattern() const;

  /// Change pattern text for pattern number
  void setPatternText(FXint patno,const FXString& text);

  /// Get pattern text for given pattern number
  FXString getPatternText(FXint patno) const;

  /// Return number of patterns
  FXint getNumPatterns() const;

  /// Allow pattern entry
  void allowPatternEntry(FXbool flag);

  /// Return true if pattern entry is allowed
  FXbool allowPatternEntry() const;

  /// Set the inter-item spacing (in pixels)
  void setItemSpace(FXint s);

  /// Return the inter-item spacing (in pixels)
  FXint getItemSpace() const;

  /// Change file list style
  void setFileBoxStyle(FXuint style);

  /// Return file list style
  FXuint getFileBoxStyle() const;

  /// Return true if showing hidden files
  FXbool showHiddenFiles() const;

  /// Show or hide hidden files
  void showHiddenFiles(FXbool flag);

  /// Return true if image preview on
  FXbool showImages() const;

  /// Show or hide preview images
  void showImages(FXbool flag);

  /// Return images preview size
  FXint getImageSize() const;

  /// Change images preview size
  void setImageSize(FXint size);

  /// Show readonly button
  void showReadOnly(FXbool flag);

  /// Return true if readonly is shown
  FXbool shownReadOnly() const;

  /// Set initial state of readonly button
  void setReadOnly(FXbool flag);

  /// Get readonly state
  FXbool getReadOnly() const;

  /// Allow or disallow navigation
  void allowNavigation(FXbool flag);

  /// Is navigation allowed?
  FXbool allowNavigation() const { return navigable; }

  /// Set draggable files
  void setDraggableFiles(FXbool flag);

  /// Are files draggable?
  FXbool getDraggableFiles() const;

  /// Set file time format
  void setTimeFormat(const FXString& fmt);

  /// Return file time format
  FXString getTimeFormat() const;

  /// Change file associations; delete old ones if owned
  void setAssociations(FXFileAssociations* assoc,FXbool owned=false);

  /// Return file associations
  FXFileAssociations* getAssociations() const;

  /// Change icon loader
  void setIconSource(FXIconSource* src);

  /// Return icon loader
  FXIconSource* getIconSource() const;

  /**
  * Given filename pattern of the form "GIF Format (*.gif)",
  * returns the pattern only, i.e. "*.gif" in this case.
  * If the parentheses are not found then returns the entire
  * input pattern.
  */
  static FXString patternFromText(const FXString& pattern);

  /**
  * Given a pattern of the form "*.gif,*.GIF", return
  * the first extension of the pattern, i.e. "gif" in this
  * example. Returns empty string if it doesn't work out.
  */
  static FXString extensionFromPattern(const FXString& pattern);

  /// Save object to a stream
  virtual void save(FXStream& store) const;

  /// Load object from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXFileSelector();
  };

}

#endif
