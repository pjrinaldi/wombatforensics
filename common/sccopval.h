/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. 
All rights reserved. */
/* |	SCC Viewer Technology - Source file
|
|	File:           sccopval.h
|	Module:         SCC Common Options Model
|	Environment:    Portable
|	Function:       Common Options Model Structures and Macros
*/


#ifndef _SCCOPVAL_H
#define _SCCOPVAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/* charsets.h contains charset values for the */
/* DEFAULTINPUTCHARSET option */
#include "charsets.h"
	
/* SCCOPT_EX_FLAVOR may be one of the following: */
/* HX */
#define SCCEX_FLAVOR_GENERICHTML				20
#define SCCEX_FLAVOR_HTML20 					21
#define SCCEX_FLAVOR_HTML30 					22
#define SCCEX_FLAVOR_HTML40 					23
#define SCCEX_FLAVOR_NS30   					28
#define SCCEX_FLAVOR_NS40   					29
#define SCCEX_FLAVOR_MS30   					33
#define SCCEX_FLAVOR_MS40   					34
/* WX */
#define SCCEX_FLAVOR_WML11  					36
#define SCCEX_FLAVOR_TEXT   					37
#define SCCEX_FLAVOR_XHTMLB1					38
#define SCCEX_FLAVOR_CHTML2 					39
#define SCCEX_FLAVOR_HDML   					40
#define SCCEX_FLAVOR_WCA11  					41
#define SCCEX_FLAVOR_AG33PALM   				42
#define SCCEX_FLAVOR_AG33CE 					43
#define SCCEX_FLAVOR_WGENERICHTML   			44
#define SCCEX_FLAVOR_WML11_TBL  				45
#define SCCEX_FLAVOR_XHTMLB1_NOTBL  			46
#define SCCEX_FLAVOR_WCA11_NOTBL				47
#define SCCEX_FLAVOR_AG33PALM_NOTBL 			48
#define SCCEX_FLAVOR_AG33CE_NOTBL   			49
#define SCCEX_FLAVOR_WML20  					50

/* PAGEML */
#define SCCEX_FLAVOR_PAGEML 					51

/* SEARCHML */
#define SCCEX_FLAVOR_SEARCHML   				52

/* old flavor values for backward compatibility */
#define SCCHTML_FLAVOR_20   					17  		/* renumbered so it doesn't clash with WML - the assumption is */
															/* that no one is using this flavor                            */
#define SCCHTML_FLAVOR_40   					2
#define SCCHTML_FLAVOR_30   					3
#define SCCHTML_FLAVOR_99   					4
#define SCCHTML_FLAVOR_NS11 					5
#define SCCHTML_FLAVOR_NS20 					6
#define SCCHTML_FLAVOR_NS30 					7
#define SCCHTML_FLAVOR_MO21 					8
#define SCCHTML_FLAVOR_MS20 					9
#define SCCHTML_FLAVOR_MS30 					10
#define SCCHTML_FLAVOR_GENERIC  				11
#define SCCHTML_FLAVOR_MS15 					12
#define SCCHTML_FLAVOR_MS40 					13
#define SCCHTML_FLAVOR_MS50 					14
#define SCCHTML_FLAVOR_NS40 					15
#define SCCHTML_FLAVOR_NS46 					16
#define SCCWML_FLAVOR_WML11 					1
#define SCCEX_FLAVOR_MO21   					25
#define SCCEX_FLAVOR_NS11   					26
#define SCCEX_FLAVOR_NS20   					27
#define SCCEX_FLAVOR_NS46   					30
#define SCCEX_FLAVOR_MS15   					31
#define SCCEX_FLAVOR_MS20   					32
#define SCCEX_FLAVOR_MS50   					35

/* SCCOPT_EX_COMPLIANCEFLAGS bit flags */
#define SCCEX_CFLAG_STRICTDTD   				0x0001		/* Set to enforce strict DTD compliance in the output we write  */
#define SCCHTML_FLAG_STRICT_DTD 				0x0001
#define SCCEX_CFLAG_WELLFORMED  				0x0002		/* Set to make HTML well formed                                 */
#define SCCHTML_FLAG_WELLFORMED 				0x0002

/* SCCOPT_EX_LABELFLAGS bit flags */
#define SCCEX_LABELFLAGS_SSDBCELLSOFF   		0x0001		/* Set to turn off labels for spreadsheet and database cells    */
#define SCCWML_LABELFLAGS_SSDBCELLSOFF  		0x0001
#define SCCEX_LABELFLAGS_WPCELLSON  			0x0002		/* Set to label word processing table cells                     */
#define SCCWML_LABELFLAGS_WPCELLSON 			0x0002
#define SCCWML_LABELFLAGS_GENBULLETSANDNUMSOFF  0x0004		/* Set to turn off list number/bullet generation (deprecated)   */

/* Bit Fields for default compare */
#define SCCCOMPARE_DEFAULT_DATETIME 			0x100
#define SCCCOMPARE_DEFAULT_CHARSET  			0x200

/* Values for SCCOPT_GRAPHIC_OUTPUTDPI */
#define SCCGRAPHIC_MAINTAIN_IMAGE_DPI   		0			/* special value indicates DO NOT resize exported image         */
#define SCCGRAPHIC_MAX_SANE_BITMAP_DPI  		2400		/* This define is now deprecated.*/
#define SCCGRAPHIC_DEFAULT_OUTPUT_DPI   		96  		/* Default DPI to use.                                          */

#define SCCGRAPHIC_DEFAULTTRANSPARENCYCOLOR     -1          /*Turns SCCOPT_GRAPHIC_TRANSPARENCYCOLOR option off*/

#define SCCGRAPHIC_NOCROPPING		            -1
#define SCCGRAPHIC_CROPTOCONTENT	             0
    
/* Possible values for the SCCOPT_GRAPHIC_WATERMARK_HORIZONTAL_OFFSETFROM option */
#define SCCOPT_GRAPHIC_WATERMARK_HORIZONTAL_OFFSETFROM_CENTER   0
#define SCCOPT_GRAPHIC_WATERMARK_HORIZONTAL_OFFSETFROM_LEFT     1
#define SCCOPT_GRAPHIC_WATERMARK_HORIZONTAL_OFFSETFROM_RIGHT    2

