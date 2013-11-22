/* $Id: hexEditor.cpp,v 1.14 2006-11-05 04:42:43 ganzhorn Exp $
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

#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <algorithm>
#include <iostream>

#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>
#include <QPainter>
#include <QPixmap>
#include <QFocusEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QMouseEvent>

#include "hexEditor.hpp"
#include "translate.hpp"
#include "local.h"

extern int errno;

HexEditor::HexEditor( QWidget * parent )
    : QWidget(parent)
{
  _cols   = 5;
  _rows   = 10;
  _charsPerByte   = 2;
  _base           = 16;
  _topLeft        = 0;
  _topMargin = _wordSpacing    = 1;
  _bytesPerWord   = 2;
  _lastValidWord  = -1;
  _selection[SelectionStart] = _selection[SelectionEnd] = -1;

  setFocusPolicy(Qt::StrongFocus);
  // the first setBytesPerWord should not come before the first setFont()
  QFont font("fixed");
  font.setStyleHint(QFont::TypeWriter);
  font.setFixedPitch(1);
  setFont( font );
  //setBackgroundRole( QPalette::HighlightedText );
  //setStyleSheet("background-color: white;");
}

HexEditor::~HexEditor() 
{
  _reader.close();
}

bool HexEditor::isModified() const
{
  return ( _reader.is_open() && _delta.numEdits() );
}

bool HexEditor::save( QString filename )
{
  if( !isModified() ) {
    QString errMsg = "Error, file is not open or has not been modified. "
      "Aborting save.";
    QMessageBox::warning(this,PROGRAM_STRING,errMsg);
    return false;
  }

  if( filename != "" ) {
    // copy open reader file to filename
    QString oldFilename = _reader.filename();

    if( oldFilename == "" )
      return false;

    _reader.close();
    FILE* fin = fopen(C_STR(oldFilename),"r");
    if( !fin ) {
      QString errMsg = "Error opening \"" + oldFilename + "\" for reading: " +
	strerror(errno);
      QMessageBox::warning(this,PROGRAM_STRING,errMsg);
      return false;
    }
    FILE* fout = fopen(C_STR(filename),"w");
    if( !fout ) {
      QString errMsg = "Error opening \"" + filename+ "\" for writing: " +
	strerror(errno);
      QMessageBox::warning(this,PROGRAM_STRING,errMsg);
      fclose(fin);
      return false;
    }
    int length;
    uchar bytes[4096];
    while( (length = fread(bytes,1,4096,fin)) ) {
      fwrite( bytes, 1, length, fout );
    }
    fclose(fin);
    fclose(fout);
  } else {
    filename = _reader.filename();
    _reader.close();
  }

  if( filename == "" )
    return false;

  if( !_delta.numEdits() )
    return false;

  if( !writeDeltas(C_STR(filename),_delta) ) {
    _reader.open(C_STR(filename));
    return false;
  }
  _delta.clear();
  _reader.open(C_STR(filename));
  setTopLeft(_topLeft);
  return true;
}

QString HexEditor::filename() const
{
  return _reader.filename();
}

bool HexEditor::open( const QString & filename )
{
  if( closeFile() == QMessageBox::Cancel )
    return false;

  if(!_reader.open(C_STR(filename))) {
    QMessageBox::critical( this, "HexEdit", 
			   "Error loading \"" + filename + "\"\n" +
			  _reader.lastError(),
			   QMessageBox::Ok,0);
    return false;
  }
  _delta.clear();           // reset modification tree

  // set the new range for the scrollbar
  _cursor.setRange(0,_reader.size());
  _cursor.setCharsPerByte(_charsPerByte);
  setSelection(SelectionStart,-1);
  setSelection(SelectionEnd,-1);
  emit rangeChanged(0,_reader.size()/bytesPerLine());
  calculateFontMetrics();     // update labels
  setTopLeft(0);              // reset the GUI
  return true;
}

int HexEditor::closeFile(bool force) 
{  
  int retval = QMessageBox::No;
  if( _delta.numEdits() ) {
    QString message = 
      "Save changes to \"" + QString(_reader.filename()) + "\"?";
    if( force ) {
      retval = QMessageBox::warning( this, 
				     PROGRAM_STRING,
				     message,
				     QMessageBox::Yes,
				     QMessageBox::No );
    } else {
      retval = QMessageBox::warning( this, 
				     PROGRAM_STRING,
				     message,
				     QMessageBox::Yes,
				     QMessageBox::No,
				     QMessageBox::Cancel);
    }
  }
  if( retval == QMessageBox::Yes ) 
    save();
  return retval;
}
  
void HexEditor::setBytesPerWord( int nbytes )
{
  _bytesPerWord = nbytes;
  calculateFontMetrics();
  QResizeEvent *re = new QResizeEvent(QSize(size()),QSize(size()));
  resizeEvent(re);
  delete re;
}
int HexEditor::fontHeight() const
{
    return _fontHeight;
}
int HexEditor::lineSpacing() const
{
  return _lineSpacing;
}
int HexEditor::fontMaxWidth() const
{
  return _fontMaxWidth;
}
void HexEditor::calculateFontMetrics()
{
  _lineSpacing = fontMetrics().lineSpacing();
  _fontMaxWidth = fontMetrics().maxWidth();
  _wordWidth   = _fontMaxWidth*charsPerWord();
  _fontHeight  = fontMetrics().height();
  // see how many bytes are needed to show the size of this file
  // log base 16 -> log16(x) = log(x)/log(16)
  if( _reader.size() ) {
     double width = log(static_cast<float>(_reader.size()))/log(16.0)+2;
     _offsetLabelBytes = static_cast<int>( width );
  } else {
     _offsetLabelBytes = 0;
  }
  _leftMargin = _topMargin + _fontMaxWidth*(_offsetLabelBytes + 2);
  // make sure bboxes are updated with new offset subdivision
  QResizeEvent *re = new QResizeEvent(QSize(size()),QSize(size()));
  resizeEvent(re);
  delete re;
}
void HexEditor::setFont(const QFont& font )
{ 
  if( !font.fixedPitch() ) {
    cerr << "setFont() failed, font was not fixedPitch()." << endl;
    return;
  }
  QWidget::setFont(font);
  calculateFontMetrics();
}
// set the top left editor to offset in reader
void HexEditor::setTopLeft( off_t offset )
{
  static bool inTopLeft;
  if( inTopLeft ) {
     // don't nest
     return;
  }
  inTopLeft = true;
  try {
     if( offset < 0 ) {
	_topLeft = 0;
     } else if( offset > _reader.size() ) {
	_topLeft = _reader.size();
     } else {
	_topLeft = offset;
     }
     // only let _topLeft be an integer multiple of the line length (round down)
     _topLeft = (_topLeft/bytesPerLine()) * bytesPerLine();
     // update the labels
     //  setOffsetLabels(_topLeft);
     
     const Delta * delta;
     
     _reader.seek(_topLeft);
     _reader.read(_data,bytesPerPage());
     
     // update data from delta map
     for( offset = _delta.lower_bound(_topLeft); 
	  (offset != -1) && (offset < _topLeft + bytesPerPage());
	  offset = _delta.lower_bound(offset) ) {
	delta = _delta.search(offset);
	_data[offset++-_topLeft] = delta->newData()[0];
     }
     
     repaint();
     emit topLeftChanged(_topLeft);
  } catch( const exception &e ) {
     inTopLeft = false;
     throw e;
  }
  inTopLeft = false;
}

//void HexEditor::setOffsetLabels( off_t topLeft )
//{
  // need to impliment manually printing labels
//}

int HexEditor::topMargin() const
{
  return _topMargin;
}
int HexEditor::leftMargin() const
{
  return _leftMargin;
}
//
// access fn's for offset manip
//
int HexEditor::bytesPerPage() const
{
  return _rows*_cols*bytesPerWord();
}
int HexEditor::bytesPerWord() const
{
  return _bytesPerWord;
}
int HexEditor::bytesPerLine() const
{
  return bytesPerWord()*wordsPerLine();
}
int HexEditor::wordsPerLine() const
{
  return _cols;
}
int HexEditor::linesPerPage() const
{
  return _rows;
}
int HexEditor::wordsPerPage() const
{
  return _rows*_cols;
}
int HexEditor::charsPerByte() const
{
  return _charsPerByte;
}
int HexEditor::charsPerWord() const
{
  return _charsPerByte*bytesPerWord();
}
int HexEditor::charsPerLine() const
{
  return _charsPerByte*(bytesPerLine());
}
// translate local byte offsets to global byte offsets
off_t HexEditor::globalOffset( off_t local ) const
{
  return local+_topLeft;
}
// translate global byte offsets to viewport byte offsets
off_t HexEditor::localOffset( off_t global ) const
{
  return global-_topLeft;
}

int HexEditor::offsetToPercent(
   off_t offset
   )
{
   // round up
   return _reader.size() ? (int)ceil(100.0*offset/_reader.size()) : 0;
}

// public slots:
bool HexEditor::browseLoadFile()
{
  QString filename = QFileDialog::getOpenFileName();
  if( filename.isNull() ) 
    return false;
  return open(filename);
}

QRect HexEditor::charBBox( off_t charIdx ) const {
  //  byteIdx =  charIdx/charsPerByte
  //  wordIdx =  byteIdx/bytesPerWord
  int wordIdx = (charIdx/charsPerByte())/bytesPerWord();
  int localCharIdx = charIdx % charsPerWord();
  return QRect( _wordBBox[wordIdx].left() + localCharIdx*fontMaxWidth() +
		wordSpacing()/2, 
		_wordBBox[wordIdx].top(),
		fontMaxWidth(),
		fontHeight() );
}

QRect HexEditor::byteBBox( off_t byteIdx ) const {
  //  wordIdx =  byteIdx/bytesPerWord
  int wordIdx = byteIdx/bytesPerWord();
  int localByteIdx = byteIdx % bytesPerWord();
  return QRect( _wordBBox[wordIdx].left() + localByteIdx*2*fontMaxWidth() +
		wordSpacing()/2, 
		_wordBBox[wordIdx].top(),
		fontMaxWidth()*2,
		lineSpacing() );
}

void HexEditor::setTopLeftToPercent( int percent )
{
   setTopLeft( (_reader.size()/100)*percent );
}

// 
// slot for setting cursor offset.
//
void HexEditor::setOffset( off_t offset )
{
  off_t oldWordOffset = localWordOffset();
  _cursor.setOffset( offset, 0 );
  // updateWord clamps the wordIdx to [0,_rows*_cols)
  updateWord( oldWordOffset ); 
  emit offsetChanged( _cursor.byteOffset() );
}

void HexEditor::nextLine()
{
  setTopLeft(_topLeft+bytesPerLine());
}
void HexEditor::prevLine()
{
  setTopLeft(_topLeft-bytesPerLine());
}
void HexEditor::nextPage()
{
  setTopLeft(_topLeft+bytesPerPage());
}
void HexEditor::prevPage()
{
  setTopLeft(_topLeft-bytesPerPage());
}

off_t HexEditor::localByteOffsetAtXY(off_t x, off_t y) 
{
  off_t wordIdx;
  off_t wordLength = wordSpacing()+wordWidth();
  off_t line = min(y/lineSpacing(),(off_t)linesPerPage());
  
  // constrain x to be less than the right side of the last char on a line
  x = max( (off_t)0, x - leftMargin());
  x = min(wordsPerLine()*wordLength - 1 ,x);
  // constrain y to be > topMargin() and less than bottom of last line 
  y = max( (off_t)0, y - topMargin());
  line = min(y/lineSpacing(), (off_t)linesPerPage()-1);
  wordIdx = line*wordsPerLine() + x/wordLength;
  
  off_t byteOffsetInWord = (x%wordLength)*bytesPerWord()/wordLength;
  // =  wordIdx*bytesPerWord + byteOffsetInWord
  return min( (off_t) bytesPerPage()-1, 
	     wordIdx*bytesPerWord() + byteOffsetInWord);

}
//
// event handler implimentation:
//
void HexEditor::setCursorFromXY(int x,int y)
{
  off_t oldWordIdx = localWordOffset();

  _cursor.setOffset( _topLeft+localByteOffsetAtXY(x,y) ,0 );

  // update where the cursor used to be, and where it is now
  if( oldWordIdx != localWordOffset() ) {
    updateWord( oldWordIdx );
  }
  updateWord( localWordOffset() );
  emit offsetChanged(_cursor.byteOffset());
}

void HexEditor::mousePressEvent( QMouseEvent* e )
{
  setCursorFromXY(e->x(),e->y());

  off_t byte_offset = localByteOffset();
  QRect bbox = byteBBox(byte_offset);
  if( e->x() > bbox.right() ) {
    byte_offset++;
  } 
  setSelection( SelectionStart, globalOffset( byte_offset ));
}

void HexEditor::mouseMoveEvent( QMouseEvent* e )
{
  setCursorFromXY(e->x(),e->y());

  off_t byte_offset = localByteOffset();
  QRect bbox = byteBBox(byte_offset);
  if(e->x() > bbox.right() ) {
    byte_offset++;
  }

  setSelection( SelectionEnd, globalOffset( byte_offset ));
}

void HexEditor::mouseReleaseEvent( QMouseEvent* e )
{
  setCursorFromXY(e->x(),e->y());

  off_t byte_offset = localByteOffset();
  QRect bbox = byteBBox(byte_offset);
  if(e->x() > bbox.right() ) {
    byte_offset++;
  }

  setSelection( SelectionEnd, globalOffset( byte_offset ));
}

off_t HexEditor::selectionStart() const
{
  if( _selection[SelectionStart] == -1 || _selection[SelectionEnd] == -1 )
    return -1;
  return min(_selection[SelectionStart],_selection[SelectionEnd]);
}
// note: end is open. range is: [start,end)

off_t HexEditor::selectionEnd() const
{
  if( _selection[SelectionStart] == -1 || _selection[SelectionEnd] == -1 )
    return -1;
  return max(_selection[SelectionStart],_selection[SelectionEnd]);
}

void HexEditor::setSelection(SelectionPos_e pos, off_t offset)
{
  if( !_reader.is_open() ) {
    return;
  }
  if( pos == SelectionStart ) {
    _selection[SelectionEnd] = -1;
  } 
  _selection[pos] = offset;

  if( _selection[SelectionStart] < 0 ) {
    emit selectionChanged("");
  } else {
    if( selectionEnd() > -1 && selectionEnd() <= _reader.size() ) {
      QString data;
      const Delta* delta;
      for(off_t i = selectionStart();
	  i < selectionEnd();
	  ++i) {
	if( (delta = _delta.search(i)) ) 
	  data += Translate::ByteToHex(delta->newData()[0]);
	else
	  data += Translate::ByteToHex(_reader[i]);
      }
      emit selectionChanged( data );
    } else {
      emit selectionChanged( "" );
    }
  }

  repaint();
}

//
// Editor implimentation
//
void HexEditor::keyPressEvent( QKeyEvent *e )
{
  int key = e->key();

  switch(_base) {
  case -1:
    // ascii
    // range is taken from qnamespace.h
    if( key >= Qt::Key_Space  && key <= Qt::Key_AsciiTilde ) {
      seeCursor();
      vector<uchar> oldData;
      vector<uchar> newData;
      vector<uchar> chars;
      
      oldData.push_back( *(_data.begin()+localByteOffset()) );
      Translate::ByteToChar(chars,oldData);
      chars[0] = key;
      Translate::CharToByte(newData,chars);
      _delta.insert( _cursor.byteOffset(),
		     oldData,
		     newData);
      _data[_cursor.byteOffset()-_topLeft] = newData[0];
      cursorRight();
      setSelection(SelectionStart,-1);
      return;
    }
    break;
  case 16:
    if ( key >= 'A' && key <= 'F' ) {
      key = tolower(key);
    }
    if ( (key >= 'a' && key <= 'f') ||
	 (key >= '0' && key <= '9') ) {
      //
      // make sure we can see where the cursor is
      //
      seeCursor();
      vector<uchar> oldData;
      vector<uchar> newData;
      vector<uchar> hex;

      oldData.push_back( *(_data.begin()+localByteOffset()) );
      Translate::ByteToHex(hex,oldData);
      hex[_cursor.charOffset()] = key;
      Translate::HexToByte(newData,hex);

      _delta.insert( _cursor.byteOffset(),
		     oldData,
		     newData );
      _data[_cursor.byteOffset()-_topLeft] = newData[0];
      // move to the next char
      cursorRight();
      setSelection(SelectionStart,-1);
      return;
    }
    break;
  case 8:
    if( key >= '0' && key < '8' ) {
      // valid octal key
      seeCursor();
      vector<uchar> oldData;
      vector<uchar> newData;
      vector<uchar> octal;
      
      oldData.push_back( *(_data.begin()+localByteOffset()) );
      Translate::ByteToOctal(octal,oldData);
      octal[_cursor.charOffset()] = key;
      Translate::OctalToByte(newData,octal);
      
      _delta.insert( _cursor.byteOffset(),
		     oldData,
		     newData );
      _data[_cursor.byteOffset()-_topLeft] = newData[0];
      cursorRight();
      setSelection(SelectionStart,-1);
      return;
    }
    break;
  case 2:
    if( key >= '0' && key < '2' ) {
      // valid binary key
      seeCursor();
      vector<uchar> oldData;
      vector<uchar> newData;
      vector<uchar> binary;
      
      oldData.push_back( *(_data.begin()+localByteOffset()) );
      Translate::ByteToBinary(binary,oldData);
      binary[_cursor.charOffset()] = key;
      Translate::BinaryToByte(newData,binary);
      
      _delta.insert( _cursor.byteOffset(),
		     oldData,
		     newData );
      _data[_cursor.byteOffset()-_topLeft] = newData[0];
      cursorRight();
      setSelection(SelectionStart,-1);
      return;
    }
    break;
  }
  switch ( e->key() ) {
  case Qt::Key_Left:
    cursorLeft();
    break;
  case Qt::Key_Right:
    cursorRight();
    break;
  case Qt::Key_Up:
    cursorUp();
    break;
  case Qt::Key_Down:
    cursorDown();
    break;
  case Qt::Key_PageUp:
    prevPage();
    break;
  case Qt::Key_PageDown:
    nextPage();
    break;
  case Qt::Key_End:
    setTopLeft( _reader.size() - bytesPerPage()/2 );
    break;
  case Qt::Key_Home:
    setTopLeft(0);
    break;
  default:
    e->ignore();
    break;
  }
}

void HexEditor::resizeEvent( QResizeEvent * e )
{
  int height= lineSpacing();
  int totalWordWidth = wordWidth() + wordSpacing();
  int linewidth = e->size().width();

  // don't let _rows or _cols drop below 1
  _rows = max(1,(e->size().height() - _topMargin)/height);
  _cols = max(1,(e->size().width() - _leftMargin)/totalWordWidth);
  
  // now update the line && word bbox vectors
  _lineBBox.reserve(_rows);
  _wordBBox.reserve(_rows*_cols);
  int top,left;
  for(int r = 0; r < _rows; r++) {
    top = r*height + _topMargin;
    for(int c = 0; c < _cols; c++) {
      left = totalWordWidth*c + _leftMargin;
      _wordBBox[r*_cols+c] = QRect(left,             //left
				   top,              //top
				   totalWordWidth,   //width
				   height);          //height
    }
    _lineBBox[r] = QRect(_leftMargin,top,linewidth,height);
  }
  // calculate offset label bounding box
  _labelBBox.setRect(0,                        // x
		     0,                        // y
		     _leftMargin,              // width
		     e->size().height());      // height
		     
  // do this to recalculate the amount of displayed data.
  setTopLeft(_topLeft);
  emit rangeChanged(0,_reader.size()/bytesPerLine());
}
//
// Reimplimented to be more efficient then repainting the whole screen on
// focus events.
//
void HexEditor::focusInEvent( QFocusEvent* ) 
{
  updateWord( localWordOffset() );
}
void HexEditor::focusOutEvent( QFocusEvent* ) 
{
  updateWord( localWordOffset() );
}
// generate's paint events for wordIdx (global wordIdx)
// is safe to call with any wordIdx
void HexEditor::updateWord( off_t wordIdx )
{
  if( wordIdx > -1 && wordIdx < _rows*_cols ) 
    repaint(_wordBBox[wordIdx]);
}

void HexEditor::paintLabels( QPainter* paintPtr) 
{
  // ignore redraw range for first aproximation:
  int y = _wordBBox[0].bottom();
  unsigned int i;
  off_t offset = _topLeft;
  uchar *ucptr;
  QString label;

  for(int row = 0; row < _rows;++row) {
    label = "";
#ifdef WORDS_BIGENDIAN
    for(i=0, ucptr = (uchar*)&offset; i<sizeof(off_t);++i) {
      label += Translate::ByteToHex(*ucptr++);
    }
#else
    // make sure we write offset labels in big-endian (easier to read)
    ucptr = (uchar*)(&offset) + sizeof(off_t)-1;
    for(i=0;i<sizeof(off_t);++i) {
      label += Translate::ByteToHex(*ucptr--);
      //label += Translate::ByteToChar(label, *ucptr--);
    }
#endif
    label = label.mid(sizeof(off_t)*2-_offsetLabelBytes);
    paintPtr->drawText( 5, y, label  );
    offset+=bytesPerLine();
    y+=lineSpacing();
  }
  // draw dividing line between offset labels and data
  int x = leftMargin()-fontMaxWidth();
  paintPtr->drawLine(x,topMargin(),
		     x,height()-topMargin());
}
//
// painting optimizations, each time resize is called, it calculates
// the bounding boxes for each word and each line.
// This data can then be retrieved with wordBBox()
// We can then test intersections to see which words need to be redrawn
//
void HexEditor::paintEvent( QPaintEvent* e)
{
/*  QPixmap      pixmap(this->width(),this->height());
    pixmap.fill(backgroundRole());*/
  QPainter     paint( this );//&pixmap);

  // set up painter;/
  paint.setFont(font());
  const QPalette& p = qApp->palette();
  paint.setBrush(p.background());

  if( _labelBBox.intersects(e->rect()) ) {
    paintLabels(&paint);
  }

  QString text;
  if( !getDisplayText(text) ) {
      cerr << "[error] - internal inconsitency. Please file bug report."
	   << endl;
      return;
  }

  // both cursor and selection is drawn underneath the text
  drawCursor( paint );
  drawSelection( paint );
  
  // Find the stop/start row/col idx's for the repaint
  int totalWordWidth = wordWidth()+1;
  //int totalWordWidth = wordWidth()+wordSpacing();
  int row_start = max(0,(e->rect().top()-topMargin())/lineSpacing() );
  int col_start = max(0,(e->rect().left()-leftMargin())/totalWordWidth);
  int row_stop  = min(_rows-1,e->rect().bottom() / lineSpacing());
  int col_stop  = min(_cols-1,e->rect().right() / totalWordWidth);

  // draw text in repaint event
  drawTextRegion( paint, text, row_start, row_stop, col_start, col_stop );
}

