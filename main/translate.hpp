/* $Id: translate.hpp,v 1.4 2006-11-05 04:42:43 ganzhorn Exp $
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
#include <vector>
#include <QString>

#include "local.h"

typedef unsigned char uchar;

namespace Translate {
  const char* ByteToHex(uchar b); 
  void ByteToHex(vector<uchar>& dst , const vector<uchar>& src);
  void ByteToChar(vector<uchar>& dst, const vector<uchar>& src);
  void ByteToOctal(vector<uchar>& dst,const vector<uchar>& src);
  void ByteToBinary(vector<uchar>& dst,const vector<uchar>& src);

  void ByteToHex(QString& dst , const vector<uchar>& src);
  void ByteToChar(QString& dst, const vector<uchar>& src);
  void ByteToOctal(QString& dst,const vector<uchar>& src);
  void ByteToBinary(QString& dst,const vector<uchar>& src);

  void BinaryToByte(vector<uchar>& dst,const vector<uchar>& src);

  void OctalToByte(vector<uchar>& dst,const vector<uchar>& src);

  void CharToByte(vector<uchar>& dst, const vector<uchar>& src);
  void CharToByte(vector<uchar>& dst, const QString& src);
  
  void HexToByte(vector<uchar>& dst , const vector<uchar>& src);
  void HexToByte(vector<uchar>& dst , const QString& src);
  
};
