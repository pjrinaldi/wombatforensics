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

/*
 * Updates to enable editor to open a forensic image.
 * Copyright (C) 2014 Pasquale J. Rinaldi, Jr. <pjrinaldi@gmail.com>
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
#include "translate.h"
#include "reader.h" // some systems #define to map fn's to thier 64 bit
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

  //if( !filename.empty() )
    //open(filename);
}

Reader::~Reader()
{
  if( _is_open )
    close();
}

//
// public methods
//
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

off_t Reader::CurrentPage()
{
    return _offset/_pageSize;
}

bool Reader::openimage(TskObject* tskpointer)
{
    tskptr = tskpointer;
    if(is_open())
        close();
    _filename = "test.txt";
    //_size = tskptr->length;
    _size = tskptr->imglength; // length in bytes for selected file
    qDebug() << "image length:" << tskptr->imglength;
    _pageSize = tskptr->blocksize;
    off_t npages = _size/_pageSize;
    //off_t npages = _size/_pageSize +1;
    // don't need the +1 since the _pageSize is blocksize and should always have no remainder
    qDebug() << "block size:" << _pageSize << "num of pages:" << npages;
    _numpages = npages;
    _data.resize(npages);
    fill(_data.begin(), _data.begin()+npages, (uchar*)0);
    _is_open = true;
    _firstPage = _lastPage = 0;

    return loadimagepage(0);
}

bool Reader::close()
{
    if(!is_open())
        return false;
 /*
  if( !is_open() ) {
    _error = "attempted to close non-open reader.";
    return false;
  }
  */

    _filename = "";
    _error = "";
  /*
  if( EOF == fclose(_fptr) ) {
    _error = strerror(errno);
    return false;
  }
  */
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

size_t Reader::readimage(vector<uchar>& v, size_t numbytes)
{
    int lastPageIdx = 0;
    size_t bytesread;
    // MODIFY THIS TO WHERE IT'S NOT LOADING THE WHOLE IMAGE, BUT ONLY A PAGE'S WORTH..., IF ANYTHING AT ALL HERE
    if(_offset+(int)numbytes >= size())
    {
        _eof = true;
        if(size() == 0)
            v.erase(v.begin(), v.end()); // added to clear the previous values when a file is of size 0
        lastPageIdx = _data.size()-1;
        bytesread = size() - tell();
        numbytes = bytesread;
    }
    else
    {
        lastPageIdx = (_offset+numbytes)/_pageSize;
        bytesread = numbytes;
    }

    if(!numbytes)
        return numbytes;
    v.erase(v.begin(), v.end());
    v.reserve(v.size() + numbytes);
    for(int page = _offset/_pageSize; page <= lastPageIdx; page++)
    {
        try
        {
            loadimagepage(page);
        }
        catch(bad_alloc)
        {
            return(_offset/_pageSize - page)*_pageSize;
        }
        int start = _offset%_pageSize;
        int stop  = (page == lastPageIdx)? start+numbytes: _pageSize;
        for(int i = start; i < stop; i++)
        {
            v.push_back(_data[page][i]);
        }
        numbytes -= stop-start;
        _offset  += stop-start;
    }

    return bytesread;
}

size_t Reader::read(vector<uchar>& v, size_t numBytes)
{
  int lastPageIdx = 0;
  size_t bytesRead;

  // if we don't have enough bytes left... then set page_end to lastPageIdx
  // to the last page and set bytesRead to the remaining number of bytes
  if( _offset+(int)numBytes >= size() ) {
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

off_t Reader::seekimage(off_t offset)
{
    if(!is_open())
        return -1;

    _eof = false;
    _offset = max(min(offset, size()-1), (off_t)0);

    return _offset;
    //_offset = tsk_img_read(tskptr->readimginfo, _offset, 
}

off_t Reader::seek(off_t offset)
{
  if( !is_open() )
    return -1;
  
  // seek resets eof... even if the seek is past eof
  _eof = false;

  _offset = max( min( offset, size()-1 ), (off_t)0);
  /*
  if( fseek(_fptr, _offset, SEEK_SET) ) {
    _error = "Seek to offset:";
    _error += _offset;
    _error += " failed.";
    return -1;
  }*/

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

off_t Reader::NumberPages() const
{
    return _numpages;
    //return _maxPages;
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

bool Reader::loadimagepage(off_t pageIdx)
{
    off_t retval = 0;
    if(!is_open())
        return false;
    if(_data[pageIdx] != 0)
        return true;
    if(!nFreePages())
    {
        if(abs(_firstPage - pageIdx) > abs(_lastPage - pageIdx))
            while(!freePage(_firstPage++));
        else
            while(!freePage(_lastPage--));
    }
    _data[pageIdx] = new uchar[_pageSize];
    --nFreePages();
    if(tskptr->objecttype <= 5)
    {
        retval = tsk_img_read(tskptr->readimginfo, tskptr->offset + pageIdx*_pageSize, (char*)_data[pageIdx], _pageSize);
    }
    else
    {
        //retval = tsk_img_read(tskptr->readimginfo, tskptr->offset + pageIdx*_pageSize, (char*)_data[pageIdx], _pageSize);
        //retval = tsk_fs_file_read(tskptr->readfileinfo, tskptr->offset + pageIdx*_pageSize, (char*)_data[pageIdx], _pageSize, TSK_FS_FILE_READ_FLAG_SLACK);
    }
    if(retval > 0)
    {
        if(pageIdx < _firstPage)
            _firstPage = pageIdx;
        if(pageIdx > _lastPage)
            _lastPage = pageIdx;
    }

    return 1;
}

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
