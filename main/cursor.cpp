/* $Id: cursor.cpp,v 1.3 2006-11-05 04:42:43 ganzhorn Exp $
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

#include "cursor.hpp"

//
// Cursor implimentation
//

Cursor::Cursor(off_t byteOffset, 
	       off_t charOffset, 
	       off_t low,
	       off_t high,
	       off_t charsPerByte) 
: _byteOffset(byteOffset), 
  _charOffset(charOffset), 
  _low(low),
  _high(high),
  _charsPerByte(charsPerByte)
{}
// note: range includes low but excludes high: [low,high) (like stl)
// note: [foo,foo) is considered the empty set
bool Cursor::setRange( off_t low, off_t high )
{
  if ( low > high )
    return false;
  _low = low;
  _high = high;
  return true;
}

off_t Cursor::decrByByte( off_t n )
{
  // check for underflow
  if (n > _byteOffset)
    return OutOfRange;
  else
    _byteOffset -= n;
  return _byteOffset;
}
off_t Cursor::decrByChar( off_t n )
{
  off_t byteDelta = n/_charsPerByte;
  off_t charDelta = n%_charsPerByte;
  
  if ( charDelta > _charOffset ) {
    if( byteDelta == MAX_OFFSET ) {
      throw out_of_range("Cursor decriment underflowed.\n");
    }
    byteDelta++;
  }
  // if underflow.. then just return
  if( byteDelta && OutOfRange == decrByByte( byteDelta ) ) {
      return _charOffset;
  }
  if ( charDelta > _charOffset ) {
    // offset == maxidx - abs(_charOffset - charDelta)
    _charOffset = (_charsPerByte) - (charDelta - _charOffset);
  } else {
    _charOffset -= charDelta;
  }
  return _charOffset;
}
// note: off_t is always positive
off_t Cursor::incrByChar( off_t n )
{
  off_t byteDelta = n/_charsPerByte;
  off_t charDelta = n%_charsPerByte;

  // carry
  if ( charDelta + _charOffset >= _charsPerByte ) {
    // Note: this cannot overflow unless chars perbyte == 1
    if (  byteDelta == MAX_OFFSET ) {
      throw out_of_range("Cursor increment exceeded maximum offset.\n");
    }
    byteDelta++;
  }
  // if out of range, just leave the char offset where it is
  if( byteDelta && OutOfRange == incrByByte( byteDelta ) ) {
      return _charOffset;
  }
  // limit _charOffset to _charsPerByte - 1
  _charOffset = (charDelta + _charOffset)%_charsPerByte;
  return _charOffset;
}

off_t Cursor::incrByByte( off_t byteDelta )
{
  if ( byteDelta > MAX_OFFSET - _byteOffset ) {
    throw out_of_range("Cursor increment exceeded maximum offset.\n");
  }
  if ( _byteOffset + byteDelta >= _high ) 
    return OutOfRange;
  return (_byteOffset += byteDelta);
}

//
// accessor implimentations
//
off_t Cursor::byteOffset() const
{
  return _byteOffset;
}
off_t Cursor::charOffset() const
{
  return _charOffset;
}
#define MIN(a,b) (a < b)?a:b
off_t Cursor::setCharsPerByte( off_t charsPerByte )
{
  _charsPerByte = charsPerByte;
  _charOffset = MIN(_charOffset,charsPerByte-1);
  return _charsPerByte;
}
// _byteOffset values are clamped to:
// [_low,_high)
// _charOffset values are clamped to:
// [0,_charsPerByte-1]
void Cursor::setOffset( off_t byteOffset, off_t charOffset )
{
  assignClamped(_byteOffset,byteOffset,0,_high);
  assignClamped(_charOffset,charOffset,0,_charsPerByte);
}
off_t Cursor::charOffsetAbsolute() const
{
  return _byteOffset*_charsPerByte + _charOffset;
}
//
// void Cursor::assignClamped(dst,src, low,high);
// convienience function for assigning dst = src
// clamped to [low,high)
//
void Cursor::assignClamped(off_t& dst, off_t src, off_t low, off_t high)
{
  if( src < low ) {
    dst = low;
  } else if( src >= high ) {
    dst = high-1;
  } else {
    dst = src;
  }
} 
