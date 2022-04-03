#include "xfs.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

/*
 * Allocation Group (ag) Layout
 * superblock               - 1 sector 512 bytes
 * ag free block info       - 1 sector 512 bytes
 * ag inode b+tree info     - 1 sector 512 bytes
 * ag internal free list    - 1 sector 512 bytes
 * root of inode b+tree     - 1 blocks blksz bytes
 * root free space b+tree   - 1 blocks blksz bytes
 * free list                - 4 blocks blksz bytes
 * inodes (64)              - 64 * inodesize bytes
 * remaining space meta/data - rest up to agblocks * blocksize
*/
void ParseXfs(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, uint32_t blocksize, uint16_t inodesize, uint16_t inodesperblock, uint32_t agblocks, uint32_t agcount, uint64_t rootinode)
{
    qDebug() << "blocksize:" << blocksize << "inodesize:" << inodesize << "inodes per block:" << inodesperblock << "alloc group blocks:" << agblocks << "alloc group count:" << agcount << "root inode:" << rootinode;
    qDebug() << "curstartsector:" << curstartsector << "ptreecnt:" << ptreecnt;
    //ParseInode(curimg, curstartsector, ptreecnt
    // root inode is 128, so 128/8 puts us in block 16...
    uint64_t rootinodeoff = curstartsector*512 + (rootinode / inodesperblock) * blocksize;
    qDebug() << "root inode off:" << rootinodeoff;
    uint16_t inodemagic = qFromBigEndian<uint16_t>(curimg->ReadContent(rootinodeoff, 2));
    if(inodemagic == 0x494e) // xfs inode | IN
    {
        // start parsing inode structure...
        uint16_t mode = qFromBigEndian<uint16_t>(curimg->ReadContent(rootinodeoff + 2, 2));
        uint8_t version = qFromBigEndian<uint8_t>(curimg->ReadContent(rootinodeoff + 4, 1));
        uint8_t format = qFromBigEndian<uint8_t>(curimg->ReadContent(rootinodeoff + 5, 1));
        uint16_t onlink = qFromBigEndian<uint16_t>(curimg->ReadContent(rootinodeoff + 6, 2));
        uint32_t uid = qFromBigEndian<uint32_t>(curimg->ReadContent(rootinodeoff + 8, 4));
        uint32_t gid = qFromBigEndian<uint32_t>(curimg->ReadContent(rootinodeoff + 12, 4));
        uint32_t nlink = qFromBigEndian<uint32_t>(curimg->ReadContent(rootinodeoff + 16, 4));
        int32_t accesstime = qFromBigEndian<int32_t>(curimg->ReadContent(rootinodeoff + 32, 4)); // +4 for nano sec
        int32_t modifytime = qFromBigEndian<int32_t>(curimg->ReadContent(rootinodeoff + 40, 4)); // +4 for nano sec
        int32_t changetime = qFromBigEndian<int32_t>(curimg->ReadContent(rootinodeoff + 48, 4)); // +4 for nano sec
        qint64 filesize = qFromBigEndian<qint64>(curimg->ReadContent(rootinodeoff + 56, 8));
        quint64 nblocks = qFromBigEndian<qint64>(curimg->ReadContent(rootinodeoff + 64, 8)); // # of blocks storing all data
        uint32_t extsize = qFromBigEndian<uint32_t>(curimg->ReadContent(rootinodeoff + 72, 4)); // not sure if i'd use
        uint32_t numextents = qFromBigEndian<uint32_t>(curimg->ReadContent(rootinodeoff + 78, 4)); // # of data extents for inode
        uint32_t numextattrextents = qFromBigEndian<uint32_t>(curimg->ReadContent(rootinodeoff + 82, 4)); // # of ext attr extents
        uint8_t extattrfork = qFromBigEndian<uint8_t>(curimg->ReadContent(rootinodeoff + 86, 1)); // # offset is # * 8
        int8_t attrforkformat = qFromBigEndian<int8_t>(curimg->ReadContent(rootinodeoff + 87, 1));
        uint32_t createtime = qFromBigEndian<uint32_t>(curimg->ReadContent(rootinodeoff + 148, 4));
        quint64 fullinode = qFromBigEndian<quint64>(curimg->ReadContent(rootinodeoff + 148, 8));
        qDebug() << "filesize:" << filesize << "nblocks:" << nblocks << "numextents:" << numextents << "extattrfork:" << extattrfork;
        qDebug() << "full inode:" << fullinode;
        qDebug() << "mode:" << mode << "format:" << format << "attrforkformat:" << attrforkformat;
        /*
            #define S_IFLNK  0120000
            #define S_IFREG  0100000
            #define S_IFBLK  0060000
            #define S_IFDIR  0040000
            #define S_IFCHR  0020000
            XFS_DINODE_FMT_DEV,             // xfs_dev_t
            XFS_DINODE_FMT_LOCAL,           // bulk data
            XFS_DINODE_FMT_EXTENTS,         // struct xfs_bmbt_rec
            XFS_DINODE_FMT_BTREE,           // struct xfs_bmdr_block
            XFS_DINODE_FMT_UUID
            XFS_DINODE_FMT_RMAP
         */ 
        // data fork starts at byte offset 176
    }
}
