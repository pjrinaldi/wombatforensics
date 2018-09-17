/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. 
All rights reserved.*/

/*
|   SCC Viewer Technology - Source file
|
|   Code:            sccex.h
|   Module:          SCC Export Public Header
|   Developer:
|   Environment:     Portable
|   Function:        Data Access Structures and Macros
*/


#ifndef _SCCEX_H
#define _SCCEX_H

#ifdef WINDOWS
#ifndef SCC_PACK_SET
#define SCC_PACK_SET 1
#define SCC_PACKED_BY_SCCEX_H 1
#pragma pack(push,8)
#endif /* SCC_PACK_SET */
#endif /* WINDOWS */

#include <sccda.h>
#include <sccvw.h>
#include <sccanno.h>
#ifndef NLM
#include <wchar.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*
|   VTHEXPORT definition
*/

typedef VTHDOC VTHEXPORT, *VTLPHEXPORT;

/*
|   EX callback function definition and commands
*/

typedef DAERR (DA_ENTRYMODPTR EXCALLBACKPROC)(VTHEXPORT hExport, VTSYSPARAM dwCallbackData, VTDWORD dwCommandOrInfoId, VTLPVOID pCommandOrInfoData);

/*  #define EXINFO_GRAPHIC_HEIGHT   1   * pCommandOrInfoData is pointer to DWORD containing height of the resulting graphic */
/*  #define EXINFO_GRAPHIC_WIDTH    2   * pCommandOrInfoData is pointer to DWORD containing width of the resulting graphic */

/*
|   Callback IDs
*/

#define     EX_CALLBACK_ID_OEMSTRINFO              0x0FE   /* Publishing Server OEM string      */
#define     EX_CALLBACK_ID_URLINFO                 0x0FF   /* Publishing Server URL             */
/*          EX_CALLBACK_ID_STRFILECALLBACK         0x0FF      Publishing Server File Callback   */
#define     EX_CALLBACK_ID_GETROOTURL              0x100
#define     EX_CALLBACK_ID_CREATENEWFILE           0x101
#define     EX_CALLBACK_ID_NEWFILEINFO             0x102
#define     EX_CALLBACK_ID_OEMOUTPUT               0x103
/* Left space here for two callbacks that might come back */
#define     EX_CALLBACK_ID_EXPORTCOMPLETE          0x106   /* Not available in HTML Export or Wireless Export */

/*
|   Callback IDs reserved for RainGear
*/

#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_0    0x104
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_1    0x105
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_2    0x106
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_3    0x107
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_4    0x108
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_5    0x109
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_6    0x10A
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_7    0x10B
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_8    0x10C
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_9    0x10D
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_A    0x10E
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_B    0x10F
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_C    0x110
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_D    0x111
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_E    0x112
#define     EX_CALLBACK_ID_RESVD_FOR_RAINGEAR_F    0x113

#define     EX_CALLBACK_ID_PROCESSLINK             0x114
#define     EX_CALLBACK_ID_PROCESSELEMENTSTR       0x115
#define     EX_CALLBACK_ID_CUSTOMELEMENTLIST       0x116
#define     EX_CALLBACK_ID_GRAPHICSIZEINFO         0x117 /* EXGDSF callback with graphic size information                     */
#define     EX_CALLBACK_ID_GRAPHICEXPORTFAILURE    0x118 /* EXGDSF callback on any error in EXGDSF's VwExportWriteFunc        */
#define     EX_CALLBACK_ID_OEMOUTPUT_VER2          0x119
#define     EX_CALLBACK_ID_PROCESSELEMENTSTR_VER2  0x11A
#define     EX_CALLBACK_ID_ALTLINK                 0x11B
#define     EX_CALLBACK_ID_REFLINK                 0x11C
#define     EX_CALLBACK_ID_ENTERARCHIVE            0x11D
#define     EX_CALLBACK_ID_LEAVEARCHIVE            0x11E
#define     EX_CALLBACK_ID_PROCESSHYPERLINK        0x11F
#define     EX_CALLBACK_ID_GRAPHICINFO             0x120
#define     EX_CALLBACK_ID_OCRERROR                0x121
#define     EX_CALLBACK_ID_PAGECOUNT               0x122
#define     EX_CALLBACK_ID_PROCESSHIGHLIGHT        0x123
#define     EX_CALLBACK_ID_REFLINKEX               0x124
#define     EX_CALLBACK_ID_BEGINPAGE               0x125
#define     EX_CALLBACK_ID_ENDPAGE                 0x126

