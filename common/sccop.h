/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. 
All rights reserved.*/
/*
|   SCC Viewer Technology - Source file
|
|   File:           sccopt.h
|   Module:         SCC Common Options Model
|   Environment:    Portable
|   Function:       Common Options Model Structures and Macros
*/


#ifndef _SCCOPT_H
#define _SCCOPT_H

#ifdef WINDOWS
#ifndef SCC_PACK_SET
#define SCC_PACK_SET 1
#define SCC_PACKED_BY_SCCOP_H 1
#pragma pack(push,8)
#endif /* SCC_PACK_SET */
#endif /* WINDOWS */

#include <sccid.h>
#include <sccopval.h>
#include <baseio.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef VTHANDLE HOPTIONSET, * PHOPTIONSET;

/* Used by VW for the tempfile naming callback */
typedef VTSYSVAL (IO_ENTRYMODPTR VWCALLBACKPROC)(VTSYSVAL,VTDWORD,VTDWORD,VTLPVOID);

/* Used by redirect temp file callback */
typedef VTDWORD (IO_ENTRYMODPTR REDIRECTTEMPFILECALLBACKPROC)(HIOFILE *phFile,  VTVOID *pSpec,  VTDWORD dwFileFlags);


/*-----------------------------------------------------------------------------
|   IMPORTANT!!!!!                IMPORTANT!!!!!!                 IMPORTANT!!!
|
|   If you are adding an option, there are several things that must be done.
|   First of all, an SCCOPT_ must be added for it in sccop.h.  Any values that
|   the options will take must be defined in sccopval.h.  An entry for the
|   the option should be added to the structure that will track the option.
|   If the structure is an existing structure, a copy of it must be made and
|   given a name with the PREVIOUS release number in it.  This is done so that
|   when people upgrade to new technology, they will be able to keep their
|   existing options.  See UTLoadDefaultOptions for more info on this.
|   Lastly, make sure that the code to handle the option is in UTSetOption,
|   UTGetOption, and UTCopyOptionChanges.  If the option allocated memory,
|   make sure to handle it in UTFreeAllocatedOptions.  Also, do not forget to
|   add entries to the UTOPTIONDELTA struct so that we don't get any array
|   overruns.  Finally, please note that the global option set (gOptions) is
|   NOT to be written to except by UTSetGlobalOption or UTSetGlobalOptionSet.
|------------------------------------------------------------------------------
|   The UTOPTIONDELTA stuct contains an array of Option IDs which is used to
|   identify which options have been changed, so that they may be updated in
|   the global option set before it is saved to disk.  When adding options,
|   make sure that the size of this struct is increased if neccesary to account
|   for the increased number of options which may change.
|------------------------------------------------------------------------------
*/
/*
|-------------------------------------------------------------
|   Option numbers used are 1-139 (except for 11-14).
|   Option 98 is actually the character value.
|-------------------------------------------------------------
*/

/*
|   Options callback Action IDs
*/
#define SCCOPT_CALLBACK_BEGIN                           0
#define SCCOPT_CALLBACK_END                             1
#define SCCOPT_CALLBACK_OPTION                          2

#define SCCUT_FILENAMEMAX       256

/*
|   O P T I O N   I D S
|   High nibble is flags, next three are the option ID, low word
|   is data size (0 = no intrinsic data size)
*/


#define SCCOPT_INVALID                                      0       /* Invalid option */

/*
|   HTML/Wireless Export Options
*/
#define SCCOPT_EX_OUTPUTCHARACTERSET                        1       /* Output character set                                                     */
#define SCCOPT_EX_TEMPLATE                                  3       /* Template file                                                            */
#define SCCOPT_EX_FLAVOR                                    7       /* Output flavor                                                            */
#define SCCOPT_EX_USEDEFTIMEFORDRAMCOMPARE                  15      /* Use default Date/Time for Dramamine compare                              */
#define SCCOPT_EX_COMPLIANCEFLAGS                           27      /* Option flags for HTML Export                                             */
#define SCCOPT_EX_CHARBYTEORDER                             31      /* Describes what format of Unicode to use in HTML Export.                  */
#define SCCOPT_EX_UNICODECALLBACKSTR                        32      /* Determines whether callbacks should use the Unicode structs              */
#define SCCOPT_EX_NOSOURCEFORMATTING                        33      /* Determines whether the exported text is formatted for user readability   */
#define SCCOPT_EX_FALLBACKFONT                              35      /* Indicates the default font to use                                        */
#define SCCOPT_EX_SIMPLESTYLENAMES                          36      /* Discard invalid style names characters                                   */
#define SCCOPT_EX_TEXTBUFFERSIZE                            37      /* Size of the device's text/primary buffer in bytes                        */
#define SCCOPT_EX_GENBULLETSANDNUMS                         41      /* Generate list numbers instead of using list tags                         */
#define SCCOPT_EX_LABELFLAGS                                45      /* Flags for labeling text in WML                                           */
#define SCCOPT_EX_GRAPHICSKIPSIZE                           47      /* Skip graphics this size or smaller                                       */
#define SCCOPT_EX_GRAPHICBUFFERSIZE                         48      /* Size in bytes of the device's graphics buffer                            */
#define SCCOPT_EX_JAVASCRIPTTABS                            49      /* Using JavaScript to handle the tab stop                                  */
#define SCCOPT_EX_PAGESIZE                                  50      /* Page size in characters                                                  */
#define SCCOPT_EX_COLLAPSEWHITESPACE                        51      /* Remove excess whitespace                                                 */
#define SCCOPT_EX_MAXURLLENGTH                              52      /* Max. length of URLs in characters                                        */
#define SCCOPT_EX_SEPARATEGRAPHICSBUFFER                    53      /* Use separate buffer for graphics                                         */
#define SCCOPT_EX_CALLBACKS                                 54      /* Use callback flags to turn on/off individual EX callbacks                */
#define SCCOPT_EX_GRIDROWS                                  55      /* Number of rows in a grid                                                 */
#define SCCOPT_EX_GRIDCOLS                                  56      /* Number of columns in a grid                                              */
#define SCCOPT_EX_GRIDADVANCE                               57      /* Indicates the traversal direction of a grid when using next/previous     */
#define SCCOPT_EX_GRIDWRAP                                  58      /* Indicates that sheet traversal should continue when an edge is reached   */
#define SCCOPT_EX_PREVENTGRAPHICOVERLAP                     59      /* Prevent graphics from overlapping                                        */
#define SCCOPT_EX_FONTFLAGS                                 60      /* Option flags to suppress different font attributes                       */
#define SCCOPT_EX_SHOWHIDDENTEXT                            79      /* Show hidden text                                                         */
#define SCCOPT_EX_EXTRACTEMBEDDEDFILES                      94      /* extraction of the attachments */
#define SCCOPT_EX_SHOWHIDDENSSDATA                          95      /* Show hidden rows, columns and sheets for the spread sheet                */
#define SCCOPT_EX_CHANGETRACKING                            96      /* Export the tracking change for the word processing document              */
#define SCCOPT_EX_SHOWSPREADSHEETBORDER                     97      /* Show spreadsheet border                                                  */
#define SCCOPT_EX_SSDBROWCOLHEADINGS                        106     /* Show row and columns headings                                            */
#define SCCOPT_EX_SSDBBORDERS                               107     /* Show SS/DB border                                                        */
#define SCCOPT_EX_SUPPRESSMETADATA                          131     /* PHTML only - suppress output of input file metadata                      */
#define SCCOPT_EX_SSOVERRIDE                                134     /* OracleText override output FI for spreadsheets                           */
#define SCCOPT_EX_PDFINDEXMODE                              135     /* OracleText override output <div> creation for PDFs with word delims      */
#define SCCOPT_EX_PERFORMANCEMODE                           137     /* Set performance mode for chunker.                                        */

/* Options used by SX/XX/CA */
#define SCCOPT_XML_DEF_METHOD                               61      /* XML definition method                                                    */
#define SCCOPT_XML_DEF_REFERENCE                            62      /* Path to XML definition file                                              */
#define SCCOPT_XML_SEARCHML_FLAGS                           63      /* Option flags for XML Export                                              */
#define SCCOPT_XML_SEARCHML_UNMAPPEDTEXT                    109     /* Control production of unmapped text in SearchML.                         */
#define SCCOPT_CA_FLAGS               SCCOPT_XML_SEARCHML_FLAGS     /* Option flags for CA.  Actually an alias for similar SearchML flags.      */
#define SCCOPT_CA_XMLOUTPUT                                 105     /*  CATest should produce UTF8 encoded XML output.  */

/*  SearchML options.  */
#define SCCOPT_XML_SEARCHML_CHAR_ATTRS                      75      /*  The character attributes to track                                       */
#define SCCOPT_XML_SEARCHML_PARA_ATTRS                      76      /*  The paragraph attributes to track                                       */
#define SCCOPT_XML_SEARCHML_OFFSET                          77      /*  Should offset be tracked?                                               */
#define SCCOPT_XML_NULLREPLACECHAR                          102     /*  Replace null path separator characters with this character.             */

/* PageML options */
#define SCCOPT_XML_PAGEML_FLAGS                             64      /* Option flags for PageML                                                  */
#define SCCOPT_XML_PAGEML_PRINTERNAME                       74      /* Option for the format device for the PageML                              */

/* EXXML option.  (EXXML is the XML Writer that consumes from the XChunker) */
#define SCCOPT_EXXML_SUBSTREAMROOTS                         78      /* A wide string consisting of comma-delimited substrings                   */
#define SCCOPT_EXXML_DEF_METHOD                             91      /* XML definition method for EXXML                                          */
#define SCCOPT_EXXML_DEF_REFERENCE                          92      /* Path to XML definition file for EXXML                                    */

/* CCFLEX options (Chunker to Flexiondoc component) */
#define SCCOPT_CCFLEX_FULLYATTRIBUTESTYLES                  85      /* TRUE to fully attribute styles                                           */
#define SCCOPT_CCFLEX_REMOVECURRENTPOINT                    86      /* TRUE to remove references to current point in vector output. Deprecated. */
#define SCCOPT_CCFLEX_REMOVEFONTGROUPS                      87      /* TRUE to replace font groups with references to individual fonts          */
#define SCCOPT_CCFLEX_INCLUDETEXTOFFSETS                    88      /* TRUE to include text_offset attribute on tx.p and tx.r elements          */
#define SCCOPT_CCFLEX_EXTRACTEMBEDDINGS                     89      /* List of FI Id's for embeddings to extract without conversion             */
#define SCCOPT_CCFLEX_CONVERTEMBEDDINGS                     90      /* List of FI Id's for embeddings to extract and convert. Converted to type */
                                                                    /* specified by SCCOPT_GRAPHIC_TYPE                                         */
#define SCCOPT_CCFLEX_FORMATOPTIONS                         99      /* Options for how to convert various formats - see sccopval.h              */
#define SCCOPT_CCFLEX_UNITS                                 111     /* Measurement units to use - see sccopval.h                                */

/* CCOD options (Flexiondoc to Open Document component) */
#define SCCOPT_CCOD_GRAPHICOPTIONS                          110

/* these are left for backward compatibility */
#define SCCOPT_OUTPUTCHARACTERSET                           1
#define SCCOPT_HTML_OUTPUTCHARACTERSET                      1
#define SCCOPT_HTML_TEMPLATE                                3
#define SCCOPT_HTML_FLAVOR                                  7
#define SCCOPT_USE_DEFAULTS_FOR_DRAMAMINE_COMPARE           15
#define SCCOPT_HTML_ENABLEPSCALLBACK                        16      /* HTML Export option to enable publishing server callback **UNUSED**       */
#define SCCOPT_HTML_FLAGS                                   27
#define SCCOPT_CHARBYTEORDER                                31
#define SCCOPT_UNICODECALLBACKSTR                           32
#define SCCOPT_NO_SOURCEFORMATTING                          33
#define SCCOPT_HTML_FALLBACKFONT                            35
#define SCCOPT_SIMPLESTYLENAMES                             36
#define SCCOPT_HTML_SIMPLESTYLENAMES                        36
#define SCCOPT_WML_DECKSIZE                                 37
#define SCCOPT_HTML_GENBULLETSANDNUMS                       41
#define SCCOPT_WML_OUTPUTCHARACTERSET                       42
#define SCCOPT_WML_TEMPLATE                                 43      /* mapped to SCCOPT_EX_TEMPLATE */
#define SCCOPT_WML_FLAVOR                                   44
#define SCCOPT_WML_LABELFLAGS                               45
#define SCCOPT_HTML_JAVASCRIPTTABS                          49
#define SCCOPT_WML_GRAPHICSKIPSIZE                          47
#define SCCOPT_HTML_PAGESIZE                                50
#define SCCOPT_WML_COLLAPSEWHITESPACE                       51
#define SCCOPT_WML_MAXURLLENGTH                             52

/*
|  General graphic options
*/
#define SCCOPT_GRAPHIC_WIDTH                                4       /* Width in pixels of exported graphic */
#define SCCOPT_GRAPHIC_HEIGHT                               5       /* Height in pixels of exported graphic */
#define SCCOPT_GRAPHIC_TYPE                                 6       /* Graphic output type */
#define SCCOPT_HTML_GRAPHICTYPE                             6       /* for backward compatibility */
#define SCCOPT_GRAPHIC_FLAGS                                19      /* Flags that specify whether aspect ratio should be */
                                                                    /* maintained, and whether the image should be centered. */
#define SCCOPT_GRAPHIC_CROP                                 8
#define SCCOPT_GRAPHIC_XDPI                                 25
#define SCCOPT_GRAPHIC_YDPI                                 26
#define SCCOPT_GRAPHIC_OUTPUTDPI                            34      /* Output DPI for exported images ... if supplied! */
#define SCCOPT_GDSF_DEFAULTGRAPHICONERROR                   29
#define SCCOPT_GRAPHIC_SIZELIMIT                            38      /* Maximum size of exported graphics in pixels */
#define SCCOPT_GRAPHIC_WIDTHLIMIT                           39      /* Maximum width of exported graphic  - overrides SCCOPT_GRAPHIC_WIDTH */
#define SCCOPT_GRAPHIC_HEIGHTLIMIT                          40      /* Maximum height of exported graphic - overrides SCCOPT_GRAPHIC_HEIGHT */
#define SCCOPT_GRAPHIC_PAGENUMBER                           80
#define SCCOPT_GRAPHIC_LASTPAGE                             81
#define SCCOPT_GRAPHIC_MULTIPAGE                            82
#define SCCOPT_GRAPHIC_IMAGEEXPORT                          83
#define SCCOPT_IMAGEX_TIFFOPTIONS                           84
#define SCCOPT_ACCEPT_ALT_GRAPHICS                          93
#define SCCOPT_GRAPHIC_TRANSPARENCYCOLOR                    103
#define SCCOPT_GRAPHIC_CROPPING                             104

#define SCCOPT_GRAPHIC_WATERMARK_OPACITY                    113
#define SCCOPT_GRAPHIC_WATERMARK_POSITION                   114
#define SCCOPT_GRAPHIC_WATERMARK_SCALETYPE                  115
#define SCCOPT_GRAPHIC_WATERMARK_SCALEPERCENT               116
#define SCCOPT_GRAPHIC_WATERMARK_SCALEHEIGHT                117
#define SCCOPT_GRAPHIC_WATERMARK_SCALEWIDTH                 118
#define SCCOPT_GRAPHIC_WATERMARK_HORIZONTALPOS              119
#define SCCOPT_GRAPHIC_WATERMARK_VERTICALPOS                120
#define SCCOPT_GRAPHIC_WATERMARK_PATH                       121
#define SCCOPT_GRAPHIC_WATERMARK_IOTYPE                     122
#define SCCOPT_GRAPHIC_WATERMARK_HORIZONTAL_OFFSET          123
#define SCCOPT_GRAPHIC_WATERMARK_VERTICAL_OFFSET            124
#define SCCOPT_GRAPHIC_WATERMARK_HORIZONTAL_OFFSETFROM      125
#define SCCOPT_GRAPHIC_WATERMARK_VERTICAL_OFFSETFROM        126
/*  Skip 127 here because it is used elsewhere for an option acutally in use  .  */
#define SCCOPT_GRAPHIC_RENDERASPAGE                         128
#define SCCOPT_GRAYSCALE_TO_COLOR                           129
#define SCCOPT_GRAPHIC_TRANSPARENCYCOLORTHRESHOLD           139


/*
|   General options
*/
#define SCCOPT_FALLBACKFORMAT                               2       /* Fallback format options                                                  */
#define SCCOPT_UNMAPPABLECHAR                               98      /* Sets the unmappable Unicode character                                    */
#define SCCOPT_FORMATFLAGS                                  SCCID_FORMATFLAGS     /* Generic bit field for flags applicable to a wide range of products.  ISO formatting of Date/Times for example.  */

