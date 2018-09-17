/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. 
All rights reserved. */

/** BASEIO.H **/
/*
 This file contains the #defines and typedefs for the fundamental
 OIT abstraction layer on file I/O.
*/

#ifndef BASEIO_H
#define BASEIO_H

#ifdef WINDOWS
#ifndef SCC_PACK_SET
#define SCC_PACK_SET 1
#define SCC_PACKED_BY_BASEIO_H 1
#pragma pack(push,8)
#endif /* SCC_PACK_SET */
#endif /* WINDOWS */

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef WIN32
#define IO_ENTRYSC      __declspec(dllexport)
#define IO_ENTRYMOD     __cdecl
#define IO_ENTRYMODPTR  IO_ENTRYMOD *
#endif /*WIN32*/

#ifdef UNIX
#define IO_ENTRYSC
#define IO_ENTRYMOD
#define IO_ENTRYMODPTR  IO_ENTRYMOD *
#endif   /*UNIX*/

#ifdef NLM
#define IO_ENTRYSC
#define IO_ENTRYMOD
#define IO_ENTRYMODPTR  IO_ENTRYMOD *
#endif   /*NLM*/

  /*
  |   A file handle
  */

typedef VTSYSVAL HIOFILE;
#define HIOFILE_INVALID  ((HIOFILE)-1)

  /*
  |   Error handling typedef and defines
  */

typedef int IOERR;

#define IOERR_OK                0
#define IOERR_TRUE              0
#define IOERR_UNKNOWN           -1
#define IOERR_INVALIDSPEC       -2    /* The file spec given (IOSPEC) cannot be used in this environment */
#define IOERR_ALLOCFAIL         -3    /* The IO system could not allocate memory */
#define IOERR_BADPARAM          -4    /* The one of the parameter contained bad or insufficient info */
#define IOERR_NOFILE            -5    /* File not found */
#define IOERR_NOCREATE          -6    /* File could not be created */
#define IOERR_BADINFOID         -7    /* dwInfoId parameter to IOGetInfo was invalid */
#define IOERR_SEEKOUTOFRANGE    -8    /* Seeking out of range in a range type file */
#define IOERR_EOF               -9    /* End of file reached */
#define IOERR_FALSE             -10
#define IOERR_LOCKFAIL          -11   /* Failed to lock allocated memory (Windows) */
#define IOERR_BADFILESIZE       -12
#define IOERR_INVALIDSTATE      -13
#define IOERR_SHORTREAD         -14   /* Failed to read the expected bytecount */
#define IOERR_SHORTWRITE        -15   /* Failed to write the expected bytecount */
#define IOERR_UNABLETOEXPAND    -16   /* Tried to expand an OLE2 file and failed. */
#define IOERR_BADFILENAME		-17   /* Full file name length exceed the exceeds OS limits */
#define IOERR_FILENAMERESERVED  -18   /* Os has this filename reserved */
#define IOERR_INSUFFICIENTBUFFER -19  /* Buffer size too small for requested string */

  /*
  |   Base IO routine definitions
  */
#ifndef NLM
typedef IOERR (IO_ENTRYMODPTR IOSEEK64PROC)(HIOFILE hFile, VTWORD wFrom, VTOFF_T Offset);
typedef IOERR (IO_ENTRYMODPTR IOTELL64PROC)(HIOFILE hFile, VTOFF_T *  Offset);
#endif
typedef IOERR (IO_ENTRYMODPTR IOOPENPROC)(HIOFILE * phFile, VTDWORD dwType, VTVOID * pSpec, VTDWORD dwFlags);
typedef IOERR (IO_ENTRYMODPTR IOCLOSEPROC)(HIOFILE hFile);
typedef IOERR (IO_ENTRYMODPTR IOREADPROC)(HIOFILE hFile, VTBYTE * pData, VTDWORD dwSize, VTDWORD * pCount);
typedef IOERR (IO_ENTRYMODPTR IOWRITEPROC)(HIOFILE hFile, VTBYTE * pData, VTDWORD dwSize, VTDWORD * pCount);
typedef IOERR (IO_ENTRYMODPTR IOSEEKPROC)(HIOFILE hFile, VTWORD wFrom, VTLONG dwOffset);
typedef IOERR (IO_ENTRYMODPTR IOTELLPROC)(HIOFILE hFile, VTDWORD * dwOffset);
typedef IOERR (IO_ENTRYMODPTR IOGETINFOPROC)(HIOFILE hFile, VTDWORD dwInfoId, VTVOID * pInfo);

 /*
 | Encryption routines
 */
typedef IOERR  (IO_ENTRYMODPTR IOUTMD5) (VTBYTE *pMsg, VTDWORD dwMsgLength, VTBYTE *pDigest);

  /*
  |   Defines for dwFlags in IOOPEN function
  */

