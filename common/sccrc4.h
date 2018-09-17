/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved. */




#ifndef _SCCRC4_H
#define _SCCRC4_H

#ifdef WINDOWS
#ifndef SCC_PACK_SET
#define SCC_PACK_SET 1
#define SCC_PACKED_BY_SCCRC4_H 1
#pragma pack(push,8)
#endif /* SCC_PACK_SET */
#endif /* WINDOWS */

#ifdef __cplusplus
   extern "C" {
#endif

typedef struct VTRC4_CTXtag
{
    VTBYTE state[256];
    VTBYTE x;
    VTBYTE y;
} VTRC4_CTX, *PVTRC4_CTX;


#ifdef __cplusplus
   }
#endif

#ifdef SCC_PACKED_BY_SCCRC4_H
#pragma pack(pop)
#undef SCC_PACKED_BY_SCCRC4_H
#undef SCC_PACK_SET
#endif /* SCC_PACKED_BY_SCCRC4_H */

#endif /* _SCCRC4_H */