/*
|   RAINBOW OPTIONS - NB 3/17/97
*/
#define SCCOPT_RAINBOW_DATADIRECTORY                        9       /* directory location of FCM and PEM Files */
#define SCCOPT_RAINBOW_CONVERSION_SEL                       10      /* Defines which section types get converted */

/*
|   GIF export options
*/
#define SCCOPT_GIF_INTERLACED                               17      /* Interlaced or non-interlaced GIF output */
#define SCCOPT_GIF_SPLASHPALETTE                            28      /* Use splash palette for GIF (turn off for quantization) */

/*
|   JPEG export options
*/
#define SCCOPT_JPEG_QUALITY                                 18      /* Quality of JPEG output (0-100) - 100 means high quality, */
                                                                    /* less compression. */
/*
|   TIF export options
*/
#define SCCOPT_TIFF_FORMAT                                  20
#define SCCOPT_TIFF_COMPRESSION                             21
#define SCCOPT_TIFF_COLORTRANSITION                         22
#define SCCOPT_TIFF_XRESOLUTION                             23
#define SCCOPT_TIFF_YRESOLUTION                             24

/*
|   Graphic sizing option
|   (DWORD) Algorithm to use when sizing graphics. The smoothing
|   algorithm takes approximately twice as long as the quick
|   algorithm for color images and 25% longer for grayscale.
*/
#define SCCOPT_GRAPHIC_SIZEMETHOD                           30
#define SCCOPT_GRAPHICSIZEMETHOD                            30      /* for backward compatibility */

#define	SCCOPT_QUICKTHUMBNAIL								130

/* New Stack Buffer Overrun handler option */
#define SCCOPT_USENEWSBOHANDLER                             108

/* Option for setting the disk I/O buffer sizes              */
#define SCCOPT_IO_BUFFERSIZE                                112

/* Option for extract XML metadata from filter */
#define SCCOPT_EXTRACTXMPMETADATA                                   127
/*OCR Options*/
#define SCCOPT_OCR_TECH                                     65
#define SCCOPT_OCR_QUALITY                                  66


/* Redirect temp file option */
#define SCCOPT_REDIRECTTEMPFILE                              67            

/* Lotus Notes directory. NSF import filter option. */     
#define SCCOPT_LOTUSNOTESDIRECTORY                          132

/*  When on text should be stroked out by the Autocad filter.  */
#define SCCOPT_STROKE_TEXT                                  138


#define SCCOPT_PROCESS_OLE_EMBEDDINGS                       133

/* 
| Thread initialization options.  These are not used bu UTSetOption, 
| but this is the best place to define them.
*/
/* defines for thread initialization. */
#define SCCOPT_INIT_PTHREADS                                134
#define SCCOPT_INIT_NOTHREADS                               135
#define SCCOPT_INIT_NATIVETHREADS                           136 

/* Replace null path separator with this characters. Alias for the equivalent SearchML option */
#define SCCOPT_NULLREPLACECHAR                           SCCOPT_XML_NULLREPLACECHAR



/*
|   General Viewer Technology Options
|
|   These are all defined to be the same as their VW equivalents,
|   but they don't have to be.  I just did this for neatness' sake.
|
*/
#define SCCOPT_RESOURCELIBRARYID                            SCCID_RESOURCELIBRARYID
#define SCCOPT_DEFAULTDISPLAYFONT                           SCCID_DEFAULTDISPLAYFONT
#define SCCOPT_PRINTHEADER                                  SCCID_PRINTHEADER
#define SCCOPT_DEFAULTPRINTMARGINS                          SCCID_DEFAULTPRINTMARGINS
#define SCCOPT_DEFAULTPRINTFONT                             SCCID_DEFAULTPRINTFONT
#define SCCOPT_PRINTHEADERFONT                              SCCID_PRINTHEADERFONT
#define SCCOPT_PRINTINFO                                    SCCID_PRINTINFO
#define SCCOPT_PRINTJOBNAME                                 SCCID_PRINTJOBNAME
#define SCCOPT_WHATTOPRINT                                  SCCID_WHATTOPRINT
#define SCCOPT_PRINTSTARTPAGE                               SCCID_PRINTSTARTPAGE
#define SCCOPT_PRINTENDPAGE                                 SCCID_PRINTENDPAGE
#define SCCOPT_PRINTCOLLATE                                 SCCID_PRINTCOLLATE
#define SCCOPT_PRINTCOPIES                                  SCCID_PRINTCOPIES
#define SCCOPT_HAVESELECTION                                SCCID_HAVESELECTION
#define SCCOPT_USEDOCPAGESETTINGS                           SCCID_USEDOCPAGESETTINGS


/*
|   Added to avoid namespace collisions with the original SCCOPT_FALLBACKFORMAT.
*/
#define SCCOPT_FALLBACKFORMAT_VT                            SCCID_FALLBACKFORMAT

/*
|   Added to avoid namespace collisions with the original SCCOPT_UNMAPPABLECHAR.
*/
#define SCCOPT_UNMAPPABLECHAR_VT                            SCCID_UNMAPPABLECHAR

#define SCCOPT_DEFAULTINPUTCHARSET                          SCCID_DEFAULTINPUTCHARSET

/*
|   Font Options
*/
#define SCCOPT_FONTSCALINGFACTOR                            SCCID_FONTSCALINGFACTOR
#define SCCOPT_DISPLAYFONTALIAS                             SCCID_DISPLAYFONTALIAS
#define SCCOPT_PRINTFONTALIAS                               SCCID_PRINTFONTALIAS
#define SCCOPT_ORIGFONTSCALINGFACTOR                        SCCID_ORIGFONTSCALINGFACTOR
#define SCCOPT_FONTFILTER                                   SCCID_FONTFILTER
/*
|   Document Options
*/
#define SCCOPT_WPDISPLAYMODE                                SCCID_WPDISPLAYMODE
#define SCCOPT_WPZOOM                                       SCCID_WPZOOM
#define SCCOPT_WPFITMODE                                    SCCID_WPFITMODE
#define SCCOPT_WPWRAPTOWINDOW                               SCCID_WPWRAPTOWINDOW
#define SCCOPT_WPPRINTMODE                                  SCCID_WPPRINTMODE
#define SCCOPT_WPDISABLEHYPERLINKS                          SCCID_WPDISABLEHYPERLINKS
#define SCCOPT_WPDISABLEPOSITIONALFRAMES                    SCCID_WPDISABLEPOSITIONALFRAMES
#define SCCOPT_WPDISABLEEMAILHEADER							SCCID_WPDISABLEEMAILHEADER


/*
|   HTML and Email Document Options
*/
#define SCCOPT_HTMLDISPLAYMODE                                SCCID_HTMLDISPLAYMODE
#define SCCOPT_HTMLFITMODE                                    SCCID_HTMLFITMODE
#define SCCOPT_EMAILDISPLAYMODE								  SCCID_EMAILDISPLAYMODE
#define SCCOPT_EMAILFITMODE                                   SCCID_EMAILFITMODE


/*
|   Bitmap Options
*/
#define SCCOPT_BMPROTATION                                  SCCID_BMPROTATION
#define SCCOPT_BMPPRINTASPECT                               SCCID_BMPPRINTASPECT
#define SCCOPT_BMPPRINTBORDER                               SCCID_BMPPRINTBORDER
#define SCCOPT_BMPDITHER                                    SCCID_BMPDITHER
#define SCCOPT_BMPFITMODE                                   SCCID_BMPFITMODE
#define SCCOPT_BMPFITPOS                                    SCCID_BMPFITPOS
#define SCCOPT_BMPZOOM                                      SCCID_BMPZOOM
#define SCCOPT_BMPSHOWFULLSCREEN                            SCCID_BMPSHOWFULLSCREEN
#define SCCOPT_BMPZOOMEVENT                                 SCCID_BMPZOOMEVENT
#define SCCOPT_BMPDITHERAVAILABLE                           SCCID_BMPDITHERAVAILABLE

/*
|   Vector Options
*/
#define SCCOPT_VECPRINTASPECT                               SCCID_VECPRINTASPECT
#define SCCOPT_VECPRINTBORDER                               SCCID_VECPRINTBORDER
#define SCCOPT_VECFITMODE                                   SCCID_VECFITMODE
#define SCCOPT_VECZOOM                                      SCCID_VECZOOM
#define SCCOPT_VECSHOWFULLSCREEN                            SCCID_VECSHOWFULLSCREEN
#define SCCOPT_VECZOOMEVENT                                 SCCID_VECZOOMEVENT
#define SCCOPT_VECSHOWBACKGROUND                            SCCID_VECSHOWBACKGROUND
#define SCCOPT_VECPRINTBACKGROUND                           SCCID_VECPRINTBACKGROUND
#define SCCOPT_VECDRAFTMODE                                 SCCID_VECDRAFTMODE

/*
 | For both Vector and Bitmaps
 */
#define SCCOPT_MAINTAINZOOM									SCCID_MAINTAINZOOM


/*
|   Spreadsheet Options
*/
#define SCCOPT_SSSHOWGRIDLINES                              SCCID_SSSHOWGRIDLINES
#define SCCOPT_SSPRINTGRIDLINES                             SCCID_SSPRINTGRIDLINES
#define SCCOPT_SSPRINTHEADINGS                              SCCID_SSPRINTHEADINGS
#define SCCOPT_SSCLIPBOARD                                  SCCID_SSCLIPBOARD
#define SCCOPT_SSBADTYPE                                    SCCID_SSBADTYPE
#define SCCOPT_SSDRAFTMODE                                  SCCID_SSDRAFTMODE
#define SCCOPT_SSPRINTFITTOPAGE                             SCCID_SSPRINTFITTOPAGE
#define SCCOPT_SSPRINTSCALEPERCENT                          SCCID_SSPRINTSCALEPERCENT
#define SCCOPT_SSPRINTSCALEXHIGH                            SCCID_SSPRINTSCALEXHIGH
#define SCCOPT_SSPRINTSCALEXWIDE                            SCCID_SSPRINTSCALEXWIDE
#define SCCOPT_SSPRINTDIRECTION                             SCCID_SSPRINTDIRECTION
#define SCCOPT_SSSHOWHEADINGS                               SCCID_SSSHOWHEADINGS
#define SCCOPT_SS4DIGITDATES                                SCCID_SS4DIGITDATES
#define SCCOPT_SSSHOWHIDDENCELLS                            SCCID_SSSHOWHIDDENCELLS

/*IX specific SS/DB options*/
#define SCCOPT_MAXSSDBPAGEWIDTH                             100
#define SCCOPT_MAXSSDBPAGEHEIGHT                            101
/*
|   Database options
*/
#define SCCOPT_DBSHOWGRIDLINES                              SCCID_DBSHOWGRIDLINES
#define SCCOPT_DBPRINTGRIDLINES                             SCCID_DBPRINTGRIDLINES
#define SCCOPT_DBPRINTHEADINGS                              SCCID_DBPRINTHEADINGS
#define SCCOPT_DBCLIPBOARD                                  SCCID_DBCLIPBOARD
#define SCCOPT_DBFIELDNAMESTOCLIP                           SCCID_DBFIELDNAMESTOCLIP
#define SCCOPT_DBDRAFTMODE                                  SCCID_DBDRAFTMODE
#define SCCOPT_DBPRINTFITTOPAGE                             SCCID_DBPRINTFITTOPAGE
/*
|PDF Export Filter options
*/
#define SCCOPT_APPLYFILTER                                  SCCID_APPLYFILTER
#define SCCOPT_DOLINEARIZATION                              SCCID_DOLINEARIZATION
#define SCCOPT_UNRESTRICTEDFONTEMBEDDING                    SCCID_UNRESTRICTEDFONTEMBEDDING
#define SCCOPT_EXPORTEMAILATTACHMENTS                       SCCID_EXPORTEMAILATTACHMENTS
#define SCCOPT_IMAGE_PASSTHROUGH                            SCCID_IMAGE_PASSTHROUGH
#define SCCOPT_FONTEMBEDPOLICY                              SCCID_FONTEMBEDPOLICY

/*
|   Watermark options
*/
#define SCCOPT_ENABLEWATERMARK                              SCCID_ENABLEWATERMARK
#define SCCOPT_WATERMARKPOSITION                            SCCID_WATERMARKPOSITION
#define SCCOPT_WATERMARKIO                                  SCCID_WATERMARKIO

/*
|   Page layout options
*/
#define SCCOPT_DEFAULTPAGESIZE                              SCCID_DEFAULTPAGESIZE
#define SCCOPT_FONTDIRECTORY                                SCCID_FONTDIRECTORY
#define SCCOPT_EMBEDFONTS                                   SCCID_EMBEDFONTS
/*
|   Clipboard Options
*/
#define SCCOPT_TOCLIPBOARD                                  SCCID_TOCLIPBOARD
#define SCCOPT_DEFAULTCLIPBOARDFONT                         SCCID_DEFAULTCLIPBOARDFONT

/*
|   System Options
*/
#define SCCOPT_DIALOGFLAGS                                  SCCID_DIALOGFLAGS
#define SCCOPT_SYSTEMFLAGS                                  SCCID_SYSTEMFLAGS
#define SCCOPT_INTLFLAGS                                    SCCID_INTLFLAGS
#define SCCOPT_SCROLLFLAGS                                  SCCID_SCROLLFLAGS
#define SCCOPT_OLEFLAGS                                     SCCID_OLEFLAGS
#define SCCOPT_FIFLAGS                                      SCCID_FIFLAGS
#define SCCOPT_TEMPDIR                                      SCCID_TEMPDIR
#define SCCOPT_TEMPDIREX                                    SCCID_TEMPDIREX
#define SCCOPT_NATIVEFILTERFLAG                             SCCID_NATIVEFILTERFLAG
#define SCCOPT_TEMPFUNC                                     SCCID_TEMPFUNC
#define SCCOPT_DOCUMENTMEMORYMODE							SCCID_DOCUMENTMEMORYMODE

#define SCCOPT_RENDERING_PREFER_OIT                         SCCID_RENDERING_PREFER_OIT
#define SCCOPT_RENDER_DISABLEALPHABLENDING                  SCCID_RENDER_DISABLEALPHABLENDING
#define SCCOPT_RENDER_ENABLEALPHABLENDING                   SCCID_RENDER_ENABLEALPHABLENDING

/* compression licensing options (to disable patented methods) */

#define SCCOPT_FILTERLZW                                    SCCID_FILTERLZW
#define SCCOPT_FILTERJPG                                    SCCID_FILTERJPG

/* Option to turn off pagebreaks... For example, on very large system dumps. Used by the text filter. */
#define SCCOPT_ASCIIFILTERPAGEOFF									SCCID_ASCIIFILTERPAGEOFF

/* Excel option to disable putting out blank cells after last valid data cell */
#define SCCOPT_FILTERNOBLANK                                SCCID_FILTERNOBLANK

/* Presentation option to enable putting out timing (animation) data */
#define SCCOPT_FILTER_PARSE_TIMING_DATA                        SCCID_FILTER_PARSE_TIMING_DATA

/* Option to ignore verification of the PST File password */
#define SCCOPT_IGNORE_PASSWORD                              SCCID_IGNORE_PASSWORD

/* Option to reorder BIDI text in PDF files */
#define SCCOPT_PDF_FILTER_REORDER_BIDI						SCCID_PDF_FILTER_REORDER_BIDI

/* Option to Drop Hyphens from end of the line in PDF files */
#define SCCOPT_PDF_FILTER_DROPHYPHENS						SCCID_PDF_FILTER_DROPHYPHENS

/* PDF filter spacing threshold */
#define SCCOPT_PDF_FILTER_WORD_DELIM_FRACTION               SCCID_PDF_FILTER_WORD_DELIM_FRACTION

/* PDF filter embedded objects limit */
#define SCCOPT_PDF_FILTER_MAX_EMBEDDED_OBJECTS              SCCID_PDF_FILTER_MAX_EMBEDDED_OBJECTS

/* PDF filter vector paths limit */
#define SCCOPT_PDF_FILTER_MAX_VECTOR_PATHS                  SCCID_PDF_FILTER_MAX_VECTOR_PATHS

/*option to set html input filter's behavior towards conditional comments.*/
#define SCCOPT_HTML_COND_COMMENT_MODE                       SCCID_HTML_COND_COMMENT_MODE

/* Excel option to output revision history */
#define SCCOPT_GENERATEEXCELREVISIONS						SCCID_GENERATEEXCELREVISIONS

