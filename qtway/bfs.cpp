#include "bfs.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

qint64 ToOffset(blockrun run, int32_t agshift, int32_t blkshift)
{
    return ToBlock(run, agshift) << blkshift; 
}

qint64 ToBlock(blockrun run, int32_t agshift)
{
    return ((((off_t)run.allocgroup) << agshift) | (off_t)run.start);
}

// to block run from byte offset and content length
blockrun ToBlockRun(off_t start, int16_t length, int32_t agshift)
{
	blockrun run;
	run.allocgroup = start >> agshift;
	run.start = start & ((1UL << agshift) - 1);
	run.len = length;

	return run;
}

blockrun Run(int32_t group, uint16_t start, uint16_t length)
{
	blockrun run;
	run.allocgroup = group;
	run.start = start;
	run.len = length;
	
        return run;
}

uint64_t ParseBfsDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, uint32_t blocksize, uint32_t blockshift, int32_t inodesize, int32_t blksperag, int32_t allocshift, int32_t agcnt, int32_t dirag, uint16_t dirblk, int32_t indag, uint16_t indblk, uint64_t parinode, blockrun rootblkrun)
{
    qDebug() << "blockshift:" << blockshift << "allocshift:" << allocshift;
    qDebug() << "alloc group count:" << agcnt;
    blockrun rootrun;
    rootrun.allocgroup = dirag;
    rootrun.start = dirblk;
    rootrun.len = 1;
    uint64_t inodeblock = ToBlock(rootrun, allocshift);
    uint64_t inodeoff = ToOffset(rootrun, blockshift, allocshift);
    qDebug() << "root offset:" << inodeoff << "root block:" << inodeblock;
    quint64 inodecnt = 0;
    QString parpath = "";
    if(parinode == 0)
	parpath = "/";
    // read inode provided
    blockrun indxrun;
    indxrun.allocgroup = indag;
    indxrun.start = indblk;
    indxrun.len = 1;
    //uint64_t inodeoff = curstartsector*512 + (blksperag * blocksize * dirag) + (dirblk * blocksize);
    //uint64_t indexoff = blksperag * blocksize * indag + blocksize * indblk;
    uint64_t indexoff = ToOffset(indxrun, blockshift, allocshift);
    uint64_t indexblk = ToBlock(indxrun, allocshift);
    qDebug() << "block size:" << blocksize << "inode size:" << inodesize << "rootdirag:" << dirag << "rootdirblk:" << dirblk << "rootindxag:" << indag << "rootindxblk:" << indblk;
    qDebug() << "blksperag:" << blksperag << "dirag:" << dirag;
    qDebug() << "inodeoff:" << inodeoff << "indexoff:" << indexoff << "indexblk:" << indexblk;
    if(qFromLittleEndian<int32_t>(curimg->ReadContent(curstartsector*512 + inodeoff, 4)) != 0x3bbe0ad9) // not an inode
    {
	qDebug() << "inode is not valid.";
        //qDebug() << "if rootdir, then use indexoff.";
        //if(parinode == 0)
        //    inodeoff = indexoff;
    }
    /*
    if(qFromLittleEndian<int32_t>(curimg->ReadContent(curstartsector*512 + inodeoff, 4)) != 0x3bbe0ad9) // not an inode
    {
        qDebug() << "index offset is still not a valid inode, so exit gracefully here...";
    }
    */
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
        QList<int32_t> directag;
        QList<uint16_t> directblk;
        QList<uint16_t> directlen;
        directag.clear();
        directblk.clear();
        directlen.clear();
        // type not used, so skip 4 bytes (60, 4)
        // inode size is repetitive, so skip 4 bytes (64, 4)
        // binode_etc* etc, i think it's 4 bytes (68, 4)
        for(int i=0; i < 12; i++)
        {
            int32_t curag = qFromLittleEndian<int32_t>(curimg->ReadContent(inodeoff + 72 + i*8, 4));
            uint16_t curblk = qFromLittleEndian<uint16_t>(curimg->ReadContent(inodeoff + 76 + i*8, 2));
            uint16_t curlen = qFromLittleEndian<uint16_t>(curimg->ReadContent(inodeoff + 78 + i*8, 2));
            if(curag == 0x00000000 && curblk == 0x0000 && curlen == 0x0000)
                break;
            directag.append(curag);
            directblk.append(curblk);
            directlen.append(curlen);
        }
        qDebug() << "directag:" << directag << "directblk:" << directblk << "directlen:" << directlen;
        //uint64_t maxdirectrange = qFromLittleEndian<uint64_t>(curimg->ReadContent(inodeoff + 168, 8));
        //qDebug() << "max direct range:" << maxdirectrange;
        int32_t indirectag = qFromLittleEndian<int32_t>(curimg->ReadContent(inodeoff + 176, 4));
        uint16_t indirectblk = qFromLittleEndian<uint16_t>(curimg->ReadContent(inodeoff + 180, 2));
        uint16_t indirectlen = qFromLittleEndian<uint16_t>(curimg->ReadContent(inodeoff + 182, 2));
        if(indirectag == 0x00000000 && indirectblk == 0x0000 && indirectlen == 0x0000)
        {
            // doesn't exist, skip
        }
        else
        {
            qDebug() << "parse this to get the next set of direct blocks out of the indirect block";
        }
        int32_t dindirectag = qFromLittleEndian<int32_t>(curimg->ReadContent(inodeoff + 192, 4));
        uint16_t dindirectblk = qFromLittleEndian<uint16_t>(curimg->ReadContent(inodeoff + 196, 2));
        uint16_t dindirectlen = qFromLittleEndian<uint16_t>(curimg->ReadContent(inodeoff + 198, 2));
        if(dindirectag == 0x00000000 && dindirectblk == 0x0000 && dindirectlen == 0x0000)
        {
            // doesn't exist, skip
        }
        else
        {
            qDebug() << "parse this to get the next set of indirect blocks to then get the direct blocks out of the double indirect block";
        }
        uint64_t datasize = qFromLittleEndian<uint64_t>(curimg->ReadContent(inodeoff + 208, 8));
        qDebug() << "data size:" << datasize;
        // NEXT IS THE SMALL DATA WHICH I NEED TO FIUGRE OUT HOW TO CLASSIFY, EITHER A SINGLE ADS TYPE OR SPLIT IT UP INTO
        // ITS RESPECTIVE ATTRIBUTES AND LIST ACCORDINGLY AS CHILDREN...
        // NOW I NEED TO BUILD THE LAYOUT
        QString layout = "";
        for(int i=0; i < directblk.count(); i++)
        {
            blockrun currun;
            currun.allocgroup = directag.at(i);
            currun.start = directblk.at(i);
            currun.len = directlen.at(i);
            int64_t curoff = ToOffset(currun, blockshift, allocshift);
            //int64_t curoff = (directag.at(i) * blksperag * blocksize) + directblk.at(i) * blocksize;
            uint64_t curlen = directlen.at(i) * blocksize;
            qDebug() << "curoff:" << curoff << "curlen:" << curlen;
        }
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
        // NOW SKIP TO THE END OF THE INODE OFFSET AND BEGIN TO PARSE THE BTREE WITH THE HEADER NODE
        uint64_t btreeoff = inodeoff + datasize;
        qDebug() << "btreeoff:" << btreeoff;
        int32_t btreeheadmagic = qFromLittleEndian<int32_t>(curimg->ReadContent(btreeoff, 4));
        int32_t btreenodesize = qFromLittleEndian<int32_t>(curimg->ReadContent(btreeoff + 4, 4));
        int32_t btreemaxlevels = qFromLittleEndian<int32_t>(curimg->ReadContent(btreeoff + 8, 4));
        int32_t btreedatatype = qFromLittleEndian<int32_t>(curimg->ReadContent(btreeoff + 12, 4));
        uint64_t btreerootnodeptr = qFromLittleEndian<uint64_t>(curimg->ReadContent(btreeoff + 16, 8));
        uint64_t btreefreenodeptr = qFromLittleEndian<uint64_t>(curimg->ReadContent(btreeoff + 24, 8));
        uint64_t btreemaxsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(btreeoff + 32, 8));
        qDebug() << "magic:" << btreeheadmagic << "nodesize:" << btreenodesize << "maxlevels:" << btreemaxlevels;
        qDebug() << "datatype:" << btreedatatype << "rootnodeptr:" << btreerootnodeptr << "freenodeptr:" << btreefreenodeptr;
        qDebug() << "maxsize:" << btreemaxsize;
        // NOW SKIP TO THE ROOT NODE PTR TO GET THE KEY/VAL PAIRS FOR THE ROOT DIRECTORY NODE...
        uint64_t rnodeoff = btreeoff + btreerootnodeptr;
        uint64_t leftlink = qFromLittleEndian<uint64_t>(curimg->ReadContent(rnodeoff, 8));
        uint64_t rghtlink = qFromLittleEndian<uint64_t>(curimg->ReadContent(rnodeoff + 8, 8));
        uint64_t oflowlnk = qFromLittleEndian<uint64_t>(curimg->ReadContent(rnodeoff + 16, 8));
        int16_t keycnt = qFromLittleEndian<int16_t>(curimg->ReadContent(rnodeoff + 24, 2));
        int16_t keylen = qFromLittleEndian<int16_t>(curimg->ReadContent(rnodeoff + 26, 2));
        uint64_t rvaloff = rnodeoff + 28 + keylen + keycnt * 2;
        QList<int16_t> keyidxlist;
        keyidxlist.clear();
        keyidxlist.append(0);
        for(int i=0; i < keycnt; i++)
        {
            keyidxlist.append(qFromLittleEndian<int16_t>(curimg->ReadContent(rnodeoff + 28 + keylen + i*2, 2)));
        }
        //keyidxlist.append(keylen);
        //qDebug() << "rnodeoff:" << rnodeoff;
        //qDebug() << "keycnt:" << keycnt << "keylen:" << keylen;
        //qDebug() << "keyidxlist:" << keyidxlist;
        //qDebug() << "rvaloff:" << rvaloff;
        QHash<QString, uint64_t> itemlist;
        itemlist.clear();
        qDebug() << "keycnt:" << keycnt;
        for(int i=0; i < keycnt; i++)
        {
            QString tmpstring = QString::fromStdString(curimg->ReadContent(rnodeoff + 28 + keyidxlist.at(i), keyidxlist.at(i+1) - keyidxlist.at(i)).toStdString());
            //blockrun tmprun;
            //tmprun.allocgroup = qFromLittleEndian<uint32_t>(curimg->ReadContent(rvaloff + i*8, 4));
            //tmprun.start = qFromLittleEndian<uint16_t>(curimg->ReadContent(rvaloff + i*8 + 4, 2));
            //tmprun.len = qFromLittleEndian<uint16_t>(curimg->ReadContent(rvaloff + i*8 + 6, 2));
            uint64_t tmpoff = qFromLittleEndian<uint64_t>(curimg->ReadContent(rvaloff + i*8, 8));
            if(tmpstring != "." && tmpstring != "..")
                itemlist.insert(tmpstring, tmpoff);
            qDebug() << "File Name, Offset, for:" << i << tmpstring << tmpoff;
        }
        qDebug() << "itemlist:" << itemlist;
        QHashIterator<QString, uint64_t> i(itemlist);
        while(i.hasNext())
        {
            i.next();
            ParseInode(curimg, curstartsector, ptreecnt, blocksize, blockshift, inodesize, blksperag, allocshift, i.key(), i.value(), inodecnt);
        }
        // this is the list of inode blocks for each directory entry.. parsing these will tell me if it is a directory or file and
        // stuff about it, so the recursive function should be ParseBfsInode()...
        // int64_t array of value for each key...
    }
    return inodecnt;
}

uint64_t ParseInode(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, uint32_t blocksize, uint32_t blockshift, int32_t inodesize, int32_t blocksperag, int32_t allocshift, QString filename, uint64_t inodeoff, uint64_t inodecnt)
{
    qDebug() << "inode name:" << filename << "inodeoff:" << inodeoff * blocksize;
    blockrun tmprun = ToBlockRun(inodeoff, 1, allocshift);
    qDebug() << "tmprun alloc|start|len:" << tmprun.allocgroup << tmprun.start << tmprun.len;
    qDebug() << "inode offset:" << ToOffset(tmprun, allocshift, blockshift);
    return inodecnt;
}
