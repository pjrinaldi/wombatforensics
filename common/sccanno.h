/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved. */
/*
 |  SCCANNO.H
 |
 |  Annotation definitions
 */

#if !defined(_SCCANNO_H_)
#define _SCCANNO_H_

#include <scctype.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*
 | SCCPOS  structure
 */

typedef struct SCCPOStag
{
    VTDWORD         dwChangeId;
    VTDWORD         dwExtraId;
    VTDWORD         dwData1;
    VTDWORD         dwData2;
} SCCPOS, *PSCCPOS;

/*
 |  SCCAREAPOS structure
 |  Indicates a rectangular area in a document.
 |  For spreadsheets and databases this will be a range of cells.
 */

typedef struct SCCAREAPOStag
{
    VTDWORD   dwSection;   /* 0-based number of sheet/image/slide */
    VTDWORD   dwLeft;      /* Left coordinate or column */
    VTDWORD   dwTop;       /* Top cooridinate or row */
    VTDWORD   dwWidth;     /* Width or number of columns */
    VTDWORD   dwHeight;    /* Height or number of rows */
                           /* The next 4 are only valid for redactions when dwUnits are SCCANNO_UNITS_PCT*/
    VTDOUBLE  dTop;        /* Top of redaction in percentage of parent embedding */
    VTDOUBLE  dBottom;     /* Bottom of redaction in percentage of parent embedding */
    VTDOUBLE  dLeft;       /* Left... */
    VTDOUBLE  dRight;      /* Right... */
    VTDWORD   dwUnits;     /* Unit type: SCCANNO_UNITS_PIXELS, SCCANNO_UNITS_TWIPS, SCCANNO_UNITS_CELLS, SCCANNO_UNITS_PCT */
} SCCAREAPOS, *PSCCAREAPOS;

/* Unit types for the SCCAREAPOS struct */
#define SCCANNO_UNITS_PIXELS    1
#define SCCANNO_UNITS_TWIPS     2
#define SCCANNO_UNITS_CELLS     3
#define SCCANNO_UNITS_PCT       4

/*
 |  Color ref
 */

typedef   VTDWORD      SCCANNOCOLORREF;

/*
 | Macro used to set SCCANNOCOLORREF structure
 */

#define SCCANNORGB(r,g,b)    ((SCCANNOCOLORREF)(((VTBYTE)(r)|((VTDWORD)(VTBYTE)(g)<<8))|(((VTDWORD)(VTBYTE)(b))<<16)))
#define SCCANNORGBA(r,g,b,a) ((SCCANNOCOLORREF)(((VTBYTE)(r)|((VTDWORD)(VTBYTE)(g)<<8))|(((VTDWORD)(VTBYTE)(b))<<16))|(((VTDWORD)(VTBYTE)(a))<<24))

/*
|  Annotation types
*/

#define SCCANNO_HILITETEXT        1
#define SCCANNO_INSERTICON        2
#define SCCANNO_HIDETEXT          3
#define SCCANNO_INSERTBITMAP      4
#define SCCANNO_INSERTTEXT        5
#define SCCANNO_INSERTPARABREAK   6
#define SCCANNO_HIDEPARABREAK     7
#define SCCANNO_HILITEAREA        8
#define SCCANNO_IMAGESTAMP        9

/*
|
|  SCCANNOHIDETEXT structure
|
*/

typedef struct SCCANNOHIDETEXT40tag
{
    VTDWORD   dwSize;
    VTDWORD   dwUser;
    SCCPOS    dwStartPos;
    SCCPOS    dwEndPos;
} SCCANNOHIDETEXT40, * PSCCANNOHIDETEXT40;

typedef struct SCCANNOHIDETEXT41tag
{
    VTDWORD   dwSize;
    VTDWORD   dwUser;
    SCCPOS    sStartPos;
    SCCPOS    sEndPos;
    VTDWORD   dwFiller1;
    VTDWORD   dwData;
} SCCANNOHIDETEXT41, * PSCCANNOHIDETEXT41;

typedef struct SCCANNOHIDETEXT80tag
{
    VTDWORD    dwSize;       /* sizeof(SCCANNOHIDETEXT80) */
    VTDWORD    dwUser;
    SCCPOS     sStartPos;    /* Start position of the annotation */
    SCCPOS     sEndPos;      /* End position of the annotation */
    VTDWORD    dwData;
} SCCANNOHIDETEXT80, * PSCCANNOHIDETEXT80;