/*
|   Typedefs and Defines for Export Utility Function EUNeedNewFile
*/

#define     EUERR_OK                    000
#define     EUERR_FCREATE_FAIL          101

#define     CU_HAVERELATIVEPATH         0x01    /* szRelativePath is valid   */
#define     CU_HAVEABSOLUTEPATH         0x02    /* szAbsolutePath is valid  */
#define     CU_SIBLING                  0x201
#define     CU_CHILD                    0x202
#define     CU_ROOT                     0x204
#define     CU_COPY                     0x208

#define     VT_MAX_URL                  2048
#define     VT_MAX_UNICODE_URL          (VT_MAX_URL * sizeof(VTWORD))
/* VT_MAX_PATH remains for backwards compatibility.  Future development should use VT_MAX_URL. */
#define     VT_MAX_PATH                 VT_MAX_URL
#define     VT_MAX_FILEPATH             SCCUT_FILENAMEMAX
#define     VT_MAX_UNICODE_FILEPATH     (VT_MAX_FILEPATH * sizeof(VTWORD))

/*
|   File I/O structure definition
*/

typedef struct EXPSFILECALLBACKDATAtag
{
   VTDWORD              dwSize;
   VTDWORD              dwCurrFileID;           /* ID of the current file */
   VTDWORD              dwLinkID;               /* ID of the link file */
   VTDWORD              dwOffset;               /* Seek offset in the current file where URL starts */
   VTDWORD              dwLinkStrLen;           /* string length of the URL */
   VTDWORD              dwFlags;                /* Flags */
} EXPSFILECALLBACKDATA;

typedef struct EXURLFILEIOCALLBACKDATAtag
{
   VTDWORD              dwSize;                 /* Set by caller to sizeof(EXURLFILEIOCALLBACKDATA) */
   VTBYTE               szURLString[VT_MAX_URL];
   VTDWORD              dwFileID;               /* Each output file created is given a unique ID by export. */
} EXURLFILEIOCALLBACKDATA;

/*
|   This structure is being added for use with Unicode path names.
|   Using this will prevent the szURLString field from being overloaded
|   with data that is not actually bytes, but WORDs.
*/
typedef struct EXURLFILEIOCALLBACKDATAWtag
{
   VTDWORD              dwSize;                 /* Set by caller to sizeof(EXURLFILEIOCALLBACKDATAW) */
   VTWCHAR              wzURLString[VT_MAX_URL];
   VTDWORD              dwFileID;               /* Each output file created is given a unique ID by export. */
} EXURLFILEIOCALLBACKDATAW;

/*
|   These #defines are to provide backwards compatibility for HTML
|   Export users.  They should remain in the code indefinitely.
*/
#define EXHTMLFILEIOCALLBACKDATA        EXURLFILEIOCALLBACKDATA
#define EXHTMLFILEIOCALLBACKDATAtag     EXURLFILEIOCALLBACKDATAtag
#define EXHTMLFILEIOCALLBACKDATAW       EXURLFILEIOCALLBACKDATAW
#define EXHTMLFILEIOCALLBACKDATAWtag    EXURLFILEIOCALLBACKDATAWtag

/* Defines for the dwInfoGraphic of the EXGRAPHICALTSTRINGINFO structure. */
#define EXINFO_GRAPHIC_ALTSTRINGID 1 /* When dwInfoGraphicType is 1, then dwBasedOnType will be an dwAltStringId DWORD. */

typedef struct EXGRAPHICINFOCALLBACKDATAtag
{
    VTDWORD dwGraphicInfoType; /* See defines above. */
    VTDWORD dwBasedOnType;     /* This is based on dwGraphicInfoType. */
} EXGRAPHICALTSTRINGINFO;


