#include "iso9660.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

void ParseIsoDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt)
{
    quint64 curinode = 0;
    uint32_t blocksize = 0;
    uint8_t pvcount = 0;
    uint8_t svcount = 0;
    QList<uint32_t> pvpathtablesize;
    QList<uint32_t> pvpathtableblk;
    QList<uint32_t> pvrootdirblk;
    QList<uint32_t> pvrootdirlen;
    QList<uint32_t> svpathtablesize;
    QList<uint32_t> svpathtableblk;
    QList<uint32_t> svrootdirblk;
    QList<uint32_t> svrootdirlen;
    QList<uint32_t> parsedblocks;
    //QStringList dirlist;

    pvrootdirblk.clear();
    pvrootdirlen.clear();
    svrootdirblk.clear();
    svrootdirlen.clear();
    parsedblocks.clear();
    //dirlist.clear();

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
		if(line.startsWith("PV" + QString::number(i) + " Path Table Size|"))
		    pvpathtablesize.append(line.split("|").at(1).toUInt());
		else if(line.startsWith("PV" + QString::number(i) + " Location of Occurrence of Type L Path Table|"))
		    pvpathtableblk.append(line.split("|").at(1).toUInt());
		
		if(line.startsWith("PV" + QString::number(i) + " Data Length|"))
		    pvrootdirlen.append(line.split("|").at(1).toUInt());
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
		if(line.startsWith("SV" + QString::number(i) + " Path Table Size|"))
		    svpathtablesize.append(line.split("|").at(1).toUInt());
		else if(line.startsWith("SV" + QString::number(i) + " Location of Occurrence of Type L Path Table|"))
		    svpathtableblk.append(line.split("|").at(1).toUInt());

		if(line.startsWith("SV" + QString::number(i) + " Data Length|"))
		    svrootdirlen.append(line.split("|").at(1).toUInt());
		if(line.startsWith("SV" + QString::number(i) + " Extent Location|"))
		    svrootdirblk.append(line.split("|").at(1).toUInt());
	    }
	}
        propfile.close();
    }
    //qDebug() << "pvrootdirblk:" << pvrootdirblk << "pvrootdirlen:" << pvrootdirlen << "svrootdirblk:" << svrootdirblk << "svrootdirlen:" << svrootdirlen;
    //qDebug() << "curinode start:" << curinode;
    
    for(int i=0; i < pvrootdirblk.count(); i++)
    {
	parsedblocks.append(pvrootdirblk.at(i));
	curinode = ParseDirectoryContents(curimg, ptreecnt, pvrootdirblk.at(i), pvrootdirlen.at(i), 0, blocksize, false, curinode, "", &parsedblocks);
	//qDebug() << "pv curinode:" << curinode;
    }
    for(int i=0; i < svrootdirblk.count(); i++)
    {
	parsedblocks.append(svrootdirblk.at(i));
	curinode = ParseDirectoryContents(curimg, ptreecnt, svrootdirblk.at(i), svrootdirlen.at(i), 0, blocksize, true, curinode, "", &parsedblocks);
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
    //qDebug() << "pv start curinode:" << curinode;
    /*
    for(int i=0; i < pvpathtableblk.count(); i++)
	curinode = ParsePathTable(curimg, ptreecnt, blocksize, pvpathtableblk.at(i), pvpathtablesize.at(i), curinode);
    //qDebug() << "pv end curinode:" << curinode;
    for(int i=0; i < svpathtableblk.count(); i++)
	curinode = ParsePathTable(curimg, ptreecnt, blocksize, svpathtableblk.at(i), svpathtablesize.at(i), curinode);
    */
    //qDebug() << "sv end curinode:" << curinode;
    /*
    qDebug() << "parse iso:" << curstartsector << ptreecnt;
    qDebug() << "pvcount:" << pvcount << "svcount:" << svcount;
    qDebug() << "blocksize:" << blocksize;
    qDebug() << "pvpathtablesize:" << pvpathtablesize << "pvpathtableblk:" << pvpathtableblk << "pvrootdirblk:" << pvrootdirblk << "pvrootdirlen:" << pvrootdirlen;
    qDebug() << "svpathtablesize:" << svpathtablesize << "svpathtableblk:" << svpathtableblk << "svrootdirblk:" << svrootdirblk << "svrootdirlen:" << svrootdirlen;
    */
}