/* Mime option to restrict the number of viewable email headers.*/
/* depreciating SCCOPT_WPMIMEHEADEROUTPUT, but leaving it for backward compatibility */
#define SCCOPT_WPMIMEHEADEROUTPUT							SCCID_WPMIMEHEADEROUTPUT
#define SCCOPT_WPEMAILHEADEROUTPUT							SCCID_WPEMAILHEADEROUTPUT

#define SCCOPT_MAILHEADERVISIBLE							SCCID_MAILHEADERVISIBLE
#define SCCOPT_MAILHEADERHIDDEN								SCCID_MAILHEADERHIDDEN
#define SCCOPT_MAILHEADERCUSTOM								SCCID_MAILHEADERCUSTOM

/* antialiasing options */
#define SCCOPT_ANTIALIAS                                    SCCID_ANTIALIAS

/* Option to enable output of full TIFF/EXIF properties */
#define SCCOPT_FILTEREXIFFULL                               SCCID_FILTEREXIFFULL

/*
|   Archive Flags
*/
#define SCCOPT_ARCSAVEEVENT                                 SCCID_ARCSAVEEVENT
#define SCCOPT_ARCSORTORDER                                 SCCID_ARCSORTORDER
#define SCCOPT_ARCOUTPUTPATH                                SCCID_ARCOUTPUTPATH
#define SCCOPT_ARCFLAGS                                     SCCID_ARCFLAGS
#define SCCOPT_ARCSAVETHIS                                  SCCID_ARCSAVETHIS
#define SCCOPT_ARCVIEWTHIS                                  SCCID_ARCVIEWTHIS
#define SCCOPT_ARCFULLPATH                                  SCCID_ARCFULLPATH
#define SCCOPT_ARCRENAME                                    SCCID_ARCRENAME

/*
|   International Options
*/
#define SCCOPT_NUMBERFORMAT                                 SCCID_NUMBERFORMAT
#define SCCOPT_DAYNAMES                                     SCCID_DAYNAMES
#define SCCOPT_MONTHNAMES                                   SCCID_MONTHNAMES

#define SCCOPT_TIMEZONE                                     SCCID_TIMEZONE

/*
|   Bidirectional Text Options
*/
#define SCCOPT_REORDERMETHOD                                SCCID_REORDERMETHOD

/*
|   Unix Options file name option
*/
#define SCCOPT_GETUNIXOPTIONSFILENAME                       SCCID_GETUNIXOPTIONSFILENAME

/*
|   Option for running filter in optimized mode
*/
#define SCCOPT_FILTERPERFORMANCE                            SCCID_FILTERPERFORMANCE
#define SCCOPT_ENABLEALLSUBOBJECTS                          SCCID_ENABLEALLSUBOBJECTS   /* This will override filter performance */
#define SCCOPT_FILTERRESERVED                               SCCID_FILTERRESERVED
#define SCCOPT_FILTERDISABLETABLEOUTPUT                     SCCID_FILTERDISABLETABLEOUTPUT
/* 
|   Option for filter handle xmp metadata 
*/
#define SCCOPT_PARSEXMPMETADATA                             SCCID_PARSEXMPMETADATA

/*
|     Html 5 options
*/
#define SCCOPT_OUTPUT_STRUCTURE                          SCCID_OUTPUT_STRUCTURE
#define SCCOPT_URLPATH_RESOURCES                         SCCID_URLPATH_RESOURCES
#define SCCOPT_URLPATH_OUTPUT                            SCCID_URLPATH_OUTPUT
#define SCCOPT_POST_LIBRARY_SCRIPT                       SCCID_POST_LIBRARY_SCRIPT
#define SCCOPT_PRE_LIBRARY_SCRIPT                        SCCID_PRE_LIBRARY_SCRIPT
#define SCCOPT_OUTPUT_RAWTEXT                            SCCID_OUTPUT_RAWTEXT
#define SCCOPT_FONT_PERMISSIONS_MODE                     SCCID_FONT_PERMISSIONS_MODE
#define SCCOPT_FONT_BASE_URL                             SCCID_FONT_BASE_URL
#define SCCOPT_FONT_REFERENCE_METHOD                     SCCID_FONT_REFERENCE_METHOD
#define SCCOPT_EMAIL_ATTACHMENT_HANDLING                 SCCID_EMAIL_ATTACHMENT_HANDLING
#define SCCOPT_EXTERNAL_STYLESHEET                       SCCID_EXTERNAL_STYLESHEET
#define SCCOPT_STAMP_IMAGE_FILE                          SCCID_STAMP_IMAGE_FILE
#define SCCOPT_STAMP_IMAGE_URL                           SCCID_STAMP_IMAGE_URL
#define SCCOPT_WV_LIBRARY_NAME                           SCCID_WV_LIBRARY_NAME
#define SCCOPT_WV_STYLESHEET_NAME                        SCCID_WV_STYLESHEET_NAME
#define SCCOPT_REDACTION_COLOR                           SCCID_REDACTION_COLOR
#define SCCOPT_REDACTION_LABEL_FONT_NAME                 SCCID_REDACTION_LABEL_FONT_NAME
#define SCCOPT_REDACTION_LABEL_FONT_SIZE                 SCCID_REDACTION_LABEL_FONT_SIZE
#define SCCOPT_REDACTIONS_ENABLED                        SCCID_REDACTIONS_ENABLED
#define SCCOPT_SHOW_REDACTION_LABELS                     SCCID_SHOW_REDACTION_LABELS
#define SCCOPT_MARGIN_TEXT_FONT_NAME                     SCCID_MARGIN_TEXT_FONT_NAME
#define SCCOPT_MARGIN_TEXT_FONT_SIZE                     SCCID_MARGIN_TEXT_FONT_SIZE
#define SCCOPT_MARGIN_TEXT_LINE                          SCCID_MARGIN_TEXT_LINE
#define SCCOPT_VECTOROBJECTLIMIT                         SCCID_VECTOROBJECTLIMIT




/*
|	FNT embedded font option
*/
#define SCCOPT_RENDER_EMBEDDED_FONTS					 SCCID_RENDER_EMBEDDED_FONTS
/*
|   Structures used for options.
*/

/* Internal options */
#define SCCOPT_PAGEINFO_REQUIREDDATA           1001


typedef struct OPTIONITEMtag
{
    VTDWORD dwItemId;                /* Unique id */
    VTDWORD dwSize;                  /* Size of pData */
    VTBYTE *pData;
    struct OPTIONITEMtag *next;
} SCCUTOPTIONITEM, *PSCCUTOPTIONITEM;

typedef struct
{
    VTDWORD dwLargest;                /* Size of the largest item in this list */
    VTDWORD dwLength;                 /* Number of items in this list */
    VTDWORD dwUnique;                 /* The next unique id to assgn to an item */
    PSCCUTOPTIONITEM pCurrent;        /* The current item(for GETFIRST and GETNEXT) */
    PSCCUTOPTIONITEM head;            /* Top 'o the list */
} SCCUTOPTIONITEMLIST, *PSCCUTOPTIONITEMLIST;

typedef struct SCCUTEMAILHEADERINFOtag
{
    VTDWORD dwHeaderID;                               /* Email header id, eg SO_MAIL_TO */
    VTDWORD dwSubtypeID;                              /* SO_MAILTYPE_CONTACT, NOTE, etc. */
    VTWCHAR wsMimeHeaderName[SCCUT_MAIL_NAMELENGTH];  /* Custom header name(NULL if standard) */
    VTWCHAR wsMimeHeaderLabel[SCCUT_MAIL_NAMELENGTH]; /* Label to use for rendering(also NULL if standard) */
} SCCUTEMAILHEADERINFO, *PSCCUTEMAILHEADERINFO;

typedef struct SCCUTEMAILOPTIONStag
{
    VTWORD wHeaderArray[SCCUT_MAIL_ARRAYSIZE];
    SCCUTEMAILHEADERINFO sCustom[MAX_NONSTANDARD_HEADERS];  
} SCCUTEMAILOPTIONS, *PSCCUTEMAILOPTIONS;

typedef struct SCCUTPRINTMARGINStag
{
    VTDWORD dwTop;
    VTDWORD dwBottom;
    VTDWORD dwLeft;
    VTDWORD dwRight;
} SCCUTPRINTMARGINS, * PSCCUTPRINTMARGINS;

typedef struct SCCUTTEMPDIRSPECtag
{
    VTDWORD dwSize;
    VTDWORD dwSpecType;
    VTTCHAR szTempDirName[SCCUT_FILENAMEMAX];
} SCCUTTEMPDIRSPEC, * PSCCUTTEMPDIRSPEC;

#define MAX_TEMPDIR_PATH_LENGTH 8192

typedef struct SCCUTTEMPDIRSPECEXtag
{
    VTDWORD dwSize;
    VTDWORD dwSpecType;
    VTTCHAR szTempDirName[MAX_TEMPDIR_PATH_LENGTH];
} SCCUTTEMPDIRSPECEX, * PSCCUTTEMPDIRSPECEX;

typedef struct SCCUTTEMPFILSPECtag
{
    VTLPVOID pViewInfo;
    VWCALLBACKPROC pCallback;
/*
|   For now, dwCallbackData will only be valid when an export app uses this
|   structure.
*/
    VTDWORD dwCallbackData;
} SCCUTTEMPFILSPEC, * PSCCUTTEMPFILSPEC;

/*
|   Struct used by OI Export filters to recieve the default font.
*/
#define SCCUT_MAXFALLBACKFONTLEN    40      /* Max len of *pName in characters (not bytes) */
typedef struct SCCUTFALLBACKFONTtag
{
    VTLPVOID pName;
    VTWORD   wType;
} SCCUTFALLBACKFONT, * LPSCCUTFALLBACKFONT;

typedef struct SCCUTFONTSPECtag
{
    VTTCHAR szFace[40];
    VTWORD wHeight;
    VTWORD wAttr;
    VTWORD wType;
} SCCUTFONTSPEC, * LPSCCUTFONTSPEC;

#define SCCUT_FONTALIAS_COUNT               0x0001
#define SCCUT_FONTALIAS_ALIASNAME           0x0002
#define SCCUT_FONTALIAS_GETALIASBYID        0x0004
#define SCCUT_FONTALIAS_GETALIASID          0x0008
#define SCCUT_FONTALIAS_REMOVEALIASBYID     0x0010
#define SCCUT_FONTALIAS_REMOVEALIASBYNAME   0x0020
#define SCCUT_FONTALIAS_REMOVEALL           0x0040
#define SCCUT_FONTALIAS_USEDEFAULTS         0x0080

#define SCCUT_FONTNAMEMAX                   128
#define SCCUT_MAXFONTMAPENTRIES             100
#define SCCUT_MAXALIASES                     16

typedef struct SCCUTFONTALIAS832tag
{
    VTDWORD dwSize;                                         /* sizeof(SCCUTFONTALIAS) */
    VTDWORD dwAliasID;                                      /* ID of the aliasing in the current list of aliases */
    VTDWORD dwFlags;                                        /* set of flags */
    VTWORD  szwOriginal[SCCUT_FONTNAMEMAX];                 /* original name of the font */
    VTWORD  szwAlias[SCCUT_FONTNAMEMAX];                    /* new font name to use in place of original name */
} SCCUTFONTALIAS832, * PSCCUTFONTALIAS832;

typedef struct SCCUTFONTALIAStag
{
    VTDWORD dwSize;                                           /* sizeof( SCCUTFONTALIAS ) */
    VTDWORD dwAliasID;                                        /* ID of the aliasing in the current list of aliases */
    VTDWORD dwFlags;                                          /* set of flags */
    VTWORD  szwOriginal[ SCCUT_FONTNAMEMAX ];                 /* original name of the font */
    VTWORD  szwAlias[ SCCUT_FONTNAMEMAX * SCCUT_MAXALIASES ]; /* new font name(s) to use in place of original name */
} SCCUTFONTALIAS, * PSCCUTFONTALIAS;

typedef struct SCCUTFONTALIASMAPtag
{
    VTDWORD     dwCount;                                /* number of entries in the map */
    VTHANDLE    hOriginal[SCCUT_MAXFONTMAPENTRIES];     /* memory handles for original font names */
    VTHANDLE    hAlias[SCCUT_MAXFONTMAPENTRIES];        /* memory handles for alias font names */
    VTLPWORD    szwOriginal[SCCUT_MAXFONTMAPENTRIES];   /* array of all the original font names */
    VTLPWORD    szwAlias[SCCUT_MAXFONTMAPENTRIES];      /* array of all the alias font names */
} SCCUTFONTALIASMAP, * PSCCUTFONTALIASMAP;

typedef struct FONTNAMELISTtag *PFONTNAMELIST;
typedef struct FONTNAMELISTtag
{
    VTBYTE          szFontName[SCCUT_FILENAMEMAX];          /* Name of font to include or exclude*/
    PFONTNAMELIST   pNextFont;                              /* Pointer to a FONTNAMELIST structure that contains the name of the next font to include or exclude*/   
}FONTNAMELIST;

typedef struct FONTFILTERLISTtag
{
    VTBOOL          bExclude;                               /* If true then the accompaning font list is an exclusion list, if false the list is an inclusion list*/
    PFONTNAMELIST   pFontList;                              /* Pointer to a FONTNAMELIST structure that contains the names of the fonts to include or exclude*/
}FONTFILTERLIST;

typedef struct SCCUTUNIXOPTIONSFILEINFOtag
{
    VTDWORD dwSize;                                         /* sizeof(SCCVWUNIXOPTIONSFILEINFO) */
    VTBYTE  szFName[SCCUT_FILENAMEMAX];                     /* array holding the name of the options file */
}SCCUTUNIXOPTIONSFILEINFO;

typedef struct SCCUTIMAGEZOOMtag
{
    VTDWORD  dwXNu;
    VTDWORD  dwXDe;
    VTDWORD  dwYNu;
    VTDWORD  dwYDe;
} SCCUTIMAGEZOOM, * PSCCUTIMAGEZOOM;

#define SCCUT_RESOURCEIDMAX 6
typedef struct SCCUTRESOURCEIDtag
{
    VTDWORD dwSize;                                         /* sizeof(SCCVWRESOURCEID) */
    VTTCHAR szResourceID[SCCUT_RESOURCEIDMAX];              /* 0-5 character ID defining version of the LO DLL w/ unique resources */
} SCCUTRESOURCEID, * PSCCUTRESOURCEID;

typedef struct SCCUTNUMBERFORMAT775tag
{
    VTTCHAR cDecimalSep;
    VTTCHAR cThousandSep;
    VTTCHAR cDateSep;
    VTTCHAR cTimeSep;
    VTTCHAR szCurrencySymbol[8];
    VTTCHAR szAM[8];
    VTTCHAR szPM[8];
    VTDWORD dwNumBytesAM;  /* store the number of bytes in the "AM" string */
    VTDWORD dwNumBytesPM;  /* store the number of bytes in the "PM" string */
    VTWORD wCurrencyPosition;
    VTWORD wShortDateOrder;
} SCCUTNUMBERFORMAT775, * PSCCUTNUMBERFORMAT775;

typedef struct SCCUTNUMBERFORMATtag
{
    VTTCHAR cDecimalSep;
    VTTCHAR cThousandSep;
    VTTCHAR cDateSep;
    VTTCHAR cTimeSep;
    VTTCHAR szCurrencySymbol[8];
    VTTCHAR szAM[8];
    VTTCHAR szPM[8];
    VTDWORD dwNumBytesAM;  /* store the number of bytes in the "AM" string */
    VTDWORD dwNumBytesPM;  /* store the number of bytes in the "PM" string */
    VTWORD  wCurrencyPosition;
    VTWORD  wShortDateOrder;
    VTWORD  wShortDateYearDigits;
    VTWORD  wShortDateMonthDigits;
    VTWORD  wShortDateDayDigits;
    VTWORD  wShortDateFlags;
} SCCUTNUMBERFORMAT, * PSCCUTNUMBERFORMAT;

typedef struct SCCUTDAYNAMEStag
{
    VTWORD aFullNames[7][16];
    VTWORD aAbbrevNames[7][16];
} SCCUTDAYNAMES, * PSCCUTDAYNAMES;

typedef struct SCCUTMONTHNAMEStag
{
    VTWORD aFullNames[13][16];
    VTWORD aAbbrevNames[13][16];
} SCCUTMONTHNAMES, * PSCCUTMONTHNAMES;

typedef struct SCCUTTIFFOPTIONStag
{
    VTDWORD dwFormat;
    VTDWORD dwCompression;
    VTDWORD dwColorTransition;
    VTDWORD XResolution;
    VTDWORD YResolution;
} SCCUTTIFFOPTIONS, * PSCCUTTIFFOPTIONS;

