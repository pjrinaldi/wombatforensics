/* $Id: save.cpp,v 1.4 2006-11-05 04:42:43 ganzhorn Exp $
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

#include <errno.h>
#include <string>
#include <vector>
#include <qmessagebox.h>
#include <sys/types.h>

#include "translate.hpp"
#include "local.h"
#include "delta.hpp"
#include "reader.hpp"

extern int errno;

bool writeDeltas( const string& filename , DeltaMap& dmap )
{
  FILE* fptr = fopen( filename.c_str(), "r+" );
  if( NULL == fptr ) {
    QString message = "Error opening \"";
    message += filename.c_str();
    message += "\": ";
    message += strerror(errno);
    QMessageBox::critical( NULL, PROGRAM_STRING,
			   message,
			   "Ok");
    return false;
  }

  const Delta * deltaPtr;
  for(off_t offset = dmap.lower_bound(0);
      offset != -1;
      offset = dmap.lower_bound(offset+1)) {
    if( fseek(fptr,offset,SEEK_SET) ) {
      fclose(fptr);
      QString message = "Error seeking to offset ";
      message += offset;
      message += ": ";
      message += strerror(errno);
      QMessageBox::critical( NULL, PROGRAM_STRING,
			     message,
			     "Ok");
      return false;
    }
    deltaPtr = dmap.search(offset);
    fwrite( &(deltaPtr->newData()[0]), sizeof(uchar) , 
	    deltaPtr->newData().size(), fptr);
  }
  fclose(fptr);
  return true;
}