/* Possible values for the SCCOPT_GRAPHIC_WATERMARK_VERTICAL_OFFSETFROM option */
#define SCCOPT_GRAPHIC_WATERMARK_VERTICAL_OFFSETFROM_CENTER   0
#define SCCOPT_GRAPHIC_WATERMARK_VERTICAL_OFFSETFROM_TOP      1
#define SCCOPT_GRAPHIC_WATERMARK_VERTICAL_OFFSETFROM_BOTTOM   2

/* Possible types for the SCCOPT_GRAPHIC_WATERMARK_SCALETYPE option. */
#define SCCGRAPHIC_WATERMARK_SCALETYPE_NONE				0
#define SCCGRAPHIC_WATERMARK_SCALETYPE_PERCENT			1

/*Values for watermark positions*/
#define SCCGRAPHIC_OFFSETFROMCENTER						1

#define SCCGRAPHIC_INCHES								1
#define SCCGRAPHIC_POINTS								2
#define SCCGRAPHIC_CENTIMETERS							3
#define SCCGRAPHIC_PICAS								4

/*Values for watermark scaling.*/
#define SCCGRAPHIC_NOMAP								0
#define SCCGRAPHIC_FITTOPAGE							1
#define SCCGRAPHIC_SCALE								2

/*Compression type for 24 bit images.*/
#define SCCCMP24BIT_JPEG								1
#define SCCCMP24BIT_JPEG2000							2

/*Font embedding policy type. */
#define SCCFONTS_REDUCESIZE                             0  /* Default */
#define SCCFONTS_EMBEDALL                               1

/* Flags for font permissions */
#define SCCFONTS_DEFAULT_PERMISSIONS                    0x00010000
#define SCCFONTS_REQUIRE_INSTALLABLE                    0x00020000
#define SCCFONTS_REQUIRE_PREVIEW_PRINT                  0x00040000
#define SCCFONTS_REQUIRE_EDITABLE                       0x00080000

/* Font embedding type flags */
#define SCCFONTS_REFERENCE_BY_NAME                      1
#define SCCFONTS_REFERENCE_EXPORTED                     2
#define SCCFONTS_REFERENCE_BY_BASE_URL                  4

/* Redaction Font default size */
#define SCCFONTS_DEFAULT_SIZE                           18

/* Email attachment handling flags */
#define SCCOPT_EXPORT                                   1
#define SCCOPT_EXPORT_RECURSIVE                         2
#define SCCOPT_EXTRACT                                  4
#define SCCOPT_NO_ATTACHMENTS                           0

/* Possible values for the SCCOPT_DOCUMENTMEMORYMODE option... */
#define SCCDOCUMENTMEMORYMODE_SMALLEST  1     /* 4MB  */
#define SCCDOCUMENTMEMORYMODE_SMALL     2     /* 16MB */
#define SCCDOCUMENTMEMORYMODE_MEDIUM    3     /* 64MB */
#define SCCDOCUMENTMEMORYMODE_LARGE     4     /* 256MB */
#define SCCDOCUMENTMEMORYMODE_LARGEST   5     /* 1 GB */
#ifdef ANDROID
#define SCCDOCUMENTMEMORYMODE_DEFAULT   SCCDOCUMENTMEMORYMODE_SMALLEST
#else
#define SCCDOCUMENTMEMORYMODE_DEFAULT   SCCDOCUMENTMEMORYMODE_LARGE
#endif
 
/* Conversion Selection #defines  - Bit Fields - NB 3/17/97 */
#define SCCRAINBOW_CONVERT_TEXT 				0x0100		/* Do Text Conversion                                           */
#define SCCRAINBOW_CONVERT_GRAPHICS 			0x0200		/* Do Graphics Conversion                                       */
#define SCCRAINBOW_CONVERT_EQUATIONS			0x0400		/* Do Equation conversion                                       */

/* SCCOPT_GRAPHIC_SIZEMETHOD may be one of the following: */
#define SCCGRAPHIC_QUICKSIZING  				0			/* Use quick algorithm to size graphics                         */
#define SCCGRAPHIC_SMOOTHSIZING 				1			/* Use smooth algorithm to size graphics                        */
#define SCCGRAPHIC_SMOOTHGRAYSCALESIZING		2			/* Use smooth algorithm for grayscale graphics only             */

/* SCCOPT_EX_CHARBYTEORDER may be one of the following: */
#define SCCEX_CHARBYTEORDER_TEMPLATE			0			/* Use whatever byte-ordering the template is using.            */
															/* If the template is not Unicode, Motorola order will be used  */
#define SCCEX_CHARBYTEORDER_BIGENDIAN			1			/* Use Motorola-ordered Unicode                                 */
#define SCCEX_CHARBYTEORDER_LITTLEENDIAN		2			/* Use Intel-ordered Unicode                                    */
#define SCCOPT_CHARBYTEORDER_TEMPLATE			0			/* Use whatever byte-ordering the template is using.            */
															/* If the template is not Unicode, Motorola order will be used  */
#define SCCOPT_CHARBYTEORDER_BIGENDIAN			1			/* Use Motorola-ordered Unicode                                 */
#define SCCOPT_CHARBYTEORDER_LITTLEENDIAN		2			/* Use Intel-ordered Unicode                                    */

/*  Flags available for SCCOPT_FORMATFLAGS.  */
#define SCCOPT_FLAGS_ISODATETIMES               1           /*  bit 0:  On if we are to format dates and times according to ISO 8601.  */
#define SCCOPT_FLAGS_STRICTFILEACCESS           2           /*  bit 1   On if we only use the full URL or path to access the file */

/* SCCOPT_EX_FALLBACKFONT needs to know what the type of string the font name is */
#define SCCEX_FALLBACKFONT_SINGLEBYTE			0
#define SCCEX_FALLBACKFONT_DOUBLEBYTE			1
#define SCCOPT_EX_FALLBACKFONT_SINGLEBYTE		0
#define SCCOPT_EX_FALLBACKFONT_DOUBLEBYTE		1

