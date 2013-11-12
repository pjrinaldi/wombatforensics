/* $Id: compareDlg.cpp,v 1.9 2008-09-11 01:49:00 salem Exp $
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

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <qlayout.h>
#include <qpushbutton.h>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <qcombobox.h>
#include <qtabwidget.h>
#include <QBitmap>
#include <QVBoxLayout>
#include <QMenuBar>

#include "compareDlg.hpp"
#include "offsetConstraint.hpp"
#include "hexGui.hpp"
#include "reader.hpp"
#include "box.hpp"

// icon images:
#include "img/prev.xbm"
#include "img/next.xbm"
#include "img/last.xbm"
#include "img/first.xbm"
#include "img/exit.xbm"

//extern QApplication * qApp;

CompareDlg::CompareDlg(const char *file0,
		       const char *file1,
		       QWidget*parent)
    : QMainWindow(parent)
{
  tabLayout = new QTabWidget(this);
  setCentralWidget(tabLayout);

  QWidget *options = new QWidget(tabLayout);
  QVBoxLayout *optionLayout = new QVBoxLayout(options);

  optionLayout->setMargin(3);
  optionLayout->setSpacing(2);
  
  // setup alignment entries:
  offsetConstraint = new OffsetConstraint(options);
  offsetConstraint->setTitle( "Constrain offset to X*n+Y" );
  optionLayout->addWidget(offsetConstraint);
  // setup cursor offsets
  diffOffsets = new QCheckBox("Allow different file offsets",options);
  optionLayout->addWidget(diffOffsets);
  // add a dummy strechable widget for alignment.
  optionLayout->addStretch(1);
  connect(diffOffsets,SIGNAL(stateChanged(int)),
	  this,SLOT(setAllowDiffOffsets(int)));
  diffOffsets->setChecked(false);
  setAllowDiffOffsets(false);

  // setup menu
  QMenu* fileMenu = menuBar()->addMenu("File");
  fileMenu->addAction( QBitmap::fromData(QSize(exit_width,exit_height),
					 exit_bits),"Exit",qApp,SLOT(quit()));
			

  // setup toolbar 
  QToolBar *tools = new QToolBar(this);

  tools->addAction(QBitmap::fromData(QSize(first_width,first_height),first_bits),
		  "First Difference",this,SLOT(first()));
  tools->addAction(QBitmap::fromData(QSize(prev_width,prev_height),prev_bits),
		  "Previous Difference", this,SLOT(prev()));
  tools->addAction(QBitmap::fromData(QSize(next_width,next_height),next_bits),
		  "Next Difference", this,SLOT(next()));
  tools->addAction(QBitmap::fromData(QSize(last_width,last_height),last_bits),
		  "Last Difference", this,SLOT(last()));

  // setup compare section
  QWidget *view = new QWidget(tabLayout);
  QVBoxLayout *layout = new QVBoxLayout(view);

  hexGui[0] = new HexGui(view);
  hexGui[1] = new HexGui(view);
  reader[0] = hexGui[0]->reader();
  reader[1] = hexGui[1]->reader();
  
  if( file0 )
    hexGui[0]->open(file0);
  if( file1 )
    hexGui[1]->open(file1);

  layout->addWidget(hexGui[0]);
  layout->addWidget(hexGui[1]);
  hexGui[0]->setWindowFlags(Qt::SubWindow);
  hexGui[1]->setWindowFlags(Qt::SubWindow);


  tabLayout->addTab(view,"&Compare");
  tabLayout->addTab(options,"&Options");
  addToolBar(tools);
  // request a size large enough for both hexEditors
  resize(500,500);
  setWindowTitle("Compare dialog");
}

bool CompareDlg::enabled()
{
  if( reader[0]->is_open() && reader[1]->is_open() ) {
    // check to see if modified!!!
    if( hexGui[0]->isModified() || hexGui[1]->isModified() ) {
      QMessageBox::critical(this,PROGRAM_STRING,
		  "Error, cannot use comparison"
		  " functions on modified buffers.");
      return false;
    }
    return true;
  }

  return false;
}
// public slots
void CompareDlg::setAllowDiffOffsets(int state)
{
  diffOffsets->setChecked(state);
}

bool CompareDlg::first()
{
  if( !enabled() )
    return false;
  
  // see Compare::last() for description of concept
  off_t offset = offsetConstraint->next(0);
  hexGui[0]->gotoOffset(offset);
  hexGui[1]->gotoOffset(offset);
  if( prev() )
    return true;
  hexGui[0]->gotoOffset(0);
  hexGui[1]->gotoOffset(0);
  return next();
}

bool CompareDlg::last()
{
  if( !enabled() ) 
    return false;

  // look at lastidx 's previous's next range
  // prev(x) is garranteed to not contain x, and next(prev(x)) will contain x
  off_t min_size_decr = min( reader[0]->size()-1,
			     reader[1]->size()-1 );
  off_t offset = offsetConstraint->prev( min_size_decr );
  hexGui[0]->gotoOffset(offset);
  hexGui[1]->gotoOffset(offset);
  if( next() )
    return true;
  hexGui[0]->gotoOffset( min_size_decr );
  hexGui[1]->gotoOffset( min_size_decr );
  return prev();
}

bool CompareDlg::prev() 
{
  if( !enabled() )
    return false;;
  // find previous diff
  off_t offset[2];
  offset[0] = hexGui[0]->offset();
  offset[1] = (diffOffsets->isChecked()) ? hexGui[1]->offset() : offset[0];
  
  if( offsetConstraint->getStride() > 1 ) {
    off_t stop[2];
    off_t pos[2];
    for( pos[0] = offset[0] = offsetConstraint->prev(offset[0]), 
	   pos[1] = offset[1] = offsetConstraint->prev(offset[1]);
	 offset[0] >= 0 && offset[1] >= 0;
	 pos[0] = offset[0] = offsetConstraint->prev(offset[0]),
	   pos[1] = offset[1] = offsetConstraint->prev(offset[1])) {
      
      // calculate stop
      stop[0] = offset[0]+offsetConstraint->getStride();
      stop[1] = offset[1]+offsetConstraint->getStride();
      
      // ignore lookups on less than a full stride length
      // this would only happen if the file size is less than the stride
      if( stop[0] > reader[0]->size() || stop[1] > reader[1]->size() )
	return false;

      while( pos[0] < stop[0] ) {
	if( (*reader[0])[pos[0]++] != (*reader[1])[pos[1]++] ) {
	  // we have a difference
	  hexGui[0]->setSelection(offset[0],stop[0]);
	  hexGui[1]->setSelection(offset[1],stop[1]);
	  hexGui[0]->gotoOffset(offset[0]);
	  hexGui[1]->gotoOffset(offset[1]);
	  return true;
	}
      }
    }
  } else {
    // just go one char at a time

    // make sure we are not past the eof of the second buffer
    if( offset[1] >= reader[1]->size() )
      return false;
    if( offset[0] <= 0 || offset[1] <= 0 )
      return false;
    do {
      offset[0]--;
      offset[1]--;
      
      if( (*reader[0])[offset[0]] != (*reader[1])[offset[1]] ) {
	hexGui[0]->gotoOffset(offset[0]);
	hexGui[1]->gotoOffset(offset[1]);
	hexGui[0]->setSelection(offset[0],offset[0]+1);
	hexGui[1]->setSelection(offset[1],offset[1]+1);
	return true;
      }
    } while( offset[0] > 0 && offset[1] > 0 );
  }
  return false;
}

bool CompareDlg::next() 
{
  // find next diff
  if( !enabled() )
    return false;

  off_t offset[2];
  offset[0] = hexGui[0]->offset();
  offset[1] = (diffOffsets->isChecked()) ? hexGui[1]->offset() : offset[0];
  
  off_t size[2];
  size[0] = reader[0]->size();
  size[1] = reader[1]->size();
  
  if( offsetConstraint->getStride() > 1 ) {
    off_t stop[2];
    off_t pos[2];
    for( pos[0] = offset[0] = offsetConstraint->next(offset[0]),
	   pos[1] = offset[1] = offsetConstraint->next(offset[1]);
	 offset[0] < size[0] && offset[1] < size[1];
	 pos[0] = offset[0] = offsetConstraint->next( offset[0] ),
	   pos[1] = offset[1] = offsetConstraint->next( offset[1] )) {
      // compare from offset to offset+stride()
      stop[0] = offset[0]+offsetConstraint->getStride();
      stop[1] = offset[1]+offsetConstraint->getStride();
      // if this will go past the bounds of the files, then the difference
      // does not matter, ignore it.
      if( stop[0] > size[0] || stop[1] > size[1] ) {
	return false;
      }
      
      while( pos[0] < stop[0] ) {
	if( (*reader[0])[pos[0]++] != (*reader[1])[pos[1]++] ) {
	  // we have a difference
	  hexGui[0]->setSelection(offset[0],stop[0]);
	  hexGui[1]->setSelection(offset[1],stop[1]);
	  hexGui[0]->gotoOffset(offset[0]);
	  hexGui[1]->gotoOffset(offset[1]);
	  return true; // bail out of fn
	}
      }
    }
  } else {
    // just go one at a time
    if( offset[0] >= size[0] || offset[1] >= size[1] ) 
      return false;
    if( offset[0] < 0 || offset[1] < 0 ) 
      return false;
    offset[0]++;
    offset[1]++;
    while( offset[0] < size[0] && offset[1] < size[1] ) {
      // don't compare the one we are on
      if( (*reader[0])[offset[0]] != (*reader[1])[offset[1]] ) {
	hexGui[0]->gotoOffset(offset[0]);
	hexGui[1]->gotoOffset(offset[1]);
	hexGui[0]->setSelection(offset[0],offset[0]+1);
	hexGui[1]->setSelection(offset[1],offset[1]+1);
	return true;
      }
      offset[0]++;
      offset[1]++;
    } 
  }
  return false;
}
