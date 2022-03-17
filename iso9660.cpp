#include "iso9660.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

void ParseIsoDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt)
{
    quint64 curinode = 0;
    uint32_t blocksize = 0;
    uint8_t pvcount = 0;
    uint8_t svcount = 0;
    //QList<uint32_t> pvpathtablesize;
    //QList<uint32_t> pvpathtableblk;
    QList<uint32_t> pvrootdirblk;
    //QList<uint32_t> svpathtablesize;
    //QList<uint32_t> svpathtableblk;
    QList<uint32_t> svrootdirblk;
    QList<uint32_t> parsedblocks;

    pvrootdirblk.clear();
    svrootdirblk.clear();
    parsedblocks.clear();

    QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/prop");
    if(!propfile.isOpen())
	propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(propfile.isOpen())
    {
        while(!propfile.atEnd())
        {
            QString line = propfile.readLine();
	    if(line.startsWith("PV1 Logical Block Size|"))
		blocksize = line.split("|").at(1).toUInt();
	    if(line.startsWith("Primary Volume Count|"))
		pvcount = line.split("|").at(1).toUInt();
	    else if(line.startsWith("Supplementary/Enhanced Volume Count|"))
		svcount = line.split("|").at(1).toUInt();
        }
	for(int i=1; i <= pvcount; i++)
	{
	    propfile.seek(0);
	    while(!propfile.atEnd())
	    {
		QString line = propfile.readLine();
		/*
		if(line.startsWith("PV" + QString::number(i) + " Path Table Size|"))
		    pvpathtablesize.append(line.split("|").at(1).toUInt());
		else if(line.startsWith("PV" + QString::number(i) + " Location of Occurrence of Type L Path Table|"))
		    pvpathtableblk.append(line.split("|").at(1).toUInt());
		else if(line.startsWith("PV" + QString::number(i) + " Extent Location|"))
		    pvrootdirblk.append(line.split("|").at(1).toUInt());
		*/
		if(line.startsWith("PV" + QString::number(i) + " Extent Location|"))
		    pvrootdirblk.append(line.split("|").at(1).toUInt());
	    }
	}
	for(int i=1; i <= svcount; i++)
	{
	    propfile.seek(0);
	    while(!propfile.atEnd())
	    {
		QString line = propfile.readLine();
		/*
		if(line.startsWith("SV" + QString::number(i) + " Path Table Size|"))
		    svpathtablesize.append(line.split("|").at(1).toUInt());
		else if(line.startsWith("SV" + QString::number(i) + " Location of Occurrence of Type L Path Table|"))
		    svpathtableblk.append(line.split("|").at(1).toUInt());
		else if(line.startsWith("SV" + QString::number(i) + " Extent Location|"))
		    svrootdirblk.append(line.split("|").at(1).toUInt());
		*/
		if(line.startsWith("SV" + QString::number(i) + " Extent Location|"))
		    svrootdirblk.append(line.split("|").at(1).toUInt());
	    }
	}
        propfile.close();
    }
    //qDebug() << "pvrootdirblk:" << pvrootdirblk << "svrootdirblk:" << svrootdirblk;
    //qDebug() << "curinode start:" << curinode;
    for(int i=0; i < pvrootdirblk.count(); i++)
    {
	parsedblocks.append(pvrootdirblk.at(i));
	curinode = ParseDirectoryContents(curimg, ptreecnt, pvrootdirblk.at(i), 0, blocksize, false, curinode, "", &parsedblocks);
	//qDebug() << "pv curinode:" << curinode;
    }
    for(int i=0; i < svrootdirblk.count(); i++)
    {
	parsedblocks.append(svrootdirblk.at(i));
	curinode = ParseDirectoryContents(curimg, ptreecnt, svrootdirblk.at(i), 0, blocksize, true, curinode, "", &parsedblocks);
	//qDebug() << "sv curinode:" << curinode;
    }
    //qDebug() << "curinode end:" << curinode;
    //qDebug() << "parsed blocks:" << parsedblocks;
    // what if i skip this operation and just for each pv/sv launch the root directory parse...
    // parse each pv path table
    /*
    for(int i=0; i < pvpathtableblk.count(); i++)
    {
	uint64_t curoff = pvpathtableblk.at(i) * blocksize;
	uint8_t lendi = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoff, 1));
	uint8_t extattrlen = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoff + 1, 1));
	uint32_t extblk = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoff + 2, 4));
	uint16_t pardirnum = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoff + 6, 2));
	QByteArray dirid = curimg->ReadContent(curoff + 8, lendi);
	qDebug() << "curoff:" << curoff << "lendi:" << lendi << "extattrlen:" << extattrlen << "extblk:" << extblk << "pardirnum:" << pardirnum << "dirid:" << dirid;
	ParseDirectoryContents(curimg, extblk, 0, blocksize, false);
	//QString dirid = QString::fromStdString(curimg->ReadContent(curoff + 8, lendi).toStdString());
    }
    for(int i=0; i < svpathtableblk.count(); i++)
    {
	uint64_t curoff = svpathtableblk.at(i) * blocksize;
	uint8_t lendi = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoff, 1));
	uint8_t extattrlen = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoff + 1, 1));
	uint32_t extblk = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoff + 2, 4));
	uint16_t pardirnum = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoff + 6, 2));
	QByteArray dirid = curimg->ReadContent(curoff + 8, lendi);
	qDebug() << "curoff:" << curoff << "lendi:" << lendi << "extattrlen:" << extattrlen << "extblk:" << extblk << "pardirnum:" << pardirnum << "dirid:" << dirid;
	ParseDirectoryContents(curimg, extblk, 0, blocksize, true);
    }
    */
    // PROCESS IS TO PARSE SMALL PATH TABLE RECORD, WHICH LEADS TO THE DIRECTORY RECORD WHICH THEN TAKES ME TO THE FILE RECORD INFO???
    /*
    qDebug() << "parse iso:" << curstartsector << ptreecnt;
    qDebug() << "pvcount:" << pvcount << "svcount:" << svcount;
    qDebug() << "pvpathtablesize:" << pvpathtablesize << "pvpathtableblk:" << pvpathtableblk << "pvrootdirblk:" << pvrootdirblk;
    qDebug() << "svpathtablesize:" << svpathtablesize << "svpathtableblk:" << svpathtableblk << "svrootdirblk:" << svrootdirblk;
    qDebug() << "blocksize:" << blocksize;
    */
}
quint64 ParseDirectoryContents(ForImg* curimg, uint8_t ptreecnt, uint32_t dirextblk, uint32_t parblk, uint32_t blocksize, bool utf16, quint64 parinode, QString parpath, QList<uint32_t>* parsedblocks) // FILE/DIRECTORY RECORD
{
    // MAY NEED TO KEEP TRACK OF FILE/DIR RECORD BLOCKS SO THERE IS NO OVERLAP BETWEEN THE P/S VOLUMES
    //qDebug() << "parinode:" << parinode;
    quint64 curinode = parinode;
    if(parblk != 0 && parinode > 0)
	curinode = parinode + 1;
    //qDebug() << "curinode:" << curinode;
    uint64_t extblkoff = dirextblk * blocksize;
    uint16_t curoff = 0; // starting point which goes to 2048
    while(curoff <= 2048)
    {
	//qDebug() << "curoff:" << curoff;
	uint8_t lendr = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff, 1));
	if(lendr == 0x00)
	    break;
	uint8_t extattrlen = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 1, 1));
	uint32_t extblk = qFromLittleEndian<uint32_t>(curimg->ReadContent(extblkoff + curoff + 2, 4));
	if(!parsedblocks->contains(extblk))
	{
	    // need to fix layout so it's a function of blocksize...
	    uint32_t datalen = qFromLittleEndian<uint32_t>(curimg->ReadContent(extblkoff + curoff + 10, 4));
	    //qDebug() << "# of blocks:" << datalen / blocksize;
	    quint64 physicalsize = 0;
	    int phyblkcnt = datalen / blocksize;
	    int phyremcnt = datalen % blocksize;
	    if(phyblkcnt == 0)
		physicalsize = blocksize;
	    else
	    {
		physicalsize = blocksize * phyblkcnt;
		if(phyremcnt > 0)
		    physicalsize += blocksize;
	    }
	    //qDebug() << "physicalsize:" << physicalsize;
	    // recording date and time (18, 7) need to get 1 at a time and format like before...
	    uint8_t fileflags = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 25, 1));
	    uint8_t fileunitsize = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 26, 1));
	    uint8_t interleavegapsize = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 27, 1));
	    uint16_t volseqnumber = qFromLittleEndian<uint16_t>(curimg->ReadContent(extblkoff + curoff + 28, 4));
	    uint8_t lenfi = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 32, 1));
	    if(extblk != dirextblk && extblk != parblk)
	    {
		QString filepath = "/";
		if(!parpath.isEmpty())
		    filepath = parpath;
		parsedblocks->append(extblk);
		QString fileid = "";
		if(!utf16)
		    fileid = QString::fromStdString(curimg->ReadContent(extblkoff + curoff + 33, lenfi).toStdString());
		else
		{
		    //qDebug() << "curoff before filename:" << curoff << extblkoff + curoff;
		    for(uint8_t i=0; i < lenfi/2; i++)
			fileid += QString(QChar(qFromBigEndian<uint16_t>(curimg->ReadContent(extblkoff + curoff + i*2 + 33, 2))));
		}
		if(extattrlen > 0) // parse extended attribute record
		{
		    qDebug() << "parse extended attribute record here...";
		}
		fileid = fileid.left(fileid.indexOf(QChar(';')));
		QString curlayout = QString::number(extblk * 2048) + "," + QString::number(physicalsize) + ";";
		//qDebug() << "dirblk:" << dirextblk << "lendr:" << lendr << "extattrlen:" << extattrlen << "extblk:" << extblk << "datalen:" << datalen << "file flags:" << QString::number(fileflags, 2) << QString::number(fileflags, 16) << "fileunitsize:" << fileunitsize << "interleavegapsize:" << interleavegapsize << "lenfi:" << lenfi << "fileid:" << fileid << "curinode:" << curinode;
		//qDebug() << "file layout:" << curlayout;
		// add node and properties here...
		uint8_t itemtype = 0;
		uint8_t isdeleted = 0;
		QTextStream out;
		QFile fileprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(curinode) + ".prop");
		if(!fileprop.isOpen())
		    fileprop.open(QIODevice::Append | QIODevice::Text);
		out.setDevice(&fileprop);
		out << "Layout|" << curlayout << "|File layout in offset,size; format." << Qt::endl;
		out << "Physical Size|" << QString::number(physicalsize) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
		out << "Logical Size|" << QString::number(datalen) << "|Size in Bytes for the file." << Qt::endl;
		QHash<QString, QVariant> nodedata;
		nodedata.clear();
		nodedata.insert("name", QByteArray(fileid.toUtf8()).toBase64());
		nodedata.insert("path", QByteArray(filepath.toUtf8()).toBase64());
		nodedata.insert("size", datalen);
		if(fileflags & 0x02) // directory
		{
		    itemtype = 3;
		    if(isdeleted == 1)
			itemtype = 2;
		}
		else
		{
		    itemtype = 5;
		    if(isdeleted == 1)
			itemtype = 4;
		}
		// need to implement cat/sig here...
		if(datalen > 0) // Get Cat/Sig
		{
		    if(fileflags & 0x02) // directory
		    {
			nodedata.insert("cat", "Directory");
			nodedata.insert("sig", "Directory");
		    }
		    else
		    {
			// need to fix this so it doesn't read more than the file size which is 24 bytes.
			QString catsig = GenerateCategorySignature(curimg, fileid, curlayout.split(";").at(0).split(",").at(0).toULongLong());
			nodedata.insert("cat", catsig.split("/").first());
			nodedata.insert("sig", catsig.split("/").last());
		    }
		}
		else
		{
		    nodedata.insert("cat", "Empty");
		    nodedata.insert("sig", "Zero File");
		}
		nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(curinode)));
		//qDebug() << "file id:" << nodedata.value("id").toString();
		QString parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
		if(parblk != 0 && parinode > 0)
		    parentstr += QString("-f" + QString::number(parinode));
		mutex.lock();
		treenodemodel->AddNode(nodedata, parentstr, 5, 0);
		mutex.unlock();
		if(nodedata.value("id").toString().split("-").count() == 3)
		{
		    listeditems.append(nodedata.value("id").toString());
		    filesfound++;
		    isignals->ProgUpd();
		}
		curinode++;
		nodedata.clear();
		out.flush();
		fileprop.close();
		if(fileflags & 0x02) // directory
		{
		    //qDebug() << "this is a directory, so parse with ParseDirectoryContents...";
		    curinode = ParseDirectoryContents(curimg, ptreecnt, extblk, dirextblk, blocksize, utf16, curinode - 1, QString(filepath + fileid + "/"), parsedblocks);
		}
	    }
	}
	//else
	//    qDebug() << "skipped because this dir/file record was already parsed.";
	curoff = curoff + lendr;
    }
    return curinode;
}
        /*
        nodedata.insert("create", createdate);
        nodedata.insert("access", accessdate);
        nodedata.insert("modify", modifydate);
        nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
        QString parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
        if(parinode > 0)
            parentstr += QString("-f" + QString::number(parinode));
        */

