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
    QHash<uint32_t, QString> parsedblocks;

    pvrootdirblk.clear();
    pvrootdirlen.clear();
    svrootdirblk.clear();
    svrootdirlen.clear();
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
		if(line.startsWith("SV" + QString::number(i) + " Data Length|"))
		    svrootdirlen.append(line.split("|").at(1).toUInt());
		if(line.startsWith("SV" + QString::number(i) + " Extent Location|"))
		    svrootdirblk.append(line.split("|").at(1).toUInt());
	    }
	}
        propfile.close();
    }
    
    for(int i=0; i < pvrootdirblk.count(); i++)
    {
	parsedblocks.insert(pvrootdirblk.at(i), "root");
	curinode = ParseDirectoryContents(curimg, ptreecnt, pvrootdirblk.at(i), pvrootdirlen.at(i), 0, blocksize, false, curinode, "", &parsedblocks);
    }
    for(int i=0; i < svrootdirblk.count(); i++)
    {
	parsedblocks.insert(svrootdirblk.at(i), "root");
	curinode = ParseDirectoryContents(curimg, ptreecnt, svrootdirblk.at(i), svrootdirlen.at(i), 0, blocksize, true, curinode, "", &parsedblocks);
    }
    parsedblocks.clear();
}

quint64 ParseDirectoryContents(ForImg* curimg, uint8_t ptreecnt, uint32_t dirextblk, uint32_t dirdatalen, uint32_t parblk, uint32_t blocksize, bool utf16, quint64 parinode, QString parpath, QHash<uint32_t, QString>* parsedblocks) // FILE/DIRECTORY RECORD
{
    quint64 curinode = parinode;
    if(parblk != 0)
	curinode = parinode + 1;
    uint64_t extblkoff = dirextblk * blocksize;
    uint16_t curoff = 0; // starting point which goes to 2048
    int blkcnt = dirdatalen / blocksize;
    if(dirdatalen % blocksize != 0)
	blkcnt++;
    for(int i=0; i < blkcnt; i++)
    {
	extblkoff = (dirextblk + i) * blocksize;
	curoff = 0;
	while(curoff < blocksize)
	{
	    uint8_t lendr = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff, 1));
	    if(lendr == 0x00)
		break;
	    uint8_t extattrlen = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 1, 1));
	    uint32_t extblk = qFromLittleEndian<uint32_t>(curimg->ReadContent(extblkoff + curoff + 2, 4));
	    uint8_t lenfi = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 32, 1));
	    QString fileid = "";
	    if(!utf16)
		fileid = QString::fromStdString(curimg->ReadContent(extblkoff + curoff + 33, lenfi).toStdString());
	    else
	    {
		for(uint8_t j=0; j < lenfi/2; j++)
		    fileid += QString(QChar(qFromBigEndian<uint16_t>(curimg->ReadContent(extblkoff + curoff + j*2 + 33, 2))));
	    }
	    fileid = fileid.left(fileid.indexOf(QChar(';')));
	    if(!parsedblocks->contains(extblk) || parsedblocks->contains(extblk) && parsedblocks->value(extblk) != fileid)
	    {
		uint32_t datalen = qFromLittleEndian<uint32_t>(curimg->ReadContent(extblkoff + curoff + 10, 4));
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
		uint8_t fileflags = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 25, 1));
		uint8_t fileunitsize = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 26, 1));
		uint8_t interleavegapsize = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 27, 1));
		uint16_t volseqnumber = qFromLittleEndian<uint16_t>(curimg->ReadContent(extblkoff + curoff + 28, 2));
		if(extblk != dirextblk && extblk != parblk)
		{
		    QString filepath = "/";
		    if(!parpath.isEmpty())
			filepath = parpath;
		    parsedblocks->insert(extblk, fileid);
		    if(extattrlen > 0) // parse extended attribute record
		    {
			qDebug() << "parse extended attr record here...";
		    }
		    QString curlayout = QString::number(extblk * blocksize) + "," + QString::number(physicalsize) + ";";
		    uint8_t itemtype = 5;
		    QTextStream out;
		    QFile fileprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(curinode) + ".prop");
		    if(!fileprop.isOpen())
			fileprop.open(QIODevice::Append | QIODevice::Text);
		    if(fileprop.isOpen())
		    {
			out.setDevice(&fileprop);
			out << "Layout|" << curlayout << "|File layout in offset,size; format." << Qt::endl;
			out << "Physical Size|" << QString::number(physicalsize) << "|Size of the blocks the file takes up in bytes." << Qt::endl;
			out << "Logical Size|" << QString::number(datalen) << "|Size in Bytes for the file." << Qt::endl;
			out.flush();
			fileprop.close();
		    }
		    QHash<QString, QVariant> nodedata;
		    nodedata.clear();
		    nodedata.insert("name", QByteArray(fileid.toUtf8()).toBase64());
		    nodedata.insert("path", QByteArray(filepath.toUtf8()).toBase64());
		    nodedata.insert("size", datalen);
		    if(fileflags & 0x02) // directory
			itemtype = 3;
		    if(datalen > 0) // Get Cat/Sig
		    {
			if(fileflags & 0x02) // directory
			{
			    nodedata.insert("cat", "Directory");
			    nodedata.insert("sig", "Directory");
			}
			else
			{
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
		    QString parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
		    if(parblk != 0)
			parentstr += QString("-f" + QString::number(parinode));
		    /*
		    nodedata.insert("create", createdate);
		    nodedata.insert("access", accessdate);
		    nodedata.insert("modify", modifydate);
		    */
		    mutex.lock();
		    treenodemodel->AddNode(nodedata, parentstr, itemtype, 0);
		    mutex.unlock();
		    if(nodedata.value("id").toString().split("-").count() == 3)
		    {
			listeditems.append(nodedata.value("id").toString());
			filesfound++;
			isignals->ProgUpd();
		    }
		    curinode++;
		    nodedata.clear();
		    if(fileflags & 0x02) // directory
		    {
			curinode = ParseDirectoryContents(curimg, ptreecnt, extblk, datalen, dirextblk, blocksize, utf16, curinode - 1, QString(filepath + fileid + "/"), parsedblocks);
		    }
		}
	    }
	    //else
	    //    qDebug() << "skipped because this dir/file record was already parsed.";
	    curoff = curoff + lendr;
	}
    }
    return curinode;
}