typedef struct SCCANNOHIDETEXT81tag
{
    VTDWORD      dwSize;       /* sizeof(SCCANNOHIDETEXT81) */
    VTDWORD      dwUser;
    SCCPOS       sStartPos;    /* Start position of the annotation */
    SCCPOS       sEndPos;      /* End position of the annotation */
    VTSYSPARAM   dwData;
} SCCANNOHIDETEXT81, *PSCCANNOHIDETEXT81;

/*
|
|  SCCANNOHILITETEXT structure
|
*/

typedef struct SCCANNOHILITETEXT40tag
{
    VTDWORD   dwSize;
    VTDWORD   dwUser;
    SCCPOS    dwStartPos;
    SCCPOS    dwEndPos;
    VTDWORD   dwInteraction;
    VTDWORD   dwDisplay;
} SCCANNOHILITETEXT40, * PSCCANNOHILITETEXT40;

typedef struct SCCANNOHILITETEXT41tag
{
    VTDWORD   dwSize;
    VTDWORD   dwUser;
    SCCPOS    sStartPos;
    SCCPOS    sEndPos;
    VTDWORD   dwInteraction;
    VTDWORD   dwData;
    VTDWORD   dwDummy1;
    VTDWORD   dwDummy2;
    VTDWORD   dwDisplay;
} SCCANNOHILITETEXT41, * PSCCANNOHILITETEXT41;

typedef struct SCCANNOHILITETEXT81tag
{
    VTDWORD      dwSize;
    VTDWORD      dwUser;
    SCCPOS       sStartPos;
    SCCPOS       sEndPos;
    VTDWORD      dwInteraction;
    VTSYSPARAM   dwData;
    VTDWORD      dwDummy1;
    VTDWORD      dwDummy2;
    VTDWORD      dwDisplay;
} SCCANNOHILITETEXT81, * PSCCANNOHILITETEXT81;

typedef struct SCCANNOHILITETEXT85tag
{
    VTDWORD      dwSize;
    VTDWORD      dwUser;
    SCCPOS       sStartPos;
    SCCPOS       sEndPos;
    VTDWORD      dwInteraction;
    VTSYSPARAM   dwData;
    VTDWORD      dwDummy1;
    VTDWORD      dwDummy2;
    VTDWORD      dwDisplay;
    VTDWORD      dwAnchorId;
} SCCANNOHILITETEXT85, * PSCCANNOHILITETEXT85;

typedef struct SCCANNOHILITETEXT852tag
{
    VTDWORD      dwSize;
    VTDWORD      dwUser;
    SCCPOS       sStartPos;
    SCCPOS       sEndPos;
    VTDWORD      dwInteraction;
    VTSYSPARAM   dwData;
    VTDWORD      dwDummy1;
    VTDWORD      dwDummy2;
    VTDWORD      dwDisplay;
    VTDWORD      dwAnchorId;
    VTSYSPARAM   label; /* Currently used for redaction labels.
                           This will be passed in through the SDK as a UTF-16
                           string (VTWCHAR *), but will be converted to a
                           CPPUtil wide string (WString *) for internal use. */
} SCCANNOHILITETEXT852, *PSCCANNOHILITETEXT852;

/*
|
|  SCCANNOINSERTICON40 structure
|
*/

typedef struct SCCANNOINSERTICON40tag
{
    VTDWORD    dwSize;
    VTDWORD    dwUser;
    VTDWORD    dwIconPos;
    VTDWORD    dwIconEnd;
    VTDWORD    dwInteraction;
    VTHANDLE   hIcon;
} SCCANNOINSERTICON40, * PSCCANNOINSERTICON40;

typedef struct SCCANNOINSERTICON41tag
{
    VTDWORD    dwSize;
    VTDWORD    dwUser;
    SCCPOS     sIconPos;
    SCCPOS     sIconEnd;
    VTDWORD    dwInteraction;
    VTDWORD    dwData;
    VTDWORD    dwDummy1;
    VTDWORD    dwDummy2;
    VTHANDLE   hIcon;
} SCCANNOINSERTICON41, * PSCCANNOINSERTICON41;