typedef struct
{
    VTDWORD     dwSize;
    VTDWORD     dwColorSpace;
    VTDWORD     dwCompression;
    VTDWORD     dwByteOrder;
    VTDWORD     dwTIFFFlags;
    VTDWORD     dwFillOrder;
} EXTIFFOPTIONS, * LPEXTIFFOPTIONS;

typedef struct WATERMARKPOStag
{
    VTDWORD dwWatermarkPos;
    VTLONG  lVerticalPos;
    VTLONG  lHorizontalPos;
}WATERMARKPOS, *LPWATERMARKPOS;

typedef struct WATERMARKPATHtag
{
    VTCHAR  szWaterMarkPath[SCCUT_FILENAMEMAX];
    VTDWORD dwMaxSize;
}WATERMARKPATH, *LPWATERMARKPATH; 

typedef struct WATERMARKIOtag
{
    VTVOID*         phDoc;
    VTDWORD         dwType;
    WATERMARKPATH   Path;
    VTDWORD         dwScalingMethod;
    VTDWORD         dwScalePercent;
}WATERMARKIO, *LPWATERMARKIO; 

typedef struct DEFAULTPAGESIZEtag
{
    VTFLOAT fHeight;
    VTFLOAT fWidth;
    VTDWORD wUnits;
}DEFAULTPAGESIZE, *LPDEFAULTPAGESIZE;

#define  MAX_DEF_REFERENCE_LENGTH  1024

typedef struct SCCUTXXDEFREFtag
{
    VTDWORD dwSize;
    VTBYTE  utf8RefName[MAX_DEF_REFERENCE_LENGTH];
}SCCUTXXDEFREFINFO;

#define SCCUTFORMATLIST_LENGTH  128

typedef struct SCCUTFORMATLISTtag
{
    VTDWORD   dwSize;
    VTDWORD   dwFormats[SCCUTFORMATLIST_LENGTH];
} SCCUTFORMATLIST, * PSCCUTFORMATLIST;


/* Here lies the structure/values to support the SCCOPT_IO_BUFFERSIZE option */

typedef struct SCCBUFFEROPTIONStag
{
    VTDWORD dwReadBufferSize;   /* size of the default I/O Read buffer in KB        */
    VTDWORD dwMMapBufferSize;   /* size of the default I/O Memory Map buffer in KB  */
    VTDWORD dwTempBufferSize;   /* Maximum amount of in-memory cache per file in KB */
    VTDWORD dwFlags;            /* set flags                                        */
} SCCBUFFEROPTIONS, *PSCCBUFFEROPTIONS;

/* dwFlags values */
#define SCCBUFOPT_SET_READBUFSIZE   1
#define SCCBUFOPT_SET_MMAPBUFSIZE   2
#define SCCBUFOPT_SET_TEMPBUFSIZE   4

/* stock buffer sizes in KB */
#define SCCBUFOPT_DEFAULT_READBUFSIZE      2
#define SCCBUFOPT_DEFAULT_MMAPBUFSIZE   8192
#define SCCBUFOPT_DEFAULT_TEMPBUFSIZE   2048

#define SCCBUFOPT_MIN_READBUFSIZE          1
#define SCCBUFOPT_MIN_MMAPBUFSIZE          0
#define SCCBUFOPT_MIN_TEMPBUFSIZE          0

#define SCCBUFOPT_MAX_READBUFSIZE 0x003fffff
#define SCCBUFOPT_MAX_MMAPBUFSIZE 0x003fffff
#define SCCBUFOPT_MAX_TEMPBUFSIZE 0x003fffff

/*
|   General Structures Used in Option Processing
|
|   Note:   If you change a struct, the old struct must be kept.
|           Name it UTSTRUCTNAMEXXX where XXX is the previous release.
|           Then, create a copy of the UTOPTIONS struct (named UTOPTIONSXXX),
|           and replace UTSTRUCTNAME with UTSTRUCTNAMEXXX.  This is so that
|           new versions of the technology can read in old options files.
|           Also, add new members to structs at the END of the struct!
*/

typedef struct UTPRINTOPTIONStag
{
    SCCUTPRINTMARGINS sMargins;
    VTBOOL            bHeader;
    SCCUTFONTSPEC     sHeaderFont;
    SCCUTFONTSPEC     sDefaultFont;
    VTDWORD           dwWhatToPrint;
    VTDWORD           dwStartPage;
    VTDWORD           dwEndPage;
    VTBOOL            bCollate;
    VTDWORD           dwCopies;
    VTTCHAR           szJobName[128];
    VTBOOL            bUseDocPageSettings;
    SCCUTFONTALIASMAP sFontAlias;
} UTPRINTOPTIONS, * PUTPRINTOPTIONS;

typedef struct UTDISPLAYOPTIONS521tag
{
    SCCUTFONTSPEC     sDefaultFont;
    VTDWORD           dwFallbackFormat;
    VTBOOL            bHaveSelection;
    VTWORD            wDummy;
    VTDWORD           aDummy[5];
    VTDWORD           dwScaleFactor;        /* ranges from 40 to 300, with 100 as the default */
    VTDWORD           dwOriginalFontScale;
    SCCUTFONTALIASMAP sFontAlias;
} UTDISPLAYOPTIONS521, * PUTDISPLAYOPTIONS521;

typedef struct UTDISPLAYOPTIONStag
{
    SCCUTFONTSPEC     sDefaultFont;
    VTDWORD           dwFallbackFormat;
    VTBOOL            bHaveSelection;
    VTWORD            wDummy;
    VTDWORD           aDummy[5];
    VTDWORD           dwScaleFactor;        /* ranges from 40 to 300, with 100 as the default */
    VTDWORD           dwOriginalFontScale;
    SCCUTFONTALIASMAP sFontAlias;
    VTDWORD           dwReorderMethod;
} UTDISPLAYOPTIONS, * PUTDISPLAYOPTIONS;

typedef struct UTSSOPTIONS75tag
{
    VTDWORD dwClipboard;
    VTBOOL  bShowGridlines;
    VTBOOL  bPrintGridlines;
    VTBOOL  bPrintHeadings;
    VTDWORD dwPrintFitToPage;
    VTBOOL  bDraftMode;
    VTDWORD dwPrintScalePercent;
    VTDWORD dwPrintScaleXWide;
    VTDWORD dwPrintScaleXHigh;
    VTDWORD dwPrintDirection;
} UTSSOPTIONS75, * PUTSSOPTIONS75;

typedef struct UTSSOPTIONS832tag
{
    VTDWORD dwClipboard;
    VTBOOL  bShowGridlines;
    VTBOOL  bPrintGridlines;
    VTBOOL  bPrintHeadings;
    VTDWORD dwPrintFitToPage;
    VTBOOL  bDraftMode;
    VTDWORD dwPrintScalePercent;
    VTDWORD dwPrintScaleXWide;
    VTDWORD dwPrintScaleXHigh;
    VTDWORD dwPrintDirection;
    VTBOOL  bShowHeadings;
} UTSSOPTIONS832, *PUTSSOPTIONS832;

typedef struct UTSSOPTIONStag
{
    VTDWORD dwClipboard;
    VTDWORD dwPrintFitToPage;
    VTDWORD dwPrintScalePercent;
    VTDWORD dwPrintScaleXWide;
    VTDWORD dwPrintScaleXHigh;
    VTDWORD dwPrintDirection;
    VTDWORD dwSSFlags;
} UTSSOPTIONS, *PUTSSOPTIONS;

typedef struct UTDBOPTIONStag
{
    VTDWORD dwClipboard;
    VTBOOL  bShowGridlines;
    VTBOOL  bPrintGridlines;
    VTBOOL  bClipboardHeadings;
    VTBOOL  bPrintHeadings;
    VTBOOL  bFieldNamesToClip;
    VTDWORD dwPrintFitToPage;
    VTBOOL  bDraftMode;
} UTDBOPTIONS, * PUTDBOPTIONS;

typedef struct UTWATERMARKOPTIONStag
{
    VTBOOL          bEnableWatermark;
    WATERMARKPOS    WatermarkPos;
    WATERMARKIO     WatermarkIO;
} UTWATERMARKOPTIONS, *PUTWATERMARKOPTIONS;

typedef struct SCCFONTOUTPUTtag
{
    VTDWORD         dwOutput;
    VTBYTE         *szFontName;
} SCCFONTOUTPUT, *PSCCFONTOUTPUT;

typedef struct SCCFONTOUTPUTURLtag
{
    VTBYTE         *szFontName;
    VTBYTE         *szUrl;
} SCCFONTOUTPUTURL, *PSCCFONTOUTPUTURL;



typedef struct UTSCRIPTtag
{
    VTDWORD         dwSize;
    VTBYTE        **ppScripts;
} UTSCRIPT, *PUTSCRIPT;

typedef struct UTWPOPTIONS820tag
{
	VTDWORD			dwMode;
	SCCUTIMAGEZOOM	sZoom;
	VTDWORD			dwFitMode;
	VTDWORD			dwWrapToWindow;
	VTBOOL 			bDraftPrintMode;
	VTBOOL 			bDisableHyperlinks;
	VTBOOL 			bDisablePositionalFrames;
} UTWPOPTIONS820, * PUTWPOPTIONS820;

typedef struct UTWPOPTIONStag
{
    VTDWORD         dwMode;
    SCCUTIMAGEZOOM  sZoom;
    VTDWORD         dwFitMode;
    VTDWORD         dwWrapToWindow;
    VTBOOL          bDraftPrintMode;
    VTBOOL          bDisableHyperlinks;
    VTBOOL          bDisablePositionalFrames;
	VTDWORD			dwWPFlags;
} UTWPOPTIONS, * PUTWPOPTIONS;

typedef struct UTARCOPTIONS600tag
{
    VTDWORD dwSaveEvent;
    VTDWORD dwSortOrder;
    VTDWORD dwArcFlags;
    VTTCHAR  szOutputDir[260];
} UTARCOPTIONS600, * PUTARCOPTIONS600;

typedef struct UTARCOPTIONS77tag
{
    VTDWORD dwSaveEvent;
    VTDWORD dwSortOrder;
    VTDWORD dwArcFlags;
    VTTCHAR szOutputDir[260];
    VTBOOL  bFullPath;
} UTARCOPTIONS77, * PUTARCOPTIONS77;

typedef struct UTARCOPTIONStag
{
    VTDWORD dwSaveEvent;
    VTDWORD dwSortOrder;
    VTDWORD dwArcFlags;
    VTTCHAR szOutputDir[260];
    VTBOOL  bFullPath;
    VTDWORD dwPromptDiag;
} UTARCOPTIONS, * PUTARCOPTIONS;

typedef struct UTBMPOPTIONStag
{
    VTDWORD             dwFitMode;
    VTDWORD             dwFitPosition;
    SCCUTIMAGEZOOM      sZoom;
    VTDWORD             dwRotation;
    VTBOOL              bDither;
    VTBOOL              bDitherAvailable;
    VTDWORD             dwPrintAspect;
    VTBOOL              bPrintBorder;
    VTBOOL              bShowFullScreen;
    VTDWORD             dwZoomEvent;
} UTBMPOPTIONS, * PUTBMPOPTIONS;

typedef struct UTVECOPTIONStag
{
    VTDWORD             dwFitMode;
    SCCUTIMAGEZOOM      sZoom;
    VTDWORD             dwPrintAspect;
    VTBOOL              bPrintBorder;
    VTBOOL              bShowFullScreen;
    VTDWORD             dwZoomEvent;
    VTBOOL              bShowBackground;
    VTBOOL              bPrintBackground;
    VTBOOL              bDraftMode;
} UTVECOPTIONS, * PUTVECOPTIONS;

typedef struct UTOPTIONDELTA521tag
{
    VTDWORD dwCount;
    VTDWORD aChanges[20];
} UTOPTIONDELTA521, * PUTOPTIONDELTA521;

typedef struct UTPDFOPTIONStag
{
    VTFLOAT     fWordDelimFraction;
    VTDWORD     dwMaxEmbeddedObjects;
    VTDWORD     dwMaxVectorPaths;
} UTPDFOPTIONS, *PUTPDFOPTIONS;

/* NB! If this number is incremented, a legacy UTOPTIONDELTA structure must be
 * created. Back in 832 it was incremented by 1, so we need to track that. If
 * you need a larger options delta structure, make it *much* larger (200?) so
 * we don't need to mess with this again in the near future.
 */
#define SCCUT_MAX_OPTIONTRACK830    120
#define SCCUT_MAX_OPTIONTRACK835    121
#define SCCUT_MAX_OPTIONTRACK       200

typedef struct UTOPTIONDELTAtag830
{
    VTDWORD dwCount;
    VTDWORD aChanges[SCCUT_MAX_OPTIONTRACK830];
} UTOPTIONDELTA830, * PUTOPTIONDELTA830;

typedef struct UTOPTIONDELTAtag835
{
    VTDWORD dwCount;
    VTDWORD aChanges[SCCUT_MAX_OPTIONTRACK835];
} UTOPTIONDELTA835, * PUTOPTIONDELTA835;

typedef struct UTOPTIONDELTAtag
{
    VTDWORD dwCount;
    VTDWORD aChanges[SCCUT_MAX_OPTIONTRACK];
} UTOPTIONDELTA, * PUTOPTIONDELTA;

typedef struct UTCLIPOPTIONStag
{
    VTDWORD         dwToClipboard;
    SCCUTFONTSPEC   sDefaultFont;
} UTCLIPOPTIONS, * PUTCLIPOPTIONS;

typedef struct UTSYSTEMOPTIONS521tag
{
    VTDWORD         dwDialogFlags;
    VTDWORD         dwSystemFlags;
    VTDWORD         dwIntlFlags;
    VTDWORD         dwScrollFlags;
    VTDWORD         dwHardOptions;
    VTDWORD         dwOleFlags;
    SCCUTRESOURCEID sResourceLibraryID;
} UTSYSTEMOPTIONS521, * PUTSYSTEMOPTIONS521;

typedef struct UTSYSTEMOPTIONS610tag
{
    VTDWORD         dwDialogFlags;
    VTDWORD         dwSystemFlags;
    VTDWORD         dwIntlFlags;
    VTDWORD         dwScrollFlags;
    VTDWORD         dwHardOptions;
    VTDWORD         dwOleFlags;
    SCCUTRESOURCEID sResourceLibraryID;
    VTWORD          wUnmappableChar;
    VTDWORD         dwFIFlags;
} UTSYSTEMOPTIONS610, * PUTSYSTEMOPTIONS610;

typedef struct UTSYSTEMOPTIONS70tag
{
    VTDWORD          dwDialogFlags;
    VTDWORD          dwSystemFlags;
    VTDWORD          dwIntlFlags;
    VTDWORD          dwScrollFlags;
    VTDWORD          dwHardOptions;
    VTDWORD          dwOleFlags;
    SCCUTRESOURCEID  sResourceLibraryID;
    VTWORD           wUnmappableChar;
    VTDWORD          dwFIFlags;
    SCCUTTEMPDIRSPEC sTempDirName;
    VTBOOL           bUseNativeFilter;
} UTSYSTEMOPTIONS70, * PUTSYSTEMOPTIONS70;

typedef struct UTSYSTEMOPTIONS71tag
{
    VTDWORD          dwDialogFlags;
    VTDWORD          dwSystemFlags;
    VTDWORD          dwIntlFlags;
    VTDWORD          dwScrollFlags;
    VTDWORD          dwHardOptions;
    VTDWORD          dwOleFlags;
    SCCUTRESOURCEID  sResourceLibraryID;
    VTWORD           wUnmappableChar;
    VTDWORD          dwFIFlags;
    SCCUTTEMPDIRSPEC sTempDirName;
    VTBOOL           bUseNativeFilter;
    SCCUTTEMPFILSPEC sTmpNameCallback;

} UTSYSTEMOPTIONS71, * PUTSYSTEMOPTIONS71;

typedef struct UTSYSTEMOPTIONS77tag
{
    VTDWORD          dwDialogFlags;
    VTDWORD          dwSystemFlags;
    VTDWORD          dwIntlFlags;
    VTDWORD          dwScrollFlags;
    VTDWORD          dwHardOptions;
    VTDWORD          dwOleFlags;
    SCCUTRESOURCEID  sResourceLibraryID;
    VTWORD           wUnmappableChar;
    VTDWORD          dwFIFlags;
    SCCUTTEMPDIRSPEC sTempDirName;
    VTBOOL           bUseNativeFilter;
    SCCUTTEMPFILSPEC sTmpNameCallback;
    VTDWORD          dwFilterFlags;

} UTSYSTEMOPTIONS77, * PUTSYSTEMOPTIONS77;

