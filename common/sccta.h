/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved. */
/*
|   Text Access
|   Include File sccta.h
|
|   TTTTT    A
|     T     A A
|     T    A   A
|     T    AAAAA
|     T    A   A
|     T    A   A
|
|
*/

#ifndef _SCCTA_H
#define _SCCTA_H

#ifdef WINDOWS
#ifndef SCC_PACK_SET
#define SCC_PACK_SET 1
#define SCC_PACKED_BY_SCCTA_H 1
#pragma pack(push,8)
#endif /* SCC_PACK_SET */
#endif /* WINDOWS */

#include <sccda.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
|  T Y P E D E F S
*/

typedef VTHDOC   VTHTEXT;
typedef VTLPHDOC VTLPHTEXT;


/*
|  P R O T O T Y P E S
*/
DA_ENTRYSC DAERR DA_ENTRYMOD TAOpenText(VTHDOC hDoc,
    VTLPHTEXT phText);
DA_ENTRYSC DAERR DA_ENTRYMOD TACloseText(
    VTHTEXT hText);
DA_ENTRYSC DAERR DA_ENTRYMOD TAReadFirst(
    VTHTEXT   hText,
    VTLPBYTE  lpBuf,
    VTDWORD   dwBufSize,
    VTLPDWORD lpRet);
DA_ENTRYSC DAERR DA_ENTRYMOD TAReadNext(
    VTHTEXT    hText,
    VTLPBYTE   pText,
    VTDWORD    dwBufSize,
    VTLPDWORD  lpRetCount);

#ifdef __cplusplus
}
#endif

#ifdef SCC_PACKED_BY_SCCTA_H
#pragma pack(pop)
#undef SCC_PACKED_BY_SCCTA_H
#undef SCC_PACK_SET
#endif /* SCC_PACKED_BY_SCCTA_H */

#endif /* SCCTA_H */
