/* $Id: hexGui.hpp,v 1.5 2006-11-05 04:42:43 ganzhorn Exp $
 * This file is part of lfhex.
 * Copyright (C) 2006 Salem Ganzhorn <eyekode@yahoo.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _GUI_HPP_
#define _GUI_HPP_

#include <QMainWindow>
#include <QLabel>
#include <QCloseEvent>

#include "hexEditor.hpp"
#include "conversionDlg.hpp"
#include "local.h"

class QScrollBar;
class QMenu;
class QString;
class QLabel;
//class QProgressBar;

class HexGui : public QMainWindow {
  Q_OBJECT
public:
  HexGui( QWidget * parent = 0 );  
  ~HexGui();

  const QString& filename(void) const;
  // see through call to isModified in hexEditor 
  bool isModified() const {return hexEditor->isModified();}
  // public access functions
  off_t offset() const;
  Reader * reader();

signals:
  void offsetChanged(off_t offset);

public slots:
// sets selection to [start,stop)
  void setSelection(off_t start, off_t stop);
  bool browseLoadFile();
  bool browseLoadFileNew();
  bool open(const char * filename);
  void gotoOffset(off_t offset);
  void setOffsetFromStatusBar(void);

  bool save();
  bool saveAs();
  void setScrollBarRange(off_t low, off_t high);
  void setScrollBarValue(off_t pos);
  void setOffsetLabel(off_t pos);
  void closeEvent(QCloseEvent *ce);
  // had to trivially overload closeFile() == closeFile(false) for Qt slots
  int  closeFile();
  int  closeFile(bool force);
  void searchForwards();
  void searchBackwards();

protected:
  void search( bool forwards );

protected:
  enum ActionID_e {
      UndoAction,
      RedoAction,
      SaveAction,
      SaveAsAction,
      TotalActions
  };

  QLabel        *statusLabel;
  QLineEdit     *offsetLineEdit;
  //  QProgressBar  *progressBar;
  HexEditor     *hexEditor;
  ConversionDlg *conversionDlg;
  QScrollBar    *vsb;
  QAction*       actions[TotalActions];
  QMenu         *fileMenu;
  QMenu         *editMenu;
  QMenu         *viewMenu;
};

#endif
