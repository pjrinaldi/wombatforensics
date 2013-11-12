/* $Id: conversionDlg.cpp,v 1.7 2006-11-05 04:42:43 ganzhorn Exp $
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
#include <QApplication>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QMenuBar>
#include <QToolTip>

#include <iostream>
#include <float.h>

#include "conversionDlg.hpp"
#include "local.h"
#include "grid.hpp"
#include "box.hpp"

ConversionDlg::ConversionDlg ( QWidget*parent )
    : QWidget(parent)
{
  inValueChanged = false;
  

  QVBoxLayout *vbox = new QVBoxLayout(this);
  grid* g = new grid(3,this);

  g->setContentsMargins(3,3,3,3);

  vbox->addWidget(g);

  new QWidget(g);
  new QLabel(" Hex:",g);
  leditors[HexLE] = new QLineEdit(g);

  new QWidget(g);
  new QLabel(" ASCII:",g);
  leditors[AsciiLE] = new QLineEdit(g);

  byteSwapFlag[IntegerBS] = new QCheckBox("b/s",g);
  new QLabel(" Int:",g);
  leditors[IntegerLE] = new QLineEdit(g);

  byteSwapFlag[FloatBS] = new QCheckBox("b/s",g);
  new QLabel(" Float:",g);
  leditors[FloatLE]  = new QLineEdit(g);

  byteSwapFlag[DoubleBS] = new QCheckBox("b/s",g);
  new QLabel(" Double:",g);
  leditors[DoubleLE] = new QLineEdit(g);

  // make sure to do an update if any of the byte swap flags toggle
  for(int i = MinBS; i < MaxBS; ++i) {
    connect(byteSwapFlag[i],SIGNAL(clicked()),
				   this,SLOT(valueChanged()));
    // add tool tip
    byteSwapFlag[i]->setToolTip("Byte Swap Data");
  }

  // setup validators
  leditors[DoubleLE]->setValidator( new QDoubleValidator(this) );
  QDoubleValidator *vd = new QDoubleValidator(this);
  vd->setRange( FLT_MAX, FLT_MAX );
  leditors[FloatLE]->setValidator( vd );
  leditors[IntegerLE]->setValidator( new QIntValidator(this) );
  leditors[HexLE]->setValidator( new HexValidator(this) );

  // setup editor connections
  connect(leditors[DoubleLE],SIGNAL(textChanged(const QString& )),
	  this,SLOT(doubleChanged(const QString&)) );
  connect(leditors[FloatLE],SIGNAL(textChanged(const QString& )),
	  this,SLOT(floatChanged(const QString&)) );
  connect(leditors[IntegerLE],SIGNAL(textChanged(const QString&)),
  	  this,SLOT(integerChanged(const QString&)) );
  connect(leditors[HexLE],SIGNAL(textChanged(const QString&)),
	  this,SLOT(valueChanged(const QString&)));
  connect(leditors[AsciiLE],SIGNAL(textChanged(const QString&)),
	  this,SLOT(asciiChanged(const QString&)));
  
  for( int i = MinLE; i < MaxLE; ++i ) {
      connect(leditors[i],SIGNAL(returnPressed()),
	      this,SIGNAL(nextPressed()));
  }
  
  hbox * h = new hbox(this);
  vbox->addWidget(h);
  vbox->addStretch(1);
  QPushButton* prev = new QPushButton("<",h);
  QPushButton* next = new QPushButton(">",h);

  connect(prev,SIGNAL(clicked()),this,SIGNAL(prevPressed()));
  connect(next,SIGNAL(clicked()),this,SIGNAL(nextPressed()));
  
  setWindowTitle("Conversion Dialog");
}

ConversionDlg::~ConversionDlg()
{
  cout << "died" <<endl;
}

void ConversionDlg::floatChanged(const QString& str)
{
  QChar lastCh = str[str.length()-1];
  if( lastCh == 'e' || lastCh == '-' || lastCh == '.' ) {
    return;
  }

  bool   success;
  float value = str.toFloat(&success);

  if( !success && !value ) {
    return;
  }

  // double to hex:
  uchar * ptr = (uchar*) &value;
  vector<uchar> bytes;

  if( byteSwapFlag[FloatBS]->isChecked() ) {
    for( int i = sizeof(float)-1; i > -1; i--)
      bytes.push_back(ptr[i]);
  } else {
    for( unsigned int i = 0; i < sizeof(float); i++ ) 
      bytes.push_back(ptr[i]);
  }

  QString hex;
  Translate::ByteToHex(hex,bytes);

  valueChanged( hex );
}

void ConversionDlg::doubleChanged(const QString& str)
{
  QChar lastCh = str[str.length()-1];
  if( lastCh == 'e' || lastCh == '-' || lastCh == '.' ) {
    return;
  }

  bool   success;
  double value = str.toDouble(&success);

  if( !success && !value ) {
    return;
  }

  // double to hex:
  uchar * ptr = (uchar*)&value;
  vector<uchar> bytes;

  if(byteSwapFlag[DoubleBS]->isChecked() ) {
    for(int i = sizeof(double)-1; i > -1; --i)
      bytes.push_back(ptr[i]);
  } else {
    for(unsigned int i = 0; i < sizeof(double); i++ )
      bytes.push_back(ptr[i]);
  }
  QString hex;
  Translate::ByteToHex(hex,bytes);

  valueChanged( hex );
}

void ConversionDlg::asciiChanged(const QString &str)
{
  vector<uchar> bytes;
  QString hex;
  Translate::CharToByte(bytes,str);
  Translate::ByteToHex(hex,bytes);

  valueChanged( hex );
}

void ConversionDlg::integerChanged( const QString & str )
{
  bool success;
  int value   = str.toInt(&success);

  uchar * ptr = (uchar*) &value;
  vector<uchar> bytes;

  if( byteSwapFlag[IntegerBS]->isChecked() ) {
    for( int i = sizeof(int)-1; i > -1 ;--i )
      bytes.push_back(ptr[i]);
  } else {
    for( unsigned int i = 0; i < sizeof(int)/sizeof(uchar); i++)
      bytes.push_back(ptr[i]);
  }

  QString hex;
  Translate::ByteToHex(hex,bytes);
  
  valueChanged( hex );
}

// this slot just recalculates conversions
void ConversionDlg::valueChanged()
{
  // make sure the HexLE has focus, so valueChanged(str) updates all fields
  leditors[HexLE]->setFocus();
  valueChanged( leditors[HexLE]->text() );
}
// convert data to hex, then call updateData to set all the other data fields
void ConversionDlg::valueChanged( const QString& str ) 
{
  if( str.size()%2 ) {
      return;
  }
  // don't allow this call to nest
  if(inValueChanged)
    return;

  if( str.isEmpty()  )
    return;
  
  inValueChanged = true;
  // remmember cursor offset
  int cursor_pos = leditors[HexLE]->cursorPosition();
  // conversion is safe because all chars are less than 'f'
  leditors[HexLE]->setText( str );
  leditors[HexLE]->setCursorPosition( cursor_pos );
  
  vector<uchar> bytes;
  Translate::HexToByte(bytes,str);
  if( !leditors[AsciiLE]->hasFocus() ) {
    
    if(!bytes.size()) {
      for(int i = MinLE; i < MaxLE; ++i)
	leditors[i]->setText( "" );
      inValueChanged = 0;
      return;
    }
    
    // update the ascii entry:
    QString ascii;
    Translate::ByteToChar(ascii,bytes);
    
    leditors[AsciiLE]->setText( ascii );
  }
  
  // scratch conversion vars
  QString strvalue;
  uchar * ucharPtr;
  
  if( !leditors[IntegerLE]->hasFocus() ) {
    // update the int entry:
    // pad right with 0x00
    int intvalue = 0;
    ucharPtr = (uchar*) &intvalue;
    if( byteSwapFlag[IntegerBS]->isChecked() ) {
      for(unsigned int i = 0; (i < sizeof(int)) && (i < bytes.size()); i++)
	ucharPtr[sizeof(int)-1-i] = bytes[i];
    } else {
      memcpy(&intvalue,&bytes.begin()[0],
	     min(sizeof(int),bytes.size()));
    }
    strvalue.setNum(intvalue); 
    leditors[IntegerLE]->setText( strvalue );
  }
  
  if( ! leditors[FloatLE]->hasFocus() ) {
    // update the float entry:
    float fvalue;
    ucharPtr = (uchar*)(&fvalue);
    if( byteSwapFlag[FloatBS]->isChecked() ) {
      for(unsigned int i = 0; i < sizeof(float); ++i) {
	if( i < bytes.size() ) {
	  ucharPtr[sizeof(float)-1-i] = bytes[i];
	} else {
	  ucharPtr[sizeof(float)-1-i] = 0x00;
	}
      }
    } else {
      if(bytes.size() < sizeof(float) ) {
	for(unsigned int i= 0; i < sizeof(float); ++i) {
	  if( i < bytes.size() ) {
	    *ucharPtr++ = bytes[i];
	  } else {
	    *ucharPtr++ = 0x00;
	  }
	}
      } else {
	memcpy(&fvalue,&bytes.begin()[0],sizeof(float));
      }
    }
    strvalue.setNum( fvalue );
    leditors[FloatLE]->setText( strvalue );
  }

  if( ! leditors[DoubleLE]->hasFocus() ) {
    // update the double entry:
    double dvalue;
    ucharPtr = (uchar*)&dvalue;
    if( byteSwapFlag[DoubleBS]->isChecked() ) {
      for(unsigned int i= 0; i < sizeof(double); ++i) {
	if( i < bytes.size() ) {
	  ucharPtr[sizeof(double)-1-i] = bytes[i];
	} else {
	  ucharPtr[sizeof(double)-1-i] = 0x00;
	}
      }
    } else {
      if(bytes.size() < sizeof(double) ) {
	for(unsigned int i= 0; i < sizeof(double); ++i) {
	  if( i < bytes.size() ) {
	    *ucharPtr++ = bytes[i];
	  } else {
	    *ucharPtr++ = 0x00;
	  }
	}
      } else {
	memcpy(&dvalue,&bytes.begin()[0],sizeof(double));
      }
    }
    strvalue.setNum( dvalue );
    leditors[DoubleLE]->setText( strvalue );
  }

  inValueChanged = false;
}

QString ConversionDlg::hexValue() const
{
    return leditors[HexLE]->text();
}

HexValidator::HexValidator( QWidget * parent )
    : QValidator(parent)
{
}

// allow 0xfff syntax
QValidator::State HexValidator::validate ( QString &str, int &pos ) const
{
  str = str.toLower();
  for(int i = 0; i < pos; i++) {
    if( ! ((str[i] >= 'a' && str[i] <= 'f') ||
	   (str[i] >= '0' && str[i] <= '9') ) &&
	! (i == 1 && str[i] == 'x' )
	) {
      return Invalid;
    }
  }
  return Acceptable;
}

