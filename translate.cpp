/* $Id: translate.cpp,v 1.4 2006-11-05 04:42:43 ganzhorn Exp $
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

#include "translate.h"

// translation maps from byte->hex, hex->byte, and byte->char
// mappings  are static arrays for speed
#include "mappings.h"

void Translate::CharToByte(vector<uchar>& dst, const vector<uchar>& src) 
{
  // just copy the base into the byte vector
  dst = vector<uchar>(src.begin(),src.end());
}
void Translate::CharToByte(vector<uchar>& dst, const QString& src) 
{
  // just copy the base into the byte vector
  dst.resize( src.length() );
  for(int i =0 ; i < src.length(); ++i)
      dst[i] = src[i].toLatin1();
    //dst[i] = src[i].toAscii();
}


void Translate::ByteToChar(vector<uchar>& dst, const vector<uchar>& src)
{
  dst.erase(dst.begin(),dst.end());
  dst.reserve(src.size());
  for(unsigned int i = 0; i < src.size(); i++) {
    dst.push_back(TranslationTables::byteToCharMap[src[i]]);
  }
}

void Translate::ByteToChar(QString& dst, const vector<uchar>& src)
{
  dst = "";
  for(unsigned int i = 0; i < src.size(); i++) {
    dst += TranslationTables::byteToCharMap[src[i]];
  }
}

void Translate::HexToByte(vector<uchar>& dst, const vector<uchar>& src) 
{
  dst.erase(dst.begin(),dst.end());
  int start;

  if( src.size() % 2 ) {
    // if odd
    dst.reserve(src.size()/2+1);
    dst.push_back(TranslationTables::hexToByteMap[src[0]]);
    start = 1;
  } else {
    start = 0;
    dst.reserve(src.size()/2);
  }
  for(unsigned int i = start; i < src.size(); i+=2) {
    dst.push_back(TranslationTables::hexToByteMap[src[i]]*16 + 
		  TranslationTables::hexToByteMap[src[i+1]]);
  }
}

void Translate::HexToByte(vector<uchar>& dst, const QString& src) 
{
  dst.erase(dst.begin(),dst.end());
  dst.reserve(src.length()/2);
  int i = 0;
  if( src.length() % 2 ) {
      i = 1;
  }
  for(; i < src.length(); i+=2) {
    uchar hi = src[i].toLatin1();//src[i].toAscii();
    uchar lo = src[i+1].toLatin1();//src[i+1].toAscii();
    dst.push_back( TranslationTables::hexToByteMap[hi]*16 +
		   TranslationTables::hexToByteMap[lo] );
  }
}

void Translate::OctalToByte(vector<uchar> &dst, const vector<uchar>& src)
{
  dst.erase(dst.begin(),dst.end());
  dst.reserve(src.size()/3);
  for(unsigned int i = 0; i+2 < src.size(); i+=3) {
    dst.push_back(TranslationTables::hexToByteMap[src[i]]*64+
		  TranslationTables::hexToByteMap[src[i+1]]*8+
		  TranslationTables::hexToByteMap[src[i+2]]);
  }
}

void Translate::BinaryToByte(vector<uchar> &dst, const vector<uchar>& src)
{
  dst.erase(dst.begin(),dst.end());
  dst.reserve(src.size()/8);
  // FIXME: would be faster if we compared each src[i] with '1' before mult
  for(unsigned int i = 0; i < src.size(); i+=8) {
    dst.push_back(TranslationTables::hexToByteMap[src[i]]*128+
		  TranslationTables::hexToByteMap[src[i+1]]*64+
		  TranslationTables::hexToByteMap[src[i+2]]*32+
		  TranslationTables::hexToByteMap[src[i+3]]*16+
		  TranslationTables::hexToByteMap[src[i+4]]*8+
		  TranslationTables::hexToByteMap[src[i+5]]*4+
		  TranslationTables::hexToByteMap[src[i+6]]*2+
		  TranslationTables::hexToByteMap[src[i+7]]);
  }
}

const char * Translate::ByteToHex(uchar b)
{
  return TranslationTables::byteToHexMap[b];
}

void Translate::ByteToHex(vector<uchar>& dst, const vector<uchar>& src)
{
  const char * str;
  dst.erase(dst.begin(),dst.end());
  dst.reserve(src.size()*2);
  for(unsigned int i = 0; i < src.size(); i++) {
    str = TranslationTables::byteToHexMap[src[i]];
    dst.push_back(*str);
    dst.push_back(*(str+1));
  }
}

void Translate::ByteToHex(QString& dst, const vector<uchar>&src)
{
  dst = "";
  for(unsigned int i = 0; i < src.size(); i++) {
    dst += TranslationTables::byteToHexMap[src[i]];
  }
}

void Translate::ByteToOctal(vector<uchar>&dst, const vector<uchar>&src)
{
  dst.erase(dst.begin(),dst.end());
  dst.reserve(src.size()*3);

  vector<uchar>::const_iterator itr;
  const char * str;
  for(itr = src.begin(); itr != src.end(); ++itr) {
    str = TranslationTables::byteToOctalMap[*itr];
    dst.insert( dst.end(), str, str+3 );
  }
}

void Translate::ByteToOctal(QString&dst, const vector<uchar>&src)
{
  dst = "";
  vector<uchar>::const_iterator itr;
  for(itr = src.begin(); itr != src.end(); ++itr) {
    dst += TranslationTables::byteToOctalMap[*itr];
  }
}

void Translate::ByteToBinary(vector<uchar>&dst, const vector<uchar>&src)
{
  dst.erase(dst.begin(),dst.end());
  dst.reserve(src.size()*8);

  vector<uchar>::const_iterator itr;
  const char * str;
  for(itr = src.begin(); itr != src.end(); ++itr) {
    str = TranslationTables::byteToBinaryMap[*itr];
    dst.insert(dst.end(), str, str+8);
  }
}

void Translate::ByteToBinary(QString&dst, const vector<uchar>&src)
{
  dst = "";

  vector<uchar>::const_iterator itr;
  for(itr = src.begin(); itr != src.end(); ++itr) {
    dst += TranslationTables::byteToBinaryMap[*itr];
  }
}