/* SCCOPT_EX_CALLBACKS bit flags */
#define SCCEX_CALLBACKFLAG_ALLDISABLED				0x00000000
#define SCCEX_CALLBACKFLAG_CREATENEWFILE			0x00000001
#define SCCEX_CALLBACKFLAG_NEWFILEINFO				0x00000002
#define SCCEX_CALLBACKFLAG_PROCESSLINK				0x00000004
#define SCCEX_CALLBACKFLAG_CUSTOMELEMENT			0x00000008
#define SCCEX_CALLBACKFLAG_GRAPHICEXPORTFAILURE		0x00000010
#define SCCEX_CALLBACKFLAG_OEMOUTPUT				0x00000020
#define SCCEX_CALLBACKFLAG_ALTLINK					0x00000040
#define SCCEX_CALLBACKFLAG_ARCHIVE					0x00000080

/* Add additional callback flags here when needed */
#define SCCEX_CALLBACKFLAG_ALLENABLED				0xFFFFFFFF

/* Special values for SCCGRAPHIC_xxLIMIT */
#define SCCGRAPHIC_NOLIMIT						0			/* No limit - disables the option                               */

/* SCCOPT_EX_GRIDADVANCE may be one of the following: */
#define SCCEX_GRIDADVANCE_ACROSS				0
#define SCCEX_GRIDADVANCE_DOWN					1

/* SCCOPT_EX_FONTFLAGS bit flags */
#define SCCEX_FONTFLAGS_SUPPRESSSIZE			0x00000001	/* Don't write out the size attribute                           */
#define SCCEX_FONTFLAGS_SUPPRESSCOLOR			0x00000002	/* Don't write out the color attribute                          */
#define SCCEX_FONTFLAGS_SUPPRESSFACE			0x00000004	/* Don't write out the font face attribute                      */

/* SCCOPT_EX_LOGFORMAT values */
#define SCCEX_LOGFORMAT_TEXT 1
#define SCCEX_LOGFORMAT_HTML 2

/* Values for email header options */
#define SCCUT_MAIL_ARRAYSIZE       256         /* We currently have 187 different email
                                                   headers defined. This gives some room 
												   for expansion. */
#define SCCUT_MAIL_NAMELENGTH      128         /* Length of the strings in SCCUTEMAILHEADERINFO */
#define MAX_NONSTANDARD_HEADERS    100         /* Per spec, you only get 100 */
#define NONSTANDARD_HEADER_ID_BASE 0
#define NONSTANDARD_HEADER_ID_TOP  (MAX_NONSTANDARD_HEADERS-1)

/* SO_EMAILTABLE will now have a VTDWORD argument that 
    specifies the type of mail document. */

#define SCCUT_MAILTYPE_AVAILABLE_MASK	   0x000000ff
#define SCCUT_MAILTYPE_EMAIL               0x00000001 
#define SCCUT_MAILTYPE_JOURNAL             0x00000002
#define SCCUT_MAILTYPE_CONTACT             0x00000004
#define SCCUT_MAILTYPE_NOTE                0x00000008
#define SCCUT_MAILTYPE_APPOINTMENT         0x00000010
#define SCCUT_MAILTYPE_TASK                0x00000020
#define SCCUT_MAILTYPE_POST                0x00000040
#define SCCUT_MAILTYPE_DISTROLIST          0x00000080

/* Options from the viewer, moved into UT now */
#define SCCUT_WPMODE_DRAFT						1
#define SCCUT_WPMODE_NORMAL						2
#define SCCUT_WPMODE_PREVIEW					3
#define SCCUT_WPMODE_WEBLAYOUT					4

#define SCCUT_SSFLAGS_SHOWGRIDLINES				BIT0
#define SCCUT_SSFLAGS_PRINTGRIDLINES			BIT1
#define SCCUT_SSFLAGS_SHOWHEADINGS				BIT2
#define SCCUT_SSFLAGS_PRINTHEADINGS				BIT3
#define SCCUT_SSFLAGS_DRAFTMODE					BIT4
#define SCCUT_SSFLAGS_SHOWHIDDENCELLS			BIT5

#define SCCUT_CLIPFORMAT_TEXT					0x00000001
#define SCCUT_CLIPFORMAT_RTF					0x00000002
#define SCCUT_CLIPFORMAT_AMI2					0x00000004
#define SCCUT_CLIPFORMAT_AMI					0x00000008
#define SCCUT_CLIPFORMAT_PROWRITE				0x00000010
#define SCCUT_CLIPFORMAT_WORDSTAR				0x00000020
#define SCCUT_CLIPFORMAT_LEGACY					0x00000040
#define SCCUT_CLIPFORMAT_WINBITMAP				0x00000080
#define SCCUT_CLIPFORMAT_WINDIB					0x00000100
#define SCCUT_CLIPFORMAT_WINMETAFILE			0x00000400
#define SCCUT_CLIPFORMAT_WINPALETTE				0x00000200
#define SCCUT_CLIPFORMAT_MACPICT				0x00000800
#define SCCUT_CLIPFORMAT_OS2METAFILE			0x00001000
#define SCCUT_CLIPFORMAT_OS2BITMAP				0x00002000
#define SCCUT_CLIPFORMAT_OS2PALETTE				0x00004000
#define SCCUT_CLIPFORMAT_EPOCBITMAP             0x00008000
#define SCCUT_CLIPFORMAT_UNICODE				0x00010000
#if defined(_DARWIN_SOURCE)
#define SCCUT_CLIPFORMAT_TIFF                   0x00020000
#endif /* _DARWIN_SOURCE */

#define SCCUT_CLIPSUBFORMAT_TABLE				0
#define SCCUT_CLIPSUBFORMAT_OPTIMIZEDTABS		1
#define SCCUT_CLIPSUBFORMAT_TABS				2

#define SCCUT_CLIPINCLUDE_CHARATTR				0x0001
#define SCCUT_CLIPINCLUDE_CHARSIZE				0x0002
#define SCCUT_CLIPINCLUDE_CHARFACE				0x0004
#define SCCUT_CLIPINCLUDE_PARAINDENTALIGN		0x0008
#define SCCUT_CLIPINCLUDE_PARASPACING			0x0010
#define SCCUT_CLIPINCLUDE_TABSTOPS				0x0020
#define SCCUT_CLIPINCLUDE_PAGEBREAKS			0x0040

