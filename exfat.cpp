#include "exfat.h"

void LoadExFatDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem)
{
    //if(curfileitem != NULL)
    //    std::cout << "curfileitem name: " << curfileitem->name << std::endl;

    std::string dirlayout = "";
    // BYTES PER SECTOR
    uint8_t bps = 0;
    currentitem->forimg->ReadContent(&bps, currentitem->voloffset + 108, 1);
    uint16_t bytespersector = pow(2, (uint)bps);
    // SECTORS PER CLUSTER
    uint8_t spc = 0;
    currentitem->forimg->ReadContent(&spc, currentitem->voloffset + 109, 1);
    uint16_t sectorspercluster = pow(2, (uint)spc);
    // FAT OFFSET
    uint32_t fatoff = 0;
    ReadForImgContent(currentitem->forimg, &fatoff, currentitem->voloffset + 80);
    uint64_t fatoffset = fatoff + currentitem->voloffset;
    // FAT SIZE
    uint32_t fatsize = 0;
    ReadForImgContent(currentitem->forimg, &fatsize, currentitem->voloffset + 84);
    // CLUSTER AREA START
    uint32_t clusterstart = 0;
    ReadForImgContent(currentitem->forimg, &clusterstart, currentitem->voloffset + 88);
    // ROOT DIR CLUSTER
    uint32_t rootdircluster = 0;
    ReadForImgContent(currentitem->forimg, &rootdircluster, currentitem->voloffset + 96);
    std::vector<uint> clusterlist;
    clusterlist.clear();
    if(rootdircluster >= 2)
    {
	clusterlist.push_back(rootdircluster);
	GetNextCluster(currentitem->forimg, rootdircluster, 4, fatoffset, &clusterlist);
    }
    std::string rootdirlayout = ConvertBlocksToExtents(&clusterlist, sectorspercluster * bytespersector, clusterstart * bytespersector);
    if(curfileitem == NULL) // root directory
	dirlayout = rootdirlayout;
    else // sub directory
	dirlayout = curfileitem->layout;
    uint64_t diroffset = 0;
    uint64_t dirsize = 0;
    std::vector<std::string> dirlayoutlist;
    dirlayoutlist.clear();
    std::istringstream dirlayoutstream(dirlayout);
    std::string curlayout;
    while(getline(dirlayoutstream, curlayout, ';'))
	dirlayoutlist.push_back(curlayout);
    for(int i=0; i < dirlayoutlist.size(); i++)
    {
	diroffset = 0;
	dirsize = 0;
	std::size_t layoutsplit = dirlayoutlist.at(i).find(",");
	diroffset = std::stoull(dirlayoutlist.at(i).substr(0, layoutsplit));
	dirsize = std::stoull(dirlayoutlist.at(i).substr(layoutsplit+1));
	if(i == 0 && curfileitem != NULL) // first dirlayout entry and not root directory
	{
	    diroffset = diroffset + 64; // skip . and .. directories
	    dirsize = dirsize - 64; // adjust read size for the 64 byte skip
	}
	uint direntrycount = dirsize / 32;
	for(uint j=0; j < direntrycount; j++)
	{
	    FileItem tmpitem;
	    // LOGICAL SIZE
	    uint64_t logicalsize = 0;
	    // PHYSICAL SIZE
	    uint64_t physicalsize = 0;
	    // FAT CHAIN TYPE
	    int fatchain = 0;
	    // FILE ATTRIBUTE STRING
	    std::string attributes = "";
	    // FILE ATTRIBUTE INT
	    uint8_t fileattr = 0;
	    // ENTRY TYPE
	    uint8_t entrytype = 0;
	    currentitem->forimg->ReadContent(&entrytype, diroffset + j*32, 1);
	    // CLUSTER NUMBER
	    uint32_t clusternum = 0;
	    if((uint)entrytype == 0x00) // cur dir entry is free and all remaining are free
		break;
	    else if((uint)entrytype == 0x81) // $ALLOC_BITMAP file
	    {
		//std::cout << "cluster num offset: " << diroffset + j*32 + 20 << std::endl;
		ReadForImgContent(currentitem->forimg, &clusternum, diroffset + j*32 + 20);
		//std::cout << "clusternum: " << clusternum << std::endl;
		tmpitem.name = "$ALLOC_BITMAP";
		ReadForImgContent(currentitem->forimg, &logicalsize, diroffset + j*32 + 24); 
		tmpitem.size = logicalsize;
		tmpitem.path = "/";
		attributes = "Allocation Bitmap,";
	    }
	    else if((uint)entrytype == 0x82) // $UPCASE_TABLE file
	    {
		ReadForImgContent(currentitem->forimg, &clusternum, diroffset + j*32 + 20);
		tmpitem.name = "$UPCASE_TABLE";
		ReadForImgContent(currentitem->forimg, &logicalsize, diroffset + j*32 + 24);
		tmpitem.size = logicalsize;
		tmpitem.path = "/";
		attributes = "Up-Case Table,";
	    }
	    else if((uint)entrytype == 0x83) // VOLUME_LABEL (already handles so skip)
	    {
	    }
	    else if((uint)entrytype == 0x03) // Deleted VOLUME_LABEL
	    {
		// skip for now
	    }
	    else if((uint)entrytype == 0x85 || (uint)entrytype == 0x05) // File/Dir Directory Entry
	    {
		std::cout << "cluster num offset: " << diroffset + j*32 + 20 << std::endl;
		ReadForImgContent(currentitem->forimg, &clusternum, diroffset + j*32 + 20);
		std::cout << "clusternum: " << clusternum << std::endl;
		uint8_t secondarycount = 0;
		currentitem->forimg->ReadContent(&secondarycount, diroffset + j*32 + 1, 1);
		currentitem->forimg->ReadContent(&fileattr, diroffset + j*32 + 4, 1);
		if(fileattr & 0x01)
		    attributes += "Read Only,";
		if(fileattr & 0x02)
		    attributes += "Hidden File,";
		if(fileattr & 0x04)
		    attributes += "System File,";
		if(fileattr & 0x10)
		    attributes += "Sub Directory,";
		if(fileattr & 0x20)
		    attributes += "Archive File,";
		if(fileattr & 0x10)
		    tmpitem.isdirectory = true;
		if((uint)entrytype == 0x05)
		    tmpitem.isdeleted = true;

		// CREATE DATETIME
		uint16_t createdate = 0;
		ReadForImgContent(currentitem->forimg, &createdate, diroffset + j*32 + 10);
		uint16_t createtime = 0;
		ReadForImgContent(currentitem->forimg, &createtime, diroffset + j*32 + 8);
		uint8_t createzone = 0;
		currentitem->forimg->ReadContent(&createzone, diroffset + j*32 + 22, 1);
		tmpitem.create = ConvertExFatTimeToHuman(&createdate, &createtime, &createzone);
		std::cout << "Create Date|" << ConvertExFatTimeToHuman(&createdate, &createtime, &createzone) << "|" << std::endl;
		// MODIFY DATETIME
		uint16_t modifydate = 0;
		ReadForImgContent(currentitem->forimg, &modifydate, diroffset + j*32 + 14);
		uint16_t modifytime = 0;
		ReadForImgContent(currentitem->forimg, &modifytime, diroffset + j*32 + 12);
		uint8_t modifyzone = 0;
		currentitem->forimg->ReadContent(&modifyzone, diroffset + j*32 + 23, 1);
		tmpitem.modify = ConvertExFatTimeToHuman(&modifydate, &modifytime, &modifyzone);
		// ACCESS DATETIME
		uint16_t accessdate = 0;
		ReadForImgContent(currentitem->forimg, &accessdate, diroffset + j*32 + 18);
		uint16_t accesstime = 0;
		ReadForImgContent(currentitem->forimg, &accesstime, diroffset + j*32 + 16);
		uint8_t accesszone = 0;
		currentitem->forimg->ReadContent(&accesszone, diroffset + j*32 + 24, 1);
		tmpitem.access = ConvertExFatTimeToHuman(&accessdate, &accesstime, &accesszone);
		
		uint8_t namelength = 0;
		uint8_t curlength = 0;
		for(uint8_t k=1; k <= secondarycount; k++)
		{
		    uint8_t subentrytype = 0;
		    currentitem->forimg->ReadContent(&subentrytype, diroffset + (j + k)*32, 1);
		    if((uint)subentrytype == 0xc0 || (uint)subentrytype == 0x40) // Stream Extention DirEntry
		    {
			currentitem->forimg->ReadContent(&namelength, diroffset + (j + k)*32 + 3, 1);
			// FLAGS
			uint8_t flags = 0;
			currentitem->forimg->ReadContent(&flags, diroffset + (j+k)*32 + 1, 1);
			std::bitset<8> flagbits(flags);
			// FATCHAIN
			fatchain = flagbits[1];
			// LOGICAL SIZE
			ReadForImgContent(currentitem->forimg, &logicalsize, diroffset + (j+k)*32 + 8);
			// CLUSTER NUMBER
			ReadForImgContent(currentitem->forimg, &clusternum, diroffset + (j+k)*32 + 20);
			// PHYSICAL SIZE
			ReadForImgContent(currentitem->forimg, &physicalsize, diroffset + (j+k)*32 + 24);
		    }
		    else if((uint)subentrytype == 0xc1 || (uint)subentrytype == 0x41) // File Name Dir Entry
		    {
			curlength += 15;
			if(curlength <= namelength)
			{
			    for(int m=1; m < 16; m++)
			    {
				uint16_t singleletter = 0;
				ReadForImgContent(currentitem->forimg, &singleletter, diroffset + (j+k)*32 + m*2);
				tmpitem.name += (char)singleletter;
			    }
			}
			else
			{
			    int remaining = namelength + 16 - curlength;
			    for(int m=1; m < 16; m++)
			    {
				uint16_t singleletter = 0;
				ReadForImgContent(currentitem->forimg, &singleletter, diroffset + (j+k)*32 + m*2);
				tmpitem.name += (char)singleletter;
			    }
			}
			std::cout << "file name: " << tmpitem.name << std::endl;
		    }
		}
		/*
		for(uint8_t k=1; k <= secondarycount; k++)
		{
		    if(subentrytype == 0xc0 || subentrytype == 0x40) // Stream Extension Directory Entry
		    {
			QString flagstr = QString("%1").arg(qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + (j+k)*32 + 1, 1)), 8, 2, QChar('0'));
			fatchain = flagstr.mid(7, 1).toInt(nullptr, 2);
			logicalsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(rootdiroffset + (j+k)*32 + 8, 8));
			clusternum = qFromLittleEndian<uint32_t>(curimg->ReadContent(rootdiroffset + (j+k)*32 + 20, 4));
			physicalsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(rootdiroffset + (j+k)*32 + 24, 8));
		    }
		    else if(subentrytype == 0xc1 || subentrytype == 0x41) // File Name Directory Entry
		    {
			curlength += 15;
			if(curlength <= namelength)
			{
			    for(int m=1; m < 16; m++)
				filename += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + (j+k)*32 + m*2, 2))));
			}
			else
			{
			    int remaining = namelength + 16 - curlength;
			    for(int m=1; m < remaining; m++)
				filename += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + (j+k)*32 + m*2, 2))));
			}
		    }
		}
		filepath = "/";
	    }
	    else if(entrytype == 0xa0) // Volume GUID Dir Entry
	    {
		// skip for now
	    }

		*/
	    }
	    if(entrytype == 0x85 || entrytype == 0x05 || entrytype == 0x81 || entrytype == 0x82)
	    {
		if(fatchain == 0 && clusternum > 1)
		{
		    std::vector<uint32_t> clusterlist;
		    clusterlist.clear();
		    clusterlist.push_back(clusternum);
		    GetNextCluster(currentitem->forimg, clusternum, 4, fatoffset, &clusterlist);
		    tmpitem.layout = ConvertBlocksToExtents(&clusterlist, sectorspercluster * bytespersector, clusterstart * bytespersector);
		    physicalsize = clusterlist.size() * bytespersector * sectorspercluster;
		    clusterlist.clear();
		}
		else if(fatchain == 1)
		{
		    uint clustercount = (uint)ceil((float)physicalsize / (bytespersector * sectorspercluster));
		    tmpitem.layout = std::to_string(clusterstart * bytespersector + ((clusternum - 2) * bytespersector * sectorspercluster)) + "," + std::to_string(clustercount * bytespersector * sectorspercluster) + ";";
		}
		if(tmpitem.isdirectory)
		{
		    tmpitem.size = physicalsize;
		    tmpitem.cat = "Directory";
		    tmpitem.sig = "Directory";
		}
		else
		{
		    if(logicalsize > 0)
			GenerateCategorySignature(currentitem, &tmpitem.name, &(tmpitem.layout), &(tmpitem.cat), &(tmpitem.sig));
		    else
		    {
			tmpitem.cat = "Empty";
			tmpitem.sig = "Empty File";
		    }
		}
		tmpitem.properties = attributes + ">" + tmpitem.layout + ">" + std::to_string(physicalsize) + ">" + std::to_string(logicalsize);
		if(!tmpitem.layout.empty())
		    filevector->push_back(tmpitem);
	    }
	}
    }

    /* FAT32 CONTENT

    for(int k=0; k < dirlayoutlist.size(); k++)
    {
	// PARSE DIRECTORY ENTRIES
	for(uint i=0; i < direntrycount; i++)
	{
	    FileItem tmpitem;
	    // FIRST CHARACTER
	    uint8_t firstchar = 0;
	    currentitem->forimg->ReadContent(&firstchar, diroffset + i*32, 1);
	    if((uint)firstchar == 0x00) // entry is free and all remaining are free
		break;
	    else
	    {
		uint8_t fileattr = 0;
		currentitem->forimg->ReadContent(&fileattr, diroffset + i*32 + 11, 1);
		//std::cout << "file attr: " << std::hex << (uint)fileattr << std::endl;
		//std::cout << "first char: " << std::hex << (uint)firstchar << std::endl;
		if((uint)fileattr == 0x0f || (uint)fileattr == 0x3f) // long directory name
		{
		    if(firstchar & 0x40)
		    {
			if(!longnamestring.empty()) // ORPHAN LONG ENTRY
			{
			    tmpitem.name = longnamestring;
			    if(curfileitem == NULL)
				tmpitem.path = "/";
			    else
				tmpitem.path = curfileitem->path + curfileitem->name + "/";
			    tmpitem.isdeleted = true;
			    tmpitem.cat = "Orphan";
			    tmpitem.sig = "Orphan File";
			    filevector->push_back(tmpitem);
			    tmpitem.clear();
			    // orphan entry need to populate in orphans directory.
			    // maybe i don't want an orphan directory, since i don't know the orphan dir's globalid
			    // i can just place them in the directory they fall and just list them as orphan
			    // for their cat/sig
			}
		    }
		    uint lsn = ((int)firstchar & 0x0f);
		    if(lsn <= 20)
		    {
			std::string longname = "";
			int arr[13] = {1, 3, 5, 7, 9, 14, 16, 18, 20, 22, 24, 28, 30};
			for(int j=0; j < 13; j++)
			{
			    uint16_t longletter = 0;
			    ReadForImgContent(currentitem->forimg, &longletter, diroffset + i*32 + arr[j]);
			    if(longletter < 0xFFFF)
				longname += (char)longletter;
			}
			longnamestring.insert(0, longname);
		    }
*/
	/*
	 *  else if(fileattr == 0x0f || 0x3f) // long directory entry for succeeding short entry...
            {
                if(firstchar & 0x40)
                {
                    if(!longnamestring.isEmpty()) // orphan long entry
                    {
			QHash<QString, QVariant> nodedata;
			nodedata.clear();
			nodedata.insert("name", QByteArray(longnamestring.toUtf8()).toBase64());
                        nodedata.insert("path", QByteArray("/orphans/").toBase64());
                        nodedata.insert("size", 0);
                        nodedata.insert("create", "0");
                        nodedata.insert("access", "0");
                        nodedata.insert("modify", "0");
                        nodedata.insert("status", "0");
                        nodedata.insert("hash", "0");
                        nodedata.insert("cat", "Empty");
                        nodedata.insert("sig", "Empty File");
                        nodedata.insert("tag", "0");
                        nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
			nodedata.insert("match", 0);
			mutex.lock();
			treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-o"), 4, 1);
			mutex.unlock();
			nodedata.clear();
			inodecnt++;
                        //qDebug() << "orphan:" << longnamestring;
                        //orphaninfo.clear();
                        //orphaninfo.insert("filename", QVariant(longnamestring));
                        //orphanlist->append(orphaninfo);
                        //orphanlist->append(longnamestring);
                        longnamestring = "";
                    }
                }
	 */ 
/*
		}
		else
		{
		    // FILE OR DIRECTORY
		    //bool isdirectory = false; // file
		    if((uint)fileattr & 0x10)
			tmpitem.isdirectory = true; // directory
		    //std::cout << "is directory: " << isdirectory << std::endl;
		    // DELETED OR NOT DELETED
		    //bool isdeleted = false; // not deleted
		    if((uint)firstchar == 0x05 || (uint)firstchar == 0xe5)
			tmpitem.isdeleted = true; // deleted
		    //std::cout << "is deleted: " << isdeleted << std::endl;
		    // LONGNAME PROPERTY
		    tmpitem.properties = longnamestring + ">";
		    // FILE PROPERTIES
		    if(fileattr & 0x01)
			tmpitem.properties += "Read Only,";
		    if(fileattr & 0x02)
			tmpitem.properties += "Hidden File,";
		    if(fileattr & 0x04)
			tmpitem.properties += "System File,";
		    if(fileattr & 0x08)
			tmpitem.properties += "Volume ID,";
		    if(fileattr & 0x10)
			tmpitem.properties += "SubDirectory,";
		    if(fileattr & 0x20)
			tmpitem.properties += "Archive File,";
		    tmpitem.properties += ">";
		    // ALIAS FILENAME
		    std::string aliasname = "";
		    char* rname = new char[8];
		    currentitem->forimg->ReadContent((uint8_t*)rname, diroffset + i*32 + 1, 7);
		    rname[7] = 0;
		    std::string restname(rname);
		    char* ename = new char[4];
		    currentitem->forimg->ReadContent((uint8_t*)ename, diroffset + i*32 + 8, 3);
		    ename[3] = 0;
		    std::string extname(ename);
		    while(extname.size() > 0)
		    {
			std::size_t findempty = extname.find(" ", 0);
			if(findempty != std::string::npos)
			    extname.erase(findempty, 1);
			else
			    break;
		    }
		    restname.erase(std::remove_if(restname.begin(), restname.end(), isspace), restname.end());
		    if(tmpitem.isdeleted)
			aliasname = std::string(1, '_') + restname;
		    else
			aliasname = std::string(1, (char)firstchar) + restname;
		    if(extname.size() > 0)
			aliasname += "." + extname;
		    tmpitem.properties += aliasname + ">";
		    // FILENAME
		    if(!longnamestring.empty())
		    {
			tmpitem.name = longnamestring;
		    }
		    else
		    {
			tmpitem.name = aliasname;
*/
			/*
			char* rname = new char[8];
			currentitem->forimg->ReadContent((uint8_t*)rname, diroffset + i*32 + 1, 7);
			rname[7] = 0;
			std::string restname(rname);
			char* ename = new char[4];
			currentitem->forimg->ReadContent((uint8_t*)ename, diroffset + i*32 + 8, 3);
			ename[3] = 0;
			std::string extname(ename);
			while(extname.size() > 0)
			{
			    std::size_t findempty = extname.find(" ", 0);
			    if(findempty != std::string::npos)
				extname.erase(findempty, 1);
			    else
				break;
			}
			restname.erase(std::remove_if(restname.begin(), restname.end(), isspace), restname.end());
			if(tmpitem.isdeleted)
			    tmpitem.name = std::string(1, '_') + restname;
			else
			    tmpitem.name = std::string(1, (char)firstchar) + restname;
			if(extname.size() > 0)
			    tmpitem.name += "." + extname;
			*/
/*
		    }
		    if(curfileitem == NULL)
			tmpitem.path = "/";
		    else
			tmpitem.path = curfileitem->path + curfileitem->name + "/";
		    //std::cout << "tmpitem.path: " << tmpitem.path << std::endl;
		    //tmpitem.path = "/";
		    //std::cout << "file name: " << filename << std::endl;
		    longnamestring = "";
		    // LOGICAL FILE SIZE
		    uint32_t logicalsize = 0;
		    ReadForImgContent(currentitem->forimg, &logicalsize, diroffset + i*32 + 28);
		    //std::cout << "logical file size: " << logicalsize << std::endl;
		    tmpitem.size = logicalsize;
		    // CREATE DATE
		    uint16_t createdate = 0;
		    ReadForImgContent(currentitem->forimg, &createdate, diroffset + i*32 + 16);
		    uint16_t createtime = 0;
		    ReadForImgContent(currentitem->forimg, &createtime, diroffset + i*32 + 14);
		    tmpitem.create = ConvertDosTimeToHuman(&createdate, &createtime);
		    //std::cout << "Created Date: " << datecreated << std::endl;
		    // ACCESS DATE
		    uint16_t accessdate = 0;
		    ReadForImgContent(currentitem->forimg, &accessdate, diroffset + i*32 + 18);
		    tmpitem.access = ConvertDosTimeToHuman(&accessdate, NULL);
		    //std::cout << "Accessed Date: " << dateaccessed << std::endl;
		    // MODIFY DATE
		    uint16_t modifydate = 0;
		    ReadForImgContent(currentitem->forimg, &modifydate, diroffset + i*32 + 24);
		    uint16_t modifytime = 0;
		    ReadForImgContent(currentitem->forimg, &modifytime, diroffset + i*32 + 22);
		    tmpitem.modify = ConvertDosTimeToHuman(&modifydate, &modifytime);
		    //std::cout << "Modified Date: " << datemodified << std::endl;
		    uint16_t hiclusternum = 0;
		    ReadForImgContent(currentitem->forimg, &hiclusternum, diroffset + i*32 + 20); // always 0 for FAT12/16
		    uint16_t loclusternum = 0;
		    ReadForImgContent(currentitem->forimg, &loclusternum, diroffset + i*32 + 26);
		    uint32_t clusternum = ((uint32_t)hiclusternum >> 16) + loclusternum;
		    //std::cout << "clusternum: " << clusternum << std::endl;
		    std::vector<uint> clusterlist;
		    clusterlist.clear();
		    if(clusternum >= 2)
		    {
			//std::cout << "currentitem itemtext: " << currentitem->itemtext << std::endl;
			clusterlist.push_back(clusternum);
			//if(currentitem->itemtext.find("[FAT12]") != std::string::npos)
			//    GetNextCluster(currentitem->forimg, clusternum, 1, currentitem->voloffset + reservedareasize * bytespersector, &clusterlist);
			//else if(currentitem->itemtext.find("[FAT16]") != std::string::npos)
			GetNextCluster(currentitem->forimg, clusternum, 3, fatoffset, &clusterlist);
			//clusterareastart = (qulonglong)(curstartsector + reservedareasize + (fatcount * fatsize));
			//GetNextCluster(curforimg, rootdircluster, 4, fatoffset, &clusterlist);
			//void GetNextCluster(ForImg* curimg, uint32_t clusternum, uint8_t fstype, uint64_t fatoffset, std::vector<uint>* clusterlist);
			//QString::number((qulonglong)(curstartsector*512 + reservedareasize * bytespersector))
			//currentitem->voloffset + reservedareasize * bytespersector
		    }
		    //for(int i=0; i < clusterlist.size(); i++)
		    //    std::cout << "cluster " << i << ": " << clusterlist.at(i) << std::endl;
		    //uint64_t diroffset = offset + (reservedareasize + fatcount * fatsize) * bytespersector;
		    if(clusterlist.size() > 0)
			tmpitem.layout = ConvertBlocksToExtents(&clusterlist, sectorspercluster * bytespersector, clusterareastart * bytespersector);
		    tmpitem.properties += tmpitem.layout + ">";
		    uint64_t physicalsize = clusterlist.size() * sectorspercluster * bytespersector;
		    tmpitem.properties += std::to_string(physicalsize) + ">";
		    tmpitem.properties += std::to_string(logicalsize);
		    if(tmpitem.isdirectory)
		    {
			tmpitem.size = physicalsize;
			tmpitem.cat = "Directory";
			tmpitem.sig = "Directory";
		    }
		    else
		    {
			GenerateCategorySignature(currentitem, &tmpitem.name, &(tmpitem.layout), &(tmpitem.cat), &(tmpitem.sig));
			//std::cout << "cat/sig: " << tmpitem.cat << " " << tmpitem.sig << std::endl;
		    }
		    //std::cout << aliasname << ": " << tmpitem.properties << std::endl;
		    //std::cout << "tmpitem layout: " << tmpitem.layout << std::endl;
*/
		    /*
		    uint16_t hiclusternum = qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 20, 2)); // always zero for fat12/16
		    uint16_t loclusternum = qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 26, 2));
		    uint32_t clusternum = ((uint32_t)hiclusternum >> 16) + loclusternum;
		    QList<uint> clusterlist;
		    clusterlist.clear();
		    if(clusternum >= 2)
		    {
			clusterlist.append(clusternum);
			GetNextCluster(curimg, clusternum, fstype, fatoffset, &clusterlist);
		    }
		    QString layout = "";
		    if(clusterlist.count() > 0)
			layout = ConvertBlocksToExtents(clusterlist, sectorspercluster * bytespersector, clusterareastart * bytespersector);
		    out << "Layout|" << layout << "|File offset,size; layout in bytes." << Qt::endl;
		    qulonglong physicalsize = clusterlist.count() * sectorspercluster * bytespersector;
		    out << "Physical Size|" << QString::number(physicalsize) << "|Sector Size in Bytes for the file." << Qt::endl;
		     */
/*
		    if(!tmpitem.layout.empty())
			filevector->push_back(tmpitem);
		}
	    }
	}
    }
    if(curfileitem == NULL) // root directory - add virtual files
    {
	//curinode = AddVirtualFileSystemFiles(curimg, ptreecnt, fatcount, fatsize * bytespersector, curinode);
	// ADD VIRTUAL FILE SYSTEMS FILES
	// ADD $MBR
	FileItem tmpitem;
	tmpitem.size = bytespersector;
	tmpitem.name = "$MBR";
	tmpitem.path = "/";
	tmpitem.layout = std::to_string(currentitem->voloffset) + "," + std::to_string(bytespersector) + ";";
	tmpitem.isvirtual = true;
	tmpitem.cat = "System File";
	tmpitem.sig = "Master Boot Record";
	filevector->push_back(tmpitem);
	// ADD $FAT(s)
	for(int i=0; i < fatcount; i++)
	{
	    tmpitem.clear();
	    tmpitem.size = fatsize * bytespersector;
	    tmpitem.name = "$FAT" + std::to_string(i+1);
	    tmpitem.path = "/";
	    tmpitem.layout = std::to_string(currentitem->voloffset + reservedareasize * bytespersector + fatsize * i * bytespersector) + "," + std::to_string(fatsize * bytespersector) + ";";
	    tmpitem.isvirtual = true;
	    tmpitem.cat = "System File";
	    tmpitem.sig = "File Allocation Table";
	    filevector->push_back(tmpitem);
	}
*/
	/*
	// ADD ORPHANS VIRTUAL DIRECTORY
	tmpitem.clear();
	tmpitem.size = 0;
	tmpitem.name = "orphans";
	tmpitem.path = "/";
	tmpitem.isvirtual = true;
	tmpitem.isdirectory = true;
	tmpitem.cat = "Directory";
	tmpitem.sig = "Virtual Directory";
	filevector->push_back(tmpitem);
	*/
/*
    }
*/
}
/*
    for(int i=0; i < rootdirlayout.split(";", Qt::SkipEmptyParts).count(); i++)
    {
	qulonglong rootdiroffset = 0;
	qulonglong rootdirsize = 0;
	rootdiroffset = rootdirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong();
	rootdirsize = rootdirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong();
	if(dirlayout.isEmpty())
	{
	    orphanoffsets->append(rootdiroffset);
	    orphanoffsets->append(rootdirsize);
	}
	uint rootdirentrycount = rootdirsize / 32;
        //qDebug() << "current rootdirentrycount:" << rootdirentrycount;
	if(parinode == 0)
	    inodecnt = 0;
	else
	    inodecnt = parinode + 1;
	//qDebug() << "inodecnt at start of parent comparison:" << inodecnt;
	for(uint j=0; j < rootdirentrycount; j++)
	{
	    QTextStream out;
	    QFile fileprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
	    if(!fileprop.isOpen())
		fileprop.open(QIODevice::Append | QIODevice::Text);
	    out.setDevice(&fileprop);
	    uint8_t entrytype = qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32, 1));
	    //qDebug() << "inodecnt:" << inodecnt << "entrytype:" << QString::number(entrytype, 16);

	    QString filename = "";
	    QString filepath = "";
	    QString layout = "";
	    int fatchain = 0;
	    uint32_t clusternum = qFromLittleEndian<uint32_t>(curimg->ReadContent(rootdiroffset + j*32 + 20, 4));
	    uint64_t logicalsize = 0;
	    uint64_t physicalsize = 0;
	    uint8_t isdeleted = 0;
	    uint8_t itemtype = 0;
	    uint8_t fileattr = 0;
	    qint64 createdate = 0;
	    qint64 accessdate = 0;
	    qint64 modifydate = 0;

	    if(entrytype == 0x00) // cur dir entry is free and all remaining are free
		break;
	    else if(entrytype == 0x81) // $ALLOC_BITMAP file
	    {
		filename = "$ALLOC_BITMAP";
		logicalsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(rootdiroffset + j*32 + 24, 8));
		isdeleted = 0;
		itemtype = 10;
		filepath = "/";
	    }
	    else if(entrytype == 0x82) // $UPCASE_TABLE file
	    {
		filename = "$UPCASE_TABLE";
		logicalsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(rootdiroffset + j*32 + 24, 8));
		isdeleted = 0;
		itemtype = 10;
		filepath = "/";
	    }
	    else if(entrytype == 0x83) // VOLUME_LABEL (already handles so skip)
	    {
	    }
	    else if(entrytype == 0x03) // Deleted VOLUME_LABEL
	    {
		// skip for now
	    }
	    else if(entrytype == 0x85 || entrytype == 0x05) // File/Dir Directory Entry
	    {
		uint8_t secondarycount = qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 1, 1));
		fileattr = qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 4, 1));
                out << "File Attributes|";
		if(fileattr & 0x01)
		    out << "Read Only,";
		if(fileattr & 0x02)
		    out << "Hidden File,";
		if(fileattr & 0x04)
		    out << "System File,";
		if(fileattr & 0x10)
		    out << "Sub Directory,";
		if(fileattr & 0x20)
		    out << "Archive File,";
                out << "|File attributes." << Qt::endl;
		if(fileattr & 0x10)
		{
		    if(entrytype == 0x85)
		    {
			isdeleted = 0;
			itemtype = 3;
		    }
		    else if(entrytype == 0x05)
		    {
			isdeleted = 1;
			itemtype = 2;
		    }
		}
		else
		{
		    if(entrytype == 0x85)
		    {
			isdeleted = 0;
			itemtype = 5;
		    }
		    else if(entrytype == 0x05)
		    {
			isdeleted = 1;
			itemtype = 4;
		    }
		}
		createdate = ConvertExfatTimeToUnixTime(qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 9, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 8, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 11, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 10, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 22, 1)));
		modifydate = ConvertExfatTimeToUnixTime(qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 13, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 12, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 15, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 14, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 23, 1)));
		accessdate = ConvertExfatTimeToUnixTime(qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 17, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 16, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 19, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 18, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 24, 1)));
		uint8_t namelength = 0;
		uint8_t curlength = 0;
		for(uint8_t k=1; k <= secondarycount; k++)
		{
		    uint8_t subentrytype = qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + k*32, 1));
		    //qDebug() << "subentrytype:" << QString::number(subentrytype, 16);
		    if(subentrytype == 0xc0 || subentrytype == 0x40) // Stream Extension Directory Entry
		    {
			namelength = qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + (j + k)*32 + 3, 1));
			QString flagstr = QString("%1").arg(qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + (j+k)*32 + 1, 1)), 8, 2, QChar('0'));
			fatchain = flagstr.mid(7, 1).toInt(nullptr, 2);
			logicalsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(rootdiroffset + (j+k)*32 + 8, 8));
			clusternum = qFromLittleEndian<uint32_t>(curimg->ReadContent(rootdiroffset + (j+k)*32 + 20, 4));
			physicalsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(rootdiroffset + (j+k)*32 + 24, 8));
		    }
		    else if(subentrytype == 0xc1 || subentrytype == 0x41) // File Name Directory Entry
		    {
			curlength += 15;
			if(curlength <= namelength)
			{
			    for(int m=1; m < 16; m++)
				filename += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + (j+k)*32 + m*2, 2))));
			}
			else
			{
			    int remaining = namelength + 16 - curlength;
			    for(int m=1; m < remaining; m++)
				filename += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + (j+k)*32 + m*2, 2))));
			}
		    }
		}
		filepath = "/";
	    }
	    else if(entrytype == 0xa0) // Volume GUID Dir Entry
	    {
		// skip for now
	    }
	    //qDebug() << "filename:" << filename << "clusternum:" << clusternum;
	    if(fatchain == 0 && clusternum > 1)
	    {
		QList<uint32_t> clusterlist;
		clusterlist.clear();
		clusterlist.append(clusternum);
		GetNextCluster(curimg, clusternum, 4, fatoffset, &clusterlist);
		layout = ConvertBlocksToExtents(clusterlist, sectorspercluster * bytespersector, clusterareastart * bytespersector);
		clusterlist.clear();
	    }
	    else if(fatchain == 1)
	    {
		uint clustercount = (uint)ceil((float)physicalsize / (bytespersector * sectorspercluster));
		layout = QString::number(clusterareastart * bytespersector + ((clusternum - 2) * bytespersector * sectorspercluster)) + "," + QString::number(clustercount * bytespersector * sectorspercluster) + ";";
		//qDebug() << "clustercount:" << clustercount;
	    }
	    //qDebug() << "filename:" << filename << "layout:" << layout;

	    if(entrytype == 0x85 || entrytype == 0x05 || entrytype == 0x81 || entrytype == 0x82)
	    {
		for(int c=0; c < layout.split(";", Qt::SkipEmptyParts).count(); c++)
		{
		    orphanoffsets->append(layout.split(";", Qt::SkipEmptyParts).at(c).split(",").at(0).toULongLong());
		    orphanoffsets->append(layout.split(";", Qt::SkipEmptyParts).at(c).split(",").at(1).toULongLong());
		}
		out << "Layout|" << layout << "|File offset,size; layout in bytes." << Qt::endl;
		out << "Physical Size|" << QString::number(physicalsize) << "|Sector Size in Bytes for the file." << Qt::endl;
		out << "Logical Size|" << QString::number(logicalsize) << "|Size in Bytes for the file." << Qt::endl;
		if(!parfilename.isEmpty())
		    filepath = parfilename;
		QHash<QString, QVariant> nodedata;
		nodedata.clear();
		nodedata.insert("name", QByteArray(filename.toUtf8()).toBase64());
                nodedata.insert("path", QByteArray(filepath.toUtf8()).toBase64());
                nodedata.insert("size", (qulonglong)logicalsize);
                nodedata.insert("create", createdate);
                nodedata.insert("access", accessdate);
                nodedata.insert("modify", modifydate);
                nodedata.insert("status", "0");
                nodedata.insert("hash", "0");
		if(logicalsize > 0) // Get Category/Signature
		{
		    if(itemtype == 3 && isdeleted == 0)
                    {
			nodedata.insert("cat", "Directory");
                        nodedata.insert("sig", "Directory");
                    }
		    else
		    {
			QString catsig = GenerateCategorySignature(curimg, filename, layout.split(";").at(0).split(",").at(0).toULongLong());
			nodedata.insert("cat", catsig.split("/").first());
                        nodedata.insert("sig", catsig.split("/").last());
		    }
		}
		else
                {
		    nodedata.insert("cat", "Empty");
                    nodedata.insert("sig", "Zero File");
                }
		nodedata.insert("tag", "0");
                nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
		nodedata.insert("match", 0);
		QString parentstr = "";
		if(parinode == 0)
		    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
		else
		    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(parinode));
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
		if(fileattr & 0x10 && logicalsize > 0) // Sub Directory
		{
		    inodecnt = ParseExfatDirectory(curimg, curstartsector, ptreecnt, inodecnt - 1, QString(filepath + filename + "/"), layout, orphanoffsets);
		}
		nodedata.clear();
	    }
	    out.flush();
	    fileprop.close();
	}
    }
    //qDebug() << "orphanoffsets:" << orphanoffsets;
    return inodecnt;
}

void ParseExfatOrphans(ForImg* curimg, uint8_t ptreecnt, qulonglong curinode, QList<qulonglong>* orphanoffsets)
{
    //qDebug() << "start orphan processing with curinode:" << curinode;
    uint16_t bytespersector = 0;
    uint8_t sectorspercluster = 0;
    qulonglong fatoffset = 0;
    qulonglong clusterareastart = 0;
    //QString rootdirlayout = "";
    QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/prop");
    if(!propfile.isOpen())
	propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(propfile.isOpen())
    {
        while(!propfile.atEnd())
        {
            QString line = propfile.readLine();
            if(line.startsWith("Bytes Per Sector|"))
                bytespersector = line.split("|").at(1).toUInt();
            else if(line.startsWith("Sectors Per Cluster|"))
                sectorspercluster = line.split("|").at(1).toUInt();
            else if(line.startsWith("FAT Offset|"))
                fatoffset = line.split("|").at(1).toULongLong();
            //else if(line.startsWith("Root Directory Layout|"))
            //    rootdirlayout = line.split("|").at(1);
            else if(line.startsWith("Cluster Area Start|"))
                clusterareastart = line.split("|").at(1).toULongLong();
        }
        propfile.close();
    }
    //qDebug() << "orphanoffsets:" << *orphanoffsets;
    qulonglong curoffset = 0;
    while(curoffset < curimg->Size())
    {
	uint8_t entrytype = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset, 1));
	if(entrytype == 0x05)
	{
	    //qDebug() << "coffset:" << curoffset << "possible orphan.";
	    uint8_t itemtype = 4;
	    uint8_t secondarycount = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 1, 1));
	    uint8_t fileattr = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 4, 1));
	    // out information to property file
	    QString attrstr = "";
	    if(fileattr & 0x01) // Read Only
		attrstr += "Read Only,";
	    if(fileattr & 0x02) // Hidden File
		attrstr += "Hidden File,";
	    if(fileattr & 0x04) // System File
		attrstr += "System File,";
	    if(fileattr & 0x10) // Sub Directory
	    {
		attrstr += "Sub Directory,";
		itemtype = 2;
	    }
	    if(fileattr & 0x20) // Archive File
		attrstr += "Archive File,";
	    qint64 createdate = ConvertExfatTimeToUnixTime(qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 9, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 8, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 11, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 10, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 22, 1)));
	    qint64 modifydate = ConvertExfatTimeToUnixTime(qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 13, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 12, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 15, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 14, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 23, 1)));
	    qint64 accessdate = ConvertExfatTimeToUnixTime(qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 17, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 16, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 19, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 18, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 24, 1)));
	    QString filename = "";
	    uint8_t namelength = 0;
	    uint8_t curlength = 0;
	    uint8_t subentrytype = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 32, 1));
	    if(subentrytype == 0x40)
	    {
		QTextStream out;
		QFile fileprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(curinode) + ".prop");
		if(!fileprop.isOpen())
		    fileprop.open(QIODevice::Append | QIODevice::Text);
		out.setDevice(&fileprop);
		out << "File Attributes|" << attrstr << "|Attributes for the file." << Qt::endl;

		namelength = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 32 + 3, 1));
		QString flagstr = QString("%1").arg(qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 32 + 1, 1)), 8, 2, QChar('0'));
		int fatchain = flagstr.mid(7, 1).toInt(nullptr, 2);
		flagstr = "";
		uint32_t clusternum = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 32 + 20, 4));
		uint64_t logicalsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 32 + 8, 8));
		uint64_t physicalsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 32 + 24, 8));
		if(namelength > 0)
		{
		    for(uint8_t i=0; i < secondarycount - 1; i++)
		    {
			uint8_t subsubentry = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 64, 1));
			if(subsubentry == 0x41)
			{
			    curlength += 15;
			    if(curlength <= namelength)
			    {
				for(int k=1; k < 16; k++)
				    filename += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(64 + i*32 + k*2, 2))));
			    }
			    else
			    {
				int remaining = namelength + 16 - curlength;
				for(int k=1; k < remaining; k++)
				    filename += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(64 + i*32 + k*2, 2))));
			    }
			}
		    }
		    qDebug() << "filename:" << filename;
		    qulonglong parentinode = 0;
		    qulonglong inode = curinode;
		    QString layout = "";
		    if(fatchain == 0 && clusternum > 1)
		    {
			QList<uint> clusterlist;
			clusterlist.clear();
			GetNextCluster(curimg, clusternum, 4, fatoffset, &clusterlist);
			layout = ConvertBlocksToExtents(clusterlist, sectorspercluster * bytespersector, clusterareastart * bytespersector);
			clusterlist.clear();
		    }
		    else if(fatchain == 1)
		    {
			int clustercount = (int)ceil((float)physicalsize / (bytespersector * sectorspercluster));
			layout = QString::number(clusterareastart * bytespersector + ((clusternum - 2) * bytespersector * sectorspercluster)) + "," + QString::number(clustercount * bytespersector * sectorspercluster) + ";";
		    }
		    out << "Layout|" << layout << "|File offset,size; layout in bytes." << Qt::endl;
		    out << "Physical Size|" << QString::number(physicalsize) << "|Sector Size in Bytes for the file." << Qt::endl;
		    out << "Logical Size|" << QString::number(logicalsize) << "|Size in Bytes for the file." << Qt::endl;
		    QHash<QString, QVariant> nodedata;
		    nodedata.clear();
		    nodedata.insert("name", QByteArray(filename.toUtf8()).toBase64());
                    nodedata.insert("path", QByteArray("/orphans/").toBase64());
                    nodedata.insert("size", (qulonglong)logicalsize);
                    nodedata.insert("create", createdate);
                    nodedata.insert("modify", modifydate);
                    nodedata.insert("access", accessdate);
                    nodedata.insert("status", "0");
                    nodedata.insert("hash", "0");
		    if(logicalsize > 0) // Get Category/Signature
		    {
			if(itemtype == 2)
                        {
			    nodedata.insert("cat", "Directory");
                            nodedata.insert("sig", "Directory");
                        }
			else
			{
			    QString catsig = GenerateCategorySignature(curimg, filename, layout.split(";").at(0).split(",").at(0).toULongLong());
			    nodedata.insert("cat", catsig.split("/").first());
                            nodedata.insert("sig", catsig.split("/").last());
			}
		    }
		    else
                    {
			nodedata.insert("cat", "Empty");
                        nodedata.insert("sig", "Zero File");
                    }
		    nodedata.insert("tag", "0");
                    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(curinode)));
		    nodedata.insert("match", 0);
		    mutex.lock();
		    treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-o"), itemtype, 1);
		    mutex.unlock();
		    nodedata.clear();
		    out.flush();
		    fileprop.close();
		    curinode++;
		}
	    }
	}
	curoffset += 32;
	//qDebug() << "curoffset before fix:" << curoffset;
	for(int i=0; i < orphanoffsets->count() / 2; i++)
	{
	    if(curoffset == orphanoffsets->at(i))
		curoffset += orphanoffsets->at(i+1);
	}
	//qDebug() << "curoffset after fix:" << curoffset;
    }
}

 */ 
