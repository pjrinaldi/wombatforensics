#ifndef LFHEX_BOX_HPP
#define LFHEX_BOX_HPP
/* $Id: box.hpp,v 1.1 2006-11-05 04:42:43 ganzhorn Exp $
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

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QChildEvent>

template< class _layout >
class box : public QWidget {
public:
    box( QWidget* parent ) : QWidget(parent) {
	QLayout* l = new _layout(this);
	setLayout(l);
    }
    void addStretch( int s )
    {
	dynamic_cast<QBoxLayout*>(layout())->addStretch(s);
    }
protected:
    void childEvent( QChildEvent *e ) {
	if( e->added() && e->child()->isWidgetType() ) {
	    QWidget* w = dynamic_cast<QWidget*>(e->child());
	    layout()->addWidget(w);
	}
    }
};

typedef box<QHBoxLayout> hbox;
typedef box<QVBoxLayout> vbox;

#endif

