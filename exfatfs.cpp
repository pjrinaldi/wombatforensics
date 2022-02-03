#include "exfatfs.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

qulonglong ParseExfatDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, qulonglong parinode, QString parfilename, QString dirlayout, QList<qulonglong>* orphanoffsets)
{
    qulonglong inodecnt = 0;
    //uint32_t fatsize = 0;
    qulonglong fatoffset = 0;
    uint16_t bytespersector = 0;
    uint8_t sectorspercluster = 0;
    qulonglong clusterareastart = 0;
    QString rootdirlayout = "";
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
            else if(line.startsWith("Root Directory Layout|"))
                rootdirlayout = line.split("|").at(1);
            else if(line.startsWith("FAT Offset|"))
                fatoffset = line.split("|").at(1).toULongLong();
            //else if(line.startsWith("FAT Size|"))
                //fatsize = line.split("|").at(1).toUInt();
            else if(line.startsWith("Cluster Area Start|"))
                clusterareastart = line.split("|").at(1).toULongLong();
        }
        propfile.close();
    }
    if(!dirlayout.isEmpty())
	rootdirlayout = dirlayout;
    //qDebug() << "parinode:" << parinode << "parfilename:" << parfilename << "dirlayout:" << rootdirlayout;
    //qDebug() << "bps:" << bytespersector << "spc:" << sectorspercluster << "rdl:" << rootdirlayout << "fatoffset:" << fatoffset << "cas:" << clusterareastart;
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

