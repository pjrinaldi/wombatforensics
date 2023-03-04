#include "newcase.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

void ProcessForensicImage(ForImg* curimg)
{
    qInfo() << "Parsing Forensic Image:" << curimg->ImgPath();
    isignals->StatUp("Parsing Forensic Image...");
    //qDebug() << "imgpath at start of parsingforensicimage:" << curimg->ImgPath();
    //qDebug() << "mount path at start of parseforensicimage:" << curimg->MountPath();
    //qDebug() << "imgsize:" << curimg->Size();
    QHash<QString, QVariant> nodedata;
    nodedata.clear();
    nodedata.insert("name", curimg->ImgPath().split("/").last());
    if(curimg->ImgType() == 15)
    {
	QString imgname = "/" + curimg->ImgPath().split("/").last();
	nodedata.insert("path", curimg->ImgPath().split(imgname).first());
    }
    //nodedata.insert("path", "0");
    nodedata.insert("size", curimg->Size());
    //nodedata.insert("create", "0");
    //nodedata.insert("access", "0");
    //nodedata.insert("modify", "0");
    //nodedata.insert("status", "0");
    //nodedata.insert("hash", "0");
    if(curimg->ImgType() == 15)
    {
	QString catsig = GenerateCategorySignature(curimg, curimg->ImgPath().split("/").last(), 0);
	nodedata.insert("cat", catsig.split("/").first());
	nodedata.insert("sig", catsig.split("/").last());
    }
    //else
    //{
	//nodedata.insert("cat", "0");
	//nodedata.insert("sig", "0");
    //}
    //nodedata.insert("tag", "0");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last()));
    //nodedata.insert("match", "0");

    /*
     * NEED TO WRITE THIS AS I GO.... AND CLEAR STRING AS I GO
     * MIGHT BE BETTER TO GENERATE THE PREVIEW REPORT AFTER ALL THIS WITHIN THE "UPDATESTATUS()" FUNCTION
    */
    QString reportstring = "";
    reportstring += "<div id='e" + curimg->MountPath().split("/").last().split("-e").last() + "'><table style='" + ReturnCssString(2) + "' width='98%'>";
    reportstring += "<tr style='" + ReturnCssString(3) + "'><th style='" + ReturnCssString(6) + "' colspan='2'>Evidence Item (E" + curimg->MountPath().split("/").last().split("-e").last() + "): " + curimg->ImgPath() + "</th></tr>";
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
    reportstring += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(7) + "'>Image Size:</td><td style='" + ReturnCssString(7) + "'>" + QString("%L1").arg(curimg->Size()) + " bytes</td></tr>";
    reportstring += "</table></div><br/>\n";
    mutex.lock();
    AddELinkItem(curimg->MountPath().split("/").last().split("-e").last().toInt(), curimg->ImgPath().split("/").last());
    AddEvidItem(reportstring);
    mutex.unlock();
    mutex.lock();
    treenodemodel->AddNode(nodedata, "-1", -1, -1);
    mutex.unlock();
    nodedata.clear();
    QFile estatfile(curimg->MountPath() + "/stat");
    QTextStream out;
    if(!estatfile.isOpen())
        estatfile.open(QIODevice::Append | QIODevice::Text);
    if(estatfile.isOpen())
    {
        out.setDevice(&estatfile);
        // original evidence filename, evidence mount string, imgsize, id is in the mount string
        out << curimg->ImgPath() << "," << curimg->MountPath() << "," << curimg->Size();
        out.flush();
        estatfile.close();
    }

    if(curimg->ImgType() == 15)
    {
	qInfo() << "File Loaded";
    }
    else
    {
	/*
	qDebug() << "Starting Parse Volume";
	ParseVolume(curimg, imgsize, &pofflist, &psizelist, &fsinfolist);
	*/
	qInfo() << "Reading Partition Table...";
	qint64 wlisig = qFromBigEndian<qint64>(curimg->ReadContent(0, 8));
	//qDebug() << "wlisig:" << QString::number(wlisig, 16);
	uint16_t mbrsig = qFromLittleEndian<uint16_t>(curimg->ReadContent(510, 2));
	uint16_t applesig = qFromLittleEndian<uint16_t>(curimg->ReadContent(0, 2)); // should be in 2nd sector, powerpc mac's not intel mac's
	uint32_t bsdsig = qFromLittleEndian<uint32_t>(curimg->ReadContent(0, 4)); // can be at start of partition entry of a dos mbr
	uint16_t sunsig = qFromLittleEndian<uint16_t>(curimg->ReadContent(508, 2)); // worry about it later, i386 sun can be at 2nd sector of partition entry of a dos mbr
	uint64_t gptsig = qFromLittleEndian<uint64_t>(curimg->ReadContent(0, 8));
	if(mbrsig == 0xaa55) // POSSIBLY MBR OR GPT
	{
	    if((uint8_t)qFromLittleEndian<uint8_t>(curimg->ReadContent(450, 1)) == 0xee) // GPT DISK
	    {
		gptsig = qFromLittleEndian<uint64_t>(curimg->ReadContent(512, 8));
		if(gptsig == 0x5452415020494645) // GPT PARTITION TABLE
		{
		    qInfo() << "GPT Partition Table Found. Parsing...";
		    uint32_t parttablestart = qFromLittleEndian<uint32_t>(curimg->ReadContent(584, 8));
		    uint16_t partentrycount = qFromLittleEndian<uint16_t>(curimg->ReadContent(592, 4));
		    uint16_t partentrysize = qFromLittleEndian<uint16_t>(curimg->ReadContent(596, 4));
		    uint8_t ptreecnt = 0; // partition counter to add unallocated in..
		    QDir dir; // current partition directory
		    QFile pstatfile; // current statfile
		    int pcount = 0;
		    for(int i=0; i < partentrycount; i++)
		    {
			int cnt = i*partentrysize;
			uint32_t curstartsector = qFromLittleEndian<uint32_t>(curimg->ReadContent(parttablestart*512 + cnt + 32, 8));
			uint32_t curendsector = qFromLittleEndian<uint32_t>(curimg->ReadContent(parttablestart*512 + cnt + 40, 8));
			if(curendsector - curstartsector > 0) // PARTITION VALUES MAKE SENSE
			    pcount++;
		    }
		    for(int i=0; i < pcount; i++)
		    {
			uint32_t sectorcheck = 0;
			int cnt = i*partentrysize;
			uint32_t curstartsector = qFromLittleEndian<uint32_t>(curimg->ReadContent(parttablestart*512 + cnt + 32, 8));
			uint32_t curendsector = qFromLittleEndian<uint32_t>(curimg->ReadContent(parttablestart*512 + cnt + 40, 8));
			if(i ==0) // INITIAL PARTITION
			    sectorcheck = 0;
			else if(i > 0 && i < partentrycount) // MIDDLE PARTITIONS
			    sectorcheck = qFromLittleEndian<uint32_t>(curimg->ReadContent(parttablestart*512 + (i-1)*partentrysize + 32, 8)) + qFromLittleEndian<uint32_t>(curimg->ReadContent(parttablestart*512 + (i-1)*partentrysize + 40, 8));
			else if(i == pcount - 1)
			    sectorcheck = curimg->Size()/512;
			if(curendsector - curstartsector > 0) // PARTITION VALUES MAKE SENSE
			{
			    if(curstartsector > sectorcheck) // UNALLOCATED PARTITION BEFORE THE FIRST PARTITION
			    {
				ParsePartition(curimg, sectorcheck, curstartsector, ptreecnt, 0);
				ptreecnt++;
			    }
			    // NOW ADD THE ALLOCATED PARTITION READ FROM THE PARTITION TABLE
			    ParsePartition(curimg, curstartsector, (curendsector - curstartsector + 1), ptreecnt, 1);
			    ptreecnt++;
			    if(i == pcount - 1) // ADD UNALLOCATED AFTER LAST VALID PARTITION IF EXISTS
			    {
				if(curendsector < curimg->Size())
				    ParsePartition(curimg, curendsector+1, curimg->Size()/512 - 1 - curendsector, ptreecnt, 0);
			    }
			}
			else // INVALID PARTITION ENTRY
			{
			    // ADD UNALLOCATED FROM START TO THE END SECTOR HERE
			    // shouldn't need this section so populate later.
			}
		    }
		}
	    }
	    else // MBR DISK
	    {
		QString exfatstr = QString::fromStdString(curimg->ReadContent(3, 5).toStdString());
		QString fatstr = QString::fromStdString(curimg->ReadContent(54, 5).toStdString());
		QString fat32str = QString::fromStdString(curimg->ReadContent(82, 5).toStdString());
		QString bfsstr = QString::fromStdString(curimg->ReadContent(544, 4).toStdString());
		if(exfatstr.startsWith("NTFS") || exfatstr == "EXFAT" || fatstr == "FAT12" || fatstr == "FAT16" || fat32str == "FAT32" || bfsstr == "1SFB") // NTFS | EXFAT | FAT12 | FAT16 | FAT32 | BFS W/O PARTITION TABLE
		{
		    ParsePartition(curimg, 0, curimg->Size()/512, 0, 1);
		}
		else // MBR
		{
		    qInfo() << "MBR Partition Table Found. Parsing...";
		    uint8_t ptreecnt = 0;
		    uint8_t pcount = 0;
		    for(int i=0; i < 4; i++)
		    {
			if(qFromLittleEndian<uint32_t>(curimg->ReadContent(458 + i*16, 4)) > 0)
			    pcount++;
		    }
		    for(uint8_t i=0; i < pcount; i++)
		    {
			//int cnt = i*16;
			uint8_t curparttype = qFromLittleEndian<uint8_t>(curimg->ReadContent(450 + (i*16), 1));
			uint32_t curoffset = qFromLittleEndian<uint32_t>(curimg->ReadContent(454 + (i*16), 4));
			uint32_t cursize = qFromLittleEndian<uint32_t>(curimg->ReadContent(458 + (i*16), 4));
			qint64 sectorcheck = 0;
			if(i == 0) // INITIAL PARTITION
			    sectorcheck = 0;
			else if(i > 0 && i < pcount - 1) // MIDDLE PARTITIONS
			    sectorcheck = qFromLittleEndian<uint32_t>(curimg->ReadContent(454 + (i-1)*16, 4)) + qFromLittleEndian<uint32_t>(curimg->ReadContent(458 + (i-1)*16, 4));
			else if(i == pcount - 1) // LAST PARTITION
			    sectorcheck = curimg->Size()/512;
			if(curoffset > sectorcheck) // ADD UNALLOCATED PARTITION
			{
			    //qDebug() << "unallocated partition before:" << i;
			    //qDebug() << "unalloc:" << ptreecnt << "curoffset:" << sectorcheck << "curend:" << (curoffset + sectorcheck - 1) << "cursize:" << sectorcheck + curoffset;
			    ParsePartition(curimg, sectorcheck, curoffset, ptreecnt, 0);
			    ptreecnt++;
			}
			if(curparttype == 0x05) // extended partition
			{
			    //qDebug() << "extendedpartition:" << curoffset << cursize;
			    ptreecnt = ParseExtendedPartition(curimg, curoffset, cursize, ptreecnt);
			    //qDebug() << "extended partition offset:" << curoffset << "size:" << cursize;
			    //ParseExtentedPartition(curimg, curoffset, cursize, j);
			    //ParseExtendedPartition(curimg, curoffset, curoffset, cursize, pofflist, psizelist, fsinfolist); // add fsinfolist here as well...
			}
			else if(curparttype == 0x00)
			{
			    //qDebug() << "do nothing here cause it is an empty partition...";
			}
			else if(curparttype == 0x82) // Sun i386
			{
			    // parse sun table here passing pofflist and psizelist
			}
			else if(curparttype == 0xa5 || curparttype == 0xa6 || curparttype == 0xa9) // BSD
			{
			    // parse bsd table here passing pofflist nad psizelist
			}
			else
			{
			    if(cursize > 0)
			    {
				//qDebug() << "ppart:" << ptreecnt << "curoffset:" << curoffset << "curend:" << (curoffset + cursize - 1) << "cursize:" << cursize;
				//qDebug() << "begin parse file system information";
				ParsePartition(curimg, curoffset, cursize, ptreecnt, 1);
				ptreecnt++;
			    }
			}
			if(i == pcount - 1 && curoffset + cursize < curimg->Size()/512 - 1) // ADD UNALLOCATED PARTITION AFTER ALL OTHER PARTITIONS
			{
			    //qDebug() << "add unallocated partition after last partition" << i;
			    ParsePartition(curimg, curoffset + cursize, curimg->Size()/512 - (curoffset + cursize), ptreecnt, 0);
			    //ParsePartition(curimg, curendsector+1, curimg->Size()/512 - 1 - curendsector, ptreecnt, 0);
			    //ptreecnt++;
			}
		    }
		}
	    }
	}
	else if(applesig == 0x504d) // APPLE PARTITION
	{
	    qDebug() << "apple sig here...";
	}
	else if(bsdsig == 0x82564557) // BSD PARTITION
	{
	    qDebug() << "bsd part here...";
	}
	else if(sunsig == 0xDABE) // SUN PARTITION
	{
	    qDebug() << "determine if sparc or i386 and then process partitions.";
	}
	else if(gptsig == 0x5452415020494645) // GPT PARTITION
	{
	    qInfo() << "GPT Partition Table Found. Parsing...";
	    uint32_t parttablestart = qFromLittleEndian<uint32_t>(curimg->ReadContent(584, 8));
	    uint16_t partentrycount = qFromLittleEndian<uint16_t>(curimg->ReadContent(592, 4));
	    uint16_t partentrysize = qFromLittleEndian<uint16_t>(curimg->ReadContent(596, 4));
	    uint8_t ptreecnt = 0; // partition counter to add unallocated in..
	    QDir dir; // current partition directory
	    QFile pstatfile; // current statfile
	    int pcount = 0;
	    for(int i=0; i < partentrycount; i++)
	    {
		int cnt = i*partentrysize;
		uint32_t curstartsector = qFromLittleEndian<uint32_t>(curimg->ReadContent(parttablestart*512 + cnt + 32, 8));
		uint32_t curendsector = qFromLittleEndian<uint32_t>(curimg->ReadContent(parttablestart*512 + cnt + 40, 8));
		if(curendsector - curstartsector > 0) // PARTITION VALUES MAKE SENSE
		    pcount++;
	    }
	    for(int i=0; i < pcount; i++)
	    {
		uint32_t sectorcheck = 0;
		int cnt = i*partentrysize;
		uint32_t curstartsector = qFromLittleEndian<uint32_t>(curimg->ReadContent(parttablestart*512 + cnt + 32, 8));
		uint32_t curendsector = qFromLittleEndian<uint32_t>(curimg->ReadContent(parttablestart*512 + cnt + 40, 8));
		if(i ==0) // INITIAL PARTITION
		    sectorcheck = 0;
		else if(i > 0 && i < partentrycount) // MIDDLE PARTITIONS
		    sectorcheck = qFromLittleEndian<uint32_t>(curimg->ReadContent(parttablestart*512 + (i-1)*partentrysize + 32, 8)) + qFromLittleEndian<uint32_t>(curimg->ReadContent(parttablestart*512 + (i-1)*partentrysize + 40, 8));
		else if(i == pcount - 1)
		    sectorcheck = curimg->Size()/512;
		if(curendsector - curstartsector > 0) // PARTITION VALUES MAKE SENSE
		{
		    if(curstartsector > sectorcheck) // UNALLOCATED PARTITION BEFORE THE FIRST PARTITION
		    {
			ParsePartition(curimg, sectorcheck, curstartsector, ptreecnt, 0);
			ptreecnt++;
		    }
		    // NOW ADD THE ALLOCATED PARTITION READ FROM THE PARTITION TABLE
		    ParsePartition(curimg, curstartsector, (curendsector - curstartsector + 1), ptreecnt, 1);
		    ptreecnt++;
		    if(i == pcount - 1) // ADD UNALLOCATED AFTER LAST VALID PARTITION IF EXISTS
		    {
			if(curendsector < curimg->Size())
			    ParsePartition(curimg, curendsector+1, curimg->Size()/512 - 1 - curendsector, ptreecnt, 0);
		    }
		}
		else // INVALID PARTITION ENTRY
		{
		    // ADD UNALLOCATED FROM START TO THE END SECTOR HERE
		    // shouldn't need this section so populate later.
		}
	    }
	}
	else if(wlisig == 0x776f6d6261746c69) // wombatli - wombat logical image signature (8 bytes)
	{
	    qInfo() << "Wombat Logical Image Found. Parsing...";
	    ParseLogicalImage(curimg);
	}
	else // NO PARTITION MAP, JUST A FS AT ROOT OF IMAGE
	{
	    ParsePartition(curimg, 0, curimg->Size()/512, 0, 1);
	    //qDebug() << "partition signature not found correctly";
	}
    }
    //FindPartitions(curimg, &pofflist, &psizelist);
}