typedef struct SCCANNOINSERTICON81tag
{
    VTDWORD      dwSize;
    VTDWORD      dwUser;
    SCCPOS       sIconPos;
    SCCPOS       sIconEnd;
    VTDWORD      dwInteraction;
    VTSYSPARAM   dwData;
    VTDWORD      dwDummy1;
    VTDWORD      dwDummy2;
    VTHANDLE     hIcon;
} SCCANNOINSERTICON81, * PSCCANNOINSERTICON81;

/*
|
|  SCCANNOINSERTBITMAP42 structure
|
*/

typedef struct SCCANNOINSERTBITMAP42tag
{
    VTDWORD    dwSize;
    VTDWORD    dwUser;
    SCCPOS     sBitmapPos;
    SCCPOS     sBitmapEnd;
    VTDWORD    dwInteraction;
    VTDWORD    dwData;
    VTDWORD    dwDummy1;
    VTDWORD    dwDummy2;
    VTHANDLE   hBitmap;
} SCCANNOINSERTBITMAP42, * PSCCANNOINSERTBITMAP42;

typedef struct SCCANNOINSERTBITMAP81tag
{
    VTDWORD      dwSize;
    VTDWORD      dwUser;
    SCCPOS       sBitmapPos;
    SCCPOS       sBitmapEnd;
    VTDWORD      dwInteraction;
    VTSYSPARAM   dwData;
    VTDWORD      dwDummy1;
    VTDWORD      dwDummy2;
    VTHANDLE     hBitmap;
} SCCANNOINSERTBITMAP81, * PSCCANNOINSERTBITMAP81;

/*
|
|  SCCANNOINSERTTEXT80 structure
|
*/

#define INSERTTEXTALLOCGRAN   64

typedef struct SCCANNOINSERTTEXT80tag
{
    VTDWORD    dwSize;
    VTDWORD    dwUser;
    SCCPOS     sTextPos;
    VTDWORD    dwData;
    VTLPWORD   pText;
    VTDWORD    dwLength;
} SCCANNOINSERTTEXT80, * PSCCANNOINSERTTEXT80;

typedef struct SCCANNOINSERTTEXT81tag
{
    VTDWORD      dwSize;
    VTDWORD      dwUser;
    SCCPOS       sTextPos;
    VTSYSPARAM   dwData;
    VTLPWORD     pText;
    VTDWORD      dwLength;
} SCCANNOINSERTTEXT81, * PSCCANNOINSERTTEXT81;

/*
|
|  SCCANNOINSERTPARABREAK80 structure
|
*/

typedef struct SCCANNOINSERTPARABREAK80tag
{
    VTDWORD    dwSize;
    VTDWORD    dwUser;
    SCCPOS     sParaPos;
    VTDWORD    dwData;
} SCCANNOINSERTPARABREAK80, * PSCCANNOINSERTPARABREAK80;

typedef struct SCCANNOINSERTPARABREAK81tag
{
    VTDWORD      dwSize;
    VTDWORD      dwUser;
    SCCPOS       sParaPos;
    VTSYSPARAM   dwData;
} SCCANNOINSERTPARABREAK81, * PSCCANNOINSERTPARABREAK81;

/*
|
|  SCCANNOHIDEPARABREAK80 structure
|
*/

typedef struct SCCANNOHIDEPARABREAK80tag
{
    VTDWORD    dwSize;
    VTDWORD    dwUser;
    SCCPOS     sParaPos;
    VTDWORD    dwData;
} SCCANNOHIDEPARABREAK80, * PSCCANNOHIDEPARABREAK80;

typedef struct SCCANNOHIDEPARABREAK81tag
{
    VTDWORD      dwSize;
    VTDWORD      dwUser;
    SCCPOS       sParaPos;
    VTSYSPARAM   dwData;
} SCCANNOHIDEPARABREAK81, * PSCCANNOHIDEPARABREAK81;

/*
 |
 |  SCCANNOHILITEAREA structure
 |  This is an area-indexed annotation type.
 |
 */

