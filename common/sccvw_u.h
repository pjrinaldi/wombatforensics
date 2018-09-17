/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved. */
/*
|   SCC Viewer Technology - Include
|
|   Include:       sccvw_u.h (included in SCCVW.H)
|   Environment:    UNIX
|   Function:     UNIX specific definitions for Viewer Technology Specification 4.0
|
*/


#define MAX_PATH 256

typedef unsigned long (* SCCVWFUNC)(void *,unsigned long,unsigned long,unsigned long);

    /*
    |   Size defines
    */


#define SCCVW_START     2300
    /*
    |   Possible values for dwType in SCCVWDISPLAYINFO
    */


#define SCCVWTYPE_NONE      1       /* no file open in this view */
#define SCCVWTYPE_UNKNOWN   2       /* unknown section type */
#define SCCVWTYPE_WP        3       /* word processor section */
#define SCCVWTYPE_SS        4       /* spreadsheet section */
#define SCCVWTYPE_DB        5       /* database section */
#define SCCVWTYPE_HEX       6       /* hex view of any file */
#define SCCVWTYPE_IMAGE     7       /* bitmap image */
#define SCCVWTYPE_ARCHIVE   8       /* archive */
#define SCCVWTYPE_VECTOR    9       /* bitmap image */
#define SCCVWTYPE_SOUND     10      /* Sound file */

  /*
  |
  |  SCCVWDRAWPAGE40 structure
  |
  */
#ifdef XWINDOWS
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#endif

typedef struct SCCVWDRAWPAGE40tag
  {
  VTDWORD    dwSize;
  VTDWORD    dwPageToDraw;
  VTDWORD    dwReserved;
  VTDWORD    dwFlags;
  VTLONG     lUnitsPerInch;
  VTLONG     lFormatWidth;
  VTLONG     lFormatHeight;
  VTLONG     lTop;
  VTLONG     lLeft;
  VTLONG     lBottom;
  VTLONG     lRight;
  VTLONG     lResultTop;
  VTLONG     lResultLeft;
  VTLONG     lResultBottom;
  VTLONG     lResultRight;
#ifdef XWINDOWS
  GC         gc;
#endif
  } SCCVWDRAWPAGE40, * PSCCVWDRAWPAGE40;

  /*
  |
  |  SCCVWDRAWPAGE41 structure
  |
  */

typedef struct SCCVWDRAWPAGE41tag
  {
  VTDWORD    dwSize;
  VTDWORD    dwPageToDraw;
  VTDWORD    dwReserved;
  VTDWORD    dwFlags;
  VTLONG     lUnitsPerInch;
  VTLONG     lFormatWidth;
  VTLONG     lFormatHeight;
  VTLONG     lTop;
  VTLONG     lLeft;
  VTLONG     lBottom;
  VTLONG     lRight;
  VTLONG     lResultTop;
  VTLONG     lResultLeft;
  VTLONG     lResultBottom;
  VTLONG     lResultRight;
#ifdef XWINDOWS
  GC         gc;
  Drawable   drawable;
#endif
  VTHANDLE   hFormatDC;     /* Must be NULL */
  VTHANDLE   hOutputDC;     /* Must be NULL */
  VTHPALETTE hPalette;
  } SCCVWDRAWPAGE41, * PSCCVWDRAWPAGE41;

#define SCCVW_DPFLAG_RETURNPALETTE          0x0001
#define SCCVW_DPFLAG_NOTMETAFILE            0x0002
#define SCCVW_DPFLAG_IGNOREBACKGROUND       0x0004
#define SCCVW_DPFLAG_DETERMINEOUTPUTTYPE    0x0008

  /*
  |
  |  SCCVWDRAWPAGEINFO structure
  |
  */

typedef struct SCCVWDRAWPAGEINFOtag
  {
  VTDWORD    dwSize;
  VTDWORD    dwPageToDraw;
  VTDWORD    dwReserved;
  VTDWORD    dwFlags;
  VTLONG     lUnitsPerInch;
  VTLONG     lFormatWidth;
  VTLONG     lFormatHeight;
  VTLONG	 lTableWidth;
  VTLONG     lDocumentHeight;
#ifdef XWINDOWS
  GC         gc;
  Drawable   drawable;
#endif
  VTHANDLE   hOutputDC;     /* Must be NULL */
  VTHANDLE   hFormatDC;     /* Must be NULL */
  } SCCVWDRAWPAGEINFO, * PSCCVWDRAWPAGEINFO;



 /*
  |
  |  SCCVWPAGECOUNT structure
  |
  */

typedef struct SCCVWPAGECOUNTtag
  {
  VTDWORD    dwSize;
  VTDWORD    dwNumPages;  
  VTDWORD    dwFlags;
  VTLONG     lUnitsPerInch;
  VTLONG     lFormatWidth;
  VTLONG     lFormatHeight;
  VTLONG     lTop;
  VTLONG     lLeft;
  VTLONG     lBottom;
  VTLONG     lRight;  
  VTHANDLE   hFormatDC;     /* Must be NULL */
#ifdef XWINDOWS
  GC         gc;
  Drawable   drawable;
#endif 
  } SCCVWPAGECOUNT, * PSCCVWPAGECOUNT;





typedef struct tagSCCMenuItem   /*Used for call back in Context menu selection! */
    {
    VTHANDLE pViewInfo;
    HOPTIONSET hOptions;
    VTDWORD dwId;
#ifdef XWINDOWS
    Widget  itemWidget;
#endif
    } SCCVWMENUITEM, *PSCCVWMENUITEM;

#define SCC_MAX_NUM_MENUITEM        20


typedef struct SCCVWPRINTEX60tag
  {
  VTDWORD   dwSize;
  VTDWORD   dwFlags;
  VTBYTE    szPrinter[128];
  VTBYTE    szPort[128];
  VTBYTE    szDriver[128];
  VTBOOL    bDoAbortDialog;
  VTBOOL    bPrintHeader;
  VTDWORD   dwTopMargin;
  VTDWORD   dwBottomMargin;
  VTDWORD   dwLeftMargin;
  VTDWORD   dwRightMargin;
  VTBYTE    szDefaultFont[32];
  VTWORD    wDefaultFontSize;    /* in twips */
  VTSYSVAL  pAbortProc;
  VTDWORD   dwCopies;
  } SCCVWPRINTEX60, * LPSCCVWPRINTEX60;

#define SCCVW_USEPRINTERDC            0x00000001
#define SCCVW_USEPRINTERNAME          0x00000002
#define SCCVW_USEPRINTSELECTIONONLY   0x00000004
#define SCCVW_USEJOBNAME              0x00000008
#define SCCVW_USEMARGINS              0x00000010
#define SCCVW_USEPRINTHEADER          0x00000020
#define SCCVW_USEDEFAULTFONT          0x00000040
#define SCCVW_USEABORTPROC            0x00000080
#define SCCVW_DONTDISABLEPARENTS      0x00000100 /* no longer used */
#define SCCVW_USECOLLATE              0x00000200
#define SCCVW_USECOPIES               0x00000400
#define SCCVW_USEDEVMODE              0x00000800
