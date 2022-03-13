#include "bfs.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

uint64_t ParseBfsDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, uint32_t blocksize, int32_t inodesize, int32_t blksperag, int32_t dirag, uint16_t dirblk, uint16_t indblk, uint64_t parinode)
{
    QString parpath = "";
    if(parinode == 0)
	parpath = "/";
    // read inode provided
    uint64_t inodeoff = ((blksperag * blocksize * dirag) + dirblk) * blocksize;
    uint64_t indexoff = blocksize * indblk;
    qDebug() << "block size:" << blocksize << "inode size:" << inodesize << "rootdirblk:" << dirblk << "rootindxblk:" << indblk;
    qDebug() << "blksperag:" << blksperag << "dirag:" << dirag;
    qDebug() << "inodeoff:" << inodeoff << "indexoff:" << indexoff;
    if(qFromLittleEndian<int32_t>(curimg->ReadContent(curstartsector*512 + inodeoff, 4)) != 0x3bbe0ad9) // not an inode
    {
	qDebug() << "inode is not valid, don't continue, exit somehow.";
    }
    else
    {
    qDebug() << "get properties here and node information for populating tree... including layout";
    // go to data and parse directory btree to get the children.
    }
}