#define IOOPEN_READ          0x00000001
#define IOOPEN_WRITE         0x00000002
#define IOOPEN_READWRITE     0x00000003
#define IOOPEN_DELETEONCLOSE 0x00000004
#define IOOPEN_NOOLESTORAGE  0x00000008
#define IOOPEN_CREATE        0x00000010   /* For Transformation Server*/
#define IOOPEN_CLEAR         0x00000020   /* For IX on Unix; when we used to open a file read/write, the contents would not be cleared out before we started to write to the file */
#define IOOPEN_RANDOMACCESS  0x00000040   /* Windows optimization for random access caching */
#define IOOPEN_MEMORYMAPPED  0x00000080   /* Optimization to load entire file into RAM before processing */
#define IOOPEN_WRITEXCLUSIVE 0x00000100   /* For writing options files. We don't want multiple writes at the same time. */

  /*
  |   Defines for wFrom in IOSEEK function
  */
#define IOSEEK_TOP        0
#define IOSEEK_CURRENT    1
#define IOSEEK_BOTTOM     2

  /*
  |   Defines for dwInfoId in IOGETINFO function
  */

#define IOGETINFO_OSHANDLE            1
#define IOGETINFO_HSPEC               2
#define IOGETINFO_FILENAME            3
#define IOGETINFO_ISOLE2STORAGE       4
#define IOGETINFO_OLE2CLSID           5
#define IOGETINFO_PATHNAME            6     /* Human readable path name to the file (includes file name) */
#define IOGETINFO_ISOLE2ROOTSTORAGE   7
#define IOGETINFO_ISOLE2SUBSTORAGE    8
#define IOGETINFO_ISOLE2SUBSTREAM     9
#define IOGETINFO_PARENTHANDLE        10
#define IOGETINFO_FILESIZE            11
#define IOGETINFO_ISREADONLY          12
#define IOGETINFO_TIMEDATE            13    /* # seconds since Jan.1, 1970 */
#define IOGETINFO_PATHTYPE            14    /* OITYPE of the path returned by IOGETINFO_PATHNAME,
                                               this can be different than the original spec type
                                               in the case of certain Mac types and redirected IO */
#define IOGETINFO_GENSECONDARY        15
#define IOGETINFO_ISDELETEONCLOSE     16
#define IOGETINFO_FILENAME_UNICODE    17    /* Returns name using Unicode chars. */
#define IOGETINFO_PATHNAME_UNICODE    18

#define IOGETINFO_GENSECONDARYFULL    19
#define IOGETINFO_SUBDOC_SPEC         20

#define IOGETINFO_COMPRESSEDBYTESUSED 21

#define IOGETINFO_ZIPRANGE            22    /* returns start/end offsets of zip node */
#define IOGETINFO_FILESIZE64          23
#define IOGETINFO_64BITIO             24
#define IOGETINFO_COMPRESSTYPE		  25
#define IOGETINFO_IOTYPE              26

#define IOGETINFO_DPATHNAME           27
#define IOGETINFO_GENSECONDARYDP      28
#define IOGETINFO_GENSECONDARYFULLDP  29

#define IOGETINFO_NOTIFICATION_FILECOMPLETE       64  
/* values above 127 are reserved for Transformation Server */


typedef struct IOGENSECONDARYtag
{
    VTDWORD         dwSize;
    VTTCHAR *       pFileName;
    VTDWORD         dwSpecType;
    VTVOID *        pSpec;
    VTDWORD         dwOpenFlags;
} IOGENSECONDARY, * PIOGENSECONDARY;

/* This is the new structure for Unicode file names */
typedef struct IOGENSECONDARYWtag
{
    VTDWORD         dwSize;
    VTWORD *        pFileName;
    VTDWORD         dwSpecType;
    VTVOID *        pSpec;
    VTDWORD         dwOpenFlags;
} IOGENSECONDARYW, * PIOGENSECONDARYW;

/* This is the new structure for dynamic file names */
typedef struct IOGENSECONDARYDPtag
{
    VTDWORD         dwSize;
    VTVOID *        pFileName;      // same character width as original spec
    VTDWORD         dwSpecType;
    VTVOID *        pSpec;          // same character width as original spec
    VTDWORD         dwSpecLen;      // # bytes required to store name + null terminator
    VTDWORD         dwOpenFlags;
} IOGENSECONDARYDP, * PIOGENSECONDARYDP;



typedef struct BASEIOtag
{
    IOCLOSEPROC   pClose;
    IOREADPROC    pRead;
    IOWRITEPROC   pWrite;
    IOSEEKPROC    pSeek;
    IOTELLPROC    pTell;
    IOGETINFOPROC pGetInfo;
    IOOPENPROC    pOpen;        /* pOpen *MUST* be set to NULL. */
#ifndef NLM
    IOSEEK64PROC  pSeek64;
    IOTELL64PROC  pTell64;
#endif
    VTVOID *aDummy[3];
} BASEIO, * PBASEIO;

#ifdef __cplusplus
}
#endif

#ifdef SCC_PACKED_BY_BASEIO_H
#pragma pack(pop)
#undef SCC_PACKED_BY_BASEIO_H
#undef SCC_PACK_SET
#endif /* SCC_PACKED_BY_BASEIO_H */

#endif /* BASEIO_H */
