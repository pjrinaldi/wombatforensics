/********************************************************************************
*                                                                               *
*                        G I F   C u r so r   O b j e c t                       *
*                                                                               *
*********************************************************************************
* Copyright (C) 2000,2022 by Daniel Gehriger.   All Rights Reserved.            *
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
#ifndef FXGIFCURSOR_H
#define FXGIFCURSOR_H

#ifndef FXCURSOR_H
#include "FXCursor.h"
#endif

namespace FX {


/// GIF Cursor class
class FXAPI FXGIFCursor : public FXCursor {
  FXDECLARE(FXGIFCursor)
protected:
  FXGIFCursor(){}
private:
  FXGIFCursor(const FXGIFCursor&);
  FXGIFCursor &operator=(const FXGIFCursor&);
public:
  static const FXchar fileExt[];
public:

  /**
  * Construct a cursor from memory stream in Compuserve GIF format.
  * Hot spot may be specified using hx and hy parameters, since the GIF
  * format does not specify a hot spot. The image must be smaller than
  * 32x32 pixels.
  */
  FXGIFCursor(FXApp* a,const FXuchar* pix,FXint hx=0,FXint hy=0);

  /// Save pixel data only, in GIF format
  virtual FXbool savePixels(FXStream& store) const;

  /// Load pixel data only, in GIF format
  virtual FXbool loadPixels(FXStream& store);

  /// Destroy
  virtual ~FXGIFCursor(){}
  };


#ifndef FXLOADGIF
#define FXLOADGIF

/**
* Check if stream contains a GIF, return true if so.
*/
extern FXAPI FXbool fxcheckGIF(FXStream& store);


/**
* Load an GIF (Graphics Interchange Format) file from a stream; if the flag
* flag is true, the background color is considered transparent.  Upon successful return,
* the pixel array and size are returned.  If an error occurred, the pixel array is set to NULL.
*/
extern FXAPI FXbool fxloadGIF(FXStream& store,FXColor*& data,FXint& width,FXint& height,FXbool flag=true);


/**
* Save an GIF (Graphics Interchange Format) file to a stream.  If flag
* is true, the faster Floyd-Steinberg dither method instead of the slower Wu
* quantization algorithm.
*/
extern FXAPI FXbool fxsaveGIF(FXStream& store,const FXColor *data,FXint width,FXint height,FXbool flag=true);

#endif

}

#endif