void ParseLogicalImage(ForImg* curimg)
{
    //qDebug() << "parse logical image here...";
    //qDebug() << "img path:" << curimg->ImgPath();
    QFile wli(curimg->ImgPath());
    if(!wli.isOpen())
        wli.open(QIODevice::ReadOnly);
    QDataStream in(&wli);
    quint64 header;
    quint8 version;
    QString casenumber;
    QString examiner;
    QString description;
    in >> header >> version >> casenumber >> examiner >> description;
    //qDebug() << "cur pos:" << wli.pos();
    //qDebug() << QString::number(header, 16) << version;
    qint64 curoffset = wli.pos();
    //qDebug() << curoffset;
    QList<qint64> fileindxlist;
    while(!in.atEnd())
    {
        QByteArray skiparray = wli.read(1000);
        int isindx = skiparray.indexOf("wliindex");
        if(isindx >= 0)
            fileindxlist.append(curoffset + isindx);
        curoffset += 1000;
    }
    //qDebug() << "fileindxlist:" << fileindxlist;
    //qDebug() << "fileindxlist count:" << fileindxlist.count();
    for(int i=0; i < fileindxlist.count(); i++)
    {
        qint64 fileoffset = fileindxlist.at(i);
        qint64 lzfilesize = 0;
        if(i == fileindxlist.count() - 1)
            lzfilesize = wli.size() - fileindxlist.at(i);
        else
            lzfilesize = fileindxlist.at(i+1) - fileoffset;
        //qDebug() << "index:" << i << "fileoffset:" << fileoffset + 8 << "filesize:" << filesize;
        wli.seek(fileoffset);
        wli.read(8);
        //qDebug() << "wlistart:" << QString(wli.read(8));
        //qDebug() << "wli start:" << wli.read(8).toHex()
        QString filename;
        QString filepath;
        qint64 filesize;
        qint64 filecreate;
        qint64 filemodify;
        qint64 fileaccess;
        qint64 filestatus;
        QString srchash;
        QString catsig;
        quint8 itemtype;
        quint8 deleted;
        in >> filename >> filepath >> filesize >> filecreate >> fileaccess >> filemodify >> filestatus >> srchash >> catsig >> itemtype >> deleted;
        QHash<QString, QVariant> nodedata;
        QStringList filepathlist = filepath.split("/", Qt::SkipEmptyParts);
        QString curdirstr = "/";
        QString parentstr = "e" + curimg->MountPath().split("/").last().split("-e").last();
        for(int j=0; j < filepathlist.count(); j++)
        {
            parentstr = "e" + curimg->MountPath().split("/").last().split("-e").last();
            if(j > 0)
                parentstr += "-d" + QString::number(j-1);
            // NEED TO IMPLEMENT THE DIRECTORY CHECK CODE HERE.
            nodedata.clear();
            //qDebug() << "filename:" << filepathlist.at(j) << "filepath:" << curdirstr << 0 << 0 << 0 << 0 << 0 << 0 << 0 << "Directory/Directory" << 5 << 0;
            nodedata.insert("name", filepathlist.at(j));
            nodedata.insert("path", curdirstr);
            nodedata.insert("size", 0);
            nodedata.insert("create", 0);
            nodedata.insert("access", 0);
            nodedata.insert("modify", 0);
            nodedata.insert("status", 0);
            nodedata.insert("hash", 0);
            nodedata.insert("cat", "Directory");
            nodedata.insert("sig", "Directory");
            nodedata.insert("tag", 0);
            nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-d" + QString::number(j)));
	    //nodedata.insert("match", 0);
            mutex.lock();
            treenodemodel->AddNode(nodedata, parentstr, 3, 0);
            mutex.unlock();
            curdirstr += filepathlist.at(j) + "/";
        }
        //qDebug() << "parentstr:" << parentstr;
        parentstr = "e" + curimg->MountPath().split("/").last().split("-e").last() + "-d" + QString::number(filepathlist.count() - 1);
        //qDebug() << filename << filepath << filesize << filecreate << fileaccess << filemodify << filestatus << srchash << catsig << itemtype << deleted;
        nodedata.clear();
        nodedata.insert("name", filename);
        nodedata.insert("path", filepath);
        nodedata.insert("size", filesize);
        nodedata.insert("create", filecreate);
        nodedata.insert("access", fileaccess);
        nodedata.insert("modify", filemodify);
        nodedata.insert("status", filestatus);
        nodedata.insert("hash", srchash);
        nodedata.insert("cat", catsig.split("/").first());
        nodedata.insert("sig", catsig.split("/").last());
        nodedata.insert("tag", "0");
        nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-f" + QString::number(fileindxlist.at(i))));
	nodedata.insert("match", 0);
        mutex.lock();
        treenodemodel->AddNode(nodedata, parentstr, itemtype, deleted);
        mutex.unlock();

        //qDebug() << "cur pos before frame:" << wli.pos();
        //qDebug() << "filename:" << filename << "filesize:" << filesize;
        //qDebug() << "new restorepath:" << restoredir.absolutePath() + filepath + "/" + filename;
        /*
        QDir tmpdir;
        if(tmpdir.mkpath(restoredir.absolutePath() + filepath) == false)
            qDebug() << "Failed to create restored directory for current file.";
        QFile restorefile(restoredir.absolutePath() + filepath + "/" + filename);
        if(!restorefile.isOpen())
            restorefile.open(QIODevice::WriteOnly | QIODevice::Append);
        QDataStream out(&restorefile);
        // Decompress and Write Contents to the restored file
        char* cmpbuf = new char[200];
        LZ4F_dctx* lz4dctx;
        LZ4F_errorCode_t errcode;
        errcode = LZ4F_createDecompressionContext(&lz4dctx, LZ4F_getVersion());
        if(LZ4F_isError(errcode))
            qDebug() << "Create Error:" << LZ4F_getErrorName(errcode);
        size_t consumedsize = 0;
        size_t rawbufsize = 1000;
        char* rawbuf = new char[rawbufsize];
        size_t filled = 0;
        int firstchunk = 1;
        size_t ret = 1;
        while(ret != 0)
        {
            size_t readsize = firstchunk ? filled : in.readRawData(cmpbuf, 100);
            firstchunk = 0;
            const void* srcptr = (const char*)cmpbuf + consumedsize;
            consumedsize = 0;
            const void* const srcend = (const char*)srcptr + readsize;
            while(srcptr < srcend && ret != 0)
            {
                size_t dstsize = rawbufsize;
                size_t srcsize = (const char*)srcend - (const char*)srcptr;
                ret = LZ4F_decompress(lz4dctx, rawbuf, &dstsize, srcptr, &srcsize, NULL);
                //qDebug() << "dstsize:" << dstsize;
                if(LZ4F_isError(ret))
                    qDebug() << "Decompress Error:" << LZ4F_getErrorName(ret);
                if(dstsize > 0)
                {
                    int byteswrote = out.writeRawData(rawbuf, dstsize);
                    //qDebug() << "byteswrote:" << byteswrote;
                }
                srcptr = (const char*)srcptr + srcsize;
            }
            //qDebug() << "ret:" << ret;
        }
        restorefile.close();
        */
        //qDebug() << "restorefile size:" << restorefile.size();
        /*
        if(!restorefile.isOpen())
            restorefile.open(QIODevice::ReadOnly);
        restorefile.setFileTime(QDateTime::fromSecsSinceEpoch(filecreate, Qt::UTC), QFileDevice::FileBirthTime);
        restorefile.setFileTime(QDateTime::fromSecsSinceEpoch(fileaccess, Qt::UTC), QFileDevice::FileAccessTime);
        restorefile.setFileTime(QDateTime::fromSecsSinceEpoch(filemodify, Qt::UTC), QFileDevice::FileModificationTime);
        restorefile.setFileTime(QDateTime::fromSecsSinceEpoch(filestatus, Qt::UTC), QFileDevice::FileMetadataChangeTime);
        restorefile.close();
        qDebug() << "Exported" << QString(filepath + "/" + filename) << "to" << restoredir.absolutePath();
        */
    }
    //qDebug() << Qt::endl << "wombatexport finished at" << QDateTime::currentDateTime().toString("MM/dd/yyyy hh:mm:ss ap") << Qt::endl;
    wli.close();
}

void ParsePartition(ForImg* curimg, uint32_t cursectoroffset, uint32_t cursectorsize, uint8_t ptreecnt, uint8_t allocstatus)
{
    // ADD THE UNALLOCATED/ALLOCATED PARTITION
    QDir dir; // current partition directory
    QFile pstatfile; // current statfile
    QString pname = "";
    QTextStream out;
    dir.mkpath(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/");
    QHash<QString, QVariant> nodedata;
    pstatfile.setFileName(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/stat");
    if(!pstatfile.isOpen())
	pstatfile.open(QIODevice::Append | QIODevice::Text);
    if(pstatfile.isOpen())
    {
	out.setDevice(&pstatfile);
	// partition name, offset, size, partition type, id
	// DON'T THINK I NEED THE NAME, PARTITION TYPE, OR ID, MIGHT NOT EVEN NEED SIZE
	if(allocstatus == 0)
	    pname = "UNALLOCATED,";
	else if(allocstatus == 1)
            pname = ParseFileSystem(curimg, cursectoroffset, ptreecnt);
        // OFFSET, SIZE
	out << QString::number((qint64)cursectoroffset*512) << "," << QString::number((qint64)cursectorsize*512);
	out.flush();
	pstatfile.close();
    }
    //qDebug() << "partition id:" << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));

    QString reportstring = "";
    QString partstring = "";
    reportstring += "<table style='" + ReturnCssString(2) + "'>";
    //reportstring += "<table style='" + ReturnCssString(2) + "' width='98%'>";
    reportstring += "<tr style='" + ReturnCssString(15) + "'><td style='" + ReturnCssString(7) + "'>Partition (P" + QString::number(ptreecnt) + "):</td><td style='" + ReturnCssString(7) + "'>";
    partstring += "e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + ": ";
    if(allocstatus == 0)
    {
	qInfo() << "Partition" << QString::number(ptreecnt) << "is" << "Unallocated";
        partstring += "UNALLOCATED";
        reportstring += "UNALLOCATED";
    }
    else if(allocstatus == 1)
    {
        partstring += pname;
        reportstring += pname;
    }
    reportstring += "</td></tr>";
    reportstring += "<tr style='" + ReturnCssString(15) + "'><td style='" + ReturnCssString(7) + "'>Offset:</td><td style='" + ReturnCssString(7) + "'>" + QString::number((qint64)cursectoroffset * 512) + "</td></tr>";
    reportstring += "<tr style='" + ReturnCssString(15) + "'><td style='" + ReturnCssString(7) + "'>Size:</td><td style='" + ReturnCssString(7) + "'>" + QString::number((qint64)cursectorsize * 512) + "</td></tr>";

    // NEED TO ADD THE RELEVANT PARTITION/FS DATA TO THE TABLE FOR THE OUTPUT... THEN PARTITIONS ARE DONE AND I CAN MOVE ONTO
    // TAGGED ITEMS
    reportstring += "</table><br/>\n";
    //reportstring += "</table></div><br/>\n";
    mutex.lock();
    AddPartItem(reportstring, curimg->ImgPath());
    mutex.unlock();
    // I WILL NEED TO CREATE A FUNCTION TO INSERT THIS INFORMATION INTO THE EVIDENCE AREA UNDER THE CORRECT EVIDENCE HTML STRING..
    // OOH, WHAT A PAIN THIS IS TO OFFER A PREVIEW REPORT....
    /*
    reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(ptreecnt) + "):</td><td>UNALLOCATED</td></tr>";
    reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(ptreecnt) + "):</td><td>" + fsinfolist.at(i).value("vollabel").toString() + " [" + fsinfolist.at(i).value("typestr").toString() + "]</td></tr>";
    */
    partitionlist.append(partstring);
    //partitionlist.append("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + ": UNALLOCATED");
    nodedata.clear();
    if(allocstatus == 0)
	nodedata.insert("name", "UNALLOCATED");
    else if(allocstatus == 1)
	nodedata.insert("name", pname);
    //nodedata.insert("path", "0");
    nodedata.insert("size", QString::number((qint64)cursectorsize*512));
    //nodedata.insert("create", "0");
    //nodedata.insert("access", "0");
    //nodedata.insert("modify", "0");
    //nodedata.insert("status", "0");
    //nodedata.insert("hash", "0");
    //nodedata.insert("cat", "0");
    //nodedata.insert("sig", "0");
    //nodedata.insert("tag", "0");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt)));
    //nodedata.insert("match", 0);
    mutex.lock();
    treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last()), -1, 0);
    mutex.unlock();
    // FILE CARVING DIRECTORIES
    nodedata.clear();
    nodedata.insert("name", QByteArray("orphans").toBase64());
    nodedata.insert("path", QByteArray("/").toBase64());
    //nodedata.insert("size", "0");
    //nodedata.insert("create", "0");
    //nodedata.insert("access", "0");
    //nodedata.insert("modify", "0");
    //nodedata.insert("status", "0");
    //nodedata.insert("hash", "0");
    nodedata.insert("cat", "Directory");
    nodedata.insert("sig", "Virtual Directory");
    //nodedata.insert("tag", "0");
    //nodedata.insert("match", "0");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-o"));
    //nodedata.insert("match", 0);
    mutex.lock();
    treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt)), 11, 0);
    mutex.unlock();
    nodedata.clear();
    if(allocstatus == 1)
        ParseDirectoryStructure(curimg, cursectoroffset, ptreecnt);
    nodedata.insert("name", QByteArray("carved validated").toBase64());
    /*
    nodedata.insert("path",  "0");
    nodedata.insert("size", "0");
    nodedata.insert("create", "0");
    nodedata.insert("access", "0");
    nodedata.insert("modify", "0");
    nodedata.insert("status", "0");
    nodedata.insert("hash", "0");
    */
    nodedata.insert("cat", "Directory");
    nodedata.insert("sig", "Virtual Directory");
    //nodedata.insert("tag", "0");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-cv"));
    //nodedata.insert("match", 0);
    mutex.lock();
    treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt)), 11, 0);
    mutex.unlock();
    nodedata.clear();
    nodedata.insert("name", QByteArray("carved unvalidated").toBase64());
    /*
    nodedata.insert("path",  "0");
    nodedata.insert("size", "0");
    nodedata.insert("create", "0");
    nodedata.insert("access", "0");
    nodedata.insert("modify", "0");
    nodedata.insert("status", "0");
    nodedata.insert("hash", "0");
    */
    nodedata.insert("cat", "Directory");
    nodedata.insert("sig", "Virtual Directory");
    //nodedata.insert("tag", "0");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-cu"));
    //nodedata.insert("match", 0);
    mutex.lock();
    treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt)), 11, 0);
    mutex.unlock();
}

