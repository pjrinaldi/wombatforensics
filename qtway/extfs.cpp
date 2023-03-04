#include "extfs.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

quint64 ParseExtDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, quint64 curextinode, quint64 parinode, QString parfilename, QString parlayout)
{
    //qDebug() << "curextinode:" << curextinode << "parlayout:" << parlayout;
    uint32_t blocksize = 0;
    uint16_t inodesize = 0;
    uint32_t blkgrpinodecnt = 0;
    uint32_t rootinodetableaddress = 0;
    QString incompatflags = "";
    QString readonlyflags = "";
    QString inodeaddresstable = "";
    QString fstype = "";
    QString layout = "";
    float revision = 0.0;

    QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/prop");
    if(!propfile.isOpen())
	propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(propfile.isOpen())
    {
        while(!propfile.atEnd())
        {
            QString line = propfile.readLine();
	    if(line.startsWith("Block Size|"))
		blocksize = line.split("|").at(1).toUInt();
	    else if(line.startsWith("Inode Address Table|"))
		inodeaddresstable = line.split("|").at(1);
	    else if(line.startsWith("Inode Size|"))
		inodesize = line.split("|").at(1).toUInt();
	    else if(line.startsWith("Block Group Inode Count|"))
		blkgrpinodecnt = line.split("|").at(1).toUInt();
	    else if(line.startsWith("File System Type|"))
		fstype = line.split("|").at(1);
	    else if(line.startsWith("Read Only Compatible Features|"))
		readonlyflags = line.split("|").at(1);
	    else if(line.startsWith("Incompatible Features|"))
		incompatflags = line.split("|").at(1);
	    else if(line.startsWith("Layout|"))
		layout = line.split("|").at(1);
	    else if(line.startsWith("Revision Level|"))
		revision = line.split("|").at(1).toFloat();
	    if(curextinode == 2)
	    {
		if(line.startsWith("Root Inode Table Address|"))
		    rootinodetableaddress = line.split("|").at(1).toUInt();
	    }
        }
        propfile.close();
    }
    //qDebug() << "block size:" << blocksize << "inode size:" << inodesize << "block group inode count:" << blkgrpinodecnt;
    quint64 inodecnt = 0;
    uint8_t bgnumber = 0;
    quint64 inodestartingblock = 0;
    if(!parfilename.isEmpty())
	inodecnt = parinode + 1;
    for(int i=1; i <= inodeaddresstable.split(",", Qt::SkipEmptyParts).count(); i++)
    {
	if(curextinode < i*blkgrpinodecnt)
	{
	    inodestartingblock = inodeaddresstable.split(",", Qt::SkipEmptyParts).at(i-1).toULongLong();
	    bgnumber = i - 1;
	    break;
	}
    }
    //qDebug() << "block groups:" << inodeaddresstable;
    //qDebug() << "inode table starting block:" << inodestartingblock << "bgnumber:" << bgnumber;
    //qDebug() << "root inode table address:" << rootinodetableaddress;
    //qDebug() << "layout:" << layout;
    //qDebug() << "incompatflags:" << incompatflags;
    // NEED TO GET THE BLOCK LIST FOR THE CURRENT INODE SO I CAN GET IT'S CONTENTS AND PARSE THE DIRECTORY ENTRY
    // ON SUB DIRECTORIES SINCE I HAVE THE INODE'S LAYOUT, I CAN JUST READ THE LAYOUT CONTENT DIRECTLY
    quint64 relcurinode = curextinode - 1 - (bgnumber * blkgrpinodecnt);
    quint64 curoffset = curstartsector * 512 + inodestartingblock * blocksize + inodesize * relcurinode;
    QString dirlayout = "";
    
    if(parinode > 0)
    {
        dirlayout = parlayout;
        // loop over layout variable to parse the content....
    }
    else // NEED TO PARSE THE INODE TABLE FOR THE CURRENT BLOCK GROUP
    {
        QList<uint32_t> blocklist;
        blocklist.clear();
        GetContentBlocks(curimg, curstartsector, blocksize, curoffset, &incompatflags, &blocklist);
        dirlayout = ConvertBlocksToExtents(blocklist, blocksize);
        blocklist.clear();
    }
    //qDebug() << "dirlayout:" << dirlayout;
    //if(incompatflags & 0x80)
    //    out << "FS size over 2^32 blocks,";
    //if(incompatflags & 0x40)
    //    out << "Files use Extents,";
    
    uint32_t inodeflags = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 32, 4));
    
    //qDebug() << "blocklist:" << blocklist;
    //qDebug() << "dirlayout:" << dirlayout;
    for(int i=0; i < dirlayout.split(";", Qt::SkipEmptyParts).count(); i++)
    {
        quint64 curdiroffset = dirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong();
        quint64 curdirlength = dirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong();
        quint64 coffset = curstartsector * 512 + curdiroffset + 24; // SKIP THE . AND .. ENTRIES WHICH ARE ALWAYS THE 1ST TWO ENTRIES AND ARE 12 BYTES LONG EACH
	//qDebug() << "initial coffset:" << coffset;
        if(inodeflags & 0x1000) // hash trees in use
        {
            coffset = curstartsector * 512 + curdiroffset + 40; // THIS SHOULD ACCOUNT FOR HASH TREE DEPTH OF 0, NEED TO TEST FOR 1 - 3
            qDebug() << "cur directory inode uses hashed btree rather than linear direntry reading";
        }
        bool nextisdeleted = false;
        while(coffset < curdiroffset + curdirlength - 8)
        {
            uint16_t entrylength = 0;
            int lengthdiv = (8 + qFromLittleEndian<uint8_t>(curimg->ReadContent(coffset + 6, 1))) / 4;
            int lengthrem = (8 + qFromLittleEndian<uint8_t>(curimg->ReadContent(coffset + 6, 1))) % 4;
            int newlength = 0;
            if(lengthrem == 0)
                newlength = lengthdiv * 4;
            else
                newlength = lengthdiv * 4 + 4;
            int32_t extinode = qFromLittleEndian<int32_t>(curimg->ReadContent(coffset, 4));
            if(extinode > 0)
            {
		//qDebug() << "cur coffset:" << coffset;
		//qDebug() << "extinode:" << extinode;
                QTextStream out;
                QFile fileprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
                if(!fileprop.isOpen())
                    fileprop.open(QIODevice::Append | QIODevice::Text);
                out.setDevice(&fileprop);

		out << "EXTFS Inode|" << QString::number(extinode) << "|EXTFS inode value to locate file in the filesystem." << Qt::endl;
                QString filepath = "/";
                //quint64 parentinode = 0;
                if(!parfilename.isEmpty())
                {
                    filepath = parfilename;
                    //parentinode = parinode;
                }
                uint16_t namelength = 0;
                int filetype =  -1;
                entrylength = qFromLittleEndian<uint16_t>(curimg->ReadContent(coffset + 4, 2));
                if(incompatflags.contains("Directory Entries record file type,") || revision > 0.4)
                {
                    namelength = qFromLittleEndian<uint8_t>(curimg->ReadContent(coffset + 6, 1));
                    filetype = qFromLittleEndian<uint8_t>(curimg->ReadContent(coffset + 7, 1));
                }
                else
                {
                    namelength = qFromLittleEndian<uint16_t>(curimg->ReadContent(coffset + 6, 2));
                }
		//qDebug() << "namelength:" << namelength;
		//qDebug() << "newlength:" << newlength << "entrylength:" << entrylength << "namelength:" << QString::number(namelength, 16);
                QString filename = QString::fromStdString(curimg->ReadContent(coffset + 8, namelength).toStdString()).toLatin1();
                //qDebug() << "filename:" << filename;
                uint8_t isdeleted = 0;
                if(nextisdeleted)
                    isdeleted = 1;
                if(newlength < entrylength)
                    nextisdeleted = true;
                else
                    nextisdeleted = false;
                //itemtype = itemnode->itemtype; // node type 5=file, 3=dir, 4-del file, 10=vir file, 11=vir dir, -1=not file (evid image, vol, part, fs), 15=carved file
                uint8_t itemtype = 0;
                if(filetype == 0) // unknown type
                    itemtype = 0;
                else if(filetype == 1) // FILE
                {
                    itemtype = 5;
                    if(isdeleted == 1)
                        itemtype = 4;
                }
                else if(filetype == 2) // DIR
                {
                    itemtype = 3;
                    if(isdeleted == 1)
                        itemtype = 2;
                }
                else if(filetype == 3) // CHARACTER
                    itemtype = 6;
                else if(filetype == 4) // BLOCK
                    itemtype = 7;
                else if(filetype == 5) // FIFO
                    itemtype = 8;
                else if(filetype == 6) // UNIX SOCKET
                    itemtype = 9;
                else if(filetype == 7) // SYMBOLIC LINK
                    itemtype = 12;
		//qDebug() << "itemtype:" << itemtype;
                // DETERMINE WHICH BLOCK GROUP # THE CURINODE IS IN SO I CAN READ IT'S INODE'S CONTENTS AND GET THE NECCESARY METADATA
                quint64 curinodetablestartblock = 0;
                uint8_t blockgroupnumber = 0;
		//qDebug() << "inodeaddresstable:" << inodeaddresstable;
                for(int j=1; j <= inodeaddresstable.split(",", Qt::SkipEmptyParts).count(); j++)
                {
                    if(extinode < j * blkgrpinodecnt)
                    {
                        curinodetablestartblock = inodeaddresstable.split(",", Qt::SkipEmptyParts).at(j-1).toULongLong();
                        blockgroupnumber = j - 1;
                        break;
                    }
                }
                //qDebug() << "extinode:" << extinode << "block group number:" << blockgroupnumber << "curinodetablestartblock:" << curinodetablestartblock;
                quint64 logicalsize = 0;
                quint64 curinodeoffset = curstartsector * 512 + curinodetablestartblock * blocksize + inodesize * (extinode - 1 - blockgroupnumber * blkgrpinodecnt);
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
		//qDebug() << "itemtype attempt 2:" << itemtype;

                // STILL NEED TO DO FILE ATTRIBUTES, EXTENDED ATTRIBUTE BLOCK
                uint16_t lowergroupid = qFromLittleEndian<uint16_t>(curimg->ReadContent(curinodeoffset + 24, 2));
                uint16_t uppergroupid = qFromLittleEndian<uint16_t>(curimg->ReadContent(curinodeoffset + 122, 2));
                uint32_t groupid = ((uint32_t)uppergroupid >> 16) + lowergroupid;
                uint16_t loweruserid = qFromLittleEndian<uint16_t>(curimg->ReadContent(curinodeoffset + 2, 2));
                uint16_t upperuserid = qFromLittleEndian<uint16_t>(curimg->ReadContent(curinodeoffset + 120, 2));
                uint32_t userid = ((uint32_t)upperuserid >> 16) + loweruserid;
		out << "uid / gid|" << QString(QString::number(userid) + " / " + QString::number(groupid)) << "|User ID and Group ID." << Qt::endl;
                uint32_t accessdate = qFromLittleEndian<uint32_t>(curimg->ReadContent(curinodeoffset + 8, 4));
                uint32_t statusdate = qFromLittleEndian<uint32_t>(curimg->ReadContent(curinodeoffset + 12, 4));
                uint32_t modifydate = qFromLittleEndian<uint32_t>(curimg->ReadContent(curinodeoffset + 16, 4));
                uint32_t deletedate = qFromLittleEndian<uint32_t>(curimg->ReadContent(curinodeoffset + 20, 4));
		if(deletedate > 0)
		    out << "Deleted Time|" << QDateTime::fromSecsSinceEpoch(deletedate, QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP") << "|Deleted time for the file." << Qt::endl;
                uint16_t linkcount = qFromLittleEndian<uint16_t>(curimg->ReadContent(curinodeoffset + 26, 2));
		out << "Link Count|" << QString::number(linkcount) << "|Number of files pointing to this file." << Qt::endl;
                uint32_t createdate = 0;
                if(fstype.startsWith("EXT4"))
                    uint32_t createdate = qFromLittleEndian<uint32_t>(curimg->ReadContent(curinodeoffset + 144, 4));
                uint32_t curinodeflags = qFromLittleEndian<uint32_t>(curimg->ReadContent(curinodeoffset + 32, 4));
		//qDebug() << "curinodeflags:" << QString::number(curinodeflags, 16);
                QString attrstr = "";
                if(curinodeflags & 0x200000)
                    attrstr += "Stores a Large Extended Attribute,";
                if(curinodeflags & 0x80000)
                    attrstr += "Uses Extents,";
                if(curinodeflags & 0x40000)
                    attrstr += "Huge File,";
                if(curinodeflags & 0x20000)
                    attrstr += "Top of Directory,";
                if(curinodeflags & 0x10000)
                    attrstr += "Synchronous Data Write,";
                if(curinodeflags & 0x8000)
                    attrstr += "File Tail not Merged,";
                if(curinodeflags & 0x4000)
                    attrstr += "File Data Written through Journal,";
                if(curinodeflags & 0x2000)
                    attrstr += "AFS Magic Directory,";
                if(curinodeflags & 0x1000)
                    attrstr += "Hashed Indexed Directory,";
                if(curinodeflags & 0x800)
                    attrstr += "Encrypted,";
                if(curinodeflags & 0x400)
                    attrstr += "No Compression,";
                if(curinodeflags & 0x200)
                    attrstr += "Has Compression in 1 or more blocks,";
                if(curinodeflags & 0x100)
                    attrstr += "Dirty Compression,";
                if(curinodeflags & 0x80)
                    attrstr += "No Update ATIME,";
                if(curinodeflags & 0x40)
                    attrstr += "dump utility ignores file,";
                if(curinodeflags & 0x20)
                    attrstr += "Append Only,";
                if(curinodeflags & 0x10)
                    attrstr += "Immutable,";
                if(curinodeflags & 0x08)
                    attrstr += "Synchronous Writes,";
                if(curinodeflags & 0x04)
                    attrstr += "Compressed,";
                if(curinodeflags & 0x02)
                    attrstr += "Preserved for un-deletion,";
                if(curinodeflags & 0x01)
                    attrstr += "Requires Secure Deletion";
		//if(curinodeflags == 0x00)
		//    attrstr = "No attributes";
		if(!attrstr.isEmpty())
		    out << "File Attributes|" << attrstr << "|Attributes list for the file." << Qt::endl;

                QList<uint32_t> curblklist;
                curblklist.clear();
                GetContentBlocks(curimg, curstartsector, blocksize, curinodeoffset, &incompatflags, &curblklist);
                //qDebug() << "curblklist:" << curblklist;
                QString curlayout = "";
                if(curblklist.count() > 0)
                    curlayout = ConvertBlocksToExtents(curblklist, blocksize);
		out << "Layout|" << curlayout << "|File layout in offset,size; format." << Qt::endl;
                //qDebug() << "Curlayout:" << curlayout;
                quint64 physicalsize = 0;
                for(int j=0; j < curlayout.split(";", Qt::SkipEmptyParts).count(); j++)
                {
                    physicalsize += curlayout.split(";", Qt::SkipEmptyParts).at(j).split(",").at(1).toULongLong();
                }
                //int phyblkcnt = physicalsize / blocksize;
                int phyremcnt = physicalsize % blocksize;
                if(phyremcnt > 0)
                    physicalsize += blocksize;
		out << "Physical Size|" << QString::number(physicalsize) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
		out << "Logical Size|" << QString::number(logicalsize) << "|Size in Bytes for the file." << Qt::endl;

		//qDebug() << "curlayout:" << curlayout;
                curblklist.clear();
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
            }
            coffset += newlength;
        }
    }
    return inodecnt;
}

