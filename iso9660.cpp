#include "iso9660.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

void ParseIsoDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt)
{
    uint32_t blocksize = 0;
    uint8_t pvcount = 0;
    uint8_t svcount = 0;
    QList<uint32_t> pvpathtablesize;
    QList<uint32_t> pvpathtableblk;
    QList<uint32_t> pvrootdirblk;
    QList<uint32_t> svpathtablesize;
    QList<uint32_t> svpathtableblk;
    QList<uint32_t> svrootdirblk;

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
		else if(line.startsWith("PV" + QString::number(i) + " Extent Location|"))
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
		else if(line.startsWith("SV" + QString::number(i) + " Extent Location|"))
		    svrootdirblk.append(line.split("|").at(1).toUInt());
	    }
	}
        propfile.close();
    }
    // parse each pv path table
    for(int i=0; i < pvpathtableblk.count(); i++)
    {
	uint64_t curoff = pvpathtableblk.at(i) * blocksize;
	uint8_t lendi = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoff, 1));
	uint8_t extattrlen = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoff + 1, 1));
	uint32_t extblk = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoff + 2, 4));
	uint16_t pardirnum = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoff + 6, 2));
	QByteArray dirid = curimg->ReadContent(curoff + 8, lendi);
	qDebug() << "curoff:" << curoff << "lendi:" << lendi << "extattrlen:" << extattrlen << "extblk:" << extblk << "pardirnum:" << pardirnum << "dirid:" << dirid;
	ParseDirectoryContents(curimg, extblk, blocksize);
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
	ParseDirectoryContents(curimg, extblk, blocksize);
    }
    // PROCESS IS TO PARSE SMALL PATH TABLE RECORD, WHICH LEADS TO THE DIRECTORY RECORD WHICH THEN TAKES ME TO THE FILE RECORD INFO???
    /*
    qDebug() << "parse iso:" << curstartsector << ptreecnt;
    qDebug() << "pvcount:" << pvcount << "svcount:" << svcount;
    qDebug() << "pvpathtablesize:" << pvpathtablesize << "pvpathtableblk:" << pvpathtableblk << "pvrootdirblk:" << pvrootdirblk;
    qDebug() << "svpathtablesize:" << svpathtablesize << "svpathtableblk:" << svpathtableblk << "svrootdirblk:" << svrootdirblk;
    qDebug() << "blocksize:" << blocksize;
    */
}
void ParseDirectoryContents(ForImg* curimg, uint32_t dirextblk, uint32_t blocksize) // DIRECTORY RECORD
{
    uint64_t extblkoff = dirextblk * blocksize;
    uint16_t curoff = 0; // starting point which goes to 2048
    while(curoff <= 2048)
    {
	qDebug() << "curoff:" << curoff;
	uint8_t lendr = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff, 1));
	if(lendr == 0x00)
	    break;
	uint8_t extattrlen = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 1, 1));
	uint32_t extblk = qFromLittleEndian<uint32_t>(curimg->ReadContent(extblkoff + curoff + 2, 4));
	uint32_t datalen = qFromLittleEndian<uint32_t>(curimg->ReadContent(extblkoff + curoff + 10, 4));
	// recording date and time (18, 7) need to get 1 at a time and format like before...
	uint8_t fileflags = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 25, 1));
	uint8_t fileunitsize = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 26, 1));
	uint8_t interleavegapsize = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 27, 1));
	uint16_t volseqnumber = qFromLittleEndian<uint16_t>(curimg->ReadContent(extblkoff + curoff + 28, 4));
	uint8_t lenfi = qFromLittleEndian<uint8_t>(curimg->ReadContent(extblkoff + curoff + 32, 1));
	QString fileid = "";
	if(lenfi <= 11)
	    fileid = QString(curimg->ReadContent(extblkoff + curoff + 33, lenfi));
	else
	{
	    qDebug() << "curoff bfore filename:" << curoff << extblkoff + curoff;
	    for(uint8_t i=0; i < lenfi/2; i++)
		fileid += QString(QChar(qFromBigEndian<uint16_t>(curimg->ReadContent(extblkoff + curoff + i*2 + 33, 2))));
	    //int remaining = namelength + 16 - curlength;
	    //for(int m=1; m < remaining; m++)
		//filename += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + (j+k)*32 + m*2, 2))));
	}
	if(extblk != dirextblk)
	{
	    qDebug() << "lendr:" << lendr << "extattrlen:" << extattrlen << "extblk:" << extblk << "datalen:" << datalen << "file flags:" << QString::number(fileflags, 2) << QString::number(fileflags, 16) << "fileunitsize:" << fileunitsize << "interleavegapsize:" << interleavegapsize << "lenfi:" << lenfi << "fileid:" << fileid;
	    qDebug() << "file layout:" << QString(QString::number(extblk * 2048) + "," +  QString::number(datalen) + ";");
	}
	curoff = curoff + lendr;
    }
}

