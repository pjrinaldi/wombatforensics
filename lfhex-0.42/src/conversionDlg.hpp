/* $Id: conversionDlg.hpp,v 1.4 2006-11-05 04:42:43 ganzhorn Exp $
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

#ifndef _CONVERSION_DLG_HPP_
#define _CONVERSION_DLG_HPP_
#include <QWidget>
#include <QWidget>
#include <QValidator>

#include "translate.hpp"

class QLineEdit;
class QCheckBox;

class ConversionDlg : public QWidget {
  Q_OBJECT
public:
  ConversionDlg(QWidget* parent=0);
  ~ConversionDlg();

  QString hexValue() const;

signals:
  void nextPressed();
  void prevPressed();

public slots:
  void doubleChanged( const QString& str  );
  void floatChanged( const QString& str   );
  void integerChanged( const QString& str );
  void valueChanged( const QString& str   );
  void valueChanged();
  void asciiChanged( const QString& str   );
    
protected:
  enum {
    MinLE = 0,
    HexLE = 0 ,
    AsciiLE,
    DoubleLE,
    FloatLE,
    IntegerLE,
    MaxLE
  };
  enum {
    MinBS =0,
    DoubleBS=0,
    FloatBS,
    IntegerBS,
    MaxBS
  };
  QLineEdit * leditors[MaxLE];
  QCheckBox * byteSwapFlag[MaxBS];
  bool        inValueChanged;
};

class HexValidator : public QValidator {
public:
  HexValidator( QWidget* parent );
  State validate( QString & str, int & pos ) const;
};

#endif
