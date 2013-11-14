/* $Id: driver.cpp,v 1.9 2008-09-11 01:49:00 salem Exp $
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

#include <qapplication.h>
#include <stdexcept>
#include <unistd.h>

extern "C" {
  int optind;
};
//#include "hexGui.hpp"
#include "local.h"
//#include "compareDlg.hpp"

#define STUB(x) extern "C" { void x() {}}
STUB(glXCreateGLXPixmapMESA);
STUB(glXReleaseBuffersMESA);

// this is used for debugging because I cannot seem to figure out how to see
// the asci representation of a QString while in the debugger.
#include <iostream>
void coutAsciText( const QString& str )
{
  cout << &str << " -> \"" << C_STR(str) << "\"" << endl;
}

int main (int argc, char ** argv)
{
  QApplication a(argc,argv);
  a.setStyle( "plastique" );
  // parse the command line and see what major mode we should be in
  int compare = false;
  int c;
  // skip argv[0]
  optind = 1;
  while( EOF != (c = getopt(argc,argv,"c")) ) {
    switch (c) {
    case 'c':
      compare = true;
      break;
    default:
      cerr << "usage: " PROGRAM " [-c [file1 file2]] files..." <<endl;
      exit(1);
      break;
    }
  }
  if( compare ) {
    int nopts = argc - optind;
    // check to see if files were passed in
    if( nopts >= 2 ) {
      (new CompareDlg(argv[optind],argv[optind+1]))->show();
      optind+=2;
    } else {
      (new CompareDlg())->show(); 
    }
    // treat remaining cmdline options as files to be opened.
    while( optind < argc ) {
      HexGui *hg = new HexGui();
      hg->open(argv[optind++]);
      hg->show();
    }
  } else {
  // each remaining argument is assumed to be a file
    if( optind >= argc ) {
      // no files, just open a hexGui
      (new HexGui())->show();
    } else {
      while( optind < argc ) {
	HexGui *hg = new HexGui();
	hg->open(argv[optind++]);
	hg->show();
      }
    }
  }

  a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
  int ret=0;
  try {
    ret = a.exec();
  } catch (const exception& e) {
    ret = 1;
    cerr << "[error] - unhandled exeption in main:\"" << e.what() << "\"" <<endl;
  }
  return ret;
}