uint8_t ParseExtendedPartition(ForImg* curimg, uint32_t curstartsector, uint32_t cursectorsize, uint8_t ptreecnt)
{
    uint8_t pcount = 0;
    for(int i=0; i < 4; i++)
    {
	if(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 458 + i*16, 4)) > 0)
	    pcount++;
    }
    for(uint8_t i = 0; i < pcount; i++)
    {
	uint8_t curparttype = qFromLittleEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 450 + i*16, 1));
	uint32_t curoffset = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 454 + i*16, 4));
	uint32_t cursize = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 458 + i*16, 4));
	if(i == 0 && curoffset > 0) // ADD INITIAL UNALLOCATED PARTITION
	{
	    //qDebug() << "unalloc:" << ptreecnt << "curoffset:" << curstartsector << "curend:" << curstartsector + curoffset - 1 << "cursize:" << curoffset;
	    ParsePartition(curimg, curstartsector, curoffset, ptreecnt, 0);
	    ptreecnt++;
	}
        if(i > 0 && i < pcount - 1)
        {
            uint32_t sectorcheck = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 454 + (i-1)*16, 4)) + qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 458 + (i-1)*16, 4));
            if(curoffset > sectorcheck) // ADD UNALLOCATED PARTITION
            {
                ParsePartition(curimg, sectorcheck, curoffset - sectorcheck, ptreecnt, 0);
                ptreecnt++;
            }
        }
	if(curparttype == 0x05) // ANOTHER EXTENDED PARTITION
	{
	    //qDebug() << "extended partition found:" << ptreecnt << "curglobal offset:" << curstartsector + curoffset;
	    ptreecnt = ParseExtendedPartition(curimg, curstartsector + curoffset, cursize, ptreecnt);
	}
	else if(curparttype == 0x00) // do nothing
	{
	}
	else // some other allocated partition
	{
	    //qDebug() << "epart:" << ptreecnt << "globaloff:" << curstartsector + curoffset << "curend:" << (curoffset + cursize - 1) << "cursize:" << cursize << "curoffset:" << curoffset;
	    ParsePartition(curimg, curstartsector + curoffset, cursize, ptreecnt, 1);
	    ptreecnt++;
	}
	if(i == pcount - 1 && curstartsector + curoffset + cursize < cursectorsize)
	{
	    //qDebug() << "unalloc:" << ptreecnt << "curoffset:" << curstartsector << "curend:" << curstartsector + curoffset - 1 << "cursize:" << curoffset;
	    ParsePartition(curimg, curstartsector + curoffset + cursize, cursectorsize - (curstartsector + curoffset + cursize), ptreecnt, 0);
	}
    }

    return ptreecnt;
}

