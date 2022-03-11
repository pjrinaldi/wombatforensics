#ifndef BFS_H
#define BFS_H

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "commonfs.h"

// might need parentinode in here so i can if else root dir...
//uint64_t ParseBfsDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, uint64_t parinode);
uint64_t ParseBfsDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, uint32_t blocksize, int32_t inodesize, int32_t blocksperag, int32_t dirag, uint16_t dirblk, uint16_t indblk, uint64_t parinode);
#endif // bfs.h
