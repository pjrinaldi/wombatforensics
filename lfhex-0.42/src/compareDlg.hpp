#ifndef COMPARE_DLG_HPP
#define COMPARE_DLG_HPP
/* $Id: compareDlg.hpp,v 1.5 2006-11-05 04:42:43 ganzhorn Exp $
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

#include <QMainWindow>
#include <list>
#include "local.h"
// forward decl to speed compile times
class QCheckBox;
class QLineEdit;
class OffsetConstraint;
class HexGui;
class Reader;
class QTabWidget;

class CompareDlg : public QMainWindow {
  Q_OBJECT
public:
  CompareDlg(const char * file0 = 0,
	     const char * file1 = 0, 
	     QWidget * parent = 0);

 public slots:
  void setAllowDiffOffsets(int state);
  bool first();
  bool last();
  bool prev();
  bool next();
protected:
  bool enabled();
  
protected:
  QTabWidget       * tabLayout;
  QCheckBox        * diffOffsets;
  HexGui           * hexGui[2];
  Reader           * reader[2];
  OffsetConstraint * offsetConstraint;
};

#endif