#define SCCUT_FITMODE_PIXELS					1
#define SCCUT_FITMODE_ORIGINAL					SCCUT_FITMODE_PIXELS
#define SCCUT_FITMODE_WINDOW					2
#define SCCUT_FITMODE_WINDOWHEIGHT				3
#define SCCUT_FITMODE_WINDOWWIDTH				4
#define SCCUT_FITMODE_STRETCHWINDOW				5
#define SCCUT_FITMODE_BEST                      6
#define SCCUT_FITMODE_IMAGESIZE					7

#define SCCUT_FITPOS_UPPERLEFT                  0
#define SCCUT_FITPOS_CENTER                     1

#define SCCUT_ROTATION_NONE						0
#define SCCUT_ROTATION_90						90
#define SCCUT_ROTATION_180						180
#define SCCUT_ROTATION_270						270

#define SCCUT_DITHER_ON							1
#define SCCUT_DITHER_OFF						2

#define SCCUT_PRINTASPECT_SCALE		      1
#define SCCUT_PRINTASPECT_ORIGINAL		  SCCUT_PRINTASPECT_SCALE
#define SCCUT_PRINTASPECT_STRETCH				2
#define SCCUT_PRINTASPECT_IMAGESIZE			3

#define SCCUT_INTLUNITSMASK						0x0001   /* bit 0: 0=metric 1=English */
#define SCCUT_ENGLISHUNITS						0x0001

#define SCCUT_INTL24HOURMASK					0x0002   /* bit 1: 0=24 hour time 1=12 hour time */
#define SCCUT_12HOURTIME						0x0002

#define SCCUT_PRINT_ALLPAGES					1
#define SCCUT_PRINT_SELECTION					2
#define SCCUT_PRINT_PAGERANGE					3
#define SCCUT_PRINT_CURSECTION					4

#define SCCUT_DIALOG_NOHELP						0x0001
#define SCCUT_DIALOG_NOMORE						0x0002
#define SCCUT_DIALOG_NO3D						0x0004
#define SCCUT_DIALOG_NOADDOPTIONSTOMENU			0x0008
#define SCCUT_DIALOG_NOADDDISPLAYTOMENU			0x0010
#define SCCUT_DIALOG_NOADDPRINTTOMENU			0x0020
#define SCCUT_DIALOG_NOADDCLIPBOARDTOMENU		0x0040
#define SCCUT_DIALOG_NOADDDOPRINTTOMENU			0x0080
#define SCCUT_DIALOG_NOADDDOCOPYTOMENU			0x0100
#define SCCUT_DIALOG_NOADDFONTZOOMTOMENU		0x0200

#define SCCUT_SYSTEM_RAWTEXT					0x0001
#define SCCUT_SYSTEM_NOTIMER					0x0002
#define SCCUT_SYSTEM_NOOPTIONSSAVE				0x0004
#define SCCUT_SYSTEM_NOREADAHEAD				0x0008
#define SCCUT_SYSTEM_UNICODE					0x0010
#define SCCUT_SYSTEM_CJKADDON					0x0020
#define SCCUT_SYSTEM_NAMETEMPFILE				0x0100
#define SCCUT_SYSTEM_NDELTMPFILES				0x0200
#define SCCUT_SYSTEM_SEQUENTIALACCESS           0x0400
#define SCCUT_SYSTEM_DISABLESECTIONLIST			0x0800

#define SCCUT_FILTER_LZW_ENABLED				0
#define SCCUT_FILTER_LZW_DISABLED				1

#define SCCUT_FILTER_JPG_ENABLED				0
#define SCCUT_FILTER_JPG_DISABLED				1

#define SCCUT_ANTIALIAS_OFF                     0
#define SCCUT_ANTIALIAS_ALL                     1

#define SCCUT_EMAILVIEW_ENABLED					0
#define SCCUT_EMAILVIEW_DISABLED				1

/* depreciating SCCUT_WP_WPMIMEHEADER*, but leaving it for backward compatibility */
#define SCCUT_WP_MIMEHEADERALL                  0
#define SCCUT_WP_MIMEHEADERSTANDARD             1
#define SCCUT_WP_EMAILHEADERALL                 0
#define SCCUT_WP_EMAILHEADERSTANDARD            1
#define SCCUT_WP_EMAILHEADERNONE				2
#define SCCUT_WP_EMAILHEADERCUSTOM				3

/*
Special option support for CJK add-on system such as NJStar, CStar, TwinBridge
Functional code are handled in dufont.
Not documented in Viewer Spec, because we do not publish this one yet.
rmc 12-23-96
*/

#define SCCUT_OLE_ENABLEDRAGDROP				0x0001

#define SCCUT_ARCHIVE_SAVENOP					0
#define SCCUT_ARCHIVE_SAVESELECTION				1
#define SCCUT_ARCHIVE_SAVEALL					2
#define SCCUT_ARCHIVE_SAVETHIS					3
#define SCCUT_ARCHIVE_VIEWTHIS					4

#define SCCUT_SORT_NONE							1
#define SCCUT_SORT_NAME							2
#define SCCUT_SORT_SIZE							3
#define SCCUT_SORT_DATE							4
#define SCCUT_SORT_DESCENDING					0x80000000

#define SCCUT_ARC_RESTOREPATH					0x0001
#define SCCUT_ARC_HAVEPATH						0x0002

#define SCCUT_ARC_PROMPTONCOLLISION               0x00000001
#define SCCUT_ARC_PROMPTILLEGALCHAR             0x00000010

#define SCCUT_ZOOM_NOP							0
#define SCCUT_ZOOM_IN							1
#define SCCUT_ZOOM_OUT							2
#define SCCUT_ZOOM_SELECTION					3
#define SCCUT_ZOOM_RESET						4

/* Options for SS print SCCID_SSPRINTFITTOPAGE */
#define SCCUT_SSPRINTFITMODE_NOMAP				0
#define SCCUT_SSPRINTFITMODE_FITTOPAGE			1
#define SCCUT_SSPRINTFITMODE_FITTOWIDTH			2
#define SCCUT_SSPRINTFITMODE_FITTOHEIGHT		3
#define SCCUT_SSPRINTFITMODE_SCALE				4
#define SCCUT_SSPRINTFITMODE_FITTOPAGES			5

/* Options for SS print SCCID_SSPRINTDIRECTION */
#define SCCUT_SSPRINTDIRECTION_ACROSS			0
#define SCCUT_SSPRINTDIRECTION_DOWN				1

