/* $Id: reader.cpp,v 1.11 2008-09-11 01:49:00 salem Exp $
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

#include <stdexcept>
#include <algorithm>
#include <new>

// for stat:
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// for strerror and errno:
#include <string.h>
#include <errno.h>

#include <assert.h>

#include <stdexcept>
#include "translate.hpp"
#include "reader.hpp" // some systems #define to map fn's to thier 64 bit
                      // equivalents. make sure the header gets processed the
                      // same as the .cc file

Reader::Reader( const string& filename, off_t npages, off_t pageSize )
  : _filename(filename),
    _pageSize(pageSize)
{
  _maxPages = _freePages = (npages < 10) ? 10: npages;
  _error   = "";
  _is_open = false;
  _offset  = 0;
  _size    = 0;
  _firstPage = -1;
  _lastPage  = -1;

  if( !filename.empty() )
    open(filename);
}

Reader::~Reader()
{
  if( _is_open )
    close();
}

//
// public methods
//

bool Reader::openimage(std::vector<std::string> imagesfullpath)
{
    const TSK_TCHAR** images;
    images = (const char**)malloc(imagesfullpath.size()*sizeof(char*));
    for(int i=0; i < imagesfullpath.size(); i++)
        images[i] = imagesfullpath[i].c_str();
    imageinfo = tsk_img_open(imagesfullpath.size(), images, TSK_IMG_TYPE_DETECT, 0);
    if(imageinfo == NULL)
        qDebug() << "image failed to open";
    else
    {
    }
    free(images);
    /*
    const TSK_TCHAR** images;
    images = (const char**)malloc(wombatptr->evidenceobject.fullpathvector.size()*sizeof(char*));
    for(int i=0; i < wombatptr->evidenceobject.fullpathvector.size(); i++)
    {
        images[i] = wombatptr->evidenceobject.fullpathvector[i].c_str();
    }
    wombatptr->evidenceobject.imageinfo = tsk_img_open(wombatptr->evidenceobject.itemcount, images, TSK_IMG_TYPE_DETECT, 0);
    if(wombatptr->evidenceobject.imageinfo == NULL)
        qDebug() << "print image error here";
    free(images);*/
}

bool Reader::open( const string& filename )
{
  // clear old data
  if( is_open() ) {
    this->close();
    if( is_open() ) // only occurs if close somehow fails. 
      return false;
  } else {
    _is_open = false;
  }

  if( 0 == (_fptr = fopen( filename.c_str() , "r" )) ) {
    _error = "Failed to open \"" + filename + "\"";
    return false;
  }

  // get filesize
  struct stat statBuf;
  if( -1 == stat(filename.c_str(), &statBuf) ) {
    _error = "Could not stat \"" + filename + "\"";
    return false;
  }

  off_t filesize = statBuf.st_size;
  _size = filesize;
  // calculate necessary number of pages
  off_t npages   = filesize/_pageSize +1;
  // initialize page pointers
  _data.resize(npages);
  fill( _data.begin(), _data.begin()+npages, (uchar*)0 );
  
  _filename = filename;
  _is_open = true;
  _firstPage = _lastPage = 0;
  return loadPage(0);
}

bool Reader::close()
{
  if( !is_open() ) {
    _error = "attempted to close non-open reader.";
    return false;
  }

  _filename = "";
  _error    = "";
  if( EOF == fclose(_fptr) ) {
    _error = strerror(errno);
    return false;
  }
  // free data pages
  vector<uchar*>::iterator itr;
  for( itr = _data.begin(); itr != _data.end(); ++itr )
    delete [] (*itr);
  _is_open = false;
  _firstPage = _lastPage = -1;
  _freePages = _maxPages;
  return true;
}

bool Reader::is_open() const
{
  return _is_open;
}

