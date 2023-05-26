/********************************************************************************
*                                                                               *
*  D e v i c e   C o n t e x t   F o r   W i n d o w s   a n d   I m a g e s    *
*                                                                               *
*********************************************************************************
* Copyright (C) 1999,2022 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef FXDCWINDOW_H
#define FXDCWINDOW_H

#ifndef FXDC_H
#include "FXDC.h"
#endif

namespace FX {


class FXApp;
class FXDrawable;
class FXImage;
class FXBitmap;
class FXIcon;
class FXFont;


/**
* Window Device Context
*
* The Window Device Context allows drawing into an FXDrawable, such as an
* on-screen window (FXWindow and derivatives) or an off-screen image (FXImage
* and its derivatives).
* Because certain hardware resources are locked down, only one FXDCWindow may be
* locked on a drawable at any one time.
*/
class FXAPI FXDCWindow : public FXDC {
  friend class FXFont;
protected:
  FXDrawable *surface;        // Drawable surface
  FXRectangle rect;           // Paint rectangle inside drawable
  FXPixel     devfg;          // Device foreground pixel value
  FXPixel     devbg;          // Device background pixel value
#ifdef WIN32
  FXID        oldpalette;
  FXID        oldbrush;
  FXID        oldpen;
  FXbool      needsNewBrush;
  FXbool      needsNewPen;
  FXbool      needsPath;
  FXbool      needsClipReset;
#else
  void       *xftDraw;
  FXuint      flags;
#endif
private:
#ifdef WIN32
  void updateBrush();
  void updatePen();
#endif
private:
  FXDCWindow();
  FXDCWindow(const FXDCWindow&);
  FXDCWindow &operator=(const FXDCWindow&);
public:

  /// Construct for painting in response to expose;
  /// This sets the clip rectangle to the exposed rectangle
  FXDCWindow(FXDrawable* draw,FXEvent* event);

  /// Construct for normal drawing;
  /// This sets clip rectangle to the whole drawable
  FXDCWindow(FXDrawable* draw);

  /// Return active drawable
  FXDrawable *drawable() const { return surface; }

  /// Begin locks in a drawable surface
  void begin(FXDrawable *draw);

  /// End unlock the drawable surface
  void end();

  /// Read back pixel
  virtual FXColor readPixel(FXint x,FXint y);

  /// Draw points
  virtual void drawPoint(FXint x,FXint y);
  virtual void drawPoints(const FXPoint* points,FXuint npoints);
  virtual void drawPointsRel(const FXPoint* points,FXuint npoints);

  /// Draw lines
  virtual void drawLine(FXint x1,FXint y1,FXint x2,FXint y2);
  virtual void drawLines(const FXPoint* points,FXuint npoints);
  virtual void drawLinesRel(const FXPoint* points,FXuint npoints);
  virtual void drawLineSegments(const FXSegment* segments,FXuint nsegments);

  /// Draw rectangles
  virtual void drawRectangle(FXint x,FXint y,FXint w,FXint h);
  virtual void drawRectangles(const FXRectangle* rectangles,FXuint nrectangles);

  /// Draw rounded rectangle with ellipse with ew and ellips height eh
  virtual void drawRoundRectangle(FXint x,FXint y,FXint w,FXint h,FXint ew,FXint eh);

  /// Draw arcs
  virtual void drawArc(FXint x,FXint y,FXint w,FXint h,FXint ang1,FXint ang2);
  virtual void drawArcs(const FXArc* arcs,FXuint narcs);

  /// Draw ellipse
  virtual void drawEllipse(FXint x,FXint y,FXint w,FXint h);

  /// Filled rectangles
  virtual void fillRectangle(FXint x,FXint y,FXint w,FXint h);
  virtual void fillRectangles(const FXRectangle* rectangles,FXuint nrectangles);

  /// Filled rounded rectangle with ellipse with ew and ellips height eh
  virtual void fillRoundRectangle(FXint x,FXint y,FXint w,FXint h,FXint ew,FXint eh);

  /// Fill chord
  virtual void fillChord(FXint x,FXint y,FXint w,FXint h,FXint ang1,FXint ang2);
  virtual void fillChords(const FXArc* chords,FXuint nchords);

  /// Draw arcs
  virtual void fillArc(FXint x,FXint y,FXint w,FXint h,FXint ang1,FXint ang2);
  virtual void fillArcs(const FXArc* arcs,FXuint narcs);

  /// Fill ellipse
  virtual void fillEllipse(FXint x,FXint y,FXint w,FXint h);