QString ParseFileSystem(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt)
{
    QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/prop");
    QTextStream out;
    if(!propfile.isOpen())
	propfile.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&propfile);

    QString partitionname = "";
    uint16_t winsig = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 510, 2));
    uint16_t extsig = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 1080, 2));
    QString apfsig = QString::fromStdString(curimg->ReadContent(curstartsector*512 + 32, 4).toStdString());
    QString hfssig = QString::fromStdString(curimg->ReadContent(curstartsector*512 + 1024, 2).toStdString());
    QString xfssig = QString::fromStdString(curimg->ReadContent(curstartsector*512, 4).toStdString());
    QString btrsig = QString::fromStdString(curimg->ReadContent(curstartsector*512 + 65600, 8).toStdString());
    QString btlsig = QString::fromStdString(curimg->ReadContent(curstartsector*512, 8).toStdString());
    QString bfssig = QString::fromStdString(curimg->ReadContent(curstartsector*512 + 544, 4).toStdString());
    QString isosig = QString::fromStdString(curimg->ReadContent(curstartsector*512 + 32769, 5).toStdString());
    QString udfsig = QString::fromStdString(curimg->ReadContent(curstartsector*512 + 40961, 5).toStdString());
    uint64_t refsig = qFromLittleEndian<uint64_t>(curimg->ReadContent(curstartsector*512 + 3, 8)); // should be 0x00 00 00 00 53 46 65 52 (0 0 0 0 S F e R) prior to endian flip
    uint32_t f2fsig = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1024, 4));
    quint64 zfssig = qFromLittleEndian<quint64>(curimg->ReadContent(curstartsector*512 + 135168, 8));
    quint64 bcfsig1 = qFromBigEndian<quint64>(curimg->ReadContent(curstartsector*512 + 4120, 8));
    quint64 bcfsig2 = qFromBigEndian<quint64>(curimg->ReadContent(curstartsector*512 + 4128, 8));
    uint32_t zonesig = qFromBigEndian<uint32_t>(curimg->ReadContent(0, 4));
    // zonefs magic number = 0x5a4f4653 // ZOFS
    //0xE0F5E1E2
    // WILL WRITE FILE SYSTEM INFORMATION IN THIS FUNCTION AND ONLY RETURN THE QSTRING(FILESYSTEMNAME,FILESYSTEMTYPE) TO BE USED BY THE PARTITION
    if(winsig == 0xaa55 && bfssig != "1SFB") // FAT OR NTFS OR BFS
    {
	QString exfatstr = QString::fromStdString(curimg->ReadContent(curstartsector*512 + 3, 5).toStdString());
	QString fatstr = QString::fromStdString(curimg->ReadContent(curstartsector*512 + 54, 5).toStdString());
	QString fat32str = QString::fromStdString(curimg->ReadContent(curstartsector*512 + 82, 5).toStdString());
	if(fatstr == "FAT12" || fatstr == "FAT16" || fat32str == "FAT32" || exfatstr == "EXFAT") // FAT12 | FAT16 | FAT32 | EXFAT
	{
            if(fatstr == "FAT12" || fatstr == "FAT16")
            {
                if(fatstr == "FAT12")
                {
		    qInfo() << "FAT12 File System Found. Parsing...";
                    out << "File System Type Int|1|Internal File System Type represented as an integer." << Qt::endl;
	            out << "File System Type|FAT12|File System Type String." << Qt::endl;
                }
                else if(fatstr == "FAT16")
                {
		    qInfo() << "FAT16 File System Found. Parsing...";
                    out << "File System Type Int|2|Internal File System Type represented as an integer." << Qt::endl;
	            out << "File System Type|FAT16|File System Type String." << Qt::endl;
                }
            }
            else if(fat32str == "FAT32")
            {
		qInfo() << "FAT32 File System Found. Parsing...";
                out << "File System Type Int|3|Internal File System Type represented as an integer." << Qt::endl;
	        out << "File System Type|FAT32|File System Type String." << Qt::endl;
            }
            else if(exfatstr == "EXFAT")
            {
		qInfo() << "EXFAT File System Found. Parsing...";
                out << "File System Type Int|4|Internal File System Type represented as an integer." << Qt::endl;
	        out << "File System Type|EXFAT|File System Type String." << Qt::endl;
            }
            uint16_t bytespersector = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 11, 2));
            uint8_t fatcount = qFromLittleEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 16, 1));
            uint8_t sectorspercluster = qFromLittleEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 13, 1));
            //uint16_t bytesperclutser = bytespersector * sectorspercluster;
            uint16_t reservedareasize = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 14, 2));
            uint16_t rootdirmaxfiles = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 17, 2));
            out << "Reserved Area Size|" << QString::number(reservedareasize) << "|Size of the reserved area at the beginning of the file system." << Qt::endl;
            out << "Root Directory Max Files|" << QString::number(rootdirmaxfiles) << "|Maximum number of root directory entries." << Qt::endl;
            if(fatstr == "FAT12" || fatstr == "FAT16" || fat32str == "FAT32")
            {
                out << "FAT Count|" << QString::number(fatcount) << "| Number of FAT's in the file system." << Qt::endl;
                out << "Bytes Per Sector|" << QString::number(bytespersector) << "|Number of bytes per sector, usually 512." << Qt::endl;
                out << "Sectors Per Cluster|" << QString::number(sectorspercluster) << "|Number of sectors per cluster." << Qt::endl;
                /* POSSIBLY COMMON BETWEEN THE 4 FAT VERSIONS
                fsinfo.insert("mediatype", QVariant(partbuf.at(21)));
                 */ 
                out << "File System Sector Count|";
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 19, 2)) == 0)
                    out << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 32, 4)));
                else
                    out << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 19, 2)));
                out << "|Total sectors in the volume." << Qt::endl;
            }
            //qulonglong rootdiroffset = 0;
            if(fatstr == "FAT12" || fatstr == "FAT16")
            {
                uint16_t fatsize = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 22, 2));
                out << "FAT Size|" << QString::number(fatsize) << "|Size of the FAT." << Qt::endl;
                out << "Volume Serial Number|0x" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 39, 4)), 16) << "|Serial number for the volume." << Qt::endl;
                partitionname += QString::fromStdString(curimg->ReadContent(curstartsector*512 + 43, 11).toStdString());
                out << "Volume Label|" << partitionname << "|Label for the file system volume." << Qt::endl;
                partitionname +=  " [" + fatstr + "]";
                out << "FAT Offset|" << QString::number((qulonglong)(curstartsector*512 + reservedareasize * bytespersector)) << "|Byte offset to the start of the first FAT" << Qt::endl;
                out << "Root Directory Offset|" << QString::number((qulonglong)(curstartsector*512 + (reservedareasize + fatcount * fatsize) * bytespersector)) << "|Byte offset for the root directory" << Qt::endl;
                out << "Root Directory Sectors|" << QString::number(((rootdirmaxfiles * 32) + (bytespersector - 1)) / bytespersector) << "|Number of sectors for the root directory." << Qt::endl;
                out << "Root Directory Size|" << QString::number((rootdirmaxfiles * 32) + (bytespersector - 1)) << "|Size in bytes for the root directory." << Qt::endl;
                out << "Cluster Area Start|" << QString::number((qulonglong)(curstartsector + reservedareasize + fatcount * fatsize + ((rootdirmaxfiles * 32) + (bytespersector - 1)) / bytespersector)) << "|Byte offset to the start of the cluster area." << Qt::endl;
                out << "Root Directory Layout|" << QString(QString::number((qulonglong)(curstartsector*512 + (reservedareasize + fatcount * fatsize) * bytespersector)) + "," + QString::number((rootdirmaxfiles * 32) + (bytespersector - 1)) + ";") << "|Layout for the root directory." << Qt::endl;
                /*
                fsinfo.insert("extbootsig", QVariant(partbuf.at(38)));
                fsinfo.insert("fatlabel", QVariant(QString::fromStdString(partbuf.mid(54, 8).toStdString())));
                 */ 
            }
            else if(fat32str == "FAT32")
            {
                uint32_t fatsize = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 36, 4));
                out << "FAT Size|" << QString::number(fatsize) << "|Size of the FAT." << Qt::endl;
                partitionname += QString::fromStdString(curimg->ReadContent(curstartsector*512 + 71, 11).toStdString());
                out << "Volume Label|" << partitionname << "|Label for the file system volume." << Qt::endl;
                partitionname += " [FAT32]";
                out << "Volume Serial Number|0x" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 67, 4))) << "|Serial number for the volume." << Qt::endl;
                qulonglong fatoffset = (qulonglong)(curstartsector*512 + (reservedareasize * bytespersector));
                out << "FAT Offset|" << QString::number(fatoffset) << "|Byte offset to the start of the first FAT." << Qt::endl;
                uint32_t rootdircluster = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 44, 4));
                qulonglong rootdiroffset = (qulonglong)(curstartsector*512 + (reservedareasize + fatcount * fatsize) * bytespersector);
		out << "Root Directory Cluster|" << QString::number(rootdircluster) << "|Clutser offset to the start of the root directory." << Qt::endl;
		out << "Root Directory Offset|" << QString::number(rootdiroffset) << "|Byte offset to the start of the root directory." << Qt::endl;
                qulonglong clusterareastart = (qulonglong)(curstartsector + reservedareasize + (fatcount * fatsize));
                out << "Cluster Area Start|" << QString::number(clusterareastart) << "|Byte offset to the start of the cluster area." << Qt::endl;
                QList<uint> clusterlist;
                clusterlist.clear();
                // FirstSectorOfCluster = ((N-2) * sectorspercluster) + firstdatasector [rootdirstart];
                if(rootdircluster >= 2)
                {
                    clusterlist.append(rootdircluster);
                    GetNextCluster(curimg, rootdircluster, 3, fatoffset, &clusterlist);
                }
                out << "Root Directory Layout|" << ConvertBlocksToExtents(clusterlist, sectorspercluster * bytespersector, rootdiroffset) << "|Layout for the root directory";
                //qDebug() << "Extent String:" << ConvertBlocksToExtents(clusterlist, sectorspercluster * bytespersector, rootdiroffset);
                clusterlist.clear();
                /*
                fsinfo.insert("extbootsig", QVariant(partbuf.at(66)));
                fsinfo.insert("fatlabel", QVariant(QString::fromStdString(partbuf.mid(82, 8).toStdString())));
                */
            }
            else if(exfatstr == "EXFAT")
            {
                uint32_t fatsize = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 84, 4));
                out << "FAT Size|" << QString::number(fatsize) << "|Size of the FAT." << Qt::endl;
                bytespersector = pow(2, qFromLittleEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 108, 1))); // power of 2 so 2^(bytespersector)
                sectorspercluster = pow(2, qFromLittleEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 109, 1))); // power of 2 so 2^(sectorspercluster)
                out << "Bytes Per Sector|" << QString::number(bytespersector) << "|Number of bytes per sector, usually 512." << Qt::endl;
                out << "Sectors Per Cluster|" << QString::number(sectorspercluster) << "|Number of sectors per cluster." << Qt::endl;
                fatcount = qFromLittleEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 110, 1)); // 1 or 2, 2 if TexFAT is in use
                out << "FAT Count|" << QString::number(fatcount) << "| Number of FAT's in the file system." << Qt::endl;
                qulonglong fatoffset = (qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 80, 4)) * bytespersector) + (curstartsector * 512);
                out << "FAT Offset|" << QString::number(fatoffset) << "|Byte offset to the start of the first FAT." << Qt::endl;
                uint32_t rootdircluster = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 96, 4));
                uint32_t clusterstart = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 88, 4));
                out << "Cluster Area Start|" << QString::number(clusterstart) << "|Sector offset to the start of the cluster area." << Qt::endl;
                qulonglong rootdiroffset = (qulonglong)((curstartsector * 512) + (((rootdircluster - 2) * sectorspercluster) + clusterstart) * bytespersector);
                out << "Root Directory Offset|" << QString::number(rootdiroffset) << "|Byte offset to the start of the root directory." << Qt::endl;
                out << "Root Directory Size|" << QString::number(((rootdirmaxfiles * 32) + (bytespersector - 1))) << "|Size in bytes of the root directory." << Qt::endl;
                //fsinfo.insert("rootdirsectors", QVariant(((fsinfo.value("rootdirmaxfiles").toUInt() * 32) + (fsinfo.value("bytespersector").toUInt() - 1)) / fsinfo.value("bytespersector").toUInt()));
                //fsinfo.insert("rootdirsize", QVariant(fsinfo.value("rootdirsectors").toUInt() * fsinfo.value("bytespersector").toUInt()));
                QList<uint> clusterlist;
                clusterlist.clear();
                if(rootdircluster >= 2)
                {
                    clusterlist.append(rootdircluster);
                    GetNextCluster(curimg, rootdircluster, 4, fatoffset, &clusterlist);
                }
                QString rootdirlayout = ConvertBlocksToExtents(clusterlist, sectorspercluster * bytespersector, clusterstart * bytespersector);
                out << "Root Directory Layout|" << rootdirlayout << "|Layout for the root directory";
                //qDebug() << "Extent String:" << rootdirlayout;
                clusterlist.clear();
                for(int i=0; i < rootdirlayout.split(";", Qt::SkipEmptyParts).count(); i++)
                {
                    uint curoffset = 0;
                    qDebug() << "i:" << i << "rootdiroffset:" << rootdirlayout.split(";").at(i).split(",").at(0) << "rootdirlength:" << rootdirlayout.split(";").at(i).split(",").at(1);
                    while(curoffset < rootdirlayout.split(";").at(i).split(",").at(1).toUInt())
                    {
                        if(qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdirlayout.split(";").at(i).split(",").at(0).toULongLong() + curoffset, 1)) == 0x83)
                            break;
                        curoffset += 32;
                    }
                    qDebug() << "curoffset:" << curoffset;
                    if(curoffset < rootdirlayout.split(";").at(i).split(",").at(1))
                    {
                        if(qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdirlayout.split(";").at(i).split(",").at(0).toULongLong() + curoffset + 1, 1)) > 0)
                        {
                            for(int j=0; j < qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdirlayout.split(";").at(i).split(",").at(0).toULongLong() + curoffset + 1, 1)); j++)
                                partitionname += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdirlayout.split(";").at(i).split(",").at(0).toULongLong() + curoffset + 2 + j*2, 2))));
                            qDebug() << "partitionname:" << partitionname;
                            out << "Volume Label|" << partitionname << "|Label for the file system volume." << Qt::endl;
                            partitionname += " [EXFAT]";
                        }
                    }
                }
            }
	}
	else if(exfatstr.startsWith("NTFS")) // NTFS
	{
	    qInfo() << "NTFS File System Found. Parsing...";
	    out << "File System Type Int|5|Internal File System Type represented as an integer." << Qt::endl;
	    out << "File System Type|NTFS|File System Type String." << Qt::endl;
	    out << "Bytes Per Sector|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 11, 2))) << "|Number of bytes per sector, usually 512." << Qt::endl;
	    out << "Sectors Per Cluster|" << QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 13, 1))) << "|Number of sectors per cluster." << Qt::endl;
	    out << "Total Sectors|" << QString::number(qFromLittleEndian<qulonglong>(curimg->ReadContent(curstartsector*512 + 40, 8))) << "|Number of sectors in the file system." << Qt::endl;
	    out << "Bytes Per Cluster|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 11, 2)) * qFromLittleEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 13, 1))) << "|Number of bytes per cluster" << Qt::endl;
	    out << "MFT Starting Cluster|" << QString::number(qFromLittleEndian<qulonglong>(curimg->ReadContent(curstartsector*512 + 48, 8))) << "|Starting cluster number for the MFT" << Qt::endl;
	    out << "MFT Starting Offset|" << QString::number(curstartsector*512 + qFromLittleEndian<qulonglong>(curimg->ReadContent(curstartsector*512 + 48, 8)) * qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 11, 2)) * qFromLittleEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 13, 1))) << "|Starting byte for the MFT" << Qt::endl;
	    out << "MFT Entry Size|" << QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 64, 1))) << "|Entry size in clusters for an MFT Entry" << Qt::endl;
            out << "MFT Entry Bytes|1024| Entry size in bytes for an MFT Entry" << Qt::endl; // entrysize is stored at offset 64, then it should be entrysize * bytespercluster
	    out << "Serial Number|" << QString("0x" + QString::number(qFromLittleEndian<qulonglong>(curimg->ReadContent(curstartsector*512 + 72, 8)), 16)) << "|Serial number for the file system volume" << Qt::endl;
            out << "Index Record Size|" << QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 68, 1))) << "|Index record size for an index record." << Qt::endl;

            uint bytespercluster = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 11, 2)) * qFromLittleEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 13, 1));
            qint64 mftoffset = curstartsector*512 + qFromLittleEndian<qulonglong>(curimg->ReadContent(curstartsector*512 + 48, 8)) * bytespercluster;
            // GET THE MFT LAYOUT TO WRITE TO PROP FILE
            if(QString::fromStdString(curimg->ReadContent(mftoffset, 4).toStdString()) == "FILE") // a proper MFT entry
            {
                int curoffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(mftoffset + 20, 2)); // mft offset + offset to first attribute
                for(int i=0; i < qFromLittleEndian<uint16_t>(curimg->ReadContent(mftoffset + 40, 2)); i++) // loop over attributes until hit attribute before the next attribute id
                {
                    if(qFromLittleEndian<uint32_t>(curimg->ReadContent(mftoffset + curoffset, 4)) == 0x80 && qFromLittleEndian<uint8_t>(curimg->ReadContent(mftoffset + curoffset + 9, 1)) == 0) // attrtype | namelength > default$DATA attribute to parse
                        break;
                    curoffset += qFromLittleEndian<uint32_t>(curimg->ReadContent(mftoffset + curoffset + 4, 4)); // attribute length
                }
                QString runliststr = "";
                quint64 mftsize = 0;
                GetRunListLayout(curimg, curstartsector, bytespercluster, 1024, mftoffset + curoffset, &runliststr);
                //qDebug() << "runliststr for MFT Layout:" << runliststr;
		for(int j=0; j < runliststr.split(";", Qt::SkipEmptyParts).count(); j++)
		{
		    mftsize += runliststr.split(";", Qt::SkipEmptyParts).at(j).split(",").at(1).toULongLong();
		}
                //qDebug() << "runliststr for MFT Layout:" << runliststr << "mft size:" << mftsize;
	        out << "MFT Layout|" << runliststr << "|Layout for the MFT in starting offset, size; format" << Qt::endl;
                out << "Max MFT Entries|" << QString::number((mftsize)/1024) << "|Max MFT Entries allowed in the MFT" << Qt::endl;
                runliststr = "";
                mftsize = 0;
            }
            // GET VOLUME LABEL FROM THE $VOLUME_NAME SYSTEM FILE
            if(QString::fromStdString(curimg->ReadContent(mftoffset + 3 * 1024, 4).toStdString()) == "FILE") // a proper MFT entry
            {
                int curoffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(mftoffset + 3*1024 + 20, 2)); // offset to first attribute
                for(uint i=0; i < qFromLittleEndian<uint16_t>(curimg->ReadContent(mftoffset + 3*1024 + 40, 2)); i++) // loop over attributes until get to next attribute id
                {
                    if(qFromLittleEndian<uint32_t>(curimg->ReadContent(mftoffset + 3*1024 + curoffset, 4)) == 0x60) // $VOLUME_NAME attribute to parse (always resident)
                        break;
                    curoffset += qFromLittleEndian<uint32_t>(curimg->ReadContent(mftoffset + 3*1024 + curoffset + 4, 4));
                }
                for(uint k=0; k < qFromLittleEndian<uint32_t>(curimg->ReadContent(mftoffset + 3*1024 + curoffset + 16, 4))/2; k++)
                    partitionname += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(mftoffset + 3*1024 + curoffset + qFromLittleEndian<uint16_t>(curimg->ReadContent(mftoffset + 3*1024 + curoffset + 20, 2)) + k*2, 2))));
                out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
                partitionname += " [NTFS]";
            }
	}
    }
    else if(extsig == 0xef53) // EXT2/3/4
    {
        out << "File System Type Int|6|Internal File System Type represented as an integer." << Qt::endl;
        for(int i=0; i < 16; i++)
        {
            if(qFromBigEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 1144 + i, 1)) == 0x00)
                break;
            else
                partitionname += QString::fromStdString(curimg->ReadContent(curstartsector*512 + 1144 + i, 1).toStdString());
        }
        //qDebug() << "partition name:" << partitionname;
        out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        out << "Created Time|" << QDateTime::fromSecsSinceEpoch(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1288, 4)), QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP") << "|Creation time for the file system." << Qt::endl;
        out << "Mount Time|" << QDateTime::fromSecsSinceEpoch(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1068, 4)), QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP") << "|Mount time for the file system." << Qt::endl;
        out << "Write Time|" << QDateTime::fromSecsSinceEpoch(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1072, 4)), QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP") << "|Write time for the file system." << Qt::endl;
        out << "Last Check Time|" << QDateTime::fromSecsSinceEpoch(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1088, 4)), QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP") << "|Last check time for the file system." << Qt::endl;
        out << "Current State|";
        if(qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 1082, 2)) == 0x01)
            out << "Cleanly unmounted";
        else
            out << "Errors detected";
        out << "|Condition of the file system at lsat unmount." << Qt::endl;
        out << "Compatible Features|";
        uint32_t compatflags = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1116, 4));
        if(compatflags & 0x200)
            out << "Sparse Super Block,";
        if(compatflags & 0x100)
            out << "Exclude Bitmap,";
        if(compatflags & 0x80)
            out << "Exclude Inodes,";
        if(compatflags & 0x40)
            out << "Lazy Block Groups,";
        if(compatflags & 0x20)
            out << "Indexed Directories,";
        if(compatflags & 0x10)
            out << "Reserved GDT,";
        if(compatflags & 0x08)
            out << "Extended Attributes,";
        if(compatflags & 0x04)
            out << "Journal,";
        if(compatflags & 0x02)
            out << "Imagic Inodes,";
        if(compatflags & 0x01)
            out << "Directory preallocation";
        out << "|File system compatible feature set." << Qt::endl;
        uint32_t incompatflags = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1120, 4));
        out << "Incompatible Features|";
	if(incompatflags & 0x10000)
	    out << "Encrypted inodes,";
	if(incompatflags & 0x8000)
	    out << "Data in inode,";
	if(incompatflags & 0x4000)
	    out << "Large Directory >2GB or 3-level htree,";
	if(incompatflags & 0x2000)
	    out << "Metadata checksum seed in superblock,";
	if(incompatflags & 0x1000)
	    out << "Data in Directory Entry,";
	if(incompatflags & 0x400)
	    out << "Inodes can store large extended attributes,";
	if(incompatflags & 0x200)
	    out << "Flexible block groups,";
	if(incompatflags & 0x100)
	    out << "Multiple mount protection,";
	if(incompatflags & 0x80)
	    out << "FS size over 2^32 blocks,";
	if(incompatflags & 0x40)
	    out << "Files use Extents,";
	if(incompatflags & 0x10)
	    out << "Meta block groups,";
	if(incompatflags & 0x08)
	    out << "Seperate Journal device,";
	if(incompatflags & 0x04)
	    out << "FS needs journal recovery,";
	if(incompatflags & 0x02)
	    out << "Directory entries record file type,";
	if(incompatflags & 0x01)
	    out << "Compression";
        out << "|File system incompatible Feature set." << Qt::endl;
        uint32_t readonlyflags = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1124, 4));
        out << "Read Only Compatible Features|";
	if(readonlyflags & 0x2000)
	    out << "Tracks project quotas,";
	if(readonlyflags & 0x1000)
	    out << "Read only FS,";
	if(readonlyflags & 0x800)
	    out << "Replica support,";
	if(readonlyflags & 0x400)
	    out << "Metadata Checksumming support,";
	if(readonlyflags & 0x200)
	    out << "Bigalloc support,";
	if(readonlyflags & 0x100)
	    out << "Quota is handled transactionally with journal,";
	if(readonlyflags & 0x80)
	    out << "Snapshot,";
	if(readonlyflags & 0x40)
	    out << "Large Inodes exist,";
	if(readonlyflags & 0x20)
	    out << "EXT3 32k subdir limit doesn't exist,";
	if(readonlyflags & 0x10)
	    out << "Group descriptors have checksums,";
	if(readonlyflags & 0x08)
	    out << "Space usage stored in i_blocks in units of fs_blocks, not 512-byte sectors,";
	if(readonlyflags & 0x04)
	    out << "Was intented for use with htree directories,";
	if(readonlyflags & 0x02)
	    out << "Allow storing files larger than 2GB,";
	if(readonlyflags & 0x01)
	    out << "Sparse superblocks";
        out << "|File system read only compatible feature set." << Qt::endl;
        out << "File System Type|";
        if(((compatflags & 0x00000200UL) != 0) || ((incompatflags & 0x0001f7c0UL) != 0) || ((readonlyflags & 0x00000378UL) != 0))
        {
	    qInfo() << "EXT4 File System Found. Parsing...";
            out << "EXT4";
            partitionname += QString(" [EXT4]");
        }
        else if(((compatflags & 0x00000004UL) != 0) || ((incompatflags & 0x0000000cUL) != 0))
        {
	    qInfo() << "EXT3 File System Found. Parsing...";
            out << "EXT3";
            partitionname += QString(" [EXT3]");
        }
        else
        {
	    qInfo() << "EXT2 File System Found. Parsing...";
            out << "EXT2";
            partitionname += QString(" [EXT2]");
        }
        //qDebug() << "partition name with fstype:" << partitionname;
        out << "|File system type string." << Qt::endl;
        uint16_t grpdescsize = 32;
	if(incompatflags & 0x80)
        {
            grpdescsize = qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector * 512 + 1278, 2));
        }
        //qDebug() << "grpdescsize:" << grpdescsize;
        out << "Block Group Descriptor Size|" << QString::number(grpdescsize) << "|Size in bytes of the block group descriptor table entry." << Qt::endl;
	uint32_t fsblockcnt = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + 1028, 4));
	uint32_t blkgrpblkcnt = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + 1056, 4));
	uint32_t blocksize = 1024 * pow(2, qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + 1048, 4)));
        uint32_t blkgrp0startblk = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + 1044, 4));
        out << "File System Inode Count|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + 1024, 4))) << "|Number of inodes within the file system." << Qt::endl;
        out << "File System Block Count|" << QString::number(fsblockcnt) << "|Number of blocks within the file system." << Qt::endl;
        out << "Block Group 0 Start Block|" << QString::number(blkgrp0startblk) << "|Starting block number for block group 0." << Qt::endl;
        out << "Block Size|" << QString::number(blocksize) << "|Block size in bytes." << Qt::endl;
	out << "Fragment Size|" << QString::number(1024 * pow(2, qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + 1052, 4)))) << "|Fragment size in bytes." << Qt::endl;
	out << "Block Group Block Count|" << QString::number(blkgrpblkcnt) << "|Number of blocks within a block group." << Qt::endl;
	out << "Block Group Fragment Count|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + 1060, 4))) << "|Number of fragments within a block group." << Qt::endl;
	uint32_t blkgrpinodecnt = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + 1064, 4));
	out << "Block Group Inode Count|" << QString::number(blkgrpinodecnt) << "|Number of inodes within a block group." << Qt::endl;
	uint32_t creatoros = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + 1096, 4));
	out << "Creator OS|";
	if(creatoros == 0x00)
	    out << "Linux";
	else if(creatoros == 0x03)
	    out << "FreeBSD";
	out << "|Operating System used to create the file system." << Qt::endl;
	out << "Inode Size|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector * 512 + 1112, 2))) << "|Size of an inode in bytes." << Qt::endl;
	out << "Last Mounted Path|" << QString::fromStdString(curimg->ReadContent(curstartsector * 512 + 1160, 64).toStdString()) << "|Path where file system was last mounted." << Qt::endl;
	uint32_t blockgroupcount = fsblockcnt / blkgrpblkcnt;
	uint blkgrpcntrem = fsblockcnt % blkgrpblkcnt;
	if(blkgrpcntrem > 0)
	    blockgroupcount++;
	if(blockgroupcount == 0)
	    blockgroupcount = 1;
        //qDebug() << "curstartsector:" << curstartsector;
        //qDebug() << "blkgrp0startblock:" << blkgrp0startblk;
	QString inodeaddresstable = "";
        //qDebug() << "blockgroupcount:" << blockgroupcount;
        //qDebug() << "blocksize:" << blocksize;
	for(uint i=0; i < blockgroupcount; i++)
	{
	    if(blocksize == 1024)
		inodeaddresstable += QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + 2 * blocksize + i * grpdescsize + 8, 4))) + ",";
	    else
		inodeaddresstable += QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + blocksize + i * grpdescsize + 8, 4))) + ",";
	}
	//qDebug() << "inodeaddresstable:" << inodeaddresstable;
	out << "Inode Address Table|" << inodeaddresstable << "|Table of the Inode addresses for a block group." << Qt::endl;
	out << "Root Inode Table Address|";
	if(blkgrpinodecnt > 2)
	    out << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + 2056, 4)));
	else
	    out << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + 2088, 4)));
	out << "|Starting address for the Root Directory Inode Table." << Qt::endl;
	out << "Revision Level|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector * 512 + 1100, 4))) + "." + QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector * 512 + 1086, 2))) << "|File system revision level." << Qt::endl;
	// NEED TO IMPLEMENT THESE FS PROPERTIES SO I CAN USE THEM WHEN I PARSE THE EXT2/3/4 FS
	/*
        fsinfo.insert("partoffset", QVariant((qulonglong)(512 * partoffset)));
        //qDebug() << "INODE SIZE ACCORDING TO SUPERBLOCK:" << fsinfo.value("inodesize").toUInt();
        //qDebug() << "compatflags:" << fsinfo.value("compatflags").toUInt() << "incompatflags:" << fsinfo.value("incompatflags").toUInt() << "readonlyflags:" << fsinfo.value("readonlyflags").toUInt();
        //if(fsinfo.value("incompatflags").toUInt() & 0x40)
        //    qDebug() << "fs uses extents.";
        
	//qDebug() << "blocks for group descriptor table:" << (32 * (fsinfo.value("fsblockcnt").toUInt() / fsinfo.value("blockgroupblockcnt").toUInt())) / fsinfo.value("blocksize").toUInt();
	 */ 
    }
    else if(apfsig == "NXSB") // APFS Container
    {
	qInfo() << "APFS Container Found. Parsing...";
        uint64_t nxoffset = curstartsector*512;
        // THIS POSSIBLY NEEDS TO OCCUR WITHIN THE FOR LOOP OF THE CHECKPOINT DESCRIPTOR LOOP
        uint64_t nxoid = qFromLittleEndian<uint64_t>(curimg->ReadContent(nxoffset + 8, 8));
        uint64_t nxxid = qFromLittleEndian<uint64_t>(curimg->ReadContent(nxoffset + 16, 8));
        //qDebug() << "nxoid:" << nxoid << "nxxid:" << nxxid;
        uint32_t blocksize = qFromLittleEndian<uint32_t>(curimg->ReadContent(nxoffset + 36, 4));
        uint32_t nxcmapblkcnt = qFromLittleEndian<uint32_t>(curimg->ReadContent(nxoffset + 104, 4));
        int64_t nxcmapblk = qFromLittleEndian<int64_t>(curimg->ReadContent(nxoffset + 112, 8));
        //qDebug() << "nxcmapblk:" << nxcmapblk << "nxcmapblkcnt:" << nxcmapblkcnt;
        uint64_t nxcmapoffset = nxcmapblk * blocksize + curstartsector*512;
        //qDebug() << "nxcmapoffset:" << nxcmapoffset;
        uint64_t oldxid = nxxid;
        for(int i=0; i <= nxcmapblkcnt; i++)
        {
            uint64_t curoid = qFromLittleEndian<uint64_t>(curimg->ReadContent(nxcmapoffset + i*blocksize + 8, 8));
            uint64_t curxid = qFromLittleEndian<uint64_t>(curimg->ReadContent(nxcmapoffset + i*blocksize + 16, 8));
            uint32_t curtype = qFromLittleEndian<uint32_t>(curimg->ReadContent(nxcmapoffset + i*blocksize + 24, 4));
            //qDebug() << "i:" << i << "curoid:" << curoid << "curxid:" << curxid << "curtype:" << QString::number(curtype, 16);
            
            if(curoid == 0 && curxid == 0 && curtype == 0)
                break; // break out of for loop
            switch(curtype)
            {
                // MAY NOT NEED THIS CODE SINCE I DON'T CARE ABOUT THE 
                /*
		 * MIGHT NEED TO GO DOWN THIS ROUTE AND SEE IF IT CONTAINS MORE CHECKPOINTS OR LOOK AT THE HEX FOR THE CHECKPOINTS
		 * AND SEE IF ALL THE ENTRIES ARE IN THERE...
                 */ 
                /*
                case 0x4000000c: // PHYSICAL CHECKPOINT MAP
                    // NEED TO IMPLEMENT THIS PART OF THE CODE NEXT...
                    qDebug() << "use checkpoint map to get the latest superblock with whcih to parse";
                    qDebug() << "cmap offset:" << nxcmapoffset + i*blocksize;
                    qDebug() << "curoid:" << curoid << "curxid:" << curxid << "curtype:" << QString::number(curtype, 16);
                    break;
                */
                case 0x80000001: // EPHEMERAL SUPERBLOCK
                    //qDebug() << "it's a superblock so see if it's newer and use it.";
                    //qDebug() << "i:" << i << "curxid:" << curxid << "oldxid:" << oldxid;
                    if(curxid > oldxid)
                    {
                        oldxid = curxid;
                        nxoffset = nxcmapoffset + i*blocksize;
                        //qDebug() << "curoid:" << curoid << "curxid:" << curxid << "curtype:" << QString::number(curtype, 16);
                    }
                    break;
                default:
                    break;
            }
        }
        //qDebug() << "new superblock oid/offset:" << nxoid << "/" << nxoffset;
        // NOW TO DO THE OBJECT MAP PARSING FOR WHAT I NEED
        //uint64_t nxomapoid = qFromLittleEndian<uint64_t>(curimg->ReadContent(nxoffset + 160, 8));
        //uint64_t nxomapoffset = curstartsector*512 + nxomapoid * blocksize;
        //qDebug() << "nxomapoid:" << nxomapoid << "nxomapoffset:" << nxomapoffset;
        /*
        uint64_t omapbtreeoid = qFromLittleEndian<uint64_t>(curimg->ReadContent(nxomapoffset + 48, 8));
        uint32_t omapobjtype = qFromLittleEndian<uint32_t>(curimg->ReadContent(nxomapoffset + 24, 4));
        if(omapobjtype != 0x4000000b) // PHYSICAL OBJECT MAP
        {
            qDebug() << "error, not a valid object map..";
        }
        */
        // ONCE I GET THE ABOVE RIGHT, I WILL NEED TO SET THE NEW SUPERBLOCK OFFSET, SO THE BELOW PULLS THE INFORMATION
        // FROM THE CORRECT SUPERBLOCK
        //qDebug() << "superblock checksum:" << CheckChecksum(curimg, curstartsector*512 + 8, qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 36, 4)) - 8, qFromLittleEndian<uint64_t>(curimg->ReadContent(curstartsector*512, 8)));
        out << "File System Type Int|7|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|APFS|File System Type String." << Qt::endl;
        //out << "Fletcher Checksum|" << QString::fromStdString(curimg->ReadContent(nxoffset, 8.toHex()).toStdString()) << "|Fletcher checksum value." << Qt::endl;
        out << "SuperBlock Offset|" << QString::number(nxoffset) << "|Byte offset to the current superblock." << Qt::endl;
        out << "Object ID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(nxoffset + 8, 8))) << "|APFS object id." << Qt::endl;
        out << "Transaction ID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(nxoffset + 16, 8))) << "|APFS transaction id." << Qt::endl;
        // NEED TO PROCESS PROPERLY AND DO AN IF & THING FOR IT...
        //out << "Object Type|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(nxoffset + 24, 2))) << "|APFS object type: 1 - container super block, 2 - btree, 3 btree node, 5 - spaceman, 11 - object map (OMAP), 13 - file system (volume super block), 17 - reaper." << Qt::endl;
        //out << "Object Flags|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(nxoffset + 26, 2))) << "|APFS object flags: 0 - OBJ_VIRTUAL, 80 - OBJ_EPHEMERAL, 40 - OBJ_PHYSICAL, 20 - OBJ_NOHEADER, 10 - OBJ_ENCRYPTED, 08 - OBJ_NONPERSISTENT." << Qt::endl;
        //out << "Object SubType|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(nxoffset + 28, 4))) << "|APFS object subtype: 0 - none, 11 - object map (OMAP), 14 - file system tree." << Qt::endl;
        out << "Block Size|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(nxoffset + 36, 4))) << "|APFS block size in bytes, usually 4096." << Qt::endl;
        //qDebug() << "block size:" << qFromLittleEndian<uint32_t>(curimg->ReadContent(nxoffset + 36, 4));
        out << "Block Count|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(nxoffset + 40, 8))) << "|Number of blocks for the AFPS container." << Qt::endl;
        // NEED TO DO AN IF & THING FOR THE 3 FEATURES SET AS WELL
        //out << "Container Features|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(nxoffset + 48, 8))) << "|Container features options uint64_t." << Qt::endl;
        //out << "Container Read-Only Compatible Features|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(nxoffset + 56, 8))) << "|Container read-only compatible features options uint64_t." << Qt::endl;
        //out << "Container Incompatible Features|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(nxoffset + 64, 8))) << "|Container incompatible features options uint64_t." << Qt::endl;
        out << "Container UUID|" << QString::fromStdString(curimg->ReadContent(nxoffset + 72, 16).toHex().toStdString()) << "|Container's universal unique id." << Qt::endl;
        // NEED TO DISPLAY THE CONTAINER ID IN THE PROPER FORMAT BELOW
	//qDebug() << "container uuid:" << (fsinfo.value("containeruuid").toString().left(8) + "-" + fsinfo.value("containeruuid").toString().mid(8, 4) + "-" + fsinfo.value("containeruuid").toString().mid(12, 4) + "-" + fsinfo.value("containeruuid").toString().mid(16, 4) + "-" + fsinfo.value("containeruuid").toString().right(12));
        out << "Next Object ID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(nxoffset + 88, 8))) << "|Next object id." << Qt::endl;
        out << "Next Transaction ID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(nxoffset + 96, 8))) << "|Next transaction id." << Qt::endl;
        //out << "CheckPoint Descriptor Flag|" << QString::fromStdString(curimg->ReadContent(nxoffset + 107, 1).toStdString()) << "|Flag for the checkpoint descriptor." << Qt::endl;
        out << "CheckPoint Description Blocks|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(nxoffset + 104, 4))) << "|Number of blocks used by the checkpoint descriptor area." << Qt::endl;
        out << "CheckPoint Descriptor Base|" << QString::number(qFromLittleEndian<int64_t>(curimg->ReadContent(nxoffset + 112, 8))) << "|Base address of the checkpoint descriptor area or the physical object identifier of a tree that contains address information." << Qt::endl;
        out << "Container Object Map Object ID|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(nxoffset + 160, 8))) << "|Object id for the container's object map." << Qt::endl;
        out << "Maximum Container Volumes|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(nxoffset + 180, 4))) << "|Maximum number of volumes in the APFS container." << Qt::endl;
        //qDebug() << "max containers:" << qFromLittleEndian<uint32_t>(curimg->ReadContent(nxoffset + 180, 4));
        out << "Volume Object ID List|";
        for(int i=0; i < qFromLittleEndian<uint32_t>(curimg->ReadContent(nxoffset + 180, 4)); i++)
        {
            uint64_t apfsvoloid = qFromLittleEndian<uint64_t>(curimg->ReadContent(nxoffset + 184 + i*8, 8));
            if(apfsvoloid > 0)
                out << QString::number(apfsvoloid) << ",";
            //qDebug() << QString("fs [" + QString::number(i) + "] objid:") << qFromLittleEndian<uint64_t>(curimg->ReadContent(nxoffset + 184 + i*8, 8));
        }
        // NEED TO GET THE PROPERTIES NX_KEYLOCKER FOR KEYBAG LOCATION AND NX_FLAGS FOR WHETHER IT IS SOFTWARE KEY
        out << "|List of object id's for each volume within the container." << Qt::endl;
        partitionname += "APFS Container [APFS]";
        /*
        fsinfo.insert("descblocks", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(104, 4))));
        fsinfo.insert("datablocks", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(108, 4))));
        fsinfo.insert("descbase", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(112, 8))));
        fsinfo.insert("database", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(120, 8))));
        fsinfo.insert("descnext", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(128, 4))));
        fsinfo.insert("datanext", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(132, 4))));
        fsinfo.insert("descindex", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(136, 4))));
        fsinfo.insert("desclen", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(140, 4))));
        fsinfo.insert("dataindex", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(144, 4))));
        fsinfo.insert("datalen", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(148, 4))));
        fsinfo.insert("maxfilesystems", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(180, 4))));
	qDebug() << "max filesystems:" << fsinfo.value("maxfilesystems").toUInt();
	//while(fsinfo.value("maxfilesystems").toUInt() < 100)
	for(uint i=0; i < fsinfo.value("maxfilesystems").toUInt(); i++)
	{
	    qDebug() << "fs entryid:" << qFromLittleEndian<qulonglong>(partbuf.mid(184 + i*8, 8));
	}
        qDebug() << "desc blocks:" << fsinfo.value("descblocks").toUInt() << "descbase:" << fsinfo.value("descbase").toUInt();
        qDebug() << "desc index:" << fsinfo.value("descindex").toUInt() << "desc length:" << fsinfo.value("descindex").toUInt();
	// to determine volume information for each volume...
	// libfsapfs_object_map_descriptor_t *object_map_descriptor = NULL;
	// libfsapfs_object_map_btree_get_descriptor_by_object_identifier(internal_container->object_map_btree,internal_container->file_io_handle,internal_container->superblock->volume_object_identifiers[ volume_index ],&object_map_descriptor,error )
	// file_offset = (off64_t) ( object_map_descriptor->physical_address * internal_container->io_handle->block_size )
        */
    }
    else if(hfssig == "H+" || hfssig == "HX") // HFS+/HFSX
    {
        out << "File System Type Int|8|Internal File System Type represented as an integer." << Qt::endl;
        //out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;

        // VOLUME LABEL IS THE FILE NAME FOR THE ROOT DIRECTORY, CATALOG ID: 2...
        // NEED TO GET THE CATALOG START BLOCK * BLOCK SIZE + (CNID - 1)*4096 = 991232 + (2-1)*4096 = 995328
        // THIS IS THE START OF THE CATALOG LEAF NODE FOR THE ROOT FOLDER (CNID 2)
        // FIRST 14 BYTES ARE NODE DESCRIPTOR [BYTE 8 IS THE KIND AND SHOULD BE 0xFF], [BYTE 9 IS THE HEIGHT AND SHOULD BE 1 FOR LEAF NODE]
        // NEXT 2 BYTES ARE THE KEY LENGTH, 4 BYTES ARE PARENT CNID [SHOULD BE 0x00000001]
        // NEXT 4 BYTES ARE THE DATA LENGTH NODE FOR THE KEY DATA, KEY DATA IS THE VOLUME NAME
        quint32 catalogstartoffset = qFromBigEndian<quint32>(curimg->ReadContent(curstartsector*512 + 1312, 4));
        //qDebug() << "catalog start offset:" << catalogstartoffset;
        quint64 catalogstartinbytes = catalogstartoffset * 4096 + 4096; // second 4096 is (2-1) * 4096
        //qDebug() << "catalog start offset for root folder:" << catalogstartinbytes << "should be <995328>";
        //qDebug() << "THE KIND:" << curimg->ReadContent(catalogstartinbytes + 8, 1).toHex();
        //qDebug() << "HEIGHT:" << curimg->ReadContent(catalogstartinbytes + 9, 1).toHex();
        quint32 keydatalength = qFromBigEndian<quint16>(curimg->ReadContent(catalogstartinbytes + 20, 2));
        //qDebug() << "KEY DATA LENGTH:" << keydatalength;
        for(uint8_t i=0; i < keydatalength; i++)
            partitionname += QString(QChar(qFromBigEndian<uint16_t>(curimg->ReadContent(catalogstartinbytes + 22 + i*2, 2))));
        if(hfssig == "H+")
        {
	    qInfo() << "HFS+ File System Found. Parsing...";
            partitionname += " [HFS+]";
            out << "File System Type|HFS+|File System Type String." << Qt::endl;
        }
        else if(hfssig == "HX")
        {
	    qInfo() << "HFSX File System Found. Parsing...";
            partitionname += " [HFSX]";
            out << "File System Type|HFSX|File System Type String." << Qt::endl;
        }
        // NEED TO CONVERT THE 2 DATES LISTED TO ACTUAL HUMAN READABLE DATES
        out << "Volume Creation Date|" << ConvertUnixTimeToString(ConvertHfsTimeToUnixTime(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1040, 4)))) << "|Creation Date of the volume stored in local time." << Qt::endl;
        out << "Last Modification Date|" << ConvertUnixTimeToString(ConvertHfsTimeToUnixTime(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1044, 4)))) << "|UTC Last modification date of the volume." << Qt::endl;
        out << "File Count|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1056, 4))) << "|Number of files on the volume." << Qt::endl;
        out << "Folder Count|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1060, 4))) << "|Number of folders on the volume." << Qt::endl;
        out << "Cluster Size|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1064, 4))) << "|Allocation Cluster Size, usually 4096 bytes." << Qt::endl;
        out << "Next Catalog ID|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1088, 4))) << "|Next available catalog ID (node id)." << Qt::endl;
        // ALLOCATION FILE INFORMATION
        out << "Allocation Logical Size|" << QString::number(qFromBigEndian<quint64>(curimg->ReadContent(curstartsector*512 + 1136, 8))) << "|Logical size for the allocation file." << Qt::endl;
        out << "Allocation Total Blocks|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1148, 4))) << "|Total number of blocks for the allocation file." << Qt::endl;
        out << "Allocation Extents Start Block Array|";
        for(int i=0; i < 8; i++)
        {
            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1152 + i*8, 4)) > 0)
                out << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1152 + i*8, 4))) << ",";
        }
        out << "|Start block for each extent for allocation file." << Qt::endl;
        out << "Allocation Extents Block Count Array|";
        for(int i=0; i < 8; i++)
        {
            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1156 + i*8, 4)) > 0)
                out << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1156 + i*8, 4))) << ",";
        }
        out << "|Block count for each extent for allocation file." << Qt::endl;
        // EXTENTS OVERFLOW FILE INFORMATION
        out << "Extents Overflow Logical Size|" << QString::number(qFromBigEndian<quint64>(curimg->ReadContent(curstartsector*512 + 1216, 8))) << "|Logical size for the extents overflow file." << Qt::endl;
        out << "Extents Overflow Total Blocks|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1228, 4))) << "|Total number of blocks for the extents overflow file." << Qt::endl;
        out << "Extents Overflow Extents Start Block Array|";
        for(int i=0; i < 8; i++)
        {
            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1232 + i*8, 4)) > 0)
                out << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1232 + i*8, 4))) << ",";
        }
        out << "|Start block for each extent for extents overflow file." << Qt::endl;
        out << "Extents Overflow Extents Block Count Array|";
        for(int i=0; i < 8; i++)
        {
            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1236 + i*8, 4)) > 0)
                out << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1236 + i*8, 4))) << ",";
        }
        out << "|Block count for each extent for extents overflow file." << Qt::endl;
        // CATALOG FILE INFORMATION
        out << "Catalog Logical Size|" << QString::number(qFromBigEndian<quint64>(curimg->ReadContent(curstartsector*512 + 1296, 8))) << "|Logical size for the catalog file." << Qt::endl;
        out << "Catalog Total Blocks|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1308, 4))) << "|Total number of blocks for the Catalog file." << Qt::endl;
        out << "Catalog Extents Start Block Array|";
        for(int i=0; i < 8; i++)
        {
            if(qFromBigEndian<quint32>(curimg->ReadContent(curstartsector*512 + 1312 + i*8, 4)) > 0)
                out << QString::number(qFromBigEndian<quint32>(curimg->ReadContent(curstartsector*512 + 1312 + i*8, 4))) << ",";
        }
        out << "|Start block for each extent for Catalog file." << Qt::endl;
        out << "Catalog Extents Block Count Array|";
        for(int i=0; i < 8; i++)
        {
            if(qFromBigEndian<quint32>(curimg->ReadContent(curstartsector*512 + 1316 + i*8, 4)) > 0)
                out << QString::number(qFromBigEndian<quint32>(curimg->ReadContent(curstartsector*512 + 1316 + i*8, 4))) << ",";
        }
        out << "|Block count for each extent for Catalog file." << Qt::endl;

        // ATTRIBUTES FILE INFORMATION
        out << "Attributes Logical Size|" << QString::number(qFromBigEndian<quint64>(curimg->ReadContent(curstartsector*512 + 1376, 8))) << "|Logical size for the attributes file." << Qt::endl;
        out << "Attributes Total Blocks|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1388, 4))) << "|Total number of blocks for the attributes file." << Qt::endl;
        out << "Attributes Extents Start Block Array|";
        for(int i=0; i < 8; i++)
        {
            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1392 + i*8, 4)) > 0)
                out << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1392 + i*8, 4))) << ",";
        }
        out << "|Start block for each extent for attributes file." << Qt::endl;
        out << "Attributes Extents Block Count Array|";
        for(int i=0; i < 8; i++)
        {
            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1396 + i*8, 4)) > 0)
                out << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1396 + i*8, 4))) << ",";
        }
        out << "|Block count for each extent for attributes file." << Qt::endl;
        // STARTUP FILE INFORMATION
        out << "Startup Logical Size|" << QString::number(qFromBigEndian<quint64>(curimg->ReadContent(curstartsector*512 + 1456, 8))) << "|Logical size for the startup file." << Qt::endl;
        out << "Startup Total Blocks|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1468, 4))) << "|Total number of blocks for the startup file." << Qt::endl;
        out << "Startup Extents Start Block Array|";
        for(int i=0; i < 8; i++)
        {
            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1472 + i*8, 4)) > 0)
                out << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1472 + i*8, 4))) << ",";
        }
        out << "|Start block for each extent for startup file." << Qt::endl;
        out << "Startup Extents Block Count Array|";
        for(int i=0; i < 8; i++)
        {
            if(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1476 + i*8, 4)) > 0)
                out << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1476 + i*8, 4))) << ",";
        }
        out << "|Block count for each extent for startup file." << Qt::endl;
    }
    else if(xfssig == "XFSB") // XFS
    {
	qInfo() << "XFS File System Found. Parsing...";
        out << "File System Type Int|9|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|XFS|File System Type String." << Qt::endl;
        partitionname += QString::fromStdString(curimg->ReadContent(curstartsector*512 + 108, 12).toStdString());
        out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        partitionname += " [XFS]";
        out << "Block Size|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 4, 4))) << "|Size of block in bytes." << Qt::endl;
        out << "Data Blocks|" << QString::number(qFromBigEndian<quint64>(curimg->ReadContent(curstartsector*512 + 8, 8))) << "|Total number of blocks available for data." << Qt::endl;
        out << "Real Time Blocks|" << QString::number(qFromBigEndian<quint64>(curimg->ReadContent(curstartsector*512 + 16, 8))) << "|Number of blocks in the real time device." << Qt::endl;
        out << "Real Time Extents|" << QString::number(qFromBigEndian<quint64>(curimg->ReadContent(curstartsector*512 + 24, 8))) << "|Number of extents on the real time device." << Qt::endl;
        out << "UUID|" << QString::fromStdString(curimg->ReadContent(curstartsector*512 + 32, 16).toStdString()) << "Universal unique id for the file system." << Qt::endl;
        out << "Root Inode|" << QString::number(qFromBigEndian<quint64>(curimg->ReadContent(curstartsector*512 + 56, 8))) << "|Root inode number for the filesystem." << Qt::endl;
        out << "Allocation Group Blocks|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 84, 4))) << "|Size of each allocation group in blocks." << Qt::endl;
        out << "Allocation Group Count|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 88, 4))) << "|Number of allocation groups in the filesystem." << Qt::endl;
        out << "Inode Size|" << QString::number(qFromBigEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 104, 2))) << "|Size of an inode in bytes." << Qt::endl;
        out << "Inodes Per Block|" << QString::number(qFromBigEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 106, 2))) << "|Number of inodes per block." << Qt::endl;
    }
    else if(btrsig == "_BHRfS_M") // BTRFS
    {
	qInfo() << "BTRFS File System Found. Parsing...";
        out << "File System Type Int|10|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|BTRFS|File System Type String." << Qt::endl;
        partitionname += QString::fromStdString(curimg->ReadContent(curstartsector*512 + 65536 + 0x12b, 100).toStdString());
        out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        partitionname += " [BTRFS]";
        /*
        int curoffset = 65536;
        fsinfo.insert("type", QVariant(10));
        fsinfo.insert("typestr", QVariant("BTRFS"));
        fsinfo.insert("rootaddr", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(curoffset + 0x50, 8))));
        fsinfo.insert("chunkrootaddr", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(curoffset + 0x58, 8))));
        fsinfo.insert("rootdirobjid", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(curoffset + 0x80, 8))));
        fsinfo.insert("sectorsize", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(curoffset + 0x90, 4))));
        fsinfo.insert("nodesize", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(curoffset + 0x94, 4))));
        fsinfo.insert("vollabel", QVariant(QString::fromStdString(partbuf.mid(curoffset + 0x12b, 100).toStdString())));
        qDebug() << "sectorsize:" << fsinfo.value("sectorsize").toUInt();
        qDebug() << "rootdirobjid:" << fsinfo.value("rootdirobjid").toUInt();
        */
    }
    else if(btlsig == "-FVE-FS-") // BITLOCKER
    {
	qInfo() << "Bitlocker Encryption Found. Analyzing...";
        out << "File System Type Int|11|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|BITLOCKER|File System Type String." << Qt::endl;
        partitionname += QString::fromStdString(curimg->ReadContent(curstartsector*512 + 118, qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 112, 2)) - 8).toStdString());
        out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        partitionname += " [BITLOCKER]";
        // HERE IS WHERE I WILL GET THE KEY(S) INFORMATION AND ALSO GET THE USER PASSWORD AND/OR RECOVERY KEY FROM THE EXAMINER
        // PLACE THAT PROMPT HERE BASED ON THE KEY INFORMATION I READ FROM THE ENCRYPTED VOLUME
        // ATTEMPTED TO USE LIBBDE.H, BUT IT REQUIRES A FILE, AND I DON'T HAVE A FILE PER SE
        /*
        // NEED AN IMAGE TO TEST BITLOCKER
        fsinfo.insert("type", QVariant(11));
        fsinfo.insert("typestr", QVariant("BITLOCKER"));
        fsinfo.insert("metadatasize", QVariant(qFromLittleEndian<uint16_t>(partbuf.mid(8, 2))));
        fsinfo.insert("meta1offset", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(32, 8))));
        fsinfo.insert("meta2offset", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(40, 8))));
        fsinfo.insert("meta3offset", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(48, 8))));
        fsinfo.insert("mftmirrorcluster", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(56, 8))));
        fsinfo.insert("sizeminusheader", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(64, 4))));
        fsinfo.insert("nextcounter", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(96, 4))));
        fsinfo.insert("algorithm", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(100, 4))));
        fsinfo.insert("timeenabled", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(104, 8))));
        fsinfo.insert("volnamelength", QVariant(qFromLittleEndian<uint16_t>(partbuf.mid(112, 2))));
        fsinfo.insert("vollabel", QVariant(QString::fromStdString(partbuf.mid(118, fsinfo.value("volnamelength").toUInt() - 8).toStdString())));
        qDebug() << "When Bitlocker was Enabled:" << ConvertWindowsTimeToUnixTimeUTC(fsinfo.value("timeenabled").toULongLong());
        uint encryptalgo = fsinfo.value("algorithm").toUInt();
        if(encryptalgo == 0x1000)
            qDebug() << "stretch";
        else if(encryptalgo >= 0x2000 || encryptalgo <= 0x2005)
            fsinfo.insert("algoname", QVariant("256-bit AES-CCM"));
        else if(encryptalgo == 0x8000)
            fsinfo.insert("algoname", QVariant("128-bit AES + Elephant"));
        else if(encryptalgo == 0x8001)
            fsinfo.insert("algoname", QVariant("256-bit AES + Elephant"));
        else if(encryptalgo == 0x8002)
            fsinfo.insert("algoname", QVariant("128-bit AES"));
        else if(encryptalgo == 0x8003)
            fsinfo.insert("algoname", QVariant("256-bit AES"));
        else
            fsinfo.insert("algoname", QVariant("NOT DEFINED"));
        */
    }
    else if(bfssig == "1SFB") // BFS
    {
	qInfo() << "BFS File System Found. Parsing...";
        out << "File System Type Int|12|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|BFS|File System Type String." << Qt::endl;
        partitionname += QString::fromStdString(curimg->ReadContent(curstartsector*512 + 512, 32).toStdString());
        out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
	uint32_t blocksize = qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 552, 4));
	//qDebug() << "blocksize:" << blocksize;
	out << "Block Size|" << QString::number(blocksize) << "|Size in bytes for a file system block." << Qt::endl;
        out << "Block Shift|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 556, 4))) << "|Number of bits needed to shift a block number by to get a byte address." << Qt::endl;
	out << "Number of Blocks|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curstartsector*512 + 560, 8))) << "|Number of blocks in the file system." << Qt::endl;
	out << "Used Blocks|" << QString::number(qFromLittleEndian<uint64_t>(curimg->ReadContent(curstartsector*512 + 568, 8))) << "|Number of blocks in use by the file system." << Qt::endl;
	out << "Inode Size|" << QString::number(qFromLittleEndian<int32_t>(curimg->ReadContent(curstartsector*512 + 576, 4))) << "|Size in bytes for an inode." << Qt::endl;
	out << "Blocks per Allocation Group|" << QString::number(qFromLittleEndian<int32_t>(curimg->ReadContent(curstartsector*512 + 584, 4))) << "|Number of blocks in each allocation group." << Qt::endl;
        out << "Allocation Shift|" << QString::number(qFromLittleEndian<int32_t>(curimg->ReadContent(curstartsector*512 + 588, 4))) << "|Number of bits to shift an allocation group number by when converting a block run address to a byte offset." << Qt::endl;
        out << "Number of Allocation Groups|" << QString::number(qFromLittleEndian<int32_t>(curimg->ReadContent(curstartsector*512 + 592, 4))) << "|Number of allocation groups in the file system." << Qt::endl;
        // need to implement properly...
        //out << "Flags|" << QString::number(qFromLittleEndian<int32_t>(curimg->ReadContent(curstartsector*512 + 596, 4))) << "|What flags means here.." << Qt::endl;
        out << "Root Directory Allocation Group|" << QString::number(qFromLittleEndian<int32_t>(curimg->ReadContent(curstartsector*512 + 628, 4))) << "|Allocation group for the root directory." << Qt::endl;
	out << "Root Directory Start Block|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 632, 2))) << "|Starting block number for the root directory." << Qt::endl;
        out << "Root Indices Allocation Group|" << QString::number(qFromLittleEndian<int32_t>(curimg->ReadContent(curstartsector*512 + 636, 4))) << "|Allocation group for the root directory indices." << Qt::endl;
	out << "Root Indices Start Block|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 640, 2))) << "|Starting block number for the root indices." << Qt::endl;
        partitionname += " [BFS]";
        /*
        fsinfo.insert("vollabel", QVariant(QString::fromStdString(partbuf.mid(512, 32).toStdString())));
        fsinfo.insert("fsbyteorder", QVariant(qFromLittleEndian<int32_t>(partbuf.mid(548, 4))));
        //qDebug() << "fsbyteorder:" << QString::number(fsinfo.value("fsbyteorder").toInt(), 16);
        */
    }
    else if(f2fsig == 0xf2f52010) // F2FS
    {
	qInfo() << "F2FS File System Found. Parsing not yet implemented.";
        out << "File System Type Int|13|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|F2FS|File System Type String." << Qt::endl;
        //out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        partitionname += " [F2FS]";
    }
    else if(isosig == "CD001" && udfsig != "BEA01") // ISO9660
    {
	qInfo() << "ISO9660 File System Found. Parsing...";
        out << "File System Type Int|14|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|ISO9660|File System Type String." << Qt::endl;
	int8_t pvindx = 1;
	int8_t svdindx = 1;
	int8_t vpdindx = 1;
	int8_t brindx = 1;
	// NEED A FOR LOOP, WHICH STARTS AT BLOCK 16 AND GOES BY 2048 UNTIL WE GET TO THE VOLUME TERMINATOR WITH TYPE FF
	for(int i=16; i < (curimg->Size() / 2048) - 15; i++)
	{
	    uint64_t curoffset = curstartsector*512 + 2048*i;
	    uint8_t voldesctype = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset, 1));
	    if(voldesctype == 0x00) // BOOT RECORD
	    {
		//qDebug() << "Boot Record";
		out << "BR" << QString::number(brindx) << " Volume Descriptor Type|" << QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset, 1))) << "|Value for voluem descriptor type, 0 - Boot Record, 1 - Primary, 2 - Supplementary or Enhanced, 3 - Partition, 4-254 - Reserved, 255 - Set Terminator." << Qt::endl;
		out << "BR" << QString::number(brindx) << " Boot System Identifier|" << QString::fromStdString(curimg->ReadContent(curoffset + 7, 31).toStdString()) << "|System identification which can recognize and act upon the boot system use fields." << Qt::endl;
		out << "BR" << QString::number(brindx) << " Boot Indentifier|" << QString::fromStdString(curimg->ReadContent(curoffset + 39, 31).toStdString()) << "|Identification of the boot system." << Qt::endl;
		brindx++;
	    }
	    else if(voldesctype == 0x01) // PRIMARY VOLUME DESCRIPTOR
	    {
		//qDebug() << "Primary Volume Descriptor" << pvindx;
		// Primary Volume Descriptor
		//out << "Primary Volume Descriptor|" << QString::number(pvindx) << "|Identifier for this primary volume descriptor." << Qt::endl;
		out << "PV" << QString::number(pvindx) << " Volume Descriptor Type|" << QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset, 1))) << "|Value for volume descriptor type, 0 - Boot Record, 1 - Primary, 2 - Supplementary or Enhanced, 3 - Partition, 4-254 - Reserved, 255 - Set Terminator." << Qt::endl;
		partitionname += QString::fromStdString(curimg->ReadContent(curoffset + 40, 31).toStdString());
		out << "PV" << QString::number(pvindx) << " Volume Label|" << partitionname << "|Name of the volume." << Qt::endl;
		partitionname += " [ISO9660]";
		out << "PV" << QString::number(pvindx) << " Volume Space Size|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 80, 4))) << "|Number of logical blocks in which the volume space is recorded." << Qt::endl;
		out << "PV" << QString::number(pvindx) << " Volume Set Size|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 120, 2))) << "|Volume set size of the volume in bytes." << Qt::endl;
		out << "PV" << QString::number(pvindx) << " Volume Sequence Number|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 124, 2))) << "|Ordinal number of the volume in the volume set which the volume is a member." << Qt::endl;
		out << "PV" << QString::number(pvindx) << " Logical Block Size|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 128, 2))) << "|Size in bytes of a logical block." << Qt::endl;
		out << "PV" << QString::number(pvindx) << " Path Table Size|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 132, 4))) << "|Length in bytes of a recorded occurence of the path table identified by the volume descriptor." << Qt::endl;
		out << "PV" << QString::number(pvindx) << " Location of Occurrence of Type L Path Table|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 140, 4))) << "|Logical block number of the first logical block allocated to the extent which contains an occurrence of the path table." << Qt::endl;
		out << "PV" << QString::number(pvindx) << " Location of Optional Occurrence of Type L Path Table|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 144, 4))) << "|Logical block number of the first logical block allocated to the extent which contains an optional occurence of the path table. If 0, it means the extent shall not be expected to be recorded." << Qt::endl;
		// Dir Record for Root Directory - 34 bytes
		out << "PV" << QString::number(pvindx) << " Root Directory Record Length|" << QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 156, 1))) << "|Length in bytes of the root directory record." << Qt::endl;
		out << "PV" << QString::number(pvindx) << " Extended Attribute Record Length|" << QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 157, 1))) << "|Length in bytes of the extended attribute record, if recorded, otherwise 0." << Qt::endl;
		out << "PV" << QString::number(pvindx) << " Extent Location|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 158, 4))) << "|Logical block number of the first logical block allocated to the extent." << Qt::endl;
		out << "PV" << QString::number(pvindx) << " Data Length|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 166, 4))) << "|Length in bytes of the data for the file section." << Qt::endl;
		uint16_t recyear = 1900 + qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 174, 1));
		uint8_t recmonth = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 175, 1));
		uint8_t recday = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 176, 1));
		uint8_t rechr = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 177, 1));
		uint8_t recmin = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 178, 1));
		uint8_t recsec = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 179, 1));
		uint8_t recutc = qFromLittleEndian<int8_t>(curimg->ReadContent(curoffset + 180, 1));
		// skipped recutc for now.
		out << "PV" << QString::number(pvindx) << " Recording Date and Time|" << QString(QString::number(recmonth) + "/" + QString::number(recday) + "/" + QString::number(recyear) + " " + QString::number(rechr) + ":" + QString::number(recmin) + ":" + QString::number(recsec)) << "|Date and time which the information in the extent of the directory record was recorded." << Qt::endl;
		// NEED TO FIX THE FILE FLAGS SO IT READS PROPERLY...
		out << "PV" << QString::number(pvindx) << " File Flags|" << QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 181, 1)), 2) << "|Flags for the file." << Qt::endl;
		out << "PV" << QString::number(pvindx) << " File Unit Size|" << QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 182, 1))) << "|Assigned file unit size for the file section if the file section is recorded in interleaved mode, otherwise 0." << Qt::endl;
		//out << "PV" << QString::number(pvindx) << " Volume Set Identifier|" << QString::fromStdString(curimg->ReadContent(curoffset + 190, 128)) << "|complicatted." << Qt::end;
		out << "PV" << QString::number(pvindx) << " Publisher|" << QString::fromStdString(curimg->ReadContent(curoffset + 318, 128).toStdString()) << "|User who specified what should be recorded." << Qt::endl;
		out << "PV" << QString::number(pvindx) << " Data Preparer|" << QString::fromStdString(curimg->ReadContent(curoffset + 446, 128).toStdString()) << "|Person or other entity which controls prepration of the data to be recorded." << Qt::endl;
		out << "PV" << QString::number(pvindx) << " Application|" << QString::fromStdString(curimg->ReadContent(curoffset + 574, 128).toStdString()) << "|How the data was recorded." << Qt::endl;
		// skipped copyright, abstract, and bilbiographic (112 bytes)
		// creation date, skipped hundreths of a second and the utc offset
		out << "PV" << QString::number(pvindx) << " Volume Creation Date|" << QString::fromStdString(curimg->ReadContent(curoffset + 813, 4).toStdString()) << "-" << QString::fromStdString(curimg->ReadContent(curoffset + 817, 2).toStdString()) << "-" << QString::fromStdString(curimg->ReadContent(curoffset + 819, 2).toStdString()) << " " << QString::fromStdString(curimg->ReadContent(curoffset + 821, 2).toStdString()) << ":" << QString::fromStdString(curimg->ReadContent(curoffset + 823, 2).toStdString()) << ":" << QString::fromStdString(curimg->ReadContent(curoffset + 825, 2).toStdString()) << "|Creation date and time." << Qt::endl;
		out << "PV" << QString::number(pvindx) << " Volume Modification Date|" << QString::fromStdString(curimg->ReadContent(curoffset + 830, 4).toStdString()) << "-" << QString::fromStdString(curimg->ReadContent(curoffset + 834, 2).toStdString()) << "-" << QString::fromStdString(curimg->ReadContent(curoffset + 836, 2).toStdString()) << " " << QString::fromStdString(curimg->ReadContent(curoffset + 838, 2).toStdString()) << ":" << QString::fromStdString(curimg->ReadContent(curoffset + 840, 2).toStdString()) << ":" << QString::fromStdString(curimg->ReadContent(curoffset + 842, 2).toStdString()) << "|Modification date and time." << Qt::endl;
		pvindx++;
	    }
	    else if(voldesctype == 0x02) // SUPPLEMENTARY/ENHANCED VOLUME DESCRIPTOR
	    {
		//qDebug() << "Supplementary/Enhanced Volume Descriptor" << svdindx;
		out << "SV" << QString::number(svdindx) << " Volume Descriptor Type|" << QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset, 1))) << "|Value for volume descriptor type, 0 - Boot Record, 1 - Primary, 2 - Supplementary or Enhanced, 3 - Partition, 4-254 - Reserved, 255 - Set Terminator." << Qt::endl;
		out << "SV" << QString::number(svdindx) << " Volume Descriptor Version|" << QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 6, 1))) << "|Value for volume descriptor version, 1 - Supplementary, 2 - Enhanced." << Qt::endl;
		out << "SV" << QString::number(svdindx) << " Escape Sequences|";
		uint32_t escapeseq = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 88, 4));
		if(escapeseq == 0x00452f25)
		    out << "Joliet Name Encoding: USC-2 Level 3";
		else if(escapeseq == 0x00432f25)
		    out << "Joliet Name Encoding: USC-2 Level 2";
		else if(escapeseq == 0x00402f25)
		    out << "Joliet Name Encoding: USC-2 Level 1";
		out << "|Escape sequence which defines standard. Joliet Level 1: %/@, Level 2: %/C, Level 3: %/E." << Qt::endl;
		out << "SV" << QString::number(svdindx) << " Path Table Size|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 132, 4))) << "|Length in bytes of a recorded occurrence of the path table identified by this volume descriptor." << Qt::endl;
		out << "SV" << QString::number(svdindx) << " Location of Occurrence of Type L Path Table|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 140, 4))) << "|Logical block number of the first logical block allocated to the extent which  contains an occurrence of the path table." << Qt::endl;
		out << "SV" << QString::number(svdindx) << " Location of Optional Occurrence of Type L Path Table|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 144, 4))) << "|Logical block number of the first logical block allocated to the extent which contains an optional occurence of the path table. If 0, it means the extent shall not be expected to be recorded." << Qt::endl;
		// Dir Record for Root Directory - 34 bytes
		out << "SV" << QString::number(svdindx) << " Root Directory Record Length|" << QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 156, 1))) << "|Length in bytes of the root directory record." << Qt::endl;
		out << "SV" << QString::number(svdindx) << " Extent Location|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 158, 4))) << "|Logical block number of the first logical block allocated to the extent." << Qt::endl;
		out << "SV" << QString::number(svdindx) << " Data Length|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 166, 4))) << "|Length in bytes of the data for the file section." << Qt::endl;
		svdindx++;
	    }
	    else if(voldesctype == 0x03) // VOLUME PARTITION DESCRIPTOR
	    {
		qDebug() << "Volume Partition Descriptor" << vpdindx;
		out << "VP" << QString::number(vpdindx) << " Volume Partition Identifier|" << QString::fromStdString(curimg->ReadContent(curoffset + 40, 31).toStdString()) << "|Indentification of the volume partition." << Qt::endl;
		out << "VP" << QString::number(vpdindx) << " Volume Partition Location|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 72, 4))) << "|Logical block number of the first logical block allocated to the volume partition." << Qt::endl;
		out << "VP" << QString::number(vpdindx) << " Volume Partition Size|" << QString::number(qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 80, 4))) << "|Number of logical blocks in which the volume partition is recorded." << Qt::endl;
		vpdindx++;
	    }
	    else if(voldesctype == 0xFF) // VOLUME DESCRIPTOR SET TERMINATOR
	    {
		//qDebug() << "Volume Descriptor Set Terminator";
	    }
	    if(voldesctype == 0xFF)
	    {
		//qDebug() << "i:" << i;
		break;
	    }
	}
	out << "Primary Volume Count|" << QString::number(pvindx - 1) << "|Number of Primary volumes." << Qt::endl;
	out << "Supplementary/Enhanced Volume Count|" << QString::number(svdindx - 1) << "|Number of Supplemnentary/Enhanded volumes." << Qt::endl;
    }
    else if(isosig == "CD001" && udfsig == "BEA01") // UDF
    {
	qInfo() << "UDF File System Found. Parsing not yet implemented.";
        out << "File System Type Int|15|Internal File System Type represented as an integer." << Qt::endl;
	out << "File System Type|UDF|File System Type String." << Qt::endl;
        //out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        partitionname += " [UDF]";
    }
    else if(hfssig == "BD") // legacy HFS
    {
	qInfo() << "HFS File System Found. Parsing not yet implemented.";
        out << "File System Type Int|16|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|HFS|File System Type String." << Qt::endl;
        uint8_t volnamelength = qFromBigEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 1060, 1));
        partitionname += QString::fromStdString(curimg->ReadContent(curstartsector*512 + 1061, volnamelength).toStdString());
        partitionname += " [HFS]";
        out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        
        /*
        // NEED TO CONVERT THE 2 DATES LISTED TO ACTUAL HUMAN READABLE DATES
        out << "Volume Creation Date|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1040, 4))) << "|Creation Date of the volume stored in local time." << Qt::endl;
        out << "Last Modification Date|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1044, 4))) << "|UTC Last modification date of the volume." << Qt::endl;
        out << "File Count|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1056, 4))) << "|Number of files on the volume." << Qt::endl;
        out << "Folder Count|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1060, 4))) << "|Number of folders on the volume." << Qt::endl;
        out << "Cluster Size|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1064, 4))) << "|Allocation Cluster Size, usually 4096 bytes." << Qt::endl;
        out << "Next Catalog ID|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1088, 4))) << "|Next available catalog ID (node id)." << Qt::endl;
        out << "Catalog Logical Size|" << QString::number(qFromBigEndian<quint64>(curimg->ReadContent(curstartsector*512 + 1296, 8))) << "|Logical size for the catalog file." << Qt::endl;
        out << "Catalog Total Blocks|" << QString::number(qFromBigEndian<uint32_t>(curimg->ReadContent(curstartsector*512 + 1308, 4))) << "|Total number of blocks for the Catalog file." << Qt::endl;
        out << "Catalog Extents Start Block Array|";
        for(int i=0; i < 8; i++)
        {
            if(qFromBigEndian<quint32>(curimg->ReadContent(curstartsector*512 + 1312 + i*8, 4)) > 0)
                out << QString::number(qFromBigEndian<quint32>(curimg->ReadContent(curstartsector*512 + 1312 + i*8, 4))) << ",";
        }
        out << "|Start block for each extent for Catalog file." << Qt::endl;
        out << "Catalog Extents Block Count Array|";
        for(int i=0; i < 8; i++)
        {
            if(qFromBigEndian<quint32>(curimg->ReadContent(curstartsector*512 + 1316 + i*8, 4)) > 0)
                out << QString::number(qFromBigEndian<quint32>(curimg->ReadContent(curstartsector*512 + 1316 + i*8, 4))) << ",";
        }
        out << "|Block count for each extent for Catalog file." << Qt::endl;
         */
    }
    else if(zfssig == 0x00bab10c) // ZFS
    {
	qInfo() << "ZFS File System Found. Parsing not yet implemeneted.";
        out << "File System Type Int|17|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|ZFS|File System Type String." << Qt::endl;
        partitionname += " [ZFS]";
    }
    else if(refsig == 0x5265465300000000) // ReFS
    {
	qInfo() << "ReFS File System Found. Parsing not yet implemeneted.";
        out << "File System Type Int|18|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|RES|File System Type String." << Qt::endl;
        //out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        partitionname += " [REFS]";
    }
    else if(f2fsig == 0xe0f5e1e2) // EROFS
    {
	qInfo() << "EROFS File System Found. Parsing not yet implemeneted.";
        out << "File System Type Int|19|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|EROFS|File System Type String." << Qt::endl;
        partitionname += " [EROFS]";
    }
    else if(bcfsig1 == 0xc68573f64e1a45ca && bcfsig2 == 0x8265f57f48ba6d81) // BCACHEFS
    {
	qInfo() << "BCACHEFS File System Found. Parsing not yet implemeneted.";
	out << "File System Type Int|20|Internal File System Type represented as an integer." << Qt::endl;
	out << "File System Type|BCACHEFS|File System Type String." << Qt::endl;
	partitionname += "[BCACHEFS]";
    }
    else if(zonesig == 0x5a4f4653) // ZONEFS
    {
        qInfo() << "ZoneFS File System Found. Parsing not yet implemented.";
        out << "File System Type Int|21|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|ZONEFS|File System Type String." << Qt::endl;
        partitionname += "[ZONEFS]";
    }
    // need to implement bfs-12, udf-15, hfs-16, zfs-17, refs-18, erofs-19, bcachefs-20, zonefs-21
    out.flush();
    propfile.close();

    return partitionname;
}