typedef struct EXPROCESSLINKCALLBACKDATAtag
{
   VTLPVOID             pLocatorStr;            /* Pointer to a 1024 byte buffer containing the object location              */
   VTDWORD              dwLocatorStrCharset;    /* Character set that the string pLocatorStr points to is in                 */
   VTDWORD              dwObjectFileId;         /* FI ID indicating type of data contained in the embedded object            */
   VTDWORD              dwAction;               /* Return value from OEM saying what to do with this object                  */
   VTDWORD              dwLinkFlags;            /* Flags controlling how an <IMAGE> take for a verbatium link should be done */
   VTHANDLE             hReserved;              /* Reserved for future use                                                   */
} EXPROCESSLINKCALLBACKDATA;


/* dwAction field values for EXPROCESSLINKCALLBACKDATA */
#define  EX_ACTION_CREATELINK       1
#define  EX_ACTION_SKIP             2
#define  EX_ACTION_CONVERT          3
#define  EX_ACTION_ERROR            0xFFFFFFFF

/* dwLinkFlags flags */
#define  EX_FLAG_USEOUTPUTCHARSET   0x00000001
#define  EX_FLAG_PUTATTRIBUTES      0x00000002
#define  EX_FLAG_DIMENSIONSKNOWN    0x00000004
#define  EX_FLAG_DEFAULT            EX_FLAG_USEOUTPUTCHARSET | EX_FLAG_PUTATTRIBUTES | EX_FLAG_DIMENSIONSKNOWN


typedef struct EXFILEIOCALLBACKDATAtag
{
    HIOFILE         hParentFile;                /* handle of the first output file that the new file is associated with     */
    VTDWORD         dwParentOutputId;           /* FI ID indicating type of data parent file will contain                   */
    VTDWORD         dwAssociation;              /* information on how new file should relate to the file passed in hFile    */
    VTDWORD         dwOutputId;                 /* FI ID indicating type of data that this file will contain                */
    VTDWORD         dwFlags;                    /* Reserved; set to 0                                                       */
    VTDWORD         dwSpecType;
    VTLPVOID        pSpec;
    VTLPVOID        pExportData;                /* more file info based on the dwParentOutput ID;                           */
                                                /* see EXURLFILEIOCALLBACKDATA above                                        */
    VTLPVOID        pTemplateName;              /* pointer to the name of the template                                      */
} EXFILEIOCALLBACKDATA;


typedef struct EXOEMOUTCALLBACKDATAtag
{
    HIOFILE         hFile;                      /* handle of the output file currently opened for writing                   */
    VTLPBYTE        pOutStr;                    /* the OEM string specified in the template                                 */
} EXOEMOUTCALLBACKDATA;


typedef struct EXOEMOUTCALLBACKDATAWtag
{
    HIOFILE         hFile;                      /* handle of the output file currently opened for writing                   */
    VTLPWORD        pwOutStr;                   /* the OEM string specified in the template                                 */
} EXOEMOUTCALLBACKDATAW;


typedef struct EXOEMOUTCALLBACKDATA_VER2tag
{
    VTDWORD         dwSize;                     /* size of struct                                                           */
    VTDWORD         dwCharset;                  /* output character set of buffer contents                                  */
    VTDWORD         dwLength;                   /* length of the string in characters                                       */
    VTLPVOID        pOEMString;                 /* the OEM string specified in the template                                 */
    VTLPWORD        pwBuffer;                   /* pointer to output buffer                                                 */
} EXOEMOUTCALLBACKDATA_VER2;


typedef struct EXCUSTOMELEMENTCALLBACKDATAtag
{
    HIOFILE         hFile;                      /* handle to the current output file                                        */
    VTLPVOID        pKeyStr;                    /* pointer to the keyword in the custom element                             */
    VTLPVOID        pElementStr;                /* pointer to the string after the keyword                                  */
} EXCUSTOMELEMENTCALLBACKDATA;