quint64 ParsePathTable(ForImg* curimg, uint8_t ptreecnt, uint32_t blocksize, uint32_t ptblk, uint32_t ptlen, quint64 curinode)
{
    QHash<int, QString> idnodelink;
    //qDebug() << "ptblk:" << ptblk << "ptlen:" << ptlen;
    quint64 offset = ptblk * blocksize;
    //qDebug() << "offset:" << offset;
    uint32_t curoff = 0;
    int idcnt = 1;
    QString parpath = "/";
    while(curoff < ptlen)
    {
	QString parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
	//qDebug() << "curoff:" << curoff;
	//qDebug() << "fulloff:" << curoff + offset;
	uint8_t lendi = qFromLittleEndian<uint8_t>(curimg->ReadContent(offset + curoff, 1));
	//qDebug() << "lendi:" << lendi << "modulus:" << lendi % 2;
	if(lendi == 0x00)
	    break;
	uint8_t extattrlen = qFromLittleEndian<uint8_t>(curimg->ReadContent(offset + curoff + 1, 1));
	uint32_t extblk = qFromLittleEndian<uint32_t>(curimg->ReadContent(offset + curoff + 2, 4));
	uint16_t pardirid = qFromLittleEndian<uint16_t>(curimg->ReadContent(offset + curoff + 6, 2));
	QString dirid = QString::fromStdString(curimg->ReadContent(offset + curoff + 8, lendi).toStdString());
	//qDebug() << "curinode:" << curinode << "dirid:" << dirid.toUtf8() << "extblk:" << extblk << "pardirid:" << pardirid;
	// ADD NODE/PROPERTIES HERE FOR THE DIRECTORIES AND GET THEM IN THEIR PARENTS...
	if(idcnt == 1) // root directory
	{
	    qDebug() << "curinode:" << curinode;
	    //curinode = ParseDirectoryContents(curimg, ptreecnt, extblk, ?);
	    //curinode = ParseDirectoryContents(curimg, ptreecnt, pvrootdirblk.at(i), pvrootdirlen.at(i), 0, blocksize, false, curinode, "", &parsedblocks);
	    // parse root directory here...
	}
	if(idcnt > 1)
	{
	    idnodelink.insert(idcnt, QString::number(curinode) + "," + parpath + dirid + "/");
	    if(idnodelink.contains(pardirid))
	    {
		//parpath = parpath + dirid + "/";
		parpath = idnodelink.value(pardirid).split(",").at(1);
		parentstr += QString("-f" + idnodelink.value(pardirid).split(",").at(0));
	    }
	    QHash<QString, QVariant> nodedata;
	    nodedata.clear();
	    nodedata.insert("name", QByteArray(dirid.toUtf8()).toBase64());
	    nodedata.insert("path", QByteArray(parpath.toUtf8()).toBase64());
	    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(curinode)));
	    mutex.lock();
	    treenodemodel->AddNode(nodedata, parentstr, 3, 0);
	    mutex.unlock();
	    nodedata.clear();
	    //qDebug() << "dirid:" << dirid << "parpath:" << parpath << "parentstr:" << parentstr;
	    //dirlist->append(QString::number(idcnt) + "," + QString::number(curinode) + "," + QString::number(extblk) + "," + QString::number(pardirid));
	    /*
	    QTextStream out;
	    QFile fileprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(curinode) + ".prop");
	    if(!fileprop.isOpen())
		fileprop.open(QIODevice::Append | QIODevice::Text);
	    out.setDevice(&fileprop);
	    out.flush();
	    fileprop.close();
	    if(idnodelink.contains(pardirid))
		qDebug() << "dirid:" << dirid << "idcnt:" << idcnt << "paridcnt:" << pardirid << "parinode:" << idnodelink.value(pardirid);
	    else
		qDebug() << "dirid:" << dirid << "idcnt:" << idcnt << "paridcnt:" << pardirid << "parinode:" << "root and has no inode, -1";
	    QHash<QString, QVariant> nodedata;
	    nodedata.clear();
	    nodedata.insert("name", QByteArray(dirid.toUtf8()).toBase64());
	    nodedata.insert("path", QByteArray(filepath.toUtf8()).toBase64());
	    nodedata.insert("size", datalen);
	    if(parblk != 0)
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
	    nodedata.clear();
	    */
	    curinode++;
	}
	idcnt++;
	curoff = curoff + 8 + lendi;
	if(lendi % 2 != 0)
	    curoff++;
    }
    //qDebug() << "idnodelink:" << idnodelink;
    idnodelink.clear();
    return curinode;
}
quint64 ParseDirectoryContents(ForImg* curimg, uint8_t ptreecnt, uint32_t dirextblk, uint32_t dirdatalen, uint32_t parblk, uint32_t blocksize, bool utf16, quint64 parinode, QString parpath, QList<uint32_t>* parsedblocks) // FILE/DIRECTORY RECORD
{
    //qDebug() << "parinode:" << parinode;
    quint64 curinode = parinode;
    //if(parblk != 0 && parinode > 0)
    if(parblk != 0)
	curinode = parinode + 1;
    //qDebug() << "curinode:" << curinode;
    uint64_t extblkoff = dirextblk * blocksize;
    uint16_t curoff = 0; // starting point which goes to 2048
    qDebug() << "parpath:" << parpath << "dirdatalen:" << dirdatalen;
    while(curoff <= dirdatalen)
    {
	//qDebug() << "curoff:" << curoff;
	uint8_t lendr = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff, 1));
	// this is the issue. when the last entry is switching blocks, there are zero's which need to be skipped.. to get to
	// start of the next block...
	if(lendr == 0x00)
	{
	    curoff = ((curoff / blocksize) + 1) * blocksize;
	    qDebug() << "curoff:" << curoff;
	    lendr = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff, 1));
	    qDebug() << "lendr:" << lendr;
	    //qDebug() << "curofftest:" << curofftest;
	    //break;
	}
	uint8_t extattrlen = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 1, 1));
	uint32_t extblk = qFromLittleEndian<uint32_t>(curimg->ReadContent(extblkoff + curoff + 2, 4));
	uint8_t lenfi = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 32, 1));
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
		if(parblk != 0)
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
		curinode++;
		nodedata.clear();
		out.flush();
		fileprop.close();
		/*
		if(fileflags & 0x02) // directory
		{
		    qDebug() << "node id:" << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(curinode - 1)) << "datalen:" << datalen << "physize:" << physicalsize;
		    //qDebug() << "this is a directory, so parse with ParseDirectoryContents...";
		    curinode = ParseDirectoryContents(curimg, ptreecnt, extblk, physicalsize, dirextblk, blocksize, utf16, curinode - 1, QString(filepath + fileid + "/"), parsedblocks);
		}
		*/
	    }
	}
	//else
	//    qDebug() << "skipped because this dir/file record was already parsed.";
	//qDebug() << "lenfi:" << lenfi;
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