typedef struct SCCANNOHILITEAREAtag
{
    /* Common area annotation fields */
    VTDWORD           dwSize;
    VTDWORD           dwUser;
    SCCAREAPOS        sAreaPos;
    VTDWORD           dwType;          /* Annotation type (SCCANNO_HILITEAREA) */
    VTDWORD           dwDummy;
    VTDWORD           dwInteraction;
    VTDWORD           dwData;
    VTDWORD           dwAnchorId;

    /* Fields specific to area highlights */
    SCCANNOCOLORREF   fillColor;
    VTDOUBLE          fillColorAlpha;

    /* Border colors and thicknesses */
    SCCANNOCOLORREF   borderTopColor;
    VTDWORD           borderTopThickness;
    SCCANNOCOLORREF   borderLeftColor;
    VTDWORD           borderLeftThickness;
    SCCANNOCOLORREF   borderBottomColor;
    VTDWORD           borderBottomThickness;
    SCCANNOCOLORREF   borderRightColor;
    VTDWORD           borderRightThickness;
    VTDWORD           borderStyle; /* SCCANNO_BORDER_SOLID, SCCANNO_BORDER_DOT, SCCANNO_BORDER_DASH */
                                   /* We currently only support SCCANNO_BORDER_SOLID. */
} SCCANNOHILITEAREA, *PSCCANNOHILITEAREA;

typedef struct SCCANNOHILITEAREA852tag
{
    /* Common area annotation fields */
    VTDWORD           dwSize;
    VTDWORD           dwUser;
    SCCAREAPOS        sAreaPos;
    VTDWORD           dwType;          /* Annotation type (SCCANNO_HILITEAREA) */
    VTDWORD           dwFlags;         /* Currently only supports SCCANNO_REDACTION */
    VTDWORD           dwInteraction;
    VTDWORD           dwData;
    VTDWORD           dwAnchorId;

    /* Fields specific to area highlights */
    SCCANNOCOLORREF   fillColor;
    VTDOUBLE          fillColorAlpha;
    VTDWORD           dwEmbedId;

    /* Border colors and thicknesses */
    SCCANNOCOLORREF   borderTopColor;
    VTDWORD           borderTopThickness;
    SCCANNOCOLORREF   borderLeftColor;
    VTDWORD           borderLeftThickness;
    SCCANNOCOLORREF   borderBottomColor;
    VTDWORD           borderBottomThickness;
    SCCANNOCOLORREF   borderRightColor;
    VTDWORD           borderRightThickness;
    VTDWORD           borderStyle; /* SCCANNO_BORDER_SOLID, SCCANNO_BORDER_DOT, SCCANNO_BORDER_DASH */
                                   /* We currently only support SCCANNO_BORDER_SOLID. */

    /* Label (currently only used for redactions). 
       This will be passed in through the SDK as a UTF-16 string (VTWCHAR *), but will
       be converted to a CPPUtil wide string (WString *) for internal use. */
    VTSYSPARAM        label;
} SCCANNOHILITEAREA852, *PSCCANNOHILITEAREA852;

/* Border style values. Currently only SCCANNO_BORDER_SOLID is supported. */
#define SCCANNO_BORDER_NONE     0
#define SCCANNO_BORDER_SOLID    1
#define SCCANNO_BORDER_DOT      2
#define SCCANNO_BORDER_DASH     3

/*
 |
 |  SCCANNOIMAGESTAMP structure
 |  This is an area-indexed annotation type.
 |
 */

typedef struct SCCANNOIMAGESTAMPtag
{
    /* Common area annotation fields */
    VTDWORD           dwSize;
    VTDWORD           dwUser;
    SCCAREAPOS        sAreaPos;
    VTDWORD           dwType;       /* Annotation type (SCCANNO_IMAGESTAMP) */
    VTDWORD           dwDummy;
    VTDWORD           dwInteraction;
    VTDWORD           dwData;
    VTDWORD           dwAnchorId;

    /* Fields specific to image stamp annotations */
    VTFLOAT           fOpacity;     /* 0 -> invisible; 1.0 -> opaque */
    VTDWORD           dwSizeMode;   /* SCCANNO_SIZE_FIT, SCCANNO_SIZE_STRETCH, or SCCANNO_SIZE_FROM_SOURCE */
    VTLPCSTR          szStampName;  /* NULL-terminated stamp name */
} SCCANNOIMAGESTAMP, *PSCCANNOIMAGESTAMP;

/* Values for SCCANNOIMAGESTAMP's dwSizeMode field */
#define SCCANNO_SIZE_FIT            1
#define SCCANNO_SIZE_STRETCH        2
#define SCCANNO_SIZE_FROM_SOURCE    3