typedef struct EXCUSTOMELEMENTCALLBACKDATA_VER2tag
{
    VTDWORD         dwSize;                     /* size of struct (for versioning)                                          */
    VTDWORD         dwCharset;                  /* output character set of buffer contents                                  */
    VTDWORD         dwLength;                   /* length of the string in characters                                       */
    VTLPVOID        pKeyStr;                    /* pointer to the custom element keyword                                    */
    VTLPVOID        pElementStr;                /* pointer to the string after the keyword                                  */
    VTLPWORD        pwBuffer;                   /* pointer to output buffer                                                 */
} EXCUSTOMELEMENTCALLBACKDATA_VER2;


typedef struct EXALTLINKCALLBACKDATAtag
{
    VTDWORD         dwType;                     /* anchor type                                                              */
    VTLPVOID        pAltURLStr;                 /* pointer to null-terminated URL                                           */
} EXALTLINKCALLBACKDATA;

/* dwType field values for EXALTLINKCALLBACKDATA */
#define EX_ALTLINK_PREV 0
#define EX_ALTLINK_NEXT 1

typedef struct EXGRAPHICEXPORTINFOtag
{
    HIOFILE         hFile;                      /* the OPEN graphic file                                                    */
    VTLPDWORD       pXSize;                     /* ->horizontal image size                                                  */
    VTLPDWORD       pYSize;                     /* ->vertical image size                                                    */
    VTDWORD         dwOutputId;                 /* FI ID .. type of data that this file should contain                      */
    SCCERR          ExportGraphicStatus;        /* current error code to be returned from graphic export                    */
    VTLPDWORD       pImageSize;                 /* the maximum size for the image is passed in to the callback              */
                                                /* function, the expected return value is the size of the image             */
                                                /* created by the callback function.                                        */
}   EXGRAPHICEXPORTINFO;

typedef struct EXGRAPHICDEMENSIONINFOtag
{
    VTLONG          x;                          /* width of the exported graphic in pixels */
    VTLONG          y;                          /* height of the exported graphic in pixels */
    VTDWORD         dwSize;                     /* size of the exported graphic image in bytes */
} EXGRAPHICDEMENSIONINFO;

typedef struct EUNEWFILEINFOtag
{
    VTDWORD         d;
    VTLPVOID        p;
} EUNEWFILEINFO;


typedef struct EXENTERARCHIVECALLBACKDATAtag
{
    VTDWORD         dwSpecType;                 /* spec type                    */
    VTLPVOID        pSpec;                      /* pointer to spec              */
    VTLPWORD        wzFullName;                 /* full name of archived file   */
    VTDWORD         dwItemNum;                  /* archive item number          */
} EXENTERARCHIVECALLBACKDATA;

typedef struct EXLEAVEARCHIVECALLBACKDATAtag
{
    SCCERR          ExportResult;
} EXLEAVEARCHIVECALLBACKDATA;


typedef struct EXREFLINKCALLBACKDATAtag
{
    VTLPSTR         pSubdocSpec;
    VTCHAR          URL[VT_MAX_URL];
    VTLPWORD		wzFullName;
} EXREFLINKCALLBACKDATA;

typedef struct EXREFLINKCALLBACKDATAEXtag
{
    VTLPSTR         pSubdocSpec;
    VTCHAR          *URL;
    VTDWORD         dwURLlen;
    VTLPWORD		wzFullName;
} EXREFLINKCALLBACKDATAEX;



/* WX minimum text buffer size */
#define EX_TEXTBUFFERSIZE_MIN_SMALL 200
#define EX_TEXTBUFFERSIZE_MIN_BIG   400


/*
|   The default grid dimensions are based on the fact that anything
|   much bigger than this tends to kill browsers...
*/
#define EX_GRIDROWS_DEFAULT 5000
#define EX_GRIDCOLS_DEFAULT 100

/* It's possible to run export on a locator id. With h5, customers
   may want to do just that. Create a value here to match our internal
   value of SO_LOCATORTYPE_DOSPECIAL. */
#define EX_OPEN_ATTACHMENT 256

