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

/*
typedef enum
{
    NX_SUPERBLOCK = 0x0001,
    BTREE = 0x0002,
    BTREE_NODE = 0x0003,
    SPACEMAN = 0x0005,
    SPACEMAN_CAB = 0x0006,
    SPACEMAN_CIB = 0x0007,
    SPACEMAN_BITMAP = 0x0008,
    OMAP = 0x000b,
    CHECKPOINT_MAP = 0x000c,
    FS = 0x000d,
    NX_REAPER = 0x0011,
    NX_REAP_LIST = 0x0012,
    EFI_JUMPSTART = 0x0014,
    NX_FUSION_WBC = 0x0016,
    NX_FUSION_WBC_LIST = 0x0017,
    ER_STATE = 0x0018,
    GBITMAP = 0x0019,
    GBITMAP_BLOCK = 0x001b,
    INVALID = 0x0000,
    TEST = 0x00ff,
    CONTAINER_KEYBAG = 0x6b657973,
    VOLUME_KEYBAG = 0x72656373
} objecttypeenum;

typedef enum
{
    VIRTUAL = 0x0000,
    EPHEMERAL = 0x8000,
    PHYSICAL = 0x4000,
    NOHEADER = 0x2000,
    ENCRYPTED  = 0x1000,
    NONPRESISTENT = 0x0800
} objecttypeflagsenum;

typedef enum
{
    SPACEMAN_FREE_QUEUE = 0x0009,
    EXTENT_LIST_TREE = 0x000a,
    FSTREE = 0x000e,
    BLOCKREFTREE = 0x000f,
    SNAPMETATREE = 0x0010,
    OMAP_SNAPSHOT = 0x0013,
    FUSION_MIDDLE_TREE = 0x0015,
    GBITMAP_TREE = 0x001a,
} objectsubtypeenum;
*/

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