size_t Reader::read(vector<uchar>& v, size_t numBytes)
{
  int lastPageIdx = 0;
  size_t bytesRead;

  // if we don't have enough bytes left... then set page_end to lastPageIdx
  // to the last page and set bytesRead to the remaining number of bytes
  if( _offset+numBytes >= size() ) {
    _eof = true;
    lastPageIdx = _data.size()-1;
    bytesRead = size()-tell();
    // we use numBytes as a counter for the number of operations to make
    // so limit numBytes to the actual the max number of bytes left
    numBytes  = bytesRead;
  } else {
    lastPageIdx = (_offset+numBytes)/_pageSize;
    bytesRead = numBytes;
  }
  
  if( !numBytes ) 
    return numBytes;

  // only do one realloc if necessary
  v.erase(v.begin(),v.end());
  v.reserve(v.size()+numBytes);

  //  int byte_end; // == lastByteIndex+1  
  // copy the data a page at a time
  for(int page = _offset/_pageSize; page <= lastPageIdx; page++) {
    // ensure the page is loaded.
    try {
      loadPage( page );
    } 
    catch (bad_alloc){
      // out of memory
      _error = "Out of memory.";
      // return the number of bytes read
      return (_offset/_pageSize - page)*_pageSize;
    }

    int start = _offset%_pageSize;
    int stop  = (page == lastPageIdx)? start+numBytes: _pageSize;
    for(int i = start; i < stop; i++) {
      v.push_back(_data[page][i]);
    }
    numBytes -= stop-start;
    _offset  += stop-start;
  }
  return bytesRead;
}
bool Reader::eof()
{
  return (is_open())? _eof : 0;
}
off_t Reader::seek(off_t offset)
{
  if( !is_open() )
    return -1;
  
  // seek resets eof... even if the seek is past eof
  _eof = false;

  _offset = max( min( offset, size()-1 ), (off_t)0);
  if( fseek(_fptr, _offset, SEEK_SET) ) {
    _error = "Seek to offset:";
    _error += _offset;
    _error += " failed.";
    return -1;
  }
  assert(_offset == ftell(_fptr));
  return _offset;
}

off_t Reader::tell() const
{
  if(!is_open())
    return -1;
  return _offset;
}

off_t Reader::size() const
{
  return _size;
}

const char* Reader::lastError() const
{
  return _error.c_str();
}
const char* Reader::filename() const
{
  return _filename.c_str();
}

//
// Protected member fn's
//

bool Reader::loadPage(off_t pageIdx)
{
  if( !is_open() )
    return false;
  if(_data[pageIdx] != 0)
    return true;

  if( !nFreePages() ) {
    // free the page which is the furthest away from the page we are loading

    // this could be trouble if off_t is unsigned!
    if( abs(_firstPage - pageIdx) > abs(_lastPage - pageIdx) ) 
      while(!freePage(_firstPage++));
    else
      while(!freePage(_lastPage--));
  }
  _data[pageIdx] = new uchar [_pageSize];
  // page added, subtract from num available
  --nFreePages();

  fseek(_fptr,pageIdx*_pageSize, SEEK_SET);

  off_t retval = fread(_data[pageIdx], sizeof(uchar), _pageSize, _fptr);

  if( retval ) {
    if( pageIdx < _firstPage )
      _firstPage = pageIdx;

    if( pageIdx > _lastPage ) 
      _lastPage = pageIdx;
  }
  return retval;
}

bool Reader::freePage(off_t pageIdx)
{
  // check range
  if( pageIdx < 0 || (unsigned)pageIdx >= _data.size() || !_data[pageIdx] )
    return false;
  
  delete [] _data[pageIdx];
  _data[pageIdx] = 0;
  ++nFreePages(); // page freed
  return true;
}

uchar Reader::operator[] (off_t offset)
{
  if( !is_open() )
    throw logic_error("Reader::operator[] - attempt to access non-open file.");
  if( offset < 0 )
    throw out_of_range("Reader::operator[] - "
		       "attempt to access negative offset.");
  if( offset >= size() ) 
    throw out_of_range("Reader::operator[] - "
		       "attempt to access past end of file");

  off_t page_idx = offset/_pageSize;
  bool loaded = loadPage( page_idx );
  assert( loaded );
  return _data[page_idx][ offset%_pageSize ];
}

off_t
Reader::nFreePages() const
{
   return _freePages;
}

off_t&
Reader::nFreePages()
{
   return _freePages;
}

bool
Reader::dataIsAtOffset( const vector<uchar>& data, off_t pos )
{
    size_t i = 0;
    while( i < data.size() ) {
	if( data[i] != (*this)[pos+i] ) {
	    return false;
	}
	++i;
    }
    return true;
}


off_t
Reader::findIndex( off_t start, const vector<uchar>& data, off_t stop )
{
    off_t pos = start;
    while( pos <= stop ) {
	if( data[0] == (*this)[pos] ) {
	    if( dataIsAtOffset( data, pos ) ) {
		return pos;
	    }
	}
	++pos;
    }
    return size();
}

off_t
Reader::rFindIndex( off_t start, const vector<uchar>& data, off_t stop )
{
    off_t pos = start;
    while( pos >= stop ) {
	if( data[0] == (*this)[pos] ) {
	    if( dataIsAtOffset( data, pos ) ) {
		return pos;
	    }
	}
	--pos;
    }
    return size();
}

//
// ReadBuffer friends and aquaintances (only for debugging)
//

#if 0
ostream& operator<< (ostream&out, const ReadBuffer& buff)
{
  ios_base::fmtflags old_flags = out.flags();
  out.flags(old_flags | ios::hex | ios::showbase);
  for(size_t i = 0; i < buff.size(); i++)
    out << buff[i];
  // restore old flags
  out.flags(old_flags);
  return out;
}
#endif