/* 
|   The structure for the annotation
*/
typedef struct EXANNOUSERDATAtag
{
    VTDWORD     dwSize;     /* sizeof(EXANNOUSERDATA) */
    VTLPBYTE    pBookmark;  /* Used by HX only */
    VTLPBYTE    pHyperlink; /* HX only */
} EXANNOUSERDATA, *PEXANNOUSERDATA;

typedef struct EXANNOHILITETEXTtag
{
    VTDWORD     dwSize;
    VTDWORD     dwStartACC;
    VTDWORD     dwEndACC;
    VTLPBYTE    pBookmark;
    VTLPBYTE    pHyperlink;
    VTDWORD     dwOptions;
    SCCVWCOLORREF sForeground;
    SCCVWCOLORREF sBackground;
    VTWORD      wCharAttr;
    VTWORD      wCharAttrMask;
} EXANNOHILITETEXT,  *PEXANNOHILITETEXT;

typedef struct EXANNOINSERTTEXTtag
{
    VTDWORD     dwSize;
    VTDWORD     dwTextACC;
    VTLPWORD    pText;
    VTLPBYTE    pBookmark; 
    VTLPBYTE    pHyperlink; 
    VTDWORD     dwOptions;
    SCCVWCOLORREF sForeground;
    SCCVWCOLORREF sBackground;
    VTWORD      wCharAttr;
    VTWORD      wCharAttrMask;

} EXANNOINSERTTEXT,  *PEXANNOINSERTTEXT;

typedef struct EXANNOHIDETEXTtag
{
    VTDWORD     dwSize;
    VTDWORD     dwStartACC;
    VTDWORD     dwEndACC;
    VTLPBYTE    pBookmark; 
} EXANNOHIDETEXT,  *PEXANNOHIDETEXT;

/* Area highlight struct */
typedef struct EXANNOHILITEAREAtag
{
    VTDWORD         dwSize;
    VTDWORD         dwSection;        /* 0-based sheet/image/slide index */
    VTDWORD         dwTop;            /* Top coordinate or row */
    VTDWORD         dwLeft;           /* Leftmost coordinate or column */
    VTDWORD         dwWidth;          /* Width of area in coordinates or columns */
    VTDWORD         dwHeight;         /* Height of area in coordinates or rows */
    VTDWORD         dwUnits;          /* Unit type: EX_ANNO_UNITS_PIXELS or EX_ANNO_UNITS_TWIPS */
    VTDWORD         dwUser;           /* User data */

    SCCVWCOLORREF   fillColor;
    VTFLOAT         fOpacity;         /* 0-1.0; 0=invisible; applies to fillColor. */

    SCCVWCOLORREF   borderTopColor;   /* Border colors and thicknesses in twips */
    VTDWORD         borderTopThickness;
    SCCVWCOLORREF   borderLeftColor;
    VTDWORD         borderLeftThickness;
    SCCVWCOLORREF   borderBottomColor;
    VTDWORD         borderBottomThickness;
    SCCVWCOLORREF   borderRightColor;
    VTDWORD         borderRightThickness;
    VTDWORD         dwBorderStyle;     /* Not yet supported. */
                                       /* (would be EX_ANNO_BORDER_DOT, EX_ANNO_BORDER_DASH, EX_ANNO_BORDER_SOLID) */
} EXANNOHILITEAREA, *PEXANNOHILITEAREA;

/* Area highlight border styles. Currently we only support EX_ANNO_BORDER_SOLID. */
#define EX_ANNO_BORDER_NONE     SCCANNO_BORDER_NONE
#define EX_ANNO_BORDER_SOLID    SCCANNO_BORDER_SOLID
#define EX_ANNO_BORDER_DOT      SCCANNO_BORDER_DOT
#define EX_ANNO_BORDER_DASH     SCCANNO_BORDER_DASH

/* Redaction annotation struct */
#define EXANNO_MAXLABEL   128
typedef struct EXANNOREDACTTEXTtag
{
    VTDWORD   dwSize;
    VTDWORD   dwStartACC;
    VTDWORD   dwEndACC;
    VTWCHAR   wzLabel[EXANNO_MAXLABEL];
} EXANNOREDACTTEXT, *PEXANNOREDACTTEXT;