  /// Filled polygon
  virtual void fillPolygon(const FXPoint* points,FXuint npoints);
  virtual void fillConcavePolygon(const FXPoint* points,FXuint npoints);
  virtual void fillComplexPolygon(const FXPoint* points,FXuint npoints);

  /// Filled polygon with relative points
  virtual void fillPolygonRel(const FXPoint* points,FXuint npoints);
  virtual void fillConcavePolygonRel(const FXPoint* points,FXuint npoints);
  virtual void fillComplexPolygonRel(const FXPoint* points,FXuint npoints);

  /// Fill vertical gradient rectangle
  virtual void fillVerticalGradient(FXint x,FXint y,FXint w,FXint h,FXColor top,FXColor bottom);

  /// Fill horizontal gradient rectangle
  virtual void fillHorizontalGradient(FXint x,FXint y,FXint w,FXint h,FXColor left,FXColor right);

  /// Draw hashed box
  virtual void drawHashBox(FXint x,FXint y,FXint w,FXint h,FXint b=1);

  /// Draw focus rectangle
  virtual void drawFocusRectangle(FXint x,FXint y,FXint w,FXint h);

  /// Draw area from source
  virtual void drawArea(const FXDrawable* source,FXint sx,FXint sy,FXint sw,FXint sh,FXint dx,FXint dy);

  /// Draw area stretched area from source
  virtual void drawArea(const FXDrawable* source,FXint sx,FXint sy,FXint sw,FXint sh,FXint dx,FXint dy,FXint dw,FXint dh);

  /// Draw image
  virtual void drawImage(const FXImage* image,FXint dx,FXint dy);

  /// Draw bitmap
  virtual void drawBitmap(const FXBitmap* bitmap,FXint dx,FXint dy);

  /// Draw icon
  virtual void drawIcon(const FXIcon* icon,FXint dx,FXint dy);
  virtual void drawIconShaded(const FXIcon* icon,FXint dx,FXint dy);
  virtual void drawIconSunken(const FXIcon* icon,FXint dx,FXint dy);

  /// Draw string with base line starting at x, y
  virtual void drawText(FXint x,FXint y,const FXString& string);
  virtual void drawText(FXint x,FXint y,const FXchar* string,FXuint length);

  /// Draw text starting at x, y over filled background
  virtual void drawImageText(FXint x,FXint y,const FXString& string);
  virtual void drawImageText(FXint x,FXint y,const FXchar* string,FXuint length);

  /// Set foreground/background drawing color
  virtual void setForeground(FXColor clr);
  virtual void setBackground(FXColor clr);

  /// Set dash pattern
  virtual void setDashes(FXuint dashoffset,const FXuchar *dashpattern,FXuint dashlength);

  /// Set line width
  virtual void setLineWidth(FXuint linewidth=0);

  /// Set line cap style
  virtual void setLineCap(FXCapStyle capstyle=CAP_BUTT);

  /// Set line join style
  virtual void setLineJoin(FXJoinStyle joinstyle=JOIN_MITER);

  /// Set line style
  virtual void setLineStyle(FXLineStyle linestyle=LINE_SOLID);

  /// Set fill style
  virtual void setFillStyle(FXFillStyle fillstyle=FILL_SOLID);

  /// Set fill rule
  virtual void setFillRule(FXFillRule fillrule=RULE_EVEN_ODD);

  /// Set blit function
  virtual void setFunction(FXFunction func=BLT_SRC);

  /// Set the tile
  virtual void setTile(FXImage* tile,FXint dx=0,FXint dy=0);

  /// Set the stipple pattern
  virtual void setStipple(FXBitmap *stipple,FXint dx=0,FXint dy=0);

  /// Set the stipple pattern
  virtual void setStipple(FXStipplePattern stipple,FXint dx=0,FXint dy=0);

  /// Set clip region
  virtual void setClipRegion(const FXRegion& region);

  /// Set clip rectangle
  virtual void setClipRectangle(FXint x,FXint y,FXint w,FXint h);

  /// Set clip rectangle
  virtual void setClipRectangle(const FXRectangle& rectangle);

  /// Clear clipping
  virtual void clearClipRectangle();

  /// Set clip mask
  virtual void setClipMask(FXBitmap* mask,FXint dx=0,FXint dy=0);

  /// Clear clip mask
  virtual void clearClipMask();

  /// Set font to draw text with
  virtual void setFont(FXFont *fnt);

  /// Clip against child windows
  virtual void clipChildren(FXbool yes);

  /// Destructor
  virtual ~FXDCWindow();
  };

}

#endif
