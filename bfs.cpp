#include "bfs.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

uint64_t ParseBfsDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, uint32_t blocksize, int32_t inodesize, int32_t blksperag, int32_t dirag, uint16_t dirblk, uint16_t indblk, uint64_t parinode)
{
    quint64 inodecnt = 0;
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
        uint16_t bfsinode = dirblk;
        int32_t uid = qFromLittleEndian<int32_t>(curimg->ReadContent(inodeoff + 12, 4));
        int32_t gid = qFromLittleEndian<int32_t>(curimg->ReadContent(inodeoff + 16, 4));
        int32_t mode = qFromLittleEndian<int32_t>(curimg->ReadContent(inodeoff + 20, 4));
        int32_t flags = qFromLittleEndian<int32_t>(curimg->ReadContent(inodeoff + 24, 4));
        int64_t createtime = qFromLittleEndian<uint64_t>(curimg->ReadContent(inodeoff + 28, 8));
        int64_t modifytime = qFromLittleEndian<uint64_t>(curimg->ReadContent(inodeoff + 36, 8));
        int32_t parentag = qFromLittleEndian<int32_t>(curimg->ReadContent(inodeoff + 44, 4));
        uint16_t parentblk = qFromLittleEndian<uint16_t>(curimg->ReadContent(inodeoff + 48, 2));
        int32_t attrag = qFromLittleEndian<int32_t>(curimg->ReadContent(inodeoff + 52, 4));
        uint16_t attrblk = qFromLittleEndian<uint16_t>(curimg->ReadContent(inodeoff + 56, 2));
        uint16_t attrlen = qFromLittleEndian<uint16_t>(curimg->ReadContent(inodeoff + 58, 2));
        // type not used, so skip 4 bytes (60, 4)
        // inode size is repetitive, so skip 4 bytes (64, 4)
        // binode_etc* etc, i think it's 4 bytes (68, 4)
        // offset 72 - data_stream 12 direct blocks first, which is 12 sets of 8 bytes (ag, blk, len) (4, 2, 2)
        // offset 168, 8 - max_direct_range
        // offset 176, 8 - indirect block run (4, 2, 2)
        // offset 184, 8 - max_indirect_range
        // offset 192, 8 - double indirect block run (4, 2, 2)
        // offset 200, 8 - max_double_indirect_range
        // offset 208, 8 - data size (file size, logical size) - physical size should be number of blocks * blocksize
        // offset 216, 4 - int32 pad[4]
        // offset 220, 4 - small_data type
        // offset 224, 2 - small_data name_size
        // offset 226, 2 - small_data data_size
        // offset 228, ... until the end of the file


        /*
        uint16_t filemode = qFromLittleEndian<uint16_t>(curimg->ReadContent(curinodeoffset, 2));
        QString filemodestr = "---------";
        if(filemode & 0xc000) // unix socket
        {
            filemodestr.replace(0, 1, "s");
            itemtype = 9;
        }
        if(filemode & 0xa000) // symbolic link
        {
            filemodestr.replace(0, 1, "l");
            itemtype = 12;
        }
        if(filemode & 0x6000) // block device
        {
            filemodestr.replace(0, 1, "b");
            itemtype = 7;
        }
        if(filemode & 0x2000) // char device
        {
            filemodestr.replace(0, 1, "c");
            itemtype = 6;
        }
        if(filemode & 0x1000) // FIFO (pipe)
        {
            filemodestr.replace(0, 1, "p");
            itemtype = 8;
        }
        if(filemode & 0x8000) // regular file
        {
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
            filemodestr.replace(0, 1, "-");
            itemtype = 5;
            if(isdeleted == 1)
                itemtype = 4;
        }
        else if(filemode & 0x4000) // directory
        {
            logicalsize = qFromLittleEndian<uint32_t>(curimg->ReadContent(curinodeoffset + 4, 4));
            filemodestr.replace(0, 1, "d");
            itemtype = 3;
            if(isdeleted == 1)
                itemtype = 2;
        }
        if(filemode & 0x100) // user read
            filemodestr.replace(1, 1, "r");
        if(filemode & 0x080) // user write
            filemodestr.replace(2, 1, "w");
        if(filemode & 0x040) // user execute
            filemodestr.replace(3, 1, "x");
        if(filemode & 0x020) // group read
            filemodestr.replace(4, 1, "r");
        if(filemode & 0x010) // group write
            filemodestr.replace(5, 1, "w");
        if(filemode & 0x008) // group execute
            filemodestr.replace(6, 1, "x");
        if(filemode & 0x004) // other read
            filemodestr.replace(7, 1, "r");
        if(filemode & 0x002) // other write
            filemodestr.replace(8, 1, "w");
        if(filemode & 0x001) // other execute
            filemodestr.replace(9, 1, "x");
        //qDebug() << "filemodestr:" << filemodestr;
        out << "Mode|" << filemodestr << "|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]." << Qt::endl;
        */ 
        /*
        QTextStream out;
        QFile fileprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
        if(!fileprop.isOpen())
            fileprop.open(QIODevice::Append | QIODevice::Text);
        out.setDevice(&fileprop);
        QHash<QString, QVariant> nodedata;
        nodedata.clear();
        nodedata.insert("name", QByteArray(filename.toUtf8()).toBase64());
        nodedata.insert("path", QByteArray(filepath.toUtf8()).toBase64());
        nodedata.insert("size", logicalsize);
        nodedata.insert("create", createdate);
        nodedata.insert("access", accessdate);
        nodedata.insert("modify", modifydate);
        //nodedata.insert("status", "0");
        //nodedata.insert("hash", "0");
        if(logicalsize > 0) // Get Category/Signature
        {
            if(itemtype == 3 && isdeleted == 0)
            {
                nodedata.insert("cat", "Directory");
                nodedata.insert("sig", "Directory");
            }
            else
            {
                QString catsig = GenerateCategorySignature(curimg, filename, curlayout.split(";").at(0).split(",").at(0).toULongLong());
                nodedata.insert("cat", catsig.split("/").first());
                nodedata.insert("sig", catsig.split("/").last());
            }
        }
        else
        {
            nodedata.insert("cat", "Empty");
            nodedata.insert("sig", "Zero File");
        }
        //nodedata.insert("tag", "0");
        nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
        //nodedata.insert("match", 0);
        QString parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
        if(parinode > 0)
            parentstr += QString("-f" + QString::number(parinode));
        mutex.lock();
        treenodemodel->AddNode(nodedata, parentstr, itemtype, isdeleted);
        mutex.unlock();
        if(nodedata.value("id").toString().split("-").count() == 3)
        {
            listeditems.append(nodedata.value("id").toString());
            filesfound++;
            isignals->ProgUpd();
        }
        inodecnt++;
        nodedata.clear();
        out.flush();
        fileprop.close();
        if(filemode & 0x4000) // directory, so recurse
        {
            //qDebug() << "sub dir, should recurse...";
            inodecnt = ParseExtDirectory(curimg, curstartsector, ptreecnt, extinode, inodecnt - 1, QString(filepath + filename + "/"), curlayout);
        }
        */
    }
    return inodecnt;
}
