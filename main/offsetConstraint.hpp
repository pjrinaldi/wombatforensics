/* $Id: offsetConstraint.hpp,v 1.5 2006-11-05 04:42:43 ganzhorn Exp $
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

// widget for constraining offset to stride*n+offset

#include <sys/types.h>
#include <QGroupBox>

class QWidget;
class QSpinBox;

class OffsetConstraint :public QGroupBox {
  Q_OBJECT

public:
  OffsetConstraint( QWidget * parent = 0 );

  int getStride(void) const;
  int getOffset(void) const;

  // applies formula to find next/previous constrained offset
  off_t next(off_t off) const;
  off_t prev(off_t off) const;

public slots:
  void setStride(int);
  void setOffset(int);

protected:
  QSpinBox    * stride;
  QSpinBox    * offset;  // [0,stride)
  
};
