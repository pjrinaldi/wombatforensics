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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "filereader.h"
// some systems #define to map fn's to thier 64 bit
                      // equivalents. make sure the header gets processed the
                      // same as the .cc file

FileReader::FileReader(off_t npages, off_t pageSize) : _pageSize(pageSize)
{
  _maxPages = _freePages = (npages < 10) ? 10: npages;
  _error   = "";
  _is_open = false;
  _offset  = 0;
  _size    = 0;
  _firstPage = -1;
  _lastPage  = -1;
}

FileReader::~FileReader()
{
  if( _is_open )
    close();
}

//
// public methods
//
off_t FileReader::CurrentPage()
{
    return _offset/_pageSize;
}

bool FileReader::openimage(TskObject* tskpointer)
{
    tskptr = tskpointer;
    if(is_open())
        close();
    if(tskptr->blkaddrlist.count() > 0)
        _size = tskptr->blocksize*tskptr->blkaddrlist.count();
    else
        _size = tskptr->length;
    _pageSize = tskptr->blocksize;
    off_t npages = _size/_pageSize;
    if((_size - 1) % _pageSize != 0)
        npages++;
    _numpages = npages;
    _data.resize(npages);
    fill(_data.begin(), _data.begin()+npages, (uchar*)0);
    _is_open = true;
    _firstPage = _lastPage = 0;

    return loadimagepage(0);
}

bool FileReader::close()
{
    if(!is_open())
        return false;

    _error = "";
  // free data pages
    vector<uchar*>::iterator itr;
    for( itr = _data.begin(); itr != _data.end(); ++itr )
        delete [] (*itr);

    _is_open = false;
    _firstPage = _lastPage = -1;
    _freePages = _maxPages;
    return true;
}

bool FileReader::is_open() const
{
  return _is_open;
}

size_t FileReader::readimage(vector<uchar>& v, size_t numbytes)
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

bool FileReader::eof()
{
  return (is_open())? _eof : 0;
}

off_t FileReader::seekimage(off_t offset)
{
    if(!is_open())
        return -1;

    _eof = false;
    _offset = max(min(offset, size()-1), (off_t)0);

    return _offset;
}

off_t FileReader::tell() const
{
  if(!is_open())
    return -1;
  return _offset;
}

off_t FileReader::size() const
{
  return _size;
}

off_t FileReader::NumberPages() const
{
    return _numpages;
    //return _maxPages;
}

const char* FileReader::lastError() const
{
  return _error.c_str();
}

//
// Protected member fn's
//

bool FileReader::loadimagepage(off_t pageIdx)
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
    if(tskptr->objecttype == 5 || tskptr->objecttype == 6)
    {
        if(tskptr->blkaddrlist.count() > 0)
        {
            if(tskptr->blkaddrlist.at(0).toInt() == 0)
            {
                if(tskptr->objecttype == 6)
                {
                    qDebug() << "mftattrid:" << tskptr->mftattrid << "adsoffset:" << tskptr->adsoffset;
                    retval = tsk_fs_file_read_type(tskptr->readfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, tskptr->mftattrid, tskptr->adsoffset, (char*)_data[pageIdx], _pageSize, TSK_FS_FILE_READ_FLAG_NOID);
                }
                else
                {
                    retval = tsk_fs_file_read_type(tskptr->readfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, 0, 0, (char*)_data[pageIdx], _pageSize, TSK_FS_FILE_READ_FLAG_NOID);
                }
            }
            else
            {
                if(pageIdx < tskptr->blkaddrlist.count())
                {
                    retval = tsk_fs_read_block(tskptr->readfsinfo, tskptr->blkaddrlist.at(pageIdx).toInt(), (char*)_data[pageIdx], _pageSize);
                }
            }
        }
        else
        {
            if(tskptr->objecttype == 6)
            {
                qDebug() << "2 mftattrid:" << tskptr->mftattrid << "adsoffset:" << tskptr->adsoffset;
                retval = tsk_fs_file_read_type(tskptr->readfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, tskptr->mftattrid, tskptr->adsoffset, (char*)_data[pageIdx], _pageSize, TSK_FS_FILE_READ_FLAG_NOID);
            }
            else
            {
                retval = tsk_fs_file_read_type(tskptr->readfileinfo, TSK_FS_ATTR_TYPE_NTFS_DATA, 0, 0, (char*)_data[pageIdx], _pageSize, TSK_FS_FILE_READ_FLAG_NOID);
            }
            //qDebug() << retval;
        }
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

bool FileReader::freePage(off_t pageIdx)
{
  // check range
  if( pageIdx < 0 || (unsigned)pageIdx >= _data.size() || !_data[pageIdx] )
    return false;
  
  delete [] _data[pageIdx];
  _data[pageIdx] = 0;
  ++nFreePages(); // page freed
  return true;
}

uchar FileReader::operator[] (off_t offset)
{
  if( !is_open() )
    throw logic_error("FileReader::operator[] - attempt to access non-open file.");
  if( offset < 0 )
    throw out_of_range("FileReader::operator[] - "
		       "attempt to access negative offset.");
  if( offset >= size() ) 
    throw out_of_range("FileReader::operator[] - "
		       "attempt to access past end of file");

  off_t page_idx = offset/_pageSize;
  bool loaded = loadimagepage(page_idx);
  return _data[page_idx][ offset%_pageSize ];
}

off_t
FileReader::nFreePages() const
{
    //qDebug() << "Free Pages" << _freePages;
   return _freePages;
}

off_t&
FileReader::nFreePages()
{
   return _freePages;
}

bool
FileReader::dataIsAtOffset( const vector<uchar>& data, off_t pos )
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
FileReader::findIndex( off_t start, const vector<uchar>& data, off_t stop )
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
FileReader::rFindIndex( off_t start, const vector<uchar>& data, off_t stop )
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
