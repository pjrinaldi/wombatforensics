/********************************************************************************
*                                                                               *
*                     G L  R e n d e r i n g   C o n t e x t                    *
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
#ifndef FXGLCONTEXT_H
#define FXGLCONTEXT_H

#ifndef FXID_H
#include "FXId.h"
#endif

namespace FX {


/// GL Context
class FXAPI FXGLContext : public FXId {
  FXDECLARE(FXGLContext)
private:
  FXDrawable  *surface;         // Drawable surface
  FXGLVisual  *visual;          // Visual of the context
  FXGLContext *shared;          // Shared with other
private:
  FXGLContext(const FXGLContext&);
  FXGLContext &operator=(const FXGLContext&);
protected:
  FXGLContext();
public:

  /**
  * Construct an GLContext with given GL Visual.  Optionally
  * share GL objects (textures, etc) with another GLContext shr.
  */
  FXGLContext(FXApp *a,FXGLVisual *vis,FXGLContext* shr=nullptr);

  /// Create context
  virtual void create();

  /// Detach context
  virtual void detach();

  /// Destroy context
  virtual void destroy();

  /// Change visual
  void setVisual(FXGLVisual* vis);

  /// Get the visual
  FXGLVisual* getVisual() const { return visual; }

  /// Change share context
  void setShared(FXGLContext *ctx);

  /// Get share context
  FXGLContext* getShared() const { return shared; }

  /// Return active drawable
  FXDrawable *drawable() const { return surface; }

  /// Make context current on drawable
  FXbool begin(FXDrawable *draw);

  /// Make context non current
  FXbool end();

  /// Swap front and back buffer
  void swapBuffers();

  /// Return true if THIS context is current
  FXbool isCurrent() const;

  /// Return true if thread has ANY current context
  static FXbool hasCurrent();

  /// Has double buffering
  FXbool isDoubleBuffer() const;

  /// Has stereo buffering
  FXbool isStereo() const;

  /// Save object to stream
  virtual void save(FXStream& store) const;

  /// Load object from stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXGLContext();
  };



/// Create a display list of bitmaps from font glyphs in a font
extern FXAPI FXbool glUseFXFont(FXFont* font,int first,int count,int list);

}

#endif

