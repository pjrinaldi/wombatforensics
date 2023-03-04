#ifndef XFS_H
#define XFS_H

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "commonfs.h"

void ParseXfs(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, uint32_t blocksize, uint16_t inodesize, uint16_t inodesperblock, uint32_t agblocks, uint32_t agcount, uint64_t rootinode);
void ParseInode(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, uint32_t blocksize, uint16_t inodesize, uint16_t inodesperblock, uint64_t curinode, uint64_t parinode, QString parpath);

#endif // xfs.h