typedef struct UTSYSTEMOPTIONS80tag
{
    VTDWORD          dwDialogFlags;
    VTDWORD          dwSystemFlags;
    VTDWORD          dwIntlFlags;
    VTDWORD          dwScrollFlags;
    VTDWORD          dwHardOptions;
    VTDWORD          dwOleFlags;
    SCCUTRESOURCEID  sResourceLibraryID;
    VTWORD           wUnmappableChar;
    VTDWORD          dwFIFlags;
    SCCUTTEMPDIRSPEC sTempDirName;
    VTBOOL           bUseNativeFilter;
    SCCUTTEMPFILSPEC sTmpNameCallback;
    VTDWORD          dwFilterFlags;
    VTBOOL           bRenderingPreferOIT;

} UTSYSTEMOPTIONS80, * PUTSYSTEMOPTIONS80;

typedef struct UTSYSTEMOPTIONS81tag
{
    VTDWORD          dwDialogFlags;
    VTDWORD          dwSystemFlags;
    VTDWORD          dwIntlFlags;
    VTDWORD          dwScrollFlags;
    VTDWORD          dwHardOptions;
    VTDWORD          dwOleFlags;
    SCCUTRESOURCEID  sResourceLibraryID;
    VTWORD           wUnmappableChar;
    VTDWORD          dwFIFlags;
    SCCUTTEMPDIRSPEC sTempDirName;
    VTBOOL           bUseNativeFilter;
    SCCUTTEMPFILSPEC sTmpNameCallback;
    VTDWORD          dwFilterFlags;
    VTBOOL           bRenderingPreferOIT;

} UTSYSTEMOPTIONS81, * PUTSYSTEMOPTIONS81;

typedef struct UTSYSTEMOPTIONS82tag
{
    VTDWORD          dwDialogFlags;
    VTDWORD          dwSystemFlags;
    VTDWORD          dwIntlFlags;
    VTDWORD          dwScrollFlags;
    VTDWORD          dwHardOptions;                
    VTDWORD          dwOleFlags;
    SCCUTRESOURCEID  sResourceLibraryID;
    VTWORD           wUnmappableChar;
    VTDWORD          dwFIFlags;
    SCCUTTEMPDIRSPEC sTempDirName;
    VTBOOL           bUseNativeFilter;
    SCCUTTEMPFILSPEC sTmpNameCallback;
    VTDWORD          dwFilterFlags;
    VTDWORD          dwMiscFlags;
    VTBOOL           bRenderingPreferOIT;
    VTLONG           lTimeZone;
    VTDWORD          dwDefaultInputCharset;
    SCCBUFFEROPTIONS sBufferOptions;
    VTBOOL           bExtractXMPMetaData;
    REDIRECTTEMPFILECALLBACKPROC  pRedirtTempFileCallbackFunc;
} UTSYSTEMOPTIONS82, * PUTSYSTEMOPTIONS82;

typedef struct UTSYSTEMOPTIONS830tag
{
    VTDWORD          dwDialogFlags;
    VTDWORD          dwSystemFlags;
    VTDWORD          dwIntlFlags;
    VTDWORD          dwScrollFlags;
    VTDWORD          dwHardOptions;                
    VTDWORD          dwOleFlags;
    SCCUTRESOURCEID  sResourceLibraryID;
    VTWORD           wUnmappableChar;
    VTDWORD          dwFIFlags;
    SCCUTTEMPDIRSPEC sTempDirName;
    VTBOOL           bUseNativeFilter;
    SCCUTTEMPFILSPEC sTmpNameCallback;
    VTDWORD          dwFilterFlags;
    VTDWORD          dwMiscFlags;
    VTBOOL           bRenderingPreferOIT;
    VTLONG           lTimeZone;
    VTDWORD          dwDefaultInputCharset;
    SCCBUFFEROPTIONS sBufferOptions;
    VTBOOL           bExtractXMPMetaData;
    REDIRECTTEMPFILECALLBACKPROC  pRedirtTempFileCallbackFunc;
	VTDWORD			 dwDocMemoryMode;
} UTSYSTEMOPTIONS830, * PUTSYSTEMOPTIONS830;

typedef struct UTSYSTEMOPTIONS837tag
{
    VTDWORD          dwDialogFlags;
    VTDWORD          dwSystemFlags;
    VTDWORD          dwIntlFlags;
    VTDWORD          dwScrollFlags;
    VTDWORD          dwHardOptions;                
    VTDWORD          dwOleFlags;
    SCCUTRESOURCEID  sResourceLibraryID;
    VTWORD           wUnmappableChar;
    VTDWORD          dwFIFlags;
    SCCUTTEMPDIRSPEC sTempDirName;
    VTBOOL           bUseNativeFilter;
    SCCUTTEMPFILSPEC sTmpNameCallback;
    VTDWORD          dwFilterFlags;
    VTDWORD          dwMiscFlags;
    VTBOOL           bRenderingPreferOIT;
    VTLONG           lTimeZone;
    VTDWORD          dwDefaultInputCharset;
    SCCBUFFEROPTIONS sBufferOptions;
    VTBOOL           bExtractXMPMetaData;
    VTBOOL           bParseXMPMetaData;
    REDIRECTTEMPFILECALLBACKPROC  pRedirtTempFileCallbackFunc;
	VTDWORD			 dwDocMemoryMode;
	VTDWORD			 dwFlags;
} UTSYSTEMOPTIONS837, * PUTSYSTEMOPTIONS837;

typedef struct UTSYSTEMOPTIONStag
{
    VTDWORD          dwDialogFlags;
    VTDWORD          dwSystemFlags;
    VTDWORD          dwIntlFlags;
    VTDWORD          dwScrollFlags;
    VTDWORD          dwHardOptions;                
    VTDWORD          dwOleFlags;
    SCCUTRESOURCEID  sResourceLibraryID;
    VTWORD           wUnmappableChar;
    VTDWORD          dwFIFlags;
    SCCUTTEMPDIRSPEC sTempDirName;
    VTBOOL           bUseNativeFilter;
    SCCUTTEMPFILSPEC sTmpNameCallback;
    VTDWORD          dwFilterFlags;
    VTDWORD          dwMiscFlags;
    VTBOOL           bRenderingPreferOIT;
    VTLONG           lTimeZone;
    VTDWORD          dwDefaultInputCharset;
    SCCBUFFEROPTIONS sBufferOptions;
    VTBOOL           bExtractXMPMetaData;
    VTBOOL           bParseXMPMetaData;
    REDIRECTTEMPFILECALLBACKPROC  pRedirtTempFileCallbackFunc;
	VTDWORD			 dwDocMemoryMode;
	VTDWORD			 dwFlags;
    VTDWORD          dwHtmlCondCommentMode;  /*conditional comment setting for HTML input filter.*/
	VTBOOL			 bRenderEmedFonts; /*If true we will render pdf documents using the file's embedded fonts if any are present.*/ 
	VTBOOL			 bRenderDisableAlphaBlending;  /* disable alpha blending of graphics */
} UTSYSTEMOPTIONS, * PUTSYSTEMOPTIONS;

/*-------------------------------------------------------
|   HTML Export Options Structure
|   Graphics related options reside in the GDSF structure.
|   Other options may reside in other structures as well.
|--------------------------------------------------------
*/
typedef struct UTHXOPTIONS600tag
{
    VTDWORD             dwOutputCharacterSet;
    VTDWORD             dwFlavor;
    VTDWORD             dwFlags;
    VTDWORD             dwCharByteOrder;
    VTBOOL              bNoSourceFormatting;
    VTBOOL              bUnicodeCallbackStr;
    VTDWORD             dwTemplateSize;
    VTHANDLE            hTemplate;
    VTLPBYTE            pTemplate;
    VTWORD              wzFontName[SCCUT_MAXFALLBACKFONTLEN];
    SCCUTFALLBACKFONT   sFallbackFont;
} UTHXOPTIONS600, * PUTHXOPTIONS600;

typedef struct UTHXOPTIONS601tag
{
    VTDWORD             dwOutputCharacterSet;
    VTDWORD             dwFlavor;
    VTDWORD             dwFlags;
    VTDWORD             dwCharByteOrder;
    VTBOOL              bNoSourceFormatting;
    VTBOOL              bUnicodeCallbackStr;
    VTDWORD             dwTemplateSize;
    VTHANDLE            hTemplate;
    VTLPBYTE            pTemplate;
    VTWORD              wzFontName[SCCUT_MAXFALLBACKFONTLEN];
    SCCUTFALLBACKFONT   sFallbackFont;
    VTBOOL              bSimpleStyleNames;
} UTHXOPTIONS601, * PUTHXOPTIONS601;

typedef struct UTHXOPTIONS610tag
{
    VTDWORD             dwOutputCharacterSet;
    VTDWORD             dwFlavor;
    VTDWORD             dwFlags;
    VTDWORD             dwCharByteOrder;
    VTBOOL              bNoSourceFormatting;
    VTBOOL              bUnicodeCallbackStr;
    VTDWORD             dwTemplateSize;
    VTHANDLE            hTemplate;
    VTLPBYTE            pTemplate;
    VTWORD              wzFontName[SCCUT_MAXFALLBACKFONTLEN];
    SCCUTFALLBACKFONT   sFallbackFont;
    VTBOOL              bSimpleStyleNames;
    VTBOOL              bGenBulletsAndNums;
} UTHXOPTIONS610, * PUTHXOPTIONS610;

typedef struct UTHXOPTIONS611tag
{
    VTDWORD             dwOutputCharacterSet;
    VTDWORD             dwFlavor;
    VTDWORD             dwFlags;
    VTDWORD             dwCharByteOrder;
    VTBOOL              abUnused[2];
    VTDWORD             dwTemplateSize;
    VTHANDLE            hTemplate;
    VTLPBYTE            pTemplate;
    VTWORD              wzUnused[SCCUT_MAXFALLBACKFONTLEN];
    SCCUTFALLBACKFONT   sUnused;
    VTBOOL              bSimpleStyleNames;
    VTBOOL              bGenBulletsAndNums;
    VTDWORD             dwInlineGraphicWidth;
} UTHXOPTIONS611, * PUTHXOPTIONS611;   /*  611 = 6.1.0 + MR 1 */



/*-------------------------------------------------------
|   Wireless Export Options Structure
|   Graphics related options reside in the GDSF structure.
|   Other options may reside in other structures as well.
|--------------------------------------------------------
*/


typedef struct UTWXOPTIONS610tag
{
    VTDWORD             dwOutputCharacterSet;
    VTDWORD             dwFlavor;
    VTDWORD             dwTemplateSize;
    VTHANDLE            hTemplate;
    VTLPBYTE            pTemplate;
    VTDWORD             dwDeckSize;
    VTDWORD             dwLabelFlags;
} UTWXOPTIONS610, * PUTWXOPTIONS610;

typedef struct UTWXOPTIONS611tag
{
    VTDWORD             dwOutputCharacterSet;
    VTDWORD             dwFlavor;
    VTDWORD             dwTemplateSize;
    VTHANDLE            hTemplate;
    VTLPBYTE            pTemplate;
    VTDWORD             dwDeckSize;
    VTDWORD             dwLabelFlags;
    VTDWORD             dwGraphicSkipSize;
} UTWXOPTIONS611, * PUTWXOPTIONS611;


/*-------------------------------------------------------
|   General Export Options Structure
|   Contains options common to HTML and Wireless Export.
|--------------------------------------------------------
*/
typedef struct UTEXOPTIONS610tag
{
    VTBOOL              bNoSourceFormatting;
    VTBOOL              bUnicodeCallbackStr;
    VTDWORD             dwCharByteOrder;
    SCCUTFALLBACKFONT   sFallbackFont;
    VTWORD              wzFontName[SCCUT_MAXFALLBACKFONTLEN];
} UTEXOPTIONS610, *PUTEXOPTIONS610;

typedef struct UTEXOPTIONS700tag
{
    VTBOOL              bNoSourceFormatting;
    VTBOOL              bUnicodeCallbackStr;
    VTDWORD             dwCharByteOrder;
    SCCUTFALLBACKFONT   sFallbackFont;
    VTWORD              wzFontName[SCCUT_MAXFALLBACKFONTLEN];
    VTDWORD             dwTextBufferSize;
    VTDWORD             dwGraphicBufferSize;
    VTDWORD             dwLabelFlags;
    VTDWORD             dwGraphicSkipSize;
    VTBOOL              bCollapseWhitespace;
    VTDWORD             dwMaxURLLength;
    VTBOOL              bSeparateGraphicsBuffer;
    VTDWORD             dwTemplateSize;
    VTHANDLE            hTemplate;
    VTLPBYTE            pTemplate;
    VTDWORD             dwFlavor;
    VTDWORD             dwOutputCharacterSet;
    VTBOOL              bGenBulletsAndNums;
    VTDWORD             dwPageSize;
    VTDWORD             dwComplianceFlags;
    VTBOOL              bSimpleStyleNames;
    VTBOOL              bJSTabs;
} UTEXOPTIONS700, *PUTEXOPTIONS700;

typedef struct UTEXOPTIONS710tag
{
    VTBOOL              bNoSourceFormatting;
    VTBOOL              bUnicodeCallbackStr;
    VTDWORD             dwCharByteOrder;
    SCCUTFALLBACKFONT   sFallbackFont;
    VTWORD              wzFontName[SCCUT_MAXFALLBACKFONTLEN];
    VTDWORD             dwTextBufferSize;
    VTDWORD             dwGraphicBufferSize;
    VTDWORD             dwLabelFlags;
    VTDWORD             dwGraphicSkipSize;
    VTBOOL              bCollapseWhitespace;
    VTDWORD             dwMaxURLLength;
    VTBOOL              bSeparateGraphicsBuffer;
    VTDWORD             dwTemplateSize;
    VTHANDLE            hTemplate;
    VTLPBYTE            pTemplate;
    VTDWORD             dwFlavor;
    VTDWORD             dwOutputCharacterSet;
    VTBOOL              bGenBulletsAndNums;
    VTDWORD             dwPageSize;
    VTDWORD             dwComplianceFlags;
    VTBOOL              bSimpleStyleNames;
    VTBOOL              bJSTabs;
    VTDWORD             dwCallbackFlags;
} UTEXOPTIONS710, *PUTEXOPTIONS710;

typedef struct UTEXOPTIONS750tag
{
    VTBOOL              bNoSourceFormatting;
    VTBOOL              bUnicodeCallbackStr;
    VTDWORD             dwCharByteOrder;
    SCCUTFALLBACKFONT   sFallbackFont;
    VTWORD              wzFontName[SCCUT_MAXFALLBACKFONTLEN];
    VTDWORD             dwTextBufferSize;
    VTDWORD             dwGraphicBufferSize;
    VTDWORD             dwLabelFlags;
    VTDWORD             dwGraphicSkipSize;
    VTBOOL              bCollapseWhitespace;
    VTDWORD             dwMaxURLLength;
    VTBOOL              bSeparateGraphicsBuffer;
    VTDWORD             dwTemplateSize;
    VTHANDLE            hTemplate;
    VTLPBYTE            pTemplate;
    VTDWORD             dwFlavor;
    VTDWORD             dwOutputCharacterSet;
    VTBOOL              bGenBulletsAndNums;
    VTDWORD             dwPageSize;
    VTDWORD             dwComplianceFlags;
    VTBOOL              bSimpleStyleNames;
    VTBOOL              bJSTabs;
    VTDWORD             dwCallbackFlags;
    VTDWORD             dwGridRows;
    VTDWORD             dwGridCols;
    VTDWORD             dwGridAdvance;
    VTBOOL              bGridWrap;
    VTBOOL              bPreventGraphicOverlap;
    VTDWORD             dwFontFlags;
    VTBOOL              bShowHiddenText;
} UTEXOPTIONS750, *PUTEXOPTIONS750;