bool HexEditor::getDisplayText( QString& text )
{
  // get data to draw
  switch (_base) {
  case 16:
    Translate::ByteToHex(text,_data);
    break;
  case 8:
    Translate::ByteToOctal(text,_data);
    break;
  case 2:
    Translate::ByteToBinary(text,_data);
    break;
  case -1:
    Translate::ByteToChar(text,_data);
    break;
  default:
    // error state
    return false;
  }
  return true;
}

bool HexEditor::wordModified ( off_t wordIdx ) const
{
  off_t lboffset = wordIdx*bytesPerWord()+_topLeft;
  off_t nearest_offset = _delta.lower_bound(lboffset);
  return ( nearest_offset != -1 && nearest_offset < lboffset+bytesPerWord() );
}

//
// accessors for local offsets
//
off_t HexEditor::localByteOffset()  const
{
  return _cursor.byteOffset() - _topLeft;
}
off_t HexEditor::localWordOffset() const
{
  return localByteOffset()/bytesPerWord();
}
// in offset relative to _data[0]
off_t HexEditor::localCharOffset() const
{
  return localByteOffset()*charsPerByte() + _cursor.charOffset();
}
off_t HexEditor::localLineOffset() const
{
  return localWordOffset()/wordsPerLine();
}
int HexEditor::wordWidth() const 
{
  return _fontMaxWidth*charsPerWord();
}
int HexEditor::wordSpacing() const
{
  return _wordSpacing;
}
//
// cursor movement members
//
void HexEditor::seeCursor()
{
  // see if it is already visible
  if ( _cursor.byteOffset() >= _topLeft && 
       _cursor.byteOffset() <= _topLeft+bytesPerPage()-1 ) {
    updateWord( localWordOffset() );
    return;
  } else {
    // setTopLeft so cursor is in middle line of screen
    setTopLeft( max(_cursor.byteOffset() - bytesPerPage()/2, (off_t)0) );
  }
}

