/* $Id: cursor.hpp,v 1.4 2006-11-05 04:42:43 ganzhorn Exp $
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

#ifndef _CURSOR_HPP_
#define _CURSOR_HPP_

#include <sys/types.h>
#include <limits.h>
#include <stdexcept>
#include "translate.h"

#ifdef _LARGEFILE_SOURCE
#define MAX_OFFSET LONG_LONG_MAX
#else
#define MAX_OFFSET LONG_MAX
#endif

class  Cursor {
public:
  Cursor( off_t byteOffset = 0,
	  off_t charOffset = 0,
	  off_t low        = 0,
	  off_t high       = 1,
	  off_t charsPerByte = 2 ); // defaults to hex
  Cursor( const Cursor& c);

  off_t incrByChar( off_t n );
  off_t incrByByte( off_t n );
  off_t decrByChar( off_t n );
  off_t decrByByte( off_t n );

  off_t   byteOffset() const; //returns the current byte offset
  off_t   charOffset() const; //returns the current char offset. (relative)
  off_t   charOffsetAbsolute() const; //returns the current absolute offset.

  bool   setRange( off_t low, off_t high );
  // setCharsPerByte:
  // sideEffect: sets charOffset to min(_charsPerByte-1,_charOffset)
  off_t   setCharsPerByte( off_t charsPerByte );
  void    setOffset( off_t byteOffset, off_t charOffset );

protected:
  // performs: dst = src; and clamps the value to [low,high)
  void assignClamped(off_t& dst,off_t src,off_t low,off_t high);

  enum {
    OutOfRange = -1,
  };
protected:
  off_t _byteOffset;
  off_t _charOffset;
  off_t _low;
  off_t _high;
  off_t _charsPerByte;
};

#endif