typedef struct UTEXOPTIONStag800
{
    VTBOOL              bNoSourceFormatting;
    VTBOOL              bUnicodeCallbackStr;
    VTDWORD             dwCharByteOrder;
    SCCUTFALLBACKFONT   sFallbackFont;
    VTWORD              wzFontName[SCCUT_MAXFALLBACKFONTLEN];
    VTDWORD             dwTextBufferSize;
    VTDWORD             dwGraphicBufferSize;
    VTDWORD             dwLabelFlags;
    VTDWORD             dwGraphicSkipSize;
    VTBOOL              bCollapseWhitespace;
    VTDWORD             dwMaxURLLength;
    VTBOOL              bSeparateGraphicsBuffer;
    VTDWORD             dwTemplateSize;
    VTHANDLE            hTemplate;
    VTLPBYTE            pTemplate;
    VTDWORD             dwFlavor;
    VTDWORD             dwOutputCharacterSet;
    VTBOOL              bGenBulletsAndNums;
    VTDWORD             dwPageSize;
    VTDWORD             dwComplianceFlags;
    VTBOOL              bSimpleStyleNames;
    VTBOOL              bJSTabs;
    VTDWORD             dwCallbackFlags;
    VTDWORD             dwGridRows;
    VTDWORD             dwGridCols;
    VTDWORD             dwGridAdvance;
    VTBOOL              bGridWrap;
    VTBOOL              bPreventGraphicOverlap;
    VTDWORD             dwFontFlags;
    VTBOOL              bShowHiddenText;
    VTBOOL              bShowHiddenSSData;
    VTBOOL              bChangeTracking;
    VTBOOL              bShowSpreadSheetBorder;
    VTDWORD             dwPageCount;
    VTBOOL              bMultiPage;
    VTBOOL              bImageExport;
    VTDWORD             dwExtractEmbeddedFormat;
    VTBOOL              bShowSSDBRowColHeadings;
    VTDWORD             dwSSDBBorderOptions;
} UTEXOPTIONS800, *PUTEXOPTIONS800;

/* new version for OracleText PHTML support */
typedef struct UTEXOPTIONStag
{
    VTBOOL              bNoSourceFormatting;
    VTBOOL              bUnicodeCallbackStr;
    VTDWORD             dwCharByteOrder;
    SCCUTFALLBACKFONT   sFallbackFont;
    VTWORD              wzFontName[SCCUT_MAXFALLBACKFONTLEN];
    VTDWORD             dwTextBufferSize;
    VTDWORD             dwGraphicBufferSize;
    VTDWORD             dwLabelFlags;
    VTDWORD             dwGraphicSkipSize;
    VTBOOL              bCollapseWhitespace;
    VTDWORD             dwMaxURLLength;
    VTBOOL              bSeparateGraphicsBuffer;
    VTDWORD             dwTemplateSize;
    VTHANDLE            hTemplate;
    VTLPBYTE            pTemplate;
    VTDWORD             dwFlavor;
    VTDWORD             dwOutputCharacterSet;
    VTBOOL              bGenBulletsAndNums;
    VTDWORD             dwPageSize;
    VTDWORD             dwComplianceFlags;
    VTBOOL              bSimpleStyleNames;
    VTBOOL              bJSTabs;
    VTDWORD             dwCallbackFlags;
    VTDWORD             dwGridRows;
    VTDWORD             dwGridCols;
    VTDWORD             dwGridAdvance;
    VTBOOL              bGridWrap;
    VTBOOL              bPreventGraphicOverlap;
    VTDWORD             dwFontFlags;
    VTBOOL              bShowHiddenText;
    VTBOOL              bShowHiddenSSData;
    VTBOOL              bChangeTracking;
    VTBOOL              bShowSpreadSheetBorder;
    VTDWORD             dwPageCount;
    VTBOOL              bMultiPage;
    VTBOOL              bImageExport;
    VTDWORD             dwExtractEmbeddedFormat;
    VTBOOL              bShowSSDBRowColHeadings;
    VTDWORD             dwSSDBBorderOptions;
    VTBOOL              bSuppressMetadata;          /* PHTML */
    VTDWORD             dwPerformanceMode;
} UTEXOPTIONS, *PUTEXOPTIONS;


typedef struct IMGWATERMARKPATHtag
{
    VTCHAR  szPath[SCCUT_FILENAMEMAX];
    VTDWORD dwMaxSize;
} IMGWATERMARKPATH,  *LPIMGWATERMARKPATH; 

/*
|   this structure doesn't need to be copied because
|   the options are never saved.
*/

typedef struct UTGDSFOPTIONS832tag
{
    VTDWORD             dwGraphicType;
    VTDWORD             dwGraphicWidth;
    VTDWORD             dwGraphicHeight;
    VTDWORD             dwGraphicFlags;
    VTLONG              lGraphicCropLeft;
    VTLONG              lGraphicCropRight;
    VTLONG              lGraphicCropTop;
    VTLONG              lGraphicCropBottom;
    VTDWORD             dwGraphicXDPI;
    VTDWORD             dwGraphicYDPI;
    VTDWORD             dwGraphicSizeMethod;
    VTDWORD             dwGraphicOutputDPI;
    VTDWORD             dwJPEGQuality;
    VTBOOL              bDefaultGraphicOnError;
    VTBOOL              bGifInterlace;
    SCCUTTIFFOPTIONS    sTIFFOptions;
    VTDWORD             dwGraphicSizeLimit;
    VTDWORD             dwGraphicWidthLimit;
    VTDWORD             dwGraphicHeightLimit;
    VTDWORD             dwGraphicPageNumber;
    VTBOOL              bGraphicLastPage;
    VTBOOL              bMultiPage;
    VTBOOL              bImageExport;
    EXTIFFOPTIONS       ExTiffOptions;
    VTBOOL              bAcceptAltGraphics;
    VTDWORD             rgbTransparent;
    VTDWORD             dwImageCropping;
    VTDWORD             dwMaxSSDBPageWidth;
    VTDWORD             dwMaxSSDBPageHeight;
    VTDWORD             dwWatermarkOpacity;
    VTDWORD             dwWatermarkPosition;
    VTDWORD             dwWatermarkScaleType;
    VTDWORD             dwWatermarkScalePercent;
    VTLONG              lWatermarkHorizontalPos;
    VTLONG              lWatermarkVerticalPos;
    IMGWATERMARKPATH    sWatermarkPath;
} UTGDSFOPTIONS832, * PUTGDSFOPTIONS832;

typedef struct UTGDSFOPTIONS852tag
{
    VTDWORD             dwGraphicType;
    VTDWORD             dwGraphicWidth;
    VTDWORD             dwGraphicHeight;
    VTDWORD             dwGraphicFlags;
    VTLONG              lGraphicCropLeft;
    VTLONG              lGraphicCropRight;
    VTLONG              lGraphicCropTop;
    VTLONG              lGraphicCropBottom;
    VTDWORD             dwGraphicXDPI;
    VTDWORD             dwGraphicYDPI;
    VTDWORD             dwGraphicSizeMethod;
    VTDWORD             dwGraphicOutputDPI;
    VTDWORD             dwJPEGQuality;
    VTBOOL              bDefaultGraphicOnError;
    VTBOOL              bGifInterlace;
    SCCUTTIFFOPTIONS    sTIFFOptions;
    VTDWORD             dwGraphicSizeLimit;
    VTDWORD             dwGraphicWidthLimit;
    VTDWORD             dwGraphicHeightLimit;
    VTDWORD             dwGraphicPageNumber;
    VTBOOL              bGraphicLastPage;
    VTBOOL              bMultiPage;
    VTBOOL              bImageExport;
    EXTIFFOPTIONS       ExTiffOptions;
    VTBOOL              bAcceptAltGraphics;
    VTDWORD             rgbTransparent;
    VTDWORD             dwImageCropping;
    VTDWORD             dwMaxSSDBPageWidth;
    VTDWORD             dwMaxSSDBPageHeight;
    VTDWORD             dwWatermarkOpacity;
    VTDWORD             dwWatermarkPosition;
    VTDWORD             dwWatermarkScaleType;
    VTDWORD             dwWatermarkScalePercent;
    VTLONG              lWatermarkHorizontalPos;
    VTLONG              lWatermarkVerticalPos;
    IMGWATERMARKPATH    sWatermarkPath;
	VTBOOL              bQuickThumbnail;
	VTBOOL              bRenderAsPage;
} UTGDSFOPTIONS852, * PUTGDSFOPTIONS852;

typedef struct UTGDSFOPTIONStag
{
	VTDWORD             dwGraphicType;
	VTDWORD             dwGraphicWidth;
	VTDWORD             dwGraphicHeight;
	VTDWORD             dwGraphicFlags;
	VTLONG              lGraphicCropLeft;
	VTLONG              lGraphicCropRight;
	VTLONG              lGraphicCropTop;
	VTLONG              lGraphicCropBottom;
	VTDWORD             dwGraphicXDPI;
	VTDWORD             dwGraphicYDPI;
	VTDWORD             dwGraphicSizeMethod;
	VTDWORD             dwGraphicOutputDPI;
	VTDWORD             dwJPEGQuality;
	VTBOOL              bDefaultGraphicOnError;
	VTBOOL              bGifInterlace;
	SCCUTTIFFOPTIONS    sTIFFOptions;
	VTDWORD             dwGraphicSizeLimit;
	VTDWORD             dwGraphicWidthLimit;
	VTDWORD             dwGraphicHeightLimit;
	VTDWORD             dwGraphicPageNumber;
	VTBOOL              bGraphicLastPage;
	VTBOOL              bMultiPage;
	VTBOOL              bImageExport;
	EXTIFFOPTIONS       ExTiffOptions;
	VTBOOL              bAcceptAltGraphics;
	VTDWORD             rgbTransparent;
	VTDWORD             dwImageCropping;
	VTDWORD             dwMaxSSDBPageWidth;
	VTDWORD             dwMaxSSDBPageHeight;
	VTDWORD             dwWatermarkOpacity;
	VTDWORD             dwWatermarkPosition;
	VTDWORD             dwWatermarkScaleType;
	VTDWORD             dwWatermarkScalePercent;
	VTLONG              lWatermarkHorizontalPos;
	VTLONG              lWatermarkVerticalPos;
	IMGWATERMARKPATH    sWatermarkPath;
	VTBOOL              bQuickThumbnail;
	VTBOOL              bRenderAsPage;
	VTDWORD             dwWatermarkPathType;
    VTLONG              lWatermarkHorizontalOffset;
    VTLONG              lWatermarkVerticalOffset;
    VTDWORD             dwWatermarkHorizontalOffsetFrom;
    VTDWORD             dwWatermarkVerticalOffsetFrom;
	 VTBOOL              bTransparencyThreshold;
} UTGDSFOPTIONS, *PUTGDSFOPTIONS;


/*-------------------------------------------------------
|   XML Export Options Structure
|--------------------------------------------------------
*/

typedef struct UTXXOPTIONStag750
{
    VTDWORD             dwDefMethod;
    VTDWORD             dwSearchMLFlags;
    SCCUTXXDEFREFINFO   utf8DefReferenceStr;
    VTDWORD             dwPageMLFlags;
    VTBYTE              szPrinterName[SCCUT_FILENAMEMAX];
} UTXXOPTIONS750, * PUTXXOPTIONS750;

typedef struct UTXXOPTIONStag80
{
    VTDWORD             dwDefMethod;
    VTDWORD             dwSearchMLFlags;
    SCCUTXXDEFREFINFO   utf8DefReferenceStr;
    VTDWORD             dwPageMLFlags;
    VTBYTE              szPrinterName[SCCUT_FILENAMEMAX];
    VTDWORD             dwTrackedCharAttrs;
    VTDWORD             dwTrackedParaAttrs;
    VTBOOL              bProduceOffsets;
}  UTXXOPTIONS80, * PUTXXOPTIONS80;

typedef struct UTXXOPTIONStag819
{
    VTDWORD             dwDefMethod;
    VTDWORD             dwSearchMLFlags;
    SCCUTXXDEFREFINFO   utf8DefReferenceStr;
    VTDWORD             dwPageMLFlags;
    VTBYTE              szPrinterName[SCCUT_FILENAMEMAX];
    VTDWORD             dwTrackedCharAttrs;
    VTDWORD             dwTrackedParaAttrs;
    VTBOOL              bProduceOffsets;
    VTWORD              wNullChar;
    VTBOOL              bXMLOutput;
}  UTXXOPTIONS819, * PUTXXOPTIONS819;

typedef struct UTXXOPTIONStag
{
    VTDWORD             dwDefMethod;
    VTDWORD             dwSearchMLFlags;
    SCCUTXXDEFREFINFO   utf8DefReferenceStr;
    VTDWORD             dwPageMLFlags;
    VTBYTE              szPrinterName[SCCUT_FILENAMEMAX];
    VTDWORD             dwTrackedCharAttrs;
    VTDWORD             dwTrackedParaAttrs;
    VTBOOL              bProduceOffsets;
    VTWORD              wNullChar;
    VTBOOL              bXMLOutput;
    VTDWORD             dwUnmappedText;
}  UTXXOPTIONS, * PUTXXOPTIONS;

/*-------------------------------------------------------
|   EXXML (an XCHUNKER consumer) options structure
|--------------------------------------------------------
*/

typedef struct UTEXXMLOPTIONStag
{
    /* null-terminated wide string */
    VTHANDLE            hSubstreamRootsString;
    /* Size of the root elements string in bytes.  */
    VTDWORD             dwSubstreamRootsStringSize;
    VTDWORD             dwDefMethod;
    SCCUTXXDEFREFINFO   utf8DefReferenceStr;
} UTEXXMLOPTIONS, * PUTEXXMLOPTIONS;

/*-------------------------------------------------------
|   CCFLEX (Chunker to Flexiondoc component) options structure
|--------------------------------------------------------
*/

typedef struct UTCCFLEXOPTIONStag
{
    VTBOOL   bFullyAttributeStyles;     /* SCCOPT_CCFLEX_FULLYATTRIBUTESTYLES */
    VTBOOL   bRemoveCurrentPoint;       /* SCCOPT_CCFLEX_REMOVECURRENTPOINT */
    VTBOOL   bRemoveFontGroups;         /* SCCOPT_CCFLEX_REMOVEFONTGROUPS */
    VTBOOL   bIncludeTextOffsets;       /* SCCOPT_CCFLEX_INCLUDETEXTOFFSETS */
    SCCUTFORMATLIST sExtractEmbeddings; /* SCCOPT_CCFLEX_EXTRACTEMBEDDINGS */
    SCCUTFORMATLIST sConvertEmbeddings; /* SCCOPT_CCFLEX_CONVERTEMBEDDINGS */
    VTDWORD  dwFormatOptions;           /* SCCOPT_CCFLEX_FORMATOPTIONS */
    VTDWORD  dwUnits;                   /* SCCOPT_CCFLEX_UNITS */
} UTCCFLEXOPTIONS, * PUTCCFLEXOPTIONS;

typedef struct UTCCFLEXOPTIONS770tag
{
    VTBOOL   bFullyAttributeStyles;     /* SCCOPT_CCFLEX_FULLYATTRIBUTESTYLES */
    VTBOOL   bRemoveCurrentPoint;       /* SCCOPT_CCFLEX_REMOVECURRENTPOINT */
    VTBOOL   bRemoveFontGroups;         /* SCCOPT_CCFLEX_REMOVEFONTGROUPS */
    VTBOOL   bIncludeTextOffsets;       /* SCCOPT_CCFLEX_INCLUDETEXTOFFSETS */
    SCCUTFORMATLIST sExtractEmbeddings; /* SCCOPT_CCFLEX_EXTRACTEMBEDDINGS */
    SCCUTFORMATLIST sConvertEmbeddings; /* SCCOPT_CCFLEX_CONVERTEMBEDDINGS */
    VTDWORD  dwFormatOptions;           /* SCCOPT_CCFLEX_FORMATOPTIONS */
} UTCCFLEXOPTIONS770, * PUTCCFLEXOPTIONS770;

/*-------------------------------------------------------
|   CCOD (Flexiondoc to Open Document component) options structure
|--------------------------------------------------------
*/

typedef struct UTCCODOPTIONStag
{
    VTDWORD  dwGraphicOptions;           /* SCCOPT_CCOD_GRAPHICOPTIONS */
} UTCCODOPTIONS, * PUTCCODOPTIONS;

/*---------------------------------------------------------------
| IX options structure
|----------------------------------------------------------------
*/

typedef struct UTIXOPTIONStag
{
    UTWATERMARKOPTIONS  sWaterMark;
    DEFAULTPAGESIZE     sDefaultPageSize;
    VTCHAR              szFontPath[SCCUT_FILENAMEMAX];
} UTIXOPTIONS, * PUTIXOPTIONS;

/*---------------------------------------------------------------
| PX options structure
|----------------------------------------------------------------
*/

typedef struct UTPXOPTIONStag
{
    VTBOOL              bFilter;
    VTBOOL              bEmbedFonts;
    VTBOOL              bDoLinearization;
    VTBOOL              bExportAttachments;
    VTBOOL              bSubstNonembeddableFont;
    VTBOOL              bUnrestrictedFontEmbedding;
    VTBOOL              bDirectImagePassThrough;
    VTDWORD             dwCompression24BitType;
    VTDWORD             dwFontEmbedPolicy;
} UTPXOPTIONS, * PUTPXOPTIONS;

