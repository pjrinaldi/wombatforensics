#include "hfsplus.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

qulonglong ParseHfsPlusDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, qulonglong parinode, QString parfilename)
{
    quint64 inodecnt = 0;
    uint32_t clustersize = 0;
    quint64 catalogsize = 0;
    uint32_t catalogtotalblocks = 0;
    QString catalogextstartblockarray = "";
    QString catalogextblockcntarray = "";
    // let's figure out how to parse the root directory...
    QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/prop");
    if(!propfile.isOpen())
	propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(propfile.isOpen())
    {
        while(!propfile.atEnd())
        {
            QString line = propfile.readLine();
            if(line.startsWith("Cluster Size|"))
                clustersize = line.split("|").at(1).toUInt();
            if(line.startsWith("Catalog Logical Size|"))
                catalogsize = line.split("|").at(1).toULongLong();
            if(line.startsWith("Catalog Total Blocks|"))
                catalogtotalblocks = line.split("|").at(1).toULongLong(); 
            if(line.startsWith("Catalog Extents Start Block Array|"))
                catalogextstartblockarray = line.split("|").at(1);
            if(line.startsWith("Catalog Extents Block Count Array|"))
                catalogextblockcntarray = line.split("|").at(1);
        }
        propfile.close();
    }
    /*
    qDebug() << "cluster size:" << clustersize;
    qDebug() << "catalogs logical size:" << catalogsize;
    qDebug() << "catalogtotalblocks:" << catalogtotalblocks;
    qDebug() << "catalogextstartblockarray:" << catalogextstartblockarray;
    qDebug() << "catalogextblockcntarray:" << catalogextblockcntarray;
    qDebug() << "start block cnt:" << catalogextstartblockarray.split(",", Qt::SkipEmptyParts).count();
    */

    uint16_t nodesize = 0;
    quint64 curoffset = 0;
    quint64 curblocksize = 0;
    curoffset = curstartsector*512;
    QStringList catalogblocklist = catalogextstartblockarray.split(",", Qt::SkipEmptyParts);
    QStringList catalogblockcntlist = catalogextblockcntarray.split(",", Qt::SkipEmptyParts);
    // catalog file can be split, so i need to parse the 
    for(int i=0; i < catalogblocklist.count(); i++)
    {
        curblocksize = catalogblockcntlist.at(i).toUInt() * clustersize;
        qDebug() << "curblocksize:" << curblocksize;
        uint32_t curpos = 0;
        // parse the catalog file by pieces here.
        if(i == 0) // header node
        {
            curoffset += catalogblocklist.at(i).toUInt() * clustersize; // curoffset at the first catalog block offset
            nodesize = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + 32, 2));
            qDebug() << "nodesize:" << nodesize;
            // NEED TO JUMP TO CATALOG ROOT DIRECTORY NODE START, WHICH IS CATALOG NODE ID (CNID) 2, SO IT IS (CNID - 1) * NODESIZE
            // SO FOR THIS EXAMPLE, IT IS (2-1) * 4096, NEED TO SET THE OFFSET TO BE 
            curoffset += nodesize; // SHOULD PUT ME AT 4096, START OF THE CATALOG ROOT DIRECTORY NODE
            // READING THE NODE DESCRIPTOR IS ONLY IN THE i=0 PORTION OF THE CATALOG ROOT DIRECTORY
            // GET THE NUMBER OF RECORDS
            uint16_t recordcount = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + 10, 2));
            qDebug() << "recordcount:" << recordcount; // MIGHT BE ANOTHER WAY TO LOOP OVER THE RECORDS, RATHER THAN THE CUROFFSET + CURPOS METHOD...
            //qDebug() << curimg->ReadContent(curoffset, 14).toHex();
            //curoffset += 14; // THIS PUTS THE CUROFFSET INTO THE KEY/DATA LOOP FOR THE CATALOG LEAF NODE, SO ITERATING SHOULD WORK HERE
            curpos += 14;
        }
        // THIS IS WHERE THE KEY/DATA LOOP ITERATION SHOULD START... WHILE (CURPOS < CURBLOCKSIZE)
        qDebug() << "curoffset:" << curoffset << "curpos:" << curpos << "curblocksize:" << curblocksize;
        while(curpos < curblocksize)
        {
            uint16_t curkeylength = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
            curpos += 2;
            if(curkeylength == 0)
            {
                qDebug() << "keylength is wrong...";
            }
            qDebug() << "curkeylength:" << curkeylength; // in case i want to skip a record
            uint32_t parentcnid = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos, 4));
            qDebug() << "parentcnid:" << parentcnid;
            curpos += 4;
            uint16_t keynamelength = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
            curpos += 2;
            qDebug() << "keynamelength:" << keynamelength;
            if(keynamelength > 0)
            {
                QString keyfilename = "";
                for(int j=0; j < keynamelength; j++)
                {
                    keyfilename += QString(QChar(qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos + j*2, 2))));
                }
                curpos += 2*keynamelength;
                qDebug() << "keyfilename:" << keyfilename;
            }
            //else // ALREADY SKIPPED KEYNAME LENGTH...
            //    curpos += 2;
            uint16_t datarecordtype = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
            curpos += 2;
            qDebug() << "datarecordtype:" << datarecordtype;
            if(datarecordtype == 0x01) // FOLDER RECORD 0x0001 - 88 bytes
            {
                uint16_t flags = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
                uint32_t valence = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 2, 4));
                uint32_t cnid = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 6, 4));
                uint32_t createdate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 10, 4));
                uint32_t contentmoddate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 14, 4));
                uint32_t attrmoddate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 18, 4));
                uint32_t accessdate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 22, 4));
                uint32_t backupdate = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 26, 4));
                uint32_t ownerid = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 30, 4));
                uint32_t groupid = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 34, 4));
                uint8_t adminflags = qFromBigEndian<uint8_t>(curimg->ReadContent(curoffset + curpos + 38, 1));
                uint8_t ownerflags = qFromBigEndian<uint8_t>(curimg->ReadContent(curoffset + curpos + 39, 1));
                uint16_t filemode = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos + 40, 2));
                uint32_t inodelinkraw = qFromBigEndian<uint32_t>(curimg->ReadContent(curoffset + curpos + 42, 4));
                qDebug() << "createdate:" << ConvertUnixTimeToString(ConvertHfsTimeToUnixTime(createdate));
                // 16 bytes for user information i currently don't care about
                // 16 bytes for finder information i currently don't care about
                // 4 bytes for text encoding i don't care about
                // 4 bytes reserved, so it doesn't matter
                qDebug() << "folder record";
                curpos += 86;
            }
            else if(datarecordtype == 0x02) // FILE RECORD 0x0002 - 248 bytes
            {
                qDebug() << "file record";
                // need to combine these two fields and split at the 86 mark and add the difference accordingly.
                // file record is the same as folder, except it has the data fork (80 bytes) and resource fork (80 bytes)...
                // resource fork should be an ads, the data fork should be the data layout variable...
                curpos += 246;
            }
            else if(datarecordtype == 0x03 || datarecordtype == 0x04) // FOLDER THREAD RECORD 0x03/ FILE THREAD RECORD 0x04 - SKIP
            {
                // keylength should be 6 bytes...
                // data length should another 6 bytes
                curpos += 6;
                uint16_t datanodesize = qFromBigEndian<uint16_t>(curimg->ReadContent(curoffset + curpos, 2));
                curpos += 2;
                qDebug() << "datanodesize:" << datanodesize;
                curpos += 2*datanodesize;
                //break;
            }
            else // SOMETHING IS WRONG PARSING CATALOG FILE
            {
                qDebug() << "error parsing catalog file!";
                break;
            }
        }
    }
}

/* EXTFS REFERENCE FOR NODE/PROPS FILE WHILE PARSING HFSPLUS
 *                
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
*/

/*
typedef struct {
    uint8_t logic_sz[8];        // The size (in bytes) of the fork //
    uint8_t clmp_sz[4];         // For "special files" in volume header, clump size.  For
                                // catalog files, this is number of blocks read or not used. //
    uint8_t total_blk[4];       // total blocks in all extents of the fork //
    hfs_ext_desc extents[8]; // 8 sets of 4 byte start block, 4 byte block count
} hfs_fork;

// structure for file data in catalog leaf records
typedef struct {
    hfs_file_fold_std std;      // standard data that files and folders share //
    hfs_fork data;              // data fork //
    hfs_fork resource;          // resource fork //
} hfs_file;
 */ 