void HexEditor::cursorLeft()
{
  off_t oldWordIdx = localWordOffset();
  // move the cursor
  _cursor.decrByChar(1);
  // make sure the user can see the cursor
  seeCursor();
  // redraw where the cursor used to be
  if( localWordOffset() != oldWordIdx ) 
    updateWord( oldWordIdx );
  emit offsetChanged( _cursor.byteOffset() );
}
void HexEditor::cursorRight()
{
  off_t oldWordIdx = localWordOffset();
  _cursor.incrByChar(1);
  seeCursor();
  if( localWordOffset() != oldWordIdx ) 
    updateWord( oldWordIdx );
  emit offsetChanged( _cursor.byteOffset() );
}
void HexEditor::cursorUp()
{
  off_t oldWordIdx = localWordOffset();
  _cursor.decrByByte( bytesPerLine() );
  seeCursor();
  if( localWordOffset() != oldWordIdx ) 
    updateWord( oldWordIdx );
  emit offsetChanged( _cursor.byteOffset() );
}
void HexEditor::cursorDown()
{
  off_t oldWordIdx = localWordOffset();
  _cursor.incrByByte( bytesPerLine() );
  seeCursor();
  if( localWordOffset() != oldWordIdx ) 
    updateWord( oldWordIdx );
  emit offsetChanged( _cursor.byteOffset() );
}

