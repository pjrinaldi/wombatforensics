/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved. */
/*
|   SCC Viewer Technology  X Windows Implementation
|
|   Include File Xsccvw.h (SCC Viewer Widget Public Header File)
|
|   Outside In
*/


#ifndef xsccvw_h
#define xsccvw_h

#ifndef UNIX
#define UNIX
#endif

#ifndef XWINDOWS
#define XWINDOWS
#endif

#include "sccvw.h"

#ifdef __cplusplus
extern "C" {
#endif

extern WidgetClass sccViewerWidgetClass;
typedef struct _SccViewerClassRec   * SccViewerWidgetClass;
typedef struct _SccViewerRec        * SccViewerWidget;

#ifndef XtIsSccViewerWidget
#define XtIsSccViewerWidget(w) XtIsSubclass((w), sccViewerWidgetClass)
#endif

/* Resource Definition */
#define XtNsccViewerScrollBarWidth "sccViewerScrollBarWidth"
#define XtNsccViewerHScrollBar "sccViewerHorizontalScrollBar"
#define XtNsccViewerVScrollBar "sccViewerVerticalScrollBar"
#define XtNsccViewerViewWindow "sccViewerViewWindow"
#define XtNsccViewerMessageFont "sccViewerMessageFont"
#define XtNsccViewerCallback "sccViewerCallback"

#define XtCSccViewerScrollBarWidth "SccViewerScrollBarWidth"
#define XtCSccViewerHScrollBar "SccViewerHorizontalScrollBar"
#define XtCSccViewerVScrollBar "SccViewerVerticalScrollBar"
#define XtCSccViewerViewWindow "SccViewerViewWindow"
#define XtCSccViewerMessageFont "SccViewerMessageFont"
#define XtCSccViewerCallback "SccViewerCallback"

/* Public Function Declaration */
extern long XsccViewerAcceptMessage(
        Widget sccViewer,
        VTDWORD dwMessage,
        VTSYSVAL wParam,
        VTSYSVAL lParam
        );

typedef struct _SccViewerCallbackStruct
    {
    int reason;
    unsigned long dwMessage;
    unsigned long wParam;
    unsigned long lParam;
    long        lAppReturn;
    } SccViewerCallbackStruct, *SccViewerCallbackPtr;

#ifdef __cplusplus
}  /* Close scope of 'extern "C"' declaration which encloses file. */
#endif

#endif  /* sccvw.h */