/*---------------------------------------------------------------
| H5 export options structure
|----------------------------------------------------------------
*/
typedef struct EX_MARGINTEXTLINE
{
    VTDWORD dwLineNum;          // 1, 2, 3
    VTDWORD dwLocation;         // SCC_MARGIN_LOCATION_* (sccopval.h)    
    VTWCHAR wsText[SCCOPT_MAX_MARGINTEXTLINE];
} EXMARGINTEXTLINE, *PEXMARGINTEXTLINE;


typedef struct UTH5XOPTIONStag
{
    VTDWORD                   dwOutputStructure;
    VTBOOL                    bRawText;
    VTDWORD                   dwFontPermissions;
    VTDWORD                   dwReferenceMethod;
    VTDWORD                   dwAttachmentHandling;
    VTBYTE                   *pszResourceUrl;
    VTBYTE                   *pszOutputUrl;
    VTBYTE                   *pszBaseUrl;
    VTBYTE                   *pszStylesheetName;
    VTBYTE                   *pszLibraryName;
    PSCCUTOPTIONITEMLIST      pExternalCssList;
    PSCCUTOPTIONITEMLIST      pPostScriptList;
    PSCCUTOPTIONITEMLIST      pPreScriptList;
    PSCCUTOPTIONITEMLIST      pImageFileList;
    PSCCUTOPTIONITEMLIST      pImageUrlList;

    VTBOOL                    bShowRedactionLabels;
    VTWORD                   *pwszRedactionLabelFontName;
    VTDWORD                   dwRedactionLabelFontSize;
    VTBOOL                    bRedactionsEnabled;
    VTDWORD                   dwRedactionColor;

    VTWORD                   *pwszMarginTextFontName;
    VTDWORD                   dwMarginFontSize;
    EXMARGINTEXTLINE          MarginTextLines[MAX_MARGIN_TEXT_LINES];
	 VTDWORD                   dwVectorObjectLimit;

} UTH5XOPTIONS, *PUTH5XOPTIONS;




typedef struct EXANNOSTAMPIMAGEtag
{
    VTLPBYTE    szStampName;             // UTF-8 string
    VTLPVOID    pStampImage;            //IO Spec of the image
    VTDWORD     dwSpecType;
} EXANNOSTAMPIMAGE, *PEXANNOSTAMPIMAGE;

/*-------------------------------------------------------
|   Other Miscellaneous Options Structure
|--------------------------------------------------------
*/
typedef struct UTOCROPTIONStag
{
    VTDWORD dwOcrQuality;
    VTDWORD dwOcrTech;
}UTOCROPTIONS;

typedef struct UTDRAMOPTIONStag
{
    VTBOOL      bUseDefaults;
} UTDRAMOPTIONS, * PUTDRAMOPTIONS;

typedef struct UTPSOPTIONStag
{
    VTDWORD     dwEnablePSCallback;
} UTPSOPTIONS, * PUTPSOPTIONS;

typedef struct UTRNBWOPTIONStag
{
    VTDWORD     dwDataDirectory;
    VTDWORD     dwConversionSel;
} UTRNBWOPTIONS, * PUTRNBWOPTIONS;

typedef struct UTOPTIONS521tag
{
    VTDWORD             dwSize;
    UTOPTIONDELTA521    sDelta;
    UTPRINTOPTIONS      sPrint;
    UTDISPLAYOPTIONS521 sDisplay;
    UTWPOPTIONS         sWp;
    UTSSOPTIONS75       sSs;
    UTBMPOPTIONS        sBmp;
    UTVECOPTIONS        sVec;
    UTDBOPTIONS         sDb;
    UTARCOPTIONS        sArc;
    UTCLIPOPTIONS       sClip;
    UTSYSTEMOPTIONS521  sSystem;
    SCCUTNUMBERFORMAT   sNumberFormat;
    SCCUTDAYNAMES       sDayNames;
    SCCUTMONTHNAMES     sMonthNames;
} UTOPTIONS521, * PUTOPTIONS521;

typedef struct UTOPTIONS600tag
{
    VTDWORD           dwSize;
    UTOPTIONDELTA830  sDelta;
    UTPRINTOPTIONS    sPrint;
    UTDISPLAYOPTIONS  sDisplay;
    UTWPOPTIONS       sWp;
    UTSSOPTIONS75     sSs;
    UTBMPOPTIONS      sBmp;
    UTVECOPTIONS      sVec;
    UTDBOPTIONS       sDb;
    UTARCOPTIONS600   sArc;
    UTCLIPOPTIONS     sClip;
    UTSYSTEMOPTIONS   sSystem;
    UTHXOPTIONS600    sHX;
    UTGDSFOPTIONS832  sGDSF;
    UTDRAMOPTIONS     sDramamine;
    UTPSOPTIONS       sPS;
    UTRNBWOPTIONS     sRainbow;
    UTOPTIONDELTA830  sOptionTrack;
    SCCUTMONTHNAMES   sMonthNames;
    SCCUTNUMBERFORMAT sNumberFormat;
    SCCUTDAYNAMES     sDayNames;
} UTOPTIONS600, * PUTOPTIONS600;

typedef struct UTOPTIONS601tag
{
    VTDWORD           dwSize;
    UTOPTIONDELTA830  sDelta;
    UTPRINTOPTIONS    sPrint;
    UTDISPLAYOPTIONS  sDisplay;
    UTWPOPTIONS       sWp;
    UTSSOPTIONS75     sSs;
    UTBMPOPTIONS      sBmp;
    UTVECOPTIONS      sVec;
    UTDBOPTIONS       sDb;
    UTARCOPTIONS      sArc;
    UTCLIPOPTIONS     sClip;
    UTSYSTEMOPTIONS   sSystem;
    UTHXOPTIONS601    sHX;
    UTGDSFOPTIONS832  sGDSF;
    UTDRAMOPTIONS     sDramamine;
    UTPSOPTIONS       sPS;
    UTRNBWOPTIONS     sRainbow;
    UTOPTIONDELTA830  sOptionTrack;
    SCCUTMONTHNAMES   sMonthNames;
    SCCUTNUMBERFORMAT sNumberFormat;
    SCCUTDAYNAMES     sDayNames;
} UTOPTIONS601, * PUTOPTIONS601;

typedef struct UTOPTIONS610tag
{
    VTDWORD            dwSize;
    UTOPTIONDELTA830   sDelta;
    UTPRINTOPTIONS     sPrint;
    UTDISPLAYOPTIONS   sDisplay;
    UTWPOPTIONS        sWp;
    UTSSOPTIONS75      sSs;
    UTBMPOPTIONS       sBmp;
    UTVECOPTIONS       sVec;
    UTDBOPTIONS        sDb;
    UTARCOPTIONS       sArc;
    UTCLIPOPTIONS      sClip;
    UTSYSTEMOPTIONS610 sSystem;
    UTHXOPTIONS610     sHX;
    UTWXOPTIONS610     sWX;
    UTEXOPTIONS610     sEX;
    UTGDSFOPTIONS832   sGDSF;
    UTDRAMOPTIONS      sDramamine;
    UTPSOPTIONS        sPS;
    UTRNBWOPTIONS      sRainbow;
    UTOPTIONDELTA830   sOptionTrack;
    SCCUTMONTHNAMES    sMonthNames;
    SCCUTNUMBERFORMAT  sNumberFormat;
    SCCUTDAYNAMES      sDayNames;
} UTOPTIONS610, * PUTOPTIONS610;

typedef struct UTOPTIONS611tag
{
    VTDWORD            dwSize;
    UTOPTIONDELTA830   sDelta;
    UTPRINTOPTIONS     sPrint;
    UTDISPLAYOPTIONS   sDisplay;
    UTWPOPTIONS        sWp;
    UTSSOPTIONS75      sSs;
    UTBMPOPTIONS       sBmp;
    UTVECOPTIONS       sVec;
    UTDBOPTIONS        sDb;
    UTARCOPTIONS       sArc;
    UTCLIPOPTIONS      sClip;
    UTSYSTEMOPTIONS610 sSystem;
    UTHXOPTIONS611     sHX;
    UTWXOPTIONS611     sWX;
    UTEXOPTIONS610     sEX;
    UTGDSFOPTIONS832   sGDSF;
    UTDRAMOPTIONS      sDramamine;
    UTPSOPTIONS        sPS;
    UTRNBWOPTIONS      sRainbow;
    UTOPTIONDELTA830   sOptionTrack;
    SCCUTMONTHNAMES    sMonthNames;
    SCCUTNUMBERFORMAT  sNumberFormat;
    SCCUTDAYNAMES      sDayNames;
} UTOPTIONS611, * PUTOPTIONS611;

typedef struct UTOPTIONS700tag
{
    VTDWORD           dwSize;
    UTOPTIONDELTA830  sDelta;
    UTPRINTOPTIONS    sPrint;
    UTDISPLAYOPTIONS  sDisplay;
    UTWPOPTIONS       sWp;
    UTSSOPTIONS75     sSs;
    UTBMPOPTIONS      sBmp;
    UTVECOPTIONS      sVec;
    UTDBOPTIONS       sDb;
    UTARCOPTIONS      sArc;
    UTCLIPOPTIONS     sClip;
    UTSYSTEMOPTIONS70 sSystem;
    VTBYTE            abyUnused[sizeof(UTHXOPTIONS611) + sizeof(UTWXOPTIONS611)];
    UTEXOPTIONS700    sEX;
    UTGDSFOPTIONS832  sGDSF;
    UTDRAMOPTIONS     sDramamine;
    UTPSOPTIONS       sPS;
    UTRNBWOPTIONS     sRainbow;
    UTOPTIONDELTA830  sOptionTrack;
    SCCUTMONTHNAMES   sMonthNames;
    SCCUTNUMBERFORMAT sNumberFormat;
    SCCUTDAYNAMES     sDayNames;
} UTOPTIONS700, * PUTOPTIONS700;

typedef struct UTOPTIONS710tag
{
    VTDWORD           dwSize;
    UTOPTIONDELTA830  sDelta;
    UTPRINTOPTIONS    sPrint;
    UTDISPLAYOPTIONS  sDisplay;
    UTWPOPTIONS       sWp;
    UTSSOPTIONS75     sSs;
    UTBMPOPTIONS      sBmp;
    UTVECOPTIONS      sVec;
    UTDBOPTIONS       sDb;
    UTARCOPTIONS      sArc;
    UTCLIPOPTIONS     sClip;
    UTSYSTEMOPTIONS71 sSystem;
    VTBYTE            abyUnused[sizeof(UTHXOPTIONS611) + sizeof(UTWXOPTIONS611)];
    UTEXOPTIONS710    sEX;
    UTGDSFOPTIONS832  sGDSF;
    UTDRAMOPTIONS     sDramamine;
    UTPSOPTIONS       sPS;
    UTRNBWOPTIONS     sRainbow;
    UTOPTIONDELTA830  sOptionTrack;
    SCCUTMONTHNAMES   sMonthNames;
    SCCUTNUMBERFORMAT sNumberFormat;
    SCCUTDAYNAMES     sDayNames;
} UTOPTIONS710, * PUTOPTIONS710;

typedef struct UTOPTIONS750tag
{
    VTDWORD           dwSize;
    UTOPTIONDELTA830  sDelta;
    UTPRINTOPTIONS    sPrint;
    UTDISPLAYOPTIONS  sDisplay;
    UTWPOPTIONS       sWp;
    UTSSOPTIONS75     sSs;
    UTBMPOPTIONS      sBmp;
    UTVECOPTIONS      sVec;
    UTDBOPTIONS       sDb;
    UTARCOPTIONS      sArc;
    UTCLIPOPTIONS     sClip;
    UTSYSTEMOPTIONS77 sSystem;
    VTBYTE            abyUnused[sizeof(UTHXOPTIONS611) + sizeof(UTWXOPTIONS611)];
    UTEXOPTIONS800    sEX;
    UTGDSFOPTIONS832  sGDSF;
    UTDRAMOPTIONS     sDramamine;
    UTPSOPTIONS       sPS;
    UTRNBWOPTIONS     sRainbow;
    UTOPTIONDELTA830  sOptionTrack;
    SCCUTMONTHNAMES   sMonthNames;
    SCCUTNUMBERFORMAT sNumberFormat;
    SCCUTDAYNAMES     sDayNames;
    UTXXOPTIONS750    sXX;
} UTOPTIONS750, * PUTOPTIONS750;

typedef struct UTOPTIONS770tag
{
    VTDWORD            dwSize;
    UTOPTIONDELTA830   sDelta;
    UTPRINTOPTIONS     sPrint;
    UTDISPLAYOPTIONS   sDisplay;
    UTWPOPTIONS        sWp;
    UTSSOPTIONS832     sSs;
    UTBMPOPTIONS       sBmp;
    UTVECOPTIONS       sVec;
    UTDBOPTIONS        sDb;
    UTARCOPTIONS77     sArc;
    UTCLIPOPTIONS      sClip;
    UTSYSTEMOPTIONS77  sSystem;
    VTBYTE             abyUnused[sizeof(UTHXOPTIONS611) + sizeof(UTWXOPTIONS611)];
    UTEXOPTIONS800     sEX;
    UTGDSFOPTIONS832   sGDSF;
    UTDRAMOPTIONS      sDramamine;
    UTPSOPTIONS        sPS;
    UTRNBWOPTIONS      sRainbow;
    UTOPTIONDELTA830   sOptionTrack;
    SCCUTMONTHNAMES    sMonthNames;
    SCCUTNUMBERFORMAT  sNumberFormat;
    SCCUTDAYNAMES      sDayNames;
    UTXXOPTIONS        sXX;
    UTEXXMLOPTIONS     sEXXML;
    UTCCFLEXOPTIONS770 sCCFlex;
} UTOPTIONS770, * PUTOPTIONS770;

typedef struct UTOPTIONStag80
{
    VTDWORD           dwSize;
    UTOPTIONDELTA830  sDelta;
    UTPRINTOPTIONS    sPrint;
    UTDISPLAYOPTIONS  sDisplay;
    UTWPOPTIONS       sWp;
    UTSSOPTIONS832    sSs;
    UTBMPOPTIONS      sBmp;
    UTVECOPTIONS      sVec;
    UTDBOPTIONS       sDb;
    UTARCOPTIONS      sArc;
    UTCLIPOPTIONS     sClip;
    UTSYSTEMOPTIONS80 sSystem;
    VTBYTE            abyUnused[sizeof(UTHXOPTIONS611) + sizeof(UTWXOPTIONS611)];
    UTEXOPTIONS800    sEX;
    UTGDSFOPTIONS832  sGDSF;
    UTDRAMOPTIONS     sDramamine;
    UTPSOPTIONS       sPS;
    UTRNBWOPTIONS     sRainbow;
    UTOPTIONDELTA830  sOptionTrack;
    SCCUTMONTHNAMES   sMonthNames;
    SCCUTNUMBERFORMAT sNumberFormat;
    SCCUTDAYNAMES     sDayNames;
    UTXXOPTIONS80     sXX;
    UTEXXMLOPTIONS    sEXXML;
    UTCCFLEXOPTIONS   sCCFlex;
} UTOPTIONS80, * PUTOPTIONS80;

typedef struct UTOPTIONStag819
{
    VTDWORD           dwSize;
    UTOPTIONDELTA830  sDelta;
    UTPRINTOPTIONS    sPrint;
    UTDISPLAYOPTIONS  sDisplay;
    UTWPOPTIONS       sWp;
    UTSSOPTIONS832    sSs;
    UTBMPOPTIONS      sBmp;
    UTVECOPTIONS      sVec;
    UTDBOPTIONS       sDb;
    UTARCOPTIONS      sArc;
    UTCLIPOPTIONS     sClip;
    UTSYSTEMOPTIONS   sSystem;
    VTBYTE            abyUnused[sizeof(UTHXOPTIONS611) + sizeof(UTWXOPTIONS611)];
    UTEXOPTIONS800    sEX;
    UTGDSFOPTIONS832  sGDSF;
    UTDRAMOPTIONS     sDramamine;
    UTPSOPTIONS       sPS;
    UTRNBWOPTIONS     sRainbow;
    UTOPTIONDELTA830  sOptionTrack;
    SCCUTMONTHNAMES   sMonthNames;
    SCCUTNUMBERFORMAT sNumberFormat;
    SCCUTDAYNAMES     sDayNames;
    UTXXOPTIONS819    sXX;
    UTEXXMLOPTIONS    sEXXML;
    UTCCFLEXOPTIONS   sCCFlex;
} UTOPTIONS819, * PUTOPTIONS819;

