/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved. */

  /*
  |  Outside In Viewer Technology - Include
  |
  |  Include:      SCCTYPE.H
  |  Environment:  Portable
  |  Function:     Define the Base VT* types
  |
  */

#ifndef SCCTYPE_H
#define SCCTYPE_H

#if defined(_WIN32) && !defined(NLM)
# pragma once
# ifndef WIN32
#  define WIN32
# endif
# ifndef WINDOWS
#  define WINDOWS
# endif
# include <wtypes.h>
# if !defined(PTR_TYPES_DEFINED) && (_MSC_VER >= 1300)
#  define PTR_TYPES_DEFINED
# endif
#endif


#ifndef PTR_TYPES_DEFINED
#define PTR_TYPES_DEFINED
#ifdef UNIX_64
typedef unsigned long long UINT_PTR;
typedef unsigned long long DWORD_PTR;
typedef long long           LONG_PTR;
#else
#if !defined(WIN32) || !defined(MAXUINT_PTR)
typedef unsigned long       UINT_PTR;
#endif
typedef unsigned long       DWORD_PTR;
typedef signed long         LONG_PTR;
#endif
#endif  /* PTR_TYPES_DEFINED */


#define VTVOID              void

typedef void *              VTSYSVAL;
typedef DWORD_PTR           VTSYSPARAM;

typedef char                VTCHAR;
typedef VTCHAR              VTTCHAR;
typedef unsigned char       VTBYTE;
typedef signed short        VTSHORT;
typedef unsigned short      VTWORD;
#ifdef WIN32
typedef WCHAR               VTWCHAR;
#else
typedef unsigned short      VTWCHAR;
#endif
#ifdef UNIX_64
 typedef signed int         VTLONG;
 typedef unsigned int       VTDWORD;
 typedef long               VTOFF_T;
 typedef unsigned long      VTQWORD;
#else
 typedef signed long        VTLONG;
 typedef unsigned long      VTDWORD;
#if defined(WIN32)
   typedef unsigned __int64 VTQWORD;
   typedef __int64          VTOFF_T;
#else
   typedef unsigned long long VTQWORD;
#if defined(UNIX_ZOS) || defined(IOS)
   typedef long             VTOFF_T;
#else
   typedef long long        VTOFF_T;
#endif /* UNIX_ZOS */
#endif
#endif

typedef float               VTFLOAT;
typedef double              VTDOUBLE;
typedef void *              VTHANDLE;
typedef VTHANDLE            VTHPALETTE;

#if defined(WIN32)
 typedef int                VTBOOL;
 typedef CRITICAL_SECTION   VTCRITICAL;
#elif defined(UNIX)
# ifdef UNIX_PTHREAD
#  include <pthread.h>
   typedef pthread_mutex_t VTCRITICAL;
# else
   typedef void *          VTCRITICAL;
# endif
# if defined(DECUNIX) || defined(HPUX64) || defined(LINUX64)
   typedef short            VTBOOL;
# elif defined(_DARWIN_SOURCE)
#  ifdef _OBJC_OBJC_H_
     typedef BOOL          VTBOOL;
#  else
     typedef signed char    VTBOOL;
#  endif
# else
   typedef short            VTBOOL;
# endif
#elif defined(NLM)
 typedef short              VTBOOL;
 typedef signed long        VTCRITICAL;
#endif

typedef VTVOID *            VTLPVOID;
typedef VTBYTE *            VTLPBYTE;
typedef VTCHAR *            VTLPSTR;
typedef const VTCHAR *      VTLPCSTR;
typedef VTTCHAR *           VTLPTSTR;
typedef const VTTCHAR *     VTLPCTSTR;

typedef VTWCHAR *           VTLPWSTR;
typedef const VTWCHAR *     VTLPCWSTR;

typedef VTWORD *            VTLPWORD;

typedef VTLONG *            VTLPLONG;
typedef VTDWORD *           VTLPDWORD;

typedef VTHANDLE *          VTLPHANDLE;
typedef VTBOOL *            VTLPBOOL;




#endif /* SCCTYPE_H */
