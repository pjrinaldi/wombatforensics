/* $Id: reader.hpp,v 1.6 2006-11-05 04:42:43 ganzhorn Exp $
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
#ifndef _READER_H
#define _READER_H

//
// Interface for Reader object.
//
// Reader object supports:
// open(const QString& filename ); -> open a file for reading
// bool is_open();                 -> see if file is open
// close();                        -> free resources for open file.
// int read( vector<uchar>& v, int maxSize ) -> fill v with maxSize data from
//                                              open file.
// off_t seek( off_t offset );     -> move file pointer to offset offset.
//

#include "wombatinclude.h"

typedef vector<uchar> ReadBuffer;

#ifdef __GNUC__
ostream& operator<< (ostream&out,const ReadBuffer& buff);
#endif

class Reader {
 public:
  // default is only 50*4k = 200k memory image.
  Reader(off_t npages = 50, off_t pageSize = 4096);
  ~Reader();


  // CUSTOM ABSTRACTION FUNCTIONS
  void SetData(vector<uchar*> tmpdata);
  void LoadPage();
  // END CUSTOM FUNCTIONS

  bool openimage(TskObject* tskobject);
  bool close();
  bool eof();
  bool is_open() const;
  
  size_t readimage(ReadBuffer& v, size_t numbytes);
  off_t seekimage(off_t offset);
  bool loadimagepage(off_t pageIdx);
  off_t CurrentPage(void);
  off_t tell() const; // returns the current offset or -1 if !open
  off_t size() const;
  off_t NumberPages() const;
  const char* lastError() const;

  uchar operator[] (off_t offset); // cannot be const because it does change
                                   // data by loading/swapping pages
  off_t findIndex( off_t start, const vector<uchar>& data,
		   off_t stop );
  off_t rFindIndex( off_t start, const vector<uchar>& data,
		    off_t stop );

  bool isfile;
  off_t _pageSize;
  off_t _numpages;
  off_t _size;
  off_t _firstPage;
  off_t _lastPage;
  off_t _offset;
  bool _eof;
  off_t _freePages;
  off_t _maxPages;

  bool freePage(off_t pageIdx);
  off_t nFreePages() const;
  off_t& nFreePages();

 protected:
  bool dataIsAtOffset( const vector<uchar>& data, off_t pos );
  bool loadPage(off_t pageIdx);
  //bool freePage(off_t pageIdx);
  //off_t nFreePages() const;
  //off_t& nFreePages();

 protected:
  string        _error;
  FILE*         _fptr;
  bool          _is_open;
  //bool          _eof;
  //off_t         _offset;         // current offset
  //off_t         _size;           // file size from fstat
  //off_t         _pageSize;       // number of bytes in a page
  //off_t         _firstPage;      // first currently loaded page
  //off_t         _lastPage;       // last currently loaded page
  //off_t         _maxPages;       // maximum number of pages which could be currently loaded
  //off_t         _numpages;      // total number of pages for a file size.
  //off_t         _freePages;     // number of free pages
  vector< uchar *> _data;
  TskObject* tskptr;
};

#endif