/* Options for DB print SCCID_DBPRINTFITTOPAGE */
#define SCCUT_DBPRINTFITMODE_NOMAP				0
#define SCCUT_DBPRINTFITMODE_FITTOPAGE			1
#define SCCUT_DBPRINTFITMODE_FITTOWIDTH			2
#define SCCUT_DBPRINTFITMODE_FITTOHEIGHT		3

#define SCCUT_CURRENCY_LEADS					0x0001		/* Currency symbol before the amount    */
#define SCCUT_CURRENCY_TRAILS					0x0000		/* Currency symbol after the amount     */
#define SCCUT_CURRENCY_SPACE					0x0002		/* Space between currency and amount    */
#define SCCUT_CURRENCY_NOSPACE					0x0000		/* No space between currency and amount */

#define SCCUT_DATEORDER_MDY						0
#define SCCUT_DATEORDER_DMY						1
#define SCCUT_DATEORDER_YMD						2

#define SCCUT_SYSTEM_UNKNOWNYEARS              0x0000
#define SCCUT_SYSTEM_2DIGITYEARS               0x0001
#define SCCUT_SYSTEM_4DIGITYEARS               0x0002
#define SCCUT_USE_SYSTEM_YEARS                 0x0004
#define SCCUT_OPT_SHORT_DATE_ORDER             0x0008

/* Flags for SCCID_SCROLLFLAGS */
#define SCCUT_HSCROLL_NEVER						0x0001
#define SCCUT_HSCROLL_SOMETIMES					0x0002
#define SCCUT_HSCROLL_ALWAYS					0x0004
#define SCCUT_VSCROLL_NEVER						0x0008
#define SCCUT_VSCROLL_SOMETIMES					0x0010
#define SCCUT_VSCROLL_ALWAYS					0x0020

/* Values for use with scrolling messages */
#define SCCSB_TOP								1
#define SCCSB_BOTTOM							2
#define SCCSB_LINELEFT							3
#define SCCSB_LINERIGHT							4
#define SCCSB_PAGELEFT							5
#define SCCSB_PAGERIGHT							6

#define SCCSB_LINEUP							3
#define SCCSB_LINEDOWN							4
#define SCCSB_PAGEUP							5
#define SCCSB_PAGEDOWN							6

#define SCCSB_POSITION							7

#define SCCSB_ENABLED							1
#define SCCSB_DISABLED							2



#define SCCUT_REORDER_UNICODE_OFF				0x00000000
#define SCCUT_REORDER_UNICODE_LTOR				0x00000002
#define SCCUT_REORDER_UNICODE_RTOL				0x00000003

/* hsl 20 January 99 --- added for bidirectional text functionality */
#define SCCUT_REORDER_NOVALUE					SCCUT_REORDER_UNICODE_OFF /* do not set an algorithm at all --- this is the default */
#define SCCUT_REORDER_NONE						0x00000001 /* no text is reordered in the display */
#define SCCUT_REORDER_UNICODE					SCCUT_REORDER_UNICODE_LTOR /* use the unicode bidirectional algorithm for reordering */
#define SCCUT_REORDER_FULL						0x00000004 /* mirror each line individually */



/* SCCOPT_FIFLAGS values */
#define SCCUT_FI_NORMAL							0
#define SCCUT_FI_EXTENDEDTEST					1
 
/* Option values for filter performance(SCCOPT_FILTERPERFORMANCE) 
SCCUT_FILTER_NORMALPERFORMANCE  - Filter operates normally.
SCCUT_FILTER_OPTIMIZEDFORTEXT   - Filter skips graphical information, but puts out all other content.*/
#define SCCUT_FILTER_NORMALPERFORMANCE			0
#define SCCUT_FILTER_OPTIMIZEDFORTEXT			1

/* Option values for filter performance(SCCOPT_FILTERDISABLETABLEOUTPUT) 
SCCUT_FILTER_NORMALTABLEOUTPUT  - Filter operates normally.
SCCUT_FILTER_DISABLETABLEOUTPUT - Filter skips table structure(CHANGE_CELLFORMAT, CHANGE_ROWFORMAT, etc), 
                                  but puts out any content found in the table. The text list must be the same
                                  whether or not this option value is set.*/
#define SCCUT_FILTER_NORMALTABLEOUTPUT          0
#define SCCUT_FILTER_DISABLETABLEOUTPUT         1 

#define SCCUT_FILTER_NORMALSUBOBJECTS			0
#define SCCUT_FILTER_ENABLEALLSUBOBJECTS        1

/*Bidi option values.*/
#define SCCUT_FILTER_STANDARD_BIDI				0
#define SCCUT_FILTER_REORDERED_BIDI				1

/*Timezone option values*/
#define SCC_TIMEZONE_USENATIVE                  0xF000
#define SCC_MAX_TIMEZONE                        96 /* time zone settings must be between -96 and 96 */

/*EXIF metadata option values.*/
#define SCCUT_FILTER_EXIFFULL_OFF               0 
#define SCCUT_FILTER_EXIFFULL_ON                1

/* Option values for SCCOPT_PROCESS_OLE_EMBEDDINGS  */