// slots for undo/redo.
// note: it is necessary to reset topLeft to force a reread of the data.
//
void HexEditor::redo()
{
  _delta.redo();
  setTopLeft(_topLeft);
  off_t start = selectionStart();
  off_t end   = selectionEnd();
  setSelection(SelectionStart,start);
  setSelection(SelectionEnd,end);
}

void HexEditor::undo()
{
  _delta.undo();
  setTopLeft(_topLeft);
  off_t start = selectionStart();
  off_t end   = selectionEnd();
  setSelection(SelectionStart,start);
  setSelection(SelectionEnd,end);
}

void HexEditor::search( const QString& hexText, bool forwards )
{
  QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
  //if( !hexText.length() || _reader.filename() == "" )
  if(!hexText.length() || QString(_reader.filename()).compare("") == 0)
    return;
  
  if( -1 != _delta.lower_bound(0) ) {
    QMessageBox::information(this,PROGRAM_STRING,
			     "Cannot search file with unsaved modifications.");
    return;
  }

  vector<uchar> data;
  Translate::HexToByte(data,hexText);
//  QProgressDialog progress( "Searching...","Cancel", 0,
//			    _reader.size(),this );
//  progress.setWindowModality( Qt::WindowModal );
//  progress.setWindowTitle(PROGRAM_STRING);
//  progress.setValue(0);
  off_t pos;
  if( forwards ) {
      pos = _reader.findIndex(_cursor.byteOffset(),data,_reader.size()-1);
  } else {
      pos = _reader.rFindIndex(_cursor.byteOffset(),data,0);
  }
  if( pos < _reader.size() ) {
      showMatch(pos,data.size());
      setOffset( forwards?selectionEnd():selectionStart()-1 );
      seeCursor();
  } else {
    QMessageBox::information(this,PROGRAM_STRING,
			     "Could not find search data 0x" + hexText);
  }
  QApplication::restoreOverrideCursor();
}
void HexEditor::setBaseASCII() {
  setBase(-1);
}
void HexEditor::setBaseHex()
{
  setBase(16);
}
void HexEditor::setBaseOctal()
{
  setBase(8);
}
void HexEditor::setBaseBinary()
{
  setBase(2);
}
void HexEditor::setBase(int base)
{
  switch(base) {
  case -1:
    // setup ascii editing mode
    _charsPerByte = 1;
    break;
  case 2:
    // setup binary editing mode
    _charsPerByte = 8;
    break;
  case 8:
    // setup octal editing mode
    _charsPerByte = 3;
    break;
  case 16:
    // setup hex editing mode
    _charsPerByte = 2;
    break;
  default:
    // just ignore unsupported bases for now
    return;
  }
  _base = base;
  _cursor.setCharsPerByte(_charsPerByte);
  // refresh the display 
  // note: cannot call resize() because it will ignore resize events
  //       if the size has not changed.
  QResizeEvent *re = new QResizeEvent(QSize(size()),QSize(size()));
  resizeEvent(re);
  delete re;
  // make sure we can still see the cursor
  // switching from a larger base to a smaller base has the
  // possibility of pushing the cursor off the screen
  seeCursor();
}

