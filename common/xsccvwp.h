/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved. */

#ifndef xsccvwp_h
#define xsccvwp_h


#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>
#include "xsccvw.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Widget Class Declaration */
typedef struct _SccViewerClassPart
    {
    int dummy;
    } SccViewerClassPart;

typedef struct _SccViewerClassRec
    {
    CoreClassPart               core_class;
    SccViewerClassPart          sccViewer_class;
    } SccViewerClassRec;

extern SccViewerClassRec sccViewerClassRec;


/* Widget Instance Declaration */
typedef struct _SccViewerPart
    {
    /* resources */
    Dimension       sccViewerScrollBarWidth;
    Widget      sccViewerHScrollBar;
    Widget      sccViewerVScrollBar;
    Widget      sccViewerViewWindow;
    XFontStruct *sccViewerMessageFont;
    XtCallbackList  sccViewerCallBack;

    /* Private data */
    void * pViewInfo;
    } SccViewerPart;

typedef struct _SccViewerRec
    {
    CorePart                core;
    SccViewerPart           sccViewer;
    } SccViewerRec;

#ifdef __cplusplus
}  /* Close scope of 'extern "C"' declaration which encloses file. */
#endif


#endif  /* End of  xsccvwp.h*/

