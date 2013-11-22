/* $Id: delta.hpp,v 1.5 2006-11-05 04:42:43 ganzhorn Exp $
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

#ifndef _DELTA_MAP_
#define _DELTA_MAP_

#include <sys/types.h>
#include <list>
#include <map>
#include <vector>
#include <stack>

#include <iostream>

#include "local.h"

class Delta {
public:
  Delta( off_t i, const vector<uchar>& oldData, const vector<uchar>& newData) :
    _offset(i), _old(oldData), _new(newData) {}

  void setNewData( const vector<uchar>& ndata ) { _new = ndata; }

  off_t          offset()  const { return _offset; }
  const vector<uchar>& newData() const { return _new;}
  const vector<uchar>& oldData() const { return _old;}
protected:
  off_t                 _offset;
  vector<uchar>               _old;
  vector<uchar>               _new;
};


// allocation for all delta's is dynamic, but it is all handled 
// within the class.
// the user of the class does not need to worry about memory 
// allocation/deallocation.
class DeltaMap {
public:
  DeltaMap();
  ~DeltaMap();

public:
  // insert a new delta
  size_t insert(off_t offset, 
	     const vector<uchar>& oldData,
	     const vector<uchar>& newData);
  // return the offset of the last inserted Delta.
  off_t lastOffset() const;
  // set the last delta's newData to data
  void setLastData( const vector<uchar>& data);
  int  numEdits() const { return _undo.size(); }
  // find the most recent delta
  const Delta*  search(off_t offset) const;

  // cleans undo/redo stacks and frees memory for all deltas.
  // Makes references to delta's invalid.
  void clear();

  bool undo(); //undo last insert
  bool redo(); //redo last insert

  // method for iterating over all of the modifications
  // finds the first offset that is is not less than lb
  // so lower_bound(0) returns the offset of the first delta
  // (off_t) -1 is returned on failure
  off_t lower_bound(off_t lb) const;

protected:
  typedef map<off_t,list<Delta*> > delta_map_t;
  // notes:
  // the back of the delta list is always the most recent/non undone delta.
  map<off_t,list<Delta*> > _map;
  stack<Delta*>   _undo;
  stack<Delta*>   _redo;
};

#endif