/*
|
|  SCCANNOGEN structure
|
*/

typedef struct SCCANNOGENtag
{
    VTDWORD      dwSize;
    VTDWORD      dwUser;
    SCCPOS       sStartPos;
    SCCPOS       sEndPos;
    VTDWORD      dwInteraction;
    VTSYSPARAM   dwData;
    VTDWORD      dwFlags;
    VTDWORD      dwCookieId;
} SCCANNOGEN, * PSCCANNOGEN;

/*
 |  SCCAREAANNOGEN struct
 |  "Superclass" for the area-indexed annotation types.
 |  We need to keep these fields in sync for all area annotation structs.
 */

typedef struct SCCAREAANNOGENtag
{
    VTDWORD      dwSize;
    VTDWORD      dwUser;
    SCCAREAPOS   sAreaPos;
    VTDWORD      dwType;
    VTDWORD      dwDummy;
    VTDWORD      dwInteraction;
    VTDWORD      dwData;
    VTDWORD      dwAnchorId;
} SCCAREAANNOGEN, *PSCCAREAANNOGEN;

/* Flags that can be specified in the dwFlags portion of annotations <XXX>80 and higher. */
#define SCCANNOUNDONE   0x00000001  /* This annotation is marked as undone. */

   /*
   | SCCANNOHILITESTYLE structure
   */

typedef struct SCCANNOHILITESTYLE41tag
{
    VTDWORD           dwSize;
    VTDWORD           dwStyleId;
    VTDWORD           dwOptions;
    SCCANNOCOLORREF   sForeground;
    SCCANNOCOLORREF   sBackground;
    VTWORD            wCharAttr;
} SCCANNOHILITESTYLE41, * PSCCANNOHILITESTYLE41;

typedef struct SCCANNOHILITESTYLE81tag
{
    VTDWORD           dwSize;
    VTDWORD           dwStyleId;
    VTDWORD           dwOptions;
    SCCANNOCOLORREF   sForeground;
    SCCANNOCOLORREF   sBackground;
    VTWORD            wCharAttr;
    VTWORD            wCharAttrMask; /* Only apply attributes for which
                                        corresponding bits are set in the
                                        mask. */
    SCCANNOCOLORREF   sUnderline;  
} SCCANNOHILITESTYLE81, * PSCCANNOHILITESTYLE81;

/* dwOptions of SCCCVWHILITESTYLE */
/* Note: SCCANNO_REDACTION (0x80000000) can also be set here. */
#define SCCANNO_USEFOREGROUND      0x00000001
#define SCCANNO_USEBACKGROUND      0x00000002
#define SCCANNO_USECHARATTR        0x00000004
#define SCCANNO_USEUNDERLINECOLOR  0x00000008
#define SCCANNO_USETHINUNDERLINE   0x00000010

/*
|
|  SCCANNOTATIONEVENT structure
|
*/

typedef struct SCCANNOTATIONEVENT40tag
{
    VTDWORD  dwSize;
    VTDWORD  dwEvent;
    VTDWORD  dwUser;
} SCCANNOTATIONEVENT40, *PSCCANNOTATIONEVENT40;

typedef struct SCCANNOTATIONEVENT41tag
{
    VTDWORD  dwSize;
    VTDWORD  dwEvent;
    VTDWORD  dwUser;
    VTSYSPARAM dwData;
} SCCANNOTATIONEVENT41, *PSCCANNOTATIONEVENT41;

#define SCCANNO_EVENTSINGLECLICK      0x0001
#define SCCANNO_EVENTDOUBLECLICK      0x0002
#define SCCANNO_EVENTTRANSITIONINTO   0x0004
#define SCCANNO_EVENTTRANSITIONOUTOF  0x0008
#define SCCANNO_EVENTTRANSITION       0x0010   /* Reserved for internal SCC use */
#define SCCANNO_EVENTSINGLERIGHTCLICK 0x0020


/*
|  dwDisplay values for SCCANNOHILITETEXT and SCCANNOHILITEAREA
*/

#define SCCANNO_BDEFAULT       0x10000000
#define SCCANNO_FDEFAULT       0x20000000
#define SCCANNO_USESTYLE       0x40000000
#define SCCANNO_REDACTION      0X80000000

