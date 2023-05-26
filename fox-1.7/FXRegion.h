/********************************************************************************
*                                                                               *
*                      C l i p p i n g   R e g i o n                            *
*                                                                               *
*********************************************************************************
* Copyright (C) 2000,2022 by Jeroen van der Zijp.   All Rights Reserved.        *
*********************************************************************************
* This library is free software; you can redistribute it and/or modify          *
* it under the terms of the GNU Lesser General Public License as published by   *
* the Free Software Foundation; either version 3 of the License, or             *
* (at your option) any later version.                                           *
*                                                                               *
* This library is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 *
* GNU Lesser General Public License for more details.                           *
*                                                                               *
* You should have received a copy of the GNU Lesser General Public License      *
* along with this program.  If not, see <http://www.gnu.org/licenses/>          *
********************************************************************************/
#ifndef FXREGION_H
#define FXREGION_H

namespace FX {


/// Region
class FXAPI FXRegion {
  friend class FXDC;
  friend class FXDCWindow;
  friend class FXWindow;
private:
  void *region;
public:

  /// Construct new empty region
  FXRegion();

  /// Construct new region copied from region r
  FXRegion(const FXRegion& r);

  /// Construct new region from rectangle rect
  FXRegion(const FXRectangle& rect);

  /// Construct rectangle region
  FXRegion(FXint x,FXint y,FXint w,FXint h);

  /// Construct polygon region
  FXRegion(const FXPoint* points,FXuint npoints,FXbool winding=false);

  /// Assign region r to this one
  FXRegion &operator=(const FXRegion& r);

  /// Return true if region is empty
  FXbool empty() const;

  /// Return true if region contains point
  FXbool contains(FXint x,FXint y) const;

  /// Return true if region contains rectangle
  FXbool contains(FXint x,FXint y,FXint w,FXint h) const;

  /// Return bounding box
  FXRectangle bounds() const;

  /// Offset region by dx,dy
  FXRegion& offset(FXint dx,FXint dy);

  /// Return true if region equal to this one
  FXbool operator==(const FXRegion& r) const;

  /// Return true if region not equal to this one
  FXbool operator!=(const FXRegion& r) const;

  /// Union region r with this one
  FXRegion& operator+=(const FXRegion& r);

  /// Intersect region r with this one
  FXRegion& operator*=(const FXRegion& r);

  /// Subtract region r from this one
  FXRegion& operator-=(const FXRegion& r);

  /// Xor region r with this one
  FXRegion& operator^=(const FXRegion& r);

  /// Union of this region and region r
  FXRegion operator+(const FXRegion& r) const;

  /// Intersection of this region and region r
  FXRegion operator*(const FXRegion& r) const;

  /// Subtract region r from this region
  FXRegion operator-(const FXRegion& r) const;

  /// Xor of this region and region r
  FXRegion operator^(const FXRegion& r) const;

  /// Reset region to empty
  void reset();

  /// Destroy region
 ~FXRegion();
  };

}

#endif