qulonglong AddVirtualFileSystemFiles(ForImg* curimg, uint8_t ptreecnt, uint8_t fatcount, qulonglong fatsize, qulonglong curinode)
{
    QHash<QString, QVariant> nodedata;
    nodedata.clear();
    nodedata.insert("name", QByteArray("$MBR").toBase64());
    nodedata.insert("path", QByteArray("/").toBase64());
    nodedata.insert("size", 512);
    nodedata.insert("create", "0");
    nodedata.insert("access", "0");
    nodedata.insert("modify", "0");
    nodedata.insert("status", "0");
    nodedata.insert("hash", "0");
    nodedata.insert("cat", "System File");
    nodedata.insert("sig", "Master Boot Record");
    nodedata.insert("tag", "0");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(curinode)));
    nodedata.insert("match", 0);
    mutex.lock();
    treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt)), 10, 0);
    mutex.unlock();
    nodedata.clear();
    curinode++;
    for(int i=0; i < fatcount; i++)
    {
	nodedata.clear();
	nodedata.insert("name", QByteArray(QString("$FAT" + QString::number(i+1)).toUtf8()).toBase64());
        nodedata.insert("path", QByteArray("/").toBase64());
        nodedata.insert("size", fatsize);
        nodedata.insert("create", "0");
        nodedata.insert("access", "0");
        nodedata.insert("modify", "0");
        nodedata.insert("status", "0");
        nodedata.insert("hash", "0");
        nodedata.insert("cat", "System File");
        nodedata.insert("sig", "File Allocation Table");
        nodedata.insert("tag", "0");
        nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(curinode)));
	nodedata.insert("match", 0);
	mutex.lock();
	treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt)), 10, 0);
	mutex.unlock();
	curinode++;
    }
    nodedata.clear();
    // NEED TO WRITE THE FILE PROPERTIES FOR THE 3 FILES - ALL I WOULD ADD WOULD BE THE LAYOUT VARIABLE, BUT THAT IS ACCOUNTED FOR IN THE PopulateHexContents() function
    return curinode;
}