#define SCCANNO_BBLACK         0x00000000
#define SCCANNO_BDARKRED       0x00000001
#define SCCANNO_BDARKGREEN     0x00000002
#define SCCANNO_BDARKYELLOW    0x00000003
#define SCCANNO_BDARKBLUE      0x00000004
#define SCCANNO_BDARKMAGENTA   0x00000005
#define SCCANNO_BDARKCYAN      0x00000006
#define SCCANNO_BLIGHTGRAY     0x00000007
#define SCCANNO_BGRAY          0x00000008
#define SCCANNO_BRED           0x00000009
#define SCCANNO_BGREEN         0x0000000A
#define SCCANNO_BYELLOW        0x0000000B
#define SCCANNO_BBLUE          0x0000000C
#define SCCANNO_BMAGENTA       0x0000000D
#define SCCANNO_BCYAN          0x0000000E
#define SCCANNO_BWHITE         0x0000000F

#define SCCANNO_FBLACK         0x00000000
#define SCCANNO_FDARKRED       0x00000010
#define SCCANNO_FDARKGREEN     0x00000020
#define SCCANNO_FDARKYELLOW    0x00000030
#define SCCANNO_FDARKBLUE      0x00000040
#define SCCANNO_FDARKMAGENTA   0x00000050
#define SCCANNO_FDARKCYAN      0x00000060
#define SCCANNO_FLIGHTGRAY     0x00000070
#define SCCANNO_FGRAY          0x00000080
#define SCCANNO_FRED           0x00000090
#define SCCANNO_FGREEN         0x000000A0
#define SCCANNO_FYELLOW        0x000000B0
#define SCCANNO_FBLUE          0x000000C0
#define SCCANNO_FMAGENTA       0x000000D0
#define SCCANNO_FCYAN          0x000000E0
#define SCCANNO_FWHITE         0x000000F0

/*
| SCCANNOFINDANNOTATION41 structure
*/

typedef struct SCCANNOFINDANNOTATION41tag
{
    VTDWORD    dwSize;
    VTDWORD    dwFindOptions;
    VTDWORD    dwUserTest;
    SCCPOS     sSearchPos;
    VTDWORD    dwUserResult;
    SCCPOS     sStartPos;
    SCCPOS     sEndPos;
    VTSYSPARAM dwData;
} SCCANNOFINDANNOTATION41, *PSCCANNOFINDANNOTATION41;

/*
| dwFindOptions for SCCANNOFINDANNOTATION
| note: can be or'ed with SCCANNO_ABSOLUTE or SCCANNO_MASK
*/

#define  SCCANNO_FINDFIRST          1
#define  SCCANNO_FINDLAST           2
#define  SCCANNO_FINDPREV           3
#define  SCCANNO_FINDNEXT           4
#define  SCCANNO_FINDFIRSTFROMPOS   5
#define  SCCANNO_LASTFINDFLAG       5

#define  SCCANNO_MASK               0x1000
#define  SCCANNO_ABSOLUTE           0x2000

/*
 |  dwUser flags for internal annotations (bookmarks, hyperlinks, etc.)
 */

/*
|   Bad design! No supper for you!
|   The use of these bits (OR'ed with the ACC)
|   (e.g. vwanno.cpp:2460)
|   insure that the size of a file being searched
|   cannot exceed 1MB (previous version) or 32MB (this version)
|   Compacting the bits until this can be fixed for real.
|   Temporary fix for BUG 6295451  jrw  9/13/07
*/

#define SCCANNO_USERBOOKMARKTAG        0x82000000
#define SCCANNO_USERHYPERTAG           0x84000000
#define SCCANNO_USERVWSEARCHTAG        0x88000000
#define SCCANNO_USERSEARCHTAG          0x90000000
#define SCCANNO_USERPAGETAG            0xa0000000
#define SCCANNO_USEROBJECTTAG          0xc0000000

#define SCCANNO_LOOKINGFORNONTEXTTAG   0x80000000  /* Any of the above. */

/*
| Internal use of dwDisplay
*/
#define SCCANNO_STYLESEARCHTAG    0x00001000
#define SCCANNO_STYLEOBJECTTAG    0x00001001
#define SCCANNO_STYLEHYPERTAG     0x00001002
#define SCCANNO_STYLEBOOKMARKTAG  0x00001004


#if defined(__cplusplus)
}
#endif

#endif /* _SCCANNO_H_ */
