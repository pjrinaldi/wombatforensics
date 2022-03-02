#ifndef APFS_H
#define APFS_H

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "commonfs.h"

void ParseApfsVolumes(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt);
uint64_t ReturnBTreeLayout(ForImg* curimg, uint32_t curstartsector, uint32_t blocksize, uint64_t objectmapoid, uint64_t roottreeoid);
//QString ReturnBTreeLayout(ForImg* curimg, uint32_t curstartsector, uint32_t blocksize, uint64_t objectmapoid, uint64_t roottreeoid);
void ParseApfsDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, int volid, uint32_t blocksize, QString dirlayout);
//qulonglong ParseHfsPlusDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt);
//void BuildPath(QString* path, QHash<uint32_t, QString>* dirhash, quint64 curparcnid);

bool CheckChecksum(ForImg* curimg, uint64_t blockbyteoffset, uint32_t size, uint64_t curchecksum);

typedef int64_t paddr_t;

struct prange
{
    paddr_t     startpaddr;
    uint64_t    blockcount;
};
typedef struct prange prange_t;

typedef unsigned char uuid_t[16];
typedef uint64_t oid_t;
typedef uint64_t xid_t;

struct obj_phys
{
    uint8_t checksum[8];
    oid_t   oid;
    xid_t   xid;
    uint16_t type;
    uint16_t flags;
    uint32_t subtype;
};
typedef struct obj_phys obj_phys_t;

#endif // apfs.h