typedef struct UTOPTIONStag830
{
    VTDWORD            dwSize;
    UTOPTIONDELTA830   sDelta;
    UTPRINTOPTIONS     sPrint;
    UTDISPLAYOPTIONS   sDisplay;
    UTWPOPTIONS        sWp;
    UTSSOPTIONS832     sSs;
    UTBMPOPTIONS       sBmp;
    UTVECOPTIONS       sVec;
    UTDBOPTIONS        sDb;
    UTARCOPTIONS       sArc;
    UTCLIPOPTIONS      sClip;
    UTSYSTEMOPTIONS830 sSystem;
    VTBYTE             abyUnused[sizeof(UTHXOPTIONS611) + sizeof(UTWXOPTIONS611)];
    UTEXOPTIONS800     sEX;
    UTGDSFOPTIONS832   sGDSF;
    UTDRAMOPTIONS      sDramamine;
    UTPSOPTIONS        sPS;
    UTRNBWOPTIONS      sRainbow;
    UTOPTIONDELTA830   sOptionTrack;
    SCCUTMONTHNAMES    sMonthNames;
    SCCUTNUMBERFORMAT  sNumberFormat;
    SCCUTDAYNAMES      sDayNames;
    UTXXOPTIONS        sXX;
    UTEXXMLOPTIONS     sEXXML;
    UTCCFLEXOPTIONS    sCCFlex;
    UTWATERMARKOPTIONS sWaterMark;
    DEFAULTPAGESIZE    sDefaultPageSize;
    VTCHAR             szFontPath[SCCUT_FILENAMEMAX];
    VTBOOL             bFilter;
    FONTFILTERLIST     sFontFilterList;
    UTCCODOPTIONS      sCCOD;
    VTBOOL             bEmbedFonts;
    UTOCROPTIONS       sOCR;
    VTLPVOID           pRedirtTempFileCallbackFunc;
} UTOPTIONS830, * PUTOPTIONS830;

typedef struct UTOPTIONStag832
{
    VTDWORD             dwSize;
    UTOPTIONDELTA835    sDelta;
    UTPRINTOPTIONS      sPrint;
    UTDISPLAYOPTIONS    sDisplay;
    UTWPOPTIONS         sWp;
	UTSSOPTIONS832		sSs;
    UTBMPOPTIONS        sBmp;
    UTVECOPTIONS        sVec;
    UTDBOPTIONS         sDb;
    UTARCOPTIONS        sArc;
    UTCLIPOPTIONS       sClip;
    UTSYSTEMOPTIONS     sSystem;
    VTBYTE              abyUnused[sizeof(UTHXOPTIONS611) + sizeof(UTWXOPTIONS611)];
    UTEXOPTIONS         sEX;
    UTGDSFOPTIONS832    sGDSF;
    UTDRAMOPTIONS       sDramamine;
    UTPSOPTIONS         sPS;
    UTRNBWOPTIONS       sRainbow;
    UTOPTIONDELTA835    sOptionTrack;
    SCCUTMONTHNAMES     sMonthNames;
    SCCUTNUMBERFORMAT   sNumberFormat;
    SCCUTDAYNAMES       sDayNames;
    UTXXOPTIONS         sXX;
    UTEXXMLOPTIONS      sEXXML;
    UTCCFLEXOPTIONS     sCCFlex;
	UTIXOPTIONS         sIX;
	UTPXOPTIONS         sPX;
    FONTFILTERLIST      sFontFilterList;
    UTCCODOPTIONS       sCCOD;
    UTOCROPTIONS        sOCR;
    VTLPVOID            pRedirtTempFileCallbackFunc;
} UTOPTIONS832, * PUTOPTIONS832;

typedef struct UTOPTIONStag835
{
    VTDWORD             dwSize;
    UTOPTIONDELTA835    sDelta;
    UTPRINTOPTIONS      sPrint;
    UTDISPLAYOPTIONS    sDisplay;
    UTWPOPTIONS         sWp;
    UTSSOPTIONS         sSs;
    UTBMPOPTIONS        sBmp;
    UTVECOPTIONS        sVec;
    UTDBOPTIONS         sDb;
    UTARCOPTIONS        sArc;
    UTCLIPOPTIONS       sClip;
    UTSYSTEMOPTIONS     sSystem;
    VTBYTE              abyUnused[sizeof(UTHXOPTIONS611) + sizeof(UTWXOPTIONS611)];
    UTEXOPTIONS         sEX;
    UTGDSFOPTIONS       sGDSF;
    UTDRAMOPTIONS       sDramamine;
    UTPSOPTIONS         sPS;
    UTRNBWOPTIONS       sRainbow;
    UTOPTIONDELTA835    sOptionTrack;
    SCCUTMONTHNAMES     sMonthNames;
    SCCUTNUMBERFORMAT   sNumberFormat;
    SCCUTDAYNAMES       sDayNames;
    UTXXOPTIONS         sXX;
    UTEXXMLOPTIONS      sEXXML;
    UTCCFLEXOPTIONS     sCCFlex;
    UTIXOPTIONS         sIX;
    UTPXOPTIONS         sPX;
    FONTFILTERLIST      sFontFilterList;
    UTCCODOPTIONS       sCCOD;
    UTOCROPTIONS        sOCR;
    VTLPVOID            pRedirtTempFileCallbackFunc;
    VTCHAR              szLotusNotesDirectory[SCCUT_FILENAMEMAX];
} UTOPTIONS835, * PUTOPTIONS835;

typedef struct UTOPTIONS837tag
{
    VTDWORD             dwSize;
    UTOPTIONDELTA       sDelta;
    UTPRINTOPTIONS      sPrint;
    UTDISPLAYOPTIONS    sDisplay;
    UTWPOPTIONS         sWp;
    UTSSOPTIONS         sSs;
    UTBMPOPTIONS        sBmp;
    UTVECOPTIONS        sVec;
    UTDBOPTIONS         sDb;
    UTARCOPTIONS        sArc;
    UTCLIPOPTIONS       sClip;
    UTSYSTEMOPTIONS837  sSystem;
    VTBYTE              abyUnused[sizeof(UTHXOPTIONS611) + sizeof(UTWXOPTIONS611)];
    UTEXOPTIONS         sEX;
    UTGDSFOPTIONS       sGDSF;
    UTDRAMOPTIONS       sDramamine;
    UTPSOPTIONS         sPS;
    UTRNBWOPTIONS       sRainbow;
    UTOPTIONDELTA       sOptionTrack;
    SCCUTMONTHNAMES     sMonthNames;
    SCCUTNUMBERFORMAT   sNumberFormat;
    SCCUTDAYNAMES       sDayNames;
    UTXXOPTIONS         sXX;
    UTEXXMLOPTIONS      sEXXML;
    UTCCFLEXOPTIONS     sCCFlex;
    UTIXOPTIONS         sIX;
    UTPXOPTIONS         sPX;
    FONTFILTERLIST      sFontFilterList;
    UTCCODOPTIONS       sCCOD;
    UTOCROPTIONS        sOCR;
    VTLPVOID            pRedirtTempFileCallbackFunc;
    VTCHAR              szLotusNotesDirectory[SCCUT_FILENAMEMAX];
    VTWORD              wOLEEmbeddingsMode;
} UTOPTIONS837, * PUTOPTIONS837;


typedef struct UTOPTIONS841tag
{
    VTDWORD             dwSize;
    UTOPTIONDELTA       sDelta;
    UTPRINTOPTIONS      sPrint;
    UTDISPLAYOPTIONS    sDisplay;
    UTWPOPTIONS         sWp;
    UTSSOPTIONS         sSs;
    UTBMPOPTIONS        sBmp;
    UTVECOPTIONS        sVec;
    UTDBOPTIONS         sDb;
    UTARCOPTIONS        sArc;
    UTCLIPOPTIONS       sClip;
    UTSYSTEMOPTIONS     sSystem;
    VTBYTE              abyUnused[sizeof(UTHXOPTIONS611) + sizeof(UTWXOPTIONS611)];
    UTEXOPTIONS         sEX;
    UTGDSFOPTIONS       sGDSF;
    UTDRAMOPTIONS       sDramamine;
    UTPSOPTIONS         sPS;
    UTRNBWOPTIONS       sRainbow;
    UTOPTIONDELTA       sOptionTrack;
    SCCUTMONTHNAMES     sMonthNames;
    SCCUTNUMBERFORMAT   sNumberFormat;
    SCCUTDAYNAMES       sDayNames;
    UTXXOPTIONS         sXX;
    UTEXXMLOPTIONS      sEXXML;
    UTCCFLEXOPTIONS     sCCFlex;
    UTIXOPTIONS         sIX;
    UTPXOPTIONS         sPX;
    FONTFILTERLIST      sFontFilterList;
    UTCCODOPTIONS       sCCOD;
    UTOCROPTIONS        sOCR;
    VTLPVOID            pRedirtTempFileCallbackFunc;
    VTCHAR              szLotusNotesDirectory[SCCUT_FILENAMEMAX];
    VTWORD              wOLEEmbeddingsMode;
	SCCUTEMAILOPTIONS   sEmail;
} UTOPTIONS841, * PUTOPTIONS841;

typedef struct UTOPTIONS850tag
{
    VTDWORD             dwSize;
    UTOPTIONDELTA       sDelta;
    UTPRINTOPTIONS      sPrint;
    UTDISPLAYOPTIONS    sDisplay;
    UTWPOPTIONS         sWp;
    UTSSOPTIONS         sSs;
    UTBMPOPTIONS        sBmp;
    UTVECOPTIONS        sVec;
    UTDBOPTIONS         sDb;
    UTARCOPTIONS        sArc;
    UTCLIPOPTIONS       sClip;
    UTSYSTEMOPTIONS     sSystem;
    VTBYTE              abyUnused[sizeof(UTHXOPTIONS611) + sizeof(UTWXOPTIONS611)];
    UTEXOPTIONS         sEX;
    UTGDSFOPTIONS       sGDSF;
    UTDRAMOPTIONS       sDramamine;
    UTPSOPTIONS         sPS;
    UTRNBWOPTIONS       sRainbow;
    UTOPTIONDELTA       sOptionTrack;
    SCCUTMONTHNAMES     sMonthNames;
    SCCUTNUMBERFORMAT   sNumberFormat;
    SCCUTDAYNAMES       sDayNames;
    UTXXOPTIONS         sXX;
    UTEXXMLOPTIONS      sEXXML;
    UTCCFLEXOPTIONS     sCCFlex;
    UTIXOPTIONS         sIX;
    UTPXOPTIONS         sPX;
    FONTFILTERLIST      sFontFilterList;
    UTCCODOPTIONS       sCCOD;
    UTOCROPTIONS        sOCR;
    VTLPVOID            pRedirtTempFileCallbackFunc;
    VTCHAR              szLotusNotesDirectory[SCCUT_FILENAMEMAX];
    VTWORD              wOLEEmbeddingsMode;
	SCCUTEMAILOPTIONS   sEmail;
	UTWPOPTIONS         sHtmlDoc;
	UTWPOPTIONS         sEmailDoc;
    UTH5XOPTIONS        sH5X;
} UTOPTIONS850, * PUTOPTIONS850;

typedef struct UTOPTIONS852tag
{
    VTDWORD             dwSize;
    UTOPTIONDELTA       sDelta;
    UTPRINTOPTIONS      sPrint;
    UTDISPLAYOPTIONS    sDisplay;
    UTWPOPTIONS         sWp;
    UTSSOPTIONS         sSs;
    UTBMPOPTIONS        sBmp;
    UTVECOPTIONS        sVec;
    UTDBOPTIONS         sDb;
    UTARCOPTIONS        sArc;
    UTCLIPOPTIONS       sClip;
    UTSYSTEMOPTIONS     sSystem;
    VTBYTE              abyUnused[sizeof(UTHXOPTIONS611) + sizeof(UTWXOPTIONS611)];
    UTEXOPTIONS         sEX;
	UTGDSFOPTIONS852    sGDSF;
    UTDRAMOPTIONS       sDramamine;
    UTPSOPTIONS         sPS;
    UTRNBWOPTIONS       sRainbow;
    UTOPTIONDELTA       sOptionTrack;
    SCCUTMONTHNAMES     sMonthNames;
    SCCUTNUMBERFORMAT   sNumberFormat;
    SCCUTDAYNAMES       sDayNames;
    UTXXOPTIONS         sXX;
    UTEXXMLOPTIONS      sEXXML;
    UTCCFLEXOPTIONS     sCCFlex;
    UTIXOPTIONS         sIX;
    UTPXOPTIONS         sPX;
    FONTFILTERLIST      sFontFilterList;
    UTCCODOPTIONS       sCCOD;
    UTOCROPTIONS        sOCR;
    VTLPVOID            pRedirtTempFileCallbackFunc;
    VTCHAR              szLotusNotesDirectory[SCCUT_FILENAMEMAX];
    VTWORD              wOLEEmbeddingsMode;
	SCCUTEMAILOPTIONS   sEmail;
    VTBOOL              bStrokeText;
	VTBOOL				bMaintainZoom;
	UTWPOPTIONS         sHtmlDoc;
	UTWPOPTIONS         sEmailDoc;
    UTH5XOPTIONS        sH5X;
} UTOPTIONS852, * PUTOPTIONS852;

typedef struct UTOPTIONStag
{
    VTDWORD             dwSize;
    UTOPTIONDELTA       sDelta;
    UTPRINTOPTIONS      sPrint;
    UTDISPLAYOPTIONS    sDisplay;
    UTWPOPTIONS         sWp;
    UTSSOPTIONS         sSs;
    UTBMPOPTIONS        sBmp;
    UTVECOPTIONS        sVec;
    UTDBOPTIONS         sDb;
    UTARCOPTIONS        sArc;
    UTCLIPOPTIONS       sClip;
    UTSYSTEMOPTIONS     sSystem;
    VTBYTE              abyUnused[sizeof(UTHXOPTIONS611) + sizeof(UTWXOPTIONS611)];
    UTEXOPTIONS         sEX;
    UTGDSFOPTIONS       sGDSF;
    UTDRAMOPTIONS       sDramamine;
    UTPSOPTIONS         sPS;
    UTRNBWOPTIONS       sRainbow;
    UTOPTIONDELTA       sOptionTrack;
    SCCUTMONTHNAMES     sMonthNames;
    SCCUTNUMBERFORMAT   sNumberFormat;
    SCCUTDAYNAMES       sDayNames;
    UTXXOPTIONS         sXX;
    UTEXXMLOPTIONS      sEXXML;
    UTCCFLEXOPTIONS     sCCFlex;
    UTIXOPTIONS         sIX;
    UTPXOPTIONS         sPX;
    FONTFILTERLIST      sFontFilterList;
    UTCCODOPTIONS       sCCOD;
    UTOCROPTIONS        sOCR;
    VTLPVOID            pRedirtTempFileCallbackFunc;
    VTCHAR              szLotusNotesDirectory[SCCUT_FILENAMEMAX];
    VTWORD              wOLEEmbeddingsMode;
	SCCUTEMAILOPTIONS   sEmail;
    VTBOOL              bStrokeText;
	VTBOOL				bMaintainZoom;
	UTWPOPTIONS         sHtmlDoc;
	UTWPOPTIONS         sEmailDoc;
    UTH5XOPTIONS        sH5X;
    UTPDFOPTIONS        sPdf;
} UTOPTIONS, * PUTOPTIONS;


typedef union
{
    VTDWORD  dwValue;
    VTLPBYTE szValue;
} SCCOPTIONVAL;

typedef struct
{
    VTDWORD dwOptionId;
    VTWORD  wOptionType;
    SCCOPTIONVAL Value;
} SCCOPTIONINIT;

/* Values for SCCOPTIONINIT.dwOptionType */
#define SCCOPT_DWORD       0
#define SCCOPT_STRING      1

/* Flags Option Sets */
#define SCCOPTF_DEFAULTSET     0x00000001
#define SCCOPTF_GLOBALSET      0x00000002
#define SCCOPTF_CHILDSET       0x00000004




/*** For safeguarding DLL loading via callback ***/

typedef struct SCCVWDLLLOADCALLBACKtag
{
    VTDWORD             dwSize;
    VTLPVOID            lpVector;
    VTDWORD             dwVectorLength;
    VTDWORD             dwResult;
    VTHANDLE            hLib;
    VTLPBYTE            lpszPath;
    VTLPBYTE            lpszEntryPoint;

} SCCVWDLLLOADCALLBACK, * PSCCVWDLLLOADCALLBACK;


#define SCCVW_CALLBACK_FAILURE              0
#define SCCVW_CALLBACK_CHECKED              1
#define SCCVW_CALLBACK_CHECKED_AND_LOADED   2

#ifdef __cplusplus
}
#endif

#ifdef SCC_PACKED_BY_SCCOP_H
#pragma pack(pop)
#undef SCC_PACKED_BY_SCCOP_H
#undef SCC_PACK_SET
#endif /* SCC_PACKED_BY_SCCOP_H */

#endif /* _SCCOPT_H */
