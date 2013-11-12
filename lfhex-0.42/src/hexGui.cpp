/* $Id: hexGui.cpp,v 1.13 2006-11-05 04:42:43 ganzhorn Exp $
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

#include <QFileDialog>
#include <qsizepolicy.h>
#include <qstatusbar.h>
#include <qmenubar.h>
#include <qapplication.h>
#include <qscrollbar.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <QDockWidget>
#include <QLabel>
#include <QCloseEvent>
#include <QPushButton>

#include "hexGui.hpp"
#include "box.hpp"
#include "expr.h"

HexGui::HexGui (QWidget * p) 
    : QMainWindow(p)
{
  QWidget* h = new QWidget(this);
  QHBoxLayout* l = new QHBoxLayout(h);

  hexEditor = new HexEditor(h);
  hexEditor->setSizePolicy( QSizePolicy( QSizePolicy::Expanding,
					 QSizePolicy::Expanding ) );
  l->addWidget(hexEditor);
  vsb = new QScrollBar(h);
  l->addWidget(vsb);
  vsb->setRange(0,0);

  setCentralWidget(h);

  connect( hexEditor,SIGNAL(rangeChanged(off_t,off_t)),
	   this,SLOT(setScrollBarRange(off_t,off_t)) );
  connect( vsb, SIGNAL(valueChanged(int)),
	   hexEditor,SLOT(setTopLeftToPercent(int)) );
  connect( hexEditor, SIGNAL(topLeftChanged(off_t)),
	   this, SLOT(setScrollBarValue(off_t)) );
  connect( hexEditor, SIGNAL(offsetChanged(off_t)),
	   this, SLOT(setOffsetLabel(off_t)) );

  // setup dock
  QDockWidget* dock = new QDockWidget("Conversions Assistant",this);
  vbox* vb = new vbox(dock);
  conversionDlg = new ConversionDlg(vb);
  connect(conversionDlg,SIGNAL(nextPressed()),this,SLOT(searchForwards()));
  connect(conversionDlg,SIGNAL(prevPressed()),this,SLOT(searchBackwards()));

  dock->setWidget(vb);
  dock->hide();
  connect(hexEditor,SIGNAL(selectionChanged(const QString &)),
	  conversionDlg, SLOT(valueChanged(const QString&)) );
  addDockWidget( Qt::RightDockWidgetArea, dock );
  dock->setFloating(true);

  // setup menu:
  fileMenu = new QMenu("&File",this);
  fileMenu->addAction("&Open...",this,SLOT(browseLoadFile()));
  fileMenu->addAction("&Open in new editor...",this,
		      SLOT(browseLoadFileNew()));
  fileMenu->addSeparator();
  actions[SaveAction] = fileMenu->addAction("&Save",this,SLOT(save()));
  actions[SaveAsAction] = fileMenu->addAction("Save &As...",this,
					      SLOT(saveAs()));
  // only include close functions in menu if this is a toplevel window
  if( isTopLevel() ) {
    fileMenu->addSeparator();
    if( isTopLevel() )
      fileMenu->addAction("Close Editor",this,SLOT(close()));
    else
      fileMenu->addAction("Close File",this,SLOT(closeFile()));
    fileMenu->addAction("&Exit Application",qApp, SLOT(quit()));
  }
  menuBar()->addMenu( fileMenu );

  // edit pulldown
  editMenu = new QMenu( "&Edit", this );
  actions[UndoAction] = editMenu->addAction("&Undo",hexEditor,SLOT(undo()));
  actions[RedoAction] = editMenu->addAction("&Redo",hexEditor,SLOT(redo()));
  menuBar()->addMenu( editMenu );

  // view menu
  viewMenu = new QMenu("&View",this);
  QAction* dockAction =  dock->toggleViewAction();
  viewMenu->addAction( dockAction );

  // menu for selecting editing base (octal/binary/hex)
  QMenu * baseMenu = new QMenu("&Editing base", this);
  baseMenu->addAction("&Hex",hexEditor,SLOT(setBaseHex()));
  baseMenu->addAction("&ASCII",hexEditor,SLOT(setBaseASCII()));
  baseMenu->addAction("&Binary",hexEditor,SLOT(setBaseBinary()));
  baseMenu->addAction("&Octal",hexEditor,SLOT(setBaseOctal()));
  viewMenu->addMenu(baseMenu);
  // menu for selecting bytes per column
  QMenu * nbytesMenu = new QMenu("&Bytes per column",this);
  nbytesMenu->addAction("&1",hexEditor,SLOT(set1BPC()));
  nbytesMenu->addAction("&2",hexEditor,SLOT(set2BPC()));
  nbytesMenu->addAction("&4",hexEditor,SLOT(set4BPC()));
  nbytesMenu->addAction("&8",hexEditor,SLOT(set8BPC()));
  viewMenu->addMenu(nbytesMenu);
  menuBar()->addMenu(viewMenu);

  resize(400,196);
  statusBar()->showMessage("Ready",2000);
  statusBar()->addWidget(new QLabel("Cursor Offset:",statusBar()));
  offsetLineEdit = new QLineEdit(statusBar());
//  offsetLineEdit->setValidator( new HexValidator(offsetLineEdit) );
  connect(offsetLineEdit,SIGNAL(returnPressed()),
	  SLOT(setOffsetFromStatusBar()));
  statusBar()->addWidget(offsetLineEdit);
  //  progressBar = new QProgressBar(statusBar());
  //  statusBar()->addWidget(progressBar,1,FALSE);
  statusLabel = new QLabel("",statusBar());
  statusBar()->addWidget(statusLabel,1);
  statusBar()->setSizeGripEnabled( !parent() );
  setWindowTitle(PROGRAM_STRING);

  // deactivate save,saveas undo and redo
  actions[SaveAction]->setEnabled(false);
  actions[SaveAsAction]->setEnabled(false);
  actions[UndoAction]->setEnabled(false);
  actions[RedoAction]->setEnabled(false);
}

HexGui::~HexGui()
{
}

bool HexGui::open(const char * filename) 
{
  if (hexEditor->open(filename)) {
    if( isTopLevel() )
      setWindowTitle(filename);
    else
      statusLabel->setText(filename);
    actions[SaveAction]->setEnabled(true);
    actions[SaveAsAction]->setEnabled(true);
    actions[UndoAction]->setEnabled(true);
    actions[RedoAction]->setEnabled(true);
    return true;
  } else {
    return false;
  }
}

void HexGui::gotoOffset(off_t offset)
{
  hexEditor->setOffset(offset);
  hexEditor->seeCursor();
  emit offsetChanged(offset);
}

bool HexGui::browseLoadFile()
{
  QString filename = QFileDialog::getOpenFileName();
  if( filename.isNull() )
    return false;
  return open( C_STR(filename) );
}
bool HexGui::browseLoadFileNew()
{
  QString filename = QFileDialog::getOpenFileName();
  if( filename.isNull() )
    return false;
  HexGui * hg = new HexGui();
  bool retval = hg->open( C_STR(filename) );
  hg->show();
  return retval;
}

bool HexGui::save()
{
  statusBar()->showMessage("saveing " + hexEditor->filename()) ;
  bool retval = hexEditor->save();
  statusBar()->showMessage("done.",2000);
  return retval;
}

bool HexGui::saveAs()
{
  QString filename = QFileDialog::getSaveFileName();
  if( filename.isNull() ) 
    return false;
  statusBar()->showMessage("saveing to \"" + filename + "\"...");
  bool retval = hexEditor->save(filename);
  statusBar()->showMessage("done.",2000);
  setWindowTitle(filename);
  return retval;
}

int HexGui::closeFile()
{
  return closeFile( false );
}
int HexGui::closeFile( bool force )
{
  return hexEditor->closeFile(force);
}

void HexGui::closeEvent( QCloseEvent * ce )
{
  if( hexEditor->closeFile() == QMessageBox::Cancel ) 
    ce->ignore();
  else
    ce->accept();
}

void HexGui::setScrollBarRange(off_t low, off_t high)
{
   (void)low;(void)high;
   // range must be contained in the space of an integer, just do 100
   // increments
   vsb->setRange(0,100);
}
void HexGui::setScrollBarValue(off_t pos)
{
  // pos is the topLeft pos, set the scrollbar to the
  // location of the last byte on the page
  // Note: offsetToPercent now rounds up, so we don't
  // have to worry about if this is the topLeft or bottom right
  vsb->setValue(hexEditor->offsetToPercent(pos));
}
void HexGui::setOffsetLabel(off_t pos)
{
  QString label;
  char    buffer[64];
#if _LARGEFILE_SOURCE
  sprintf(buffer,"0x%lx",pos);
#else
  sprintf(buffer,"0x%x",pos);
#endif
  label = buffer;
  offsetLineEdit->setText(label);
}

const QString& HexGui::filename(void) const
{
  static QString value = "";
  if(!hexEditor)
    return (value="");
  return (value=hexEditor->filename());
}

void HexGui::setOffsetFromStatusBar()
{
  // remove leading "0x" if necessary
  off_t o;
  if( !expr_eval( C_STR(offsetLineEdit->text()),o ) || o < 0 ) {
     QApplication::beep();
  } else {
     gotoOffset(o);
  }
  return;
  bool  ok;
  off_t offset;
  QString value = offsetLineEdit->text();
  if( value.length() > 2 && value[1] == 'x' ) {
     value = value.mid(2);
     offset = value.toInt(&ok,16);
  } else {
     offset = value.toInt(&ok,10);
     if( !ok ) {
	offset = value.toInt(&ok,16);
     }
  }
  if( !ok ) {
     cerr << "Error converting offset: " << C_STR(offsetLineEdit->text())
	  << endl;
  } else {
     gotoOffset(offset);
  }
}

off_t HexGui::offset() const
{
  return hexEditor->offset();
}

Reader * HexGui::reader()
{
  return hexEditor->reader();
}

void HexGui::setSelection(off_t start,off_t stop)
{
  hexEditor->setSelection(HexEditor::SelectionStart,start);
  hexEditor->setSelection(HexEditor::SelectionEnd,stop);
}

void HexGui::search( bool forward )
{
  QString hex = conversionDlg->hexValue();
  if( !hex.isEmpty() ) {
      hexEditor->search( hex, forward );
  }
}

void HexGui::searchForwards()
{
  search(true);
}

void HexGui::searchBackwards()
{
  search(false);
}