off_t HexEditor::offset() const
{
  return _cursor.byteOffset();
}

Reader * HexEditor::reader()
{
  return &_reader;
}

void HexEditor::showMatch( off_t pos, int len )
{
    setSelection( SelectionStart, pos );
    setSelection( SelectionEnd, pos + len );
}

void HexEditor::drawTextRegion( QPainter& paint, const QString& text,
				int row_start, int row_stop,
				int col_start, int col_stop )
{
  paint.setPen(qApp->palette().foreground().color());
  for(int r = row_start; r <= row_stop; r++) {
    for(int c = col_start; c <= col_stop; c++) {
      int widx = r*_cols+c;
      if ( wordModified( widx ) ) {
	paint.setPen(qApp->palette().link().color());
	paint.drawText( _wordBBox[widx].left() + wordSpacing()/2,
			_wordBBox[widx].bottom(),
			text.mid(widx*charsPerWord(),charsPerWord()) );
	paint.setPen(qApp->palette().foreground().color());
      } else {
	paint.drawText( _wordBBox[widx].left() + wordSpacing()/2,
			_wordBBox[widx].bottom(),
			text.mid(widx*charsPerWord(),charsPerWord()) );
      }
    }
  }
}

void HexEditor::drawSelection( QPainter& paint )
{
  // draw selection
  off_t start = max( (off_t)0, selectionStart() - _topLeft);
  if( start < bytesPerPage() ) {
    off_t stop = min(selectionEnd() - _topLeft, (off_t)bytesPerPage());
    paint.setPen(Qt::NoPen);
    paint.setBrush( qApp->palette().highlight() );
    // foreach line with selection
    stop--;
    while( start <= stop ) {
      // linestop = min(stop,endofline)
      off_t linestop = 
	min(stop, start+bytesPerLine()-1 -(start % bytesPerLine()));
      QRect bbox = byteBBox(start);
      bbox.setRight( byteBBox(linestop).right() );
      paint.drawRect( bbox );
      start = linestop+1;
    }
  }
}

void HexEditor::drawCursor( QPainter& paint )
{
  QBrush b = qApp->palette().mid();
  if( localWordOffset() > -1 && localWordOffset() < wordsPerPage() ) {
    if( hasFocus() ) {
      // draw a solid cursor
      paint.setPen(Qt::NoPen);
      paint.setBrush(b);
    } else {
      // just draw the outline
      paint.setPen(b.color());
      paint.setBrush(Qt::NoBrush);
    }
    QRect box = charBBox( localCharOffset() );
    paint.drawRect( box );
  }
}
