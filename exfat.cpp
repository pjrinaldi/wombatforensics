#include "exfat.h"

void LoadExFatDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem)
{
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
		ReadForImgContent(currentitem->forimg, &clusternum, diroffset + j*32 + 20);
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
		ReadForImgContent(currentitem->forimg, &clusternum, diroffset + j*32 + 20);
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
			tmpitem.size = logicalsize;
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
		    }
		}
		//std::cout << "file name: " << tmpitem.name << std::endl;
		// PATH
		if(curfileitem == NULL)
		    tmpitem.path = "/";
		else
		    tmpitem.path = curfileitem->path + curfileitem->name + "/";
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
	else
	{
	    if(entrytype == 0x85 || entrytype == 0x05 || entrytype == 0x81 || entrytype == 0x82)
	    {
		for(int c=0; c < layout.split(";", Qt::SkipEmptyParts).count(); c++)
		{
		    orphanoffsets->append(layout.split(";", Qt::SkipEmptyParts).at(c).split(",").at(0).toULongLong());
		    orphanoffsets->append(layout.split(";", Qt::SkipEmptyParts).at(c).split(",").at(1).toULongLong());
		}
	    }
	}
    }
    //qDebug() << "orphanoffsets:" << orphanoffsets;
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
