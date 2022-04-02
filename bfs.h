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

typedef struct blockrun
{
    int32_t allocgroup;
    uint16_t start;
    uint16_t len = 1;
} blockrun;

qint64 ToOffset(blockrun run, int32_t agshift, int32_t blkshift);
qint64 ToBlock(blockrun run, int32_t agshift);
blockrun ToBlockRun(off_t start, int16_t length, int32_t agshift);
blockrun Run(int32_t group, uint16_t start, uint16_t length);

uint64_t ParseBfsDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, uint32_t blocksize, uint32_t blockshift, int32_t inodesize, int32_t blocksperag, int32_t allocshift, int32_t agcnt, int32_t dirag, uint16_t dirblk, int32_t indag, uint16_t indblk, uint64_t parinode, blockrun rootblkrun);
uint64_t ParseInode(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, uint32_t blocksize, uint32_t blockshift, int32_t inodesize, int32_t blocksperag, int32_t allocshift, QString filename, uint64_t inodeoff, uint64_t inodecnt);

#endif // bfs.h
