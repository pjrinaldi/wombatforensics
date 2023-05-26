/********************************************************************************
*                                                                               *
*           S i n g l e - P r e c i s i o n    S p h e r e    C l a s s         *
*                                                                               *
*********************************************************************************
* Copyright (C) 2004,2022 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef FXSPHEREF_H
#define FXSPHEREF_H

namespace FX {


class FXRangef;
class FXMat4f;


/// Spherical bounds
class FXAPI FXSpheref {
public:
  FXVec3f center;
  FXfloat radius;
public:

  /// Default constructor; value is not initialized
  FXSpheref(){}

  /// Copy constructor
  FXSpheref(const FXSpheref& sphere):center(sphere.center),radius(sphere.radius){}

  /// Initialize from center and radius
  FXSpheref(const FXVec3f& cen,FXfloat rad=0.0f):center(cen),radius(rad){}

  /// Initialize from center and radius
  FXSpheref(FXfloat x,FXfloat y,FXfloat z,FXfloat rad=0.0f):center(x,y,z),radius(rad){}

  /// Initialize sphere to fully contain the given bounding box
  FXSpheref(const FXRangef& bounds);

  /// Assignment
  FXSpheref& operator=(const FXSpheref& sphere){ center=sphere.center; radius=sphere.radius; return *this; }

  /// Set value from another sphere
  FXSpheref& set(const FXSpheref& sphere){ center=sphere.center; radius=sphere.radius; return *this; }

  /// Set value from center and radius
  FXSpheref& set(const FXVec3f& cen,FXfloat rad=0.0f){ center=cen; radius=rad; return *this; }

  /// Set value from center and radius
  FXSpheref& set(FXfloat x,FXfloat y,FXfloat z,FXfloat rad=0.0f){ center.set(x,y,z); radius=rad; return *this; }

  /// Comparison
  FXbool operator==(const FXSpheref& s) const { return center==s.center && radius==s.radius;}
  FXbool operator!=(const FXSpheref& s) const { return center!=s.center || radius!=s.radius;}

  /// Diameter of sphere
  FXfloat diameter() const { return radius*2.0f; }

  /// Area of sphere
  FXfloat area() const { return radius*radius*12.5663706143591729538505735331f; }

  /// Volume of sphere
  FXfloat volume() const { return radius*radius*radius*4.18879020478639098461685784437f; }

  /// Test if empty
  FXbool empty() const { return radius<0.0f; }

  /// Test if sphere contains point x,y,z
  FXbool contains(FXfloat x,FXfloat y,FXfloat z) const;

  /// Test if sphere contains point p
  FXbool contains(const FXVec3f& p) const;

  /// Test if sphere properly contains another box
  FXbool contains(const FXRangef& box) const;

  /// Test if sphere properly contains another sphere
  FXbool contains(const FXSpheref& sphere) const;

  /// Include point
  FXSpheref& include(FXfloat x,FXfloat y,FXfloat z);

  /// Include point
  FXSpheref& include(const FXVec3f& p);

  /// Expand radius to include point
  FXSpheref& includeInRadius(FXfloat x,FXfloat y,FXfloat z);

  /// Expand radius to include point
  FXSpheref& includeInRadius(const FXVec3f& p);

  /// Include given range into this one
  FXSpheref& include(const FXRangef& box);

  /// Expand radius to include box
  FXSpheref& includeInRadius(const FXRangef& box);

  /// Include given sphere into this one
  FXSpheref& include(const FXSpheref& sphere);

  /// Expand radius to include sphere
  FXSpheref& includeInRadius(const FXSpheref& sphere);

  /// Intersect sphere with normalized plane ax+by+cz+w; returns -1,0,+1
  FXint intersect(const FXVec4f& plane) const;

  /// Intersect sphere with ray u-v
  FXbool intersect(const FXVec3f& u,const FXVec3f& v) const;

  /// Intersect box with ray pos+lambda*dir, returning true if hit
  FXbool intersect(const FXVec3f& pos,const FXVec3f& dir,FXfloat hit[]) const;

  /// Transform sphere by 4x4 matrix
  FXSpheref transform(const FXMat4f& mat) const;

  /// Destructor
 ~FXSpheref(){}
  };


/// Test if box overlaps with sphere
extern FXAPI FXbool overlap(const FXRangef& a,const FXSpheref& b);

/// Test if sphere overlaps with box
extern FXAPI FXbool overlap(const FXSpheref& a,const FXRangef& b);

/// Test if spheres overlap
extern FXAPI FXbool overlap(const FXSpheref& a,const FXSpheref& b);

/// Save object to a stream
extern FXAPI FXStream& operator<<(FXStream& store,const FXSpheref& sphere);

/// Load object from a stream
extern FXAPI FXStream& operator>>(FXStream& store,FXSpheref& sphere);

}

#endif