void GetContentBlocks(ForImg* curimg, uint32_t curstartsector, uint32_t blocksize, quint64 curoffset, QString* incompatflags, QList<uint32_t>* blocklist)
{
    uint32_t inodeflags = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 32, 4));
    if(incompatflags->contains("Files use Extents,") && inodeflags & 0x80000) // FS USES EXTENTS AND INODE USES EXTENTS
    {
        uint16_t extententries = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 42, 2));
        uint16_t extentdepth = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 46, 2));
        //qDebug() << "extententries:" << extententries << "extentdepth:" << extentdepth;
        if(extentdepth == 0) // use ext4_extent
        {
            for(uint16_t i=0; i < extententries; i++)
            {
                uint16_t blocklength = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 56 + i*12, 2));
                uint16_t starthi = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 58 + i*12, 2));
                uint32_t startlo = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 60 + i*12, 4));
                quint64 startblock = (((uint64_t)starthi >> 32) + startlo); // block #, not bytes
                blocklist->append(startblock);
            }
        }
        else // use ext4_extent_idx
        {
	    QList<uint32_t> leafnodes;
	    leafnodes.clear();
	    for(uint16_t i=0; i < extententries; i++)
		leafnodes.append(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 56 + i*12, 4)));
	    for(int i=0; i < leafnodes.count(); i++)
	    {
		uint16_t lextententries = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector * 512 + leafnodes.at(i) * blocksize + 2, 2));
		uint16_t lextentdepth = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector * 512 + leafnodes.at(i) * blocksize + 6, 2));
		if(extentdepth == 0) // use ext4_extent
		{
		    for(uint16_t j=0; j < lextententries; j++)
		    {
			uint16_t blocklength = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector * 512 + leafnodes.at(i) * blocksize + 16 + j*12, 2));
			uint16_t starthi = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector * 512 + leafnodes.at(i) * blocksize + 18 + j*12, 2));
			uint32_t startlo = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + leafnodes.at(i) * blocksize + 20 + j*12, 4));
			quint64 startblock = (((uint64_t)starthi >> 32) + startlo); // block #, not bytes
			blocklist->append(startblock);
		    }
		}
		else // use ext4_extent_idx
		{
		    qDebug() << "repeat leafnode exercise here...";
		}
		//qDebug() << "leaf header:" << QString::number(qFromLittleEndian<uint16_t>(leafnode.mid(0, 2)), 16);
		//qDebug() << "extent entries:" << qFromLittleEndian<uint16_t>(leafnode.mid(2, 2));
		//qDebug() << "max extent entries:" << qFromLittleEndian<uint16_t>(leafnode.mid(4, 2));
		//qDebug() << "extent depth:" << qFromLittleEndian<uint16_t>(leafnode.mid(6, 2));
	    }
	    //qDebug() << "extent header:" << QString::number(qFromLittleEndian<uint16_t>(curinodebuf.mid(40, 2)), 16);
	    //qDebug() << "extent entries:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(42, 2));
	    //qDebug() << "max extent entries:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(44, 2));
	    //qDebug() << "extent depth:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(46, 2));
	    
	    //qDebug() << "ei_block:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(52, 4));
	    //qDebug() << "ei_leaf_lo:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(56, 4));
	    //qDebug() << "ei_leaf_hi:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(60, 2));
	    //qDebug() << "use extent idx";
        }
    }
    else // direct and indirect blocks
    {
	for(int i=0; i < 12; i++)
	{
	    uint32_t curdirectblock = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 40 + i*4, 4));
	    if(curdirectblock > 0)
		blocklist->append(curdirectblock);
	}
	//qDebug() << "blocklist before indirects:" << *blocklist;
	uint32_t singleindirect = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 88, 4));
	uint32_t doubleindirect = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 92, 4));
	uint32_t tripleindirect = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 96, 4));
        if(singleindirect > 0)
        {
            for(uint i=0; i < blocksize / 4; i++)
            {
                uint32_t cursingledirect = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + singleindirect * blocksize + i*4, 4));
                if(cursingledirect > 0)
                    blocklist->append(cursingledirect);
            }
        }
        if(doubleindirect > 0)
        {
            QList<uint32_t> sinlist;
            sinlist.clear();
            for(uint i=0; i < blocksize / 4; i++)
            {
                uint32_t sindirect = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + doubleindirect * blocksize + i*4, 4));
                if(sindirect > 0)
                    sinlist.append(sindirect);
            }
            for(int i=0; i < sinlist.count(); i++)
            {
                for(uint j=0; j < blocksize / 4; j++)
                {
                    uint32_t sdirect = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + sinlist.at(i) * blocksize + j*4, 4));
                    if(sdirect > 0)
                        blocklist->append(sdirect);
                }
            }
            sinlist.clear();
        }
        if(tripleindirect > 0)
        {
            QList<uint32_t> dinlist;
            QList<uint32_t> sinlist;
            dinlist.clear();
            sinlist.clear();
            for(uint i=0; i < blocksize / 4; i++)
            {
                uint32_t dindirect = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + tripleindirect * blocksize + i*4, 4));
                if(dindirect > 0)
                    dinlist.append(dindirect);
            }
            for(int i=0; i < dinlist.count(); i++)
            {
                for(uint j=0; j < blocksize / 4; j++)
                {
                    uint32_t sindirect = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + dinlist.at(i) * blocksize + j*4, 4));
                    if(sindirect > 0)
                        sinlist.append(sindirect);
                }
                for(int j=0; j < sinlist.count(); j++)
                {
                    for(uint k=0; k < blocksize / 4; k++)
                    {
                        uint32_t sdirect = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + sinlist.at(j) * blocksize + k*4, 4));
                        if(sdirect > 0)
                            blocklist->append(sdirect);
                    }
                }
            }
        }
    }
}

