#include "extfs.h"

void LoadExtDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem)
{
    // COMPATIBILITY FLAGS
    uint32_t compatflags = 0;
    std::string compatibilityflags = "";
    ReadForImgContent(currentitem->forimg, &compatflags, currentitem->voloffset + 1116);
    // INCOMPATIBLE FLAGS
    uint32_t incompatflags = 0;
    std::string incompatibleflags = "";
    ReadForImgContent(currentitem->forimg, &incompatflags, currentitem->voloffset + 1120);
    // READ ONLY FLAGS
    uint32_t roflags = 0;
    std::string readonlyflags = "";
    ReadForImgContent(currentitem->forimg, &roflags, currentitem->voloffset + 1124);
    // GROUP DESCRIPTOR SIZE
    uint16_t grpdescsize = 32;
    if(incompatflags & 0x80)
	ReadForImgContent(currentitem->forimg, &grpdescsize, currentitem->voloffset + 1278);
    // FILE SYSTEM BLOCK COUNT
    uint32_t fsblockcnt = 0;
    ReadForImgContent(currentitem->forimg, &fsblockcnt, currentitem->voloffset + 1028);
    // BLOCK GROUP BLOCK COUNT
    uint32_t blockgroupblockcount = 0;
    ReadForImgContent(currentitem->forimg, &blockgroupblockcount, currentitem->voloffset + 1056);
    // BLOCK SIZE
    uint32_t blksize = 0;
    ReadForImgContent(currentitem->forimg, &blksize, currentitem->voloffset + 1048);
    uint32_t blocksize = 1024 * pow(2, blksize);
    // INODE SIZE
    uint16_t inodesize = 0;
    ReadForImgContent(currentitem->forimg, &inodesize, currentitem->voloffset + 1112);
    // BLOCK GROUP INODE COUNT
    uint32_t blockgroupinodecount = 0;
    ReadForImgContent(currentitem->forimg, &blockgroupinodecount, currentitem->voloffset + 1064);
    // INODE ADDRESS TABLE
    uint32_t blockgroupcount = fsblockcnt / blockgroupblockcount;
    uint blkgrpcntrem = fsblockcnt % blockgroupblockcount;
    if(blkgrpcntrem > 0)
	blockgroupcount++;
    if(blockgroupcount == 0)
	blockgroupcount = 1;
    //std::string inodeaddresstable = "";
    std::vector<uint32_t> inodeaddresstable;
    inodeaddresstable.clear();
    for(uint i=0; i < blockgroupcount; i++)
    {
	uint32_t iat = 0;
	if(blocksize == 1024)
	    ReadForImgContent(currentitem->forimg, &iat, currentitem->voloffset + 2*blocksize + i * grpdescsize + 8);
	else
	    ReadForImgContent(currentitem->forimg, &iat, currentitem->voloffset + blocksize + i * grpdescsize + 8);
	inodeaddresstable.push_back(iat);
	//inodeaddresstable += std::to_string(iat);
    }
    // ROOT INODE TABLE ADDRESS
    uint32_t rootinodetableaddress = 0;
    if(blockgroupinodecount > 2)
	ReadForImgContent(currentitem->forimg, &rootinodetableaddress, currentitem->voloffset + 2056);
    else
	ReadForImgContent(currentitem->forimg, &rootinodetableaddress, currentitem->voloffset + 2088);
    // REVISION LEVEL
    uint32_t revmaj = 0;
    ReadForImgContent(currentitem->forimg, &revmaj, currentitem->voloffset + 1100);
    uint16_t revmin = 0;
    ReadForImgContent(currentitem->forimg, &revmin, currentitem->voloffset + 1086);

    // CURRENT INODE
    uint64_t currentinode = 2;
    //if(curfileitem == NULL)
	//currentinode = 2;
    //else
    if(curfileitem != NULL)
    {
	std::size_t extinodesplit = curfileitem->properties.find(">");
	std::cout << "cur item ext inode: " << std::stoull(curfileitem->properties.substr(0, extinodesplit)) << std::endl;
	currentinode = std::stoull(curfileitem->properties.substr(0, extinodesplit));
    }
    uint64_t inodestartingblock = 0;
    uint8_t blockgroupnumber = 0;
    for(int i=1; i <= inodeaddresstable.size(); i++)
    {
	if(currentinode < i*blockgroupinodecount)
	{
	    inodestartingblock = inodeaddresstable.at(i-1);
	    blockgroupnumber = i - 1;
	    break;
	}
    }
    uint64_t relativecurrentinode = currentinode - 1 - (blockgroupnumber * blockgroupinodecount);
    uint64_t currentoffset = currentitem->voloffset + inodestartingblock * blocksize + inodesize * relativecurrentinode;
    std::string currentlayout = "";
    if(curfileitem != NULL)
	currentlayout = curfileitem->layout;
    else
    {
	std::vector<uint32_t> blocklist;
	blocklist.clear();
	GetContentBlocks(currentitem, blocksize, currentoffset, incompatflags, &blocklist);
	currentlayout = ConvertBlocksToExtents(&blocklist, blocksize);
    }
    // INODE FLAGS
    uint32_t inodeflags = 0;
    ReadForImgContent(currentitem->forimg, &inodeflags, currentoffset + 32);
    // GET THE DIRECTORY CONTENT OFFSETS/LENGTHS AND THEN LOOP OVER THEM
    std::vector<std::string>layoutlist;
    layoutlist.clear();
    std::istringstream ll(currentlayout);
    std::string ls;
    while(getline(ll, ls, ';'))
	layoutlist.push_back(ls);
    for(int i=0; i < layoutlist.size(); i++)
    {
	std::size_t layoutsplit = layoutlist.at(i).find(",");
	uint64_t curoffset = std::stoull(layoutlist.at(i).substr(0, layoutsplit));
	uint64_t curlength = std::stoull(layoutlist.at(i).substr(layoutsplit+1));
	uint64_t curoff = currentitem->voloffset + curoffset + 24; // SKIP . AND .. ENTRIES WHICH ARE ALWAYS 1ST 2 AND 12 BYTES EACH
	if(inodeflags & 0x1000) // hash trees in use
	{
	    curoff = currentitem->voloffset + curoffset + 40; // THIS SHOULD ACCOUNT FOR HASH TREE DEPTH OF 0, NEED TO TEST FOR 1 - 3
	}
	bool nextisdeleted = false;
	while(curoff < curoffset + curlength - 8)
	{
	    uint16_t entrylength = 0;
	    uint8_t curlen = 0;
	    currentitem->forimg->ReadContent(&curlen, curoff + 6, 1);
	    int lengthdiv = (8 + curlen) / 4;
	    int lengthrem = (8 + curlen) % 4;
	    int newlength = 0;
	    if(lengthrem = 0)
		newlength = lengthdiv * 4;
	    else
		newlength = lengthdiv * 4 + 4;
	    int32_t extinode = 0;
	    //ReadForImgContent(currentitem->forimg, &extinode, curoff);
	    if(extinode > 0)
	    {
	    }
	}
    }
    /*
     */ 
}
/*
    //qDebug() << "blocklist:" << blocklist;
    //qDebug() << "dirlayout:" << dirlayout;
    for(int i=0; i < dirlayout.split(";", Qt::SkipEmptyParts).count(); i++)
    {
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
*/
void GetContentBlocks(CurrentItem* currentitem, uint32_t blocksize, uint64_t currentoffset, uint32_t incompatflags, std::vector<uint32_t>* blocklist)
{
    uint32_t inodeflags = 0;
    ReadForImgContent(currentitem->forimg, &inodeflags, currentoffset + 32);
    if(incompatflags & 0x40 && inodeflags & 0x80000) // FS USES EXTENTS AND INODE USES EXTENTS
    {
	uint16_t extententries = 0;
	ReadForImgContent(currentitem->forimg, &extententries, currentoffset + 42);
	uint16_t extentdepth = 0;
	ReadForImgContent(currentitem->forimg, &extentdepth, currentoffset + 46);
	if(extentdepth == 0) // use ext4_extent
	{
	    for(uint16_t i=0; i < extententries; i++)
	    {
		uint16_t blocklength = 0;
		ReadForImgContent(currentitem->forimg, &blocklength, currentoffset + 56 + i*12);
		uint16_t starthi = 0;
		ReadForImgContent(currentitem->forimg, &starthi, currentoffset + 58 + i*12);
		uint32_t startlo = 0;
		ReadForImgContent(currentitem->forimg, &startlo, currentoffset + 60 + i*12);
		uint64_t startblock = (((uint64_t)starthi >> 32) + startlo); // block #, not bytes
		blocklist->push_back(startblock);
	    }
	}
	else // use ext4_extent_idx
	{
	    std::vector<uint32_t> leafnodes;
	    leafnodes.clear();
	    for(uint16_t i=0; i < extententries; i++)
	    {
		uint32_t leafnode = 0;
		ReadForImgContent(currentitem->forimg, &leafnode, currentoffset + 56 + i*12);
		leafnodes.push_back(leafnode);
	    }
	    for(int i=0; i < leafnodes.size(); i++)
	    {
		uint16_t lextententries = 0;
		ReadForImgContent(currentitem->forimg, &lextententries, currentoffset + leafnodes.at(i) * blocksize + 2);
		uint16_t lextentdepth = 0;
		ReadForImgContent(currentitem->forimg, &lextentdepth, currentoffset + leafnodes.at(i) * blocksize + 6);
		if(lextentdepth == 0) // use ext4_extent
		{
		    for(uint16_t j=0; j < lextententries; j++)
		    {
			uint16_t blocklength = 0;
			ReadForImgContent(currentitem->forimg, &blocklength, currentoffset + leafnodes.at(i) * blocksize + 16 + j*12);
			uint16_t starthi = 0;
			ReadForImgContent(currentitem->forimg, &starthi, currentoffset + leafnodes.at(i) * blocksize + 18 + j*12);
			uint32_t startlo = 0;
			ReadForImgContent(currentitem->forimg, &startlo, currentoffset + leafnodes.at(i) * blocksize + 20 + j*12);
			uint64_t startblock = (((uint64_t)starthi >> 32) + startlo);
			blocklist->push_back(startblock);
		    }
		}
		else // use ext4_extent_idx
		{
		    std::cout << "repeat leafnode exercise here...";
		}
	    }
	}
    }
    else // direct and indirect blocks
    {
	for(int i=0; i < 12; i++)
	{
	    uint32_t curdirectblock = 0;
	    ReadForImgContent(currentitem->forimg, &curdirectblock, currentoffset + 40 + i*4);
	    if(curdirectblock > 0)
		blocklist->push_back(curdirectblock);
	}
	uint32_t singleindirect = 0;
	ReadForImgContent(currentitem->forimg, &singleindirect, currentoffset + 88);
	uint32_t doubleindirect = 0;
	ReadForImgContent(currentitem->forimg, &doubleindirect, currentoffset + 92);
	uint32_t tripleindirect = 0;
	ReadForImgContent(currentitem->forimg, &tripleindirect, currentoffset + 96);
	if(singleindirect > 0)
	{
	    for(uint i=0; i < blocksize / 4; i++)
	    {
		uint32_t cursingledirect = 0;
		ReadForImgContent(currentitem->forimg, &cursingledirect, currentoffset + singleindirect * blocksize + i*4);
		if(cursingledirect > 0)
		    blocklist->push_back(cursingledirect);
	    }
	}
	if(doubleindirect > 0)
	{
	    std::vector<uint32_t> sinlist;
	    sinlist.clear();
	    for(uint i=0; i < blocksize / 4; i++)
	    {
		uint32_t sindirect = 0;
		ReadForImgContent(currentitem->forimg, &sindirect, currentoffset + doubleindirect * blocksize + i*4);
		if(sindirect > 0)
		    sinlist.push_back(sindirect);
	    }
	    for(int i=0; i < sinlist.size(); i++)
	    {
		for(uint j=0; j < blocksize / 4; j++)
		{
		    uint32_t sdirect = 0;
		    ReadForImgContent(currentitem->forimg, &sdirect, currentoffset + sinlist.at(i) * blocksize + j*4);
		    if(sdirect > 0)
			blocklist->push_back(sdirect);
		}
	    }
	    sinlist.clear();
	}
	if(tripleindirect > 0)
	{
	    std::vector<uint32_t> dinlist;
	    std::vector<uint32_t> sinlist;
	    dinlist.clear();
	    sinlist.clear();
	    for(uint i=0; i < blocksize / 4; i++)
	    {
		uint32_t dindirect = 0;
		ReadForImgContent(currentitem->forimg, &dindirect, currentoffset + tripleindirect * blocksize + i*4);
		if(dindirect > 0)
		    dinlist.push_back(dindirect);
	    }
	    for(int i=0; i < dinlist.size(); i++)
	    {
		for(uint j=0; j < blocksize / 4; j++)
		{
		    uint32_t sindirect = 0;
		    ReadForImgContent(currentitem->forimg, &sindirect, currentoffset + dinlist.at(i) * blocksize + j*4);
		    if(sindirect > 0)
			sinlist.push_back(sindirect);
		}
		for(int j=0; j < sinlist.size(); j++)
		{
		    for(uint k=0; k < blocksize / 4; k++)
		    {
			uint32_t sdirect = 0;
			ReadForImgContent(currentitem->forimg, &sdirect, currentoffset + sinlist.at(j) * blocksize + k*4);
			if(sdirect > 0)
			    blocklist->push_back(sdirect);
		    }
		}
	    }
	}
    }
}

std::string ConvertBlocksToExtents(std::vector<uint32_t>* blocklist, uint32_t blocksize)
{
    std::string layout = "";
    int blockcount = 1;
    unsigned int startvalue = blocklist->at(0);
    for(int i=1; i < blocklist->size(); i++)
    {
	unsigned int oldvalue = blocklist->at(i-1);
	unsigned int newvalue = blocklist->at(i);
	if(newvalue - oldvalue == 1)
	    blockcount++;
	else
	{
	    layout += std::to_string(startvalue * blocksize) + "," + std::to_string(blockcount * blocksize) + ";";
	    startvalue = blocklist->at(i);
	    blockcount = 1;
	}
	if(i == blocklist->size() - 1)
	{
	    layout += std::to_string(startvalue * blocksize) + "," + std::to_string(blockcount * blocksize) + ";";
	    startvalue = blocklist->at(i);
	    blockcount = 1;
	}
    }
    if(blocklist->size() == 1)
    {
	layout += std::to_string(startvalue * blocksize) + "," + std::to_string(blockcount * blocksize) + ";";
    }

    return layout;
}