/* Image stamp annotation struct */
typedef struct EXANNOSTAMPtag
{
    VTDWORD    dwSize;
    VTDWORD    dwSection;         /* zero based number of (sheet/image/slide) */
    VTDWORD    dwTop;             /* Top coordinate or row */
    VTDWORD    dwLeft;            /* Leftmost coordinate or column */
    VTDWORD    dwWidth;           /* Width of area in coordinates or columns */
    VTDWORD    dwHeight;          /* Height of area in coordinates or columns */
    VTDWORD    dwUnits;           /* Unit type: EX_ANNO_UNITS_PIXELS or EX_ANNO_UNITS_TWIPS */
    VTDWORD    dwUser;            /* User data */
    VTFLOAT    fOpacity;          /* 0-1.0; 0==invisible, 1.0==opaque */
    VTDWORD    dwSizeMode;        /* EX_ANNO_SIZE_FIT, EX_ANNO_SIZE_STRETCH, EX_ANNO_SIZE_FROM_SOURCE */
    VTLPCSTR   szStampName;       /* Registered name of stamp image (NULL-terminated string) */
} EXANNOSTAMP, *PEXANNOSTAMP;

/* Values for dwUnits in the EXANNOHILITEAREA and EXANNOSTAMP structs */
#define EX_ANNO_UNITS_PIXELS        SCCANNO_UNITS_PIXELS
#define EX_ANNO_UNITS_TWIPS         SCCANNO_UNITS_TWIPS
#define EX_ANNO_UNITS_CELLS         SCCANNO_UNITS_CELLS
#define EX_ANNO_UNITS_PCT           SCCANNO_UNITS_PCT

/* Values for EXANNOSTAMP's dwSizeMode field */
#define EX_ANNO_SIZE_FIT            SCCANNO_SIZE_FIT
#define EX_ANNO_SIZE_STRETCH        SCCANNO_SIZE_STRETCH
#define EX_ANNO_SIZE_FROM_SOURCE    SCCANNO_SIZE_FROM_SOURCE


typedef struct OCRTEXTtag
{
    wchar_t * pText;
    VTDWORD byteLen;  /*This is the length in BYTES, NOT the # of wchars.*/
    struct OCRTEXTtag* pNext;  /*this allows link-list, better for dynamic allocation*/
}EXOCRTEXT, *PEXOCRTEXT;

#define OCR_MAX_NUM_TXTS 1
typedef struct OCRTEXTCOLLtag
{
    /*doing this dynamically is pretty hard, so doing static.
      dwNumOfTxts will allow for tightest possible loops*/
    EXOCRTEXT OcrTxt[OCR_MAX_NUM_TXTS];
    VTDWORD dwNumOfTxts;
}EXOCRTEXTCOLLECTION, *PEXOCRTEXTCOLLECTION;

/*
|   EX functions
*/

DA_ENTRYSC SCCERR DA_ENTRYMOD EXOpenExport(
    VTHDOC          hDoc,
    VTDWORD         dwOutputId,
    VTDWORD         dwSpecType,
    VTLPVOID        pSpec,
    VTDWORD         dwFlags,
    VTSYSPARAM      dwReserved,
    EXCALLBACKPROC  pCallbackFunc,
    VTSYSPARAM      dwCallbackData,
    VTLPHEXPORT     phExport );
DA_ENTRYSC SCCERR DA_ENTRYMOD EXRunExport(VTHEXPORT hExport);
DA_ENTRYSC SCCERR DA_ENTRYMOD EXCloseExport(VTHEXPORT hExport);
DA_ENTRYSC SCCERR DA_ENTRYMOD EXDoCallback(VTHEXPORT hExport, VTDWORD dwCommandOrInfoId, VTLPVOID pCommandOrInfoData);
DA_ENTRYSC SCCERR DA_ENTRYMOD EXExportStatus(VTHEXPORT hExport, VTDWORD dwStatusType, VTLPVOID pStatus);
DA_ENTRYSC SCCERR DA_ENTRYMOD EXCountPages(VTHDOC hDoc, VTDWORD dwOutputId, VTLPDWORD pdwPageCount);

