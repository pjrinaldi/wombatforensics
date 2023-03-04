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
    // root inode is 128, so 128/8 puts us in block 16...
    uint64_t rootinodeoff = curstartsector*512 + (rootinode / inodesperblock) * blocksize;
    qDebug() << "root inode off:" << rootinodeoff;
    uint16_t inodemagic = qFromBigEndian<uint16_t>(curimg->ReadContent(rootinodeoff, 2));
    if(inodemagic == 0x494e) // xfs inode | IN
    {
        // start parsing inode structure...
        uint16_t mode = qFromBigEndian<uint16_t>(curimg->ReadContent(rootinodeoff + 2, 2));
        int8_t version = qFromBigEndian<int8_t>(curimg->ReadContent(rootinodeoff + 4, 1));
        int8_t format = qFromBigEndian<int8_t>(curimg->ReadContent(rootinodeoff + 5, 1));
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
        quint64 fullinode = qFromBigEndian<quint64>(curimg->ReadContent(rootinodeoff + 152, 8));
        qDebug() << "filesize:" << filesize << "nblocks:" << nblocks << "numextents:" << numextents << "extattrfork:" << extattrfork;
        qDebug() << "full inode:" << fullinode;
        qDebug() << "mode:" << QString::number(mode, 16) << "format:" << format << "attrforkformat:" << attrforkformat;
        //bool isdir = false;
        uint8_t itemtype = 0;
        uint8_t isdeleted = 0;
        uint8_t fmt = 0;
        //if(format & 0) // device
        //    qDebug() << "block and character devices";
        //  FORMAT & IS WRONG, NEED TO WORK ON THIS TO GET IT RIGHT...
        if(format & 1)
        {
            fmt += 1;
            qDebug() << "local";
        }
        if(format & 2)
        {
            fmt += 2;
            qDebug() << "extents";
        }
        if(format & 3)
        {
            fmt += 4;
            qDebug() << "btree";
        }
        if(format & 4)
        {
            fmt += 8;
            qDebug() << "uuid";
        }
        if(format & 5)
        {
            fmt += 16;
            qDebug() << "reverse mapping b+tree rooted in the fork.";
        }
        qDebug() << "fmt:" << QString::number(fmt, 2) << QString::number(fmt, 16) << fmt;
        QString filemodestr = "---------";
        if(mode & 0xc000) // unix socket
        {
            filemodestr.replace(0, 1, "s");
            itemtype = 9;
        }
        if(mode & 0xa000) // symbolic link
        {
            filemodestr.replace(0, 1, "l");
            itemtype = 12;
        }
        if(mode & 0x6000) // block device
        {
            filemodestr.replace(0, 1, "b");
            itemtype = 7;
        }
        if(mode & 0x2000) // char device
        {
            filemodestr.replace(0, 1, "c");
            itemtype = 6;
        }
        if(mode & 0x1000) // FIFO (pipe)
        {
            filemodestr.replace(0, 1, "p");
            itemtype = 8;
        }
        if(mode & 0x8000) // regular file
        {
            /*
            if(readonlyflags.contains("Allow storing files larger than 2GB,")) // LARGE FILE SUPPORT
            {
                uint32_t lowersize = qFromLittleEndian<uint32_t>(curimg->ReadContent(curinodeoffset + 4, 4));
                uint32_t uppersize = qFromLittleEndian<uint32_t>(curimg->ReadContent(curinodeoffset + 108, 4));
                logicalsize = ((quint64)uppersize >> 32) + lowersize;
            }
            else
            {
                logicalsize = qFromLittleEndian<uint32_t>(curimg->ReadContent(curinodeoffset + 4, 4));
            }
            */
            filemodestr.replace(0, 1, "-");
            itemtype = 5;
            if(isdeleted == 1)
                itemtype = 4;
        }
        else if(mode & 0x4000) // directory
        {
            //isdir = true;
            //logicalsize = qFromLittleEndian<uint32_t>(curimg->ReadContent(curinodeoffset + 4, 4));
            filemodestr.replace(0, 1, "d");
            itemtype = 3;
            if(isdeleted == 1)
                itemtype = 2;
        }
        if(mode & 0x100) // user read
            filemodestr.replace(1, 1, "r");
        if(mode & 0x080) // user write
            filemodestr.replace(2, 1, "w");
        if(mode & 0x040) // user execute
            filemodestr.replace(3, 1, "x");
        if(mode & 0x020) // group read
            filemodestr.replace(4, 1, "r");
        if(mode & 0x010) // group write
            filemodestr.replace(5, 1, "w");
        if(mode & 0x008) // group execute
            filemodestr.replace(6, 1, "x");
        if(mode & 0x004) // other read
            filemodestr.replace(7, 1, "r");
        if(mode & 0x002) // other write
            filemodestr.replace(8, 1, "w");
        if(mode & 0x001) // other execute
            filemodestr.replace(9, 1, "x");
        qDebug() << "filemodestr:" << filemodestr;
        uint64_t dataforkoff = rootinodeoff + 176;
        if(itemtype == 5) // regular file
        {
            /*
            qDebug() << "regular file";
            // check format
            if(fmt & 2)
                qDebug() << "extents";
            if(fmt & 4)
                qDebug() << "btree";
            if(fmt & 16)
                qDebug() << "reverse map";
            */
        }
        else if(itemtype == 3) // directory
        {
            if(fmt == 21) // local,btree,reversemap - short form directory, xfs_dir2_sf_t
            {
                uint8_t entrycount = qFromBigEndian<uint8_t>(curimg->ReadContent(dataforkoff, 1));
                uint8_t entry64cnt = qFromBigEndian<uint8_t>(curimg->ReadContent(dataforkoff + 1, 1));
                uint32_t parinode = qFromBigEndian<uint32_t>(curimg->ReadContent(dataforkoff + 2, 4));
                uint8_t curoff = 6;
                while(curoff < filesize)
                {
                    qDebug() << "curoff:" << curoff;
                    uint8_t namelen = qFromBigEndian<uint8_t>(curimg->ReadContent(dataforkoff + curoff, 1));
                    uint16_t entryoff = qFromBigEndian<uint16_t>(curimg->ReadContent(dataforkoff + curoff + 1, 2));
                    QString entryname = QString::fromStdString(curimg->ReadContent(dataforkoff + curoff + 3, namelen).toStdString());
                    uint8_t ftype = qFromBigEndian<uint8_t>(curimg->ReadContent(dataforkoff + curoff + 3 + namelen, 1)); // 1-file,2-dir,
                    uint32_t entryinode = qFromBigEndian<uint32_t>(curimg->ReadContent(dataforkoff + curoff + 4 + namelen, 4));
                    qDebug() << "name|type|inode:" << entryname << ftype << entryinode;
		    qDebug() << "entry inode offset:" << curstartsector*512 + (entryinode / inodesperblock) * blocksize;
                    curoff = curoff + 8 + namelen;
		    ParseInode(curimg, curstartsector, ptreecnt, blocksize, inodesize, inodesperblock, entryinode, parinode, "");
                }
            }
            /*
            qDebug() << "directory";
            // check format
            if(fmt & 1)
                qDebug() << "local";
            if(fmt & 2)
                qDebug() << "extents";
            if(fmt & 4)
                qDebug() << "btree";
            if(fmt & 8)
                qDebug() << "uuid";
            if(fmt & 16)
                qDebug() << "reverse map";
            */
        }
        //out << "Mode|" << filemodestr << "|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]." << Qt::endl;
        // data fork starts at byte offset 176
    }
}

void ParseInode(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, uint32_t blocksize, uint16_t inodesize, uint16_t inodesperblock, uint64_t curinode, uint64_t parinode, QString parpath)
{
    // if the curinode is 131, then we take (131 / inodesperblock) to get the inodeblock offset, then we add (131-128) * inodesize to get the curinodeoff
    uint64_t firstinode = (curinode / inodesperblock) * inodesperblock;
    qDebug() << "curinode:" << curinode << "firstinode:" << firstinode;
    uint64_t inodeoff = curstartsector*512 + (firstinode / inodesperblock) * blocksize + ((curinode - firstinode) * inodesize);
    qDebug() << "cur inode block offset:" << inodeoff;
    uint16_t inodemagic = qFromBigEndian<uint16_t>(curimg->ReadContent(inodeoff, 2));
    if(inodemagic == 0x494e) // xfs inode | IN
    {
        qDebug() << "inode offset works.";
    }
    else
        qDebug() << "inode offset fails.";
}
