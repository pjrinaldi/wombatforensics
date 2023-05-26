/********************************************************************************
*                                                                               *
*      A d d i t i o n a l   F O X   I n c l u d e   F i l e   F o r   3 D      *
*                                                                               *
*********************************************************************************
* Copyright (C) 1997,2022 by Jeroen van der Zijp.   All Rights Reserved.        *
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
#ifndef FX3D_H
#define FX3D_H

// OpenGL includes
#ifdef WIN32
#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
#undef max
#undef min
#endif
#ifdef HAVE_GL_H
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <GL/gl.h>
#endif
#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif
#ifndef GLAPI
#define GLAPI
#endif
#ifdef HAVE_GLU_H
#include <GL/glu.h>
#endif

// Additional FOX includes
#include "FXVec2f.h"
#include "FXVec2d.h"
#include "FXVec3f.h"
#include "FXVec3d.h"
#include "FXVec4f.h"
#include "FXVec4d.h"
#include "FXQuatf.h"
#include "FXQuatd.h"
#include "FXMat2f.h"
#include "FXMat2d.h"
#include "FXMat3f.h"
#include "FXMat3d.h"
#include "FXMat4f.h"
#include "FXMat4d.h"
#include "FXRangef.h"
#include "FXRanged.h"
#include "FXSpheref.h"
#include "FXSphered.h"
#include "FXExtentf.h"
#include "FXExtentd.h"
#include "FXComplexf.h"
#include "FXComplexd.h"
#include "FXGLVisual.h"
#include "FXGLContext.h"
#include "FXGLCanvas.h"
#include "FXGLViewer.h"
#include "FXGLObject.h"

#ifndef FX_NO_GLOBAL_NAMESPACE
using namespace FX;
#endif

#endif
