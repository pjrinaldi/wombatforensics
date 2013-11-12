/* $Id: offsetConstraint.cpp,v 1.7 2006-11-05 04:42:43 ganzhorn Exp $
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
#include <QSpinBox>
#include <QHBoxLayout>

#include <algorithm> // for min<>()

#include "local.h"
#include "offsetConstraint.hpp"
#include "box.hpp"

OffsetConstraint::OffsetConstraint( QWidget * parent )
    : QGroupBox(parent)
{
  QLayout* l = new QHBoxLayout(this);
  this->setLayout(l);
  hbox * h = new hbox(this);
  l->addWidget(h);
  new QLabel("X =",h);
  stride = new QSpinBox(h);
  stride->setRange(1,16);
  stride->setValue(1);
  connect(stride,SIGNAL(valueChanged(int)),
	  this,SLOT(setStride(int)));

  h = new hbox(this);
  l->addWidget(h);
  new QLabel("Y =",h);
  offset = new QSpinBox(h);
  offset->setRange(1,16);
  offset->setValue(1);

  setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));
}

// public interface

int OffsetConstraint::getStride(void) const
{
  return stride->text().toInt();
}

int OffsetConstraint::getOffset(void) const
{
  return offset->text().toInt();
}

off_t OffsetConstraint::next(off_t off) const
{
  return( ((off-getOffset())/getStride()+1)*getStride()+getOffset() );
}

off_t OffsetConstraint::prev(off_t off) const
{
  return( ((off-getOffset())/getStride() - 1)*getStride()+getOffset() );
}

// public slots
void OffsetConstraint::setOffset(int x)
{
  offset->setValue( min(x,getStride()-1) );
}

void OffsetConstraint::setStride(int x)
{
  // constrain x to (0,MAXINT)
  if( x < 1 ) {
    stride->setValue(1);
    return;
  }

  // constrain offset to [0,x)

  // setRange() does not seem to work as the documentation says, so
  // we have to manually change the value
  if( offset->text().toInt() >= x )
    offset->setValue(x-1);
  offset->setRange(0,x-1);
  return;
}
  