#define SCCOPT_PROCESS_OLEEMBED_STANDARD      0 /* Process only known embedding types */ 
#define SCCOPT_PROCESS_OLEEMBED_ALL           1 /* Process all embeddings in the file */
#define SCCOPT_PROCESS_OLEEMBED_NONE          2 /* Process none of the embeddings in the file */
/* Option values for SCCOPT_EX_SEARCHML_FLAGS */
/*
    These flags work in a bitfield.  Bitfield type options have difficulties
    setting default values because the user may want to override some flags,
    but allow others to default.  Therefore, bitfield options should be
    structured in such a way as to be off by default.  Thus, some options
    suppress behavior when turned on, while other options enable behavior when
    turned on.
*/
#define SCCEX_XML_PSTYLENAMES			    0x00000001  /* ADD paragraph style name reference to p tags */
#define SCCEX_XML_CSTYLENAMES			    0x00000002  /* ADD run elements that include character style name references */
#define SCCEX_XML_EMBEDDINGS			    0x00000004  /* process embeddings */
#define SCCEX_XML_NO_XML_DECLARATION	    0x00000008  /* Don't generate xml declaration */
#define SCCEX_XML_TEXTCOOKIES			    0x00000010
#define SCCEX_IND_SUPPRESSPROPERTIES	    0x00000020  /*  Suppress processing of document properties in all indexing related products.  */
#define SCCEX_IND_GENERATED				    0x00000040  /*  Produce generated text in all indexing related products.  */
#define SCCEX_XML_SUPPRESSATTACHMENTS       0x00000080  /*  Suppress processing of attachments.  */
#define SCCEX_XML_SUPPRESSARCHIVESUBDOCS    0x00000100  /*  Suppress processing of sub-documents in archives.  */
#define SCCEX_METADATAONLY                  0x00000200  /*  Produce only metadata.  */
#define SCCEX_ANNOTATIONS                   0x00000400  /*  Annotation text should be noted as such.  */
#define SCCEX_PRODUCEURLS                   0x00000800  /*  Produce URLs for hyperlinks.  */
#define SCCEX_XML_PRODUCEOBJECTINFO         0x00001000  /*  Produce information allowing for reference of sub-document objects.  */
#define SCCEX_XML_ENABLEERRORINFO           0x00002000  /*  Output sub-document error information.  */
#define SCCEX_IND_SS_CELLINFO               0x00004000  /*  Output spreadsheet row and column information.  */
#define SCCEX_IND_SS_FORMULAS               0x00008000  /*  Output spreadsheet formula information.  */
#define SCCEX_IND_GENERATESYSTEMMETADATA    0x00010000  /*  Generate system metadata */
#define SCCEX_XML_SKIPSTYLES                0x00020000  /*  Skip style information for performance reasons.  This option overrides other style related directives.  */
#define SCCEX_IND_SS_CELLHIDDEN             0x00040000  /* Produce hidden cell attribute */


/*  SearchML character attribute options.  These must match their
    counterparts in CA.  */
#define  SCCEX_XML_SEARCHML_REVISIONDELETE        0x00010000
#define  SCCEX_XML_SEARCHML_BOLD                 0x00020000
#define  SCCEX_XML_SEARCHML_ITALIC               0x00040000
#define  SCCEX_XML_SEARCHML_UNDERLINE            0x00080000
#define  SCCEX_XML_SEARCHML_DUNDERLINE           0x00100000
#define  SCCEX_XML_SEARCHML_OUTLINE              0x00200000
#define  SCCEX_XML_SEARCHML_HIDDEN               0x00400000
#define  SCCEX_XML_SEARCHML_STRIKEOUT            0x00800000
#define  SCCEX_XML_SEARCHML_SMALLCAPS            0x01000000
#define  SCCEX_XML_SEARCHML_ALLCAPS              0x02000000
#define  SCCEX_XML_SEARCHML_OCE                  0x04000000
#define  SCCEX_XML_SEARCHML_REVISIONADD          0x10000000

/*  SearchML paragraph attribute options.  These must match their
    counterparts in CA.  */
#define  SCCEX_XML_SEARCHML_SPACING              0x00010000
#define  SCCEX_XML_SEARCHML_HEIGHT               0x00020000
#define  SCCEX_XML_SEARCHML_LEFTINDENT           0x00040000
#define  SCCEX_XML_SEARCHML_RIGHTINDENT          0x00080000
#define  SCCEX_XML_SEARCHML_FIRSTINDENT          0x00100000

/*  SearchML offset tracking options.  */
#define  SCCEX_XML_SEARCHML_OFFSET_ON            1
#define  SCCEX_XML_SEARCHML_OFFSET_OFF           0

/*  SearchML options controling production of unmapped text.  */
#define  SCCEX_XML_NO_UNMAPPEDTEXT              0
#define  SCCEX_XML_JUST_UNMAPPEDTEXT            1
#define  SCCEX_XML_BOTH_UNMAPPEDTEXT            2

/* XX */
#define SCCEX_XML_XDM_DTD					10
#define SCCEX_XML_XDM_XSD					11
#define SCCEX_XML_XDM_NONE					12

/* Option values for SCCOPT_EX_PAGEML_FLAGS */
#define SCCEX_PAGEML_TEXTOUT				0X01    /* Include text in PageML's output*/

/* Option values for SCCOPT_EX_PERFORMANCEMODE */
#define SCCEX_PERFORMANCE_NORMAL    0x00000000
#define SCCEX_PERFORMANCE_TEXTONLY  0x00000001  /* style/visual elements will not be processed.*/
#define SCCEX_PERFORMANCE_TEXTANDFONTS 0x00000002  /* visual elements will not be processed. Style elements will only be processed
                                                   where they describe font and charset information.*/

/* Flags for FONTOUTPUT */
#define SCCOPT_SUBSETFONT            0x00000001
#define SCCOPT_NAMEFONT              0x00000002
#define SCCOPT_IGNOREFONT            0x00000004

/* Item values for DAGetOptionItem */
#define SCCOPT_FIRSTITEM             0x00000001
#define SCCOPT_NEXTITEM              0x00000002
#define SCCOPT_ITEMSIZE              0x00000003
#define SCCOPT_LISTSIZE              0x00000004

#define SCCOPT_ALLITEMS	        ((VTDWORD)(-1))

/*Option Values for Tiff Options*/
#define SCCGRAPHIC_TIFF1BITBW		1/*1 bit black and white*/
#define SCCGRAPHIC_TIFF8BITPAL		2/*8 bit palette*/
#define SCCGRAPHIC_TIFF24BITRGB		3/*24 bit RGB*/ 
#define SCCGRAPHIC_TIFF8BITGRAYSCALE 4/*8 bit gray scale */

#define SCCGRAPHIC_TIFFCOMPNONE		1/*No compression*/
#define SCCGRAPHIC_TIFFCOMPPB		2/*Packbits compression*/
#define SCCGRAPHIC_TIFFCOMPLZW		3/*LZW compression*/
#define SCCGRAPHIC_TIFFCOMP1D		4/*CCITT - 1D*/
#define SCCGRAPHIC_TIFFCOMPGRP3		5/*CCITT - Group 3 Fax*/
#define SCCGRAPHIC_TIFFCOMPGRP4		6/*CCITT - Group 4 Fax*/

