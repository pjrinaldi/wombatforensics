/* $Id: delta.cpp,v 1.4 2006-11-05 04:42:43 ganzhorn Exp $
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

#include "delta.hpp"

//
// DeltaMap class implimentation:
//

DeltaMap::DeltaMap() {} // noop
DeltaMap::~DeltaMap()
{
  // just free all of the Delta's
  clear();
}

// Free all the Delta's
// Note: this invalidates all pointers to Delta's returned through search()
// do NOT keep returned pointers arround for long.
//
void DeltaMap::clear()
{
  _map.erase(_map.begin(),_map.end());
  
  // note: the union of _undo and _redo should contain ALL of the Delta's
  // in the map. The intersection of _undo and _redo should be the empty set.
  while ( !_undo.empty() ) {
    delete _undo.top();
    _undo.pop();
  }
  while ( !_redo.empty() ) {
    delete _redo.top();
    _redo.pop();
  }
}

// returns const ptr to the most recent delta on offset off_t
const Delta * DeltaMap::search( off_t offset ) const
{
  delta_map_t::const_iterator itr = _map.find(offset);
  if ( itr != _map.end() && (itr->second).size() )
    return itr->second.back();
  else
    return 0;
}

size_t DeltaMap::insert( off_t offset, 
		      const vector<uchar>& oldData, 
		      const vector<uchar>& newData )
{
  //
  // only modify the newData of the top undo iff the offsets match
  // and the redo stack is empty.
  //
  if( _undo.size() && _undo.top()->offset() == offset && _redo.empty() ) {
    //just modify the newData of the top delta
    _undo.top()->setNewData( newData );

    // _map[offset] _should_ exist
    return _map[offset].size();
  } else {
    // create a new delta
    Delta * tmp = new Delta(offset, oldData, newData);
    _map[offset].push_back( tmp );
    _undo.push( tmp );
    return _map[offset].size();
  }
}

bool DeltaMap::undo()
{
  if( _undo.empty() )
    return false;

  Delta* d = _undo.top();
  _undo.pop();
  // find the delta in the delta list indexed by d->offset and remove it.
  map<off_t,list<Delta*> >::iterator itr = _map.find(d->offset());
  itr->second.remove(d);
  // remove the entry iff the list is empty
  if( itr->second.empty() ) {
    _map.erase(itr);
  }
  // push the delta back onto the redo stack.
  _redo.push( d );
  return true;
}

bool DeltaMap::redo()
{
  if( _redo.empty() )
    return false;

  // insert the delta back into the modification list
  Delta *d = _redo.top();
  _redo.pop();
  _map[d->offset()].push_back( d );
  _undo.push( d );
  return true;
}

// methods for iterating over all the modifications in the tree
off_t DeltaMap::lower_bound(off_t lb) const
{
  delta_map_t::const_iterator itr = _map.lower_bound(lb);
  if( itr == _map.end() ) {
    return -1;
  } else {
    return itr->first;
  }
}
