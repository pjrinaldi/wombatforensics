/* $Id: grid.cpp,v 1.1 2006-11-05 04:42:43 ganzhorn Exp $
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

#include "grid.hpp"
#include <QGridLayout>
#include <QChildEvent>

grid::grid( int cols, QWidget* parent )
    : QWidget(parent), m_cols(cols), m_row(0), m_col(0)
{
    
    setLayout( new QGridLayout(this) );
}

void grid::childEvent( QChildEvent* e )
{
    if( e->added() && e->child()->isWidgetType() ) {
	QWidget* w = dynamic_cast<QWidget*>(e->child());
	QGridLayout* l = dynamic_cast<QGridLayout*>(layout());
	l->addWidget(w,m_row,m_col);
	++m_col;
	if( m_col >= m_cols ) {
	    m_col = 0;
	    ++m_row;
	}
    }
}