#define SCCGRAPHIC_TIFFBOBIGE		1/*This will use "big-endian" (Motorola) byte ordering.*/
#define SCCGRAPHIC_TIFFBOLITTLEE	0/*This will use "little-endian" (Intel) byte ordering.*/

#define SCCGRAPHIC_TIFFFLAGS_NONE	 0x00000000 /*No flags are used.*/
#define SCCGRAPHIC_TIFFFLAGS_ONEFILE 0x00000001/*When this flag is set, the output of a multiple pages from one input document will generate a single file with a separate image for each page converted.*/
#define SCCGRAPHIC_TIFFFLAGS_ONESTRIP	0x00000002/*When this flag is set the number of rows per strip is equal to the image height*/
#define SCCGRAPHIC_TIFFFLAGS_GRAY2COLOR 0x00000004/*When this flag is set, we can change grayscale TIFFs to color JPEGs in FI_TIFFANDJPEG*/

#define SCCGRAPHIC_TIFF_FILLORDER2		1
#define SCCGRAPHIC_TIFF_FILLORDER1 		0

#define SCCFILTER_TEXT_MASK             BIT0
#define SCCFILTER_LZW_MASK              BIT1
#define SCCFILTER_JPG_MASK              BIT2
/*added this for excel. When this is set, we don't put out blank cells after the last valid data cell - rf 12/1/03*/
#define SCCFILTER_NO_BLANK_CELLS_MASK   BIT3
/* Added to allow filters to know if they are being invoked as a normal or a support filter. */
#define SCCFILTER_SUPPORT_MASK          BIT4
/* This will over-ride bit 0 and output all sub-objects */
#define SCCFILTER_ALLSUBOBJ_MASK        BIT5    
/* Added to tell filters when to output full EXIF document properties */
#define SCCFILTER_EXIFFULL              BIT6
/*Added to let FA know which filter to load if the Extract XMP metadata option is set */
#define SCCFILTER_EXTRACTXMPFILTER_MASK        BIT7
#define SCCFILTER_PARSEXMPFILTER_MASK        BIT8
/* Added to tell the text filter not to output page breaks */
#define SCCFILTER_NOFF_MASK             BIT9
/* Added to tell the PST filter to ignore verification of password */
#define SCCFILTER_IGNORE_PASSWORD_MASK  BIT10
/* Added to tell the PDF filter to reorder BIDI text */
#define SCCFILTER_PDF_REORDER_BIDI_MASK	BIT11
/* Added to tell the PDF filter to Drop Hyphens at the end of line */
#define SCCFILTER_PDF_DROPHYPHENS_MASK	BIT12

/* Added to tell the Excel filters when to output revisions history. Only output when set */
#define SCCFILTER_SS_OUTPUT_REVISIONS_MASK		BIT13

/* Added to tell filters when to skip putting out table structures.*/
#define SCCFILTER_TABLEOUTPUT_MASK BIT14

/* Added for presentation filters to parse and describe animation data.  */
#define SCCFILTER_PARSE_TIMING_DATA_MASK BIT15

/* Added to communicate with the performance filter   BIT16 - BIT31  jrw  12/17/07 */
#define SCCFILTER_RESERVED              0xffff0000

/* added to put antialias flag in dwMiscFlags -- jm 6/21/05 */
#define SCCANTIALIAS_MASK BIT0
/* Add the Stack Buffer Overrun flag  jrw  12/5/06 */
#define SCCSBO_MASK         BIT1    
/* Add PDF Index Mode flag  jrw  10/24/13 */
#define SCCPDFINDEXMODE_MASK    BIT2
/* Add the Spreadsheet export override flag  jrw  1/12/11 */
/* Change it to a value rather than a flag   jrw  5/30/12 */
#define SCCSSOVERRIDE_MASK  0xffff0000
#define SCCSSOVERRIDE_SHIFT 16

/* Option value for the extraction of attachment */
#define SCCEX_EXTRACT_OFF  1
#define SCCEX_EXTRACT_CONVERT  2
#define SCCEX_EXTRACT_BINARY 3

/* Options value for the SS/DB cell border */
#define SCCEX_SSDBBORDERS_CREATEIFMISSING      0     /* Create SS/DB cell border if they are not provide by the source docoument */
#define SCCEX_SSDBBORDERS_OFF                  1     /* Turn off the cell border */
#define SCCEX_SSDBBORDERS_USESOURCE            2     /* Use the cell botder provide by the source document */

/* Options for Margin Locations */
#define SCC_MARGIN_LOCATION_TOP_LEFT        1
#define SCC_MARGIN_LOCATION_TOP_CENTER      2
#define SCC_MARGIN_LOCATION_TOP_RIGHT       3
#define SCC_MARGIN_LOCATION_BOTTOM_LEFT     4
#define SCC_MARGIN_LOCATION_BOTTOM_CENTER   5
#define SCC_MARGIN_LOCATION_BOTTOM_RIGHT    6

#define SCCOPT_MAX_MARGINTEXTLINE           128
#define MAX_MARGIN_TEXT_LINES               18