/* Annotation functions */
DA_ENTRYSC SCCERR DA_ENTRYMOD EXHiliteText(VTHEXPORT hExport,PEXANNOHILITETEXT pHiliteText);
DA_ENTRYSC SCCERR DA_ENTRYMOD EXInsertText(VTHEXPORT hExport, PEXANNOINSERTTEXT pInsertText);
DA_ENTRYSC SCCERR DA_ENTRYMOD EXHideText(VTHEXPORT hExport, PEXANNOHIDETEXT pHideText);
DA_ENTRYSC SCCERR DA_ENTRYMOD EXClearAllAnnotations(VTHEXPORT hExport);

/* Area highlight and comment functions */
DA_ENTRYSC SCCERR DA_ENTRYMOD EXHiliteTextEx(VTHEXPORT hExport, PEXANNOHILITETEXT pHilite, VTLPDWORD pAnchorId);
DA_ENTRYSC SCCERR DA_ENTRYMOD EXRedactText(VTHEXPORT hExport, PEXANNOREDACTTEXT pRedaction);
DA_ENTRYSC SCCERR DA_ENTRYMOD EXHiliteArea(VTHEXPORT hExport, PEXANNOHILITEAREA pHilite, VTLPDWORD pAnchorId);
DA_ENTRYSC SCCERR DA_ENTRYMOD EXAddComment(VTHEXPORT hExport, VTDWORD dwAnchorId, VTLPCWSTR pComment);
DA_ENTRYSC SCCERR DA_ENTRYMOD EXAddHiliteProperty(VTHEXPORT hExport, VTDWORD dwAnchorId, VTLPCWSTR wsName, VTLPCWSTR wsValue);
DA_ENTRYSC SCCERR DA_ENTRYMOD EXApplyHilites(VTHEXPORT hExport, const VTBYTE *pHilites);

/* Image stamp annotation function */
DA_ENTRYSC SCCERR DA_ENTRYMOD EXAddStampAnnotation(VTHEXPORT hExport, PEXANNOSTAMP pStamp, VTLPDWORD pAnchorId);

DA_ENTRYSC SCCERR DA_ENTRYMOD EXGetFilename(VTHEXPORT hExport, EXFILEIOCALLBACKDATA * pData);
DA_ENTRYSC VTLPWORD DA_ENTRYMOD EXGetFileExtension(VTWORD wOutputId );

/* The following calls are not available in HTML Export and Wireless Export */
DA_ENTRYSC SCCERR DA_ENTRYMOD EXRunExportAsync(VTHEXPORT hExport);
DA_ENTRYSC SCCERR DA_ENTRYMOD EXCheckExportStatus(VTHEXPORT hExport);

/* New key/value pair api for use with h5 export */
DA_ENTRYSC SCCERR DA_ENTRYMOD EXAddKeyValueString( VTHEXPORT hExport, VTLPCSTR szKeyName, VTLPCSTR szValue );
DA_ENTRYSC SCCERR DA_ENTRYMOD EXAddKeyValueInt( VTHEXPORT hExport, VTLPCSTR szKeyName, VTDWORD dwValue);
DA_ENTRYSC SCCERR DA_ENTRYMOD EXAddKeyValueFloat( VTHEXPORT hExport, VTLPCSTR szKeyName, VTFLOAT fValue );

/* Export Property API */
DA_ENTRYSC SCCERR DA_ENTRYMOD EXGetProperties(VTHEXPORT hExport, VTDWORD dwID, EXPROPERTIES* pProperty);

#ifdef __cplusplus
}
#endif

#ifdef SCC_PACKED_BY_SCCEX_H
#pragma pack(pop)
#undef SCC_PACKED_BY_SCCEX_H
#undef SCC_PACK_SET
#endif /* SCC_PACKED_BY_SCCEX_H */

#endif /* _SCCEX_H */