void ParseDirectoryStructure(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt)
{
    // NEED TO FIGURE OUT WHAT IS NEEDED TO PARSE AN ENTRY FOR EACH FILE SYSTEM TYPE
    // THEN I NEED TO READ THAT INFORMATION FROM THE PROPRETIES FILE SO I CAN PARSE THE DIRECTORIES
    // ALSO SEE IF THERE IS A WAY TO AUTOMATE THE CALL FOR EACH FILE/DIRECTORY
    // FAT/EXFAT VARIABLES
    uint8_t fstype = 0;
    uint8_t fatcount = 0;
    uint32_t fatsize = 0;
    uint16_t bytespersector = 0;
    // BFS VARIABLES
    uint32_t blocksize = 0;
    int32_t inodesize = 0;
    int32_t rootdirag = 0;
    int32_t blksperag = 0;
    uint16_t rootdirblk = 0;
    int32_t rootindxag = 0;
    uint16_t rootindxblk = 0;
    uint32_t blockshift = 0;
    int32_t allocshift = 0;
    int32_t agcnt = 0;
    uint16_t inodesperblock = 0;
    uint64_t rootinode = 0;
    uint32_t agblocks = 0;
    uint32_t agcount = 0;
    blockrun rootblockrun;

    QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/prop");
    if(!propfile.isOpen())
	propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(propfile.isOpen())
    {
        while(!propfile.atEnd())
        {
            QString line = propfile.readLine();
            if(line.startsWith("File System Type Int|"))
                fstype = line.split("|").at(1).toUInt();
	    else if(line.startsWith("FAT Count|"))
		fatcount = line.split("|").at(1).toUInt();
	    else if(line.startsWith("FAT Size|"))
		fatsize = line.split("|").at(1).toUInt();
	    else if(line.startsWith("Bytes Per Sector|"))
		bytespersector = line.split("|").at(1).toUInt();
	    if(line.startsWith("Block Size|"))
		blocksize = line.split("|").at(1).toUInt();
	    if(line.startsWith("Inode Size|"))
		inodesize = line.split("|").at(1).toUInt();
	    if(line.startsWith("Blocks per Allocation Group|"))
		blksperag = line.split("|").at(1).toInt();
	    if(line.startsWith("Root Directory Allocation Group|"))
		rootdirag = line.split("|").at(1).toInt();
	    if(line.startsWith("Root Directory Start Block|"))
		rootdirblk = line.split("|").at(1).toUInt();
	    if(line.startsWith("Root Indices Allocation Group|"))
		rootindxag = line.split("|").at(1).toInt();
	    if(line.startsWith("Root Indices Start Block|"))
		rootindxblk = line.split("|").at(1).toUInt();
            if(line.startsWith("Block Shift|"))
                blockshift = line.split("|").at(1).toUInt();
            if(line.startsWith("Allocation Shift|"))
                allocshift = line.split("|").at(1).toInt();
            if(line.startsWith("Number of Allocation Groups|"))
                agcnt = line.split("|").at(1).toInt();
            if(line.startsWith("Inodes Per Block|"))
                inodesperblock = line.split("|").at(1).toUInt();
            if(line.startsWith("Root Inode|"))
                rootinode = line.split("|").at(1).toULongLong();
            if(line.startsWith("Allocation Group Blocks|"))
                agblocks = line.split("|").at(1).toUInt();
            if(line.startsWith("Allocation Group Count|"))
                agcount = line.split("|").at(1).toUInt();
        }
        propfile.close();
    }
    if(fstype > 0 and fstype < 4) // FAT12 || FAT16 || FAT32
    {
	qulonglong curinode = 0;
	curinode = ParseFatDirectory(curimg, curstartsector, ptreecnt, 0, "", "");
	curinode = AddVirtualFileSystemFiles(curimg, ptreecnt, fatcount, fatsize * bytespersector, curinode);
    }
    else if(fstype == 4) // EXFAT
    {
	QList<qulonglong> orphanoffsets;
	orphanoffsets.clear();
        qulonglong curinode = 0;
	curinode = ParseExfatDirectory(curimg, curstartsector, ptreecnt, 0, "", "", &orphanoffsets);
        curinode = AddVirtualFileSystemFiles(curimg, ptreecnt, fatcount, fatsize * bytespersector, curinode);
	//qDebug() << "orphanoffsets:" << orphanoffsets;
	ParseExfatOrphans(curimg, ptreecnt, curinode, &orphanoffsets);
	orphanoffsets.clear();
    }
    else if(fstype == 5) // NTFS
    {
	quint64 curinode = 0;
        QHash<quint64, quint64> dirntinodehash;
	QHash<quint64, uint16_t> ntinodehash;
	dirntinodehash.clear();
        ntinodehash.clear();
	curinode = ParseNtfsDirectory(curimg, curstartsector, ptreecnt, 5, 0, "", "", &dirntinodehash, &ntinodehash);
	//qDebug() << "ntinodehash:" << ntinodehash;
        qDebug() << "begin parsing ntfs orphans...";
        qDebug() << "curinode prior to beginning ntfs orphans:" << curinode;
        //isignals->StatUp("Parsing Forensic Image...");
        ParseNtfsOrphans(curimg, curstartsector, ptreecnt, curinode, &dirntinodehash, &ntinodehash);
        qDebug() << "end parsing ntfs orphans...";
        dirntinodehash.clear();
	ntinodehash.clear();
    }
    else if(fstype == 6) // EXT2/3/4
    {
	quint64 curinode = 0;
	curinode = ParseExtDirectory(curimg, curstartsector, ptreecnt, 2, 0, "", "");
    }
    else if(fstype == 7) // APFS
    {
        //quint64 curinode = 0;
        //qDebug() << "ptreecnt:" << ptreecnt << "partitionlist.count():" << partitionlist.count();
        qInfo() << "Parsing APFS Volumes...";
        ParseApfsVolumes(curimg, curstartsector, ptreecnt);
        //qDebug() << "ptreecnt:" << ptreecnt;
    }
    else if(fstype == 8) // HFS+/X
    {
        quint64 curinode = 0;
        curinode = ParseHfsPlusDirectory(curimg, curstartsector, ptreecnt);
    }
    else if(fstype == 9) // XFS
    {
        ParseXfs(curimg, curstartsector, ptreecnt, blocksize, (uint16_t)inodesize, inodesperblock, agblocks, agcount, rootinode);
    }
    else if(fstype == 11) // BITLOCKER
    {
        quint64 curinode = 0;
    }
    else if(fstype == 12) // BFS
    {
	quint64 curinode = 0;
	qInfo() << "Parsing BFS...";
	//curinode = ParseBfsDirectory(curimg, curstartsector, ptreecnt, 0);
	curinode = ParseBfsDirectory(curimg, curstartsector, ptreecnt, blocksize, blockshift, inodesize, blksperag, allocshift, agcnt, rootdirag, rootdirblk, rootindxag, rootindxblk, 0, rootblockrun);
    }
    else if(fstype == 14) // ISO9660
    {
	qInfo() << "Parsing ISO9660...";
	ParseIsoDirectory(curimg, curstartsector, ptreecnt);
    }
    //qDebug() << "fs type:" << fstype << "bps:" << bytespersector << "fo:" << fatoffset << "fs:" << fatsize << "rdl:" << rootdirlayout;
}