/* Flags for SCCOPT_CCFLEX_FORMATOPTIONS */
#define CCFLEX_FORMATOPTIONS_BITMAPASBITMAP         0x00000001  /* Produce a bitmap for every bitmap section */
#define CCFLEX_FORMATOPTIONS_CHARTASBITMAP          0x00000002  /* Produce a bitmap for every chart section */
#define CCFLEX_FORMATOPTIONS_PRESENTATIONASBITMAP   0x00000004  /* Produce a bitmap for every presentation section */
#define CCFLEX_FORMATOPTIONS_VECTORASBITMAP         0x00000008  /* Produce a bitmap for every vector section */
#define CCFLEX_FORMATOPTIONS_NOBITMAPELEMENTS       0x00000010  /* Don't produce flexiondoc elements for bitmap sections */
#define CCFLEX_FORMATOPTIONS_NOCHARTELEMENTS        0x00000020  /* Don't produce flexiondoc elements for chart sections */
#define CCFLEX_FORMATOPTIONS_NOPRESENTATIONELEMENTS 0x00000040  /* Don't produce flexiondoc elements for presentation sections */
#define CCFLEX_FORMATOPTIONS_NOVECTORELEMENTS       0x00000080  /* Don't produce flexiondoc elements for vector sections */
#define CCFLEX_FORMATOPTIONS_ISODATES               0x00000100  /* Use ISO 8601 formatting for all date and date/time values */
#define CCFLEX_FORMATOPTIONS_FLATTENSTYLES          0x00000200  /* Flatten styles to eliminate need to process 'basedon' attribute */
#define CCFLEX_FORMATOPTIONS_REMOVECURRENTPOINT     0x00000400  /* Remove references to current point in vector output. Deprecated in 8.4.1 */
#define CCFLEX_FORMATOPTIONS_REMOVEFONTGROUPS       0x00000800  /* Replace font groups with references to individual fonts */
#define CCFLEX_FORMATOPTIONS_INCLUDETEXTOFFSETS     0x00001000  /* Include text_offset attribute on tx.p and tx.r elements */
#define CCFLEX_FORMATOPTIONS_USEFULLFILEPATHS       0x00002000  /* Use full path for value of "path" attribute of locator_file */
#define CCFLEX_FORMATOPTIONS_SEPARATESTYLETABLES    0x00004000  /* Place style_tables element in a separate file for more efficient memory usage */
#define CCFLEX_FORMATOPTIONS_DELIMITERS             0x00008000  /* Include 'delimiter' and 'word-delimiter' special characters */
#define CCFLEX_FORMATOPTIONS_OPTIMIZESECTIONS       0x00010000  /* Use wp.section elements to delineate column references */
#define CCFLEX_FORMATOPTIONS_CHARMAPPING            0x00060000  /* Character mapping options (one of four options, see below) */
#define CCFLEX_FORMATOPTIONS_GENERATESYSTEMMETADATA   0x00080000  /* Generate system metadata */
#define CCFLEX_FORMATOPTIONS_PROCESSARCHIVESUBDOCS    0x00100000  /* produce flexiondoc elements for archive subdocs */
#define CCFLEX_FORMATOPTIONS_PROCESSEMBEDDINGSUBDOCS  0x00200000  /* produce flexiondoc elements for embeddings */
#define CCFLEX_FORMATOPTIONS_PROCESSATTACHMENTSUBDOCS 0x00400000  /* produce flexiondoc elements for email attachments */


/* Character mapping options for CCFLEX_FORMATOPTIONS_CHARMAPPING */
#define CCFLEX_CHARMAPPING_DEFAULT                  0x00000000  /* Default behavior: All text is mapped to unicode, in tx.text elements */
#define CCFLEX_CHARMAPPING_NOMAPPING                0x00020000  /* All text is left in the original character set, in tx.utext elements */
#define CCFLEX_CHARMAPPING_MAPTEXT                  0x00040000  /* Text is mapped to unicode where possble, unmappable text is left in the original character set */
#define CCFLEX_CHARMAPPING_BOTH                     0x00060000  /* Both mapped and unmapped text is included as an alt element containing tx.text and tx.utext */


/* Values for SCCOPT_CCFLEX_UNITS */
#define CCFLEX_LINEARUNITS_DEFAULT               0x00
#define CCFLEX_LINEARUNITS_CM                    0x01  /* Centimeters */
#define CCFLEX_LINEARUNITS_CO                    0x02  /* Ciceros */
#define CCFLEX_LINEARUNITS_DD                    0x03  /* Didots */
#define CCFLEX_LINEARUNITS_FT                    0x04  /* Feet */
#define CCFLEX_LINEARUNITS_IN                    0x05  /* Inches */
#define CCFLEX_LINEARUNITS_KM                    0x06  /* Kilometers */
#define CCFLEX_LINEARUNITS_M                     0x07  /* Meters */
#define CCFLEX_LINEARUNITS_MI                    0x08  /* Miles */
#define CCFLEX_LINEARUNITS_MM                    0x09  /* Millimeters */
#define CCFLEX_LINEARUNITS_PC                    0x0A  /* Picas */
#define CCFLEX_LINEARUNITS_PT                    0x0B  /* Points */
#define CCFLEX_LINEARUNITS_TP                    0x0C  /* Twips */
#define CCFLEX_LINEARUNITS_YD                    0x0D  /* Yards */


/* Values for SCCOPT_CCOD_GRAPHICOPTIONS */
#define CCOD_GRAPHICOPTIONS_STANDARD        0    /* Standard behavior: Supported graphic types are extracted, others are converted */
#define CCOD_GRAPHICOPTIONS_NONE            1    /* No graphics are extracted or converted */
#define CCOD_GRAPHICOPTIONS_EXTRACT         2    /* Supported graphic types are extracted, others are ignored */
#define CCOD_GRAPHICOPTIONS_CONVERT         3    /* All graphics are converted */

/*OCR STUFF*/
#define OCR_TECH_NONE 0
#define OCR_TECH_NUANCE 1
#define OCR_TECH_OTHER  2

#define OCR_QUAL_FAST 0
#define OCR_QUAL_SLOW 1
#define OCR_QUAL_BALANCED 2

/*HTML input filter option values*/
#define HTML_COND_COMMENT_NONE   0x0000  /*don't output any conditional comments*/
#define HTML_COND_COMMENT_IE5    0x0001  /*output the conditional comments for IE v5.0*/
#define HTML_COND_COMMENT_IE6    0x0002  /*output the conditional comments for IE v6.0*/
#define HTML_COND_COMMENT_IE7    0x0004  /*output the conditional comments for IE v7.0*/
#define HTML_COND_COMMENT_IE8    0x0008  /*output the conditional comments for IE v8.0*/
#define HTML_COND_COMMENT_IE9    0x0010  /*output the conditional comments for IE v9.0*/
#define HTML_COND_COMMENT_ALL    0xFFFF  /*output ALL conditional comments, even malformed.*/

/* Values for SCCOPT_OUTPUT_STRUCTURE */
#define SCCWV_STRUCTURE_FLAT              0
#define SCCWV_STRUCTURE_AJAX_CHUNKED      1
#define SCCWV_STRUCTURE_AJAX_STREAMING    2
#define SCCWV_STRUCTURE_PAGE_STREAMING    3
#define SCCWV_STRUCTURE_DYNAMIC           4

/*END HTML input filter option values*/

#ifdef __cplusplus
}
#endif

#endif /* _SCCOPVAL_H */
