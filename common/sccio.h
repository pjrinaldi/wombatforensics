/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. 
All rights reserved.*/

 /*
  |  SCC Viewer Technology - Include
  |
  |  Include:       SCCIO.H
  |  Environment:   Portable
  |  Function:      Defines for redirectable IO in Viewer Technology
  |
  */

#ifndef SCCIO_H
#define SCCIO_H

#ifdef WINDOWS
#ifndef SCC_PACK_SET
#define SCC_PACK_SET 1
#define SCC_PACKED_BY_SCCIO_H 1
#pragma pack(push,8)
#endif /* SCC_PACK_SET */
#endif /* WINDOWS */

#include <baseio.h>  /* defines the basic IO abstraction (BASEIO struct) */

#ifdef __cplusplus
extern "C"
{
#endif

#include <sccrc4.h>

#ifdef WIN32
#define IO_OSHANDLETYPE VTHANDLE
#endif /*WIN32*/

#ifdef UNIX
#if defined(_DARWIN_SOURCE)
#if defined(__OBJC__)
typedef NSFileHandle   *IO_OSHANDLETYPE;
#else
typedef VTLPVOID          IO_OSHANDLETYPE;
#endif /* __OBJC__ */
#else
#define IO_OSHANDLETYPE int
#endif /* Mac OS X */
#endif   /*UNIX*/

#ifdef NLM
#define IO_OSHANDLETYPE int
#endif   /*NLM*/


  /*
  |  Defines for dwType in IOOPEN function
  */

#define IOTYPE_DOSPATH                1
#define IOTYPE_OEMPATH                IOTYPE_DOSPATH
#define IOTYPE_ANSIPATH               2
#define IOTYPE_MACPATH                3
#define IOTYPE_UNICODEPATH            4
#define IOTYPE_MACFSSPEC              5
#define IOTYPE_MACHFS                 6
#define IOTYPE_TEMP                   7
#define IOTYPE_RANGE                  8
#define IOTYPE_SECONDARY              9
#define IOTYPE_ISTREAM                10
#define IOTYPE_SUBSTREAM              11
#define IOTYPE_SUBSTORAGE             12
#define IOTYPE_REDIRECT               13
#define IOTYPE_ISTORAGE               14
#define IOTYPE_UNIXPATH               15
#define IOTYPE_SUBOBJECT              16
#define IOTYPE_REMOTEFILTERACCESS     17
#define IOTYPE_CHUNKER                18
#define IOTYPE_COMPRESS               19
#define IOTYPE_SECONDARYFULL          20
#define IOTYPE_IOSPEC                 21    /* reserved */
#define IOTYPE_MEMORY                 22
/*** for using an open file handle, See sccda.c ***/
#define IOTYPE_HANDLE                 23
/*** IOTYPE_URL reserved for Transformation Server */
#define IOTYPE_URL                    24
#define IOTYPE_FITEMP                 25
#define IOTYPE_PATH                   26  /* generic file system path (used by transformation server) */
#define IOTYPE_LOCATOR                27  /* IOLOCATOR path */
#define IOTYPE_LINKEDOBJECT           28  /* Linked object (attachment).  */
#define IOTYPE_ARCHIVEOBJECT          29  /* Used in DAOpenDocument to open Archive Nodes*/

/* These IOTYPEs were added to support writable OLE2 using
* Microsoft's interface.
*/
#define IOTYPE_MSTREAM                30
#define IOTYPE_MSTORAGE               31
#define IOTYPE_MROOTSTORAGE           32

/* These IOTYPEs were added to support writable OLE2 using
* our portable interface.
*/
#define IOTYPE_OLE2STREAM             33
#define IOTYPE_OLE2STORAGE            34
#define IOTYPE_OLE2ROOTSTORAGE        35

/* added to support OLE Package embeddings */
#define IOTYPE_OLEPACKAGE             36

/* added for DASaveObject */
#define IOTYPE_OBJECT                 37

/* added for in-memory temp files */
#define IOTYPE_PHYSICALTEMP           38

/* support for zip archive files */
#define IOTYPE_ZIPFILE                39
#define IOTYPE_ZIPNODE                40

/* support for cab files */
#define IOTYPE_CABINET                41
#define IOTYPE_CABFILE                42

/* support for 7z files */
#define IOTYPE_7ZARCHIVE              43
#define IOTYPE_7ZNODE                 44

/* added to support reopening objects for saving */
#define IOTYPE_OPENOBJECT             45  /* Like IOTYPE_SUBOBJECT, but will only open the object */
#define IOTYPE_OPENALTERNATE          46  /* Like IOTYPE_SUBOBJECT, but will only open the alternate graphic */

/* added to support piece files in Open Office XML documents. */
#define IOTYPE_PIECEFILE              47

#ifdef WINDOWS
#define IOTYPE_WIN32VPATH IOTYPE_ANSIPATH
#endif

#ifdef UNIX
#define IOTYPE_WIN32VPATH IOTYPE_UNIXPATH
#endif

#ifdef NLM
#define IOTYPE_WIN32VPATH IOTYPE_DOSPATH
#endif




  /*** Only used by DA to request opening a file requested through DASaveTreeNode */
#define IOTYPE_USERREQUEST        29


  /*
  |  Structures passed to IOOPEN function
  */

typedef struct IOSPECMACHFStag
{
    short       vRefNum;
    VTLONG      dirId;
    VTTCHAR     fileName[256];
} IOSPECMACHFS, * PIOSPECMACHFS;

typedef struct IOSPECRANGEtag
{
    HIOFILE     hRefFile;
#ifdef SCCFEATURE_64BITIO
    VTOFF_T       dwFirstByte;
    VTOFF_T       dwLastByte;
#else
    VTDWORD     dwFirstByte;
    VTDWORD     dwLastByte;
#endif
} IOSPECRANGE, * PIOSPECRANGE;

typedef struct IOSPECCOMPRESStag
{
    HIOFILE     hRefFile;
    VTDWORD     dwFlags;
    VTDWORD     dwType;         /* type of compression */
    VTDWORD     dwReserved1;
    VTDWORD     dwReserved2;
    VTDWORD     dwReserved3;
    VTSYSVAL    pResPointer1;
} IOSPECCOMPRESS, * PIOSPECCOMPRESS;

typedef struct IOSPECSECONDARYtag
{
    HIOFILE     hRefFile;
    VTTCHAR     *pszFileName;
} IOSPECSECONDARY, * PIOSPECSECONDARY;

typedef struct IOSPECSUBSTREAMtag
{
    HIOFILE     hRefStorage;
    VTTCHAR     szStreamName[256];
} IOSPECSUBSTREAM, * PIOSPECSUBSTREAM;

typedef struct IOSPECSUBSTORAGEtag
{
    HIOFILE     hRefStorage;
    VTTCHAR     szStorageName[256];
} IOSPECSUBSTORAGE, * PIOSPECSUBSTORAGE;

typedef struct IOSPECMROOTSTORAGEtag
{
    /* Through NTFS, the maximum filename size is 255 UNICODE
    * characters plus the terminator.
    */
    HIOFILE     hRefFile;
    VTTCHAR     szFileName[512];
    VTDWORD     dwFlags;
}
IOSPECMROOTSTORAGE, * PIOSPECMROOTSTORAGE;

typedef struct IOSPECMSTREAMtag
{
    HIOFILE     hRefStorage;
    /* Maximum stream name size is 31 UNICODE characters
    * plus the terminator.
    */
    VTTCHAR     szStreamName[256];
}
IOSPECMSTREAM, * PIOSPECMSTREAM;

typedef struct IOSPECMSTORAGEtag
{
    HIOFILE     hRefStorage;
    /* Maximum storage name size is 31 UNICODE characters
    * plus the terminator.
    */
    VTTCHAR     szStorageName[256];
}
IOSPECMSTORAGE, * PIOSPECMSTORAGE;

/* Parameters to the IOGetInfo() / IOSetInfo() calls for Microsoft OLE2. */
#define IOSETINFO_OLE2CLSID  1

/* These mirror the STGM defines in objbase.h */
#define SCC_STGM_CREATE             0x00001000L
#define SCC_STGM_DIRECT             0x00000000L
#define SCC_STGM_READWRITE          0x00000002L
#define SCC_STGM_SHARE_EXCLUSIVE    0x00000010L
#define SCC_STGM_SHARE_DENY_WRITE   0x00000020L
#define SCC_STGM_DELETEONRELEASE    0x04000000L

typedef struct IOSPECOLE2ROOTSTORAGEtag
        {
        HIOFILE     hRefFile;
        VTTCHAR     szFileName[512];
        VTDWORD     dwFlags;
        } IOSPECOLE2ROOTSTORAGE, * PIOSPECOLE2ROOTSTORAGE;

/* When opening an OLE2 root storage, we can wrap the provided
* reference file and not re-open it on OLE2 document open and
* not close it on OLE2 document close.  Normal behavior is to
* create/open a new file that the OLE2 document owns and let it
* close it on document close.
*/
#define SCC_WRAP_REFERENCE_FILE     0x00000001

typedef struct IOSPECOLE2STREAMtag
        {
        HIOFILE     hRefStorage;
        VTTCHAR     szStreamName[256];
        } IOSPECOLE2STREAM, * PIOSPECOLE2STREAM;

typedef struct IOSPECOLE2STORAGEtag
        {
        HIOFILE     hRefStorage;
        VTTCHAR     szStorageName[256];
        } IOSPECOLE2STORAGE, * PIOSPECOLE2STORAGE;

typedef struct IOSPECSUBOBJECTtag
{
    VTDWORD     dwStructSize;
    VTSYSPARAM  hDoc;  /* VTHDOC is Defined in SCCDA.H, which is not included here */
    VTDWORD     dwObjectId;
    VTDWORD     dwStreamId;
    VTDWORD     dwReserved1;
    VTDWORD     dwReserved2;
} IOSPECSUBOBJECT, * PIOSPECSUBOBJECT;

/* used to access items in zip archive files,
 * added for MS Office 12 support */
typedef struct IOSPECZIPNODEtag
{
    HIOFILE     hRefFile;   /* zip file (block I/O) */
    VTCHAR      szNodeName[512];
    VTQWORD     qwFileIndex;
    VTBOOL      bLZWOff;    /* TRUE if LZW decompression disabled */
    VTWORD      wRetInfo;
    VTWORD      wFlags;
} IOSPECZIPNODE, * PIOSPECZIPNODE;
#define IOSPEC_ZIPNODE_FILE_INVALID     ((VTQWORD)-1)
#define IOSPEC_ZIPNODEFLAG_RESETPWDATTEMPTNUM 0x0001

typedef struct IOSPECZIPFILEtag
{
    VTWORD wFIID;
    VTVOID *pPasswordData;  /* OIT internal password protected data */
    VTWORD wRetInfo;
} IOSPECZIPFILE, * PIOSPECZIPFILE;
/* possible values for IOSPECZIPFILE.wRetInfo and IOSPECARCNODE.wRetInfo */
#define IOSPEC_ZIPRET_UNSUPPORTEDCOMPRESSION    0x0001
#define IOSPEC_ZIPRET_MULTIDISKFILE             0x0002
#define IOSPEC_ZIPRET_PASSWORDPROTECTED         0x0004
#define IOSPEC_ZIPRET_BADPASSWORD               0x0008

/* optionally used when opening handle as archive file */
typedef struct IOSPECARCHIVEFILEtag
{
    VTWORD      wFIID;          /* FI Id of ref file used for passwd request */
    VTVOID*     pPasswordData;  /* OIT internal password protected data */

    VTWORD      wRetInfo;       /* see values below */
} IOSPECARCHIVEFILE, * PIOSPECARCHIVEFILE;
/* possible values for IOSPECARCHIVEFILE.wRetInfo and IOSPECARCNODE.wRetInfo */
#define IOSPEC_ARCRET_PASSWORDPROTECTED         0x0001
#define IOSPEC_ARCRET_BADPASSWORD               0x0002
#define IOSPEC_ARCRET_UNSUPPORTEDCOMPRESSION    0x0004
#define IOSPEC_ARCRET_MULTIVOLUMEFILE           0x0008

/* used to access items in archive files (only MS cabinet & 7z for now) */
typedef struct IOSPECARCNODEtag
{
    HIOFILE                 hRefFile;   /* archive file (block I/O) */

    VTWORD                  wFlags;         /* see value(s) above */
    VTWORD                  wFileIndex;     /* file index */
    VTWORD                  wszFileName[512];/* null terminated file name */

    VTWORD                  wRetInfo;       /* extended return information */
} IOSPECARCNODE, * PIOSPECARCNODE;
/* possible values for IOSPECARCNODE.dwFlags */
#define IOSPEC_ARCNODEFLAG_INDEX        0x0000
#define IOSPEC_ARCNODEFLAG_NAME         0x0001
#define IOSPEC_ARCNODEFLAG_CHECKREAD    0x0002
#define IOSPEC_ARCNODEFLAG_RESETPWDATTEMPTNUM 0x0004

/* used for archive objects in DA */
typedef struct IOSPECARCHIVEOBJECTtag
{
    VTDWORD dwStructSize;
    VTSYSPARAM hDoc;        /* Parent Doc hDoc. VTHDOC is Defined in SCCDA.H, which is not included here */
    VTDWORD dwNodeId;       /* Node ID */
    VTDWORD dwStreamId;     /* I'm not sure if this is necessary */
    VTDWORD dwReserved1;    /* Must always be 0 */
    VTDWORD dwReserved2;    /* Must always be 0 */
} IOSPECARCHIVEOBJECT, * PIOSPECARCHIVEOBJECT;


/*
    Linked objects.
*/
typedef struct IOSPECLINKEDOBJECTtag
{
    VTDWORD     dwStructSize;
    VTSYSPARAM  hDoc;         /*  VTHDOC is Defined in SCCDA.H, which is not included here */
    VTDWORD     dwObjectId;   /*  Object identifier.  */
    VTDWORD     dwType;       /*  Linked Object type.  (SO_LOCATORTYPE_*)  */
    VTDWORD     dwParam1;     /*  Parameter for DoSpecial call.  */
    VTDWORD     dwParam2;     /*  Parameter for DoSpecial call.  */
    VTDWORD     dwReserved1;  /*  Unused.  */
    VTDWORD     dwReserved2;  /*  Unused.  */
} IOSPECLINKEDOBJECT, * PIOSPECLINKEDOBJECT;

#define IO_FILEOPEN    0x0001
#define IO_FILEISTEMP  0x0002

typedef struct IOLOCATORSEGMENTtag
{
    VTDWORD    dwType;  /* Locator type (SO_LOCATORTYPE_* from sodefs.h) */
    VTSYSPARAM param1;  /* Parameter 1, use depends on locator segment type:
                           OLEPATH, FILENAME, HYPERLINK, BOOKMARK:  Property text ID
                           SECTION:            Section number
                           TAGREFERENCE:       Tag ID
                           DOSPECIAL:          Optional parameter
                           RANGE:              Offset
                           WINDOWSRESOURCE:    Resource type
                           COMPRESSION:        Compression type */
    VTSYSPARAM param2;  /* Parameter 2, use depends on locator segment type:
                           DOSPECIAL:          Optional parameter
                           RANGE:              Length
                           WINDOWSRESOURCE:    Resource ID
                           COMPRESSION:        Compression flags */
} IOLOCATORSEGMENT, * PIOLOCATORSEGMENT;

typedef struct IOLOCATORtag
{
    VTDWORD           dwStructSize;   /* sizeof(IOLOCATOR) */
    VTSYSPARAM        hDoc;           /* VTHDOC is Defined in SCCDA.H, which is not included here */
    VTDWORD           dwSegmentCount; /* Number of segments in this locator */
    IOLOCATORSEGMENT* pSegments;      /* Pointer to an array of locator segments */
    VTDWORD           dwObjectId;     /*  Object identifier.  */
    VTDWORD           dwFlags;
} IOLOCATOR, * PIOLOCATOR;

#define IOLOCATOR_MAXSEGMENTS  256

/* Possible values for IOLOCATOR.dwFlags */
#define IOLOCATOR_OLEOBJECT    0x00000001    /* Locator references an OLE object */


  /*
  |  Structure used for specifying the date and time of a file.
  */
typedef struct IODATETIMEtag
{
    VTDWORD         dwYear;     /* Year A.D. */
    VTDWORD         dwMonth;    /* Month, Jan == 1 */
    VTDWORD         dwDay;      /* Day of month */
    VTDWORD         dwTime;     /* Number of miliseconds since midnight */
}IODATETIME, * PIODATETIME;

typedef struct IORFAREQUESTtag
{
    VTDWORD     dwReserved1;
    VTDWORD     dwReserved2;
    VTDWORD     dwReserved3;
    VTDWORD     dwReserved4;
} IORFAREQUEST, * PIORFAREQUEST;
/* LOWORD of request is one of the following: */
#define RAREQ_CHUNKERDATA    1
#define RAREQ_RAWDATA        2


typedef IOERR (IO_ENTRYMODPTR IOGETREMOTEDATAPROC) (HIOFILE hFile, PIORFAREQUEST pRequest, VTVOID * *ppData, VTDWORD *pdwDataSize);
typedef IOERR (IO_ENTRYMODPTR IOCLOSEREMOTEDATAPROC)(HIOFILE hFile);
typedef IOERR (IO_ENTRYMODPTR IOOPENREMOTESUBOBJECTPROC)(HIOFILE hFile, VTDWORD dwObjectId, VTDWORD dwStreamId, VTDWORD dwReserved1, VTDWORD dwReserved2, VTDWORD * pdwSpecType, VTVOID * *pSpec);

typedef struct IOBASERFAtag         /* Remote Filter Access */
{
    IOGETREMOTEDATAPROC             pGetRemoteData;
    IOCLOSEREMOTEDATAPROC           pCloseRemoteData;
    IOOPENREMOTESUBOBJECTPROC       pOpenRemoteSubObject;
    VTVOID *                  pDummy[5];
} IOBASERFA, * PIOBASERFA;


  /*
  |   Structures for different types
  */

typedef struct IORANGEFILEtag
{
    BASEIO    sBaseIO;        /* Underlying IO system */
    HIOFILE   hFile;          /* Underlying IO system's handle to the file */
    VTDWORD   dwFlags;        /* Info flags */
    VTHANDLE  hThis;          /* Handle to this structure */
#ifdef SCCFEATURE_64BITIO
    VTOFF_T   dwFirstByte;    /* Offset of the begining of the logical file */
    VTOFF_T   dwLastByte;     /* Offset of the end of the logical file */
    VTOFF_T   dwCurPos;       /* Offset in this rangefile wrt dwFirstByte */
    VTOFF_T   dwSavedPos;     /* Pointer position of the reference file when the range was opened */
#else
    VTDWORD   dwFirstByte;    /* Offset of the begining of the logical file */
    VTDWORD   dwLastByte;     /* Offset of the end of the logical file */
    VTDWORD   dwCurPos;       /* Offset in this rangefile wrt dwFirstByte */
    VTDWORD   dwSavedPos;     /* Pointer position of the reference file when the range was opened */
#endif
} IORANGEFILE, * PIORANGEFILE;

typedef struct IOCOMPRESSFILEtag
{
    BASEIO                  sBaseIO;                /* Underlying IO system */
    HIOFILE                 hFile;                  /* Underlying IO system's handle to the file */
    VTDWORD                 dwType;                 /* Type of compression */
    VTDWORD                 dwFlags;                /* Info flags */
    VTHANDLE                hThis;                  /* Handle to this structure */
    VTHANDLE                hBuffer;                /* Handle to a cached buffer of decompressed data */
    VTBYTE *                pBuffer;                /* pointer to cached buffer of decompressed data */
    VTHANDLE                hData;                  /* Handle for data needed for decompression */
    VTBYTE *                pData;                  /* pointer to data needed for decompression */
    HIOFILE                 hTempFile;              /* Temp file to store extra */
#ifdef SCCFEATURE_64BITIO
    VTOFF_T                 dwBufferStartPos;       /* Tell of current buffer start position */
    VTOFF_T                 dwBufferSize;           /* size in bytes of valid buffer */
    VTOFF_T                 dwTempFileSize;         /* Current temp file size */
    VTOFF_T                 dwTempFilePos;          /* Current position in the current temp file of decompressed data */
    VTOFF_T                 dwCurFilePos;           /* Current absolute file position of decompressed data */
#else
    VTDWORD                 dwBufferStartPos;       /* Tell of current buffer start position */
    VTDWORD                 dwBufferSize;           /* size in bytes of valid buffer */
    VTDWORD                 dwTempFileSize;         /* Current temp file size */
    VTDWORD                 dwTempFilePos;          /* Current position in the current temp file of decompressed data */
    VTDWORD                 dwCurFilePos;           /* Current absolute file position of decompressed data */
#endif
    VTBYTE                  LastByte;               /* keep the last byte read for easy bit reading */
    VTBYTE                  nBitsLeft;
    VTHANDLE                hReadBuffer;            /* handle to buffer for input */
    VTBYTE *                pReadBuffer;            /* pointer to buffer for input */
#ifdef SCCFEATURE_64BITIO
    VTOFF_T                 dwReadBufferSize;       /* size of read buffer */
    VTOFF_T                 dwReadBufferPos;        /* position in the read buffer */
    VTOFF_T                 dwReadBufferFilePos;    /* position of start of read buffer in the file */
#else
    VTDWORD                 dwReadBufferSize;       /* size of read buffer */
    VTDWORD                 dwReadBufferPos;        /* position in the read buffer */
    VTDWORD                 dwReadBufferFilePos;    /* position of start of read buffer in the file */
#endif
    VTBOOL                  bReadBufferEOF;         /* is the read buffer at EOF */
    VTDWORD                 dwReserved1;
    VTDWORD                 dwReserved2;
    VTDWORD                 dwReserved3;
    VTSYSVAL                pResPointer1;
} IOCOMPRESSFILE, * PIOCOMPRESSFILE;

#define IO_COMPRESSREADBUFFERSIZE   16384
/* 
|  256KB seems to be the right balance between going to the disk infrequently
|  and not having too much memory to schlep around when we do go to disk
*/
#define IO_COMPRESSBUFFERSIZE       0x00040000

#define IO_COMPRESSTYPE_NONE        0
#define IO_COMPRESSTYPE_ZLIB        1
#define IO_COMPRESSTYPE_BASE64      2
/* LZSS Support added Nov 18, 1999 */
#define IO_COMPRESSTYPE_LZSS        3
/* Nov 2, 2000 PDF Decoding and Decompression methods types */
#define IO_COMPRESSTYPE_ASCII85     4
#define IO_COMPRESSTYPE_ASCIIHEX    5
#define IO_COMPRESSTYPE_RLE         6
#define IO_COMPRESSTYPE_FLATE       7
#define IO_COMPRESSTYPE_LZW         8
#define IO_COMPRESSTYPE_DCT         9
#define IO_COMPRESSTYPE_CCITT       11
#define IO_COMPRESSTYPE_RC4         13

/* PK Implode added Sept 11, 2000, currently supports Binary only */
#define IO_COMPRESSTYPE_PKMPLOD     10
/* MIF Ascii decode support */
#define IO_COMPRESSTYPE_MIFASCIIDECODE 12

/* Same as zlib compression, but without any header information */
#define IO_COMPRESSTYPE_ZLIB_NOHEADER  14

#define IO_COMPRESSTYPE_UUENCODED   15
#define IO_COMPRESSTYPE_XXENCODED   16
#define IO_COMPRESSTYPE_BINHEX      17
#define IO_COMPRESSTYPE_YENC        18
#define IO_COMPRESSTYPE_BTOA        19
#define IO_COMPRESSTYPE_QUOTEDPRINTABLE 20
#define IO_COMPRESSTYPE_UTF7        21
#define IO_COMPRESSTYPE_COMPRESSEDRTF   22

/* JBIG 2 added May 14, 2004 */
#define IO_COMPRESSTYPE_JBIG2       23
/* CAB support for MSZIP decompression */
#define IO_COMPRESSTYPE_MSZIP       24
/* CAB support for LZX decompression */
#define IO_COMPRESSTYPE_LZX         25

/* Advanced Encryption Standard (AES) decryption - 9/29/09 */
#define IO_COMPRESSTYPE_AES128      26
#define IO_COMPRESSTYPE_AES192      27
#define IO_COMPRESSTYPE_AES256      28

/* ZIP Crypto */
#define IO_COMPRESSTYPE_ZIPCRYPTO   29
#define IO_COMPRESSTYPE_JPXDECODE	30

/* ZIP 64 */
#define IO_COMPRESSTYPE_DEFLATE64   31

/* ZIP compression type 2-5 */
#define IO_COMPRESSTYPE_ZIPREDUCE   32

/* ZIP compression type 1 (modified LZW) */
#define IO_COMPRESSTYPE_ZIPSHRINK   33

#define IO_COMPRESSTYPE_LZMA        34
#define IO_COMPRESSTYPE_PPMD        35

/* define unknown compression */
#define IO_COMPRESSTYPE_UNKNOWN     255


/*
|  Structure definitions for memory mapped IO.
*/

#define IO_MEMFILE_INITSIZE 1024

typedef struct IOGENMEMFILEtag
{
    VTDWORD        dwSize;     /* should be set to the size of this structure */
    VTVOID *       pData;
    VTDWORD        dwDataSize; /* size of valid data */
    VTHANDLE       hData;
    VTDWORD        dwMemSize;  /* size of allocated buffer (must be >= dwDataSize) */
} IOGENMEMFILE, * PIOGENMEMFILE;

typedef struct IOMEMORYFILEtag
{
    BASEIO         sBaseIO;
    VTLPBYTE       pDataStart;
    VTLPBYTE       pCurrPos;
    VTDWORD        dwDataSize; /* size of valid data */
    VTHANDLE       hThis;
    VTDWORD        dwFlags;
    VTHANDLE       hData;
    VTDWORD        dwMemSize;  /* size of allocated buffer (must be >= dwDataSize) */
    HIOFILE        hRealFile;  /* if we need a ral temp file, use this */
}  IOMEMORYFILE, * PIOMEMORYFILE;

typedef struct IOPIECEFILEtag
{
    BASEIO         sBaseIO;

    HIOFILE        hTempFile;    
    VTBOOL         bTempfileCreated;
    VTVOID       * pPieces;
    IOSPECZIPNODE  zipnodeSpec;
} IOPIECEFILE, * PIOPIECEFILE;

/*
|  Structure definitions for file access callbacks
*/

typedef struct IOREQUESTDATAtag
{
    VTDWORD   dwSize;          /* sizeof(IOREQUESTDATA)          */
    VTWORD    wFIId;           /* FI id of reference file        */
    VTDWORD   dwSpecType;      /* file spec type                 */
    VTVOID   *pSpec;           /* pointer to a file spec         */
    VTDWORD   dwRootSpecType;  /* root file spec type            */
    VTVOID   *pRootSpec;       /* pointer to the root file spec  */
	 VTDWORD   dwAttemptNumber; /* The number of times the callback has already been called for the currently requested item of information */
} IOREQUESTDATA, * PIOREQUESTDATA;

/*****************************************************************************
 * Start of Zip stuff
 ****************************************************************************/
/* Entry in table that stores sorted file names contained in zip file */
typedef struct tagFZFILEINFO
{
    VTDWORD                 dwNameLoc;  /* offset of filename in Fzu.pNames */
    VTWORD                  wNameLen;   /* length of filename */
    VTWORD                  wGeneralBitFlags;
    VTWORD                  wExtraLen;
    VTWORD                  wCommentLen;    /* length of file comment */
    VTWORD                  wCompressionType;
    VTWORD                  wModDate;   /* modification date */
    VTWORD                  wModTime;   /* modification time */
#ifdef SCCFEATURE_64BITIO
    VTOFF_T                 qwLoc;      /* offset of actual file header */
    VTOFF_T                 qwDataLen;  /* length of compressed data */
    VTOFF_T                 qwUncompressedLen;  /* length of uncompressed data */
    VTOFF_T                 qwExtraLoc; /* offset to extra field */
    VTOFF_T                 qwCommentLoc;   /* offset to file comment */
#else
    VTQWORD                 qwLoc;      /* offset of actual file header */
    VTQWORD                 qwDataLen;  /* length of compressed data */
    VTQWORD                 qwUncompressedLen;  /* length of uncompressed data */
    VTQWORD                 qwExtraLoc; /* offset to extra field */
    VTQWORD                 qwCommentLoc;   /* offset to file comment */
#endif
} FZFILEINFO, * PFZFILEINFO;

/* Support for Zip file I/O system */
typedef struct IOZIPFILEtag
{
    BASEIO                  sBaseIO;    /* Underlying IO system */

    VTHANDLE                hThis;      /* Handle to this structure */
    HIOFILE                 hRefFile;   /* zip file */
    VTBOOL                  bCaseAware; /* filenames are case insensitive */

    VTHANDLE                hFileTable; /* handle to pFileTable array */
    PFZFILEINFO             pFileTable; /* sorted array of files in zip */
    VTQWORD                 qwFileCnt;  /* number of files in zip */
    VTHANDLE                hNames;     /* handle to pNames */
    VTCHAR *                pNames;     /* concatenation of all filenames */
    VTDWORD                 dwNamesMax; /* allocated bytes in pNames */
    VTDWORD                 dwNamesUse; /* bytes used in pNames */
    VTWORD                  wCommentLen;    /* length of file comment */
#ifdef SCCFEATURE_64BITIO
    VTOFF_T                 qwCommentLoc;   /* offset to zip file comment */
#else
    VTQWORD                 qwCommentLoc;   /* offset to zip file comment */
#endif
    VTWORD                  wszPassword[256];
    VTHANDLE                hFilterData;    /* filter data for acquiring passwords */
    VTDWORD                 dwPasswordAttemptNum;
    VTDWORD                 crcTable[256];
    VTWORD                  wFIID;          /* FI Id of ref file used for passwd request */
} IOZIPFILE,  * PIOZIPFILE;

/* I/O struct for opened nodes within a zip file */
typedef struct IOZIPNODEtag
{
    BASEIO                  sBaseIO;    /* Underlying IO system */

    VTHANDLE                hThis;      /* Handle to this structure */
    HIOFILE                 hRefFile;   /* either .hRange or .hCompress */

    VTQWORD                 qwIndex;    /* index into IOZIPFILE.pFileTable */
    HIOFILE                 hRange;     /* handle to range file */
    HIOFILE                 hCompress;  /* handle to compressed file */
    HIOFILE                 hDecrypt;   /* handle to decrypted compressed file */

    VTBYTE                  Buffer[512];/* space for buffered I/O */
#ifdef SCCFEATURE_64BITIO
    VTOFF_T                 dwBufLoc;   /* file location of start of buffer */
#else
    VTDWORD                 dwBufLoc;   /* file location of start of buffer */
#endif
    VTDWORD                 dwBufCnt;   /* amt of valid data in buffer */
#ifdef SCCFEATURE_64BITIO
    VTOFF_T                 lBufPos;    /* buffer offset wrt dwBufLoc */
    VTOFF_T                 dwVerify;   /* for verifying buffered zip IO */
#else
    VTLONG                  lBufPos;    /* buffer offset wrt dwBufLoc */
    VTDWORD                 dwVerify;   /* for verifying buffered zip IO */
#endif
} IOZIPNODE,  * PIOZIPNODE;

/* Structure returned by IOGetInfo(IOGETINFO_ZIPRANGE) */
typedef struct IOZIPRANGEtag
{
#ifdef SCCFEATURE_64BITIO
    VTOFF_T   dwFirstByte;
    VTOFF_T   dwLastByte;
#else
    VTDWORD   dwFirstByte;
    VTDWORD   dwLastByte;
#endif

    VTDWORD   dwCompressType;             /* IO_COMPRESSTYPE_xxxx */
} IOZIPRANGE, * PIOZIPRANGE;
/*****************************************************************************
 * End of Zip stuff
 ****************************************************************************/

/*****************************************************************************
 * Start of CAB stuff
 ****************************************************************************/
/* structure to provide access to CFDATA block structure */
typedef struct CABDATAtag
{
    HIOFILE     hRefFile;           /* range I/O file */

    VTWORD      wCompressedSize;    /* # of compressed bytes in block */
    VTWORD      wUncompressedSize;  /* # of uncompressed bytes in block */
} CABDATA, * PCABDATA;

/* structure to provide access to CFFILE info */
typedef struct CABFILEINFOtag
{
    VTDWORD         dwNameLoc;  /* offset into CABFILE.pNames */
    VTDWORD         dwNameLen;

    VTDWORD         dwFileSize; /* uncompressed size of this file in bytes */
    VTDWORD         dwFileLoc;  /* uncompressed offset of file in the folder */
    VTWORD          wFolderIndex;/* index into the CFFOLDER area */
    VTWORD          wDate;      /* date stamp for this file */
    VTWORD          wTime;      /* time stamp for this file */
} CABFILEINFO, * PCABFILEINFO;

typedef struct IOCABFILEtag
{
    BASEIO                  sBaseIO;    /* Underlying IO system */

    VTHANDLE                hThis;      /* Handle to this structure */
    HIOFILE                 hCabinet;   /* handle to parent IOCABFILE */

    HIOFILE                 hRange;     /* range within folder */
    VTDWORD                   dwOpenFlags;/* possible values: IOOPEN_* */

    /* IOCABINET.pFileInfo array index to obtain the cabfile info */
    VTWORD                  wFileIdx;
} IOCABFILE, * PIOCABFILE;

typedef struct IOCABFOLDERtag
{
    BASEIO      sBaseIO;    /* Underlying IO system */

    HIOFILE     hCabinet;   /* handle to parent IOCABFILE */

    VTHANDLE    hData;      /* handle to pData array */
    PCABDATA    pData;      /* array of cab data blocks */

    VTWORD      wDataIdx;   /* index into folder's pDataArray */
    VTDWORD     dwDataLoc;  /* offset within current data block */

    VTDWORD     dwDataOffset;   /* offset for 1st CFDATA block in folder */
    VTWORD      wDataCnt;       /* # of CFDATA blocks in this folder */
    VTWORD      wCompressType;  /* compression type for CFDATAs in folder */
} IOCABFOLDER, * PIOCABFOLDER;

/* struct facilitating access of continguous uncompressed cab folder data */
typedef struct CABFOLDERINFOtag
{
    HIOFILE     hRefFile;   /* handle to either &Folder or hCompress */

    IOCABFOLDER Folder;     /* underlying folder I/O handle */
    HIOFILE     hCompress;  /* valid when folder is compressed */
} CABFOLDERINFO, * PCABFOLDERINFO;

typedef struct IOCABINETtag
{
    BASEIO          sBaseIO;        /* Underlying IO system */

    VTHANDLE        hThis;          /* Handle to this structure */
    HIOFILE         hRefFile;       /* cab file */

    VTWORD          wFolderCnt;     /* count of folders */
    VTWORD          wFileCnt;       /* count of files */

    VTWORD          wHdrReserve;    /* bytes in header reserve field */
    VTBYTE          ubFldrReserve;  /* bytes in folder reserve field */
    VTBYTE          ubDataReserve;  /* bytes in data reserve field */

    VTDWORD         dwFileLoc;      /* offset of first file location */

    VTHANDLE        hFldrInfo;      /* handle to pFldrInfo array */
    PCABFOLDERINFO  pFldrInfo;      /* array of all CABFOLDERINFOs in cab */

    VTHANDLE        hFileInfo;      /* handle to pFileInfo array */
    PCABFILEINFO    pFileInfo;      /* array of file info structs */

    VTHANDLE        hNames;         /* handle to pNames */
    VTWCHAR*        pNames;         /* concatenation of all filenames */
    VTDWORD         dwNamesMax;     /* allocated bytes in pNames */
    VTDWORD         dwNamesUse;     /* bytes used in pNames */

} IOCABINET, * PIOCABINET;
/*****************************************************************************
 * End of CAB stuff
 ****************************************************************************/

/*****************************************************************************
 * Start of 7z stuff
 ****************************************************************************/
typedef struct IO7ZNODEtag
{
    BASEIO                  sBaseIO;    /* Underlying IO system */

    VTHANDLE                hThis;      /* Handle to this structure */
    HIOFILE                 h7zArchive; /* handle to parent IO7ZARCHIVE */

    VTWORD                  wIdx;       /* index of node in 7z db */

    HIOFILE                 hFile;      /* data is uncompressed into here */
#ifdef SCCFEATURE_64BITIO
    VTOFF_T                 dwFileLen;  /* amount of data stored in hFile */
    VTOFF_T                 dwOffset;   /* next read location within file */
#else
    VTDWORD                 dwFileLen;  /* amount of data stored in hFile */
    VTDWORD                 dwOffset;   /* next read location within file */
#endif
} IO7ZNODE, * PIO7ZNODE;

typedef struct IO7ZARCHIVEtag
{
    BASEIO      sBaseIO;        /* Underlying IO system */

    VTHANDLE    hThis;          /* Handle to this structure */
    HIOFILE     hRefFile;       /* top level 7z file handle */

    VTDWORD     dwFlags;        /* wszPassword contains valid value? */

    VTHANDLE    hDb;            /* 7z library's db structure */
    VTHANDLE    hSeekInStream;  /* file reading facility */
    VTHANDLE    hLookToRead;    /* file reading helper facility */
    VTHANDLE    hAlloc;         /* memory allocation facility */
    VTHANDLE    hFilterData;    /* filter data for acquiring passwords */

    VTDWORD     dwBlockIndex;   /* last block index we read from */

    VTWORD      wFIID;          /* FI Id of ref file used for passwd request */
    VTWORD      wszPassword[256];

    VTBYTE*     pBuf;           /* data buffer for reading content in node */
#ifdef SCCFEATURE_64BITIO
    VTOFF_T     dwBufSize;      /* allocated size of buffer */
    VTOFF_T     dwBufLoc;       /* offset within current node */
    VTOFF_T     dwBufCnt;       /* # bytes stored in buffer */
#else
    VTDWORD     dwBufSize;      /* allocated size of buffer */
    VTDWORD     dwBufLoc;       /* offset within current node */
    VTDWORD     dwBufCnt;       /* # bytes stored in buffer */
#endif
    VTDWORD     dwPasswordAttempNum;
} IO7ZARCHIVE, * PIO7ZARCHIVE;
/* possible values for IO7ZARCHIVE.dwFlags */
#define IO7Z_ARCFLAG_PASSWORDPROTECTED  0x00000001
#define IO7Z_ARCFLAG_PASSWORDVALID      0x00000002  /* wszPassword is valid */
/*****************************************************************************
 * End of 7z stuff
 ****************************************************************************/

/*
|       Node of a Shannon-Fano tree
|       0 is the root.
|       A zero in left field
|       means the right field is the value.
*/
typedef struct SFNODEtag
{
    VTLONG  left;
    VTLONG  right;
} SFNODE, * PSFNODE;

#define HPSFNODE PSFNODE

/*
|       Structure to store input for creating the Shannon-Fano tree.
*/
typedef struct CODEINFOtag
{
    VTLONG  lValue;
    VTSHORT nLength;
} CODEINFO, * PCODEINFO;

typedef struct IOZLIBDATAtag
{
    VTHANDLE                hLitLenTree;
    PSFNODE                 pLitLenTree;
    VTHANDLE                hDistanceTree;
    PSFNODE                 pDistanceTree;
    VTHANDLE                hCodeLenTree;
    PSFNODE                 pCodeLenTree;
    VTHANDLE                hTree;
    PSFNODE                 pTree;
    VTBOOL                  bFinal;                 /* is this the final block? */
    VTBOOL                  bFinished;              /* no more compression. eof */
    VTBYTE                  bType;                  /* what type of compression: 0=none, 1=fixed codes, 2=dynamic codes */
    VTWORD                  wLength;                /* bytes left when bType==0 */
} IOZLIBDATA, * PIOZLIBDATA;


/*********************************************************************/
/*********************************************************************/
/*************************                   *************************/
/*************************        LZSS       *************************/
/*************************                   *************************/
/*********************************************************************/
/*********************************************************************/
#define IO_CIRCBUF_SIZE             0x1000

/* LZSS Support added Nov 18, 1999 */
typedef struct IOLZSSDATAtag
{
    VTBYTE          bCurFlags;  /* the current lzss flags*/
    VTBYTE          bFlagCount; /* which flag bit to look at next */
    VTHANDLE        hCircBuf;
    VTBYTE *        pCircBuf;
    VTWORD          wCircBufPos;

} IOLZSSDATA, * PIOLZSSDATA;

/*********************************************************************/
/*********************************************************************/
/*************************                   *************************/
/*************************      PKMPLOD      *************************/
/*************************                   *************************/
/*********************************************************************/
/*********************************************************************/

/* PKMPLOD Support added Sept 11, 2000 */
/* these values correspond to the format, do not change */
#define BINARYCOMPRESSION 0
#define ASCIICOMPRESSION  1
#define ONEKDICTIONARY    4
#define TWOKDICTIONARY    5
#define FOURKDICTIONARY   6
#define EIGHTKDICTIONARY  7
#define LITERALDATA       0
#define CODEDATA          1

typedef struct IOPKMPLODDATAtag
{
    VTBYTE          bCompressionType;   /* Binary or ASCII, currently only support Binary */
    VTBYTE          bDictionarySize;    /* 1k, 2k, or 4k */
    VTBYTE          bDataType;          /* the current data type to read: literal or code */
    VTWORD          wMinMatchLen;
    VTHANDLE        hLitTree;
    PSFNODE         pLitTree;
    VTHANDLE        hLenTree;
    PSFNODE         pLenTree;
    VTHANDLE        hDistanceTree;
    PSFNODE         pDistanceTree;
} IOPKMPLODDATA, * PIOPKMPLODDATA;


/*********************************************************************/
/*********************************************************************/
/*************************                   *************************/
/*************************      ZIPREDUCE    *************************/
/*************************                   *************************/
/*********************************************************************/
/*********************************************************************/


typedef struct IOZIPREDUCEDATAtag
{
    VTBYTE Slen[256];
    VTBYTE followers[256][64];
    VTWORD factor;
    VTWORD Len;
    VTWORD V;
    VTWORD wLeftToCopy;
#ifdef SCCFEATURE_64BITIO
    VTOFF_T op;
#else
    VTLONG op;
#endif
    VTBYTE bToCopy;
    VTBYTE state;
    VTBYTE lchar;
} IOZIPREDUCEDATA, * PIOZIPREDUCEDATA;



/*********************************************************************/
/*********************************************************************/
/*****************                                ********************/
/***************** PDF DECODING AND DECOMPRESSION ********************/
/*****************                                ********************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/

/* ASCII85 decoding support added Nov 2, 2000*/
/*********************************************************************/
/*********************************************************************/
/*************************                   *************************/
/*************************      ASCII85      *************************/
/*************************                   *************************/
/*********************************************************************/
/*********************************************************************/
#define ASCII85_CHARDATA_SIZE       5   /* buffer size for character data   */
#define ASCII85_BINARYDATA_SIZE     4   /* buffer size for binary data      */

typedef struct IOASCII85DATAtag
{
    VTBYTE byCharData[ASCII85_CHARDATA_SIZE];
    VTBYTE byBinaryData[ASCII85_BINARYDATA_SIZE];
    VTBOOL bEOD;
    VTBYTE byNumber;
    VTBYTE byIndex;
} IOASCII85DATA, * PIOASCII85DATA;

typedef struct IOYENCDATAtag
{
    VTBYTE  Line[256];
    VTWORD  wCurrentPosition;
    VTWORD  wUncompressedLength;
    VTBOOL  bWithinPart;
} IOYENCDATA, * PIOYENCDATA;

typedef struct IOBTOADATAtag
{
    VTBYTE  Line[32];
    VTWORD  wCurrentPosition;
    VTWORD  wUncompressedLength;
    VTDWORD dwValue;
    VTWORD  wCount;
} IOBTOADATA, * PIOBTOADATA;

typedef struct IOQPDATAtag
{
    VTBYTE  Line[128];
    VTWORD  wCurrentPosition;
    VTWORD  wUncompressedLength;
} IOQPDATA, * PIOQPDATA;

typedef struct IOBinHexDATAtag
{
    VTDWORD dwReadLength;
    VTDWORD dwDataLength;
    VTBYTE  Line[8];
    VTWORD  wCurrentPosition;
    VTWORD  wUncompressedLength;
    VTSHORT cchLastCharacter;
    VTWORD  wRepeatCount;
} IOBINHEXDATA, * PIOBINHEXDATA;

typedef struct IOCOMPRESSEDRTFDATAtag
{
    VTHANDLE hLine;
    VTLPBYTE Line;
    VTBYTE  bFlagByte;
    VTWORD  wBitsLeft;
    VTWORD  wCurrentPosition;
    VTWORD  wPositionsBuffered;
    VTWORD  wNextWritePosition;
    VTDWORD dwCompressedSize;
    VTDWORD dwUnCompressedSize;
    VTDWORD dwSignature;
    VTDWORD dwCrc;
} IOCOMPRESSEDRTFDATA, * PIOCOMPRESSEDRTFDATA;

typedef struct IOXXEDATAtag
{
    VTBYTE  Line[128];
    VTWORD  wCurrentPosition;
    VTWORD  wUncompressedLength;
} IOXXEDATA, * PIOXXEDATA;

typedef struct IOUUEDATAtag
{
    VTBYTE  Line[128];
    VTWORD  wCurrentPosition;
    VTWORD  wUncompressedLength;
} IOUUEDATA, * PIOUUEDATA;

typedef struct IOBASE64DATAtag
{
    VTBYTE  Line[132];
    VTWORD  wCurrentPosition;
    VTWORD  wUncompressedLength;
} IOBASE64DATA, * PIOBASE64DATA;

typedef struct IOUTF7DATAtag
{
    VTWORD  UnCompressedLine[128];
    VTBYTE  Line[128];
    VTWORD  wCurrentPosition;
    VTWORD  wUncompressedLength;
} IOUTF7DATA, * PIOUTF7DATA;

/* ASCIIHex decoding support added Nov 6, 2000*/
/*********************************************************************/
/*********************************************************************/
/*************************                   *************************/
/*************************     ASCIIHEX      *************************/
/*************************                   *************************/
/*********************************************************************/
/*********************************************************************/
typedef struct IOASCIIHEXDATAtag
{
    VTBOOL bEOD;    /* used when the least significant part is padded with '0' */
} IOASCIIHEXDATA, * PIOASCIIHEXDATA;


/*********************************************************************/
/*********************************************************************/
/*************************                   *************************/
/*************************        LZW        *************************/
/*************************                   *************************/
/*********************************************************************/
/*********************************************************************/

#define MAX_CODE_SIZE       12                      /* max number of bits that can be used. */
#define SHRINK_MAX_CODE_SIZE    13
#define MIN_CODE_SIZE       8                       /* min number of bits that can be used. */
#define DICT_SIZE           (1 << MAX_CODE_SIZE)    /* size of the dictionary table (for 12 bits = 4096). */
#define SHRINK_DICT_SIZE    (1 << SHRINK_MAX_CODE_SIZE)
#define CLEAR_CODE          (1 << MIN_CODE_SIZE)    /* 256 */
#define END_OF_DATA_CODE    (CLEAR_CODE + 1)        /* 257 */
#define FIRST_CODE          (CLEAR_CODE + 2)        /* 258 */
#define SHRINK_FIRST_CODE   (CLEAR_CODE + 1)
#define STOP_CODE           0xFFFF                  /* this code determines the stop condition when traversing */
                                                    /* multiway tree.  (initially was 0, but it turned out, that it was */
                                                    /* failing when parent contains 0 which should be retrieved. */


/* This data structure defines the dictionary.  Each entry in the dictionary has a code value.
 * which is emitted by the compressor.  Each node is actually made up of two pieces:
 *          - parent_code
            - character.
 * Code values of less than 256 are actually plain text codes.
 */
typedef struct DICTTTREEtag
{
    VTWORD  wParentCode;
    VTBYTE  byCharacter;
} LZWDICTTREE, * PLZWDICTTREE;

typedef struct IOLZWDATAtag
{
    VTHANDLE        hDictTreeArray;
    PLZWDICTTREE    pDictTreeArray;
    VTWORD          wDictNextCode;          /* next code to be added to the dictionary.                                         */
    VTHANDLE        hDecodeStack;           /* for reversing strings that come out of the tree.                                 */
    VTBYTE *        pDecodeStack;
    VTWORD          wStackPointer;
    VTBYTE          byCurrentCodeSize;      /* contains number of bits, which are currently read from the file.                 */
    VTWORD          wNextBumpCode;          /* defines the code that will trigger the next jump in bits size. 511, 1023, 2047)  */

    VTWORD          wPrevCode;              /* previous code rad from the input file.                                           */
    VTWORD          wCurrentCode;           /* current code read from the input file.                                           */
    VTBYTE          byFirstChar;            /* first character of the prevous code, used to construct dictionary entry.         */
    VTBOOL          bEOD;                   /* set true when END_OF_DATA_CODE is reached.                                       */

    VTDWORD         dwBitsBuf;              /* contains bits used for reading specified number of bits.                         */
    VTDWORD         dwBitsLeft;             /* number of bits in dwBitsBuf (used for transforming big-endian to little-endian). */

    VTHANDLE        hPredictorData;         /* handle to the predictor sturcture.                                               */
    VTBYTE *        pPredictorData;         /* pointer to predictor sturcture.                                                  */
    VTBOOL bShrink;
    VTBOOL bShrinkFirst;
    VTDWORD dwDictSize;
} IOLZWDATA, * PIOLZWDATA;


#define IO_COMPRESSTYPE_NONE    0
#define IO_COMPRESSTYPE_ZLIB    1
#define IO_COMPRESSTYPE_BASE64  2

/*********************************************************************/
/*********************************************************************/
/*************************                   *************************/
/*************************     PREDICTOR     *************************/
/*************************                   *************************/
/*********************************************************************/
/*********************************************************************/

/* Predictor types: */
#define IO_PREDICTION_NONE          0
#define IO_PREDICTION_DEFAULT       1       /* also no prediction   */
#define IO_PREDICTION_TIFF          2
#define IO_PREDICTION_PNG_NONE      10      /* no prediction        */
#define IO_PREDICTION_PNG_SUB       11
#define IO_PREDICTION_PNG_UP        12
#define IO_PREDICTION_PNG_AVERAGE   13
#define IO_PREDICTION_PNG_PEATH     14
#define IO_PREDICTION_PNG_OPTIMUM   15

/* Masks for extracting data from the reserved words. */
#define PREDICTOR_MASK_COLORS       0xFF
#define PREDICTOR_MASK_BITS         0xFF00
#define PREDICTOR_MASK_EARLY        0xFF0000

/* When predictors are used we need to keep data in sliding window.  To get the size of
 * the slididng window CINFO flag from ZLIB header is read.  Its max value can be CINFO=7,
 * which gives sliding window size of 32k (2pow(CINFO+8).  Everything less than CINFO=7
 * will give smaller sliding widnow smaller and the value will be get by right shift.
 */
#define SLIDING_WINDOW_MAX          (1<<15)     /* 32k */

/*
 | For transfering extra parmeters needed for prediction from IOCOMPRESSDATA, following is used:
 |      dwReserved1 - prediction values (1 - no prediction, 2 - TIFF prediction, 10 - 15 PNG prediction)
 |                (for more info see PDF Reference Pg. 48, and PNG specification - filter algorithms)
 |      dwReserved2 - number of colmuns (pixels per line)
 |      dwReserved3 - number of color components (last siginivicant byte 0xFF
 |      dwReserved3 - bits per pixel (second byte 0xFF00)
 |      dwReserved3 - early change (third byte 0xFF0000)
 | They are either repeated or extracted in this structure.
 */
typedef struct IOPREDICTORDATAtag
{
    VTBYTE          byPredictor;        /* dwReserved1 - predictor type */
    VTDWORD         dwWidth;            /* dwReserved2 - columns (pixels per line). */
    VTBYTE          byColorComponents;  /* dwReserved3 - number of color components (0xFF) */
    VTBYTE          byBitsPerComponent; /* dwReserved3 - lower byte of dwReserved3  0xFF00 */
    VTBYTE          byEarlyChange;      /* dwReserved3 - higher byte of dwReserved3  0xFF0000 */
    VTDWORD         dwVals;             /* dwWidth*byColorComponents. - components per line */
    VTDWORD         dwPixBytes;         /* bytes per pixel. */
    VTDWORD         dwRowBytes;         /* bytes per line. */
    VTHANDLE        hPredictorLine;     /* handle to predictor line buffer */
    VTBYTE *        pPredictorLine;     /* pointer to predictor line buffer. */
    VTDWORD         dwPredictorIndex;   /* current index in predictor line. */
    VTHANDLE        hSlidingWindow;     /* handle to sliding window buffer */
    VTBYTE *        pSlidingWindow;     /* pointer to sliding widnow buffer */
    VTWORD          wSlidingIndex;      /* current index in sliding window. */
    VTWORD          wSlidingMask;       /* contains mask for wrapping of index in Sliding window (as circular buffer). */
} IOPREDICTORDATA, * PIOPREDICTORDATA;


/*********************************************************************/
/*********************************************************************/
/*************************                   *************************/
/*************************        DCT        *************************/
/*************************                   *************************/
/*********************************************************************/
/*********************************************************************/

/*
 | For transfering extra parmeters needed for DCT from IOCOMPRESSDATA, following is used:
 |      dwReserved1 - whether ColorTransform parameter is defined in the dictionary
 |                                           (0 - not defined in the dictionary,
 |                                            1 - defined in the dictionary with value stored in
 |                                                dwReserved2)
 |      dwReserved2 - ColorTransform integer (0 - no color transformation,
 |                                            1 - apply color transformation)
 |
 | ColorTransform integer is a code specifying the transformation to be performed on the
 | sample values:
 |       0 No transformation.
 |       1 If the image has three color components, transform RGB values to YUV before
 |         encoding and from YUV to RGB after decoding. If the image has four components,
 |         transform CMYK values to YUVK before encoding and from YUVK to CMYK after
 |         decoding. This option is ignored if the image has one or two color components.
 | The default value of ColorTransform is 1 if the image has three components and 0 otherwise.
 | In other words, conversion between RGB and YUV is performed for all three-component images
 | unless explicitly disabled by setting ColorTransform to 0. Additionally, the encoding
 | algorithm inserts an Adobedefined marker code in the encoded data indicating the ColorTransform
 | value used. If present, this marker code overrides the ColorTransform value given to DCTDecode.
 | Thus it is necessary to specify ColorTransform only when decoding data that does not contain
 | the Adobe-defined marker code.
 */


/*
 * JPEG markers consist of one or more 0xFF bytes, followed by a marker
 * code byte (which is not an FF).  Here are the marker codes of interest
 * in this program.  (See jdmarker.c for a more complete list.)
 */

/* Note: codes C4 and CC are NOT SOF markers */

typedef enum DCT_MARKERtag
{                           /* NON-DIFFERENTIAL HUFFMAN       */
      SOF0  = 0xc0,         /* Baseline DCT                   */
      SOF1  = 0xc1,         /* Extended sequential DCT        */
      SOF2  = 0xc2,         /* Progressive DCT                */
      SOF3  = 0xc3,         /* Spatial DPCM                   */

                            /* DIFFERENTIAL HUFFMAN           */
      SOF5  = 0xc5,         /* Differential sequential DCT    */
      SOF6  = 0xc6,         /* Differential progressive DCT   */
      SOF7  = 0xc7,         /* Differential spatial           */

                            /* NON-DIFFERENTIAL ARITHMETIC    */
      JPG   = 0xc8,         /* Reserved for JPEG extensions   */
      SOF9  = 0xc9,         /* Extended sequential DCT        */
      SOF10 = 0xca,         /* Progressive DCT                */
      SOF11 = 0xcb,         /* Spatial DPCM                   */

                            /* DIFFERENTIAL ARITHMETIC        */
      SOF13 = 0xcd,         /* Differential sequential DCT    */
      SOF14 = 0xce,         /* Differential progressive DCT   */
      SOF15 = 0xcf,         /* Differential spatial           */

      DHT   = 0xc4,         /* Define Huffman table           */
      DAC   = 0xcc,         /* Define Arithmetic conditioning */

      RST0  = 0xd0,         /* Restart with modulo 8 count 0  */
      RST1  = 0xd1,         /* Restart with modulo 8 count 1  */
      RST2  = 0xd2,         /* Restart with modulo 8 count 2  */
      RST3  = 0xd3,         /* Restart with modulo 8 count 3  */
      RST4  = 0xd4,         /* Restart with modulo 8 count 4  */
      RST5  = 0xd5,         /* Restart with modulo 8 count 5  */
      RST6  = 0xd6,         /* Restart with modulo 8 count 6  */
      RST7  = 0xd7,         /* Restart with modulo 8 count 7  */

      SOI   = 0xd8,         /* Start of image                 */
      EOI   = 0xd9,         /* End of image                   */
      SOS   = 0xda,         /* Start of scan                  */
      DQT   = 0xdb,         /* Define quantization table(s)   */
      DNL   = 0xdc,         /* Define number of lines         */
      DRI   = 0xdd,         /* Define restart interval        */
      DHP   = 0xde,         /* Define hierarchical progression*/
      EXP   = 0xdf,         /* Expand reference image(s)      */

      /* RESERVED APPLICATION SEGMENTS 0xFFE0-0xFFEF */
      APP0  = 0xe0,         /* Application Data               */
      APP1  = 0xe1,
      APP2  = 0xe2,
      APP3  = 0xe3,
      APP4  = 0xe4,
      APP5  = 0xe5,
      APP6  = 0xe6,
      APP7  = 0xe7,
      APP8  = 0xe8,
      APP9  = 0xe9,
      APP10 = 0xea,
      APP11 = 0xeb,
      APP12 = 0xec,
      APP13 = 0xed,
      APP14 = 0xee,         /* Adobe Application-Specific JPEG Marker */
      APP15 = 0xef,

      /* RESERVED JPEG EXTENSIONS      0xFFF0-0xFFFD          */
      JPG0  = 0xf0,
      JPG13 = 0xfd,

      COM   = 0xfe,         /* Comment                        */

      TEM   = 0x01,         /* Temp. private use arithmetic   */

      MARKER_ERROR = 0x100

      /* RESERVED 0xFF02-0xFFBF    */
} DCT_MARKER;

#define DCTSIZE             8   /* The basic DCT block is 8x8 samples           */
#define DCT_BLOCK_SIZE      64  /* DCTSIZE squared; # of elements in a block    */
#define NUM_QUANT_TBLS      4   /* Quantization tables are numbered 0..3        */
#define NUM_HUFF_TBLS       4   /* Huffman tables are numbered 0..3             */
#define MAX_COMPS_IN_SCAN   4   /* JPEG limit on # of components in one scan    */
#define MAX_SAMP_FACTOR     4   /* JPEG limit on sampling factors               */
#define DCT_CLIP_SIZE       768
#define DCT_CLIP_OFFSET     256
#define MAX_NUM_HUFF_CODES  256 /* Table B.5 */


#define dctCos1    0.98078528   /* cos(pi/16) */
#define dctSin1    0.19509032   /* sin(pi/16) */
#define dctCos3    0.83146961   /* cos(3*pi/16) */
#define dctSin3    0.55557023   /* sin(3*pi/16) */
#define dctCos6    0.38268343   /* cos(6*pi/16) */
#define dctSin6    0.92387953   /* sin(6*pi/16) */
#define dctSqrt2   1.41421356   /* sqrt(2) */
#define dctSqrt1d2 0.70710678   /* sqrt(2) / 2 */
/*#endif*/

/* color conversion parameters (16.16 fixed point format) */
#define dctCrToR   91881        /*  1.4020      */
#define dctCbToG  -22553        /* -0.3441363   */
#define dctCrToG  -46802        /* -0.71413636  */
#define dctCbToB  116130        /*  1.772       */



/* DCT component info */
typedef struct DCTCOMP_INFOtag
{
    VTBYTE  byComponentID;        /* component identifier (0-255) (for PDF max 4)                     */
    VTBOOL  bInScan;              /* is this component in the current scan?                           */
    VTBYTE  byHSampFactor;        /* horizontal sampling factor (1-4)                                 */
    VTBYTE  byVSampFactor;        /* vertical sampling factor (1-4)                                   */
    VTBYTE  byQuantTableNo;       /* quantization table selector (0..3) (max 4 tables can be defined) */
    VTBYTE  byDCHuffTableNo;      /* DC entropy table (Huffman) selector (0..3)                       */
    VTBYTE  byACHuffTableNo;      /* AC entropy table (Huffman) selector (0..3)                       */
    VTSHORT nPrevDC;              /* DC coefficient accumulator                                       */
} DCTCOMP_INFO;

/* DCT Huffman decoding table */
typedef struct DCTHUFF_TBLtag
{
    VTWORD  firstSym[17];         /* first symbol for this bit length     */
    VTDWORD firstCode[17];        /* first code for this bit length       */
    VTBYTE  numCodes[17];         /* number of codes of this bit length   */
    VTBYTE  sym[256];             /* symbols                              */
} DCTHUFF_TBL;

typedef struct DCTQUANT_TBLtag
{
    VTHANDLE hQuantElems;
    VTLPWORD pQuantElems;
} DCTQUANT_TBL;


typedef struct IODCTDATAtag
{
    VTWORD    wImageWidth;            /* image size (X)                                   */
    VTWORD    wImageHeight;           /* image size (Y)                                   */
    VTBYTE    byMCUWidth;             /* size of min coding unit, in data units           */
    VTBYTE    byMCUHeight;            /* size of min coding unit, in data units           */
    VTHANDLE  hCompInfo[MAX_COMPS_IN_SCAN];
    DCTCOMP_INFO* pCompInfo[MAX_COMPS_IN_SCAN];   /* info for each component              */
    VTBYTE    byNumComps;             /* number of image components (Nf) (1-255).         */
    VTBYTE    byColorTransform;       /* need YCbCr-to-RGB transform?                     */
    VTBOOL    bGotAdobeMarker;        /* set if APP14 Adobe marker was present            */
    VTWORD    wRestartInterval;       /* restart interval, as number of MCUs (0-65535)    */
    DCTQUANT_TBL QuantTbls[NUM_QUANT_TBLS];   /* quantization tables                      */
    VTBYTE    byNumQuantTbls;         /* number of quantization tables                    */
    VTHANDLE  hDCHuffTbls[NUM_HUFF_TBLS];      /* DC Huffman tables                       */
    DCTHUFF_TBL* pDCHuffTbls[NUM_HUFF_TBLS];
    VTBYTE    byNumDCHuffTbls;        /* number of DC Huffman tables                      */
    VTHANDLE  hACHuffTbls[NUM_HUFF_TBLS];      /* AC Huffman tables                       */
    DCTHUFF_TBL* pACHuffTbls[NUM_HUFF_TBLS];
    VTBYTE    byNumACHuffTbls;        /* number of AC Huffman tables (Th) (0-3)           */
    VTHANDLE  hRowBuf;                /* handle to buffer for one MCU                     */
    VTLPBYTE  pRowBuf;                /* pointer to buffer for one MCU                    */
    VTDWORD   dwRowBufWidth;
    /*VTLPBYTE    rowBuf[4][32];*/    /* buffer for one MCU                               */
    VTBYTE    byCurrentComp;          /* curent component                                 */
    VTWORD    wCurrentX;              /* current X position within image                  */
    VTWORD    wCurrentY;              /* current Y position within image                  */
    VTBYTE    byCurrentdY;            /* current position within MCU.                     */
    VTSHORT   wRestartCtr;            /* MCUs left until restart                          */
    DCT_MARKER eRrestartMarker;       /* next restart marker                              */
    VTBYTE    byInBuf;                /* input buffer for variable length codes           */
    VTBYTE    byNumInBits;            /* number of valid bits in input buffer             */

    VTBYTE    bySs, bySe;             /* progressive JPEG spectral selection parms        */
    VTBYTE    byAh, byAl;             /* progressive JPEG successive approx. parms        */
    VTBOOL    bProgressiveMode;       /* TRUE if SOFn specifies progressive mode          */

    /* clip [-256,511] --> [0,255] */
    VTHANDLE  hDctClip;
    VTLPBYTE  pDctClip;
    /*int       dctClipInit;*/

} IODCTDATA, * PIODCTDATA;

/*********************************************************************/
/*********************************************************************/
/*************************                  **************************/
/*************************  MIFAsciiDecode  **************************/
/*************************                  **************************/
/*********************************************************************/
/*********************************************************************/
/* Named MIFAsciiDecode because I have not discovered the real format name */
/* MIFAsciiDecode Support added Nov 11, 2001 */
/* these values correspond to the format, do not change */
#define MIFCOMPRESS_BINARYINASCII  1
#define MIFCOMPRESS_UNKNOWN        2
#define IMAGELINEBUFSIZE  0x80

typedef struct IOMIFAsciiDecodetag
{
    VTHANDLE      hSrcbuf;
    VTLPBYTE      Srcbuf;
    VTBOOL        HexMode;            /* Data is binary or ascii */
    VTBOOL        FirstTime;
    VTBYTE        UnGetVal;
    VTBOOL        UnGetIsValid;
    VTLONG        ByteCount;
    VTBYTE        Datatype;
    VTBOOL        DecodeComplete;
} IOMIFAsciiDecode, * PIOMIFAsciiDecode;

/*********************************************************************/
/*********************************************************************/
/*********************                         ***********************/
/*********************  CCITT G3D1, G3D2, G4D2 ***********************/
/*********************                         ***********************/
/*********************************************************************/
/*********************************************************************/

/*
 | For transfering extra parmeters needed for CCITT decompression from IOCOMPRESSDATA, following is used:
 |  dwReserved1 -
 |          bits 0-15:  K - a code identifying the encoding scheme used:
 |          (int)       K<0 Pure G42D encoding
 |                      K=0 Pure G31D encoding
 |                      K>0 Mixed G3 1D and 2D encoding, in which a line encoded 1D can be
 |                          followed by at most K-1 lines encoded 2D.
 |          bits 16-32: DamagedRowsBeforeError - The number of damaged rows of data to be tolerated
 |          (int)       before and error occures.  This entry applies only if EndOfLine is true and
 |                      K is nonnegative.
 |  dwReserved2 -
 |          bit 0:      EndOfLine - a flag indicating whether EOL bit patterns are required to be
 |                      present in the encoding.  Decompression always accepts EOL bit patterns, but
 |                      requires them only if EndOfLine is true.
 |          bit 1:      EndOfBlock - a flag indicating whether the decompression expects the encoded data
 |                      to be terminated by an EOB pattern, overriding the Rows parameter.  If false,
 |                      the decompression stops when it has decoded the number of lines indicated by Rows,
 |                      or when its data has been exausted, whichever occurs first.  The bit pattern
 |                      depends on K parameter and can be either EOFB (end-of-facsimile-block) or RTC
 |                      (return-to-control).
 |          bit 2:      EncodedByteAlign - A flag indicating whether the decompression expects extra
 |                      0-bits before each encoded line so that the line begins on a byte boundary.
 |                      If true, the filter skips over encoded bits to begin decoding each line at byte
 |                      boundary.
 |          bit 3:      EncodedWordAlign - Indicates that each line of encoded data begins on a word
 |                      (16-bit) boundary.
 |          bit 4:      Blacks1 - a flag indicating whether 1-bits are to be interpreted as black pixels
 |                      and 0-bits as white pixels, the reverse of the normal PDF convention for monochrome
 |                      image data.
 |          bit 5:      LSBtoMSB - Indicates that the fill order is least significant to most significant.
 |          bit 6:      Group3RLE - Indicates that the data is encoded using RLE compression. This is only
 |                      used by TIFF files currently. Only applies if K=0.
 |  dwReserved3 -
 |          bits 0-15:  Columns - the width of the image in pixels.  If the value is not a multiple of 8,
 |                      the compression should adjust the width of the unencoded image to the next multiple
 |                      of 8, so that each line starts on a byte boundary.
 |          bits 16-32: The height of the image in scan lines.  If the value is 0 or absent, the images
 |                      height is not predetermined, and the encoded data must be terminated by an EOB bit
 |                      pattern or by the end of the stream data.
 |
 */

/* Masks for extracting data from the reserved words. */
#define CCITT_MASK_ENCODING         0x0000FFFF  /* dwReserved1 */
#define CCITT_MASK_DAMAGE_ROWS      0xFFFF0000

#define CCITT_MASK_EOL              0x00000001  /* dwReserved2 */
#define CCITT_MASK_EOB              0x00000002
#define CCITT_MASK_BYTE_ALIGN       0x00000004
#define CCITT_MASK_WORD_ALIGN       0x00000008
#define CCITT_MASK_BLACK            0x00000010
#define CCITT_MASK_LSBTOMSB         0x00000020
#define CCITT_MASK_GROUP3RLE        0x00000040

#define CCITT_MASK_COLUMNS          0x0000FFFF  /* dwReserved3 */
#define CCITT_MASK_ROWS             0xFFFF0000

/*********************************************************************/
/*********************************************************************/
/*********************                         ***********************/
/*********************           RC4           ***********************/
/*********************                         ***********************/
/*********************************************************************/
/*********************************************************************/
typedef struct IORC4DATAtag
{
    VTHANDLE hKey;
    VTBYTE*  pKey;
    VTBYTE   byKeyLength;
    VTRC4_CTX RC4State;
} IORC4DATA, * PIORC4DATA;

/*********************************************************************/
/*********************************************************************/
/*********************                         ***********************/
/*********************           AES           ***********************/
/*********************                         ***********************/
/*********************************************************************/
/*********************************************************************/

typedef struct IOAESDATAtag
{
#ifdef SCCFEATURE_64BITIO
    VTOFF_T  offFileSize;     /* decrypted file size */
#else
    VTDWORD  offFileSize;     /* decrypted file size */
#endif
    VTDWORD  dwType;          /* AES type (i.e. AES-128, AES-192, AES-256) */
    VTDWORD  dwMode;          /* mode of operation                         */
    VTBYTE   szAESBlock[16];  /* generic AES block                         */
    VTBYTE  *pKeySchedule;    /* pointer to an AES key schedule            */
} IOAESDATA, *PIOAESDATA;


typedef struct IOZIPCRYPTODATAtag
{
#ifdef SCCFEATURE_64BITIO
    VTOFF_T  offFileSize;
#else
    VTDWORD  offFileSize;
#endif
    VTDWORD  dwCrcTable[256];
    VTDWORD  Key[3];
} IOZIPCRYPTODATA, *PIOZIPCRYPTODATA;

/*********************************************************************/
/*********************************************************************/
/*********************                         ***********************/
/*********************          JBIG2          ***********************/
/*********************                         ***********************/
/*********************************************************************/
/*********************************************************************/

#define JBIG2_SYMBOL_DICT           0
#define JBIG2_INT_TEXT              4
#define JBIG2_IMM_TEXT              6
#define JBIG2_IMM_LL_TEXT           7
#define JBIG2_PATTERN_DICT          16
#define JBIG2_INT_HALFTONE          20
#define JBIG2_IMM_HALFTONE          22
#define JBIG2_IMM_LL_HALFTONE       23
#define JBIG2_INT_GENERIC           36
#define JBIG2_IMM_GENERIC           38
#define JBIG2_IMM_LL_GENERIC        39
#define JBIG2_INT_GENERIC_REF       40
#define JBIG2_IMM_GENERIC_REF       42
#define JBIG2_IMM_LL_GENERIC_REF    43
#define JBIG2_PAGE_INFO             48
#define JBIG2_END_OF_PAGE           49
#define JBIG2_END_OF_STRIPE         50
#define JBIG2_END_OF_FILE           51
#define JBIG2_PROFILES              52
#define JBIG2_TABLES                53
#define JBIG2_EXTENSIONS            62

#define JBIG2_INTCONTEXTSIZE 512

#define JBIG2_BOTTOMLEFT    0
#define JBIG2_TOPLEFT       1
#define JBIG2_BOTTOMRIGHT   2
#define JBIG2_TOPRIGHT      3

#define JBIG2_OR            0
#define JBIG2_AND           1
#define JBIG2_XOR           2
#define JBIG2_XNOR          3

typedef struct JBIGHEADERDATA
{
    VTDWORD dwSegmentNumber;
    VTDWORD dwFlags;
    VTBYTE  bySegmentType;
    VTDWORD dwReferredToCount;
    VTBYTE  byRetainFlags;
/*  union
    {
        BYTE    byRetainFlags;
        LPDWORD pdwRetainFlags;
    } uRetainUnion;
    union
    {
        LPBYTE  byteSize;
        LPWORD  wordSize;
        LPDWORD dwordSize;
    } uReferredSegments;
 */

    union
    {
        VTBYTE  bySegmentPage;
        VTDWORD dwSegmentPage;
    } uPageAssociation;
    VTDWORD dwSegmentDataLength;

} JBIG_STRUCT, *LPJBIG_STRUCT;

typedef struct JBIG2_RegionSegmentInfo_tag
{
    VTDWORD dwXPos;
    VTDWORD dwYPos;
    VTDWORD dwWidth;
    VTDWORD dwHeight;
    VTDWORD dwByteWidth;
    VTBYTE  byFlags;
} JBIG2_RegionSegmentInfo, *PJBIG2_RegionSegmentInfo;

typedef enum HUFFMAN_ROW_TYPEtag
{
	ROW_NORMAL,
	ROW_OOB,
	ROW_LOWER,
	ROW_HIGHER
}HUFFMAN_ROW_TYPE;

typedef struct JBIG2_Arithmetic_tag
{
    VTDWORD c;
    VTLONG  a;
    VTLONG  ct;
} JBIG2_Arithmetic, *PJBIG2_Arithmetic;

typedef struct JBIG2_Huffman_tag
{
	VTBYTE	byThisByte;
	VTSHORT BitOffset;
} JBIG2_Huffman, *PJBIG2_Huffman;

typedef struct JBIG2_HUFFMAN_LINEtag
{
	VTLONG RANGELOW;
	VTLONG PREFLEN;
	VTLONG RANGELEN;
	VTLONG PREFIX;
} JBIG2_HUFFMAN_LINE, * PJBIG2_HUFFMAN_LINE;

typedef struct JBIG2_HUFFMAN_TABLEtag
{
	VTBOOL HTOOB;
	VTLONG nLines;
	const JBIG2_HUFFMAN_LINE * Lines;
}JBIG2_HUFFMAN_TABLE, *PJBIG2_HUFFMAN_TABLE;

typedef struct JBIG2_HUFFMAN_NODE_tag JBIG2_HUFFMAN_NODE;
typedef struct JBIG2_HUFFMAN_NODE_tag * PJBIG2_HUFFMAN_NODE;

struct JBIG2_HUFFMAN_NODE_tag
{
	VTLONG Index;
	PJBIG2_HUFFMAN_NODE ONE;
	PJBIG2_HUFFMAN_NODE ZERO;
};

typedef struct JBIG2_Bitmap_tag
{
	VTDWORD dwImageWidth;
	VTDWORD dwImageHeight;
	VTDWORD dwRowStride; /*Number of Bytes in Row */
	VTDWORD dwRefCount; /* number of references to this image */
	VTLPBYTE	Image;  /* Uncompressed Image Data of size (dwRowStride*dwImageHeight)*/
} JBIG2_Bitmap, *PJBIG2_Bitmap;

typedef struct JBIG2_SymbolDictionary_tag
{
	VTDWORD	dwSymCount;
	PJBIG2_Bitmap * Symbols;
}JBIG2_SymbolDictionary, *PJBIG2_SymbolDictionary;

typedef struct JBIG2_SymbolSettings_tag
{
    VTBOOL					bHuff;
	VTBOOL					bRefAgg;
	PJBIG2_SymbolDictionary	pSDInSyms;			/*Symbol distionary that is used as an input to this symbol dictionary. It contains dwNumInSyms symbols */

	VTDWORD					dwNumExSyms;		/*The number of symbols to be defined in this symbol dictionary */
    VTDWORD					dwNumNewSyms;		/*The number of symbols to be exported from this symbol dictionary */
    
	PJBIG2_HUFFMAN_TABLE	SDHUFFDH;
	PJBIG2_HUFFMAN_NODE		TreeSDHUFFDH;
	PJBIG2_HUFFMAN_TABLE	SDHUFFDW;
	PJBIG2_HUFFMAN_NODE		TreeSDHUFFDW;
	PJBIG2_HUFFMAN_TABLE	SDHUFFBMSIZE;
	PJBIG2_HUFFMAN_NODE		TreeSDHUFFBMSIZE;
	PJBIG2_HUFFMAN_TABLE	SDHUFFAGGINST;
	PJBIG2_HUFFMAN_NODE		TreeSDHUFFAGGINST;

	VTBYTE					byTemplate;
    VTCHAR					chSYMAT[4][2];      /* these are signed bytes, *not* text data. Do not make them TCHARs! */
    
    VTBYTE					byRefTemplate;      
    VTCHAR					chSYMATRef[4];   /*More signed bytes. Don't make TCHARs */

	VTBOOL					bBMPCodingUsed;
    VTBOOL					bBMPCodingKept;
    VTDWORD					dwNumProcessedSyms;
} JBIG2_SymbolSettings, *PJBIG2_SymbolSettings;

typedef struct JBIG2_GenericSettings_tag
{
    VTBOOL  bMMR;
    VTBOOL  bTPGDON;
	VTBOOL	bLTP;
    VTBYTE  byGBT;
	VTDWORD dwHeight;
	VTDWORD dwWidth;
    VTCHAR  chGBAT[4][2]; /* these are signed bytes, *not* text data. Do not make them TCHARs! */
} JBIG2_GenericSettings, *PJBIG2_GenericSettings;

typedef struct JBIG2_TextSettings_tag
{
    VTBOOL  bHuff;
    VTBOOL  bRefine;
	VTBYTE  byLogSBStrips;
    VTBYTE  byStrips;
    VTBYTE  byRefCorner;
    VTBOOL  bTransposed;
    VTBYTE  byComboOp;
    VTBOOL  bDefPix;
    VTCHAR  DeltaSOffset; /* bug9056767: Changing from VTBYTE to VTSHORT because it is a signed value -zf 04/12/10 */
    VTBOOL  bRefTemp;
	VTDWORD dwNumSyms;		/* SBNUMSYMS: The number of symbols that may be used in this region */
	VTDWORD dwSBW;			/* width of Text Region */
	VTDWORD dwSBH;			/* Height of Text Region */ 
	PJBIG2_SymbolDictionary pSBSyms; /*An Array containing the symols used in this text region. COntains SBNUMSYMS symbols */
	
	VTWORD  wHuffFlags;
	PJBIG2_HUFFMAN_TABLE SDHUFFFS;
	PJBIG2_HUFFMAN_NODE TreeSDHUFFFS;
	PJBIG2_HUFFMAN_TABLE SDHUFFDS;
	PJBIG2_HUFFMAN_NODE TreeSDHUFFDS;
	PJBIG2_HUFFMAN_TABLE SDHUFFDT;
	PJBIG2_HUFFMAN_NODE TreeSDHUFFDT;
	PJBIG2_HUFFMAN_TABLE SDHUFFRDW;
	PJBIG2_HUFFMAN_NODE TreeSDHUFFRDW;
	PJBIG2_HUFFMAN_TABLE SDHUFFRDH;
	PJBIG2_HUFFMAN_NODE TreeSDHUFFRDH;
	PJBIG2_HUFFMAN_TABLE SDHUFFRDX;
	PJBIG2_HUFFMAN_NODE TreeSDHUFFRDX;
	PJBIG2_HUFFMAN_TABLE SDHUFFRDY;
	PJBIG2_HUFFMAN_NODE TreeSDHUFFRDY;
	PJBIG2_HUFFMAN_TABLE SDHUFFRSIZE;
	PJBIG2_HUFFMAN_NODE TreeSDHUFFRSIZE;

	VTCHAR		SBRAT[4]; /* Refinement AT flags */
    VTDWORD dwNumInstances;
    VTDWORD dwNumProcessedInstances;
} JBIG2_TextSettings, *PJBIG2_TextSettings;

typedef struct JBIG2_BitmapHeader_tag
{
    VTDWORD dwWidth;
    VTDWORD dwHeight;
} JBIG2_BitmapHeader, *PJBIG2_BitmapHeader;

typedef struct JBIG2_TextInstance_tag
{
    VTLONG lS;
    VTLONG lT;
    VTLONG lID;
	VTBOOL bRefine;			/* indicates if the symbol is refined */
	PJBIG2_Bitmap Symbol;
} JBIG2_TextInstance, *PJBIG2_TextInstance;

typedef struct JBIG2_RefinementSettings_tag
{
	VTDWORD		dwGRW;
	VTDWORD		dwGRH;
	VTBYTE		byGRTemplate;
	PJBIG2_Bitmap pRefImage;
	VTLONG		lRDX;
	VTLONG		lRDY;
	VTCHAR		GRAT[4];
}JBIG2_RefinementSettings, *PJBIG2_RefinementSettings;


typedef struct IOJBIG2DATAtag
{
    JBIG2_Arithmetic        ArithState;
	JBIG2_Huffman			HuffmanState;
    JBIG2_RegionSegmentInfo SegInfo;
    JBIG2_GenericSettings   GenericSettings;
    JBIG2_SymbolSettings    SymbolSettings;
    JBIG2_TextSettings      TextSettings;

    VTHANDLE                hArithmeticContext;
    VTLPBYTE                pArithmeticContext;

    VTHANDLE                hIADHContext;
    VTLPBYTE                pIADHContext;
    VTHANDLE                hIADWContext;
    VTLPBYTE                pIADWContext;
    VTHANDLE                hIAEXContext;
    VTLPBYTE                pIAEXContext;
	VTHANDLE                hIAAIContext;
    VTLPBYTE                pIAAIContext;

    VTHANDLE                hIADTContext;
    VTLPBYTE                pIADTContext;
    VTHANDLE                hIAFSContext;
    VTLPBYTE                pIAFSContext;
    VTHANDLE                hIADSContext;
    VTLPBYTE                pIADSContext;
    VTHANDLE                hIAITContext;
    VTLPBYTE                pIAITContext;
    VTHANDLE                hIAIDContext;
    VTLPBYTE                pIAIDContext;
	VTHANDLE                hIARDXContext;
    VTLPBYTE                pIARDXContext;
	VTHANDLE                hIARDYContext;
    VTLPBYTE                pIARDYContext;
	VTHANDLE                hIARDHContext;
    VTLPBYTE                pIARDHContext;
	VTHANDLE                hIARDWContext;
    VTLPBYTE                pIARDWContext;
	VTHANDLE                hIARIContext;
    VTLPBYTE                pIARIContext;
} IOJBIG2DATA, *PIOJBIG2DATA;



/*********************************************************************/
/*********************************************************************/
/*********************                         ***********************/
/*********************           LZX           ***********************/
/*********************                         ***********************/
/*********************************************************************/
/*********************************************************************/

/* spec says max # of slots is 42, but others have disagreed */
#define LZX_MAX_SLOTS           50

#define LZX_NUM_CHARS           256

/* constants for building main tree lookup table */
#define LZX_MAIN_TREE_ELEMS     (LZX_NUM_CHARS+LZX_MAX_SLOTS*8)
#define LZX_MAIN_LOOKUP_BITS    12
#define LZX_MAIN_LOOKUP_SIZE    ((1<<LZX_MAIN_LOOKUP_BITS) + 2*LZX_MAIN_TREE_ELEMS)

/* constants for building length tree lookup table */
#define LZX_LENGTH_TREE_ELEMS   249
#define LZX_LENGTH_LOOKUP_BITS  12
#define LZX_LENGTH_LOOKUP_SIZE  ((1<<LZX_LENGTH_LOOKUP_BITS) + 2*LZX_LENGTH_TREE_ELEMS)

/* constants for building align tree lookup table */
#define LZX_ALIGN_TREE_ELEMS    8
#define LZX_ALIGN_LOOKUP_BITS   7
#define LZX_ALIGN_LOOKUP_SIZE   ((1<<LZX_ALIGN_LOOKUP_BITS) + 2*LZX_ALIGN_TREE_ELEMS)

typedef struct IOLZXDATAtag
{
    VTBOOL      bEOF;

    VTWORD      wBitBuf;            /* stores most significant bit first */
    VTWORD      wBitsLeft;          /* # bits in wBitBuf left to read */

    VTDWORD     dwBytesRead;        /* total # of uncompressed bytes read */

    /* window size is already stored in IOCOMPRESSFILE.dwReserved1 */
    VTWORD      wPositionSlots;     /* # of window subdivisions/slots */
    VTDWORD     dwTransSize;        /* 0xe8 opcode translation size */
    VTBYTE      ubBlockType;

    VTWORD      MainData[LZX_MAIN_TREE_ELEMS];      /* Main tree lengths */
    VTWORD      LengthData[LZX_LENGTH_TREE_ELEMS];  /* Length tree lengths */
    VTWORD      AlignData[LZX_ALIGN_TREE_ELEMS];    /* Align tree lengths */

    /* lookup arrays for determining huffman code values */
    VTWORD      MainLookup[LZX_MAIN_LOOKUP_SIZE];
    VTWORD      LengthLookup[LZX_LENGTH_LOOKUP_SIZE];
    VTWORD      AlignLookup[LZX_ALIGN_LOOKUP_SIZE];

    VTHANDLE    hWindow;            /* handle to memory in pWindow */
    VTBYTE*     pWindow;            /* sliding window array */
    VTDWORD     dwWindowLoc;        /* current location in pWindow */
    VTDWORD     dwDecodeLoc;        /* location needing 0xe8 decoding */

    VTDWORD     RecentOffsets[3];   /* R# values as described in lzx spec */
} IOLZXDATA, *PIOLZXDATA;

/* Structure used with IOGETINFO_DPATHNAME */
typedef struct DPATHNAMEtag
{
    VTDWORD  dwPathLen;     // # bytes required to store name + null terminator
    VTVOID  *pPath;         // same character width as original spec
} DPATHNAME, * PDPATHNAME;



/*********************************************************************/
/*********************************************************************/
 /*
  |   Macros
  */
#define IOOpenVia(a,b,c,d,e) ((PBASEIO)(a))->pOpen(b,c,d,e)
#define IOClose(a)           ((PBASEIO)(a))->pClose(a)
#define IORead(a,b,c,d)      ((PBASEIO)(a))->pRead(a,(VTBYTE *)b,c,d)
#define IOWrite(a,b,c,d)     ((PBASEIO)(a))->pWrite(a,(VTBYTE *)b,c,d)
#define IOSeek(a,b,c)        ((PBASEIO)(a))->pSeek(a,b,c)
#define IOTell(a,b)          ((PBASEIO)(a))->pTell(a,b)
#define IOGetInfo(a,b,c)     ((PBASEIO)(a))->pGetInfo(a,b,c)
#ifdef SCCFEATURE_64BITIO
#define IOSeek64(a,b,c)      ((PBASEIO)(a))->pSeek64(a,b,c)
#define IOTell64(a,b)        ((PBASEIO)(a))->pTell64(a,b)
#else
/* Let's make stubs for the 64-bit functions to avoid compiler errors. */
/*IOERR dummySeek64(HIOFILE hFile, VTWORD wFrom, VTOFF_T Offset) {return IOERR_OK; }
IOERR dummyTell64(HIOFILE hFile, VTOFF_T *Offset) { return IOERR_OK; }*/

#define IOSeek64(a,b,c)     IOERR_OK
#define IOTell64(a,b)       IOERR_OK
#endif
  /*
  |   Functions
  */
IO_ENTRYSC IOERR IO_ENTRYMOD IOCreate(HIOFILE * phFile, VTDWORD dwType, VTVOID * pSpec, VTDWORD dwFlags);
IO_ENTRYSC IOERR IO_ENTRYMOD IOOpen(HIOFILE * phFile, VTDWORD dwType, VTVOID * pSpec, VTDWORD dwFlags);
IO_ENTRYSC IOERR IO_ENTRYMOD IOSetFileTime(HIOFILE hFile, PIODATETIME pDate, VTDWORD dwFlags);
IO_ENTRYSC IOERR IO_ENTRYMOD IODelete(VTDWORD dwType, VTVOID * pSpec, VTDWORD dwFlags);
IO_ENTRYSC IOERR IO_ENTRYMOD IOGetTempFullUnicodeFileName(VTLPWORD pwszFileName);
IO_ENTRYSC IOERR IO_ENTRYMOD IOGetTempFullFileName(VTLPTSTR pszFileName, VTDWORD dwSpecType);
IO_ENTRYSC VTDWORD IO_ENTRYMOD IOMapIOERR(IOERR ieCode);
IO_ENTRYSC IOERR IO_ENTRYMOD IOFillDPathName (HIOFILE hFile, PDPATHNAME pvlp);
IO_ENTRYSC IOERR IO_ENTRYMOD IOFillDPathNameA (HIOFILE hFile, PDPATHNAME pvlp);
IO_ENTRYSC IOERR IO_ENTRYMOD IOFillDPathNameW (HIOFILE hFile, PDPATHNAME pvlp);
IO_ENTRYSC IOERR IO_ENTRYMOD IOGenSecondaryDP(HIOFILE hFile, VTDWORD dwGenType, PIOGENSECONDARYDP pSecData);

  /*
  |  Structure used to access the internal storage of file specifications.
  |  Note: This type of structure is never passed into the IO routines
  |  but is used to marshall IO specs into a single data block
  |
  |  Note: The Mac FSSpec structure is only available on the Mac
  |
  */
typedef struct IOSPECtag
{
  VTDWORD  dwSignature;
  VTDWORD  dwSize;
  VTDWORD  dwType;
  union
  {
    VTBYTE             szDosPath[1];
    VTBYTE             szAnsiPath[1];
    VTBYTE             szMacPath[1];
    VTBYTE             szUnixPath[1];
    VTWORD             szUnicodePath[1];
    IOSPECMACHFS       sMacHfs;
    IOSPECRANGE        sRange;
    IOSPECCOMPRESS     sCompress;
    VTTCHAR            szTempPrefix[1];
    VTBYTE             aGen[1];
    IOSPECSECONDARY    sSecondary;
    IOSPECSUBSTREAM    sSubStream;
    IOSPECSUBSTORAGE   sSubStorage;
    /* Writable OLE2 Microsoft */
    IOSPECMROOTSTORAGE sMRootStorage;
    IOSPECMSTORAGE     sMStorage;
    IOSPECMSTREAM      sMStream;
    /* Writable OLE2 native */
    IOSPECOLE2ROOTSTORAGE sOLE2RootStorage;
    IOSPECOLE2STREAM   sOLE2Stream;
    IOSPECOLE2STORAGE  sOLE2Storage;
    IOSPECZIPNODE      sZipNode;
    PBASEIO            pRedirect;
  } uTypes;
} IOSPEC, * PIOSPEC;

typedef VTHANDLE HIOSPEC;

#define IO_IOSPEC_SIGNATURE  0x10FED3F1

IO_ENTRYSC IOERR IO_ENTRYMOD IOAllocSpec(VTDWORD dwType, VTVOID *  pSpec, VTHANDLE * phSpec);

/* Added for writable ole2. */
IO_ENTRYSC IOERR IO_ENTRYMOD IOSetInfo(HIOFILE hFile, VTDWORD dwType, VTDWORD dwInfoId, VTVOID * pInfo);

/* dwInfoId parameters for IOSetInfo(). */
#define IOSETINFO_OLE2CLSID           1
#define IOSETINFO_CASESENSITIVITY     2

  /*
  |  Structure used to retrieve Class IDs of OLE2 files.
  */

typedef struct SCCOLE2CLSIDtag
{
  VTDWORD Data1;
  VTWORD  Data2;
  VTWORD  Data3;
  VTBYTE  Data4[8];
} SCCOLE2CLSID, * PSCCOLE2CLSID;

#ifdef __cplusplus
}
#endif

#ifdef SCC_PACKED_BY_SCCIO_H
#pragma pack(pop)
#undef SCC_PACKED_BY_SCCIO_H
#undef SCC_PACK_SET
#endif /* SCC_PACKED_BY_SCCIO_H */

#endif /*SCCIO_H*/
