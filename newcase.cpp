#include "newcase.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

/*
// ONCE THE TREE IS FULLY LOADED AND I START TO SAVE CASE, I NEED TO SAVE TREE TO A SINGLE FILE TO LOAD FROM...
//void ParseExtendedPartition(QString estring, uint32_t primextoff, uint32_t offset, uint32_t size, QList<qint64>* pofflist, QList<qint64>* psizelist, QList<QHash<QString, QVariant>>* fsinfolist)
//void ParseExtendedPartition(ForensicImage* curimg, uint32_t primextoff, uint32_t offset, uint32_t size, QList<qint64>* pofflist, QList<qint64>* psizelist, QList<QHash<QString, QVariant>>* fsinfolist)
void ParseExtendedPartition(ForImg* curimg, uint32_t primextoff, uint32_t offset, uint32_t size, QList<qint64>* pofflist, QList<qint64>* psizelist, QList<QHash<QString, QVariant>>* fsinfolist)
{
    //qDebug() << "primary extended size in bytes:" << size*512;
    /*
    QFile rawforimg(estring);
    if(!rawforimg.isOpen())
        rawforimg.open(QIODevice::ReadOnly);
    if(primextoff == offset)
        rawforimg.seek(offset * 512);
    else
        rawforimg.seek((primextoff + offset)*512);
    QByteArray sector = rawforimg.peek(512);
    rawforimg.close();
    */
    /*
    curimg->open(QIODevice::ReadOnly);
    if(primextoff == offset)
        curimg->seek(offset * 512);
    else
        curimg->seek((primextoff + offset)*512);
    QByteArray sector = curimg->read(512);
    curimg->close();
    */
/*    QByteArray sector = curimg->ReadContent(offset*512, 512);
    uint16_t mbrsig = qFromLittleEndian<uint16_t>(sector.mid(510,2));
    if(mbrsig == 0xaa55)
    {
        //qDebug() << "i am reading extended part correctly...";
	for(int i=0; i < 4; i++)
	{
	    int cnt = i*16;
	    QByteArray curpart = sector.mid(446 + cnt, 16);
	    uint8_t curparttype = curpart.at(4);
	    uint32_t curoffset = qFromLittleEndian<uint32_t>(curpart.mid(8, 4));
	    uint32_t cursize = qFromLittleEndian<uint32_t>(curpart.mid(12, 4));
	    if(curparttype == 0x05) // extended partition
	    {
		//ParseExtendedPartition(estring, primextoff, curoffset, cursize, pofflist, psizelist, fsinfolist);
		ParseExtendedPartition(curimg, primextoff, curoffset, cursize, pofflist, psizelist, fsinfolist);
	    }
	    else if(curparttype == 0x00)
	    {
	    }
	    else // primary partition to process
	    {
                off64_t curoff = 0;
		if(primextoff == offset)
		{
		    pofflist->append(offset+curoffset);
                    curoff = offset + curoffset;
		    //qDebug() << "part[i]:" << i << "offset:" << offset + curoffset << "cursize:" << cursize << "part type:" << QString::number(curparttype, 16);
		}
		else
		{
		    pofflist->append(primextoff + offset + curoffset);
                    curoff = primextoff + offset + curoffset;
		    //qDebug() << "part[i]:" << i << "offset:" << primextoff + offset + curoffset << "cursize:" << cursize << "part type:" << QString::number(curparttype, 16);
		}
		psizelist->append(cursize);
                ParseFileSystemInformation(curimg, curoff, fsinfolist);
                //ParseFileSystemInformation(curimg, curoff, fsinfolist);
                //ParseFileSystemInformation(estring, curoff, fsinfolist);
	    }
	}
    }
    //else
        //qDebug() << "i screwed up the math somewhere...";
}

//void ParseVolume(QString estring, qint64 imgsize, QList<qint64>* pofflist, QList<qint64>* psizelist, QList<QHash<QString, QVariant>>* fsinfolist) // would have to add &fsinfolist here...
//void ParseVolume(ForensicImage* tmpimg, qint64 imgsize, QList<qint64>* pofflist, QList<qint64>* psizelist, QList<QHash<QString, QVariant>>* fsinfolist) // would have to add &fsinfolist here...
void ParseVolume(ForImg* curimg, qint64 imgsize, QList<qint64>* pofflist, QList<qint64>* psizelist, QList<QHash<QString, QVariant>>* fsinfolist) // would have to add &fsinfolist here...
{
    //QIODevice* rawforimg = tmpimg;
    QString estring = curimg->ImgPath();
    //qDebug() << "estring:" << estring;
    if(estring.toLower().endsWith(".e01"))
    {
        //EwfImage* rawforimg2 = (EwfImage*)tmpimg;
        //char* tdata = new char[512];
        //char* zipbuf = new char[zipstat.size];
        //testimage->seek(0);
        //QByteArray tmparray = testimage->read(512);
        //qDebug() << "1st sector:" << tmparray.toHex();
        //qint64 retval = testimage->readData(tdata, 512);
        //qDebug() << "retval:" << retval;
        //qDebug() << "1st sector:" << QByteArray::fromRawData((const char*)tdata, 512).toHex();
        // THIS WORKS!!!!! SO I NEED TO SET A GLOBAL VECTOR WHICH CONTAINS THESE IMAGES TO LOOP OVER IT AS SUCH...
        // ALSO NEED TO MODIFY THE ENTIRE CODE TO ACCOUNT FOR THIS SOMEHOW....
    }
    else
    {
    //QByteArray sector0 = rawforimg->read(512);
    //rawforimg->close();
    QByteArray sector0 = curimg->ReadContent(0, 512);
    uint16_t mbrsig = qFromLittleEndian<uint16_t>(sector0.mid(510, 2));
    uint16_t applesig = qFromLittleEndian<uint16_t>(sector0.left(2)); // should be in 2nd sector, powerpc mac's not intel mac's
    uint32_t bsdsig = qFromLittleEndian<uint32_t>(sector0.left(4)); // can be at start of partition entry of a dos mbr
    uint16_t sunsig = qFromLittleEndian<uint16_t>(sector0.mid(508, 2)); // worry about it later, i386 sun can be at 2nd sector of partition entry of a dos mbr
    uint64_t gptsig = qFromLittleEndian<uint64_t>(sector0.left(8));
    QString exfatstr = QString::fromStdString(sector0.mid(3, 5).toStdString());
    QString fatstr = QString::fromStdString(sector0.mid(54, 5).toStdString());
    QString fat32str = QString::fromStdString(sector0.mid(82, 5).toStdString());

    if(mbrsig == 0xaa55) // POSSIBLY MBR OR GPT
    {
        //qDebug() << "imgsize:" << imgsize;
	if((uint8_t)sector0.at(450) == 0xee) // GPT DISK
        {
            QByteArray sector1 = curimg->ReadContent(512, 512);
            //QByteArray sector1 = rawforimg->read(512);
            //rawforimg.close();
	    gptsig = qFromLittleEndian<uint64_t>(sector1.left(8));
	    if(gptsig == 0x5452415020494645) // GPT PARTITION TABLE
	    {
		//qDebug() << "gpt found, parse table now...";
		uint32_t parttablestart = qFromLittleEndian<uint32_t>(sector1.mid(72, 8));
		uint16_t partentrycount = qFromLittleEndian<uint16_t>(sector1.mid(80, 4));
		uint16_t partentrysize = qFromLittleEndian<uint16_t>(sector1.mid(84, 4));
                QByteArray partentries = curimg->ReadContent(parttablestart*512, partentrycount*partentrysize);
		//rawforimg.close();
		for(int i=0; i < partentrycount; i++)
		{
		    int cnt = i*partentrysize;
		    uint32_t curstartsector = qFromLittleEndian<uint32_t>(partentries.mid(cnt + 32, 8));
		    uint32_t curendsector = qFromLittleEndian<uint32_t>(partentries.mid(cnt + 40, 8));
		    if((curendsector - curstartsector) > 0)
		    {
			pofflist->append(curstartsector);
			psizelist->append((curendsector - curstartsector + 1));
                        //qDebug() << "begin parse file system information";
                        ParseFileSystemInformation(curimg, curstartsector, fsinfolist);
                        //ParseFileSystemInformation(tmpimg, curstartsector, fsinfolist);
                        //ParseFileSystemInformation(estring, curstartsector, fsinfolist);
			//qDebug() << "partition[" << i << "] start sector:" << curstartsector << "end sector:" << curendsector << "cur size:" << curendsector - curstartsector + 1;
		    }

		}
		//qDebug() << "gpt table start sector:" << parttablestart << "partentry count:" << partentrycount << "partentrysize:" << partentrysize;
	    }
	    else
		qDebug() << "gpt should have been there, math is off...";
        }
	else // MBR DISK
	{
	    if(exfatstr.startsWith("NTFS") || exfatstr == "EXFAT" || fatstr == "FAT12" || fatstr == "FAT16" || fat32str == "FAT32") // NTFS | EXFAT | FAT12 | FAT16 | FAT32
            {
                // Windows partition/fs which starts at beginning of image with no partition table
            }
            else
            {
                for(int i=0; i < 4; i++)
                {
                    int cnt = i*16;
                    QByteArray curpart = sector0.mid(446 + cnt, 16);
                    uint8_t curparttype = curpart.at(4);
                    uint32_t curoffset = qFromLittleEndian<uint32_t>(curpart.mid(8, 4));
                    uint32_t cursize = qFromLittleEndian<uint32_t>(curpart.mid(12, 4));
                    if(curparttype == 0x05) // extended partition
                    {
                        //qDebug() << "extended partition offset:" << curoffset << "size:" << cursize;
                        //qDebug() << "parse extended partition recurse loop here...";
                        //ParseExtendedPartition(estring, curoffset, curoffset, cursize, pofflist, psizelist, fsinfolist); // add fsinfolist here as well...
                        //ParseExtendedPartition(tmpimg, curoffset, curoffset, cursize, pofflist, psizelist, fsinfolist); // add fsinfolist here as well...
                        ParseExtendedPartition(curimg, curoffset, curoffset, cursize, pofflist, psizelist, fsinfolist); // add fsinfolist here as well...
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
                        pofflist->append(curoffset);
                        psizelist->append(cursize);
                        qDebug() << "begin parse file system information";
                        ParseFileSystemInformation(curimg, curoffset, fsinfolist);
                        //ParseFileSystemInformation(tmpimg, curoffset, fsinfolist);
                        //ParseFileSystemInformation(estring, curoffset, fsinfolist);
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
        //qDebug() << "gpt and parse accordingly.";
	uint32_t parttablestart = qFromLittleEndian<uint32_t>(sector0.mid(72, 8));
	uint16_t partentrycount = qFromLittleEndian<uint16_t>(sector0.mid(80, 4));
	uint16_t partentrysize = qFromLittleEndian<uint16_t>(sector0.mid(84, 4));
        QByteArray partentries = curimg->ReadContent(parttablestart*512, partentrycount*partentrysize);
	//rawforimg.close();
	for(int i=0; i < partentrycount; i++)
	{
	    int cnt = i*partentrysize;
	    uint32_t curstartsector = qFromLittleEndian<uint32_t>(partentries.mid(cnt + 32, 8));
	    uint32_t curendsector = qFromLittleEndian<uint32_t>(partentries.mid(cnt + 40, 8));
	    if((curendsector - curstartsector) > 0)
	    {
		pofflist->append(curstartsector);
		psizelist->append((curendsector - curstartsector + 1));
                //qDebug() << "begin parse file system information";
                ParseFileSystemInformation(curimg, curstartsector, fsinfolist);
                //ParseFileSystemInformation(tmpimg, curstartsector, fsinfolist);
                //ParseFileSystemInformation(estring, curstartsector,fsinfolist);
		//qDebug() << "partition[" << i << "] start sector:" << curstartsector << "end sector:" << curendsector << "cur size:" << curendsector - curstartsector + 1;
	    }
	}
    }
    else
        qDebug() << "partition signature not found correctly";
}

//void ParseFileSystemInformation(QString estring, off64_t partoffset, QList<QHash<QString, QVariant>> *fsinfolist)
//void ParseFileSystemInformation(ForensicImage* curimg, off64_t partoffset, QList<QHash<QString, QVariant>> *fsinfolist)
void ParseFileSystemInformation(ForImg* curimg, off64_t partoffset, QList<QHash<QString, QVariant>> *fsinfolist)
{
    //QList<QHash<QString, QVariant>> fileinfolist;
    QHash<QString, QVariant> fsinfo;
    //qDebug() << "estring:" << estring << "partoffset:" << partoffset;
    QByteArray partbuf;
    partbuf.clear();
    partbuf = curimg->ReadContent(partoffset*512, 69120);
    // check for various FS's
    uint16_t winsig = qFromLittleEndian<uint16_t>(partbuf.mid(510, 2));
    uint16_t extsig = qFromLittleEndian<uint16_t>(partbuf.mid(1080, 2));
    QString apfssig = QString::fromStdString(partbuf.mid(32, 4).toStdString());
    QString hfssig = QString::fromStdString(partbuf.mid(1024, 2).toStdString());
    QString xfssig = QString::fromStdString(partbuf.mid(0, 4).toStdString());
    QString btrsig = QString::fromStdString(partbuf.mid(65600, 8).toStdString());
    QString bitlcksig = QString::fromStdString(partbuf.mid(0, 8).toStdString());
    QString bfssig = QString::fromStdString(partbuf.mid(544, 4).toStdString());
    QString isosig = QString::fromStdString(partbuf.mid(32769, 5).toStdString());
    QString udfsig = QString::fromStdString(partbuf.mid(40961, 5).toStdString());
    uint64_t refsig = qFromLittleEndian<uint64_t>(partbuf.mid(3, 8)); // should be 0x00 00 00 00 53 46 65 52 (0 0 0 0 S F e R) prior to endian flip
    //QString refsig = QString::fromStdString(
    uint32_t f2fssig = qFromLittleEndian<uint32_t>(partbuf.mid(1024, 4));

    if(winsig == 0xaa55) // FAT OR NTFS
    {
	QString exfatstr = QString::fromStdString(partbuf.mid(3, 5).toStdString());
	//qDebug() << "exfatstr:" << exfatstr;
	QString fatstr = QString::fromStdString(partbuf.mid(54, 5).toStdString());
        QString fat32str = QString::fromStdString(partbuf.mid(82, 5).toStdString());
	if(fatstr == "FAT12") // FAT12
        {
            fsinfo.insert("type", QVariant(1));
            fsinfo.insert("typestr", QVariant("FAT12"));
        }
	else if(fatstr == "FAT16") // FAT16
        {
            fsinfo.insert("type", QVariant(2));
            fsinfo.insert("typestr", QVariant("FAT16"));
        }
	else if(fat32str == "FAT32") // FAT32
        {
            fsinfo.insert("type", QVariant(3));
            fsinfo.insert("typestr", QVariant("FAT32"));
        }
	else if(exfatstr == "EXFAT") // EXFAT
        {
            fsinfo.insert("type", QVariant(4));
            fsinfo.insert("typestr", QVariant("EXFAT"));
        }
	else if(exfatstr.startsWith("NTFS")) // NTFS
        {
            fsinfo.insert("type", QVariant(5));
            fsinfo.insert("typestr", QVariant("NTFS"));
	    fsinfo.insert("partoffset", QVariant((qulonglong)partoffset));
            fsinfo.insert("bytespersector", QVariant(qFromLittleEndian<uint16_t>(partbuf.mid(11, 2))));
            //qDebug() << "bytes per sector:" << fsinfo.value("bytespersector").toUInt();
            fsinfo.insert("sectorspercluster", QVariant(partbuf.at(13)));
            //qDebug() << "sectors per cluster:" << fsinfo.value("sectorspercluster").toUInt();
            //qDebug() << "bytes per cluster:" << fsinfo.value("sectorspercluster").toUInt() * fsinfo.value("bytespersector").toUInt();
            fsinfo.insert("bytespercluster", fsinfo.value("sectorspercluster").toUInt() * fsinfo.value("bytespersector").toUInt());
            fsinfo.insert("totalsectors", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(40, 8))));
            fsinfo.insert("mftstartingcluster", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(48, 8))));
            //qDebug() << "MFT starting cluster:" << fsinfo.value("mftstartingcluster").toUInt();
	    //qDebug() << "mftentrysize:" << (uint8_t)partbuf.at(64);
            fsinfo.insert("mftentrysize", QVariant((uint8_t)partbuf.at(64)));
	    qulonglong mftentrybytes = fsinfo.value("mftentrysize").toUInt() * fsinfo.value("bytespersector").toUInt() * fsinfo.value("sectorspercluster").toUInt();
	    //qDebug() << "mftentrybytes:" << mftentrybytes;
            //fsinfo.insert("mftentrybytes", QVariant(mftentrybytes));
	    fsinfo.insert("mftentrybytes", QVariant(1024));
            mftentrybytes = 1024;
	    //qDebug() << "mft entry size in bytes:" << mftentrybytes;
            fsinfo.insert("indexrecordsize", QVariant(partbuf.at(68)));
            fsinfo.insert("serialnum", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(72, 8))));
            //qDebug() << "serial num:" << QString("0x" + QString::number(fsinfo.value("serialnum").toULongLong(), 16));
	    //qDebug() << "partoffset:" << partoffset << "sectorspercluster:" << fsinfo.value("sectorspercluster").toUInt() << "bytespersector:" << fsinfo.value("bytespersector").toUInt();
            fsinfo.insert("mftoffset", QVariant((qulonglong)((partoffset * 512) + (fsinfo.value("mftstartingcluster").toUInt() * fsinfo.value("sectorspercluster").toUInt() * fsinfo.value("bytespersector").toUInt()))));
	    //qDebug() << "mftstarting cluster;" << fsinfo.value("mftstartingcluster").toUInt();
            //qDebug() << "mftoffset:" << fsinfo.value("mftoffset").toUInt();
            // get MFT entry for $MFT to determine cluster's that contain the MFT...
            QByteArray mftentry0;
	    QByteArray mftentry3;
            mftentry0.clear();
	    mftentry3.clear();
            mftentry0 = curimg->ReadContent(fsinfo.value("mftoffset").toLongLong(), mftentrybytes);
            mftentry3 = curimg->ReadContent(fsinfo.value("mftoffset").toLongLong() + 3*mftentrybytes, mftentrybytes);
            //qDebug() << "MFT ENTRY SIGNATURE:" << QString::fromStdString(mftentry0.left(4).toStdString());
            if(QString::fromStdString(mftentry0.left(4).toStdString()) == "FILE") // a proper mft entry
            {
                int curoffset = 0;
                uint16_t firstattroffset = qFromLittleEndian<uint16_t>(mftentry0.mid(20, 2)); // offset to first attribute
                //uint32_t mftentryusedsize = qFromLittleEndian<uint32_t>(mftentry0.mid(24, 4)); // mft entry used size
                uint16_t attrcount = qFromLittleEndian<uint16_t>(mftentry0.mid(40, 2)); // next attribute id
		uint32_t attrlength = 0;
                //qDebug() << "first attr offset:" << firstattroffset << "attr count:" << attrcount;
                curoffset = firstattroffset;
                for(int i=0; i < attrcount; i++)
                {
                    uint32_t atrtype = qFromLittleEndian<uint32_t>(mftentry0.mid(curoffset, 4)); // attribute type
                    uint8_t namelength = qFromLittleEndian<uint8_t>(mftentry0.mid(curoffset + 9, 1)); // length of name
                    attrlength = qFromLittleEndian<uint32_t>(mftentry0.mid(curoffset + 4, 4)); // attribute length
                    //qDebug() << "attr type:" << atrtype << "curoffset:" << curoffset << "attrlength:" << attrlength;
		    if(atrtype == 128 && namelength == 0) // $DATA attribute to parse
		    {
			break;
		    }
                    curoffset += attrlength;
                }
		//qDebug() << "curoffset of $Data attribute:" << curoffset;
		uint64_t vcnstart = qFromLittleEndian<uint64_t>(mftentry0.mid(curoffset + 16, 8));
		uint64_t vcnend = qFromLittleEndian<uint64_t>(mftentry0.mid(curoffset + 24, 8));
		uint16_t runlistoff = qFromLittleEndian<uint16_t>(mftentry0.mid(curoffset + 32, 2));
		//uint16_t compressszunit = qFromLittleEndian<uint16_t>(mftentry0.mid(curoffset + 34, 2));
		uint64_t actualattrcontentsize = qFromLittleEndian<uint64_t>(mftentry0.mid(curoffset + 48, 8));
		//qDebug() << "Starting VCN:" << vcnstart << "Ending VCN:" << vcnend;
		//qDebug() << "attrlength:" << attrlength << "runlistoff:" << runlistoff << "actual attr content size:" << actualattrcontentsize;
		curoffset = curoffset + runlistoff;
		//qDebug() << "run list start curoffset:" << curoffset;
		int i=0;
                //QString runliststr = "";
                QStringList runlist;
                uint mftsize = 0;
                runlist.clear();
		while(curoffset < mftentrybytes) // might have to do a while < mftentrybytes and then go from there.... to build the curoffset = curoffset + 3, 4, etc...
		{
		    if(mftentry0.at(curoffset) > 0)
		    {
			//qDebug() << "1st byte:" << QString::number(mftentry0.at(curoffset), 16);
			int runlengthbytes = QString(QString::number(mftentry0.at(curoffset), 16).at(1)).toInt();
			int runlengthoffset = QString(QString::number(mftentry0.at(curoffset), 16).at(0)).toInt();
			if(runlengthbytes == 0 && runlengthoffset == 0)
			    break;
			curoffset++;
			uint runlength = 0;
			int runoffset = 0;
			if(runlengthbytes == 1)
			    runlength = qFromLittleEndian<uint8_t>(mftentry0.mid(curoffset, runlengthbytes));
			else
			    runlength = qFromLittleEndian<uint>(mftentry0.mid(curoffset, runlengthbytes));
			if(runlengthoffset == 1)
			    runoffset = qFromLittleEndian<int8_t>(mftentry0.mid(curoffset + runlengthbytes, runlengthoffset));
			else
			    runoffset = qFromLittleEndian<int>(mftentry0.mid(curoffset + runlengthbytes, runlengthoffset));
			//qDebug() << "run [" << i << "] length:" << runlengthbytes << "run [" << i << "] offset:" << runlengthoffset;
			
			//int runlength = qFromLittleEndian<int>(mftentry0.mid(curoffset, runlengthbytes));
			//int runoffset = qFromLittleEndian<int>(mftentry0.mid(curoffset + runlengthbytes, runlengthoffset));
                        //qDebug() << "temporary runoffset prior to adding unlist in:" << QString::number(runoffset, 16);
                        if(i > 0)
                        {
                            if(i > 1 && QString::number(runoffset, 16).right(1).toInt() == 1)
                                runoffset = runoffset - 0xffff - 1;
                            runoffset = runoffset + runlist.at(i-1).split(",").at(0).toUInt();
                        }
			//qDebug() << "runoffset cluster:" << runoffset << "runlength (clusters):" << runlength;
                        //runliststr += QString::number(runoffset) + "," + QString::number(runlength) + ";";
                        runlist.append(QString::number(runoffset) + "," + QString::number(runlength));
                        mftsize += runlength;
                        //runlist.append(QString::number((partoffset * 512) + runoffset * fsinfo.value("bytespercluster").toUInt()) + "," + QString::number(runlength * fsinfo.value("bytespercluster").toUInt()));
                        //if(i != 0)
                            //runoffset = runoffset 
			i++;
			curoffset += runlengthbytes + runlengthoffset;
			//qDebug() << "current offset after run [" << i-1 << "]" << curoffset;
                        //qDebug() << "bytes per cluster:" << fsinfo.value("sectorspercluster").toUInt() * fsinfo.value("bytespersector").toUInt();
			//qDebug() << "mft byte start:" << runoffset * fsinfo.value("sectorspercluster").toUInt() * fsinfo.value("bytespersector").toUInt();
			//qDebug() << "mft byte count:" << runlength * fsinfo.value("sectorspercluster").toUInt() * fsinfo.value("bytespersector").toUInt();
		    }
		    else
			break;
		    //break;
		}
		//qDebug() << "runlist:" << runlist;
                QString runliststr = "";
                for(int i=0; i < runlist.count(); i++)
                    runliststr += QString::number((partoffset * 512) + (runlist.at(i).split(",").at(0).toULongLong() * fsinfo.value("bytespercluster").toUInt())) + "," + QString::number(runlist.at(i).split(",").at(1).toULongLong() * fsinfo.value("bytespercluster").toUInt()) + ";";
                fsinfo.insert("mftlayout", QVariant(runliststr));
                fsinfo.insert("maxmftentries", (mftsize * fsinfo.value("bytespercluster").toUInt()) / mftentrybytes);
                //qDebug() << "mftlayout:" << fsinfo.value("mftlayout").toString();
            }
            else
                qDebug() << "error this is not a valid MFT ENTRY...";
	    //qDebug() << "MFT ENTRY FOR $VOLUME SIGNATURE:" << QString::fromStdString(mftentry3.left(4).toStdString());
	    if(QString::fromStdString(mftentry3.left(4).toStdString()) == "FILE") // a proper mft entry
	    {
		int curoffset = 0;
                uint16_t firstattroffset = qFromLittleEndian<uint16_t>(mftentry3.mid(20, 2)); // offset to first attribute
                //uint32_t mftentryusedsize = qFromLittleEndian<uint32_t>(mftentry0.mid(24, 4)); // mft entry used size
                uint16_t attrcount = qFromLittleEndian<uint16_t>(mftentry3.mid(40, 2)); // next attribute id
		uint32_t attrlength = 0;
                //qDebug() << "first attr offset:" << firstattroffset << "attr count:" << attrcount;
                curoffset = firstattroffset;
                for(int i=0; i < attrcount; i++)
                {
                    uint32_t atrtype = qFromLittleEndian<uint32_t>(mftentry3.mid(curoffset, 4)); // attribute type
                    //uint8_t namelength = qFromLittleEndian<uint8_t>(mftentry3.mid(curoffset + 9, 1)); // length of name
                    attrlength = qFromLittleEndian<uint32_t>(mftentry3.mid(curoffset + 4, 4)); // attribute length
                    //qDebug() << "attr type:" << atrtype << "curoffset:" << curoffset << "attrlength:" << attrlength;
		    if(atrtype == 96) // $VOLUME_NAME attribute to parse (always resident)
		    {
			break;
		    }
                    curoffset += attrlength;
                }
		uint32_t contentsize = qFromLittleEndian<uint32_t>(mftentry3.mid(curoffset + 16, 4));
		uint16_t contentoffset = qFromLittleEndian<uint16_t>(mftentry3.mid(curoffset + 20, 2));
		//qDebug() << "curoffset of $Volume attribute:" << curoffset;
		//qDebug() << "vol content offset:" << contentoffset << "vol  content size:" << contentsize;
		//qDebug() << "volname name:" << QString::fromStdString(mftentry3.mid(curoffset + contentoffset, contentsize).toStdString());
		// EASY WAY TO READ UTF16 STRING !!!!!!!!!!
		fsinfo.insert("vollabel", QVariant(QString::fromUtf16(reinterpret_cast<const ushort*>(mftentry3.mid(curoffset + contentoffset, contentsize).data()))));
		QString volnamestr = QString::fromUtf16(reinterpret_cast<const ushort*>(mftentry3.mid(curoffset + contentoffset, contentsize).data()));
		//qDebug() << "volnamestr:" << volnamestr;
	    }
        }
        // CAN MOVE BELOW TO A FUNCTION PROBABLY FOR CLEANLINESS...
        // SAME WITH WHEN I RUN THROUGH ALL THE DIRECTORY ENTRIES...
        if(fatstr == "FAT12" || fatstr == "FAT16" || fat32str == "FAT32" || exfatstr == "EXFAT")
        {
            fsinfo.insert("bytespersector", QVariant(qFromLittleEndian<uint16_t>(partbuf.mid(11, 2))));
            fsinfo.insert("sectorspercluster", QVariant(partbuf.at(13)));
            fsinfo.insert("reservedareasize", QVariant(qFromLittleEndian<uint16_t>(partbuf.mid(14, 2))));
            fsinfo.insert("fatcount", QVariant(partbuf.at(16)));
            fsinfo.insert("rootdirmaxfiles", QVariant(qFromLittleEndian<uint16_t>(partbuf.mid(17, 2))));
            fsinfo.insert("fssectorcnt", QVariant(qFromLittleEndian<uint16_t>(partbuf.mid(19, 2))));
            fsinfo.insert("mediatype", QVariant(partbuf.at(21)));
            fsinfo.insert("fs32sectorcnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(32, 4))));
            //qDebug() << "fat multiplier:" << fatmultiplier;
            //int rootdirsectors = 0;
            if(fatstr == "FAT12" || fatstr == "FAT16")
            {
		fsinfo.insert("fatsize", QVariant(qFromLittleEndian<uint16_t>(partbuf.mid(22, 2))));
                //rootdirsectors = ((fsinfo.value("rootdirmaxfiles").toUInt() * 32) + (fsinfo.value("bytespersector").toUInt() - 1)) / fsinfo.value("bytespersector").toUInt();
                fsinfo.insert("extbootsig", QVariant(partbuf.at(38)));
                fsinfo.insert("volserialnum", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(39, 4))));
                //qDebug() << "volserialnum:" << QString("0x" + QString::number(fsinfo.value("volserialnum").toUInt(), 16));
                fsinfo.insert("vollabel", QVariant(QString::fromStdString(partbuf.mid(43, 11).toStdString())));
                fsinfo.insert("fatlabel", QVariant(QString::fromStdString(partbuf.mid(54, 8).toStdString())));
                fsinfo.insert("rootdiroffset", QVariant((qulonglong)(partoffset * 512) + (fsinfo.value("reservedareasize").toUInt() + (fsinfo.value("fatcount").toUInt() * fsinfo.value("fatsize").toUInt())) * fsinfo.value("bytespersector").toUInt()));
                fsinfo.insert("rootdirsectors", QVariant(((fsinfo.value("rootdirmaxfiles").toUInt() * 32) + (fsinfo.value("bytespersector").toUInt() - 1)) / fsinfo.value("bytespersector").toUInt()));
                fsinfo.insert("rootdirsize", QVariant(fsinfo.value("rootdirsectors").toUInt() * fsinfo.value("bytespersector").toUInt()));
                fsinfo.insert("clusterareastart", QVariant((qulonglong)partoffset + fsinfo.value("reservedareasize").toUInt() + (fsinfo.value("fatcount").toUInt() * fsinfo.value("fatsize").toUInt()) + fsinfo.value("rootdirsectors").toUInt()));
                fsinfo.insert("rootdirlayout", QVariant(QString(QString::number(fsinfo.value("rootdiroffset").toUInt()) + "," + QString::number(fsinfo.value("rootdirsize").toUInt()) + ";")));
                fsinfo.insert("fatoffset", QVariant((qulonglong)(partoffset * 512) + fsinfo.value("reservedareasize").toUInt() * fsinfo.value("bytespersector").toUInt()));
		QString rootdirlayout = QString::number(fsinfo.value("rootdiroffset").toUInt()) + "," + QString::number(fsinfo.value("rootdirsize").toUInt()) + ";";
		fsinfo.insert("rootdirlayout", QVariant(rootdirlayout));
                //qDebug() << "rootdiroffset:" << fsinfo.value("rootdiroffset").toUInt() << fsinfo.value("rootdiroffset").toUInt()/fsinfo.value("bytespersector").toUInt();
                //qDebug() << "fatoffset:" << fsinfo.value("fatoffset").toUInt() << fsinfo.value("fatoffset").toUInt()/fsinfo.value("bytespersector").toUInt();
                //qDebug() << "reserved area size:" << fsinfo.value("reservedareasize").toUInt() << "fatcount:" << fsinfo.value("fatcount").toUInt() << "fatsize:" << fsinfo.value("fatsize").toUInt();
                //qDebug() << "root dir start:" << fsinfo.value("reservedareasize").toUInt() + (fsinfo.value("fatcount").toUInt() * fsinfo.value("fatsize").toUInt());
                //qDebug() << "root dir sectors:" << fsinfo.value("rootdirsectors").toUInt();
                //qDebug() << "cluster area/data start:" << fsinfo.value("reservedareasize").toUInt() + (fsinfo.value("fatcount").toUInt() * fsinfo.value("fatsize").toUInt()) + rootdirsectors;
            }
            else if(fat32str == "FAT32")
            {
                fsinfo.insert("extbootsig", QVariant(partbuf.at(66)));
                fsinfo.insert("volserialnum", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(67, 4))));
                fsinfo.insert("vollabel", QVariant(QString::fromStdString(partbuf.mid(71, 11).toStdString())));
                fsinfo.insert("fatlabel", QVariant(QString::fromStdString(partbuf.mid(82, 8).toStdString())));
                fsinfo.insert("fatsize", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(36, 4))));
                //fsinfo.insert("fat32size", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(36, 4))));
                fsinfo.insert("rootdircluster", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(44, 4))));
		//qDebug() << "rootdircluster:" << fsinfo.value("rootdircluster").toUInt();
                fsinfo.insert("rootdiroffset", QVariant((qulonglong)((partoffset * 512) + (fsinfo.value("reservedareasize").toUInt() + (fsinfo.value("fatcount").toUInt() * fsinfo.value("fatsize").toUInt())) * fsinfo.value("bytespersector").toUInt())));
                fsinfo.insert("fatoffset", QVariant((qulonglong)((partoffset * 512) + fsinfo.value("reservedareasize").toUInt() * fsinfo.value("bytespersector").toUInt())));
		//qDebug() << "fatoffset:" << fsinfo.value("fatoffset").toUInt();
                fsinfo.insert("clusterareastart", QVariant((qulonglong)(partoffset + fsinfo.value("reservedareasize").toUInt() + (fsinfo.value("fatcount").toUInt() * fsinfo.value("fatsize").toUInt()))));
                QByteArray rootfatbuf;
                rootfatbuf.clear();
                rootfatbuf = curimg->ReadContent(fsinfo.value("fatoffset").toLongLong(), fsinfo.value("fatsize").toLongLong() * fsinfo.value("bytespersector").toLongLong());
		//qDebug() << "root fat content:" << rootfatbuf.mid(0, 40).toHex();
                QList<uint> rootclusterlist;
                rootclusterlist.clear();
		//qDebug() << "typestr:" << fsinfo.value("typestr").toString();
		//qDebug() << "start get next cluster";
		//qDebug() << "rootdiroffset:" << fsinfo.value("rootdiroffset").toUInt() << fsinfo.value("rootdiroffset").toUInt() / fsinfo.value("bytespersector").toUInt();
		if(fsinfo.value("rootdircluster").toUInt() >= 2)
		    GetNextCluster(fsinfo.value("rootdircluster").toUInt(), fsinfo.value("type").toUInt(), &rootfatbuf, &rootclusterlist);
		//qDebug() << "end get next cluster";
		QString clustersize = QString::number(fsinfo.value("sectorspercluster").toUInt() * fsinfo.value("bytespersector").toUInt());
		QString rootdirlayout = QString::number(fsinfo.value("rootdiroffset").toUInt() + (fsinfo.value("rootdircluster").toUInt() - 2) * fsinfo.value("sectorspercluster").toUInt() * fsinfo.value("bytespersector").toUInt()) + "," + clustersize + ";";
		for(int j=0; j < rootclusterlist.count() - 1; j++)
		    rootdirlayout += QString::number((fsinfo.value("clusterareastart").toUInt() * fsinfo.value("bytespersector").toUInt()) + (rootclusterlist.at(j) - 2) * clustersize.toUInt()) + "," + clustersize + ";";
		fsinfo.insert("rootdirlayout", QVariant(rootdirlayout));
		//qDebug() << "rootdirlayout:" << rootdirlayout;
                // DON'T HAVE "ROOTDIRSECTORS" OR "ROOTDIRSIZE" YET... "ROOTDIRMAXFILES" is 0
                //qDebug() << "rootdiroffset:" << fsinfo.value("rootdiroffset").toUInt() << fsinfo.value("rootdiroffset").toUInt()/fsinfo.value("bytespersector").toUInt();
                //qDebug() << "fatoffset:" << fsinfo.value("fatoffset").toUInt() << fsinfo.value("fatoffset").toUInt()/fsinfo.value("bytespersector").toUInt();
                //qDebug() << "reserved area size:" << fsinfo.value("reservedareasize").toUInt() << "fatcount:" << fsinfo.value("fatcount").toUInt() << "fatsize:" << fsinfo.value("fat32size").toUInt();
                //qDebug() << "rootdirsectors:" << rootdirsectors;
                //qDebug() << "cluster area/data start (also root dir):" << fsinfo.value("reservedareasize").toUInt() + (fsinfo.value("fatcount").toUInt() * fsinfo.value("fat32size").toUInt()) + rootdirsectors;
                // FirstSectorOfCluster = ((N-2) * sectorspercluster) + firstdatasector [rootdirstart];
            }
            else if(exfatstr == "EXFAT")
            {
                fsinfo.insert("partoffset", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(64, 8)))); // sector address
                fsinfo.insert("vollength", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(72, 8)))); // volume size in sectors
                fsinfo.insert("fatsector", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(80, 4)))); // sector address of 1st FAT
                fsinfo.insert("fatsize", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(84, 4)))); // size in sectors
                fsinfo.insert("clusterstart", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(88, 4)))); // sector address of cluster heap/data region
                fsinfo.insert("clustercount", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(92, 4)))); // number of clusters in heap
                fsinfo.insert("rootdircluster", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(96, 4)))); // cluster address
                fsinfo.insert("volserialnum", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(100, 4))));
                fsinfo.insert("bytespersector", QVariant(pow(2, partbuf.at(108)))); // power of 2, so 2^(bytespersector)
                fsinfo.insert("sectorspercluster", QVariant(pow(2, partbuf.at(109)))); // power of 2, so 2^(sectorspercluster)
                fsinfo.insert("fatcount", QVariant(partbuf.at(110))); // 1 or 2, 2 if TexFAT is in use
                fsinfo.insert("fatoffset", QVariant((fsinfo.value("fatsector").toUInt() * fsinfo.value("bytespersector").toUInt()) + (qulonglong)(partoffset * 512)));
		fsinfo.insert("clusteroffset", QVariant((qulonglong)(partoffset * 512) + (fsinfo.value("clusterstart").toUInt() * fsinfo.value("bytespersector").toUInt())));
                fsinfo.insert("rootdiroffset", QVariant((qulonglong)((partoffset * 512) + (fsinfo.value("clusterstart").toUInt() + ((fsinfo.value("rootdircluster").toUInt() - 2) * fsinfo.value("sectorspercluster").toUInt())) * fsinfo.value("bytespersector").toUInt())));
                QByteArray rootfatbuf;
                rootfatbuf.clear();
                rootfatbuf = curimg->ReadContent(fsinfo.value("fatoffset").toLongLong(), fsinfo.value("fatsize").toLongLong() * fsinfo.value("bytespersector").toLongLong());
                QList<uint> rootclusterlist;
                rootclusterlist.clear();
                if(fsinfo.value("rootdircluster").toUInt() >= 2)
                    GetNextCluster(fsinfo.value("rootdircluster").toUInt(), fsinfo.value("type").toUInt(), &rootfatbuf, &rootclusterlist);
                QString clustersize = QString::number(fsinfo.value("sectorspercluster").toUInt() * fsinfo.value("bytespersector").toUInt());
                QString rootdirlayout = QString::number(fsinfo.value("rootdiroffset").toUInt()) + "," + clustersize + ";";
                for(int j=0; j < rootclusterlist.count() - 1; j++)
                    rootdirlayout += QString::number((fsinfo.value("clusterstart").toUInt() * fsinfo.value("bytespersector").toUInt()) + (rootclusterlist.at(j) - 2) * clustersize.toUInt()) + "," + clustersize + ";";
                fsinfo.insert("rootdirlayout", QVariant(rootdirlayout));
                QByteArray rootdirentry;
                rootdirentry.clear();
                QStringList rootdirlayoutlist = rootdirlayout.split(";", Qt::SkipEmptyParts);
                for(int j=0; j < rootdirlayoutlist.count(); j++)
                    rootdirentry.append(curimg->ReadContent(rootdirlayoutlist.at(j).split(",", Qt::SkipEmptyParts).at(0).toLongLong(), rootdirlayoutlist.at(j).split(",", Qt::SkipEmptyParts).at(1).toLongLong()));
                int curoffset = 0;
                while(curoffset < rootdirentry.count())
                {
                    if(((uint8_t)rootdirentry.at(curoffset)) == 0x83)
                        break;
                    curoffset += 32;
                }
                if(curoffset < rootdirentry.count())
                {
                    if(uint8_t(rootdirentry.at(curoffset + 1)) > 0)
                        fsinfo.insert("vollabel", QVariant(QString::fromUtf16(reinterpret_cast<const ushort*>(rootdirentry.mid(curoffset + 2, 2*((uint8_t(rootdirentry.at(curoffset + 1))))).data()))));
                    else
                        fsinfo.insert("vollabel", QVariant(""));
                }
                else
                    fsinfo.insert("vollabel", QVariant(""));
            }
        }
    }
    else if(extsig == 0xef53) // EXT2/3/4
    {
        fsinfo.insert("type", QVariant(6));
        fsinfo.insert("partoffset", QVariant((qulonglong)(512 * partoffset)));
        fsinfo.insert("fsinodecnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1024, 4))));
        fsinfo.insert("fsblockcnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1028, 4))));
        fsinfo.insert("blockgroup0startblk", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1044, 4))));
        fsinfo.insert("blocksize", QVariant(1024 * pow(2, qFromLittleEndian<uint32_t>(partbuf.mid(1048, 4)))));
        //qDebug() << "block size:" << fsinfo.value("blocksize").toUInt();
        fsinfo.insert("fragsize", QVariant(1024 * pow(2, qFromLittleEndian<uint32_t>(partbuf.mid(1052, 4)))));
        fsinfo.insert("blockgroupblockcnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1056, 4))));
        fsinfo.insert("blockgroupfragcnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1060, 4))));
        //qDebug() << "fs blockcount:" << fsinfo.value("fsblockcnt").toUInt();
        //qDebug() << "block group block count:" << fsinfo.value("blockgroupblockcnt").toUInt();
        fsinfo.insert("blockgroupinodecnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1064, 4))));
	fsinfo.insert("mounttime", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1068, 4))));
	fsinfo.insert("writetime", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1072, 4))));
	uint16_t fsstate = qFromLittleEndian<uint16_t>(partbuf.mid(1082, 2));
	if(fsstate == 0x01)
	    fsinfo.insert("state", QVariant("Cleanly unmounted"));
	else
	    fsinfo.insert("state", QVariant("Errors detected"));
	fsinfo.insert("lastcheck", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1088, 4))));
	uint32_t creatoros = qFromLittleEndian<uint32_t>(partbuf.mid(1096, 4));
	if(creatoros == 0x00)
	    fsinfo.insert("creator", QVariant("Linux"));
	else if(creatoros == 0x03)
	    fsinfo.insert("creator", QVariant("FreeBSD"));
        //qDebug() << "block group inode count:" << fsinfo.value("blockgroupinodecnt").toUInt();
        fsinfo.insert("inodesize", QVariant(qFromLittleEndian<uint16_t>(partbuf.mid(1112, 2))));
	uint32_t compatflags = qFromLittleEndian<uint32_t>(partbuf.mid(1116, 4));
	uint32_t incompatflags = qFromLittleEndian<uint32_t>(partbuf.mid(1120, 4));
	uint32_t readonlyflags = qFromLittleEndian<uint32_t>(partbuf.mid(1124, 4));
	QString compatstr = "";
	QString incompatstr = "";
	QString rostr = "";
	if(compatflags & 0x200)
	    compatstr += "Sparse Super Block,";
	if(compatflags & 0x100)
	    compatstr += "Exclude Bitmap,";
	if(compatflags & 0x80)
	    compatstr += "Exclude Inodes,";
	if(compatflags & 0x40)
	    compatstr += "Lazy Block Groups,";
	if(compatflags & 0x20)
	    compatstr += "Indexed Directories,";
	if(compatflags & 0x10)
	    compatstr += "Reserved GDT,";
	if(compatflags & 0x08)
	    compatstr += "Extended Attributes,";
	if(compatflags & 0x04)
	    compatstr += "Journal,";
	if(compatflags & 0x02)
	    compatstr += "Imagic Inodes,";
	if(compatflags & 0x01)
	    compatstr += "Directory preallocation";
	fsinfo.insert("compatstr", QVariant(compatstr));
	if(incompatflags & 0x10000)
	    incompatstr += "Encrypted inodes,";
	if(incompatflags & 0x8000)
	    incompatstr += "Data in inode,";
	if(incompatflags & 0x4000)
	    incompatstr += "Large Directory >2GB or 3-level htree,";
	if(incompatflags & 0x2000)
	    incompatstr += "Metadata checksum seed in superblock,";
	if(incompatflags & 0x1000)
	    incompatstr += "Data in Directory Entry,";
	if(incompatflags & 0x400)
	    incompatstr += "Inodes can store large extended attributes,";
	if(incompatflags & 0x200)
	    incompatstr += "Flexible block groups,";
	if(incompatflags & 0x100)
	    incompatstr += "Multiple mount protection,";
	if(incompatflags & 0x80)
	    incompatstr += "FS size over 2^32 blocks,";
	if(incompatflags & 0x40)
	    incompatstr += "Files use Extents,";
	if(incompatflags & 0x10)
	    incompatstr += "Meta block groups,";
	if(incompatflags & 0x08)
	    incompatstr += "Seperate Journal device,";
	if(incompatflags & 0x04)
	    incompatstr += "FS needs journal recovery,";
	if(incompatflags & 0x02)
	    incompatstr += "Directory entries record file type,";
	if(incompatflags & 0x01)
	    incompatstr += "Compression";
	fsinfo.insert("incompatstr", QVariant(incompatstr));
	if(readonlyflags & 0x2000)
	    rostr += "Tracks project quotas,";
	if(readonlyflags & 0x1000)
	    rostr += "Read only FS,";
	if(readonlyflags & 0x800)
	    rostr += "Replica support,";
	if(readonlyflags & 0x400)
	    rostr += "Metadata Checksumming support,";
	if(readonlyflags & 0x200)
	    rostr += "Bigalloc support,";
	if(readonlyflags & 0x100)
	    rostr += "Quota is handled transactionally with journal,";
	if(readonlyflags & 0x80)
	    rostr += "Snapshot,";
	if(readonlyflags & 0x40)
	    rostr += "Large Inodes exist,";
	if(readonlyflags & 0x20)
	    rostr += "EXT3 32k subdir limit doesn't exist,";
	if(readonlyflags & 0x10)
	    rostr += "Group descriptors have checksums,";
	if(readonlyflags & 0x08)
	    rostr += "Space usage stored in i_blocks in units of fs_blocks, not 512-byte sectors,";
	if(readonlyflags & 0x04)
	    rostr += "Was intented for use with htree directories,";
	if(readonlyflags & 0x02)
	    rostr += "Allow storing files larger than 2GB,";
	if(readonlyflags & 0x01)
	    rostr += "Sparse superblocks";
	fsinfo.insert("readonlystr", QVariant(rostr));
        fsinfo.insert("compatflags", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1116, 4))));
        fsinfo.insert("incompatflags", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1120, 4))));
        fsinfo.insert("readonlyflags", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1124, 4))));
        fsinfo.insert("vollabel", QString::fromStdString(partbuf.mid(1144, 16).toStdString()));
	fsinfo.insert("lastmountedpath", QString::fromStdString(partbuf.mid(1160, 64).toStdString()));
	fsinfo.insert("mkfstime", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1288, 4))));
        //qDebug() << "INODE SIZE ACCORDING TO SUPERBLOCK:" << fsinfo.value("inodesize").toUInt();
        //qDebug() << "compatflags:" << fsinfo.value("compatflags").toUInt() << "incompatflags:" << fsinfo.value("incompatflags").toUInt() << "readonlyflags:" << fsinfo.value("readonlyflags").toUInt();
        if(fsinfo.value("incompatflags").toUInt() & 0x40)
            qDebug() << "fs uses extents.";
        if(((fsinfo.value("compatflags").toUInt() & 0x00000200UL) != 0) || ((fsinfo.value("incompatflags").toUInt() & 0x0001f7c0UL) != 0) || ((fsinfo.value("readonlyflags").toUInt() & 0x00000378UL) != 0))
            fsinfo.insert("typestr", QVariant("EXT4"));
        else if(((fsinfo.value("compatflags").toUInt() & 0x00000004UL) != 0) || ((fsinfo.value("incompatflags").toUInt() & 0x0000000cUL) != 0))
            fsinfo.insert("typestr", QVariant("EXT3"));
        else
            fsinfo.insert("typestr", QVariant("EXT2"));
        uint32_t blockgroupcount = fsinfo.value("fsblockcnt").toUInt() / fsinfo.value("blockgroupblockcnt").toUInt();
        int blockgroupcntrem = fsinfo.value("fsblockcnt").toUInt() % fsinfo.value("blockgroupblockcnt").toUInt();
        if(blockgroupcntrem > 0)
            blockgroupcount++;
        qDebug() << "block group count:" << blockgroupcount;
        if(blockgroupcount == 0)
            blockgroupcount = 1;
        QString inodeaddresstable = "";
        for(uint i=0; i < blockgroupcount; i++)
        {
            //qDebug() << "inode address table offset:" << fsinfo.value("blocksize").toUInt() + i*32 + 8;
            if(fsinfo.value("blocksize").toUInt() == 1024)
                inodeaddresstable += QString::number(qFromLittleEndian<uint32_t>(partbuf.mid(2*fsinfo.value("blocksize").toUInt() + i*32 + 8, 4))) + ",";
            else
                inodeaddresstable += QString::number(qFromLittleEndian<uint32_t>(partbuf.mid(fsinfo.value("blocksize").toUInt() + i*32 + 8, 4))) + ",";
        }
        //qDebug() << "inodeaddresstable:" << inodeaddresstable;
        fsinfo.insert("inodeaddresstable", QVariant(inodeaddresstable));
        //qDebug() << "blocks for group descriptor table:" << (32 * (fsinfo.value("fsblockcnt").toUInt() / fsinfo.value("blockgroupblockcnt").toUInt())) / fsinfo.value("blocksize").toUInt();
        if(fsinfo.value("blockgroupinodecnt").toUInt() > 2)
            fsinfo.insert("rootinodetablestartingaddress", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(2056, 4))));
        else
            fsinfo.insert("rootinodetablestartingaddress", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(2088, 4))));
    }
    else if(apfssig == "NXSB") // APFS Container
    {
        fsinfo.insert("type", QVariant(7));
        fsinfo.insert("typestr", QVariant("APFS"));
        fsinfo.insert("blocksize", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(36, 4))));
        qDebug() << "blocksize:" << fsinfo.value("blocksize").toUInt();
        fsinfo.insert("blockcount", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(40, 8))));
	fsinfo.insert("containeruuid", QVariant(QString::fromStdString(partbuf.mid(72, 16).toHex().toStdString())));
	qDebug() << "container uuid:" << (fsinfo.value("containeruuid").toString().left(8) + "-" + fsinfo.value("containeruuid").toString().mid(8, 4) + "-" + fsinfo.value("containeruuid").toString().mid(12, 4) + "-" + fsinfo.value("containeruuid").toString().mid(16, 4) + "-" + fsinfo.value("containeruuid").toString().right(12));
	fsinfo.insert("nextoid", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(88, 8))));
	qDebug() << "nextoid:" << fsinfo.value("nextoid").toUInt();
	fsinfo.insert("nextxid", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(96, 8))));
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
    }
    else if(hfssig == "H+" || hfssig == "HX") // HFS+/HFSX
    {
        int curoffset = 1024;
        fsinfo.insert("type", QVariant(8));
        if(hfssig == "H+")
            fsinfo.insert("typestr", QVariant("HFS+"));
        else if(hfssig == "HX")
            fsinfo.insert("typestr", QVariant("HFSX"));
        fsinfo.insert("createdate", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 16, 4))));
        fsinfo.insert("modifydate", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 20, 4))));
        fsinfo.insert("backupdate", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 24, 4))));
        fsinfo.insert("filecount", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 32, 4))));
        fsinfo.insert("foldercount", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 36, 4))));
        fsinfo.insert("blocksize", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 40, 4))));
        fsinfo.insert("totalblocks", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 44, 4))));
        qDebug() << "block size:" << fsinfo.value("blocksize").toUInt();
        fsinfo.insert("cataloglogsize", QVariant(qFromBigEndian<qulonglong>(partbuf.mid(curoffset + 272, 8))));
        fsinfo.insert("catalogtotalblocks", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 284, 4))));
        fsinfo.insert("catalogextent0startblk", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 288, 4))));
        fsinfo.insert("catalogextent0blkcnt", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 292, 4))));
        qDebug() << "catalog startblk:" << fsinfo.value("catalogextent0startblk").toUInt();
        qDebug() << "catalog blkcnt:" << fsinfo.value("catalogextent0blkcnt").toUInt();
        // 995344 is where the volume name is stored.... in sector 1944
    }
    else if(xfssig == "XFSB") // XFS
    {
        fsinfo.insert("type", QVariant(9));
        fsinfo.insert("typestr", QVariant("XFS"));
        fsinfo.insert("blocksize", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(4, 4))));
        fsinfo.insert("dblocks", QVariant(qFromBigEndian<qulonglong>(partbuf.mid(8, 8))));
        fsinfo.insert("rootinode", QVariant(qFromBigEndian<qulonglong>(partbuf.mid(56, 8))));
        fsinfo.insert("agblocks", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(84, 4))));
        fsinfo.insert("agcount", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(88, 4))));
        fsinfo.insert("sectorsize", QVariant(qFromBigEndian<uint16_t>(partbuf.mid(102, 2))));
        fsinfo.insert("inodesize", QVariant(qFromBigEndian<uint16_t>(partbuf.mid(104, 2))));
        fsinfo.insert("inodesperblock", QVariant(qFromBigEndian<uint16_t>(partbuf.mid(106, 2))));
        fsinfo.insert("vollabel", QVariant(QString::fromStdString(partbuf.mid(108, 12).toStdString())));
        qDebug() << "vollabel:" << fsinfo.value("vollabel").toString();
        qDebug() << "block size:" << fsinfo.value("blocksize").toUInt() << "root inode:" << fsinfo.value("rootinode").toUInt() << "sectorsize:" << fsinfo.value("sectorsize").toUInt() << "inodesize:" << fsinfo.value("inodesize").toUInt() << "inodesperblock:" << fsinfo.value("inodesperblock").toUInt();
    }
    else if(btrsig == "_BHRfS_M") // BTRFS
    {
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
    }
    else if(bitlcksig == "-FVE-FS-") // BITLOCKER
    {
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
    }
    else if(bfssig == "1SFB") // BFS
    {
        fsinfo.insert("type", QVariant(12));
        fsinfo.insert("typestr", QVariant("BFS"));
        fsinfo.insert("vollabel", QVariant(QString::fromStdString(partbuf.mid(512, 32).toStdString())));
        fsinfo.insert("fsbyteorder", QVariant(qFromLittleEndian<int32_t>(partbuf.mid(548, 4))));
        //qDebug() << "fsbyteorder:" << QString::number(fsinfo.value("fsbyteorder").toInt(), 16);
    }
    else if(f2fssig == 0xf2f52010) // F2FS
    {
	fsinfo.insert("type", QVariant(13));
	fsinfo.insert("typestr", QVariant("F2FS"));
    }
    else if(isosig == "CD001" && udfsig != "BEA01") // ISO9660
    {
	fsinfo.insert("type", QVariant(14));
	fsinfo.insert("typestr", QVariant("ISO9660"));
    }
    else if(isosig == "CD001" && udfsig == "BEA01") // UDF
    {
	fsinfo.insert("type", QVariant(15));
	fsinfo.insert("typestr", QVariant("UDF"));
    }
    else if(hfssig == "BD") // legacy HFS
    {
	fsinfo.insert("type", QVariant(16));
	fsinfo.insert("typestr", QVariant("HFS"));
    }
    else if(refsig == 0x5265465300000000) // ReFS
    {
	fsinfo.insert("type", QVariant(18));
	fsinfo.insert("typestr", QVariant("REFS"));
    }
    // need to implement iso-14, udf-15, hfs-16, zfs-17, refs-18
    fsinfolist->append(fsinfo);
    partbuf.clear();
}

// QtConcurrent::map(QList<DirEntryInfo> direntrylist, ProcessFileInformation);
//ParseFileSystemInformation(QByteArray* initbuffer, int fstype, QList<FileSystemInfo>* fsinfolist)

//void ParseMFT(QString estring, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist)
//void ParseMFT(ForensicImage* curimg, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist)
void ParseMFT(ForImg* curimg, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist)
{
    qint64 curinode = fileinfolist->count();
    QByteArray mftarray;
    mftarray.clear();
    QStringList mftlist = fsinfo->value("mftlayout").toString().split(";", Qt::SkipEmptyParts);
    for(int i=0; i < mftlist.count(); i++)
        mftarray.append(curimg->ReadContent(mftlist.at(i).split(",").at(0).toLongLong(), mftlist.at(i).split(",").at(1).toLongLong()));

    //qDebug() << "mft layout:" << fsinfo->value("mftlayout").toString();
    //qDebug() << "final mftarray count:" << mftarray.count();
    //qulonglong curinode = curicnt;
    // PARSE CURRENT MFT ENTRY RECORD INDEX_ROOT AND INDEX_ALLOCATION TO GET DIRECTORY ENTRIES...
    //QByteArray curmftentrybuf = mftarray.mid(curmftentry * fsinfo->value("mftentrybytes").toUInt(), fsinfo->value("mftentrybytes").toUInt());

    //
    // ** Orphan File
    // * This occurs when the file is deleted and either:
    // * - The parent is no longer a directory
    // * - The sequence number of the parent is no longer correct
    // NOW PARSE THE MFT TO LOOK FOR NON-ALLOCATED ENTRIES
    int mftentrycount = mftarray.count() / fsinfo->value("mftentrybytes").toUInt();
    //qDebug() << "mft entry count:" << mftentrycount;
    for(int i=0; i < mftentrycount; i++)
    {
        QHash<QString, QVariant> fileinfo;
        fileinfo.clear();
        QList<QHash<QString, QVariant>> adsinfolist;
        adsinfolist.clear();
	QByteArray curmftentry = mftarray.mid(i*fsinfo->value("mftentrybytes").toUInt(), fsinfo->value("mftentrybytes").toUInt());
	qulonglong curmftentryoffset = 0;
	qulonglong mftrelativeoffset = i*fsinfo->value("mftentrybytes").toUInt();
	QStringList mftlist = fsinfo->value("mftlayout").toString().split(";", Qt::SkipEmptyParts);
	qulonglong entriespersize = 0;
	for(int j=0; j < mftlist.count(); j++)
	{
	    entriespersize += mftlist.at(j).split(",").at(1).toULongLong() / fsinfo->value("mftentrybytes").toUInt();
	    if(i < entriespersize)
	    {
		curmftentryoffset = mftlist.at(j).split(",").at(0).toULongLong() + mftrelativeoffset;
		break;
	    }
	}
	if(QString::fromStdString(curmftentry.left(4).toStdString()) == "FILE") // a proper mft entry
	{
            uint16_t sequenceid = qFromLittleEndian<uint16_t>(curmftentry.mid(16, 2)); // sequence number for entry
            fileinfo.insert("mftsequenceid", QVariant(qFromLittleEndian<uint16_t>(curmftentry.mid(16, 2)))); // sequence number for entry
            uint16_t firstattroffset = qFromLittleEndian<uint16_t>(curmftentry.mid(20, 2)); // offset to first attribute
            uint16_t attrflags = qFromLittleEndian<uint16_t>(curmftentry.mid(22, 2)); // attribute flags
            uint16_t attrcount = qFromLittleEndian<uint16_t>(curmftentry.mid(40, 2)); // next attr id
            if(attrflags == 0x00 || attrflags == 0x02) // not allocated file or dir
            {
		uint32_t attrlength = 0;
		int curoffset = firstattroffset;
                //for(int j=0; j < attrcount; j++)
                while(curoffset < fsinfo->value("mftentrybytes").toUInt())
                {
                    if(curoffset + 22 > fsinfo->value("mftentrybytes").toUInt())
                        break;
                    uint32_t attrtype = qFromLittleEndian<uint32_t>(curmftentry.mid(curoffset, 4)); // attribute type
                    attrlength = qFromLittleEndian<uint32_t>(curmftentry.mid(curoffset + 4, 4)); // attribute length
                    uint8_t resflag = curmftentry.at(curoffset + 8); // resident/non-resident flag 0/1
                    uint8_t namelength = curmftentry.at(curoffset + 9); // attribute name length
		    uint16_t nameoffset = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 10, 2)); // offset to the attr name
                    QString attrname = "";
		    uint16_t attrdataflags = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 12, 2)); // attrdata flags
		    uint32_t contentlength = qFromLittleEndian<uint32_t>(curmftentry.mid(curoffset + 16, 4)); // attribute content length
		    uint16_t contentoffset = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 20, 2)); // attribute content offset
                    if(attrtype == 0x10) // $STANDARD_INFORMATION - always resident, treenode timestamps
                    {
                        fileinfo.insert("createdate", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 24, 8)))));
                        fileinfo.insert("modifydate", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 32, 8)))));
                        fileinfo.insert("statusdate", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 40, 8)))));
                        fileinfo.insert("accessdate", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 48, 8)))));
                        uint32_t accessflags = qFromLittleEndian<uint32_t>(curmftentry.mid(curoffset + 56, 4));
                        QString attrstr = "";
                        if(attrflags == 0x00) // deleted file
                        {
                            attrstr += "Not Allocated,";
                            if(accessflags & 0x4000) // encrypted
                                fileinfo.insert("itemtype", QVariant(13));
                            else
                                fileinfo.insert("itemtype", QVariant(4));
                            fileinfo.insert("isdeleted", QVariant(1));
                        }
                        else if(attrflags == 0x02) // deleted directory
                        {
                            attrstr += "Not Allocated,";
                            if(accessflags & 0x4000) // encrypted
                                fileinfo.insert("itemtype", QVariant(13));
                            else
                                fileinfo.insert("itemtype", QVariant(2));
                            fileinfo.insert("isdeleted", QVariant(1));
                        }
                        if(accessflags & 0x01) // READ ONLY
                            attrstr += "Read Only,";
                        if(accessflags & 0x02) // Hidden
                            attrstr += "Hidden,";
                        if(accessflags & 0x04) // System
                            attrstr += "System,";
                        if(accessflags & 0x20) // Archive
                            attrstr += "Archive,";
                        if(accessflags & 0x40) // Device
                            attrstr += "Device";
                        if(accessflags & 0x80) // Normal
                            attrstr += "Normal,";
                        if(accessflags & 0x100) // Temporary
                            attrstr += "Temporary,";
                        if(accessflags & 0x200) // Sparse File
                            attrstr += "Sparse File,";
                        if(accessflags & 0x400) // Reparse Point
                            attrstr += "Reparse Point,";
                        if(accessflags & 0x800) // Compresssed
                            attrstr += "Compressed,";
                        if(accessflags & 0x1000) // Offline
                            attrstr += "Offline,";
                        if(accessflags & 0x2000) // Not Indexed
                            attrstr += "Not Indexed,";
                        if(accessflags & 0x4000) // Encrypted
                            attrstr += "Encrypted";
                        fileinfo.insert("attribute", QVariant(attrstr));
                    }
                    else if(attrtype == 0x30) // $FILE_NAME - always resident
                    {
                        uint64_t parmftentry = qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 24, 6)); // parent mft entry
                        uint16_t parsequence = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 30, 2)); // parent sequence id
                        uint8_t filenamespace = curmftentry.at(curoffset + 89);
                        uint8_t filenamelength = curmftentry.at(curoffset + 88);
                        if(filenamespace != 0x02)
                        {
                            QString filename = "";
                            for(int k=0; k < filenamelength; k++)
                                filename += QString(QChar(qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 90 + k*2, 2))));
                            fileinfo.insert("filename", QVariant(filename));
                            fileinfo.insert("ntinode", QVariant(i));
                            fileinfo.insert("parntinode", QVariant((qulonglong)parmftentry));
                            fileinfo.insert("parsequenceid", QVariant(parsequence));
                            fileinfo.insert("filecreate", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 32, 8)))));
                            fileinfo.insert("filemodify", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 40, 8)))));
                            fileinfo.insert("filestatus", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 48, 8)))));
                            fileinfo.insert("fileaccess", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 56, 8)))));
                            //fileinfo.insert("inode", QVariant(curinode));
                            //fileinfo.insert("path", QVariant("/"));
                            //fileinfo.insert("parentinode", QVariant(-1));
                            //qDebug() << "nt mft entry:" << i << "filename:" << filename << "sequence number:" << sequenceid << "parmftentry:" << parmftentry << "parent sequence number;" << parsequence;
                        }
                    }
                    else if(attrtype == 0x80) // $DATA - resident or non-resident
                    {
                        // NEED "LAYOUT"
                        // NEED TO ACCOUNT FOR POSSIBLE ALTERNATE STREAMS
                        if(namelength == 0) // main file content - not alternate data stream
                        {
                            qulonglong logicalsize = 0;
                            qulonglong physicalsize = 0;
                            if(resflag == 0x00) // resident
                            {
                                uint32_t contentsize = qFromLittleEndian<uint32_t>(curmftentry.mid(curoffset + 16, 4));
                                uint16_t contentoffset = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 20, 2));
                                logicalsize = contentsize;
                                physicalsize = contentsize;
                                fileinfo.insert("layout", QVariant(QString(QString::number(curmftentryoffset + curoffset + contentoffset) + "," + QString::number(contentsize) + ";")));
                            }
                            else if(resflag == 0x01) // non-resident
                            {
                                logicalsize = qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 48, 8));
                                uint16_t runlistoff = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 32, 2));
                                uint currunoff = curoffset + runlistoff;
                                int k = 0;
                                QStringList runlist;
                                runlist.clear();
                                while(currunoff < fsinfo->value("mftentrybytes").toUInt())
                                {
                                    if(curmftentry.at(currunoff) > 0)
                                    {
                                        QString runstr = QString("%1").arg(curmftentry.at(currunoff), 8, 2, QChar('0'));
                                        uint runlengthbytes = runstr.right(4).toInt(nullptr, 2);
                                        uint runlengthoffset = runstr.left(4).toInt(nullptr, 2);
                                        if(runlengthbytes == 0 && runlengthoffset == 0)
                                            break;
                                        currunoff++;
                                        uint runlength = 0;
                                        int runoffset = 0;
                                        if(runlengthbytes == 1)
                                            runlength = qFromLittleEndian<uint8_t>(curmftentry.mid(currunoff, runlengthbytes));
                                        else
                                            runlength = qFromLittleEndian<uint>(curmftentry.mid(currunoff, runlengthbytes));
                                        if(runlengthoffset == 1)
                                            runoffset = qFromLittleEndian<int8_t>(curmftentry.mid(currunoff + runlengthbytes, runlengthoffset));
                                        else
                                            runoffset = qFromLittleEndian<int>(curmftentry.mid(currunoff + runlengthbytes, runlengthoffset));
                                        if(k > 0)
                                        {
                                            if(k > 1 && QString::number(runoffset, 16).right(1).toInt() == 1)
                                                runoffset = runoffset - 0xffff - 1;
                                            runoffset = runoffset + runlist.at(k-1).split(",").at(0).toUInt();
                                        }
                                        physicalsize += runlength;
                                        runlist.append(QString::number(runoffset) + "," + QString::number(runlength));
                                        k++;
                                        currunoff += runlengthbytes + runlengthoffset;
                                    }
                                    else
                                        break;
                                }
                                physicalsize = physicalsize * fsinfo->value("bytespercluster").toUInt();
                                QString layout = "";
                                for(int k=0; k < runlist.count(); k++)
                                    layout += QString::number((fsinfo->value("partoffset").toUInt() * 512) + (runlist.at(k).split(",").at(0).toUInt() * fsinfo->value("bytespercluster").toUInt())) + "," + QString::number(runlist.at(k).split(",").at(1).toUInt() * fsinfo->value("bytespercluster").toUInt()) + ";";
                                fileinfo.insert("layout", QVariant(layout));
                            }
                            fileinfo.insert("logicalsize", QVariant(logicalsize));
                            fileinfo.insert("physicalsize", QVariant(physicalsize));
                        }
                        else // alternate data stream
                        {
                            QHash<QString, QVariant> adsinfo;
                            attrname = "";
                            for(int k=0; k < namelength; k++)
                                attrname += QString(QChar(qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + nameoffset + k*2, 2))));
                            adsinfo.clear();
                            // REPEAT ADS HERE AND ADD TO ADSINFO...
                            adsinfo.insert("filename", QVariant(QString("$DATA:" + attrname)));
                            qulonglong logicalsize = 0;
                            qulonglong physicalsize = 0;
                            if(resflag == 0x00) // resident
                            {
                                uint32_t contentsize = qFromLittleEndian<uint32_t>(curmftentry.mid(curoffset + 16, 4));
                                uint16_t contentoffset = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 20, 2));
                                logicalsize = contentsize;
                                physicalsize = contentsize;
                                adsinfo.insert("layout", QVariant(QString(QString::number(curmftentryoffset + curoffset + contentoffset) + "," + QString::number(contentsize) + ";")));
                            }
                            else if(resflag == 0x01) // non-resident
                            {
                                logicalsize = qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 48, 8));
                                uint16_t runlistoff = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 32, 2));
                                uint currunoff = curoffset + runlistoff;
                                int k = 0;
                                QStringList runlist;
                                runlist.clear();
                                while(currunoff < fsinfo->value("mftentrybytes").toUInt())
                                {
                                    if(curmftentry.at(currunoff) > 0)
                                    {
                                        QString runstr = QString("%1").arg(curmftentry.at(currunoff), 8, 2, QChar('0'));
                                        uint runlengthbytes = runstr.right(4).toInt(nullptr, 2);
                                        uint runlengthoffset = runstr.left(4).toInt(nullptr, 2);
                                        if(runlengthbytes == 0 && runlengthoffset == 0)
                                            break;
                                        currunoff++;
                                        uint runlength = 0;
                                        int runoffset = 0;
                                        if(runlengthbytes == 1)
                                            runlength = qFromLittleEndian<uint8_t>(curmftentry.mid(currunoff, runlengthbytes));
                                        else
                                            runlength = qFromLittleEndian<uint>(curmftentry.mid(currunoff, runlengthbytes));
                                        if(runlengthoffset == 1)
                                            runoffset = qFromLittleEndian<int8_t>(curmftentry.mid(currunoff + runlengthbytes, runlengthoffset));
                                        else
                                            runoffset = qFromLittleEndian<int>(curmftentry.mid(currunoff + runlengthbytes, runlengthoffset));
                                        if(k > 0)
                                        {
                                            if(k > 1 && QString::number(runoffset, 16).right(1).toInt() == 1)
                                                runoffset = runoffset - 0xffff - 1;
                                            runoffset = runoffset + runlist.at(k-1).split(",").at(0).toUInt();
                                        }
                                        physicalsize += runlength;
                                        runlist.append(QString::number(runoffset) + "," + QString::number(runlength));
                                        k++;
                                        currunoff += runlengthbytes + runlengthoffset;
                                    }
                                    else
                                        break;
                                }
                                //qDebug() << "runlist:" << runlist;
                                physicalsize = physicalsize * fsinfo->value("bytespercluster").toUInt();
                                QString layout = "";
                                for(int k=0; k < runlist.count(); k++)
                                    layout += QString::number((fsinfo->value("partoffset").toUInt() * 512) + (runlist.at(k).split(",").at(0).toUInt() * fsinfo->value("bytespercluster").toUInt())) + "," + QString::number(runlist.at(k).split(",").at(1).toUInt() * fsinfo->value("bytespercluster").toUInt()) + ";";
                                //qDebug() << "layout:" << layout;
                                adsinfo.insert("layout", QVariant(layout));
                            }
                            adsinfo.insert("logicalsize", QVariant(logicalsize));
                            adsinfo.insert("physicalsize", QVariant(physicalsize));
                            adsinfo.insert("ntinode", QVariant(fileinfo.value("ntinode").toUInt()));
                            adsinfo.insert("parentinode", QVariant(fileinfo.value("inode").toUInt()));
                            adsinfo.insert("parntinode", QVariant(fileinfo.value("ntinode").toUInt()));
                            adsinfo.insert("isdeletd", QVariant(fileinfo.value("isdeleted").toUInt()));
                            adsinfo.insert("itemtype", QVariant(10));
                            adsinfo.insert("path", QVariant(QString(fileinfo.value("path").toString() + fileinfo.value("filename").toString() + "/")));
                            adsinfolist.append(adsinfo);
                        }
                    }
                    else if(attrtype == 0x90) // $INDEX_ROOT - always resident
                    {
                        // NEED "LAYOUT"
                        // NEED TO ACCOUNT FOR POSSIBLE ALTERNATE STREAMS
                        if(attrflags == 0x02 || attrflags == 0x03 || (fileinfo.value("attribute").toString().contains("Hidden") && fileinfo.value("attribute").toString().contains("System"))) // directory
                        {
                            attrname = "";
                            if(namelength > 0) // get $I30 default dir attribute
                            {
                                for(int k=0; k < namelength; k++)
                                    attrname += QString(QChar(qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + nameoffset + k*2, 2))));
                            }
                            if(attrname.startsWith("$I30"))
                            {
                                fileinfo.insert("logicalsize", QVariant(contentlength));
                                fileinfo.insert("physicalsize", QVariant(contentlength));
                                fileinfo.insert("layout", QVariant(QString(QString::number(curmftentryoffset + curoffset + contentoffset) + "," + QString::number(contentlength) + ";")));
                            }
                            else // alternate stream
                            {
                                QHash<QString, QVariant> adsinfo;
                                adsinfo.clear();
                                adsinfo.insert("filename", QVariant(QString("$INDEX_ROOT:" + attrname)));
                                adsinfo.insert("logicalsize", QVariant(contentlength));
                                adsinfo.insert("physicalsize", QVariant(contentlength));
                                adsinfo.insert("layout", QVariant(QString(QString::number(curmftentryoffset + curoffset + contentoffset) + "," + QString::number(contentlength) + ";")));
                                adsinfo.insert("ntinode", QVariant(fileinfo.value("ntinode").toUInt()));
                                adsinfo.insert("parentinode", QVariant(fileinfo.value("inode").toUInt()));
                                adsinfo.insert("parntinode", QVariant(fileinfo.value("ntinode").toUInt()));
                                adsinfo.insert("isdeletd", QVariant(fileinfo.value("isdeleted").toUInt()));
                                adsinfo.insert("itemtype", QVariant(10));
                                adsinfo.insert("path", QVariant(QString(fileinfo.value("path").toString() + fileinfo.value("filename").toString() + "/")));
                                // adsinfo.insert("path", "parntinode", "parentinode", "inode", "ntinode")
                                // REPEAT THE ABOVE HERE AND ADD TO THE 
                                adsinfolist.append(adsinfo);
                            }
                        }
                    }
                    else if(attrtype == 0xa0) // $INDEX_ALLOCATION - always non-resident
                    {
                        // NEED "LAYOUT"
                        // NEED TO ACCOUNT FOR POSSIBLE ALTERNATE STREAMS
                        attrname = "";
                        if(namelength > 0)
                        {
                            for(int k=0; k < namelength; k++)
                                attrname += QString(QChar(qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + nameoffset + k*2, 2))));
                            if(!attrname.startsWith("$I30")) // alternate data stream
                            {
                                qulonglong logicalsize = 0;
                                qulonglong physicalsize = 0;
                                QHash<QString, QVariant> adsinfo;
                                adsinfo.clear();
                                logicalsize = qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 48, 8));
                                uint16_t runlistoff = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 32, 2));
                                uint currunoff = curoffset + runlistoff;
                                int k=0;
                                QStringList runlist;
                                runlist.clear();
                                while(currunoff < fsinfo->value("mftentrybytes").toUInt())
                                {
                                    if(curmftentry.at(currunoff) > 0)
                                    {
                                        QString runstr = QString("%1").arg(curmftentry.at(currunoff), 8, 2, QChar('0'));
                                        uint runlengthbytes = runstr.right(4).toInt(nullptr, 2);
                                        uint runlengthoffset = runstr.left(4).toInt(nullptr, 2);
                                        if(runlengthbytes == 0 && runlengthoffset == 0)
                                            break;
                                        currunoff++;
                                        uint runlength = 0;
                                        int runoffset = 0;
                                        if(runlengthbytes == 1)
                                            runlength = qFromLittleEndian<uint8_t>(curmftentry.mid(currunoff, runlengthbytes));
                                        else
                                            runlength = qFromLittleEndian<uint>(curmftentry.mid(currunoff, runlengthbytes));
                                        if(runlengthoffset == 1)
                                            runoffset = qFromLittleEndian<int8_t>(curmftentry.mid(currunoff + runlengthbytes, runlengthoffset));
                                        else
                                            runoffset = qFromLittleEndian<int>(curmftentry.mid(currunoff + runlengthbytes, runlengthoffset));
                                        if(k > 0)
                                        {
                                            if(k > 1 && QString::number(runoffset, 16).right(1).toInt() == 1)
                                                runoffset = runoffset - 0xffff - 1;
                                            runoffset = runoffset + runlist.at(k-1).split(",").at(0).toUInt();
                                        }
                                        physicalsize += runlength;
                                        runlist.append(QString::number(runoffset) + "," + QString::number(runlength));
                                        k++;
                                        currunoff += runlengthbytes + runlengthoffset;
                                    }
                                    else
                                        break;
                                }
                                physicalsize = physicalsize * fsinfo->value("bytespercluster").toUInt();
                                QString layout = "";
                                for(int k=0; k < runlist.count(); k++)
                                    layout += QString::number((fsinfo->value("partoffset").toUInt() * 512) + (runlist.at(k).split(",").at(0).toUInt() * fsinfo->value("bytespercluster").toUInt())) + "," + QString::number(runlist.at(k).split(",").at(1).toUInt() * fsinfo->value("bytespercluster").toUInt()) + ";";
                                adsinfo.insert("filename", QVariant(QString("$INDEX_ALLOCATION:" + attrname)));
                                adsinfo.insert("layout", QVariant(layout));
                                adsinfo.insert("logicalsize", QVariant(logicalsize));
                                adsinfo.insert("physicalsize", QVariant(physicalsize));
                                adsinfo.insert("ntinode", QVariant(fileinfo.value("ntinode").toUInt()));
                                adsinfo.insert("parentinode", QVariant(fileinfo.value("inode").toUInt()));
                                adsinfo.insert("parntinode", QVariant(fileinfo.value("ntinode").toUInt()));
                                adsinfo.insert("isdeletd", QVariant(fileinfo.value("isdeleted").toUInt()));
                                adsinfo.insert("itemtype", QVariant(10));
                                adsinfo.insert("path", QVariant(QString(fileinfo.value("path").toString() + fileinfo.value("filename").toString() + "/")));
                                adsinfolist.append(adsinfo);
                            }
                        }
                    }
		    else if(attrtype == 0xffffffff)
			break;
		    curoffset += attrlength;
                }
            }
	}
	else if(QString::fromStdString(curmftentry.left(4).toStdString()) == "BAAD") // a proper mft entry with error
	{
	    qDebug() << "a BAAD MFT to try to read... maybe an orphan..";
	}
        if(fileinfo.contains("filename"))
        {
            int hasparent = 0;
            if(fileinfo.value("parntinode").toULongLong() == 5)
            {
                hasparent = 1;
                fileinfo.insert("inode", QVariant(curinode));
                fileinfo.insert("path", QVariant("/"));
                fileinfo.insert("parentinode", QVariant(-1));
                fileinfolist->append(fileinfo);
                curinode++;
                if(adsinfolist.count() > 0)
                {
                    for(int j=0; j < adsinfolist.count(); j++)
                    {
                        QHash<QString, QVariant> curadsinfo = adsinfolist.at(j);
                        curadsinfo.insert("inode", QVariant(curinode));
                        fileinfolist->append(curadsinfo);
                        curinode++;
                    }
                }
            }
            else
            {
                for(int j=0; j < fileinfolist->count(); j++)
                {
                    if(fileinfolist->at(j).value("ntinode").toULongLong() == fileinfo.value("parntinode").toULongLong())
                    {
                        hasparent = 1;
                        int paritemtype = fileinfolist->at(j).value("itemtype").toInt();
                        QString parpath = fileinfolist->at(j).value("path").toString() + fileinfolist->at(j).value("filename").toString() + "/";
                        qulonglong parseqid = fileinfolist->at(j).value("mftsequenceid").toULongLong();
                        qulonglong parentinode = fileinfolist->at(j).value("inode").toULongLong();
                        if((paritemtype == 3 || paritemtype == 2) && parseqid == fileinfo.value("parsequenceid").toULongLong()) // deleted file
                        {
                            fileinfo.insert("inode", QVariant(curinode));
                            fileinfo.insert("path", QVariant(parpath));
                            fileinfo.insert("parentinode", QVariant(parentinode));
                            fileinfolist->append(fileinfo);
                            curinode++;
                            if(adsinfolist.count() > 0)
                            {
                                for(int k=0; k < adsinfolist.count(); k++)
                                {
                                    QHash<QString, QVariant> curadsinfo = adsinfolist.at(k);
                                    curadsinfo.insert("inode", QVariant(curinode));
                                    fileinfolist->append(curadsinfo);
                                    curinode++;
                                }
                            }
                            //qDebug() << "DELETED FILE:" << "fileinfo filename:" << fileinfo.value("filename").toString() << "parent seqid:" << fileinfo.value("parsequenceid").toULongLong();
                        }
                        else // orphan but parent has been repurposed
                        {
                            orphanlist->append(fileinfo);
                            if(adsinfolist.count() > 0)
                            {
                                for(int k=0; k < adsinfolist.count(); k++)
                                {
                                    orphanlist->append(adsinfolist.at(k));
                                }
                            }
                            //qDebug() << "ORPHAN PARENT NOT VALID:" << "fileinfo filename:" << fileinfo.value("filename").toString() << "parent seqid:" << fileinfo.value("parsequenceid").toULongLong();
                        }
                    }
                }
            }
            if(hasparent == 0) // orphan no parent exists anymore
            {
                orphanlist->append(fileinfo);
                if(adsinfolist.count() > 0)
                {
                    for(int k=0; k < adsinfolist.count(); k++)
                    {
                        orphanlist->append(adsinfolist.at(k));
                    }
                }
                //qDebug() << "ORPHAN NO PARENT:" << "fileinfo filename:" << fileinfo.value("filename").toString() << "parent seqid:" << fileinfo.value("parsequenceid").toULongLong();
            }
        }
    }
}

//void GetMftEntryContent(QString estring, qulonglong ntinode, QHash<QString, QVariant>* fileinfo, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* adsinfolist)
//void GetMftEntryContent(ForensicImage* curimg, qulonglong ntinode, QHash<QString, QVariant>* fileinfo, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* adsinfolist)
void GetMftEntryContent(ForImg* curimg, qulonglong ntinode, QHash<QString, QVariant>* fileinfo, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* adsinfolist)
{
    QByteArray mftarray;
    mftarray.clear();
    QStringList mftlist = fsinfo->value("mftlayout").toString().split(";", Qt::SkipEmptyParts);
    for(int i=0; i < mftlist.count(); i++)
        mftarray.append(curimg->ReadContent(mftlist.at(i).split(",").at(0).toLongLong(), mftlist.at(i).split(",").at(1).toLongLong()));

    // GET THE MFT ENTRY BYTE OFFSET RELATIVE TO THE FILE SYSTEM SO I CAN HIGHLIGHT IN HEX
    qulonglong curmftentryoffset = 0;
    qulonglong mftrelativeoffset = ntinode * fsinfo->value("mftentrybytes").toUInt();
    //QStringList mftlist = fsinfo->value("mftlayout").toString().split(";", Qt::SkipEmptyParts);
    qulonglong entriespersize = 0;
    for(int j=0; j < mftlist.count(); j++)
    {
        entriespersize += mftlist.at(j).split(",").at(1).toULongLong() / fsinfo->value("mftentrybytes").toUInt();
        if(ntinode < entriespersize)
        {
            curmftentryoffset = mftlist.at(j).split(",").at(0).toULongLong() + mftrelativeoffset;
            break;
        }
    }

    //int mftentrycount = mftarray.count() / fsinfo->value("mftentrybytes").toUInt();
    QByteArray curmftentry = mftarray.mid(ntinode*fsinfo->value("mftentrybytes").toUInt(), fsinfo->value("mftentrybytes").toUInt());
    mftarray.clear();
    fileinfo->insert("mftrecordlayout", QString(QString::number(curmftentryoffset) + "," + QString::number(fsinfo->value("mftentrybytes").toUInt()) + ";"));
    // NOW I'VE GOT THE CURMFTENTRY FROM NTINODE AND I NEED TO PARSE THE ATTRIBUTE TO GET REST OF THE FILEINFO...
    if(QString::fromStdString(curmftentry.left(4).toStdString()) == "FILE") // a proper mft entry
    {
	fileinfo->insert("mftsequenceid", QVariant(qFromLittleEndian<uint16_t>(curmftentry.mid(16, 2)))); // sequence number for entry
        uint16_t firstattroffset = qFromLittleEndian<uint16_t>(curmftentry.mid(20, 2)); // offset to first attribute
        uint16_t attrflags = qFromLittleEndian<uint16_t>(curmftentry.mid(22, 2)); // attribute flags
        uint16_t attrcount = qFromLittleEndian<uint16_t>(curmftentry.mid(40, 2)); // next attr id
        if(attrcount > 0)
        {
            uint32_t attrlength = 0;
            int curoffset = firstattroffset;
            for(int j=0; j < attrcount; j++)
            {
                if(curoffset + 22 > fsinfo->value("mftentrybytes").toUInt())
                    break;
                uint32_t attrtype = qFromLittleEndian<uint32_t>(curmftentry.mid(curoffset, 4)); // attribute type
                attrlength = qFromLittleEndian<uint32_t>(curmftentry.mid(curoffset + 4, 4)); // attribute length
                uint8_t resflag = curmftentry.at(curoffset + 8); // resident/non-resident flag 0/1
                uint8_t namelength = curmftentry.at(curoffset + 9); // attribute name length
                uint16_t nameoffset = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 10, 2)); // offset to attr name
                QString attrname = "";
                uint16_t attrdataflags = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 12, 2)); // attr data flags
                uint32_t contentlength = qFromLittleEndian<uint32_t>(curmftentry.mid(curoffset + 16, 4)); // attribute content length
                uint16_t contentoffset = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 20, 2)); // attribute content offset
                if(attrtype == 0x10) // $STANDARD_INFORMATION - always resident, treenode timestamps
                {
                    fileinfo->insert("createdate", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 24, 8)))));
                    fileinfo->insert("modifydate", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 32, 8)))));
                    fileinfo->insert("statusdate", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 40, 8)))));
                    fileinfo->insert("accessdate", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 48, 8)))));
		    uint32_t accessflags = qFromLittleEndian<uint32_t>(curmftentry.mid(curoffset + 56, 4));
                    QString attrstr = "";
                    if(attrflags == 0x00) // unallocated file
                    {
			attrstr += "Not Allocated,";
                        //
                        //if(accessflags & 0x4000) // encrypted
                        //    fileinfo->insert("itemtype", QVariant(13));
                        //else
                        //    fileinfo->insert("itemtype", QVariant(4));
                        //fileinfo->insert("isdeleted", QVariant(1));
                        //qDebug() << "deleted file";
                    }
                    else if(attrflags == 0x01) // allocated file
                    {
			attrstr += "Allocated,";
                        //
                        //if(accessflags & 0x4000) // encrypted
                        //    fileinfo->insert("itemtype", QVariant(13));
                        //else
                        //    fileinfo->insert("itemtype", QVariant(5));
                        //fileinfo->insert("isdeleted", QVariant(0));
                        
                        //qDebug() << "allocated file";
                    }
                    else if(attrflags == 0x02) // unallocated directory
                    {
			attrstr += "Not Allocated,";
                        //
                        //if(accessflags & 0x4000) // encrypted
                        //    fileinfo->insert("itemtype", QVariant(13));
                        //else
                        //    fileinfo->insert("itemtype", QVariant(2));
                        //fileinfo->insert("isdeleted", QVariant(1));
                        
                        //qDebug() << "deleted directory";
                    }
                    else if(attrflags == 0x03) // allocated directory
                    {
			attrstr += "Allocated,";
                        //
                        //if(accessflags & 0x4000) // encrypted
                        //    fileinfo->insert("itemtype", QVariant(13));
                        //else
                        //    fileinfo->insert("itemtype", QVariant(3));
                        //fileinfo->insert("isdeleted", QVariant(0));
                        
                        //qDebug() << "allocated directory";
                    }
                    else if(accessflags & 0x02 && accessflags & 0x04)
                    {
                        //
                        //fileinfo->insert("itemtype", QVariant(5));
                        //fileinfo->insert("isdeleted", QVariant(0));
                        
                    }
                    if(accessflags & 0x01) // READ ONLY
                        attrstr += "Read Only,";
                    if(accessflags & 0x02) // Hidden
                        attrstr += "Hidden,";
                    if(accessflags & 0x04) // System
                        attrstr += "System,";
                    if(accessflags & 0x20) // Archive
                        attrstr += "Archive,";
                    if(accessflags & 0x40) // Device
                        attrstr += "Device";
                    if(accessflags & 0x80) // Normal
                        attrstr += "Normal,";
                    if(accessflags & 0x100) // Temporary
                        attrstr += "Temporary,";
                    if(accessflags & 0x200) // Sparse File
                        attrstr += "Sparse File,";
                    if(accessflags & 0x400) // Reparse Point
                        attrstr += "Reparse Point,";
                    if(accessflags & 0x800) // Compresssed
                        attrstr += "Compressed,";
                    if(accessflags & 0x1000) // Offline
                        attrstr += "Offline,";
                    if(accessflags & 0x2000) // Not Indexed
                        attrstr += "Not Indexed,";
                    if(accessflags & 0x4000) // Encrypted
                        attrstr += "Encrypted";
                    fileinfo->insert("attribute", QVariant(attrstr));
                    fileinfo->insert("accessflags", QVariant(accessflags));
                    fileinfo->insert("attrflags", QVariant(attrflags));
                }
                else if(attrtype == 0x30) // $FILE_NAME - always resident
                {
		    uint8_t filenamespace = curmftentry.at(curoffset + 89);
                    //uint8_t filenamelength = curmftentry.at(curoffset + 88);
                    if(filenamespace != 0x02)
                    {
                        //
                        //QString filename = "";
                        //for(int k=0; k < filenamelength; k++)
                        //    filename += QString(QChar(qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 90 + k*2, 2))));
                        
                        //
                        //if(fileinfo->value("parntinode").toUInt() == 5)
                        //{
                        //    fileinfo->insert("path", QVariant("/"));
                        //    fileinfo->insert("parentinode", QVariant(-1));
                        //}
                        
                        uint32_t fnflags = qFromLittleEndian<uint32_t>(curmftentry.mid(curoffset + 80, 4));
                        fileinfo->insert("filecreate", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 32, 8)))));
                        fileinfo->insert("filemodify", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 40, 8)))));
                        fileinfo->insert("filestatus", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 48, 8)))));
                        fileinfo->insert("fileaccess", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 56, 8)))));
                        //if(fnflags & 0x10000000) // Directory
                        //    fileinfo->insert("isdir", QVariant(1));
                    }
                }
                else if(attrtype == 0x80) // $DATA - resident or non-resident
                {
		    if(namelength == 0) // main file content - not alternate data stream
		    {
			qulonglong logicalsize = 0;
			qulonglong physicalsize = 0;
			if(resflag == 0x00) // resident
			{
			    uint32_t contentsize = qFromLittleEndian<uint32_t>(curmftentry.mid(curoffset + 16, 4));
			    uint16_t contentoffset = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 20, 2));
			    logicalsize = contentsize;
			    physicalsize = contentsize;
			    fileinfo->insert("layout", QVariant(QString(QString::number(curmftentryoffset + curoffset + contentoffset) + "," + QString::number(contentsize) + ";")));
			}
			else if(resflag == 0x01) // non-resident 
			{
			    logicalsize = qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 48, 8));
			    uint16_t runlistoff = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 32, 2));
			    uint currunoff = curoffset + runlistoff;
			    int k = 0;
			    QStringList runlist;
			    runlist.clear();
			    while(currunoff < fsinfo->value("mftentrybytes").toUInt())
			    {
				if(curmftentry.at(currunoff) > 0)
				{
				    QString runstr = QString("%1").arg(curmftentry.at(currunoff), 8, 2, QChar('0'));
				    uint runlengthbytes = runstr.right(4).toInt(nullptr, 2);
				    uint runlengthoffset = runstr.left(4).toInt(nullptr, 2);
				    if(runlengthbytes == 0 && runlengthoffset == 0)
					break;
				    currunoff++;
				    uint runlength = 0;
				    int runoffset = 0;
				    if(runlengthbytes == 1)
					runlength = qFromLittleEndian<uint8_t>(curmftentry.mid(currunoff, runlengthbytes));
				    else
					runlength = qFromLittleEndian<uint>(curmftentry.mid(currunoff, runlengthbytes));
				    if(runlengthoffset == 1)
					runoffset = qFromLittleEndian<int8_t>(curmftentry.mid(currunoff + runlengthbytes, runlengthoffset));
				    else
					runoffset = qFromLittleEndian<int>(curmftentry.mid(currunoff + runlengthbytes, runlengthoffset));
				    if(k > 0)
                                    {
                                        if(k > 1 && QString::number(runoffset, 16).right(1).toInt() == 1)
                                            runoffset = runoffset - 0xffff - 1;
					runoffset = runoffset + runlist.at(k-1).split(",").at(0).toUInt();
                                    }
				    physicalsize += runlength;
				    runlist.append(QString::number(runoffset) + "," + QString::number(runlength));
				    k++;
				    currunoff += runlengthbytes + runlengthoffset;
				}
				else
				    break;
			    }
			    physicalsize = physicalsize * fsinfo->value("bytespercluster").toUInt();
			    QString layout = "";
			    for(int k=0; k < runlist.count(); k++)
				layout += QString::number((fsinfo->value("partoffset").toUInt() * 512) + (runlist.at(k).split(",").at(0).toUInt() * fsinfo->value("bytespercluster").toUInt())) + "," + QString::number(runlist.at(k).split(",").at(1).toUInt() * fsinfo->value("bytespercluster").toUInt()) + ";";
			    fileinfo->insert("layout", QVariant(layout));
			}
			fileinfo->insert("logicalsize", QVariant(logicalsize));
			fileinfo->insert("physicalsize", QVariant(physicalsize));
		    }
		    else // alternate data stream
		    {
			QHash<QString, QVariant> adsinfo;
			attrname = "";
			for(int k=0; k < namelength; k++)
			    attrname += QString(QChar(qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + nameoffset + k*2, 2))));
			adsinfo.clear();
			// REPEAT ADS HERE AND ADD TO ADSINFO...
			adsinfo.insert("filename", QVariant(QString("$DATA:" + attrname)));
			qulonglong logicalsize = 0;
			qulonglong physicalsize = 0;
			if(resflag == 0x00) // resident
			{
			    uint32_t contentsize = qFromLittleEndian<uint32_t>(curmftentry.mid(curoffset + 16, 4));
			    uint16_t contentoffset = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 20, 2));
			    logicalsize = contentsize;
			    physicalsize = contentsize;
			    adsinfo.insert("layout", QVariant(QString(QString::number(curmftentryoffset + curoffset + contentoffset) + "," + QString::number(contentsize) + ";")));
			}
			else if(resflag == 0x01) // non-resident
			{
			    logicalsize = qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 48, 8));
			    uint16_t runlistoff = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 32, 2));
			    uint currunoff = curoffset + runlistoff;
			    int k = 0;
			    QStringList runlist;
			    runlist.clear();
			    while(currunoff < fsinfo->value("mftentrybytes").toUInt())
			    {
				if(curmftentry.at(currunoff) > 0)
				{
				    QString runstr = QString("%1").arg(curmftentry.at(currunoff), 8, 2, QChar('0'));
				    uint runlengthbytes = runstr.right(4).toInt(nullptr, 2);
				    uint runlengthoffset = runstr.left(4).toInt(nullptr, 2);
				    if(runlengthbytes == 0 && runlengthoffset == 0)
					break;
				    currunoff++;
				    uint runlength = 0;
				    int runoffset = 0;
				    if(runlengthbytes == 1)
					runlength = qFromLittleEndian<uint8_t>(curmftentry.mid(currunoff, runlengthbytes));
				    else
					runlength = qFromLittleEndian<uint>(curmftentry.mid(currunoff, runlengthbytes));
				    if(runlengthoffset == 1)
					runoffset = qFromLittleEndian<int8_t>(curmftentry.mid(currunoff + runlengthbytes, runlengthoffset));
				    else
					runoffset = qFromLittleEndian<int>(curmftentry.mid(currunoff + runlengthbytes, runlengthoffset));
				    if(k > 0)
                                    {
                                        if(k > 1 && QString::number(runoffset, 16).right(1).toInt() == 1)
                                            runoffset = runoffset - 0xffff - 1;
					runoffset = runoffset + runlist.at(k-1).split(",").at(0).toUInt();
                                    }
				    physicalsize += runlength;
				    runlist.append(QString::number(runoffset) + "," + QString::number(runlength));
				    k++;
				    currunoff += runlengthbytes + runlengthoffset;
				}
				else
				    break;
			    }
			    //qDebug() << "runlist:" << runlist;
			    physicalsize = physicalsize * fsinfo->value("bytespercluster").toUInt();
			    QString layout = "";
			    for(int k=0; k < runlist.count(); k++)
				layout += QString::number((fsinfo->value("partoffset").toUInt() * 512) + (runlist.at(k).split(",").at(0).toUInt() * fsinfo->value("bytespercluster").toUInt())) + "," + QString::number(runlist.at(k).split(",").at(1).toUInt() * fsinfo->value("bytespercluster").toUInt()) + ";";
			    //qDebug() << "layout:" << layout;
			    adsinfo.insert("layout", QVariant(layout));
			}
			adsinfo.insert("logicalsize", QVariant(logicalsize));
			adsinfo.insert("physicalsize", QVariant(physicalsize));
			adsinfo.insert("ntinode", QVariant(fileinfo->value("ntinode").toUInt()));
			adsinfo.insert("parentinode", QVariant(fileinfo->value("inode").toUInt()));
			adsinfo.insert("parntinode", QVariant(fileinfo->value("ntinode").toUInt()));
			adsinfo.insert("isdeletd", QVariant(fileinfo->value("isdeleted").toUInt()));
			adsinfo.insert("itemtype", QVariant(10));
			adsinfo.insert("path", QVariant(QString(fileinfo->value("path").toString() + fileinfo->value("filename").toString() + "/")));
			adsinfolist->append(adsinfo);
		    }
                }
                else if(attrtype == 0x90) // $INDEX_ROOT - always resident
                {
                    if(attrflags == 0x02 || attrflags == 0x03 || (fileinfo->value("attribute").toString().contains("Hidden") && fileinfo->value("attribute").toString().contains("System"))) // directory
                    {
                        attrname = "";
                        if(namelength > 0) // get $I30 default dir attribute
                        {
                            for(int k=0; k < namelength; k++)
                                attrname += QString(QChar(qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + nameoffset + k*2, 2))));
                        }
                        if(attrname.startsWith("$I30"))
                        {
                            fileinfo->insert("logicalsize", QVariant(contentlength));
                            fileinfo->insert("physicalsize", QVariant(contentlength));
                            fileinfo->insert("layout", QVariant(QString(QString::number(curmftentryoffset + curoffset + contentoffset) + "," + QString::number(contentlength) + ";")));
                        }
                        else // alternate stream
                        {
			    QHash<QString, QVariant> adsinfo;
			    adsinfo.clear();
			    adsinfo.insert("filename", QVariant(QString("$INDEX_ROOT:" + attrname)));
			    adsinfo.insert("logicalsize", QVariant(contentlength));
			    adsinfo.insert("physicalsize", QVariant(contentlength));
			    adsinfo.insert("layout", QVariant(QString(QString::number(curmftentryoffset + curoffset + contentoffset) + "," + QString::number(contentlength) + ";")));
			    adsinfo.insert("ntinode", QVariant(fileinfo->value("ntinode").toUInt()));
			    adsinfo.insert("parentinode", QVariant(fileinfo->value("inode").toUInt()));
			    adsinfo.insert("parntinode", QVariant(fileinfo->value("ntinode").toUInt()));
			    adsinfo.insert("isdeletd", QVariant(fileinfo->value("isdeleted").toUInt()));
			    adsinfo.insert("itemtype", QVariant(10));
			    adsinfo.insert("path", QVariant(QString(fileinfo->value("path").toString() + fileinfo->value("filename").toString() + "/")));
			    // adsinfo.insert("path", "parntinode", "parentinode", "inode", "ntinode")
			    // REPEAT THE ABOVE HERE AND ADD TO THE 
			    adsinfolist->append(adsinfo);
                        }
                    }
                }
                else if(attrtype == 0xa0) // INDEX_ALLOCATION - always non-resident
                {
		    attrname = "";
		    if(namelength > 0)
		    {
			for(int k=0; k < namelength; k++)
			    attrname += QString(QChar(qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + nameoffset + k*2, 2))));
			if(!attrname.startsWith("$I30")) // alternate data stream
			{
			    qulonglong logicalsize = 0;
			    qulonglong physicalsize = 0;
			    QHash<QString, QVariant> adsinfo;
			    adsinfo.clear();
			    logicalsize = qFromLittleEndian<uint64_t>(curmftentry.mid(curoffset + 48, 8));
			    uint16_t runlistoff = qFromLittleEndian<uint16_t>(curmftentry.mid(curoffset + 32, 2));
			    uint currunoff = curoffset + runlistoff;
			    int k=0;
			    QStringList runlist;
			    runlist.clear();
			    while(currunoff < fsinfo->value("mftentrybytes").toUInt())
			    {
				if(curmftentry.at(currunoff) > 0)
				{
				    QString runstr = QString("%1").arg(curmftentry.at(currunoff), 8, 2, QChar('0'));
				    uint runlengthbytes = runstr.right(4).toInt(nullptr, 2);
				    uint runlengthoffset = runstr.left(4).toInt(nullptr, 2);
				    if(runlengthbytes == 0 && runlengthoffset == 0)
					break;
				    currunoff++;
				    uint runlength = 0;
				    int runoffset = 0;
				    if(runlengthbytes == 1)
					runlength = qFromLittleEndian<uint8_t>(curmftentry.mid(currunoff, runlengthbytes));
				    else
					runlength = qFromLittleEndian<uint>(curmftentry.mid(currunoff, runlengthbytes));
				    if(runlengthoffset == 1)
					runoffset = qFromLittleEndian<int8_t>(curmftentry.mid(currunoff + runlengthbytes, runlengthoffset));
				    else
					runoffset = qFromLittleEndian<int>(curmftentry.mid(currunoff + runlengthbytes, runlengthoffset));
				    if(k > 0)
                                    {
                                        if(k > 1 && QString::number(runoffset, 16).right(1).toInt() == 1)
                                            runoffset = runoffset - 0xffff - 1;
					runoffset = runoffset + runlist.at(k-1).split(",").at(0).toUInt();
                                    }
				    physicalsize += runlength;
				    runlist.append(QString::number(runoffset) + "," + QString::number(runlength));
				    k++;
				    currunoff += runlengthbytes + runlengthoffset;
				}
				else
				    break;
			    }
			    physicalsize = physicalsize * fsinfo->value("bytespercluster").toUInt();
			    QString layout = "";
			    for(int k=0; k < runlist.count(); k++)
				layout += QString::number((fsinfo->value("partoffset").toUInt() * 512) + (runlist.at(k).split(",").at(0).toUInt() * fsinfo->value("bytespercluster").toUInt())) + "," + QString::number(runlist.at(k).split(",").at(1).toUInt() * fsinfo->value("bytespercluster").toUInt()) + ";";
			    adsinfo.insert("filename", QVariant(QString("$INDEX_ALLOCATION:" + attrname)));
			    adsinfo.insert("layout", QVariant(layout));
			    adsinfo.insert("logicalsize", QVariant(logicalsize));
			    adsinfo.insert("physicalsize", QVariant(physicalsize));
			    adsinfo.insert("ntinode", QVariant(fileinfo->value("ntinode").toUInt()));
			    adsinfo.insert("parentinode", QVariant(fileinfo->value("inode").toUInt()));
			    adsinfo.insert("parntinode", QVariant(fileinfo->value("ntinode").toUInt()));
			    adsinfo.insert("isdeletd", QVariant(fileinfo->value("isdeleted").toUInt()));
			    adsinfo.insert("itemtype", QVariant(10));
			    adsinfo.insert("path", QVariant(QString(fileinfo->value("path").toString() + fileinfo->value("filename").toString() + "/")));
			    adsinfolist->append(adsinfo);
			}
		    }
                }
                else if(attrtype == 0xffffffff)
                    break;
                curoffset += attrlength;
            }
        }
    }
    else if(QString::fromStdString(curmftentry.left(4).toStdString()) == "BAAD") // probably orphan
    {
        qDebug() << "BAAD ENTRY... Do something with it.";
    }
}

//void ParseNtfsDirectory(QString estring, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, QHash<QString, QVariant>* parfileinfo, qulonglong curmftentry, qulonglong curicnt)
//void ParseNtfsDirectory(ForensicImage* curimg, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, QHash<QString, QVariant>* parfileinfo, qulonglong curmftentry, qulonglong curicnt)
void ParseNtfsDirectory(ForImg* curimg, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, QHash<QString, QVariant>* parfileinfo, qulonglong curmftentry, qulonglong curicnt)
{
    QHash<QString, QVariant> fileinfo;
    QByteArray mftarray;
    mftarray.clear();
    QStringList mftlist = fsinfo->value("mftlayout").toString().split(";", Qt::SkipEmptyParts);
    for(int i=0; i < mftlist.count(); i++)
        mftarray.append(curimg->ReadContent(mftlist.at(i).split(",").at(0).toLongLong(), mftlist.at(i).split(",").at(1).toLongLong()));
    qulonglong maxmftentries = mftarray.count() / fsinfo->value("mftentrybytes").toUInt();
    qulonglong curinode = curicnt;
    // PARSE CURRENT MFT ENTRY RECORD INDEX_ROOT AND INDEX_ALLOCATION TO GET DIRECTORY ENTRIES...
    QByteArray curmftentrybuf = mftarray.mid(curmftentry * fsinfo->value("mftentrybytes").toUInt(), fsinfo->value("mftentrybytes").toUInt());
    mftarray.clear();
    int curoffset = 0;
    uint16_t firstattroffset = qFromLittleEndian<uint16_t>(curmftentrybuf.mid(20, 2)); // offset to first attribute
    uint16_t attrcount = qFromLittleEndian<uint16_t>(curmftentrybuf.mid(40, 2)); // next attribute id
    uint32_t attrlength = 0;
    QByteArray indxroot;
    QByteArray indxalloc;
    indxroot.clear();
    indxalloc.clear();
    curoffset = firstattroffset;
    for(int i=0; i < attrcount; i++)
    {
        uint32_t attrtype = qFromLittleEndian<uint32_t>(curmftentrybuf.mid(curoffset, 4)); // attribute type
        uint8_t namelength = qFromLittleEndian<uint8_t>(curmftentrybuf.mid(curoffset + 9, 1)); // length of name
        attrlength = qFromLittleEndian<uint32_t>(curmftentrybuf.mid(curoffset + 4, 4)); // attribute length
        uint16_t nameoffset = qFromLittleEndian<uint16_t>(curmftentrybuf.mid(curoffset + 10, 2)); // offset to the attr name
        QString attrname = "";
        uint16_t attrdataflags = qFromLittleEndian<uint16_t>(curmftentrybuf.mid(curoffset + 12, 2)); // attrdata flags
        if(attrtype == 0x90) // $INDEX_ROOT - ALWAYS RESIDENT
        {
            uint32_t contentlength = qFromLittleEndian<uint32_t>(curmftentrybuf.mid(curoffset + 16, 4)); // attribute content length
            uint16_t contentoffset = qFromLittleEndian<uint16_t>(curmftentrybuf.mid(curoffset + 20, 2)); // attribute content offset
            indxroot = curmftentrybuf.mid(curoffset + contentoffset, contentlength);
        }
        else if(attrtype == 0xa0) // $INDEX_ALLOCATION - ALWAYS NON-RESIDENT
        {
	    uint16_t runlistoff = qFromLittleEndian<uint16_t>(curmftentrybuf.mid(curoffset + 32, 2));
            int j=0;
            QStringList runlist;
            runlist.clear();
	    uint currunoff = curoffset + runlistoff;
            while(currunoff < fsinfo->value("mftentrybytes").toUInt())
            {
                if((uint8_t)curmftentrybuf.at(currunoff) > 0)
                {
                    QString runstr = QString("%1").arg(curmftentrybuf.at(currunoff), 8, 2, QChar('0'));
                    uint runlengthbytes = runstr.right(4).toInt(nullptr, 2);
                    uint runlengthoffset = runstr.left(4).toInt(nullptr, 2);
                    if(runlengthbytes == 0 && runlengthoffset == 0)
                        break;
                    currunoff++;
                    uint runlength = 0;
                    int runoffset = 0;
		    // I EITHER NEED TO CAST THE VALUES WITH EXTENSIVE IF'S SO I CAN GET THE NEGATIVE WHERE NEEDED
		    // OR HAVE A RANGE OF VARIABLES AND THEN USE THE IF'S TO PICK WHICH ONE...
                    if(runlengthbytes == 1)
                        runlength = qFromLittleEndian<uint8_t>(curmftentrybuf.mid(currunoff, runlengthbytes));
                    else
                        runlength = qFromLittleEndian<uint>(curmftentrybuf.mid(currunoff, runlengthbytes));
                    if(runlengthoffset == 1)
                        runoffset = qFromLittleEndian<int8_t>(curmftentrybuf.mid(currunoff + runlengthbytes, runlengthoffset));
                    else
                        runoffset = qFromLittleEndian<int>(curmftentrybuf.mid(currunoff + runlengthbytes, runlengthoffset));
                    if(j > 0)
                    {
                        if(j > 1 && QString::number(runoffset, 16).right(1).toInt() == 1)
                            runoffset = runoffset - 0xffff - 1;
                        runoffset = runoffset + runlist.at(j-1).split(",").at(0).toUInt();
                    }
                    runlist.append(QString::number(runoffset) + "," + QString::number(runlength));
                    j++;
                    currunoff += runlengthbytes + runlengthoffset;
                }
                else
                    break;
            }
            //qDebug() << "runlist:" << runlist;
            for(j=0; j < runlist.count(); j++)
                indxalloc.append(curimg->ReadContent(((fsinfo->value("partoffset").toLongLong()*512) + (runlist.at(j).split(",").at(0).toLongLong()*fsinfo->value("bytespercluster").toLongLong())), (runlist.at(j).split(",").at(1).toLongLong() * fsinfo->value("bytespercluster").toLongLong())));
        }
        else if(attrtype == 0xffffffff)
            break;
        curoffset += attrlength;
    }
    
    uint32_t indxrecordsize = qFromLittleEndian<uint32_t>(indxroot.mid(8, 4)); // INDEX RECORD SIZE (Bytes)
    if(indxalloc.count() > 0)
    {
        // ALSO NEED TO ADD THE INDXROOT ENTRIES HERE FIRST....
        int indxrecordcount = indxalloc.count() / indxrecordsize; // NUMBER OF INDEX RECORDS IN ALLOCATION
        for(int i=0; i < indxrecordcount; i++)
        {
            QByteArray indxrecord = indxalloc.mid(i*indxrecordsize, indxrecordsize);
            if(indxrecord.left(4).startsWith("INDX"))
            {
                uint curpos = 0;
                fileinfo.clear();
                uint32_t startoffset = qFromLittleEndian<uint32_t>(indxrecord.mid(24, 4));
                uint32_t endoffset = qFromLittleEndian<uint32_t>(indxrecord.mid(28, 4));
                uint32_t allocoffset = qFromLittleEndian<uint32_t>(indxrecord.mid(32, 4));
                curpos = 24 + startoffset;
                while(curpos < (uint)indxrecord.count())
                {
                    uint64_t ntinode = 0;
                    ntinode = qFromLittleEndian<uint64_t>(indxrecord.mid(curpos, 6)); // nt inode for index entry
                    uint16_t i30seqid = qFromLittleEndian<uint16_t>(indxrecord.mid(curpos + 6, 2)); // seq number for index entry
                    uint16_t entrylength = qFromLittleEndian<uint16_t>(indxrecord.mid(curpos + 8, 2)); // entry length;
                    uint16_t fnattrlength = qFromLittleEndian<uint16_t>(indxrecord.mid(curpos + 10, 2)); // $FILE_NAME attr length
                    if(ntinode <= maxmftentries && entrylength > 0 && entrylength < indxrecordsize && fnattrlength < entrylength && fnattrlength > 66 && entrylength % 4 == 0)
                    {
                        QByteArray fnattrbuf = indxrecord.mid(curpos + 16, fnattrlength);
                        if(fnattrbuf.count() == fnattrlength)
                        {
                            uint8_t fntype = fnattrbuf.at(65);
                            if(fntype != 0x02)
                            {
                                uint8_t filenamelength = fnattrbuf.at(64);
                                QString filename = "";
                                for(int j=0; j < filenamelength; j++)
                                    filename += QString(QChar(qFromLittleEndian<uint16_t>(fnattrbuf.mid(66 + j*2, 2))));
                                if(filename != ".")
                                {
                                    uint64_t parntinode = qFromLittleEndian<uint64_t>(fnattrbuf.mid(0, 6)); // parent nt inode for the entry
                                    fileinfo.insert("i30parentsequenceid", QVariant(qFromLittleEndian<uint16_t>(fnattrbuf.mid(6, 2)))); // parent seq number for entry
                                    fileinfo.insert("i30sequenceid", QVariant(i30seqid)); // i30 sequence id
                                    fileinfo.insert("ntinode", QVariant((qulonglong)ntinode)); // nt inode
                                    fileinfo.insert("parntinode", QVariant((qulonglong)parntinode)); // parent nt inode
                                    fileinfo.insert("i30create", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(fnattrbuf.mid(8, 8)))));
                                    fileinfo.insert("i30modify", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(fnattrbuf.mid(16, 8)))));
                                    fileinfo.insert("i30change", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(fnattrbuf.mid(24, 8)))));
                                    fileinfo.insert("i30access", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(fnattrbuf.mid(32, 8)))));
                                    QList<QHash<QString, QVariant>> adsinfolist;
                                    adsinfolist.clear();
                                    fileinfo.insert("filename", QVariant(filename));
                                    fileinfo.insert("inode", QVariant(curinode));
                                    GetMftEntryContent(curimg, ntinode, &fileinfo, fsinfo, &adsinfolist);
                                    //GetMftEntryContent(estring, ntinode, &fileinfo, fsinfo, &adsinfolist);
                                    uint16_t attrflags = fileinfo.value("attrflags").toUInt();
                                    uint32_t accessflags = fileinfo.value("accessflags").toUInt();
                                    if(curpos > endoffset)
                                    {
                                        if(attrflags == 0x00) // unalloc file
                                        {
                                            if(accessflags & 0x4000) // encrypted
                                                fileinfo.insert("itemtype", QVariant(13));
                                            else
                                                fileinfo.insert("itemtype", QVariant(4));
                                            fileinfo.insert("isdeleted", QVariant(1));
                                        }
                                        else if(attrflags == 0x02) // unalloc dir
                                        {
                                            if(accessflags & 0x4000) // encrypted
                                                fileinfo.insert("itemtype", QVariant(13));
                                            else
                                                fileinfo.insert("itemtype", QVariant(2));
                                            fileinfo.insert("isdeleted", QVariant(1));
                                        }
                                        else
                                        {
                                            fileinfo.insert("itemtype", QVariant(4));
                                            fileinfo.insert("isdeleted", QVariant(1));
                                        }
                                    }
                                    else if(parfileinfo != NULL)
                                    {
                                        if(parfileinfo->value("ntinode").toUInt() != parntinode)
                                        {
                                            if(attrflags == 0x00) // unalloc file
                                            {
                                                if(accessflags & 0x4000) // encrypted
                                                    fileinfo.insert("itemtype", QVariant(13));
                                                else
                                                    fileinfo.insert("itemtype", QVariant(4));
                                                fileinfo.insert("isdeleted", QVariant(1));
                                            }
                                            else if(attrflags == 0x02) // unalloc dir
                                            {
                                                if(accessflags & 0x4000) // encrypted
                                                    fileinfo.insert("itemtype", QVariant(13));
                                                else
                                                    fileinfo.insert("itemtype", QVariant(2));
                                                fileinfo.insert("isdeleted", QVariant(1));
                                            }
                                            else
                                            {
                                                fileinfo.insert("itemtype", QVariant(4));
                                                fileinfo.insert("isdeleted", QVariant(1));
                                            }
                                        }
                                        else
                                        {
                                            if(attrflags == 0x01) //alloc file
                                            {
                                                if(accessflags & 0x4000) // encrypted
                                                    fileinfo.insert("itemtype", QVariant(13));
                                                else
                                                    fileinfo.insert("itemtype", QVariant(5));
                                                fileinfo.insert("isdeleted", QVariant(0));
                                            }
                                            else if(attrflags == 0x03) // alloc dir
                                            {
                                                if(accessflags & 0x4000) // encrypted
                                                    fileinfo.insert("itemtype", QVariant(13));
                                                else
                                                    fileinfo.insert("itemtype", QVariant(3));
                                                fileinfo.insert("isdeleted", QVariant(0));
                                            }
                                            else
                                            {
                                                fileinfo.insert("itemtype", QVariant(5));
                                                fileinfo.insert("isdeleted", QVariant(0));
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if(attrflags == 0x01) // alloc file
                                        {
                                            if(accessflags & 0x4000) // encrypted
                                                fileinfo.insert("itemtype", QVariant(13));
                                            else
                                                fileinfo.insert("itemtype", QVariant(5));
                                            fileinfo.insert("isdeleted", QVariant(0));
                                        }
                                        else if(attrflags == 0x03) // alloc dir
                                        {
                                            if(accessflags & 0x4000) // encrypted
                                                fileinfo.insert("itemtype", QVariant(13));
                                            else
                                                fileinfo.insert("itemtype", QVariant(3));
                                            fileinfo.insert("isdeleted", QVariant(0));
                                        }
                                        else
                                        {
                                            fileinfo.insert("itemtype", QVariant(5));
                                            fileinfo.insert("isdeleted", QVariant(0));
                                        }
                                    }
                                    if(parfileinfo == NULL)
                                    {
                                        fileinfo.insert("path", QVariant("/"));
                                        fileinfo.insert("parentinode", QVariant(-1));
                                    }
                                    else
                                    {
                                        fileinfo.insert("path", QVariant(QString(parfileinfo->value("path").toString() + parfileinfo->value("filename").toString() + "/")));
                                        fileinfo.insert("parentinode", QVariant(parfileinfo->value("inode").toULongLong()));
                                    }
                                    fileinfolist->append(fileinfo);
                                    curinode++;
                                    if(fileinfo.value("itemtype").toUInt() == 2 || fileinfo.value("itemtype").toUInt() == 3) // directory
                                    {
                                        ParseNtfsDirectory(curimg, fsinfo, fileinfolist, orphanlist, &fileinfo, ntinode, curinode); // should be able to get rid of mftentries...
                                        //ParseNtfsDirectory(estring, fsinfo, fileinfolist, orphanlist, &fileinfo, ntinode, curinode); // should be able to get rid of mftentries...
                                        curinode = fileinfolist->count();
                                    }
                                    fileinfo.clear();
                                    if(adsinfolist.count() > 0)
                                    {
                                        for(int j=0; j < adsinfolist.count(); j++)
                                        {
                                            QHash<QString, QVariant> curadsinfo = adsinfolist.at(j);
                                            curadsinfo.insert("inode", QVariant(curinode));
                                            fileinfolist->append(curadsinfo);
                                            curinode++;
                                        }
                                        adsinfolist.clear();
                                    }
                                }
                            }
                        }
                        curpos = curpos + entrylength;
                    }
                    else
                        curpos = curpos + 4;
                }
            }
        }
        indxalloc.clear();
    }
    else // 0x00 NO INDEX_ALLOC
    {
        uint32_t startoffset = qFromLittleEndian<uint32_t>(indxroot.mid(16, 4));
        uint32_t endoffset = qFromLittleEndian<uint32_t>(indxroot.mid(20, 4));
        uint32_t allocoffset = qFromLittleEndian<uint32_t>(indxroot.mid(24, 4));
        uint curpos = 16 + startoffset;
        while(curpos < allocoffset)
        {
            uint16_t indxentrylength = qFromLittleEndian<uint16_t>(indxroot.mid(curpos + 8, 2));
            uint16_t filenamelength = qFromLittleEndian<uint16_t>(indxroot.mid(curpos + 10, 2));
            uint64_t ntinode = qFromLittleEndian<uint64_t>(indxroot.mid(curpos, 6)); 
	    if(indxentrylength > 0 && filenamelength > 0 && ntinode <= maxmftentries && indxentrylength < indxrecordsize && filenamelength < indxentrylength && filenamelength > 66 && indxentrylength % 4 == 0)
            {
                QByteArray filenamebuf = indxroot.mid(curpos + 16, filenamelength);
                uint8_t fnametype = filenamebuf.at(65);
                if(fnametype != 0x02)
                {
                    uint8_t fnamelength = filenamebuf.at(64);
                    QString filename = "";
                    for(int j=0; j < fnamelength; j++)
                        filename += QString(QChar(qFromLittleEndian<uint16_t>(filenamebuf.mid(66 + j*2, 2))));
                    if(filename != ".")
                    {
	                fileinfo.insert("i30sequenceid", QVariant(qFromLittleEndian<uint16_t>(indxroot.mid(curpos + 6, 2)))); // sequence number for entry
                        uint64_t parntinode = qFromLittleEndian<uint64_t>(filenamebuf.mid(0, 6)); // parent nt inode for the entry
                        fileinfo.insert("i30parentsequenceid", QVariant(qFromLittleEndian<uint16_t>(filenamebuf.mid(6, 2)))); // parent sequence number for entry
                        fileinfo.insert("ntinode", QVariant((qulonglong)ntinode)); // current nt inode
                        fileinfo.insert("parntinode", QVariant((qulonglong)parntinode)); // current parent nt inode
                        fileinfo.insert("i30create", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(8, 8)))));
                        fileinfo.insert("i30modify", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(16, 8)))));
                        fileinfo.insert("i30change", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(24, 8)))));
                        fileinfo.insert("i30access", QVariant(ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(filenamebuf.mid(32, 8)))));
                        QList<QHash<QString, QVariant>> adsinfolist;
                        adsinfolist.clear();
                        fileinfo.insert("filename", QVariant(filename));
                        fileinfo.insert("inode", QVariant(curinode));
                        GetMftEntryContent(curimg, ntinode, &fileinfo, fsinfo, &adsinfolist);
                        //GetMftEntryContent(estring, ntinode, &fileinfo, fsinfo, &adsinfolist);
                        uint16_t attrflags = fileinfo.value("attrflags").toUInt();
                        uint32_t accessflags = fileinfo.value("accessflags").toUInt();
                        if(curpos > endoffset)
                        {
                            if(attrflags == 0x00) // unalloc file
                            {
                                if(accessflags & 0x4000) // encrypted
                                    fileinfo.insert("itemtype", QVariant(13));
                                else
                                    fileinfo.insert("itemtype", QVariant(4));
                                fileinfo.insert("isdeleted", QVariant(1));
                            }
                            else if(attrflags == 0x02) // unalloc dir
                            {
                                if(accessflags & 0x4000) // encrypted
                                    fileinfo.insert("itemtype", QVariant(13));
                                else
                                    fileinfo.insert("itemtype", QVariant(2));
                                fileinfo.insert("isdeleted", QVariant(1));
                            }
                            else
                            {
                                fileinfo.insert("itemtype", QVariant(4));
                                fileinfo.insert("isdeleted", QVariant(1));
                            }
                        }
                        else if(parfileinfo != NULL)
                        {
                            if(parfileinfo->value("ntinode").toUInt() != parntinode)
                            {
                                if(attrflags == 0x00) // unalloc file
                                {
                                    if(accessflags & 0x4000) // encrypted
                                        fileinfo.insert("itemtype", QVariant(13));
                                    else
                                        fileinfo.insert("itemtype", QVariant(4));
                                    fileinfo.insert("isdeleted", QVariant(1));
                                }
                                else if(attrflags == 0x02) // unalloc dir
                                {
                                    if(accessflags & 0x4000) // encrypted
                                        fileinfo.insert("itemtype", QVariant(13));
                                    else
                                        fileinfo.insert("itemtype", QVariant(2));
                                    fileinfo.insert("isdeleted", QVariant(1));
                                }
                                else
                                {
                                    fileinfo.insert("itemtype", QVariant(4));
                                    fileinfo.insert("isdeleted", QVariant(1));
                                }
                            }
                            else
                            {
                                if(attrflags == 0x01) //alloc file
                                {
                                    if(accessflags & 0x4000) // encrypted
                                        fileinfo.insert("itemtype", QVariant(13));
                                    else
                                        fileinfo.insert("itemtype", QVariant(5));
                                    fileinfo.insert("isdeleted", QVariant(0));
                                }
                                else if(attrflags == 0x03) // alloc dir
                                {
                                    if(accessflags & 0x4000) // encrypted
                                        fileinfo.insert("itemtype", QVariant(13));
                                    else
                                        fileinfo.insert("itemtype", QVariant(3));
                                    fileinfo.insert("isdeleted", QVariant(0));
                                }
                                else
                                {
                                    fileinfo.insert("itemtype", QVariant(5));
                                    fileinfo.insert("isdeleted", QVariant(0));
                                }
                            }
                        }
                        else
                        {
                            if(attrflags == 0x01) // alloc file
                            {
                                if(accessflags & 0x4000) // encrypted
                                    fileinfo.insert("itemtype", QVariant(13));
                                else
                                    fileinfo.insert("itemtype", QVariant(5));
                                fileinfo.insert("isdeleted", QVariant(0));
                            }
                            else if(attrflags == 0x03) // alloc dir
                            {
                                if(accessflags & 0x4000) // encrypted
                                    fileinfo.insert("itemtype", QVariant(13));
                                else
                                    fileinfo.insert("itemtype", QVariant(3));
                                fileinfo.insert("isdeleted", QVariant(0));
                            }
                            else
                            {
                                fileinfo.insert("itemtype", QVariant(5));
                                fileinfo.insert("isdeleted", QVariant(0));
                            }
                        }
                        if(parfileinfo == NULL)
                        {
                            fileinfo.insert("path", QVariant("/"));
                            fileinfo.insert("parentinode", QVariant(-1));
                        }
                        else
                        {
                            fileinfo.insert("path", QVariant(QString(parfileinfo->value("path").toString() + parfileinfo->value("filename").toString() + "/")));
                            fileinfo.insert("parentinode", QVariant(parfileinfo->value("inode").toULongLong()));
                        }
                        fileinfolist->append(fileinfo);
                        curinode++;
                        if(fileinfo.value("itemtype").toUInt() == 2 || fileinfo.value("itemtype").toUInt() == 3) // directory
                        {
                            ParseNtfsDirectory(curimg, fsinfo, fileinfolist, orphanlist, &fileinfo, ntinode, curinode); // should be able to get rid of mftentries...
                            //ParseNtfsDirectory(estring, fsinfo, fileinfolist, orphanlist, &fileinfo, ntinode, curinode); // should be able to get rid of mftentries...
                            curinode = fileinfolist->count();
                        }
                        fileinfo.clear();
                        if(adsinfolist.count() > 0)
                        {
                            for(int j=0; j < adsinfolist.count(); j++)
                            {
                                QHash<QString, QVariant> curadsinfo = adsinfolist.at(j);
                                curadsinfo.insert("inode", QVariant(curinode));
                                fileinfolist->append(curadsinfo);
                                curinode++;
                            }
                            adsinfolist.clear();
                        }
                    }
                }
                curpos = curpos + indxentrylength;
            }
            else
                curpos = curpos + 4;
        }
        indxroot.clear();
    }
}

//void ParseExtDirectory(QString estring, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, QHash<QString, QVariant>* parfileinfo, qulonglong curinode, qulonglong curicnt)
void ParseExtDirectory(ForensicImage* curimg, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, QHash<QString, QVariant>* parfileinfo, qulonglong curinode, qulonglong curicnt)
{
    //qDebug() << "current inode:" << curinode;
    // DETERMINE WHICH BLOCK GROUP THE CUR INODE IS A PART OF.
    //qDebug() << "inode address table:" << fsinfo->value("inodeaddresstable").toString();
    QStringList blockgroups = fsinfo->value("inodeaddresstable").toString().split(",", Qt::SkipEmptyParts);
    qulonglong inodetablestartingblock = 0;
    int bgnumber = 0;
    qulonglong inodecnt = curicnt;
    for(int i=1; i <= blockgroups.count(); i++)
    {
        if(curinode < i*fsinfo->value("blockgroupinodecnt").toUInt())
        {
            inodetablestartingblock = blockgroups.at(i-1).toULongLong();
            bgnumber = i - 1;
            break;
        }
    }
    //qDebug() << "blockgroups:" << blockgroups;
    //qDebug() << "inode table starting block:" << inodetablestartingblock;
    QHash<QString, QVariant> fileinfo;
    fileinfo.clear();

    //QFile efile(estring);
    QByteArray direntrybuf;
    uint32_t inodeflags = 0;
    // I NEED TO DO THIS TO GET THE BLOCK LIST FOR THE CURRENT INODE SO I CAN GET IT'S CONTENT AND READ TEH DIRECTORY ENTRY, BUT THIS IS A GIVEN
    // ON SUB DIRECTORIES SINCE I WILL HAVE THE INODE'S LAYOUT AND CURBLOCKLIST, SO I CAN SKIP THIS PART WHEN THESE VALUES ARE KNOWN...
    if(parfileinfo != NULL)
    {
        direntrybuf.clear();
        curimg->open(QIODevice::ReadOnly);
        QStringList layoutlist = parfileinfo->value("layout").toString().split(";", Qt::SkipEmptyParts);
        for(int i=0; i < layoutlist.count(); i++)
        {
            curimg->seek(layoutlist.at(i).split(",").at(0).toUInt());
            direntrybuf.append(curimg->read(layoutlist.at(i).split(",").at(1).toUInt()));
        }
        curimg->close();
    }
    else
    {
        QByteArray inodetablebuf;
        inodetablebuf.clear();
        curimg->open(QIODevice::ReadOnly);
        curimg->seek(fsinfo->value("partoffset").toUInt() + (inodetablestartingblock * fsinfo->value("blocksize").toUInt()));
        inodetablebuf = curimg->read(fsinfo->value("inodesize").toUInt() * fsinfo->value("blockgroupinodecnt").toUInt());
        curimg->close();
        // NOW I HAVE THE INODE TABLE FOR THE CURRENT BLOCK GROUP. I CAN GO THE CURINODE's OFFSET and parse it's inode table entry to get the content for the directory entry...
        QStringList blkstrlist;
        blkstrlist.clear();
        //QList<uint32_t> blocklist;
        //blocklist.clear();
        qulonglong relcurinode = curinode - 1 - (bgnumber * fsinfo->value("blockgroupinodecnt").toUInt());
        inodeflags = qFromLittleEndian<uint32_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 32, 4));
	if((fsinfo->value("incompatflags").toUInt() & 0x40) && (inodeflags & 0x80000)) // FS USES EXTENTS && INODE USES EXTENTS
	{
            uint16_t extententries = qFromLittleEndian<uint16_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 42, 2));
            uint16_t extentdepth = qFromLittleEndian<uint16_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 46, 2));
            if(extentdepth == 0) // use ext4_extent
            {
                for(int i=0; i < extententries; i++)
                {
                    //qDebug() << "ee_block:" << qFromLittleEndian<uint32_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 52 + i*12, 4));
                    //qDebug() << "ee_length:" << qFromLittleEndian<uint16_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 56 + i*12, 2));
                    //qDebug() << "ee_start_hi:" << qFromLittleEndian<uint16_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 58 + i*12, 2));
                    //qDebug() << "ee_start_lo:" << qFromLittleEndian<uint32_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 60 + i*12, 4));
                    uint16_t blocklength = qFromLittleEndian<uint16_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 56 + i*12, 2));
                    uint16_t starthi = qFromLittleEndian<uint16_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 58 + i*12, 2));
                    uint32_t startlo = qFromLittleEndian<uint32_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 60 + i*12, 4));
                    uint64_t startblock = (((uint64_t)starthi >> 32) + startlo) * fsinfo->value("blocksize").toUInt();
                    //qDebug() << "startblock:" << startblock;
                    blkstrlist.append(QString::number(startblock) + "," + QString::number(blocklength * fsinfo->value("blocksize").toUInt()));
                }
            }
            else // use ext4_extent_idx
            {
		QList<uint32_t> leafnodes;
		leafnodes.clear();
		for(int i=0; i < extententries; i++)
		{
		    leafnodes.append(qFromLittleEndian<uint32_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 56 + i*12, 4)));
		}
		for(int i=0; i < leafnodes.count(); i++)
		{
		    QByteArray leafnode;
		    leafnode.clear();
                    curimg->open(QIODevice::ReadOnly);
                    curimg->seek(fsinfo->value("partoffset").toUInt() + (leafnodes.at(i) * fsinfo->value("blocksize").toUInt()));
                    leafnode = curimg->read(fsinfo->value("blockszie").toUInt());
                    curimg->close();
			uint16_t extententries = qFromLittleEndian<uint16_t>(leafnode.mid(2, 2));
			uint16_t extentdepth = qFromLittleEndian<uint16_t>(leafnode.mid(6, 2));
			if(extentdepth == 0) // use ext4_extent
			{
			    for(int j=0; j < extententries; j++)
			    {
				uint16_t blocklength = qFromLittleEndian<uint16_t>(leafnode.mid(16 + j*12, 2));
				uint16_t starthi = qFromLittleEndian<uint16_t>(leafnode.mid(18 + j*12, 2));
				uint32_t startlo = qFromLittleEndian<uint32_t>(leafnode.mid(20 + j*12, 4));
				uint64_t startblock = (((uint64_t)starthi >> 32) + startlo) * fsinfo->value("blocksize").toUInt();
				blkstrlist.append(QString::number(startblock) + "," + QString::number(blocklength * fsinfo->value("blocksize").toUInt()));
			    }
			}
			else // use ext4_extent_idx
			{
			    qDebug() << "repeat leafnode exercise here...";
			}
			//qDebug() << "leaf header:" << QString::number(qFromLittleEndian<uint16_t>(leafnode.mid(0, 2)), 16);
			//qDebug() << "extent entries:" << qFromLittleEndian<uint16_t>(leafnode.mid(2, 2));
			//qDebug() << "max extent entries:" << qFromLittleEndian<uint16_t>(leafnode.mid(4, 2));
			//qDebug() << "extent depth:" << qFromLittleEndian<uint16_t>(leafnode.mid(6, 2));
		    //}
		}
		//qDebug() << "extent header:" << QString::number(qFromLittleEndian<uint16_t>(curinodebuf.mid(40, 2)), 16);
		//qDebug() << "extent entries:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(42, 2));
		//qDebug() << "max extent entries:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(44, 2));
		//qDebug() << "extent depth:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(46, 2));
		
		//qDebug() << "ei_block:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(52, 4));
		//qDebug() << "ei_leaf_lo:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(56, 4));
		//qDebug() << "ei_leaf_hi:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(60, 2));
		//qDebug() << "use extent idx";
            }
	}
        else
        {
	    for(int i=0; i < 12; i++)
	    {
		uint32_t curdirectblock = qFromLittleEndian<uint32_t>(inodetablebuf.mid( fsinfo->value("inodesize").toUInt() * relcurinode + (40 + i*4), 4));
		if(curdirectblock > 0)
		{
		    blkstrlist.append(QString::number(curdirectblock * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
		    //blocklist.append(curdirectblock);
		}
	    }
	    //qDebug() << "current block list before i get the indirect block pointers.." << blocklist;
	    uint32_t singleindirect = qFromLittleEndian<uint32_t>(inodetablebuf.mid((relcurinode)*fsinfo->value("inodesize").toUInt() + 88, 4));
	    uint32_t doubleindirect = qFromLittleEndian<uint32_t>(inodetablebuf.mid((relcurinode)*fsinfo->value("inodesize").toUInt() + 92, 4));
	    uint32_t tripleindirect = qFromLittleEndian<uint32_t>(inodetablebuf.mid((relcurinode)*fsinfo->value("inodesize").toUInt() + 96, 4));
	    // NEED TO PARSE THESE BLOCKS TO ADD TO THE BLOCKLIST's TOTAL
	    if(singleindirect > 0)
	    {
                curimg->open(QIODevice::ReadOnly);
                curimg->seek(fsinfo->value("partoffset").toUInt() + (singleindirect * fsinfo->value("blocksize").toUInt()));
                QByteArray singlebuf = curimg->read(fsinfo->value("blocksize").toUInt());
                curimg->close();
		    for(int i=0; i < singlebuf.count() / 4; i++)
		    {
			uint32_t cursingledirect = qFromLittleEndian<uint32_t>(singlebuf.mid(i*4, 4));
			if(cursingledirect > 0)
			{
			    blkstrlist.append(QString::number(cursingledirect * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
			    //blocklist.append(cursingledirect);
			}
		    }
		//}
	    }
	    if(doubleindirect > 0)
	    {
		QList<uint32_t> sinlist;
		sinlist.clear();
                curimg->open(QIODevice::ReadOnly);
                curimg->seek(fsinfo->value("partoffset").toUInt() + (doubleindirect * fsinfo->value("blocksize").toUInt()));
                QByteArray doublebuf = curimg->read(fsinfo->value("blocksize").toUInt());
                curimg->close();
		    for(int i=0; i < doublebuf.count() / 4; i++)
		    {
			uint32_t sindirect = qFromLittleEndian<uint32_t>(doublebuf.mid(i*4, 4));
			if(sindirect > 0)
			    sinlist.append(sindirect);
		    }
		//}
		for(int i=0; i < sinlist.count(); i++)
		{
                    curimg->open(QIODevice::ReadOnly);
                    curimg->seek(fsinfo->value("partoffset").toUInt() + (sinlist.at(i) * fsinfo->value("blocksize").toUInt()));
                    QByteArray sinbuf = curimg->read(fsinfo->value("blocksize").toUInt());
                    curimg->close();
			for(int j=0; j < sinbuf.count() / 4; j++)
			{
			    uint32_t sdirect = qFromLittleEndian<uint32_t>(sinbuf.mid(j*4, 4));
			    if(sdirect > 0)
			    {
				blkstrlist.append(QString::number(sdirect * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
				//blocklist.append(sdirect);
			    }
			}
		    //}
		}
	    }
	    if(tripleindirect > 0)
	    {
		QList<uint32_t> dinlist;
		dinlist.clear();
		QList<uint32_t> sinlist;
		sinlist.clear();
                curimg->open(QIODevice::ReadOnly);
                curimg->seek(fsinfo->value("partoffset").toUInt() + (tripleindirect * fsinfo->value("blocksize").toUInt()));
                QByteArray triplebuf = curimg->read(fsinfo->value("blocksize").toUInt());
                curimg->close();
		    for(int i=0; i < triplebuf.count() / 4; i++)
		    {
			uint32_t dindirect = qFromLittleEndian<uint32_t>(triplebuf.mid(i*4, 4));
			if(dindirect > 0)
			    dinlist.append(dindirect);
		    }
		//}
		for(int i=0; i < dinlist.count(); i++)
		{
                    curimg->open(QIODevice::ReadOnly);
                    curimg->seek(fsinfo->value("partoffset").toUInt() + (dinlist.at(i) * fsinfo->value("blocksize").toUInt()));
                    QByteArray dinbuf = curimg->read(fsinfo->value("blocksize").toUInt());
                    curimg->close();
			for(int j=0; j < dinbuf.count() / 4; j++)
			{
			    uint32_t sindirect = qFromLittleEndian<uint32_t>(dinbuf.mid(j*4, 4));
			    if(sindirect > 0)
				sinlist.append(sindirect);
			}
		    //}
		    for(int j=0; j < sinlist.count(); j++)
		    {
                        curimg->open(QIODevice::ReadOnly);
                        curimg->seek(fsinfo->value("partoffset").toUInt() + (sinlist.at(j) * fsinfo->value("blocksize").toUInt()));
                        QByteArray sinbuf = curimg->read(fsinfo->value("blocksize").toUInt());
                        curimg->close();
			    for(int k=0; k < sinbuf.count() / 4; k++)
			    {
				uint32_t sdirect = qFromLittleEndian<uint32_t>(sinbuf.mid(k*4, 4));
				if(sdirect > 0)
				{
				    blkstrlist.append(QString::number(sdirect * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
				    //blocklist.append(sdirect);
				}
			    }
			//}
		    }
		}
	    }
        }
        //qDebug() << "block list to compare to parfileinfo layout:" << blocklist;
        //qDebug() << "blkstr list:" << blkstrlist;
        // GET THE DIRECTORY ENTRY CONTENT FOR THE CURRENT INODE
        direntrybuf.clear();
        curimg->open(QIODevice::ReadOnly);
        for(int i=0; i < blkstrlist.count(); i++)
        {
            curimg->seek(fsinfo->value("partoffset").toUInt() + blkstrlist.at(i).split(",").at(0).toUInt());
            direntrybuf.append(curimg->read(blkstrlist.at(i).split(",").at(1).toUInt()));
        }
        curimg->close();
    }
    int curoffset = 24; // SKIP THE . AND .. ENTRIES WHICH ARE ALWAYS THE 1ST TWO ENTRIES AND ARE 12 BYTES LONG EACH
    if(inodeflags & 0x1000)
    {
	curoffset = 40; // THIS SHOULD ACCOUNT FOR HASH TREE DEPTH OF 0, NEED TEST FOR 1 - 3
        qDebug() << "cur directory inode uses hashed btree rather than linear direntry reading";
    }
    bool nextisdeleted = false;
    while(curoffset < direntrybuf.count() - 8)
    {
        fileinfo.clear();
        int entrylength = 0;
        int lengthdiv = (8 + qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 6))) / 4;
        int remdiv = (8 + qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 6))) % 4;
        int newlength = 0;
        if(remdiv == 0)
            newlength = lengthdiv * 4;
        else
            newlength = lengthdiv * 4 + 4;
        // need to make this value "extinode" and the "inode" the standard increment as the other file systems...
        // then i can move the extinode to the properties such as deleted time, etc...
        fileinfo.insert("inode", QVariant(inodecnt));
        fileinfo.insert("extinode", QVariant(qFromLittleEndian<int32_t>(direntrybuf.mid(curoffset, 4))));
        if(fileinfo.value("extinode").toInt() > 0)
        {
	    //qDebug() << "extinode:" << QString::number(fileinfo.value("extinode").toUInt(), 16);
            if(parfileinfo == NULL)
            {
	        fileinfo.insert("path", QVariant("/"));
	        fileinfo.insert("parentinode", QVariant(-1));
            }
            else
            {
                fileinfo.insert("path", QVariant(QString(parfileinfo->value("path").toString() + parfileinfo->value("filename").toString() + QString("/"))));
                fileinfo.insert("parentinode", QVariant(parfileinfo->value("inode").toUInt()));
            }
            int namelength = 0;
            int filetype = -1;
            entrylength = qFromLittleEndian<uint16_t>(direntrybuf.mid(curoffset + 4, 2));
            if(fsinfo->value("incompatflags").toUInt() & 0x0002)
            {
                //qDebug() << "name length where only byte offset 6 is used..";
                namelength = qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 6));
                filetype = qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 7));
            }
            else
            {
                //qDebug() << "name length where byte offset 6 and and 7 are used...";
                namelength = qFromLittleEndian<uint16_t>(direntrybuf.mid(curoffset + 6, 2));
            }
            fileinfo.insert("filename", QVariant(QString::fromStdString(direntrybuf.mid(curoffset + 8, namelength).toStdString())));
            //qDebug() << "filename:" << fileinfo.value("filename").toString();
            // NEED TO USE THE INODE TO THEN GET THE RELEVANT METADATA...
            // FILE TYPE GETS US INFO, SO IF IT'S A DIRECTORY, WE CAN PARSE THE DIRECTORY INODE, WITH THIS RECURSIVE FUNCTION...
            if(nextisdeleted)
                fileinfo.insert("isdeleted", QVariant(1));
            else
                fileinfo.insert("isdeleted", QVariant(0));
            //qDebug() << "newlength:" << newlength << "entrylength:" << entrylength << "namelength:" << namelength;
            //if(newlength < entrylength && entrylength < 264)
            if(newlength < entrylength)
                nextisdeleted = true;
            else
                nextisdeleted = false;
            //qDebug() << "nextisdeleted:" << nextisdeleted;
            //itemtype = itemnode->itemtype; // node type 5=file, 3=dir, 4-del file, 10=vir file, 11=vir dir, -1=not file (evid image, vol, part, fs), 15=carved file
            if(filetype == 0) // unknown type
                fileinfo.insert("itemtype", QVariant(0));
            else if(filetype == 1) // FILE
            {
                if(fileinfo.value("isdeleted").toUInt() == 0)
                    fileinfo.insert("itemtype", QVariant(5));
                else
                    fileinfo.insert("itemtype", QVariant(4));
            }
            else if(filetype == 2) // DIR
            {
                if(fileinfo.value("isdeleted").toUInt() == 0)
                    fileinfo.insert("itemtype", QVariant(3));
                else
                    fileinfo.insert("itemtype", QVariant(2));
            }
            else if(filetype == 3) // character device
                fileinfo.insert("itemtype", QVariant(6));
            else if(filetype == 4) // block device
                fileinfo.insert("itemtype", QVariant(7));
            else if(filetype == 5) // FIFO
                fileinfo.insert("itemtype", QVariant(8));
            else if(filetype == 6) // Unix socket
                fileinfo.insert("itemtype", QVariant(9));
            else if(filetype == 7) // Symbolic Link
                fileinfo.insert("itemtype", QVariant(12));
            // DETERMINE WHICH BLOCK GROUP # THE CURINODE IS IN SO I CAN READ IT'S INODE'S CONTENTS AND GET THE NECCESARY METADATA
	    qulonglong curinodetablestartblock =  0;
	    QByteArray curinodetablebuf;
	    curinodetablebuf.clear();
            int blockgroupnumber = 0;
	    for(int i=1; i <= blockgroups.count(); i++)
	    {
		if(fileinfo.value("extinode").toUInt() < i*fsinfo->value("blockgroupinodecnt").toUInt())
                {
		    curinodetablestartblock = blockgroups.at(i-1).toULongLong();
                    blockgroupnumber = i - 1;
                    break;
                }
	    }
            //qDebug() << "block group number:" << curinodetablestartblock;
            curimg->open(QIODevice::ReadOnly);
            curimg->seek(fsinfo->value("partoffset").toUInt() + (curinodetablestartblock * fsinfo->value("blocksize").toUInt()));
            curinodetablebuf = curimg->read(fsinfo->value("inodesize").toUInt() * fsinfo->value("blockgroupinodecnt").toUInt());
            curimg->close();
            //qDebug() << "curinodetablebuf inode entry number:" << fsinfo->value("inodesize").toUInt() * (fileinfo.value("inode").toUInt() - 1 - (blockgroupnumber * fsinfo->value("blockgroupinodecnt").toUInt()));
	    QByteArray curinodebuf = curinodetablebuf.mid(fsinfo->value("inodesize").toUInt() * (fileinfo.value("extinode").toUInt() - 1 - (blockgroupnumber * fsinfo->value("blockgroupinodecnt").toUInt())), fsinfo->value("inodesize").toUInt());
            uint16_t filemode = qFromLittleEndian<uint16_t>(curinodebuf.mid(0, 2));
            QString filemodestr = "---------";
            if(filemode & 0xc000) // unix socket
                filemodestr.replace(0, 1, "s");
            if(filemode & 0xa000) // symbolic link
                filemodestr.replace(0, 1, "l");
            if(filemode & 0x6000) // block device
                filemodestr.replace(0, 1, "b");
            if(filemode & 0x2000) // char device
                filemodestr.replace(0, 1, "c");
            if(filemode & 0x1000) // FIFO (pipe)
                filemodestr.replace(0, 1, "p");
            if(filemode & 0x8000) // regular file
            {
                if(fsinfo->value("readonlyflags").toUInt() & 0x0002) // LARGE FILE SUPPORT
                {
                    uint32_t lowersize = qFromLittleEndian<uint32_t>(curinodebuf.mid(4, 4));
                    uint32_t uppersize = qFromLittleEndian<uint32_t>(curinodebuf.mid(108, 4));
                    fileinfo.insert("logicalsize", QVariant(((qulonglong)uppersize >> 32) + lowersize));
                }
                else
                {
                    fileinfo.insert("logicalsize", QVariant(qFromLittleEndian<uint32_t>(curinodebuf.mid(4, 4))));
                }
                filemodestr.replace(0, 1, "-");
            }
            else if(filemode & 0x4000) // directory
            {
                fileinfo.insert("logicalsize", QVariant(qFromLittleEndian<uint32_t>(curinodebuf.mid(4, 4))));
                filemodestr.replace(0, 1, "d");
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
            fileinfo.insert("mode", QVariant(filemodestr));

            // STILL NEED TO DO FILE ATTRIBUTES, EXTENDED ATTRIBUTE BLOCK
            uint16_t lowergroupid = qFromLittleEndian<uint16_t>(curinodebuf.mid(24, 2));
            uint16_t uppergroupid = qFromLittleEndian<uint16_t>(curinodebuf.mid(122, 2));
            fileinfo.insert("groupid", QVariant(((uint32_t)uppergroupid >> 16) + lowergroupid));
            uint16_t loweruserid = qFromLittleEndian<uint16_t>(curinodebuf.mid(2, 2));
            uint16_t upperuserid = qFromLittleEndian<uint16_t>(curinodebuf.mid(120, 2));
            fileinfo.insert("userid", QVariant(((uint32_t)upperuserid >> 16) + loweruserid));
            fileinfo.insert("accessdate", qFromLittleEndian<uint32_t>(curinodebuf.mid(8, 4)));
            fileinfo.insert("statusdate", qFromLittleEndian<uint32_t>(curinodebuf.mid(12, 4)));
            fileinfo.insert("modifydate", qFromLittleEndian<uint32_t>(curinodebuf.mid(16, 4)));
            fileinfo.insert("deletedate", qFromLittleEndian<uint32_t>(curinodebuf.mid(20, 4)));
	    fileinfo.insert("linkcount", qFromLittleEndian<uint16_t>(curinodebuf.mid(26, 2)));
	    //qDebug() << "fsinfo type:" << fsinfo->value("typestr").toString();
	    //qDebug() << "create date:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(144, 4));
	    if(fsinfo->value("typestr").toString().startsWith("EXT4"))
		fileinfo.insert("createdate", qFromLittleEndian<uint32_t>(curinodebuf.mid(144, 4)));
            // GET BLOCKLIST FOR THE CURINODEBUF
            QStringList curblkstrlist;
            curblkstrlist.clear();
            QList<uint32_t> curblocklist;
            curblocklist.clear();
            
            uint32_t curinodeflags = qFromLittleEndian<uint32_t>(curinodebuf.mid(32, 4));
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
	    fileinfo.insert("attribute", QVariant(attrstr));

            if((fsinfo->value("incompatflags").toUInt() & 0x40) && (curinodeflags & 0x80000)) // FS USES EXTENTS && INODE USES EXTENTS
            {
                uint16_t extententries = qFromLittleEndian<uint16_t>(curinodebuf.mid(42, 2));
                uint16_t extentdepth = qFromLittleEndian<uint16_t>(curinodebuf.mid(46, 2));
                if(extentdepth == 0) // use ext4_extent
                {
                    for(int i=0; i < extententries; i++)
                    {
			//qDebug() << "logical block:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(52 + i*12, 4));
                        uint16_t blocklength = qFromLittleEndian<uint16_t>(curinodebuf.mid(56 + i*12, 2));
                        uint16_t starthi = qFromLittleEndian<uint16_t>(curinodebuf.mid(58 + i*12, 2));
                        uint32_t startlo = qFromLittleEndian<uint32_t>(curinodebuf.mid(60 + i*12, 4));
                        uint64_t startblock = (((uint64_t)starthi >> 32) + startlo) * fsinfo->value("blocksize").toUInt();
                        curblkstrlist.append(QString::number(startblock) + "," + QString::number(blocklength * fsinfo->value("blocksize").toUInt()));
                    }
                }
                else // use ext4_entent_idx
                {
                    QList<uint32_t> leafnodes;
                    leafnodes.clear();
                    // NEED TO FIGURE OUT HOW TO HANDLE EXTENT DEPTH IN THE LOOP...
                    for(int i=0; i < extententries; i++)
                    {
			//qDebug() << "logical block:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(52 + i*12, 4));
                        leafnodes.append(qFromLittleEndian<uint32_t>(curinodebuf.mid(56 + i*12, 4)));
                        //qDebug() << "ei_leaf_lo: leaf node" << qFromLittleEndian<uint32_t>(curinodebuf.mid(56 + i*12, 4));
                    }
                    //qDebug() << "leafnodes:" << leafnodes;
                    for(int i=0; i < leafnodes.count(); i++)
                    {
                        QByteArray leafnode;
                        leafnode.clear();
                        curimg->open(QIODevice::ReadOnly);
                        curimg->seek(fsinfo->value("partoffset").toUInt() + (leafnodes.at(i) * fsinfo->value("blocksize").toUInt()));
                        leafnode = curimg->read(fsinfo->value("blocksize").toUInt());
                        curimg->close();
                            uint16_t extententries = qFromLittleEndian<uint16_t>(leafnode.mid(2, 2));
                            uint16_t extentdepth = qFromLittleEndian<uint16_t>(leafnode.mid(6, 2));
                            if(extentdepth == 0) // use ext4_extent
                            {
                                for(int j=0; j < extententries; j++)
                                {
				    //qDebug() << "logical block:" << qFromLittleEndian<uint32_t>(leafnode.mid(12 + j*12, 4));
                                    uint16_t blocklength = qFromLittleEndian<uint16_t>(leafnode.mid(16 + j*12, 2));
                                    uint16_t starthi = qFromLittleEndian<uint16_t>(leafnode.mid(18 + j*12, 2));
                                    uint32_t startlo = qFromLittleEndian<uint32_t>(leafnode.mid(20 + j*12, 4));
                                    uint64_t startblock = (((uint64_t)starthi >> 32) + startlo) * fsinfo->value("blocksize").toUInt();
                                    curblkstrlist.append(QString::number(startblock) + "," + QString::number(blocklength * fsinfo->value("blocksize").toUInt()));
                                }
                            }
                            else // use ext4_extent_idx
                            {
                                qDebug() << "repeat leafnode exercise here...";
                            }
                            //qDebug() << "leaf header:" << QString::number(qFromLittleEndian<uint16_t>(leafnode.mid(0, 2)), 16);
                            //qDebug() << "extent entries:" << qFromLittleEndian<uint16_t>(leafnode.mid(2, 2));
                            //qDebug() << "max extent entries:" << qFromLittleEndian<uint16_t>(leafnode.mid(4, 2));
                            //qDebug() << "extent depth:" << qFromLittleEndian<uint16_t>(leafnode.mid(6, 2));
                        //}
                    }
                    //qDebug() << "extent header:" << QString::number(qFromLittleEndian<uint16_t>(curinodebuf.mid(40, 2)), 16);
                    //qDebug() << "extent entries:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(42, 2));
                    //qDebug() << "max extent entries:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(44, 2));
                    //qDebug() << "extent depth:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(46, 2));
                    
                    //qDebug() << "ei_block:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(52, 4));
                    //qDebug() << "ei_leaf_lo:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(56, 4));
                    //qDebug() << "ei_leaf_hi:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(60, 2));
                    //qDebug() << "use extent idx";
                }
            }
            else
            {
                for(int i=0; i < 12; i++)
                {
                    uint32_t curdirectblock = qFromLittleEndian<uint32_t>(curinodebuf.mid(40 + i*4, 4));
                    if(curdirectblock > 0)
                    {
                        curblocklist.append(curdirectblock);
                        curblkstrlist.append(QString::number(curdirectblock * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
                    }
                }
                //qDebug() << fileinfo.value("filename").toString() << "current block list before i get the indirect block pointers.." << curblocklist;
                uint32_t cursingleindirect = qFromLittleEndian<uint32_t>(curinodebuf.mid(88, 4));
                uint32_t curdoubleindirect = qFromLittleEndian<uint32_t>(curinodebuf.mid(92, 4));
                uint32_t curtripleindirect = qFromLittleEndian<uint32_t>(curinodebuf.mid(96, 4));
                if(cursingleindirect > 0)
                {
                    curimg->open(QIODevice::ReadOnly);
                    curimg->seek(fsinfo->value("partoffset").toUInt() + (cursingleindirect * fsinfo->value("blocksize").toUInt()));
                    QByteArray singlebuf = curimg->read(fsinfo->value("blocksize").toUInt());
                    curimg->close();
                        for(int i=0; i < singlebuf.count() / 4; i++)
                        {
                            uint32_t cursingledirect = qFromLittleEndian<uint32_t>(singlebuf.mid(i*4, 4));
                            if(cursingledirect > 0)
                            {
                                curblocklist.append(cursingledirect);
                                curblkstrlist.append(QString::number(cursingledirect * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
                            }
                        }
                    //}
                }
                if(curdoubleindirect > 0)
                {
                    QList<uint32_t> sinlist;
                    sinlist.clear();
                    curimg->open(QIODevice::ReadOnly);
                    curimg->seek(fsinfo->value("partoffset").toUInt() + (curdoubleindirect * fsinfo->value("blocksize").toUInt()));
                    QByteArray doublebuf = curimg->read(fsinfo->value("blocksize").toUInt());
                    curimg->close();
                        for(int i=0; i < doublebuf.count() / 4; i++)
                        {
                            uint32_t sindirect = qFromLittleEndian<uint32_t>(doublebuf.mid(i*4, 4));
                            if(sindirect > 0)
                                sinlist.append(sindirect);
                        }
                    //}
                    for(int i=0; i < sinlist.count(); i++)
                    {
                        curimg->open(QIODevice::ReadOnly);
                        curimg->seek(fsinfo->value("partoffset").toUInt() + (sinlist.at(i) * fsinfo->value("blocksize").toUInt()));
                        QByteArray sinbuf = curimg->read(fsinfo->value("blocksize").toUInt());
                        curimg->close();
                            for(int j=0; j < sinbuf.count() / 4; j++)
                            {
                                uint32_t sdirect = qFromLittleEndian<uint32_t>(sinbuf.mid(j*4, 4));
                                if(sdirect > 0)
                                {
                                    curblkstrlist.append(QString::number(sdirect * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
                                    curblocklist.append(sdirect);
                                }
                            }
                        //}
                    }
                }
                if(curtripleindirect > 0)
                {
                    QList<uint32_t> dinlist;
                    dinlist.clear();
                    QList<uint32_t> sinlist;
                    sinlist.clear();
                    curimg->open(QIODevice::ReadOnly);
                    curimg->seek(fsinfo->value("partoffset").toUInt() + (curtripleindirect * fsinfo->value("blocksize").toUInt()));
                    QByteArray triplebuf = curimg->read(fsinfo->value("blocksize").toUInt());
                    curimg->close();
                        for(int i=0; i < triplebuf.count() / 4; i++)
                        {
                            uint32_t dindirect = qFromLittleEndian<uint32_t>(triplebuf.mid(i*4, 4));
                            if(dindirect > 0)
                                dinlist.append(dindirect);
                        }
                    //}
                    for(int i=0; i < dinlist.count(); i++)
                    {
                        curimg->open(QIODevice::ReadOnly);
                        curimg->seek(fsinfo->value("partoffset").toUInt() + (dinlist.at(i) * fsinfo->value("blocksize").toUInt()));
                        QByteArray dinbuf = curimg->read(fsinfo->value("blocksize").toUInt());
                        curimg->close();
                            for(int j=0; j < dinbuf.count() / 4; j++)
                            {
                                uint32_t sindirect = qFromLittleEndian<uint32_t>(dinbuf.mid(j*4, 4));
                                if(sindirect > 0)
                                    sinlist.append(sindirect);
                            }
                        //}
                        for(int j=0; j < sinlist.count(); j++)
                        {
                            curimg->open(QIODevice::ReadOnly);
                            curimg->seek(fsinfo->value("partoffset").toUInt() + (sinlist.at(j) * fsinfo->value("blocksize").toUInt()));
                            QByteArray sinbuf = curimg->read(fsinfo->value("blocksize").toUInt());
                            curimg->close();
                                for(int k=0; k < sinbuf.count() / 4; k++)
                                {
                                    uint32_t sdirect = qFromLittleEndian<uint32_t>(sinbuf.mid(k*4, 4));
                                    if(sdirect > 0)
                                    {
                                        curblocklist.append(sdirect);
                                        curblkstrlist.append(QString::number(sdirect * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
                                    }
                                }
                            //}
                        }
                    }
                }
            }
            //qDebug() << "curblkstr:" << curblkstrlist;

            // the physical size is wrong for extent's since it's sum of the length's / blocksize -> that # + 1 (if remainder) * blocksize...
	    qulonglong physize = 0;
	    for(int i=0; i < curblkstrlist.count(); i++)
	    {
		physize += curblkstrlist.at(i).split(",").at(1).toUInt();
	    }
	    int phyblkcnt = physize / fsinfo->value("blocksize").toUInt();
	    int phyremcnt = physize % fsinfo->value("blocksize").toUInt();
	    if(phyremcnt > 0)
		phyblkcnt++;
	    //qDebug() << "physize:" << physize << "phyblkcnt:" << phyblkcnt * fsinfo->value("blocksize").toUInt();
            fileinfo.insert("physicalsize", QVariant(phyblkcnt * fsinfo->value("blocksize").toUInt()));

            //qDebug() << "curblock list:" << curblocklist;
            //fileinfo.insert("physicalsize", QVariant(curblocklist.count() * fsinfo->value("blocksize").toUInt()));
            //qDebug() << "logical/physical size:" << fileinfo.value("logicalsize").toUInt() << fileinfo.value("physicalsize").toUInt();
            //qDebug() << "curblocklist:" << curblocklist;
            QString layout = "";
            if(curblocklist.count() > 0)
                layout = ConvertBlocksToExtents(curblocklist, fsinfo->value("blocksize").toUInt());
            fileinfo.insert("layout", QVariant(layout));
            //qDebug() << fileinfo.value("filename").toString() << "layout:" << layout;

            fileinfolist->append(fileinfo);
            inodecnt++;
            if(filemode & 0x4000) // directory so recurse it's value...
            {
                //qDebug() << "inodecnt before sub dir expansion:" << inodecnt;
                ParseExtDirectory(curimg, fsinfo, fileinfolist, orphanlist, &fileinfo, fileinfo.value("extinode").toULongLong(), inodecnt); // initial attempt to recurse...
                //ParseExtDirectory(estring, fsinfo, fileinfolist, orphanlist, &fileinfo, fileinfo.value("extinode").toULongLong(), inodecnt); // initial attempt to recurse...
                inodecnt = fileinfolist->count();
                //qDebug() << "fileinfolist count after sub dir expansion:" << fileinfolist->count();
            }

        }
        curoffset += newlength;
    }
}

//void ParseExFatDirEntry(QString estring, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist)
void ParseExFatDirEntry(ForensicImage* curimg, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist)
{
    QString clustersize = QString::number(fsinfo->value("sectorspercluster").toUInt() * fsinfo->value("bytespersector").toUInt());
    QString clusterstr = "";
    QHash<QString, QVariant> fileinfo;
    fileinfo.clear();
    QByteArray fatbuf;
    fatbuf.clear();
    QByteArray rootdirbuf;
    rootdirbuf.clear();
    int rootdirentrycount = 0;
    QStringList rootdirlayoutlist = fsinfo->value("rootdirlayout").toString().split(";", Qt::SkipEmptyParts);
    curimg->open(QIODevice::ReadOnly);
    for(int i=0; i < rootdirlayoutlist.count(); i++)
    {
        curimg->seek(rootdirlayoutlist.at(i).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
        rootdirbuf.append(curimg->read(rootdirlayoutlist.at(i).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
    }
    curimg->seek(fsinfo->value("fatoffset").toUInt());
    fatbuf = curimg->read(fsinfo->value("fatsize").toUInt() * fsinfo->value("bytespersector").toUInt());
    curimg->close();
    rootdirentrycount = rootdirbuf.count() / 32;
    uint inodecnt = 0;
    for(int i=0; i < rootdirentrycount; i++)
    {
        fileinfo.clear();
        QList<uint> clusterlist;
        clusterlist.clear();
        QString layout = "";
	int fatchain = 0;
	uint8_t entrytype = rootdirbuf.at(i*32);
        fileinfo.insert("entrytype", QVariant(entrytype));
	if(entrytype == 0x00) // cur dir entry is free and all remaining are free
	    break;
	else if(entrytype == 0x81) // ALLOC_BITMAP file
	{
	    fileinfo.insert("filename", QVariant("$ALLOC_BITMAP"));
	    fileinfo.insert("clusternum", QVariant(qFromLittleEndian<uint32_t>(rootdirbuf.mid(i*32 + 20, 4))));
	    fileinfo.insert("logicalsize", QVariant((qulonglong)qFromLittleEndian<uint64_t>(rootdirbuf.mid(i*32 + 24, 8))));
	    fileinfo.insert("inode", QVariant(inodecnt));
	    fileinfo.insert("isdeleted", QVariant(0));
	    fileinfo.insert("path", QVariant("/"));
	    fileinfo.insert("parentinode", QVariant(-1));
            fileinfo.insert("itemtype", QVariant(10));
	}
	else if(entrytype == 0x82) // $UPCASE_TABLE file
	{
            fileinfo.insert("filename", QVariant("$UPCASE_TABLE"));
            fileinfo.insert("clusternum", QVariant(qFromLittleEndian<uint32_t>(rootdirbuf.mid(i*32 + 20, 4))));
            fileinfo.insert("logicalsize", QVariant(qFromLittleEndian<qulonglong>(rootdirbuf.mid(i*32 + 24, 8))));
            fileinfo.insert("inode", QVariant(inodecnt));
            fileinfo.insert("isdeleted", QVariant(0));
            fileinfo.insert("path", QVariant("/"));
            fileinfo.insert("parentinode", QVariant(-1));
            fileinfo.insert("itemtype", QVariant(10));
	}
	else if(entrytype == 0x83) // VOLUME_LABEL (already handles so skip...
	{
	}
	else if(entrytype == 0x03) // Deleted VOLUME_LABEL
	{
            // skip for now...
	}
	else if(entrytype == 0x85 || entrytype == 0x05) // File/Dir Directory Entry
	{
	    // where i will sub loop for the c0,c1(s) entries
	    //qDebug() << "Entry Type:" << QString("0x" + QString::number(entrytype, 16));
            //qDebug() << "secondary count:" << fileinfo.value("secondarycount").toUInt();
            fileinfo.insert("secondarycount", QVariant(rootdirbuf.at(i*32 + 1)));
	    // FILE ATTRIBUTES
	    uint8_t fileattr = rootdirbuf.at(i*32 + 4);
	    QString attrstr = "";
	    if(fileattr & 0x01)
		attrstr += "Read Only,";
	    else if(fileattr & 0x02)
		attrstr += "Hidden File,";
	    else if(fileattr & 0x04)
		attrstr += "System File,";
	    else if(fileattr & 0x10)
		attrstr += "Sub Directory,";
	    else if(fileattr & 0x20)
		attrstr += "Archive File,";
	    fileinfo.insert("attribute", QVariant(attrstr));
	    fileinfo.insert("fileattr", QVariant(fileattr));
            if(entrytype == 0x85)
            {
                if(fileattr & 0x10)
                    fileinfo.insert("itemtype", QVariant(3));
                else
                    fileinfo.insert("itemtype", QVariant(5));
            }
            else if(entrytype == 0x05)
            {
                if(fileattr & 0x10)
                    fileinfo.insert("itemtype", QVariant(2));
                else
                    fileinfo.insert("itemtype", QVariant(4));
            }
            fileinfo.insert("createdate", QVariant(ConvertExfatTimeToUnixTime(rootdirbuf.at(i*32 + 9), rootdirbuf.at(i*32 + 8), rootdirbuf.at(i*32 + 11), rootdirbuf.at(i*32 + 10), rootdirbuf.at(i*32 + 22))));
            fileinfo.insert("modifydate", QVariant(ConvertExfatTimeToUnixTime(rootdirbuf.at(i*32 + 13), rootdirbuf.at(i*32 + 12), rootdirbuf.at(i*32 + 15), rootdirbuf.at(i*32 + 14), rootdirbuf.at(i*32 + 23))));
            fileinfo.insert("accessdate", QVariant(ConvertExfatTimeToUnixTime(rootdirbuf.at(i*32 + 17), rootdirbuf.at(i*32 + 16), rootdirbuf.at(i*32 + 19), rootdirbuf.at(i*32 + 18), rootdirbuf.at(i*32 + 24))));
	    //qDebug() << "create date:" << QDateTime::fromSecsSinceEpoch(fileinfo.value("createdate").toInt(), QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP");
            
            // PROCESS THE REMANING 0X85 STRUCTURE HERE THEN LOOP OVER SECONDARY VALUES.
            // SECONDARY COUNT DETERMINES HOW MANY STREAM/FILENAME DIR ENTRIES FOR MY SUB LOOP...
            //qDebug() << "starting i:" << i;
	    QString filename = "";
	    uint8_t namelength = 0;
	    uint8_t curlength = 0;
            for(int j=1; j <= fileinfo.value("secondarycount").toInt(); j++)
            {
                uint8_t subentrytype = rootdirbuf.at(i*32 + j*32);
                if(subentrytype == 0xc0 || subentrytype == 0x40) // Stream Extension Directory Entry
                {
		    namelength = rootdirbuf.at((i+j)*32 + 3);
		    QString flagstr = QString("%1").arg(rootdirbuf.at((i+j)*32 + 1), 8, 2, QChar('0'));
		    //int allocpossible = flagstr.right(1).toInt(nullptr, 2);
		    fatchain = flagstr.mid(7, 1).toInt(nullptr, 2);
		    fileinfo.insert("logicalsize", QVariant(qFromLittleEndian<qulonglong>(rootdirbuf.mid((i+j)*32 + 8, 8))));
		    fileinfo.insert("clusternum", QVariant(qFromLittleEndian<uint32_t>(rootdirbuf.mid((i+j)*32 + 20, 4))));
		    fileinfo.insert("physicalsize", QVariant(qFromLittleEndian<qulonglong>(rootdirbuf.mid((i+j)*32 + 24, 8))));
                }
                else if(subentrytype == 0xc1 || subentrytype == 0x41) // File Name Directory Entry
                {
		    curlength += 15;
		    if(curlength <= namelength)
		    {
			for(int k=1; k < 16; k++)
			    filename += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid((i+j)*32 + k*2, 2))));
		    }
		    else
		    {
			int remaining = namelength + 16 - curlength;
			//qDebug() << "remaining + 1:" << remaining;
			for(int k=1; k < remaining; k++)
			    filename += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid((i+j)*32 + k*2, 2))));
		    }
		    //qDebug() << j << "namelength:" << namelength;
                }
                //qDebug() << "entry type:" << QString("0x" + QString::number(subentrytype, 16));
            }
	    //qDebug() << "filename:" << QString("'" + filename + "'") << "curlength:" << curlength;
	    fileinfo.insert("filename", QVariant(filename));
            if(entrytype == 0x85)
	        fileinfo.insert("isdeleted", QVariant(0));
            else if(entrytype == 0x05)
                fileinfo.insert("isdeleted", QVariant(1));
	    fileinfo.insert("path", QVariant("/"));
	    fileinfo.insert("parentinode", QVariant(-1));
	    fileinfo.insert("inode", QVariant(inodecnt));
	    //i = i + fileinfo.value("secondarycount").toInt();
            //qDebug() << "ending i:" << i;
            //qDebug() << "i+j ending i:" << i + fileinfo.value("secondarycount").toInt();
	}
	else if(entrytype == 0x05) // Deleted File/Dir Directory Entry
	{
	    // where i will sub loop for the 40,41 entries
	}
	else if(entrytype == 0xa0) // Volume GUID Dir Entry
	{
            // skip for now
	}

	//qDebug() << "Entry Type:" << QString("0x" + QString::number(entrytype, 16));
        if(fatchain == 0 && fileinfo.value("clusternum").toUInt() > 1)
	{
            GetNextCluster(fileinfo.value("clusternum").toUInt(), fsinfo->value("type").toUInt(), &fatbuf, &clusterlist);
	    clusterstr = QString::number(fileinfo.value("clusternum").toUInt()) + ",";
	    layout = QString::number(fsinfo->value("clusteroffset").toULongLong() + (fileinfo.value("clusternum").toUInt() - 2) * fsinfo->value("sectorspercluster").toUInt() * fsinfo->value("bytespersector").toUInt()) + "," + clustersize + ";";
	    for(int j=0; j < clusterlist.count() - 1; j++)
	    {
		clusterstr += QString::number(clusterlist.at(j)) + ",";
		layout += QString::number(fsinfo->value("clusteroffset").toULongLong() + (clusterlist.at(j) - 2) * clustersize.toUInt()) + "," + clustersize + ";";
	    }
	}
	else if(fatchain == 1)
	{
	    // int myValue = (int) ceil( (float)myIntNumber / myOtherInt );
	    int clustercount = (int)ceil((float)fileinfo.value("physicalsize").toULongLong() / clustersize.toUInt());
	    //qDebug() << "logical size:" << fileinfo.value("logicalsize").toULongLong() << "physical size:" << fileinfo.value("physicalsize").toULongLong() << "clustercount:" << clustercount;
	    clusterstr = QString::number(fileinfo.value("clusternum").toUInt()) + ",";
	    layout = QString::number(fsinfo->value("clusteroffset").toULongLong() + (fileinfo.value("clusternum").toUInt() - 2) * clustersize.toUInt()) + "," + QString::number(clustercount * clustersize.toUInt()) + ";";
	    //qDebug() << "layout:" << layout;
	}
        fileinfo.insert("clusterlist", QVariant(clusterstr));
        fileinfo.insert("layout", QVariant(layout));
        // ALSO NEED ITEMTYPE
        if(entrytype == 0x85 || entrytype == 0x05 || entrytype == 0x81 || entrytype == 0x82)
        {
            //qDebug() << "clusterstr:" << clusterstr << "clusterlist:" << clusterlist;
            //qDebug() << "file name:" << fileinfo.value("filename").toString() << "inode:" << fileinfo.value("inode").toUInt() << "Layout:" << fileinfo.value("layout").toString();
            fileinfolist->append(fileinfo);
            inodecnt++;
	    if(fileinfo.value("fileattr").toUInt() & 0x10) // Sub Directory
	    {
		//qDebug() << "Sub directory, parse here...";
                ParseExfatSubDir(curimg, fsinfo, &fileinfo, fileinfolist, &inodecnt, &fatbuf);
                //ParseExfatSubDir(estring, fsinfo, &fileinfo, fileinfolist, &inodecnt, &fatbuf);
	    }
        }
    }
    // EXFAT ORPHAN'S ARE SEARCHING THE WHOLE FILESYSTEM SPACE FOR 0X05 FOLLOWED BY 0X40 AND 0X41'S....
    // SO I WOULD NEED TO LOOP OVER THE PARTITION AND LOOK FOR 0x05's WHICH ARE NOT ALREADY COVERED...

    // USE FATBUF, AND THEN LOOP OVER THE FILEINFOLIST.COUNT(); THEN I CAN COLLECT ALL THE LAYOUT'S OR CLUSTERSTR'S FOR EACH FILE TO KNOW WHERE I CAN LOOK THAT ISN'T AN EXISTING DIRECTORY
    // THEN I CAN SEARCH THROUGH THE RAW IMAGE FOR ALL 0x05's, WHICH SHOULD SHORTEN WHAT I NEED...., I COULD JUST DO FILEINFO'S WHERE FILEATTR == 0x10
    //qDebug() << "Start Initial Orphan Run";
    QString olayout = fsinfo->value("rootdirlayout").toString();
    for(int i=0; i < fileinfolist->count(); i++)
    {
        if(fileinfolist->at(i).value("fileattr").toUInt() & 0x10) // is a directory
        {
            olayout += fileinfolist->at(i).value("layout").toString();
            //qDebug() << "dir:" << i << fileinfolist->at(i).value("clusterlist").toString() << fileinfolist->at(i).value("layout").toString();
        }
    }
    //qDebug() << "olayout:" << olayout;
    QStringList olist = olayout.split(";", Qt::SkipEmptyParts);
    //if(!efile.isOpen())
    //    efile.open(QIODevice::ReadOnly);
    //if(efile.isOpen())
    //{
        int coffset = 0;
        //while(!efile.atEnd())
        curimg->open(QIODevice::ReadOnly);
        //while(!curimg->atEnd())
        while(coffset <= curimg->size())
        {
            QHash<QString, QVariant> orphaninfo;
            orphaninfo.clear();
            //efile.seek(coffset);
            curimg->seek(coffset);
            QByteArray tmparray = curimg->read(32);
            //QByteArray tmparray = efile.read(32);
            if(tmparray.at(0) == 0x05 && (tmparray.at(1) >= 0 && tmparray.at(1) < 256))
            {
                uint itemtype = 4;
                //qDebug() << "secondary count:" << QString::number(tmparray.at(1));
                QString attrstr = "";
                if(tmparray.at(4) & 0x01)
                    attrstr += "Read Only,";
                else if(tmparray.at(4) & 0x02)
                    attrstr += "Hidden File,";
                else if(tmparray.at(4) & 0x04)
                    attrstr += "System File,";
                else if(tmparray.at(4) & 0x10)
                {
                    attrstr += "Sub Directory,";
                    itemtype = 2;
                }
                else if(tmparray.at(4) & 0x20)
                    attrstr += "Archive File,";
                orphaninfo.insert("secondarycount", QVariant(tmparray.at(1)));
                orphaninfo.insert("attribute", QVariant(attrstr));
                orphaninfo.insert("fileattr", QVariant(tmparray.at(4)));
                orphaninfo.insert("itemtype", QVariant(itemtype));
                orphaninfo.insert("createdate", QVariant(ConvertExfatTimeToUnixTime(tmparray.at(9), tmparray.at(8), tmparray.at(11), tmparray.at(10), tmparray.at(22))));
                orphaninfo.insert("modifydate", QVariant(ConvertExfatTimeToUnixTime(tmparray.at(13), tmparray.at(12), tmparray.at(15), tmparray.at(14), tmparray.at(23))));
                orphaninfo.insert("accessdate", QVariant(ConvertExfatTimeToUnixTime(tmparray.at(17), tmparray.at(16), tmparray.at(19), tmparray.at(18), tmparray.at(24))));
                //qDebug() << "Create Date:" << ConvertExfatTimeToUnixTime(tmparray.at(9), tmparray.at(8), tmparray.at(11), tmparray.at(10), tmparray.at(22));
                //qDebug() << "Modify Date:" << ConvertExfatTimeToUnixTime(tmparray.at(13), tmparray.at(12), tmparray.at(15), tmparray.at(14), tmparray.at(23));
                //qDebug() << "Access Date:" << ConvertExfatTimeToUnixTime(tmparray.at(17), tmparray.at(16), tmparray.at(19), tmparray.at(18), tmparray.at(24));
	        //qDebug() << "create date:" << QDateTime::fromSecsSinceEpoch(fileinfo.value("createdate").toInt(), QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP");
                QString filename = "";
                uint8_t namelength = 0;
                uint8_t curlength = 0;
                curimg->seek(coffset + 32);
                QByteArray tmp40array = curimg->read(32);
                //efile.seek(coffset + 32);
                //QByteArray tmp40array = efile.read(32);
                if(tmp40array.at(0) == 0x40)
                {
                    namelength = tmp40array.at(3);
                    QString flagstr = QString("%1").arg(tmp40array.at(1), 8, 2, QChar('0'));
                    //int allocpossible = flagstr.right(1).toInt(nullptr, 2);
                    int fatchain = flagstr.mid(7, 1).toInt(nullptr, 2);
                    //qDebug() << "allocpossible:" << allocpossible;
                    //qDebug() << "fatchain:" << fatchain;
                    orphaninfo.insert("clusternum", QVariant(qFromLittleEndian<uint32_t>(tmp40array.mid(20, 4))));
                    orphaninfo.insert("logicalsize", QVariant(qFromLittleEndian<qulonglong>(tmp40array.mid(8, 8))));
                    orphaninfo.insert("physicalsize", QVariant(qFromLittleEndian<qulonglong>(tmp40array.mid(24, 8))));
                    //qDebug() << "1st cluster:" << qFromLittleEndian<uint32_t>(tmp40array.mid(20, 4)) << "cluster count:" << fsinfo->value("clustercount").toUInt();
                    //qDebug() << "logical size:" << qFromLittleEndian<qulonglong>(tmp40array.mid(8, 8));
                    //qDebug() << "physical size:" << qFromLittleEndian<qulonglong>(tmp40array.mid(24, 8));
                    if(namelength > 0)
                    {
                        curimg->seek(coffset + 64);
                        QByteArray tmp41array = curimg->read((tmparray.at(1)-1) * 32);
                        //efile.seek(coffset + 64);
                        //QByteArray tmp41array = efile.read((tmparray.at(1)-1) * 32);
                        for(int j=0; j < tmparray.at(1) - 1; j++)
                        {
                            if(tmp41array.at(j*32) == 0x41)
                            {
                                curlength += 15;
                                if(curlength <= namelength)
                                {
                                    for(int k=1; k < 16; k++)
                                        filename += QString(QChar(qFromLittleEndian<uint16_t>(tmp41array.mid(j*32 + k*2, 2))));
                                }
                                else
                                {
                                    int remaining = namelength + 16 - curlength;
                                    for(int k=1; k < remaining; k++)
                                        filename += QString(QChar(qFromLittleEndian<uint16_t>(tmp41array.mid(j*32 + k*2, 2))));
                                }
                            }
                        }
                        orphaninfo.insert("filename", QVariant(filename));
                        orphaninfo.insert("isdeleted", QVariant(1));
                        //orphaninfo.insert("path", QVariant("/"));
                        //orphaninfo.insert("parentinode", QVariant(-1));
                        //orphaninfo.insert("inode", QVariant(inodecnt));
                        QList<uint> clusterlist;
                        QString layout = "";
                        clusterlist.clear();
                        if(fatchain == 0 && orphaninfo.value("clusternum").toUInt() > 1)
                        {
                            GetNextCluster(orphaninfo.value("clusternum").toUInt(), fsinfo->value("type").toUInt(), &fatbuf, &clusterlist);
                            clusterstr = QString::number(orphaninfo.value("clusternum").toUInt()) + ",";
                            layout = QString::number(fsinfo->value("clusteroffset").toULongLong() + (orphaninfo.value("clusternum").toUInt() - 2) * clustersize.toUInt()) + "," + clustersize + ";";
                            for(int m=0; m < clusterlist.count() - 1; m++)
                            {
                                clusterstr += QString::number(clusterlist.at(m)) + ",";
                                layout = QString::number(fsinfo->value("clusteroffset").toULongLong() + (clusterlist.at(m) - 2) * clustersize.toUInt()) + "," + clustersize + ";";
                            }
                        }
                        else if(fatchain == 1)
                        {
                            int clustercount = (int)ceil((float)orphaninfo.value("physicalsize").toULongLong() / clustersize.toUInt());
                            clusterstr = QString::number(orphaninfo.value("clusternum").toUInt()) + ",";
                            layout = QString::number(fsinfo->value("clusteroffset").toULongLong() + (orphaninfo.value("clusternum").toUInt() - 2) * clustersize.toUInt()) + "," + QString::number(clustercount * clustersize.toUInt()) + ";";
                        }
                        orphaninfo.insert("clusterlist", QVariant(clusterstr));
                        orphaninfo.insert("layout", QVariant(layout));

                        //qDebug() << "filename:" << filename;

                        orphanlist->append(orphaninfo);
                    }
                }
            }
            coffset += 32;
            //qDebug() << "coffset before olist fix:" << coffset;
            for(int i=0; i < olist.count(); i++)
            {
                if(coffset == olist.at(i).split(",").at(0).toLongLong())
                {
                    //qDebug() << "coffset before olist fix:" << coffset;
                    coffset += olist.at(i).split(",").at(1).toLongLong();
                    //qDebug() << "coffset after olist fix:" << coffset;
                }
            }
            //qDebug() << "coffset after olist fix:" << coffset;
        }
        curimg->close();
        //efile.close();
    //}
}

//void ParseExfatSubDir(QString estring, QHash<QString, QVariant>* fsinfo, QHash<QString, QVariant>* parfileinfo, QList<QHash<QString, QVariant>>* fileinfolist, uint* inodecnt, QByteArray* fatbuf)
void ParseExfatSubDir(ForensicImage* curimg, QHash<QString, QVariant>* fsinfo, QHash<QString, QVariant>* parfileinfo, QList<QHash<QString, QVariant>>* fileinfolist, uint* inodecnt, QByteArray* fatbuf)
{
    QString clustersize = QString::number(fsinfo->value("sectorspercluster").toUInt() * fsinfo->value("bytespersector").toUInt());
    QString clusterstr = "";
    uint curinode = *inodecnt;
    QHash<QString, QVariant> fileinfo;
    fileinfo.clear();
    QByteArray dirbuf;
    dirbuf.clear();
    QStringList layoutlist = parfileinfo->value("layout").toString().split(";", Qt::SkipEmptyParts);
    curimg->open(QIODevice::ReadOnly);
    for(int i=0; i < layoutlist.count(); i++)
    {
        curimg->seek(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
        dirbuf.append(curimg->read(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
    }
    curimg->close();
    //qDebug() << "dir fat content:" << dirbuf.mid(0, 40).toHex();
    int direntrycount = dirbuf.count() / 32;
    for(int i=0; i < direntrycount; i++)
    {
        fileinfo.clear();
        QList<uint> clusterlist;
        clusterlist.clear();
        QString layout = "";
        int fatchain = 0;
        uint8_t entrytype = dirbuf.at(i*32);
        fileinfo.insert("entrytype", QVariant(entrytype));
        if(entrytype == 0x00) // cur dir entry is free and all remaning are free
            break;
        else if(entrytype == 0x85 || entrytype == 0x05) // File/Dir Directory Entry || Deleted File/Dir Directory Entry
        {
            fileinfo.insert("secondarycount", QVariant(dirbuf.at(i*32 + 1)));
            uint8_t fileattr = dirbuf.at(i*32 + 4);
            QString attrstr = "";
	    if(fileattr & 0x01)
		attrstr += "Read Only,";
	    else if(fileattr & 0x02)
		attrstr += "Hidden File,";
	    else if(fileattr & 0x04)
		attrstr += "System File,";
	    else if(fileattr & 0x10)
		attrstr += "Sub Directory,";
	    else if(fileattr & 0x20)
		attrstr += "Archive File,";
	    fileinfo.insert("attribute", QVariant(attrstr));
	    fileinfo.insert("fileattr", QVariant(fileattr));
            if(entrytype == 0x85)
            {
                if(fileattr & 0x10)
                    fileinfo.insert("itemtype", QVariant(3));
                else
                    fileinfo.insert("itemtype", QVariant(5));
            }
            else if(entrytype == 0x05)
            {
                if(fileattr & 0x10)
                    fileinfo.insert("itemtype", QVariant(2));
                else
                    fileinfo.insert("itemtype", QVariant(4));
            }
            fileinfo.insert("createdate", QVariant(ConvertExfatTimeToUnixTime(dirbuf.at(i*32 + 9), dirbuf.at(i*32 + 8), dirbuf.at(i*32 + 11), dirbuf.at(i*32 + 10), dirbuf.at(i*32 + 22))));
            fileinfo.insert("modifydate", QVariant(ConvertExfatTimeToUnixTime(dirbuf.at(i*32 + 13), dirbuf.at(i*32 + 12), dirbuf.at(i*32 + 15), dirbuf.at(i*32 + 14), dirbuf.at(i*32 + 23))));
            fileinfo.insert("accessdate", QVariant(ConvertExfatTimeToUnixTime(dirbuf.at(i*32 + 17), dirbuf.at(i*32 + 16), dirbuf.at(i*32 + 19), dirbuf.at(i*32 + 18), dirbuf.at(i*32 + 24))));
	    QString filename = "";
	    uint8_t namelength = 0;
	    uint8_t curlength = 0;
            for(int j=1; j <= fileinfo.value("secondarycount").toInt(); j++)
            {
                uint8_t subentrytype = dirbuf.at(i*32 + j*32);
                if(subentrytype == 0xc0 || subentrytype == 0x40) // Stream Extension Directory Entry
                {
		    namelength = dirbuf.at((i+j)*32 + 3);
		    QString flagstr = QString("%1").arg(dirbuf.at((i+j)*32 + 1), 8, 2, QChar('0'));
		    //int allocpossible = flagstr.right(1).toInt(nullptr, 2);
		    fatchain = flagstr.mid(7, 1).toInt(nullptr, 2);
		    fileinfo.insert("logicalsize", QVariant(qFromLittleEndian<qulonglong>(dirbuf.mid((i+j)*32 + 8, 8))));
		    fileinfo.insert("clusternum", QVariant(qFromLittleEndian<uint32_t>(dirbuf.mid((i+j)*32 + 20, 4))));
		    fileinfo.insert("physicalsize", QVariant(qFromLittleEndian<qulonglong>(dirbuf.mid((i+j)*32 + 24, 8))));
                }
                else if(subentrytype == 0xc1 || subentrytype == 0x41) // File Name Directory Entry
                {
		    curlength += 15;
		    if(curlength <= namelength)
		    {
			for(int k=1; k < 16; k++)
			    filename += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid((i+j)*32 + k*2, 2))));
		    }
		    else
		    {
			int remaining = namelength + 16 - curlength;
			//qDebug() << "remaining + 1:" << remaining;
			for(int k=1; k < remaining; k++)
			    filename += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid((i+j)*32 + k*2, 2))));
		    }
                }
            }
	    fileinfo.insert("filename", QVariant(filename));
            if(entrytype == 0x85)
	        fileinfo.insert("isdeleted", QVariant(0));
            else if(entrytype == 0x05)
                fileinfo.insert("isdeleted", QVariant(1));
            fileinfo.insert("path", QVariant(QString(parfileinfo->value("path").toString().toUtf8() + parfileinfo->value("filename").toString().toUtf8() + QString("/"))));
            fileinfo.insert("parentinode", QVariant(parfileinfo->value("inode").toInt()));
	    fileinfo.insert("inode", QVariant(curinode));
        }
        if(fatchain == 0 && fileinfo.value("clusternum").toUInt() > 1)
        {
            GetNextCluster(fileinfo.value("clusternum").toUInt(), fsinfo->value("type").toUInt(), fatbuf, &clusterlist);
	    clusterstr = QString::number(fileinfo.value("clusternum").toUInt()) + ",";
	    layout = QString::number(fsinfo->value("clusteroffset").toULongLong() + (fileinfo.value("clusternum").toUInt() - 2) * fsinfo->value("sectorspercluster").toUInt() * fsinfo->value("bytespersector").toUInt()) + "," + clustersize + ";";
	    for(int j=0; j < clusterlist.count() - 1; j++)
	    {
		clusterstr += QString::number(clusterlist.at(j)) + ",";
		layout += QString::number(fsinfo->value("clusteroffset").toULongLong() + (clusterlist.at(j) - 2) * clustersize.toUInt()) + "," + clustersize + ";";
	    }
        }
        else if(fatchain == 1)
        {
	    int clustercount = (int)ceil((float)fileinfo.value("physicalsize").toULongLong() / clustersize.toUInt());
	    clusterstr = QString::number(fileinfo.value("clusternum").toUInt()) + ",";
	    layout = QString::number(fsinfo->value("clusteroffset").toULongLong() + (fileinfo.value("clusternum").toUInt() - 2) * clustersize.toUInt()) + "," + QString::number(clustercount * clustersize.toUInt()) + ";";
        }
        fileinfo.insert("clusterlist", QVariant(clusterstr));
        fileinfo.insert("layout", QVariant(layout));
        if(entrytype == 0x85 || entrytype == 0x05)
        {
            fileinfolist->append(fileinfo);
            curinode++;
            *inodecnt = curinode;
            if(fileinfo.value("fileattr").toUInt() & 0x10 && fileinfo.value("physicalsize").toUInt() > 0) // Sub Directory
            {
                //qDebug() << "Sub directory, parse here...";
                ParseExfatSubDir(curimg, fsinfo, &fileinfo, fileinfolist, inodecnt, fatbuf);
                //ParseExfatSubDir(estring, fsinfo, &fileinfo, fileinfolist, inodecnt, fatbuf);
                curinode = *inodecnt;
            }
        }
    }
}

//void ParseDirectory(QString estring, QHash<QString, QVariant> *fsinfo, QList<QHash<QString, QVariant>> *fileinfolist, QList<QString>* orphanlist)
//void ParseFatDirEntry(QString estring, QHash<QString, QVariant> *fsinfo, QList<QHash<QString, QVariant>> *fileinfolist, QList<QHash<QString, QVariant>>* orphanlist)
void ParseFatDirEntry(ForensicImage* curimg, QHash<QString, QVariant> *fsinfo, QList<QHash<QString, QVariant>> *fileinfolist, QList<QHash<QString, QVariant>>* orphanlist)
{
    QHash<QString, QVariant> fileinfo;
    QHash<QString, QVariant> orphaninfo;
    fileinfo.clear();
    QByteArray fatbuf;
    fatbuf.clear();
    QByteArray rootdirbuf;
    rootdirbuf.clear();
    int rootdirentrycount = 0;
    QStringList rootdirlayoutlist = fsinfo->value("rootdirlayout").toString().split(";", Qt::SkipEmptyParts);
    curimg->open(QIODevice::ReadOnly);
    for(int j=0; j < rootdirlayoutlist.count(); j++)
    {
        curimg->seek(rootdirlayoutlist.at(j).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
        rootdirbuf.append(curimg->read(rootdirlayoutlist.at(j).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
    }
    curimg->close();
    rootdirentrycount = rootdirbuf.count() / 32;
    //qDebug() << "rootdirentrycount:" << rootdirentrycount;
    uint inodecnt = 0;
    QString longnamestring = "";
    for(int i=0; i < rootdirentrycount; i++)
    {
        fileinfo.insert("fileattr", QVariant(rootdirbuf.at(i*32 + 11)));
        uint8_t firstchar = rootdirbuf.at(i*32);
        if(firstchar == 0x00) // entry is free and all remaining are free
            break;
        uint8_t fileattr = rootdirbuf.at(i*32 + 11);
        QString attrstr = "";
        if(fileattr & 0x01)
            attrstr += "Read Only,";
        else if(fileattr & 0x02)
            attrstr += "Hidden File,";
        else if(fileattr & 0x04)
            attrstr += "System File,";
        else if(fileattr & 0x08)
            attrstr += "Volume ID,";
        else if(fileattr & 0x10)
            attrstr += "SubDirectory,";
        else if(fileattr & 0x20)
            attrstr += "Archive File,";
        fileinfo.insert("attribute", QVariant(attrstr));
	//qDebug() << "attr string:" << attrstr;

        if(fileattr != 0x0f && fileattr != 0x00 && fileattr != 0x3f) // need to process differently // 0x3f is ATTR_LONG_NAME_MASK which is a long name entry sub-component
        {
            if(!longnamestring.isEmpty())
            {
                fileinfo.insert("longname", QVariant(longnamestring));
                longnamestring = "";
            }
            else
                fileinfo.insert("longname", QVariant(""));
            fileinfo.insert("firstchar", QVariant(rootdirbuf.at(i*32)));
            if((uint8_t)fileinfo.value("firstchar").toUInt() == 0xe5 || (uint8_t)fileinfo.value("firstchar").toUInt() == 0x05) // was allocated but now free
                fileinfo.insert("isdeleted", QVariant(1));
            else
                fileinfo.insert("isdeleted", QVariant(0));
            fileinfo.insert("restname", QString::fromStdString(rootdirbuf.mid(i*32 + 1, 7).toStdString()).replace(" ", ""));
            fileinfo.insert("extname", QString::fromStdString(rootdirbuf.mid(i*32 + 8, 3).toStdString()).replace(" ", ""));
            //uint8_t createtenth = rootdirbuf.at(i*32 + 13); // NOT GOING TO USE RIGHT NOW...
            fileinfo.insert("createdate", QVariant(ConvertDosTimeToUnixTime(rootdirbuf.at(i*32 + 15), rootdirbuf.at(i*32 + 14), rootdirbuf.at(i*32 + 17), rootdirbuf.at(i*32 + 16))));
            fileinfo.insert("accessdate", QVariant(ConvertDosTimeToUnixTime(0x00, 0x00, rootdirbuf.at(i*32 + 19), rootdirbuf.at(i*32 + 18))));
            fileinfo.insert("modifydate", QVariant(ConvertDosTimeToUnixTime(rootdirbuf.at(i*32 + 23), rootdirbuf.at(i*32 + 22), rootdirbuf.at(i*32 + 25), rootdirbuf.at(i*32 + 24))));
            uint16_t hiclusternum = qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 20, 2)); // always zero for fat12/16
            uint16_t loclusternum = qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 26, 2));
            fileinfo.insert("clusternum", QVariant(((uint32_t)hiclusternum >> 16) + loclusternum));
            if(fileinfo.value("extname").toString().count() > 0)
                fileinfo.insert("aliasname", QVariant(QString(char(firstchar) + fileinfo.value("restname").toString().toUtf8() + "." + fileinfo.value("extname").toString().toUtf8())));
            else
                fileinfo.insert("aliasname", QVariant(QString(char(firstchar) + fileinfo.value("restname").toString().toUtf8())));
            fileinfo.insert("inode", QVariant(inodecnt));
            if(fileinfo.value("longname").toString().isEmpty())
                fileinfo.insert("filename", fileinfo.value("aliasname"));
            else
                fileinfo.insert("filename", fileinfo.value("longname"));
	    //qDebug() << fileinfo.value("longname").toString() << "inodecnt:" << inodecnt;
            fileinfo.insert("parentinode",  QVariant(-1));
            fileinfo.insert("path", QVariant("/"));
	    QList<uint> clusterlist;
	    clusterlist.clear();
	    QString layout = "";
	    if(fileattr & 0x08) // volume id attribute
		fileinfo.insert("physicalsize", QVariant(0));
	    else
	    {
		if(fileinfo.value("clusternum").toUInt() >= 2)
		    GetNextCluster(fileinfo.value("clusternum").toUInt(), fsinfo->value("type").toUInt(), &fatbuf, &clusterlist);
		QString clusterstr = QString::number(fileinfo.value("clusternum").toUInt()) + ",";
		QString clustersize = QString::number(fsinfo->value("sectorspercluster").toUInt() * fsinfo->value("bytespersector").toUInt());
		layout = QString::number((fsinfo->value("clusterareastart").toUInt() * fsinfo->value("bytespersector").toUInt()) + (fileinfo.value("clusternum").toUInt() - 2) * fsinfo->value("sectorspercluster").toUInt() * fsinfo->value("bytespersector").toUInt()) + "," + clustersize + ";";
		for(int j=0; j < clusterlist.count()-1; j++)
		{
		    clusterstr += QString::number(clusterlist.at(j)) + ",";
		    layout += QString::number((fsinfo->value("clusterareastart").toUInt() * fsinfo->value("bytespersector").toUInt()) + (clusterlist.at(j) - 2) * clustersize.toUInt()) + "," + clustersize + ";";
		}
		fileinfo.insert("clusterlist", QVariant(clusterstr));
		fileinfo.insert("layout", QVariant(layout));
		fileinfo.insert("physicalsize", QVariant(clusterlist.count() * fsinfo->value("sectorspercluster").toUInt() * fsinfo->value("bytespersector").toUInt()));
	    }
            //qDebug() << "inodecnt:" << inodecnt << "alias name:" << fileinfo.value("aliasname").toString() << "clusternum:" << fileinfo.value("clusternum").toUInt();
            if(fileattr & 0x10) // sub directory attribute
	    {
		QStringList layoutlist = layout.split(";", Qt::SkipEmptyParts);
		int direntrycnt = 0;
		int lastdirentry = 0;
		QByteArray dirsizebuf;
		dirsizebuf.clear();
                curimg->open(QIODevice::ReadOnly);
                for(int j=0; j < layoutlist.count(); j++)
                {
                    curimg->seek(layoutlist.at(j).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
                    dirsizebuf.append(curimg->read(layoutlist.at(j).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
                }
                curimg->close();
		direntrycnt = dirsizebuf.count() / 32;
		for(int j=0; j < direntrycnt; j++)
		{
		    uint8_t firstchar = dirsizebuf.at(j*32);
		    if(firstchar == 0x00) // entry is free and all remaining are free
		    {
			lastdirentry = j;
			break;
		    }
		}
		fileinfo.insert("logicalsize", QVariant(lastdirentry * 32));
	    }
            else
                fileinfo.insert("logicalsize", QVariant(qFromLittleEndian<uint32_t>(rootdirbuf.mid(i*32 + 28, 4))));
            if(fileattr & 0x10) // sub directory attribute
	    {
		if(firstchar == 0xe5 || firstchar == 0x05) // deleted directory
		    fileinfo.insert("itemtype", QVariant(2)); // deleted directory
		else
		    fileinfo.insert("itemtype", QVariant(3)); // directory
	    }
	    else if(firstchar == 0xe5 || firstchar == 0x05) // deleted file
		fileinfo.insert("itemtype", QVariant(4)); // deleted file
            else
                fileinfo.insert("itemtype", QVariant(5)); // regular file
            fileinfolist->append(fileinfo);
            inodecnt++;
            if(fileattr & 0x10 && fileinfo.value("physicalsize").toUInt() > 0) // sub directory
            {
                // POSSIBLY REMOVE THIS IF SO IT WILL PROCESS DELETED AS WELL IF THEY CONTAIN CHILDREN...
		if(firstchar != 0xe5 && firstchar != 0x05) // not deleted
		    ParseSubDirectory(curimg, fsinfo, &fileinfo, fileinfolist, &inodecnt, &fatbuf, orphanlist);
		    //ParseSubDirectory(estring, fsinfo, &fileinfo, fileinfolist, &inodecnt, &fatbuf, orphanlist);
            }
        }
        else if(fileattr == 0x0f || 0x3f) // long directory entry for succeeding short entry...
        {
            if(rootdirbuf.at(i*32) & 0x40)
            {
                if(!longnamestring.isEmpty()) // orphan long entry
                {
                    orphaninfo.clear();
                    orphaninfo.insert("filename", QVariant(longnamestring));
                    orphanlist->append(orphaninfo);
		    //orphanlist->append(longnamestring);
                    longnamestring = "";
                }
            }
	    QString l3 = "";
	    QString l2 = "";
	    QString l1 = "";
	    if(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 28, 2)) < 0xFFFF)
		l3 += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 28, 2))));
	    if(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 30, 2)) < 0xFFFF)
		l3 += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 30, 2))));
	    if(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 14, 2)) < 0xFFFF)
		l2 += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 14, 2))));
	    if(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 16, 2)) < 0xFFFF)
		l2 += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 16, 2))));
	    if(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 18, 2)) < 0xFFFF)
		l2 += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 18, 2))));
	    if(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 20, 2)) < 0xFFFF)
		l2 += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 20, 2))));
	    if(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 22, 2)) < 0xFFFF)
		l2 += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 22, 2))));
	    if(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 24, 2)) < 0xFFFF)
		l2 += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 24, 2))));
	    if(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 1, 2)) < 0xFFFF)
		l1 += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 1, 2))));
	    if(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 3, 2)) < 0xFFFF)
		l1 += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 3, 2))));
	    if(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 5, 2)) < 0xFFFF)
		l1 += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 5, 2))));
	    if(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 7, 2)) < 0xFFFF)
		l1 += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 7, 2))));
	    if(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 9, 2)) < 0xFFFF)
		l1 += QString(QChar(qFromLittleEndian<uint16_t>(rootdirbuf.mid(i*32 + 9, 2))));
	    longnamestring.prepend(QString(l1 + l2 + l3).toUtf8());
        }
    }
}

//void ParseSubDirectory(QString estring, QHash<QString, QVariant>* fsinfo, QHash<QString, QVariant>* parentinfo, QList<QHash<QString, QVariant>>* fileinfolist, uint* inodecnt, QByteArray* fatbuf, QList<QHash<QString, QVariant>>* orphanlist)
void ParseSubDirectory(ForensicImage* curimg, QHash<QString, QVariant>* fsinfo, QHash<QString, QVariant>* parentinfo, QList<QHash<QString, QVariant>>* fileinfolist, uint* inodecnt, QByteArray* fatbuf, QList<QHash<QString, QVariant>>* orphanlist)
{
    uint curinode = *inodecnt;
    QHash<QString, QVariant> fileinfo;
    QHash<QString, QVariant> orphaninfo;
    fileinfo.clear();
    QByteArray dirbuf;
    dirbuf.clear();
    QStringList layoutlist = parentinfo->value("layout").toString().split(";", Qt::SkipEmptyParts);
    curimg->open(QIODevice::ReadOnly);
    for(int i=0; i < layoutlist.count(); i++)
    {
        if(i ==0)
        {
            curimg->seek(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(0).toULongLong() + 64); // # + 64 skips the first 2 dir entries of . and ..
            dirbuf.append(curimg->read(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(1).toULongLong() - 64)); // # - 64 accounts for skipping the first 2 dir entries of . and ..
        }
        else
        {
            curimg->seek(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
            dirbuf.append(curimg->read(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(i).toULongLong()));
        }
    }
    curimg->close();
    //qDebug() << "dir fat content:" << fatbuf->mid(0, 40).toHex();
    QString longnamestring = "";
    for(int i=0; i < dirbuf.count() / 32; i++)
    {
        fileinfo.insert("fileattr", QVariant(dirbuf.at(i*32 + 11)));
        uint8_t firstchar = dirbuf.at(i*32);
        if(firstchar == 0x00) // entry is free and all remaining are free
            break;
        uint8_t fileattr = dirbuf.at(i*32 + 11);
        QString attrstr = "";
        if(fileattr & 0x01)
            attrstr += "Read Only,";
        else if(fileattr & 0x02)
            attrstr += "Hidden File,";
        else if(fileattr & 0x04)
            attrstr += "System File,";
        else if(fileattr & 0x08)
            attrstr += "Volume ID,";
        else if(fileattr & 0x10)
            attrstr += "SubDirectory,";
        else if(fileattr & 0x20)
            attrstr += "Archive File,";
        fileinfo.insert("attribute", QVariant(attrstr));
        if(fileattr != 0x0f && fileattr != 0x00 && fileattr != 0x3f) // regular directory entry
        {
            if(!longnamestring.isEmpty())
            {
                fileinfo.insert("longname", QVariant(longnamestring));
                longnamestring = "";
            }
            else
                fileinfo.insert("longname", QVariant(""));
            fileinfo.insert("firstchar", QVariant(dirbuf.at(i*32)));
	    // MAY NOT NEED THIS ISDELETED SINCE I CAN ASSIGN AN ITEMTYPE...
            if(firstchar == 0xe5 || firstchar == 0x05) // was allocated but now free
                fileinfo.insert("isdeleted", QVariant(1));
            else
                fileinfo.insert("isdeleted", QVariant(0));
            fileinfo.insert("restname", QString::fromStdString(dirbuf.mid(i*32 + 1, 7).toStdString()).replace(" ", ""));
            fileinfo.insert("extname", QString::fromStdString(dirbuf.mid(i*32 + 8, 3).toStdString()).replace(" ", ""));
            //uint8_t createtenth = rootdirbuf.at(i*32 + 13); // NOT GOING TO USE RIGHT NOW...
            fileinfo.insert("createdate", QVariant(ConvertDosTimeToUnixTime(dirbuf.at(i*32 + 15), dirbuf.at(i*32 + 14), dirbuf.at(i*32 + 17), dirbuf.at(i*32 + 16))));
            fileinfo.insert("accessdate", QVariant(ConvertDosTimeToUnixTime(0x00, 0x00, dirbuf.at(i*32 + 19), dirbuf.at(i*32 + 18))));
            fileinfo.insert("modifydate", QVariant(ConvertDosTimeToUnixTime(dirbuf.at(i*32 + 23), dirbuf.at(i*32 + 22), dirbuf.at(i*32 + 25), dirbuf.at(i*32 + 24))));
            uint16_t hiclusternum = qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 20, 2)); // always zero for fat12/16
            uint16_t loclusternum = qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 26, 2));
            fileinfo.insert("clusternum", QVariant(((uint32_t)hiclusternum >> 16) + loclusternum));
            if(fileinfo.value("extname").toString().count() > 0)
                fileinfo.insert("aliasname", QVariant(QString(char(firstchar) + fileinfo.value("restname").toString().toUtf8() + "." + fileinfo.value("extname").toString().toUtf8())));
            else
                fileinfo.insert("aliasname", QVariant(QString(char(firstchar) + fileinfo.value("restname").toString().toUtf8())));
            fileinfo.insert("inode", QVariant(curinode));
	    //qDebug() << fileinfo.value("longname").toString() << "inode:" << curinode;
            fileinfo.insert("parentinode", QVariant(parentinfo->value("inode").toInt()));
            if(parentinfo->value("longname").toString().isEmpty())
                fileinfo.insert("path", QVariant(QString(parentinfo->value("path").toString().toUtf8() + parentinfo->value("aliasname").toString().toUtf8() + QString("/"))));
            else
                fileinfo.insert("path", QVariant(QString(parentinfo->value("path").toString().toUtf8() + parentinfo->value("longname").toString().toUtf8() + QString("/"))));
            QList<uint> clusterlist;
            clusterlist.clear();
	    if(fileinfo.value("clusternum").toUInt() >= 2)
		GetNextCluster(fileinfo.value("clusternum").toUInt(), fsinfo->value("type").toUInt(), fatbuf, &clusterlist);
            //qDebug() << "aliasname:" << fileinfo.value("aliasname").toString() << "path:" << fileinfo.value("path").toString() << curinode;
            //qDebug() << "inodecnt:" << curinode << "alias name:" << fileinfo.value("aliasname").toString() << "clusternum:" << fileinfo.value("clusternum").toUInt();
	    QString clustersize = QString::number(fsinfo->value("sectorspercluster").toUInt() * fsinfo->value("bytespersector").toUInt());
            // (fsinfo->value("clusterareastart").toUInt() * fsinfo->value("bytespersector").toUInt())
	    QString layout = QString::number((fsinfo->value("clusterareastart").toUInt() * fsinfo->value("bytespersector").toUInt()) + (fileinfo.value("clusternum").toUInt() - 2) * fsinfo->value("sectorspercluster").toUInt() * fsinfo->value("bytespersector").toUInt()) + "," + clustersize + ";";
            QString clusterstr = QString::number(fileinfo.value("clusternum").toUInt()) + ",";
            for(int j=0; j < clusterlist.count()-1; j++)
            {
                clusterstr += QString::number(clusterlist.at(j)) + ",";
		layout += QString::number((fsinfo->value("clusterareastart").toUInt() * fsinfo->value("bytespersector").toUInt()) + (clusterlist.at(j) - 2) * clustersize.toUInt()) + "," + clustersize + ";";
            }
            fileinfo.insert("clusterlist", QVariant(clusterstr));
	    fileinfo.insert("layout", QVariant(layout));
            fileinfo.insert("physicalsize", QVariant(clusterlist.count() * fsinfo->value("sectorspercluster").toUInt() * fsinfo->value("bytespersector").toUInt()));
            if(fileattr & 0x10) // sub directory entry
	    {
		QStringList layoutlist = layout.split(";", Qt::SkipEmptyParts);
		int direntrycnt = 0;
		int lastdirentry = 0;
		QByteArray dirsizebuf;
		dirsizebuf.clear();
                curimg->open(QIODevice::ReadOnly);
                for(int j=0; j < layoutlist.count(); j++)
                {
                    curimg->seek(layoutlist.at(j).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
                    dirsizebuf.append(curimg->read(layoutlist.at(j).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
                }
                curimg->close();
		direntrycnt = dirsizebuf.count() / 32;
		for(int j=0; j < direntrycnt; j++)
		{
		    uint8_t firstchar = dirsizebuf.at(j*32);
		    if(firstchar == 0x00) // entry is free and all remaining are free
		    {
			lastdirentry = j;
			break;
		    }
		}
		fileinfo.insert("logicalsize", QVariant(lastdirentry * 32));
	    }
            else
                fileinfo.insert("logicalsize", QVariant(qFromLittleEndian<uint32_t>(dirbuf.mid(i*32 + 28, 4))));
            if(fileattr & 0x10) // directory
		if(firstchar == 0xe5 || firstchar == 0x05) // deleted directory
		    fileinfo.insert("itemtype", QVariant(2)); // deleted directory
		else
		    fileinfo.insert("itemtype", QVariant(3)); // directory
	    else if(firstchar == 0xe5 || firstchar == 0x05) // deleted
		fileinfo.insert("itemtype", QVariant(4)); // deleted file
            else
                fileinfo.insert("itemtype", QVariant(5)); // regular file
            fileinfolist->append(fileinfo);
            curinode++;
            *inodecnt = curinode;
            if(fileattr & 0x10 && fileinfo.value("physicalsize").toUInt() > 0) // sub directory
            {
		if(firstchar != 0xe5 && firstchar != 0x05) // not deleted
		    ParseSubDirectory(curimg, fsinfo, &fileinfo, fileinfolist, inodecnt, fatbuf, orphanlist);
		curinode = *inodecnt;
            }
        }
        else if(fileattr == 0x0f || 0x3f) // long directory entry for succeeding short entry...
        {
            if(dirbuf.at(i*32) & 0x40)
            {
                if(!longnamestring.isEmpty()) // orphan long entry
                {
                    orphaninfo.clear();
                    orphaninfo.insert("filename", QVariant(longnamestring));
                    orphanlist->append(orphaninfo);
		    //orphanlist->append(longnamestring);
                    longnamestring = "";
                }
            }
	    QString l3 = "";
	    QString l2 = "";
	    QString l1 = "";
	    if(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 28, 2)) < 0xFFFF)
		l3 += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 28, 2))));
	    if(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 30, 2)) < 0xFFFF)
		l3 += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 30, 2))));
	    if(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 14, 2)) < 0xFFFF)
		l2 += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 14, 2))));
	    if(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 16, 2)) < 0xFFFF)
		l2 += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 16, 2))));
	    if(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 18, 2)) < 0xFFFF)
		l2 += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 18, 2))));
	    if(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 20, 2)) < 0xFFFF)
		l2 += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 20, 2))));
	    if(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 22, 2)) < 0xFFFF)
		l2 += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 22, 2))));
	    if(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 24, 2)) < 0xFFFF)
		l2 += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 24, 2))));
	    if(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 1, 2)) < 0xFFFF)
		l1 += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 1, 2))));
	    if(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 3, 2)) < 0xFFFF)
		l1 += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 3, 2))));
	    if(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 5, 2)) < 0xFFFF)
		l1 += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 5, 2))));
	    if(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 7, 2)) < 0xFFFF)
		l1 += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 7, 2))));
	    if(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 9, 2)) < 0xFFFF)
		l1 += QString(QChar(qFromLittleEndian<uint16_t>(dirbuf.mid(i*32 + 9, 2))));
	    longnamestring.prepend(QString(l1 + l2 + l3).toUtf8());
        }
    }
}

void GetNextCluster(uint32_t clusternum, uint fstype, QByteArray* fatbuf, QList<uint>* clusterlist)
{
    uint32_t curcluster = 0;
    int fatbyte1 = 0;
    if(fstype == 1) // FAT12
    {
        fatbyte1 = clusternum + (clusternum / 2);
        if(clusternum & 0x0001) // ODD
            curcluster = (qFromLittleEndian<uint16_t>(fatbuf->mid(fatbyte1, 2)) >> 4);
        else // EVEN
            curcluster = (qFromLittleEndian<uint16_t>(fatbuf->mid(fatbyte1, 2)) & 0x0FFF);
        if(curcluster != 0x0FF7)
            clusterlist->append(curcluster);
        if(curcluster < 0x0FF8 && curcluster >= 2)
            GetNextCluster(curcluster, fstype, fatbuf, clusterlist);
    }
    else if(fstype == 2) // FAT16
    {
        if(clusternum >= 2)
        {
            fatbyte1 = clusternum * 2;
            curcluster = qFromLittleEndian<uint16_t>(fatbuf->mid(fatbyte1, 2));
            if(curcluster != 0xFFF7)
                clusterlist->append(curcluster);
            if(curcluster < 0xFFF8 && curcluster >= 2)
                GetNextCluster(curcluster, fstype, fatbuf, clusterlist);
        }
    }
    else if(fstype == 3) // FAT32
    {
	if(clusternum >= 2)
	{
	    fatbyte1 = clusternum * 4;
	    //qDebug() << "fatbyte1:" << fatbyte1;
	    curcluster = (qFromLittleEndian<uint32_t>(fatbuf->mid(fatbyte1, 4)) & 0x0FFFFFFF);
	    //qDebug() << "curcluster:" << QString::number(curcluster, 16);
	    if(curcluster != 0x0FFFFFF7)
		clusterlist->append(curcluster);
	    if(curcluster < 0x0FFFFFF7 && curcluster >= 2)
		GetNextCluster(curcluster, fstype, fatbuf, clusterlist);
	}
    }
    else if(fstype == 4) // EXFAT
    {
        if(clusternum >= 2)
        {
            fatbyte1 = clusternum * 4;
            curcluster = qFromLittleEndian<uint32_t>(fatbuf->mid(fatbyte1, 4));
            if(curcluster != 0xFFFFFFF7)
                clusterlist->append(curcluster);
            if(curcluster < 0xFFFFFFF7 && curcluster >= 2)
                GetNextCluster(curcluster, fstype, fatbuf, clusterlist);
        }
    }
}

//void ProcessVolume(QString evidstring)
//void ProcessVolume(ForensicImage* tmpimg)
void ProcessVolume(ForImg* curimg)
{
    QString evidstring = curimg->ImgPath();
    //QString evidstring = tmpimg->ImgPath();
    qDebug() << "imgpath at start of processvolume:" << evidstring;
    QList<qint64> pofflist;
    pofflist.clear();
    QList<qint64> psizelist;
    psizelist.clear();
    QList<QHash<QString, QVariant>> fsinfolist;
    fsinfolist.clear();
    qint64 imgsize = 0;
    int evidcnt = 0;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*" + evidstring.split("/").last() + "*"), QDir::NoSymLinks | QDir::Dirs);
    evidcnt = evidfiles.first().split(QString(evidstring.split("/").last() + "-e")).last().toInt();
    QString evidencename = evidstring.split("/").last();
    QString evidencepath = wombatvariable.tmpmntpath + evidencename + "-e" + QString::number(evidcnt) + "/";
    QString emntstring = evidstring;
    if(evidstring.toLower().endsWith(".e01"))
    {
        emntstring = evidstring;
        //emntstring = wombatvariable.imgdatapath + evidstring.split("/").last() + "/ewf1";
    }
    else if(evidstring.toLower().endsWith(".aff") || evidstring.toLower().endsWith(".000") || evidstring.toLower().endsWith("001") || evidstring.toLower().endsWith(".zmg"))
        emntstring = wombatvariable.imgdatapath + evidstring.split("/").last() + "/" + evidstring.split("/").last() + ".raw";
    QFileInfo efileinfo(emntstring);
    if(evidstring.toLower().endsWith(".e01"))
    {
        imgsize = curimg->Size();
        //imgsize = tmpimg->size();
        //imgsize = testimage->size();
    }
    else
        imgsize = efileinfo.size();
    QList<QVariant> nodedata;
    nodedata.clear();
    nodedata << evidencename << "0" << QString::number(imgsize) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt));
    QString reportstring = "";
    reportstring += "<div id='e" + QString::number(evidcnt) + "'><table width='98%'>";
    reportstring += "<tr><th colspan='2'>Evidence Item (E" + QString::number(evidcnt) + "):" + evidstring + "</th></tr>";
    reportstring += "<tr class='odd vtop'><td>Image Size:</td><td>" + QString::number(imgsize) + " bytes</td></tr>";
    //reportstring += "<tr class='even vtop'><td>Sector Size:</td><td>" + QString::number(imginfo->sector_size) + " bytes</td></tr>";
    
    mutex.lock();
    treenodemodel->AddNode(nodedata, "-1", -1, -1);
    mutex.unlock();
    QFile estatfile(evidencepath + "stat");
    QTextStream out;
    if(!estatfile.isOpen())
        estatfile.open(QIODevice::Append | QIODevice::Text);
    if(estatfile.isOpen())
    {
        out.setDevice(&estatfile);
        // original evidence filename, evidence mount string, imgsize, id
        out << evidstring << "," << emntstring << "," << QString::number(imgsize) << "," << QString("e" + QString::number(evidcnt)) << ",0";
        out.flush();
        estatfile.close();
    }
    qDebug() << "Starting Parse Volume";
    ParseVolume(curimg, imgsize, &pofflist, &psizelist, &fsinfolist);
    //ParseVolume(tmpimg, imgsize, &pofflist, &psizelist, &fsinfolist);
    qDebug() << "Ending Parse Volume";
    //ParseVolume(emntstring, imgsize, &pofflist, &psizelist, &fsinfolist);
    if(pofflist.count() == 0)
    {	
	// virtual attempt to process entire image as a filesystem...
	pofflist.append(0);
	psizelist.append(imgsize/512);
        ParseFileSystemInformation(curimg, 0, &fsinfolist);
        //ParseFileSystemInformation(emntstring, 0, &fsinfolist);
        //ParseFileSystemInformation(tmpimg, 0, &fsinfolist);
    }
    //qDebug() << "pofflist:" << pofflist << "psizelist:" << psizelist;
    // add partitions to tree and decide about stat/prop files and where to put them...
    int ptreecnt = 0;
    QString curpartpath;
    QDir dir;
    QFile pstatfile;
    for(int i=0; i < pofflist.count(); i++)
    {
        // might want to also look at having a single stat for all parts/fs and one for all files for each part/fs..
        // probably should write tree out to a file for easy loading on case open...
	if(i == 0) // INITIAL PARTITION
	{
	    if(pofflist.at(i)*512 > 0) // UNALLOCATED PARTITION BEFORE THE FIRST PARTITION
	    {
                curpartpath = evidencepath + "p" + QString::number(ptreecnt) + "/";
                dir.mkpath(curpartpath);
                pstatfile.setFileName(curpartpath + "stat");
                if(!pstatfile.isOpen())
                    pstatfile.open(QIODevice::Append | QIODevice::Text);
                if(pstatfile.isOpen())
                {
                    out.setDevice(&pstatfile);
                    // partition name, offset, size, partition type, id
                    out << "UNALLOCATED,0," << QString::number(pofflist.at(i)*512) << ",0," << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
                    out.flush();
                    pstatfile.close();
                }
                reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(ptreecnt) + "):</td><td>UNALLOCATED</td></tr>";
                partitionlist.append("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + ": UNALLOCATED");
		nodedata.clear();
		nodedata << "UNALLOCATED" << "0" << QString::number(pofflist.at(i)*512) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
		mutex.lock();
		treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), -1, 0);
		mutex.unlock();
                // FILE CARVING DIRECTORIES
                nodedata.clear();
                nodedata << QByteArray("carved validated").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-cv");
                mutex.lock();
                treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt)), 11, 0);
                mutex.unlock();
                nodedata.clear();
                nodedata << QByteArray("carved unvalidated").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-cu");
                mutex.lock();
                treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt)), 11, 0);
                mutex.unlock();
		ptreecnt++;
	    }
            curpartpath = evidencepath + "p" + QString::number(ptreecnt) + "/";
            dir.mkpath(curpartpath);
            pstatfile.setFileName(curpartpath + "stat");
            if(!pstatfile.isOpen())
                pstatfile.open(QIODevice::Append | QIODevice::Text);
            if(pstatfile.isOpen())
            {
                out.setDevice(&pstatfile);
                // FOR OUT STAT FILE, MAYBE I SHOULD JUST LOOP OVER THE HASH PROPERTIES AND STORE THEM ALL IN A COMMA LIST... OR A NAME|VALUE, LIST
                // OR I CAN FIGURE OUT WHAT I NEED FOR EACH FILE TYPE AND GENERATE STRING ACCORDINGLY THROUGH A FUNCTION... QString GetFileSystemOutString(fsinfolist.at(i));
                out << "ALLOCATED," << QString::number(pofflist.at(i)*512) << "," << QString::number(psizelist.at(i)*512) << ",1," << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
                out.flush();
                pstatfile.close();
            }
            reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(ptreecnt) + "):</td><td>" + fsinfolist.at(i).value("vollabel").toString() + " [" + fsinfolist.at(i).value("typestr").toString() + "]</td></tr>";
            partitionlist.append("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + ": " + fsinfolist.at(i).value("vollabel").toString() + " [" + fsinfolist.at(i).value("typestr").toString() + "]");
	    nodedata.clear();
	    nodedata << QString(fsinfolist.at(i).value("vollabel").toString() + " [" + fsinfolist.at(i).value("typestr").toString() + "]") << "0" << QString::number(psizelist.at(i)*512) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
	    mutex.lock();
	    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), -1, 0);
	    mutex.unlock();
            WriteFileSystemProperties((QHash<QString, QVariant>*)&(fsinfolist.at(i)), QString(curpartpath + "prop"));
            QList<QHash<QString, QVariant>> fileinfolist;
            QHash<QString, QHash<QString, QVariant>> mftentries;
	    QList<QHash<QString, QVariant>> orphanlist;
            // IF FAT12/16/32 THEN
            // MAYBE CHANGE NAME FROM PARSEDIRECTORY TO PARSEFATDIRENTRY
	    if(fsinfolist.at(i).value("type").toUInt() == 1 || fsinfolist.at(i).value("type").toUInt() == 2 || fsinfolist.at(i).value("type").toUInt() == 3) // FAT12 || FAT16 || FAT32
            {
		//ParseFatDirEntry(tmpimg, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);
		//ParseFatDirEntry(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);
            }
	    else if(fsinfolist.at(i).value("type").toUInt() == 4) // EXFAT
            {
		//ParseExFatDirEntry(tmpimg, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);
		//ParseExFatDirEntry(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);
            }
	    else if(fsinfolist.at(i).value("type").toUInt() == 5) // NTFS
            {
		//ParseNtfsDirectory(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &mftentries, &fileinfolist, &orphanlist, 5);
                qDebug() << "begin parse ntfs directory";
		ParseNtfsDirectory(curimg, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, NULL, 5, 0);
                qDebug() << "end parse ntfs directory";
		//ParseNtfsDirectory(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, NULL, 5, 0);
		//ParseNtfsDirectory(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, 5);
                qDebug() << "begin parse mft";
                ParseMFT(curimg, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);// may need to pass adsinfolist here...
                qDebug() << "end parse mft";
                //ParseMFT(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);// may need to pass adsinfolist here...
            }
            else if(fsinfolist.at(i).value("type").toUInt() == 6) // EXT2/3/4
                //ParseExtDirectory(tmpimg, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, NULL, 2, 0);
                //ParseExtDirectory(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, NULL, 2, 0);
            //ParseDirectory(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);

            qDebug() << "begin populate files";
            fileinfolist.clear();
            orphanlist.clear();
            //PopulateFiles(tmpimg, curpartpath, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, evidcnt, ptreecnt); 
            PopulateFiles(curimg, curpartpath, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, evidcnt, ptreecnt); 
            qDebug() << "end populate files";
            //PopulateFiles(emntstring, curpartpath, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, evidcnt, ptreecnt); 
            // FILE CARVING DIRECTORIES
            nodedata.clear();
            nodedata << QByteArray("carved validated").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-cv");
            mutex.lock();
            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt)), 11, 0);
            mutex.unlock();
            nodedata.clear();
            nodedata << QByteArray("carved unvalidated").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-cu");
            mutex.lock();
            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt)), 11, 0);
            mutex.unlock();
	    ptreecnt++;
	    //qDebug() << "1st partition which has unalloc before...";
	}
	if(i > 0 && i < pofflist.count()) // MIDDLE PARTITIONS
	{
	    if(pofflist.at(i) > (pofflist.at(i-1) + psizelist.at(i-1)))
	    {
                curpartpath = evidencepath + "p" + QString::number(ptreecnt) + "/";
                dir.mkpath(curpartpath);
                pstatfile.setFileName(curpartpath + "stat");
                if(!pstatfile.isOpen())
                    pstatfile.open(QIODevice::Append | QIODevice::Text);
                if(pstatfile.isOpen())
                {
                    out.setDevice(&pstatfile);
                    // partition name, offset, size, partition type, id
                    out << "UNALLOCATED," << QString::number((pofflist.at(i-1) + psizelist.at(i-1))*512) << "," << QString::number(512*(pofflist.at(i) - (pofflist.at(i-1) + psizelist.at(i-1)))) << ",0," << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
                    out.flush();
                    pstatfile.close();
                }
                reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(ptreecnt) + "):</td><td>UNALLOCATED</td></tr>";
                partitionlist.append("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + ": UNALLOCATED");
		nodedata.clear();
		nodedata << "UNALLOCATED" << "0" << QString::number(512*(pofflist.at(i) - (pofflist.at(i-1) + psizelist.at(i-1)))) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
		mutex.lock();
		treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), -1, 0);
		mutex.unlock();
                // FILE CARVING DIRECTORIES
                nodedata.clear();
                nodedata << QByteArray("carved validated").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-cv");
                mutex.lock();
                treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt)), 11, 0);
                mutex.unlock();
                nodedata.clear();
                nodedata << QByteArray("carved unvalidated").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-cu");
                mutex.lock();
                treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt)), 11, 0);
                mutex.unlock();
		ptreecnt++;
	    }
	    // add existing partition here...
	    nodedata.clear();
	    //nodedata << fsvolname << "0" << QString::number(psizelist.at(i)) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
            curpartpath = evidencepath + "p" + QString::number(ptreecnt) + "/";
            dir.mkpath(curpartpath);
            pstatfile.setFileName(curpartpath + "stat");
            if(!pstatfile.isOpen())
                pstatfile.open(QIODevice::Append | QIODevice::Text);
            if(pstatfile.isOpen())
            {
                out.setDevice(&pstatfile);
                // partition name, offset, size, partition type, id
                out << "ALLOCATED," << QString::number(pofflist.at(i)*512) << "," << QString::number(psizelist.at(i)*512) << ",1," << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
                out.flush();
                pstatfile.close();
            }
            reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(ptreecnt) + "):</td><td>" + fsinfolist.at(i).value("vollabel").toString() + " [" + fsinfolist.at(i).value("typestr").toString() + "]</td></tr>";
            partitionlist.append("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + ": " + fsinfolist.at(i).value("vollabel").toString() + " [" + fsinfolist.at(i).value("typestr").toString() + "]");
	    nodedata << QString(fsinfolist.at(i).value("vollabel").toString() + " [" + fsinfolist.at(i).value("typestr").toString() + "]") << "0" << QString::number(psizelist.at(i)*512) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
	    //nodedata << "ALLOCATED" << "0" << QString::number(psizelist.at(i)*512) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
	    mutex.lock();
	    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), -1, 0);
	    mutex.unlock();
            WriteFileSystemProperties((QHash<QString, QVariant>*)&(fsinfolist.at(i)), QString(curpartpath + "prop"));
            QList<QHash<QString, QVariant>> fileinfolist;
            QHash<QString, QHash<QString, QVariant>> mftentries;
            QList<QHash<QString, QVariant>> orphanlist;
            // IF FAT12/16/32 THEN
            // MAYBE CHANGE NAME FROM PARSEDIRECTORY TO PARSEFATDIRENTRY
	    if(fsinfolist.at(i).value("type").toUInt() == 1 || fsinfolist.at(i).value("type").toUInt() == 2 || fsinfolist.at(i).value("type").toUInt() == 3) // FAT12 || FAT16 || FAT32
            {
		//ParseFatDirEntry(tmpimg, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);
		//ParseFatDirEntry(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);
            }
	    else if(fsinfolist.at(i).value("type").toUInt() == 4) // EXFAT
            {
		//ParseExFatDirEntry(tmpimg, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);
		//ParseExFatDirEntry(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);
            }
	    else if(fsinfolist.at(i).value("type").toUInt() == 5) // NTFS
            {
                qDebug() << "begin parse ntfs directory";
		ParseNtfsDirectory(curimg, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, NULL, 5, 0);
                qDebug() << "end parse ntfs directory";
		//ParseNtfsDirectory(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, NULL, 5, 0);
		//ParseNtfsDirectory(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &mftentries, &fileinfolist, &orphanlist, 5);
		//ParseNtfsDirectory(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, 5);
                qDebug() << "begin parse mft";
                ParseMFT(curimg, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist); // may need to pass adsinfolist here...
                qDebug() << "end parse mft";
                //ParseMFT(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist); // may need to pass adsinfolist here...
            }
            else if(fsinfolist.at(i).value("type").toUInt() == 6) // EXT2/3/4
                //ParseExtDirectory(tmpimg, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, NULL, 2, 0);
                //ParseExtDirectory(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, NULL, 2, 0);
            //ParseDirectory(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);
            // ELSE EXFAT THEN
            // ELSE ... THEN

            qDebug() << "begin populate files";
            fileinfolist.clear();
            orphanlist.clear();
            //PopulateFiles(tmpimg, curpartpath, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, evidcnt, ptreecnt); 
            PopulateFiles(curimg, curpartpath, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, evidcnt, ptreecnt); 
            qDebug() << "end populate files";
            //PopulateFiles(emntstring, curpartpath, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, evidcnt, ptreecnt); 
            // FILE CARVING DIRECTORIES
            nodedata.clear();
            nodedata << QByteArray("carved validated").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-cv");
            mutex.lock();
            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt)), 11, 0);
            mutex.unlock();
            nodedata.clear();
            nodedata << QByteArray("carved unvalidated").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-cu");
            mutex.lock();
            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt)), 11, 0);
            mutex.unlock();
	    ptreecnt++;
	}
	if(i == pofflist.count() - 1 && ((pofflist.at(i) + psizelist.at(i))*512) < imgsize)
	{
            curpartpath = evidencepath + "p" + QString::number(ptreecnt) + "/";
            dir.mkpath(curpartpath);
            pstatfile.setFileName(curpartpath + "stat");
            if(!pstatfile.isOpen())
                pstatfile.open(QIODevice::Append | QIODevice::Text);
            if(pstatfile.isOpen())
            {
                out.setDevice(&pstatfile);
                // partition name, offset, size, partition type, id
                out << "UNALLOCATED," << QString::number((psizelist.at(i) + pofflist.at(i))*512) << "," << QString::number(imgsize - 512*(pofflist.at(i) + psizelist.at(i))) << ",1," << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
                out.flush();
                pstatfile.close();
            }
            reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(ptreecnt) + "):</td><td>UNALLOCATED</td></tr>";
            partitionlist.append("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + ": UNALLOCATED");
	    nodedata.clear();
	    nodedata << "UNALLOCATED" << "0" << QString::number(imgsize - 512*(pofflist.at(i) + psizelist.at(i))) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
	    mutex.lock();
	    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), -1, 0);
	    mutex.unlock();
	    //add final unalloc partition to tree here...
            // FILE CARVING DIRECTORIES
            nodedata.clear();
            nodedata << QByteArray("carved validated").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-cv");
            mutex.lock();
            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt)), 11, 0);
            mutex.unlock();
            nodedata.clear();
            nodedata << QByteArray("carved unvalidated").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-cu");
            mutex.lock();
            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt)), 11, 0);
            mutex.unlock();
	    ptreecnt++;
	    //qDebug() << "unalloc exists after last partition...";
	}
    }
    reportstring += "</table></div><br/>\n";
    EvidenceReportData tmpdata;
    tmpdata.evidid = evidcnt;
    tmpdata.evidname = evidencename;
    tmpdata.evidcontent = reportstring;
    evidrepdatalist.append(tmpdata);
    nodedata.clear();
    nodedata << QByteArray("carved manually").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-cm");
    mutex.lock();
    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), 11, 0);
    mutex.unlock();
}

//void PopulateFiles(QString emntstring, QString curpartpath, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, int evidcnt, int ptreecnt)
//void PopulateFiles(ForensicImage* curimg, QString curpartpath, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, int evidcnt, int ptreecnt)
void PopulateFiles(ForImg* curimg, QString curpartpath, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, int evidcnt, int ptreecnt)
{
    //QList<QHash<QString, QVariant>> overflow;
    QList<QVariant> nodedata;
    nodedata.clear();
    //overflow.clear();
    for(int j=0; j < fileinfolist->count(); j++)
    {
        QString parentstr = "";
        if(fileinfolist->at(j).value("parentinode").toInt() == -1)
            parentstr = QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
        else
            parentstr = QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-f" + QString::number(fileinfolist->at(j).value("parentinode").toInt()));
        nodedata.clear();
        QByteArray ba;
        ba.clear();
        ba.append(fileinfolist->at(j).value("filename").toString().toUtf8());
        nodedata << ba.toBase64();
        ba.clear();
        //qDebug() << "filename:" << fileinfolist->at(j).value("filename").toString();
        //qDebug() << "alias name:" << fileinfolist.at(j).value("aliasname").toString() << "long name:" << fileinfolist.at(j).value("longname").toString();
        ba.append(fileinfolist->at(j).value("path").toString().toUtf8());
        nodedata << ba.toBase64() << QVariant(fileinfolist->at(j).value("logicalsize").toUInt()) << fileinfolist->at(j).value("createdate", "0") << fileinfolist->at(j).value("accessdate", "0") << fileinfolist->at(j).value("modifydate", "0") << fileinfolist->at(j).value("statusdate", "0") << QVariant("0");
        if(fileinfolist->at(j).value("logicalsize").toUInt() > 0)
        {
	    if(fileinfolist->at(j).value("itemtype").toUInt() == 3 && fileinfolist->at(j).value("isdeleted").toInt() == 0)
		nodedata << QVariant("Directory") << QVariant("Directory"); // category << signature
	    else
	    {
		QByteArray sigbuf;
		sigbuf.clear();
                if(fileinfolist->at(j).value("layout").toString().split(";", Qt::SkipEmptyParts).count() > 0)
                    sigbuf = curimg->ReadContent(fileinfolist->at(j).value("layout").toString().split(";", Qt::SkipEmptyParts).at(0).split(",").at(0).toLongLong(), fileinfolist->at(j).value("layout").toString().split(";", Qt::SkipEmptyParts).at(0).split(",").at(1).toLongLong());
		QString mimestr = GenerateCategorySignature(sigbuf, fileinfolist->at(j).value("filename").toString());
                sigbuf.clear();
		nodedata << QVariant(mimestr.split("/").at(0)) << QVariant(mimestr.split("/").at(1)); // category << signature
	    }
        }
        else
            nodedata << "Empty" << "Empty File";
        nodedata << QVariant("0") << QVariant(QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-f" + QString::number(fileinfolist->at(j).value("inode").toUInt())));
        mutex.lock();
        treenodemodel->AddNode(nodedata, parentstr, fileinfolist->at(j).value("itemtype").toInt(), fileinfolist->at(j).value("isdeleted").toInt());
        mutex.unlock();
        if(nodedata.at(11).toString().split("-").count() == 3)
        {
            listeditems.append(nodedata.at(11).toString());
            filesfound++;
            isignals->ProgUpd();
        }
        WriteFileProperties((QHash<QString, QVariant>*)&(fileinfolist->at(j)), QString(curpartpath + "f" + QString::number(fileinfolist->at(j).value("inode").toUInt()) + ".prop"));
    }
    int curinode = fileinfolist->count();
    AddVirtualFileSystemFiles(fsinfo, &curinode, curpartpath, QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt)));
    // add orphan directory and orphan files...
    // MAYBE I WANT ORPHAN FOLDER TO NOT BE A FOUND FILE COUNT ???? BUT WHAT COULD IT BE ?????
    // I'M GUESSING THE ORPHANS COUNT (CURINODE) AND FILESFOUND WILL BE OFF
    QByteArray ba;
    ba.clear();
    nodedata.clear();
    ba.append("orphans");
    nodedata << ba.toBase64();
    ba.clear();
    ba.append("/");
    QString parentstr = QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-f" + QString::number(curinode));
    nodedata << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QVariant("Directory") << QVariant("Virtual Directory") << "0" << QVariant(parentstr);
    mutex.lock();
    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt)), 11, 0);
    mutex.unlock();
    curinode++;
    // NEED TO MODIFY ORPHANLIST TO BE A QHASH INSTEAD OF A QSTRINGLIST AND UPDATE THE FAT ORPHAN FILES ACCORDINGLY...
    for(int j=0; j < orphanlist->count(); j++)
    {
        QString mimestr = "0/0";
        if(orphanlist->at(j).contains("layout"))
        {
            QByteArray sigbuf;
            sigbuf.clear();
            qDebug() << "orphanlist:" << orphanlist->at(j).value("filename").toString() << orphanlist->at(j).value("layout").toString();
            if(orphanlist->at(j).value("layout").toString().split(";").count() > 1)
            {
                sigbuf = curimg->ReadContent(orphanlist->at(j).value("layout").toString().split(";").at(0).split(",").at(0).toLongLong(), orphanlist->at(j).value("layout").toString().split(";").at(0).split(",").at(1).toLongLong());
                mimestr = GenerateCategorySignature(sigbuf, orphanlist->at(j).value("filename").toString());
                sigbuf.clear();
            }
        }

        QString curid = QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-f" + QString::number(curinode + j));
        nodedata.clear();
        ba.clear();
        ba.append(orphanlist->at(j).value("filename").toString().toUtf8());
        nodedata << ba.toBase64();
        ba.clear();
        ba.append("/orphans/");
        nodedata << ba.toBase64() << orphanlist->at(j).value("logicalsize", "0") << orphanlist->at(j).value("createdate", "0") << orphanlist->at(j).value("accessdate", "0") << orphanlist->at(j).value("modifydate", "0") << orphanlist->at(j).value("statusdate", "0") << "0" << QVariant(mimestr.split("/").at(0)) << QVariant(mimestr.split("/").at(1)) << "0" << QVariant(curid);
        mutex.lock();
        treenodemodel->AddNode(nodedata, parentstr, 4, 1);
        mutex.unlock();
        WriteFileProperties((QHash<QString, QVariant>*)&(orphanlist->at(j)), QString(curpartpath + "f" + QString::number(curinode + j) + ".prop"));
    }
}

void AddVirtualFileSystemFiles(QHash<QString, QVariant>* fsinfo, int* curinode, QString partpath, QString parentstr)
{
    QList<QVariant> nodedata;
    nodedata.clear();
    int inode = *curinode;
    if(fsinfo->value("type").toUInt() == 1 || fsinfo->value("type").toUInt() == 2 || fsinfo->value("type").toUInt() == 3 || fsinfo->value("type") == 4) // FAT12 || FAT16 || FAT32 || EXFAT
    {
        nodedata << QByteArray("$MBR").toBase64() << QByteArray("/").toBase64() << "512" << "0" << "0" << "0" << "0" << "0" << "System File" << "Master Boot Record" << "0" << QString(parentstr + "-f" + QString::number(inode));
        mutex.lock();
        treenodemodel->AddNode(nodedata, parentstr, 10, 0);
        mutex.unlock();
        inode++;
        for(int i=0; i < fsinfo->value("fatcount").toInt(); i++)
        {
            QByteArray ba;
            ba.clear();
            ba.append(QString("$FAT" + QString::number(i+1)).toUtf8());
            nodedata.clear();
            nodedata << ba.toBase64() << QByteArray("/").toBase64() << QVariant(fsinfo->value("fatsize").toUInt() * fsinfo->value("bytespersector").toUInt()) << "0" << "0" << "0" << "0" << "0" << "System File" << "File Allocation Table" << "0" << QString(parentstr + "-f" + QString::number(inode));
            mutex.lock();
            treenodemodel->AddNode(nodedata, parentstr, 10, 0);
            mutex.unlock();
            inode++;
        }
        // NEED TO WRITE THE FILE PROPERTIES FOR THE 3 FILES
        //if(fsinfo->value("type").toUInt() == 4) // Might Need to Add the Alloc_Bitmap, and the Other special's, but I think they are taken care when parsing root directory...
    }
    *curinode = inode;
}
void WriteFileProperties(QHash<QString, QVariant>*fileinfo, QString pathstring)
{
    QFile filepropfile(pathstring);
    if(!filepropfile.isOpen())
        filepropfile.open(QIODevice::Append | QIODevice::Text);
    if(filepropfile.isOpen())
    {
        QTextStream out;
        out.setDevice(&filepropfile);
        // need to do an if(fileinfo->value("type").toUInt() == 1) // FAT12
        // for the various custom properties i need for each filesystem.
	if(fileinfo->contains("extinode"))
	    out << "EXTFS inode|" << fileinfo->value("extinode").toUInt() << "|EXTFS inode value to locate file in the filesystem" << Qt::endl;
	if(fileinfo->contains("aliasname"))
	    out << "Alias Name|" << fileinfo->value("aliasname").toString() << "|8.3 file name" << Qt::endl;
        out << "File Attributes|" << fileinfo->value("attribute").toString() << "|File attributes." << Qt::endl;
	if(fileinfo->contains("clusterlist"))
	    out << "Cluster List|" << fileinfo->value("clusterlist").toString() << "|Clusters occupied by the file." << Qt::endl;
        out << "Layout|" << fileinfo->value("layout").toString() << "|File offset,size; layout." << Qt::endl;
        out << "Physical Size|" << QString::number(fileinfo->value("physicalsize").toUInt()) << "|Sector Size in Bytes for the file." << Qt::endl;
	if(fileinfo->contains("deletedate"))
	    out << "Deleted Time|" << QDateTime::fromSecsSinceEpoch(fileinfo->value("deletedate").toInt(), QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP") << "|Deleted time for the file." << Qt::endl;
	if(fileinfo->contains("mode"))
	    out << "Mode|" << fileinfo->value("mode").toString() << "|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]" << Qt::endl;
	if(fileinfo->contains("userid"))
	    out << "uid / gid|" << QString(QString::number(fileinfo->value("userid").toUInt()) + " / " + QString::number(fileinfo->value("groupid").toUInt())) << "|User ID and Group ID" << Qt::endl;
	if(fileinfo->contains("linkcount"))
	    out << "Link Count|" << QString::number(fileinfo->value("linkcount").toUInt()) << "|Number of files pointing to this file" << Qt::endl;
	if(fileinfo->contains("ntinode"))
	    out << "MFT Entry Record|" << QString::number(fileinfo->value("ntinode").toUInt()) << "|Entry record for the file in the Master File Table (MFT)" << Qt::endl;
	if(fileinfo->contains("mftrecordlayout"))
	    out << "MFT Record Layout|" << fileinfo->value("mftrecordlayout").toString() << "|Byte offset and size for the MFT Entry record" << Qt::endl;
	if(fileinfo->contains("parntinode"))
	    out << "Parent MFT Entry Record|" << QString::number(fileinfo->value("parntinode").toUInt()) << "|Parent Entry Record for the current entry record in the MFT" << Qt::endl;
	if(fileinfo->contains("filecreate"))
	    out << "$FILE_NAME Create Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("filecreate").toUInt()) << "|File Creation time as recorded in the $FILE_NAME attribute" << Qt::endl;
	if(fileinfo->contains("filemodify"))
	    out << "$FILE_NAME Modified Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("filemodify").toUInt()) << "|File Modification time as recorded in the $FILE_NAME attribute" << Qt::endl;
	if(fileinfo->contains("filestatus"))
	    out << "$FILE_NAME Status Changed Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("filestatus").toUInt()) << "|File Status Changed time as recorded in the $FILE_NAME attribute" << Qt::endl;
	if(fileinfo->contains("fileaccess"))
	    out << "$FILE_NAME Accessed Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("fileaccess").toUInt()) << "|File Accessed time as recorded in the $FILE_NAME attribute" << Qt::endl;
        if(fileinfo->contains("mftsequenceid"))
            out << "MFT Sequence ID|" << QString::number(fileinfo->value("mftsequenceid").toUInt()) << "|Sequence ID for the file as recorded in the MFT entry" << Qt::endl;
        if(fileinfo->contains("i30sequenceid"))
            out << "$I30 Sequence ID|" << QString::number(fileinfo->value("i30sequenceid").toUInt()) << "|Sequence ID for the file as recorded in the $I30 entry" << Qt::endl;
        if(fileinfo->contains("i30create"))
            out << "$I30 Create Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("i30create").toUInt()) << "|File Creation time as recorded in the $I30 entry" << Qt::endl;
        if(fileinfo->contains("i30modify"))
            out << "$I30 Modify Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("i30modify").toUInt()) << "|File Modification time as recorded in the $I30 entry" << Qt::endl;
        if(fileinfo->contains("i30status"))
            out << "$I30 Status Changed Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("i30status").toUInt()) << "|File Status Changed time as recorded in the $I30 entry" << Qt::endl;
        if(fileinfo->contains("i30access"))
            out << "$I30 Accessed Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("i30access").toUInt()) << "|File Accessed time as recorded in the $I30 entry" << Qt::endl;
        out.flush();
        filepropfile.close();
    }
}

void WriteFileSystemProperties(QHash<QString, QVariant>* fsinfo, QString pathstring)
{
    QFile fspropfile(pathstring);
    if(!fspropfile.isOpen())
        fspropfile.open(QIODevice::Append | QIODevice::Text);
    if(fspropfile.isOpen())
    {
        // IMPLEMENT ANY FAT32/EXFAT FS PROPERTIES THAT ARE INTERESTING
        QTextStream out;
        out.setDevice(&fspropfile);
        out << "File System Type|" << QString::number(fsinfo->value("type").toUInt()) << "|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|" << fsinfo->value("typestr").toString() << "|File System Type String." << Qt::endl;
        out << "Bytes Per Sector|" << QString::number(fsinfo->value("bytespersector").toUInt()) << "|Number of Bytes per Sector, usually 512." << Qt::endl;
        out << "Sectors Per Cluster|" << QString::number(fsinfo->value("sectorspercluster").toUInt()) << "|Number of Sectors per Cluster." << Qt::endl;
        out << "Total Sectors|" << QString::number(fsinfo->value("totalsectors").toUInt()) << "|Number of sectors in the file system." << Qt::endl;
        out << "Volume Label|" << fsinfo->value("vollabel").toString() << "|Volume Label for the file system." << Qt::endl;
        if(fsinfo->value("type").toUInt() == 1 || fsinfo->value("type").toUInt() == 2 || fsinfo->value("type").toUInt() == 3 || fsinfo->value("type").toUInt() == 4) // FAT12 || FAT16 || FAT32 || EXFAT
        {
            out << "Fat Count|" << QString::number(fsinfo->value("fatcount").toUInt()) << "|Number of FAT's in the file system." << Qt::endl;
            out << "FAT Offset|" << QString::number(fsinfo->value("fatoffset").toUInt()) << "|Byte offset to the start of the first FAT" << Qt::endl;
            out << "FAT Size|" << QString::number(fsinfo->value("fatsize").toUInt()) << "|Size of the FAT." << Qt::endl;
            out << "Reserved Area Size|" << QString::number(fsinfo->value("reservedareasize").toUInt()) << "|Size of the reserved area at the beginning of the file system." << Qt::endl;
            out << "File System Sector Count|" << QString::number(fsinfo->value("fssectorcnt").toUInt()) << "|Total Sectors in the volume" << Qt::endl;
            out << "Root Directory Offset|" << QString::number(fsinfo->value("rootdiroffset").toUInt()) << "|Byte Offset for the root directory" << Qt::endl;
            out << "Root Directory Max Files|" << QString::number(fsinfo->value("rootdirmaxfiles").toUInt()) << "|Maximum number of root directory entries" << Qt::endl;
            out << "Root Directory Sectors|" << QString::number(fsinfo->value("rootdirsectors").toUInt()) << "|Number of sectors for the root directory" << Qt::endl;
            out << "Root Directory Size|" << QString::number(fsinfo->value("rootdirsize").toUInt()) << "|Size in bytes for the root directory" << Qt::endl;
            out << "Cluster Area Start|" << QString::number(fsinfo->value("clusterareastart").toUInt()) << "|Byte offset to the start of the cluster area" << Qt::endl;
        }
	else if(fsinfo->value("type").toUInt() == 5) // NTFS
	{
	    out << "Bytes Per Cluster|" << QString::number(fsinfo->value("bytespercluster").toUInt()) << "|Number of bytes per cluster" << Qt::endl;
	    out << "MFT Starting Cluster|" << QString::number(fsinfo->value("mftstartingcluster").toUInt()) << "|Starting cluster number for the MFT" << Qt::endl;
	    out << "MFT Starting Offset|" << QString::number(fsinfo->value("mftoffset").toUInt()) << "|Starting byte for the MFT" << Qt::endl;
	    out << "MFT Entry Size|" << QString::number(fsinfo->value("mftentrysize").toUInt()) << "|Entry size in clusters for an MFT Entry" << Qt::endl;
            out << "MFT Entry Bytes|" << QString::number(fsinfo->value("mftentrybytes").toUInt()) << "| Entry size in bytes for an MFT Entry" << Qt::endl;
	    out << "Serial Number|" << QString::number(fsinfo->value("serialnum").toUInt(), 16) << "|Serial number for the file system volume" << Qt::endl;
	    out << "MFT Layout|" << fsinfo->value("mftlayout").toString() << "|Layout for the MFT in starting offset, size; format" << Qt::endl;
            out << "Max MFT Entries|" << QString::number(fsinfo->value("maxmftentries").toUInt()) << "|Max MFT Entries allowed in the MFT" << Qt::endl;
	}
	else if(fsinfo->value("type").toUInt() == 6) // EXT2/3/4
	{
	    out << "Created Time|" << QDateTime::fromSecsSinceEpoch(fsinfo->value("mkfstime").toInt(), QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP") << "|Creation time for the file system" << Qt::endl;
	    out << "Mount Time|" << QDateTime::fromSecsSinceEpoch(fsinfo->value("mounttime").toInt(), QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP") << "|Mount time for the file system" << Qt::endl;
	    out << "Write Time|" << QDateTime::fromSecsSinceEpoch(fsinfo->value("writetime").toInt(), QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP") << "|Write time for the file system" << Qt::endl;
	    out << "Last Check Time|" << QDateTime::fromSecsSinceEpoch(fsinfo->value("lastcheck").toInt(), QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP") << "|Last check time for the file system" << Qt::endl;
	    out << "Current State|" << fsinfo->value("state").toString() << "Condition of the file system at last unmount" << Qt::endl;
	    out << "Compatible Features|" << fsinfo->value("compatstr").toString() << "File System Compatible Feature Set" << Qt::endl;
	    out << "Incompatible Features|" << fsinfo->value("incompatstr").toString() << "File System Incompatible Feature Set" << Qt::endl;
	    out << "Read Only Compatible Features|" << fsinfo->value("readonlystr").toString() << "File System Read Only Compatible Feature Set" << Qt::endl;
	}
        out.flush();
        fspropfile.close();
    }
    //qDebug() << "fsinfo type:" << fsinfo->value("typestr");
}
*/
void ProcessForensicImage(ForImg* curimg)
{
    qInfo() << "Parsing Forensic Image:" << curimg->ImgPath();
    isignals->StatUp("Parsing Forensic Image");
    qDebug() << "imgpath at start of parsingforensicimage:" << curimg->ImgPath();
    qDebug() << "mount path at start of parseforensicimage:" << curimg->MountPath();
    qDebug() << "imgsize:" << curimg->Size();
    QList<QVariant> nodedata;
    nodedata.clear();
    nodedata << curimg->ImgPath().split("/").last() << "0" << QString::number(curimg->Size()) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + curimg->MountPath().split("/").last().split("-e").last());
    /*
     * NEED TO WRITE THIS AS I GO.... AND CLEAR STRING AS I GO
     *
     * MIGHT BE BETTER TO GENERATE THE PREVIEW REPORT AFTER ALL THIS WITHIN THE "UPDATESTATUS()" FUNCTION
     *
    QString reportstring = "";
    reportstring += "<div id='e" + curimg->MountPath().split("/").last().split("-e").last() + "'><table width='98%'>";
    reportstring += "<tr><th colspan='2'>Evidence Item (E" + curimg->MountPath().split("/").last().split("-e").last() + "):" + curimg->ImgPath() + "</th></tr>";
    reportstring += "<tr class='odd vtop'><td>Image Size:</td><td>" + QString::number(curimg->Size()) + " bytes</td></tr>";
    EvidenceReportData tmpdata;
    tmpdata.evidid = evidcnt;
    tmpdata.evidname = evidencename;
    tmpdata.evidcontent = reportstring;
    evidrepdatalist.append(tmpdata);
    AddELinkItem(evidrepdatalist.at(i).evidid, evidrepdatalist.at(i).evidname);
    AddEvidItem(evidrepdatalist.at(i).evidcontent);
    */
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

    /*
    qDebug() << "Starting Parse Volume";
    ParseVolume(curimg, imgsize, &pofflist, &psizelist, &fsinfolist);
    */
    qInfo() << "Reading Partition Table...";
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
	    if(exfatstr.startsWith("NTFS") || exfatstr == "EXFAT" || fatstr == "FAT12" || fatstr == "FAT16" || fat32str == "FAT32") // NTFS | EXFAT | FAT12 | FAT16 | FAT32 W/O PARTITION TABLE
            {
		ParsePartition(curimg, 0, curimg->Size()/512, 0, 1);
            }
            else // MBR
            {
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
    else // NO PARTITION MAP, JUST A FS AT ROOT OF IMAGE
    {
	ParsePartition(curimg, 0, curimg->Size()/512, 0, 1);
        //qDebug() << "partition signature not found correctly";
    }
    //FindPartitions(curimg, &pofflist, &psizelist);
}

void ParsePartition(ForImg* curimg, uint32_t cursectoroffset, uint32_t cursectorsize, uint8_t ptreecnt, uint8_t allocstatus)
{
    // ADD THE UNALLOCATED/ALLOCATED PARTITION
    QDir dir; // current partition directory
    QFile pstatfile; // current statfile
    QString pname = "";
    QTextStream out;
    dir.mkpath(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/");
    QList<QVariant> nodedata;
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
	out << pname << "," << QString::number((qint64)cursectoroffset*512) << "," << QString::number((qint64)cursectorsize*512) << ",0," << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
	out.flush();
	pstatfile.close();
    }
    //qDebug() << "partition id:" << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
    /*
    reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(ptreecnt) + "):</td><td>UNALLOCATED</td></tr>";
    reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(ptreecnt) + "):</td><td>" + fsinfolist.at(i).value("vollabel").toString() + " [" + fsinfolist.at(i).value("typestr").toString() + "]</td></tr>";
    */
    //partitionlist.append("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + ": UNALLOCATED");
    nodedata.clear();
    if(allocstatus == 0)
	nodedata << "UNALLOCATED";
    else if(allocstatus == 1)
	nodedata << pname;
    nodedata << "0" << QString::number((qint64)cursectorsize*512) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
    mutex.lock();
    treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last()), -1, 0);
    mutex.unlock();
    // FILE CARVING DIRECTORIES
    nodedata.clear();
    if(allocstatus == 1)
        ParseDirectoryStructure(curimg, cursectoroffset, ptreecnt);
    nodedata << QByteArray("carved validated").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-cv");
    mutex.lock();
    treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt)), 11, 0);
    mutex.unlock();
    nodedata.clear();
    nodedata << QByteArray("carved unvalidated").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-cu");
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
    // WILL WRITE FILE SYSTEM INFORMATION IN THIS FUNCTION AND ONLY RETURN THE QSTRING(FILESYSTEMNAME,FILESYSTEMTYPE) TO BE USED BY THE PARTITION
    if(winsig == 0xaa55) // FAT OR NTFS
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
                    out << "File System Type Int|1|Internal File System Type represented as an integer." << Qt::endl;
	            out << "File System Type|FAT12|File System Type String." << Qt::endl;
                }
                else if(fatstr == "FAT16")
                {
                    out << "File System Type Int|2|Internal File System Type represented as an integer." << Qt::endl;
	            out << "File System Type|FAT16|File System Type String." << Qt::endl;
                }
            }
            else if(fat32str == "FAT32")
            {
                out << "File System Type Int|3|Internal File System Type represented as an integer." << Qt::endl;
	        out << "File System Type|FAT32|File System Type String." << Qt::endl;
            }
            else if(exfatstr == "EXFAT")
            {
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
                curoffset += qFromLittleEndian<uint16_t>(curimg->ReadContent(mftoffset + curoffset + 32, 2)); // move the current offset to the start of the runlist
                int runlistinc = 0;
                QStringList runlist;
                runlist.clear();
                uint mftsize = 0;
                while(curoffset < 1024) // curoffset < mftentrybytes
                {
                    if(qFromLittleEndian<uint8_t>(curimg->ReadContent(mftoffset + curoffset, 1)) > 0)
                    {
                        int runlengthbytes = QString(QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(mftoffset + curoffset, 1)), 16).at(1)).toInt();
                        int runlengthoffset = QString(QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(mftoffset + curoffset, 1)), 16).at(0)).toInt();
                        if(runlengthbytes == 0 && runlengthoffset == 0)
                            break;
                        curoffset++;
                        uint runlength = 0;
                        int runoffset = 0;
                        if(runlengthbytes == 1)
                            runlength = qFromLittleEndian<uint8_t>(curimg->ReadContent(mftoffset + curoffset, runlengthbytes));
                        else
                            runlength = qFromLittleEndian<uint>(curimg->ReadContent(mftoffset + curoffset, runlengthbytes));
                        if(runlengthoffset == 1)
                            runoffset = qFromLittleEndian<int8_t>(curimg->ReadContent(mftoffset + curoffset + runlengthbytes, runlengthoffset));
                        else
                        {
                            QString runstr = "";
                            for(int i=runlengthoffset-1; i >= 0; i--)
                                runstr += QString(curimg->ReadContent(mftoffset + curoffset + runlengthbytes + i, 1).toHex());
                            runoffset = runstr.toInt(nullptr, 16);
                        }
                        if(runlistinc > 0)
                        {
                            if(runlistinc > 1 && QString::number(runoffset, 16).right(1).toInt() == 1)
                                runoffset = runoffset - 0xffff - 1;
                            runoffset = runoffset + runlist.at(runlistinc-1).split(",").at(0).toUInt();
                        }
                        runlist.append(QString::number(runoffset) + "," + QString::number(runlength));
                        mftsize += runlength;
                        runlistinc++;
                        curoffset += runlengthbytes + runlengthoffset;
                    }
                    else
                        break;
                }
                QString runliststr = "";
                for(int i=0; i < runlist.count(); i++)
                    runliststr += QString::number(curstartsector*512 + runlist.at(i).split(",").at(0).toULongLong() * bytespercluster) + "," + QString::number(runlist.at(i).split(",").at(1).toULongLong() * bytespercluster);
	        out << "MFT Layout|" << runliststr << "|Layout for the MFT in starting offset, size; format" << Qt::endl;
                out << "Max MFT Entries|" << QString::number((mftsize * bytespercluster)/1024) << "|Max MFT Entries allowed in the MFT" << Qt::endl;
                runliststr = "";
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
        partitionname += QString::fromStdString(curimg->ReadContent(curstartsector*512 + 1144, 16).toStdString());
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
            out << "EXT4";
            partitionname += " [EXT4]";
        }
        else if(((compatflags & 0x00000004UL) != 0) || ((incompatflags & 0x0000000cUL) != 0))
        {
            out << "EXT3";
            partitionname += " [EXT3]";
        }
        else
        {
            out << "EXT2";
            partitionname += " [EXT2]";
        }
        out << "|File system type string." << Qt::endl;
	// NEED TO IMPLEMENT THESE FS PROPERTIES SO I CAN USE THEM WHEN I PARSE THE EXT2/3/4 FS
	/*
        fsinfo.insert("partoffset", QVariant((qulonglong)(512 * partoffset)));
        fsinfo.insert("fsinodecnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1024, 4))));
        fsinfo.insert("fsblockcnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1028, 4))));
        fsinfo.insert("blockgroup0startblk", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1044, 4))));
        fsinfo.insert("blocksize", QVariant(1024 * pow(2, qFromLittleEndian<uint32_t>(partbuf.mid(1048, 4)))));
        //qDebug() << "block size:" << fsinfo.value("blocksize").toUInt();
        fsinfo.insert("fragsize", QVariant(1024 * pow(2, qFromLittleEndian<uint32_t>(partbuf.mid(1052, 4)))));
        fsinfo.insert("blockgroupblockcnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1056, 4))));
        fsinfo.insert("blockgroupfragcnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1060, 4))));
        //qDebug() << "fs blockcount:" << fsinfo.value("fsblockcnt").toUInt();
        //qDebug() << "block group block count:" << fsinfo.value("blockgroupblockcnt").toUInt();
        fsinfo.insert("blockgroupinodecnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1064, 4))));
	uint32_t creatoros = qFromLittleEndian<uint32_t>(partbuf.mid(1096, 4));
	if(creatoros == 0x00)
	    fsinfo.insert("creator", QVariant("Linux"));
	else if(creatoros == 0x03)
	    fsinfo.insert("creator", QVariant("FreeBSD"));
        //qDebug() << "block group inode count:" << fsinfo.value("blockgroupinodecnt").toUInt();
        fsinfo.insert("inodesize", QVariant(qFromLittleEndian<uint16_t>(partbuf.mid(1112, 2))));
        fsinfo.insert("vollabel", QString::fromStdString(partbuf.mid(1144, 16).toStdString()));
	fsinfo.insert("lastmountedpath", QString::fromStdString(partbuf.mid(1160, 64).toStdString()));
        //qDebug() << "INODE SIZE ACCORDING TO SUPERBLOCK:" << fsinfo.value("inodesize").toUInt();
        //qDebug() << "compatflags:" << fsinfo.value("compatflags").toUInt() << "incompatflags:" << fsinfo.value("incompatflags").toUInt() << "readonlyflags:" << fsinfo.value("readonlyflags").toUInt();
        //if(fsinfo.value("incompatflags").toUInt() & 0x40)
        //    qDebug() << "fs uses extents.";
        uint32_t blockgroupcount = fsinfo.value("fsblockcnt").toUInt() / fsinfo.value("blockgroupblockcnt").toUInt();
        int blockgroupcntrem = fsinfo.value("fsblockcnt").toUInt() % fsinfo.value("blockgroupblockcnt").toUInt();
        if(blockgroupcntrem > 0)
            blockgroupcount++;
        qDebug() << "block group count:" << blockgroupcount;
        if(blockgroupcount == 0)
            blockgroupcount = 1;
        QString inodeaddresstable = "";
        for(uint i=0; i < blockgroupcount; i++)
        {
            //qDebug() << "inode address table offset:" << fsinfo.value("blocksize").toUInt() + i*32 + 8;
            if(fsinfo.value("blocksize").toUInt() == 1024)
                inodeaddresstable += QString::number(qFromLittleEndian<uint32_t>(partbuf.mid(2*fsinfo.value("blocksize").toUInt() + i*32 + 8, 4))) + ",";
            else
                inodeaddresstable += QString::number(qFromLittleEndian<uint32_t>(partbuf.mid(fsinfo.value("blocksize").toUInt() + i*32 + 8, 4))) + ",";
        }
        //qDebug() << "inodeaddresstable:" << inodeaddresstable;
        fsinfo.insert("inodeaddresstable", QVariant(inodeaddresstable));
        //qDebug() << "blocks for group descriptor table:" << (32 * (fsinfo.value("fsblockcnt").toUInt() / fsinfo.value("blockgroupblockcnt").toUInt())) / fsinfo.value("blocksize").toUInt();
        //if(fsinfo.value("blockgroupinodecnt").toUInt() > 2)
        //    fsinfo.insert("rootinodetablestartingaddress", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(2056, 4))));
        //else
        //    fsinfo.insert("rootinodetablestartingaddress", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(2088, 4))));
	 */ 
    }
    else if(apfsig == "NXSB") // APFS Container
    {
        out << "File System Type Int|7|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|APFS|File System Type String." << Qt::endl;
        partitionname += " [APFS]";
        /*
        fsinfo.insert("type", QVariant(7));
        fsinfo.insert("typestr", QVariant("APFS"));
        fsinfo.insert("blocksize", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(36, 4))));
        qDebug() << "blocksize:" << fsinfo.value("blocksize").toUInt();
        fsinfo.insert("blockcount", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(40, 8))));
	fsinfo.insert("containeruuid", QVariant(QString::fromStdString(partbuf.mid(72, 16).toHex().toStdString())));
	qDebug() << "container uuid:" << (fsinfo.value("containeruuid").toString().left(8) + "-" + fsinfo.value("containeruuid").toString().mid(8, 4) + "-" + fsinfo.value("containeruuid").toString().mid(12, 4) + "-" + fsinfo.value("containeruuid").toString().mid(16, 4) + "-" + fsinfo.value("containeruuid").toString().right(12));
	fsinfo.insert("nextoid", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(88, 8))));
	qDebug() << "nextoid:" << fsinfo.value("nextoid").toUInt();
	fsinfo.insert("nextxid", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(96, 8))));
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
        if(hfssig == "H+")
        {
            partitionname += " [HFS+]";
            out << "File System Type|HFS+|File System Type String." << Qt::endl;
        }
        else if(hfssig == "HX")
        {
            partitionname += " [HFSX]";
            out << "File System Type|HFSX|File System Type String." << Qt::endl;
        }
        /*
        int curoffset = 1024;
        fsinfo.insert("type", QVariant(8));
        if(hfssig == "H+")
            fsinfo.insert("typestr", QVariant("HFS+"));
        else if(hfssig == "HX")
            fsinfo.insert("typestr", QVariant("HFSX"));
        fsinfo.insert("createdate", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 16, 4))));
        fsinfo.insert("modifydate", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 20, 4))));
        fsinfo.insert("backupdate", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 24, 4))));
        fsinfo.insert("filecount", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 32, 4))));
        fsinfo.insert("foldercount", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 36, 4))));
        fsinfo.insert("blocksize", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 40, 4))));
        fsinfo.insert("totalblocks", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 44, 4))));
        qDebug() << "block size:" << fsinfo.value("blocksize").toUInt();
        fsinfo.insert("cataloglogsize", QVariant(qFromBigEndian<qulonglong>(partbuf.mid(curoffset + 272, 8))));
        fsinfo.insert("catalogtotalblocks", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 284, 4))));
        fsinfo.insert("catalogextent0startblk", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 288, 4))));
        fsinfo.insert("catalogextent0blkcnt", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(curoffset + 292, 4))));
        qDebug() << "catalog startblk:" << fsinfo.value("catalogextent0startblk").toUInt();
        qDebug() << "catalog blkcnt:" << fsinfo.value("catalogextent0blkcnt").toUInt();
        // 995344 is where the volume name is stored.... in sector 1944
        */
    }
    else if(xfssig == "XFSB") // XFS
    {
        out << "File System Type Int|9|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|XFS|File System Type String." << Qt::endl;
        partitionname += QString::fromStdString(curimg->ReadContent(curstartsector*512 + 108, 12).toStdString());
        out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        partitionname += " [XFS]";
        /*
        fsinfo.insert("type", QVariant(9));
        fsinfo.insert("typestr", QVariant("XFS"));
        fsinfo.insert("blocksize", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(4, 4))));
        fsinfo.insert("dblocks", QVariant(qFromBigEndian<qulonglong>(partbuf.mid(8, 8))));
        fsinfo.insert("rootinode", QVariant(qFromBigEndian<qulonglong>(partbuf.mid(56, 8))));
        fsinfo.insert("agblocks", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(84, 4))));
        fsinfo.insert("agcount", QVariant(qFromBigEndian<uint32_t>(partbuf.mid(88, 4))));
        fsinfo.insert("sectorsize", QVariant(qFromBigEndian<uint16_t>(partbuf.mid(102, 2))));
        fsinfo.insert("inodesize", QVariant(qFromBigEndian<uint16_t>(partbuf.mid(104, 2))));
        fsinfo.insert("inodesperblock", QVariant(qFromBigEndian<uint16_t>(partbuf.mid(106, 2))));
        fsinfo.insert("vollabel", QVariant(QString::fromStdString(partbuf.mid(108, 12).toStdString())));
        qDebug() << "vollabel:" << fsinfo.value("vollabel").toString();
        qDebug() << "block size:" << fsinfo.value("blocksize").toUInt() << "root inode:" << fsinfo.value("rootinode").toUInt() << "sectorsize:" << fsinfo.value("sectorsize").toUInt() << "inodesize:" << fsinfo.value("inodesize").toUInt() << "inodesperblock:" << fsinfo.value("inodesperblock").toUInt();
        */
    }
    else if(btrsig == "_BHRfS_M") // BTRFS
    {
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
        out << "File System Type Int|11|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|BITLOCKER|File System Type String." << Qt::endl;
        partitionname += QString::fromStdString(curimg->ReadContent(curstartsector*512 + 118, qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 112, 2)) - 8).toStdString());
        out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        partitionname += " [BITLOCKER]";
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
        out << "File System Type Int|12|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|BFS|File System Type String." << Qt::endl;
        partitionname += QString::fromStdString(curimg->ReadContent(curstartsector*512 + 512, 32).toStdString());
        out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        partitionname += " [BFS]";
        /*
        fsinfo.insert("vollabel", QVariant(QString::fromStdString(partbuf.mid(512, 32).toStdString())));
        fsinfo.insert("fsbyteorder", QVariant(qFromLittleEndian<int32_t>(partbuf.mid(548, 4))));
        //qDebug() << "fsbyteorder:" << QString::number(fsinfo.value("fsbyteorder").toInt(), 16);
        */
    }
    else if(f2fsig == 0xf2f52010) // F2FS
    {
        out << "File System Type Int|13|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|F2FS|File System Type String." << Qt::endl;
        //out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        partitionname += " [F2FS]";
    }
    else if(isosig == "CD001" && udfsig != "BEA01") // ISO9660
    {
        out << "File System Type Int|14|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|ISO9660|File System Type String." << Qt::endl;
        //out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        partitionname += " [ISO9660]";
    }
    else if(isosig == "CD001" && udfsig == "BEA01") // UDF
    {
        out << "File System Type Int|15|Internal File System Type represented as an integer." << Qt::endl;
	out << "File System Type|UDF|File System Type String." << Qt::endl;
        //out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        partitionname += " [UDF]";
    }
    else if(hfssig == "BD") // legacy HFS
    {
        out << "File System Type Int|16|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|HFS|File System Type String." << Qt::endl;
        //out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        partitionname += " [HFS]";
    }
    else if(refsig == 0x5265465300000000) // ReFS
    {
        out << "File System Type Int|18|Internal File System Type represented as an integer." << Qt::endl;
        out << "File System Type|RES|File System Type String." << Qt::endl;
        //out << "Volume Label|" << partitionname << "|Volume Label for the file system." << Qt::endl;
        partitionname += " [REFS]";
    }
    // need to implement iso-14, udf-15, hfs-16, zfs-17, refs-18
    out.flush();
    propfile.close();

    return partitionname;
}

void GetNextCluster(ForImg* curimg, uint32_t clusternum, uint8_t fstype, qulonglong fatoffset, QList<uint>* clusterlist)
{
    uint32_t curcluster = 0;
    int fatbyte1 = 0;
    if(fstype == 1) // FAT12
    {
        fatbyte1 = clusternum + (clusternum / 2);
        if(clusternum & 0x0001) // ODD
            curcluster = (qFromLittleEndian<uint16_t>(curimg->ReadContent(fatoffset + fatbyte1, 2)) >> 4);
        else // EVEN
            curcluster = (qFromLittleEndian<uint16_t>(curimg->ReadContent(fatoffset + fatbyte1, 2)) & 0x0FFF);
        //if(curcluster != 0x0FF7)
        //    clusterlist->append(curcluster);
        //if(curcluster < 0x0FF8 && curcluster >= 2)
        if(curcluster < 0x0FF7 && curcluster >= 2)
        {
            clusterlist->append(curcluster);
            GetNextCluster(curimg, curcluster, fstype, fatoffset, clusterlist);
        }
    }
    else if(fstype == 2) // FAT16
    {
        if(clusternum >= 2)
        {
            fatbyte1 = clusternum * 2;
            curcluster = qFromLittleEndian<uint16_t>(curimg->ReadContent(fatoffset + fatbyte1, 2));
            //if(curcluster != 0xFFF7)
            //    clusterlist->append(curcluster);
            //if(curcluster < 0xFFF8 && curcluster >= 2)
            if(curcluster < 0xFFF7 && curcluster >= 2)
            {
                clusterlist->append(curcluster);
                GetNextCluster(curimg, curcluster, fstype, fatoffset, clusterlist);
            }
        }
    }
    else if(fstype == 3) // FAT32
    {
        if(clusternum >= 2)
        {
            fatbyte1 = clusternum * 4;
            curcluster = (qFromLittleEndian<uint32_t>(curimg->ReadContent(fatoffset + fatbyte1, 4)) & 0x0FFFFFFF);
	    //if(curcluster != 0x0FFFFFF7)
		//clusterlist->append(curcluster);
	    if(curcluster < 0x0FFFFFF7 && curcluster >= 2)
            {
                clusterlist->append(curcluster);
		GetNextCluster(curimg, curcluster, fstype, fatoffset, clusterlist);
            }
        }
    }
    else if(fstype == 4) // EXFAT
    {
        if(clusternum >= 2)
        {
            fatbyte1 = clusternum * 4;
            curcluster = qFromLittleEndian<uint32_t>(curimg->ReadContent(fatoffset + fatbyte1, 4));
	    //if(curcluster != 0xFFFFFFF7)
		//clusterlist->append(curcluster);
	    if(curcluster < 0xFFFFFFF7 && curcluster >= 2)
            {
                clusterlist->append(curcluster);
		GetNextCluster(curimg, curcluster, fstype, fatoffset, clusterlist);
            }
        }
    }
}

QString ConvertBlocksToExtents(QList<uint> blocklist, uint blocksize, qulonglong rootdiroffset)
{
    // FirstSectorOfCluster = ((N-2) * sectorspercluster) + firstdatasector [rootdirstart];
    //QString rootdirlayout = QString::number(rootdiroffset + ((rootdircluster - 2) * sectorspercluster * bytespersector)) + "," + QString::number(sectorspercluster * bytespersector) + ";";
    QString extentstring = "";
    int blkcnt = 1;
    uint startvalue = blocklist.at(0);
    for(int i=1; i < blocklist.count(); i++)
    {
        uint oldvalue = blocklist.at(i-1);
        uint newvalue = blocklist.at(i);
        if(newvalue - oldvalue == 1)
            blkcnt++;
        else
        {
            if(rootdiroffset > 0)
                extentstring += QString::number(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + QString::number(blkcnt * blocksize) + ";";
            else
                extentstring += QString::number(startvalue * blocksize) + "," + QString::number(blkcnt * blocksize) + ";";
            startvalue = blocklist.at(i);
            blkcnt = 1;
        }
        if(i == blocklist.count() - 1)
        {
            if(rootdiroffset > 0)
                extentstring += QString::number(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + QString::number(blkcnt * blocksize) + ";";
            else
                extentstring += QString::number(startvalue * blocksize) + "," + QString::number(blkcnt * blocksize) + ";";
            startvalue = blocklist.at(i);
            blkcnt = 1;
        }
    }
    if(blocklist.count() == 1)
    {
        if(rootdiroffset > 0)
            extentstring += QString::number(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + QString::number(blkcnt * blocksize) + ";";
        else
            extentstring += QString::number(startvalue * blocksize) + "," + QString::number(blkcnt * blocksize) + ";";
    }

    return extentstring;
}

qulonglong AddVirtualFileSystemFiles(ForImg* curimg, uint8_t ptreecnt, uint8_t fatcount, qulonglong fatsize, qulonglong curinode)
{
    QList<QVariant> nodedata;
    nodedata.clear();
    nodedata << QByteArray("$MBR").toBase64() << QByteArray("/").toBase64() << "512" << "0" << "0" << "0" << "0" << "0" << "System File" << "Master Boot Record" << "0" << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(curinode));
    mutex.lock();
    treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt)), 10, 0);
    mutex.unlock();
    nodedata.clear();
    curinode++;
    for(int i=0; i < fatcount; i++)
    {
	nodedata.clear();
	nodedata << QByteArray(QString("$FAT" + QString::number(i+1)).toUtf8()).toBase64() << QByteArray("/").toBase64() << fatsize << "0" << "0" << "0" << "0" << "0" << "System File" << "File Allocation Table" << "0" << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(curinode));
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
    uint8_t fstype = 0;
    uint8_t fatcount = 0;
    uint32_t fatsize = 0;
    uint16_t bytespersector = 0;

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
        }
        propfile.close();
    }
    if(fstype > 0 and fstype < 4) // FAT12 || FAT16 || FAT32
    {
	qulonglong curinode = 0;
	curinode = ParseFatDirectory(curimg, curstartsector, ptreecnt, 0, 0, "", "");
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
        //ParseNtfsDirectory(curimg, curstartsector, ptreecnt);
    }
    else if(fstype == 6) // EXT2/3/4
    {
	quint64 curinode = 0;
	curinode = ParseExtDirectory(curimg, curstartsector, ptreecnt, 2, 0, "", "");
    }
    //qDebug() << "fs type:" << fstype << "bps:" << bytespersector << "fo:" << fatoffset << "fs:" << fatsize << "rdl:" << rootdirlayout;
}

qulonglong ParseFatDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, uint8_t orphandirexists, qulonglong parinode, QString parfilename, QString dirlayout)
{
    qulonglong inodecnt = 0;
    //uint32_t fatsize = 0;
    qulonglong fatoffset = 0;
    uint16_t bytespersector = 0;
    uint8_t sectorspercluster = 0;
    uint8_t fstype = 0;
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
            //    fatsize = line.split("|").at(1).toUInt();
            else if(line.startsWith("Cluster Area Start|"))
                clusterareastart = line.split("|").at(1).toULongLong();
	    else if(line.startsWith("File System Type Int|"))
		fstype = line.split("|").at(1).toUInt();
        }
        propfile.close();
    }
    if(!dirlayout.isEmpty())
	rootdirlayout = dirlayout;
    //qDebug() << "rootdirlayout:" << rootdirlayout;
    //qDebug() << "bps:" << bytespersector << "fo:" << fatoffset << "fs:" << fatsize << "rdl:" << rootdirlayout;
    for(int i=0; i < rootdirlayout.split(";", Qt::SkipEmptyParts).count(); i++)
    {
        //qDebug() << "root dir entry:" << i;
	qulonglong rootdiroffset = 0;
	qulonglong rootdirsize = 0;
	if(i == 0)
	{
	    if(dirlayout.isEmpty()) // root directory
	    {
	        rootdiroffset = rootdirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong();
		rootdirsize = rootdirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong();
	    }
	    else // sub directory
	    {
		rootdiroffset = rootdirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong() + 64; // skip . and .. directories
		rootdirsize = rootdirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong() - 64; // adjust read size for the 64 byte skip
	    }
	}
        uint rootdirentrycount = rootdirsize / 32;
        //qDebug() << "current rootdirentrycount:" << rootdirentrycount;
	if(parinode == 0)
	    inodecnt = 0;
	else
	    inodecnt = parinode + 1;
	//qDebug() << "inodecnt at start of parent comparison:" << inodecnt;
        QString longnamestring = "";
        for(uint j=0; j < rootdirentrycount; j++)
        {
	    QString longname = "";
            QTextStream out;
            QFile fileprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
            if(!fileprop.isOpen())
                fileprop.open(QIODevice::Append | QIODevice::Text);
            out.setDevice(&fileprop);

            uint8_t firstchar = qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32, 1));
            if(firstchar == 0x00) // entry is free and all remaining are free
                break;
            uint8_t fileattr = qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 11, 1));
            if(fileattr != 0x0f && fileattr != 0x00 && fileattr != 0x3f) // need to process differently // 0x3f is ATTR_LONG_NAME_MASK which is a long name entry sub component
            {
                if(!longnamestring.isEmpty())
                {
                    out << "Long Name|" << longnamestring << "|Long name for the file." << Qt::endl;
		    longname = longnamestring;
                    longnamestring = "";
                }
                else
                    out << "Long Name| |Long name for the file." << Qt::endl;
                out << "File Attributes|";
                if(fileattr & 0x01)
                    out << "Read Only,";
                if(fileattr & 0x02)
                    out << "Hidden File,";
                if(fileattr & 0x04)
                    out << "System File,";
                if(fileattr & 0x08)
                    out << "Volume ID,";
                if(fileattr & 0x10)
                    out << "SubDirectory,";
                if(fileattr & 0x20)
                    out << "Archive File,";
                out << "|File attributes." << Qt::endl;
                uint8_t isdeleted = 0;
                if(firstchar == 0xe5 || firstchar == 0x05) // was allocated but now free
                    isdeleted = 1;
                QString restname = QString::fromStdString(curimg->ReadContent(rootdiroffset + j*32 + 1, 7).toStdString()).replace(" ", "");
                QString extname = QString::fromStdString(curimg->ReadContent(rootdiroffset + j*32 + 8, 3).toStdString()).replace(" ", "");
                QString aliasname = QString(char(firstchar));
                out << "Alias Name|";
                if(extname.count() > 0)
                {
                    aliasname += QString(restname.toUtf8() + "." + extname.toUtf8());
                }
                else
                {
                    aliasname += QString(restname.toUtf8());
                }
                out << aliasname << "|8.3 file name." << Qt::endl;
		//uint8_t createtenth = rootdirbuf.at(i*32 + 13); // NOT GOING TO USE RIGHT NOW...
                qint64 createdate = ConvertDosTimeToUnixTime(qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 15, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 14, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 17, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 16, 1)));
                qint64 accessdate = ConvertDosTimeToUnixTime(0x00, 0x00, qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 19, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 18, 1)));
                qint64 modifydate = ConvertDosTimeToUnixTime(qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 23, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 22, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 25, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 24, 1)));
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
                clusterlist.clear();
		uint32_t logicalsize = 0;
		uint8_t itemtype = 0;
		if(fileattr & 0x10) // sub directory attribute
		{
		    if(firstchar == 0xe5 || firstchar == 0x05) // deleted directory
			itemtype = 2;
		    else // directory
			itemtype = 3;
                    qulonglong curdirsize = 0;
                    int lastdirentry = 0;
                    int curdircnt = 0;
                    for(int k = 0; k < layout.split(";", Qt::SkipEmptyParts).count(); k++)
                    {
                        curdirsize += layout.split(";").at(k).split(",").at(1).toULongLong();
                    }
                    for(int k=0; k < layout.split(";", Qt::SkipEmptyParts).count(); k++)
                    {
                        for(uint m=0; m < curdirsize / 32; m++)
                        {
                            uint8_t curfirstchar = qFromLittleEndian<uint8_t>(curimg->ReadContent(layout.split(";").at(k).split(",").at(0).toULongLong() + m*32, 1));
                            if(curfirstchar == 0x00) // entry is free and all remaining are free
                            {
                                lastdirentry = curdircnt;
                                break;
                            }
                            curdircnt++;
                        }
                    }
                    logicalsize = lastdirentry * 32;
		}
		else
		{
		    if(firstchar == 0xe5 || firstchar == 0x05) // deleted file
			itemtype = 4;
		    else
			itemtype = 5;
		    logicalsize = qFromLittleEndian<uint32_t>(curimg->ReadContent(rootdiroffset + j*32 + 28, 4));
		}
		//qDebug() << "logicalsize:" << logicalsize;
		// ADD FILE INFO TO THE NODE TREE...
		QString filepath = "";
		QString filename = "";
		if(longname.isEmpty())
		    filename = aliasname;
		else
		    filename = longname;
		QList<QVariant> nodedata;
		nodedata.clear();
		nodedata << QByteArray(filename.toStdString().c_str()).toBase64();
		if(parfilename.isEmpty())
		    filepath = "/";
		else
		    filepath = parfilename;
		nodedata << QByteArray(filepath.toUtf8()).toBase64() << logicalsize << createdate << accessdate << modifydate << 0 << 0;
		if(logicalsize > 0) // Get Category/Signature
		{
		    if(itemtype == 3 && isdeleted == 0)
			nodedata << "Directory" << "Directory"; // category << signature
		    else
		    {
			QString catsig = GenerateCategorySignature(curimg, filename, layout.split(";").at(0).split(",").at(0).toULongLong());
			//qDebug() << filename.left(20) << catsig;
                        nodedata << catsig.split("/").first() << catsig.split("/").last();
		    }
		}
                else
                    nodedata << "Empty" << "Zero File";
                nodedata << "0" << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt));
                QString parentstr = "";
                if(parinode == 0)
                    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
                else
                    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(parinode));
                mutex.lock();
                treenodemodel->AddNode(nodedata, parentstr, itemtype, isdeleted);
                mutex.unlock();
                if(nodedata.at(11).toString().split("-").count() == 3)
                {
                    listeditems.append(nodedata.at(11).toString());
                    filesfound++;
                    isignals->ProgUpd();
                }
                inodecnt++;
                out.flush();
                fileprop.close();
                nodedata.clear();
		//qDebug() << "inodecnt:" << inodecnt << filename << layout;
                if(fileattr & 0x10 && logicalsize > 0) // sub directory
                {
                    if(firstchar != 0xe5 && firstchar != 0x05) // not deleted
			inodecnt = ParseFatDirectory(curimg, curstartsector, ptreecnt, orphandirexists, inodecnt-1, QString(filepath + filename + "/"), layout);
                }
            }
            else if(fileattr == 0x0f || 0x3f) // long directory entry for succeeding short entry...
            {
                if(firstchar & 0x40)
                {
                    if(!longnamestring.isEmpty()) // orphan long entry
                    {
			QList<QVariant> nodedata;
			if(orphandirexists == 0) // orphans dir doesn't exists, so create it
			{
			    nodedata.clear();
			    nodedata << QByteArray("orphans").toBase64() << QByteArray("/").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-o");
			    mutex.lock();
			    treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt)), 11, 0);
			    mutex.unlock();
			    nodedata.clear();
			    orphandirexists = 1;
			}
			nodedata.clear();
			nodedata << QByteArray(longnamestring.toUtf8()).toBase64() << QByteArray("/orphans/").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "Empty" << "Empty File" << "0" << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt));
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
                QString l3 = "";
                QString l2 = "";
                QString l1 = "";
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 28, 2)) < 0xFFFF)
                    l3 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 28, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 30, 2)) < 0xFFFF)
                    l3 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 30, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 14, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 14, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 16, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 16, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 18, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 18, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 20, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 20, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 22, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 22, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 24, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 24, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 1, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 1, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 3, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 3, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 5, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 5, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 7, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 7, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 9, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 9, 2))));
                longnamestring.prepend(QString(l1 + l2 + l3).toUtf8());
            }
        }
    }
    return inodecnt;
}

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
		if(!parfilename.isEmpty())
		    filepath = parfilename;
		QList<QVariant> nodedata;
		nodedata.clear();
		nodedata << QByteArray(filename.toUtf8()).toBase64() << QByteArray(filepath.toUtf8()).toBase64() << (qulonglong)logicalsize << createdate << accessdate << modifydate << 0 << 0;
		if(logicalsize > 0) // Get Category/Signature
		{
		    if(itemtype == 3 && isdeleted == 0)
			nodedata << "Directory" << "Directory";
		    else
		    {
			QString catsig = GenerateCategorySignature(curimg, filename, layout.split(";").at(0).split(",").at(0).toULongLong());
			nodedata << catsig.split("/").first() << catsig.split("/").last();
		    }
		}
		else
		    nodedata << "Empty" << "Zero File";
		nodedata << "0" << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt));
		QString parentstr = "";
		if(parinode == 0)
		    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
		else
		    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(parinode));
		mutex.lock();
		treenodemodel->AddNode(nodedata, parentstr, itemtype, isdeleted);
		mutex.unlock();
		if(nodedata.at(11).toString().split("-").count() == 3)
		{
		    listeditems.append(nodedata.at(11).toString());
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
    uint8_t orphandirexists = 0;
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
		    QList<QVariant> nodedata;
		    if(orphandirexists == 0) // orphans dir doesn't exist yet, so create it
		    {
			nodedata.clear();
			nodedata << QByteArray("orphans").toBase64() << QByteArray("/").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-o");
			mutex.lock();
			treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt)), 11, 0);
			mutex.unlock();
			nodedata.clear();
			orphandirexists = 1;
		    }
		    nodedata.clear();
		    nodedata << QByteArray(filename.toUtf8()).toBase64() << QByteArray("/orphans/").toBase64() << (qulonglong)logicalsize << createdate << modifydate << accessdate << "0" << "0";
		    if(logicalsize > 0) // Get Category/Signature
		    {
			if(itemtype == 2)
			    nodedata << "Directory" << "Directory";
			else
			{
			    QString catsig = GenerateCategorySignature(curimg, filename, layout.split(";").at(0).split(",").at(0).toULongLong());
			    nodedata << catsig.split("/").first() << catsig.split("/").last();
			}
		    }
		    else
			nodedata << "Empty" << "Zero File";
		    nodedata << "0" << QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(curinode));
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

quint64 ParseExtDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, quint64 curextinode, quint64 parinode, QString parfilename, QString dirlayout)
{
    QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/prop");
    if(!propfile.isOpen())
	propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(propfile.isOpen())
    {
        while(!propfile.atEnd())
        {
            QString line = propfile.readLine();
	    /*
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
	    */
        }
        propfile.close();
    }
    quint64 inodecnt = 0;
    /*
	if(parinode == 0)
	    inodecnt = 0;
	else
	    inodecnt = parinode + 1;
    */
}


/*
 *    //qDebug() << "current inode:" << curinode;
    // DETERMINE WHICH BLOCK GROUP THE CUR INODE IS A PART OF.
    //qDebug() << "inode address table:" << fsinfo->value("inodeaddresstable").toString();
    QStringList blockgroups = fsinfo->value("inodeaddresstable").toString().split(",", Qt::SkipEmptyParts);
    qulonglong inodetablestartingblock = 0;
    int bgnumber = 0;
    qulonglong inodecnt = curicnt;

    for(int i=1; i <= blockgroups.count(); i++)
    {
        if(curinode < i*fsinfo->value("blockgroupinodecnt").toUInt())
        {
            inodetablestartingblock = blockgroups.at(i-1).toULongLong();
            bgnumber = i - 1;
            break;
        }
    }
    //qDebug() << "blockgroups:" << blockgroups;
    //qDebug() << "inode table starting block:" << inodetablestartingblock;

    QHash<QString, QVariant> fileinfo;
    fileinfo.clear();

    //QFile efile(estring);
    QByteArray direntrybuf;
    uint32_t inodeflags = 0;
    // I NEED TO DO THIS TO GET THE BLOCK LIST FOR THE CURRENT INODE SO I CAN GET IT'S CONTENT AND READ TEH DIRECTORY ENTRY, BUT THIS IS A GIVEN
    // ON SUB DIRECTORIES SINCE I WILL HAVE THE INODE'S LAYOUT AND CURBLOCKLIST, SO I CAN SKIP THIS PART WHEN THESE VALUES ARE KNOWN...
    if(parfileinfo != NULL)
    {
        direntrybuf.clear();
        curimg->open(QIODevice::ReadOnly);
        QStringList layoutlist = parfileinfo->value("layout").toString().split(";", Qt::SkipEmptyParts);
        for(int i=0; i < layoutlist.count(); i++)
        {
            curimg->seek(layoutlist.at(i).split(",").at(0).toUInt());
            direntrybuf.append(curimg->read(layoutlist.at(i).split(",").at(1).toUInt()));
        }
        curimg->close();
    }
    else
    {
        QByteArray inodetablebuf;
        inodetablebuf.clear();
        curimg->open(QIODevice::ReadOnly);
        curimg->seek(fsinfo->value("partoffset").toUInt() + (inodetablestartingblock * fsinfo->value("blocksize").toUInt()));
        inodetablebuf = curimg->read(fsinfo->value("inodesize").toUInt() * fsinfo->value("blockgroupinodecnt").toUInt());
        curimg->close();
        // NOW I HAVE THE INODE TABLE FOR THE CURRENT BLOCK GROUP. I CAN GO THE CURINODE's OFFSET and parse it's inode table entry to get the content for the directory entry...
        QStringList blkstrlist;
        blkstrlist.clear();
        //QList<uint32_t> blocklist;
        //blocklist.clear();
        qulonglong relcurinode = curinode - 1 - (bgnumber * fsinfo->value("blockgroupinodecnt").toUInt());
        inodeflags = qFromLittleEndian<uint32_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 32, 4));
	if((fsinfo->value("incompatflags").toUInt() & 0x40) && (inodeflags & 0x80000)) // FS USES EXTENTS && INODE USES EXTENTS
	{
            uint16_t extententries = qFromLittleEndian<uint16_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 42, 2));
            uint16_t extentdepth = qFromLittleEndian<uint16_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 46, 2));
            if(extentdepth == 0) // use ext4_extent
            {
                for(int i=0; i < extententries; i++)
                {
                    //qDebug() << "ee_block:" << qFromLittleEndian<uint32_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 52 + i*12, 4));
                    //qDebug() << "ee_length:" << qFromLittleEndian<uint16_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 56 + i*12, 2));
                    //qDebug() << "ee_start_hi:" << qFromLittleEndian<uint16_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 58 + i*12, 2));
                    //qDebug() << "ee_start_lo:" << qFromLittleEndian<uint32_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 60 + i*12, 4));
                    uint16_t blocklength = qFromLittleEndian<uint16_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 56 + i*12, 2));
                    uint16_t starthi = qFromLittleEndian<uint16_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 58 + i*12, 2));
                    uint32_t startlo = qFromLittleEndian<uint32_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 60 + i*12, 4));
                    uint64_t startblock = (((uint64_t)starthi >> 32) + startlo) * fsinfo->value("blocksize").toUInt();
                    //qDebug() << "startblock:" << startblock;
                    blkstrlist.append(QString::number(startblock) + "," + QString::number(blocklength * fsinfo->value("blocksize").toUInt()));
                }
            }
            else // use ext4_extent_idx
            {
		QList<uint32_t> leafnodes;
		leafnodes.clear();
		for(int i=0; i < extententries; i++)
		{
		    leafnodes.append(qFromLittleEndian<uint32_t>(inodetablebuf.mid(fsinfo->value("inodesize").toUInt() * relcurinode + 56 + i*12, 4)));
		}
		for(int i=0; i < leafnodes.count(); i++)
		{
		    QByteArray leafnode;
		    leafnode.clear();
                    curimg->open(QIODevice::ReadOnly);
                    curimg->seek(fsinfo->value("partoffset").toUInt() + (leafnodes.at(i) * fsinfo->value("blocksize").toUInt()));
                    leafnode = curimg->read(fsinfo->value("blockszie").toUInt());
                    curimg->close();
			uint16_t extententries = qFromLittleEndian<uint16_t>(leafnode.mid(2, 2));
			uint16_t extentdepth = qFromLittleEndian<uint16_t>(leafnode.mid(6, 2));
			if(extentdepth == 0) // use ext4_extent
			{
			    for(int j=0; j < extententries; j++)
			    {
				uint16_t blocklength = qFromLittleEndian<uint16_t>(leafnode.mid(16 + j*12, 2));
				uint16_t starthi = qFromLittleEndian<uint16_t>(leafnode.mid(18 + j*12, 2));
				uint32_t startlo = qFromLittleEndian<uint32_t>(leafnode.mid(20 + j*12, 4));
				uint64_t startblock = (((uint64_t)starthi >> 32) + startlo) * fsinfo->value("blocksize").toUInt();
				blkstrlist.append(QString::number(startblock) + "," + QString::number(blocklength * fsinfo->value("blocksize").toUInt()));
			    }
			}
			else // use ext4_extent_idx
			{
			    qDebug() << "repeat leafnode exercise here...";
			}
			//qDebug() << "leaf header:" << QString::number(qFromLittleEndian<uint16_t>(leafnode.mid(0, 2)), 16);
			//qDebug() << "extent entries:" << qFromLittleEndian<uint16_t>(leafnode.mid(2, 2));
			//qDebug() << "max extent entries:" << qFromLittleEndian<uint16_t>(leafnode.mid(4, 2));
			//qDebug() << "extent depth:" << qFromLittleEndian<uint16_t>(leafnode.mid(6, 2));
		    //}
		}
		//qDebug() << "extent header:" << QString::number(qFromLittleEndian<uint16_t>(curinodebuf.mid(40, 2)), 16);
		//qDebug() << "extent entries:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(42, 2));
		//qDebug() << "max extent entries:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(44, 2));
		//qDebug() << "extent depth:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(46, 2));
		
		//qDebug() << "ei_block:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(52, 4));
		//qDebug() << "ei_leaf_lo:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(56, 4));
		//qDebug() << "ei_leaf_hi:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(60, 2));
		//qDebug() << "use extent idx";
            }
	}
        else
        {
	    for(int i=0; i < 12; i++)
	    {
		uint32_t curdirectblock = qFromLittleEndian<uint32_t>(inodetablebuf.mid( fsinfo->value("inodesize").toUInt() * relcurinode + (40 + i*4), 4));
		if(curdirectblock > 0)
		{
		    blkstrlist.append(QString::number(curdirectblock * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
		    //blocklist.append(curdirectblock);
		}
	    }
	    //qDebug() << "current block list before i get the indirect block pointers.." << blocklist;
	    uint32_t singleindirect = qFromLittleEndian<uint32_t>(inodetablebuf.mid((relcurinode)*fsinfo->value("inodesize").toUInt() + 88, 4));
	    uint32_t doubleindirect = qFromLittleEndian<uint32_t>(inodetablebuf.mid((relcurinode)*fsinfo->value("inodesize").toUInt() + 92, 4));
	    uint32_t tripleindirect = qFromLittleEndian<uint32_t>(inodetablebuf.mid((relcurinode)*fsinfo->value("inodesize").toUInt() + 96, 4));
	    // NEED TO PARSE THESE BLOCKS TO ADD TO THE BLOCKLIST's TOTAL
	    if(singleindirect > 0)
	    {
                curimg->open(QIODevice::ReadOnly);
                curimg->seek(fsinfo->value("partoffset").toUInt() + (singleindirect * fsinfo->value("blocksize").toUInt()));
                QByteArray singlebuf = curimg->read(fsinfo->value("blocksize").toUInt());
                curimg->close();
		    for(int i=0; i < singlebuf.count() / 4; i++)
		    {
			uint32_t cursingledirect = qFromLittleEndian<uint32_t>(singlebuf.mid(i*4, 4));
			if(cursingledirect > 0)
			{
			    blkstrlist.append(QString::number(cursingledirect * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
			    //blocklist.append(cursingledirect);
			}
		    }
		//}
	    }
	    if(doubleindirect > 0)
	    {
		QList<uint32_t> sinlist;
		sinlist.clear();
                curimg->open(QIODevice::ReadOnly);
                curimg->seek(fsinfo->value("partoffset").toUInt() + (doubleindirect * fsinfo->value("blocksize").toUInt()));
                QByteArray doublebuf = curimg->read(fsinfo->value("blocksize").toUInt());
                curimg->close();
		    for(int i=0; i < doublebuf.count() / 4; i++)
		    {
			uint32_t sindirect = qFromLittleEndian<uint32_t>(doublebuf.mid(i*4, 4));
			if(sindirect > 0)
			    sinlist.append(sindirect);
		    }
		//}
		for(int i=0; i < sinlist.count(); i++)
		{
                    curimg->open(QIODevice::ReadOnly);
                    curimg->seek(fsinfo->value("partoffset").toUInt() + (sinlist.at(i) * fsinfo->value("blocksize").toUInt()));
                    QByteArray sinbuf = curimg->read(fsinfo->value("blocksize").toUInt());
                    curimg->close();
			for(int j=0; j < sinbuf.count() / 4; j++)
			{
			    uint32_t sdirect = qFromLittleEndian<uint32_t>(sinbuf.mid(j*4, 4));
			    if(sdirect > 0)
			    {
				blkstrlist.append(QString::number(sdirect * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
				//blocklist.append(sdirect);
			    }
			}
		    //}
		}
	    }
	    if(tripleindirect > 0)
	    {
		QList<uint32_t> dinlist;
		dinlist.clear();
		QList<uint32_t> sinlist;
		sinlist.clear();
                curimg->open(QIODevice::ReadOnly);
                curimg->seek(fsinfo->value("partoffset").toUInt() + (tripleindirect * fsinfo->value("blocksize").toUInt()));
                QByteArray triplebuf = curimg->read(fsinfo->value("blocksize").toUInt());
                curimg->close();
		    for(int i=0; i < triplebuf.count() / 4; i++)
		    {
			uint32_t dindirect = qFromLittleEndian<uint32_t>(triplebuf.mid(i*4, 4));
			if(dindirect > 0)
			    dinlist.append(dindirect);
		    }
		//}
		for(int i=0; i < dinlist.count(); i++)
		{
                    curimg->open(QIODevice::ReadOnly);
                    curimg->seek(fsinfo->value("partoffset").toUInt() + (dinlist.at(i) * fsinfo->value("blocksize").toUInt()));
                    QByteArray dinbuf = curimg->read(fsinfo->value("blocksize").toUInt());
                    curimg->close();
			for(int j=0; j < dinbuf.count() / 4; j++)
			{
			    uint32_t sindirect = qFromLittleEndian<uint32_t>(dinbuf.mid(j*4, 4));
			    if(sindirect > 0)
				sinlist.append(sindirect);
			}
		    //}
		    for(int j=0; j < sinlist.count(); j++)
		    {
                        curimg->open(QIODevice::ReadOnly);
                        curimg->seek(fsinfo->value("partoffset").toUInt() + (sinlist.at(j) * fsinfo->value("blocksize").toUInt()));
                        QByteArray sinbuf = curimg->read(fsinfo->value("blocksize").toUInt());
                        curimg->close();
			    for(int k=0; k < sinbuf.count() / 4; k++)
			    {
				uint32_t sdirect = qFromLittleEndian<uint32_t>(sinbuf.mid(k*4, 4));
				if(sdirect > 0)
				{
				    blkstrlist.append(QString::number(sdirect * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
				    //blocklist.append(sdirect);
				}
			    }
			//}
		    }
		}
	    }
        }
        //qDebug() << "block list to compare to parfileinfo layout:" << blocklist;
        //qDebug() << "blkstr list:" << blkstrlist;
        // GET THE DIRECTORY ENTRY CONTENT FOR THE CURRENT INODE
        direntrybuf.clear();
        curimg->open(QIODevice::ReadOnly);
        for(int i=0; i < blkstrlist.count(); i++)
        {
            curimg->seek(fsinfo->value("partoffset").toUInt() + blkstrlist.at(i).split(",").at(0).toUInt());
            direntrybuf.append(curimg->read(blkstrlist.at(i).split(",").at(1).toUInt()));
        }
        curimg->close();
    }
    int curoffset = 24; // SKIP THE . AND .. ENTRIES WHICH ARE ALWAYS THE 1ST TWO ENTRIES AND ARE 12 BYTES LONG EACH
    if(inodeflags & 0x1000)
    {
	curoffset = 40; // THIS SHOULD ACCOUNT FOR HASH TREE DEPTH OF 0, NEED TEST FOR 1 - 3
        qDebug() << "cur directory inode uses hashed btree rather than linear direntry reading";
    }
    bool nextisdeleted = false;
    while(curoffset < direntrybuf.count() - 8)
    {
        fileinfo.clear();
        int entrylength = 0;
        int lengthdiv = (8 + qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 6))) / 4;
        int remdiv = (8 + qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 6))) % 4;
        int newlength = 0;
        if(remdiv == 0)
            newlength = lengthdiv * 4;
        else
            newlength = lengthdiv * 4 + 4;
        // need to make this value "extinode" and the "inode" the standard increment as the other file systems...
        // then i can move the extinode to the properties such as deleted time, etc...
        fileinfo.insert("inode", QVariant(inodecnt));
        fileinfo.insert("extinode", QVariant(qFromLittleEndian<int32_t>(direntrybuf.mid(curoffset, 4))));
        if(fileinfo.value("extinode").toInt() > 0)
        {
	    //qDebug() << "extinode:" << QString::number(fileinfo.value("extinode").toUInt(), 16);
            if(parfileinfo == NULL)
            {
	        fileinfo.insert("path", QVariant("/"));
	        fileinfo.insert("parentinode", QVariant(-1));
            }
            else
            {
                fileinfo.insert("path", QVariant(QString(parfileinfo->value("path").toString() + parfileinfo->value("filename").toString() + QString("/"))));
                fileinfo.insert("parentinode", QVariant(parfileinfo->value("inode").toUInt()));
            }
            int namelength = 0;
            int filetype = -1;
            entrylength = qFromLittleEndian<uint16_t>(direntrybuf.mid(curoffset + 4, 2));
            if(fsinfo->value("incompatflags").toUInt() & 0x0002)
            {
                //qDebug() << "name length where only byte offset 6 is used..";
                namelength = qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 6));
                filetype = qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 7));
            }
            else
            {
                //qDebug() << "name length where byte offset 6 and and 7 are used...";
                namelength = qFromLittleEndian<uint16_t>(direntrybuf.mid(curoffset + 6, 2));
            }
            fileinfo.insert("filename", QVariant(QString::fromStdString(direntrybuf.mid(curoffset + 8, namelength).toStdString())));
            //qDebug() << "filename:" << fileinfo.value("filename").toString();
            // NEED TO USE THE INODE TO THEN GET THE RELEVANT METADATA...
            // FILE TYPE GETS US INFO, SO IF IT'S A DIRECTORY, WE CAN PARSE THE DIRECTORY INODE, WITH THIS RECURSIVE FUNCTION...
            if(nextisdeleted)
                fileinfo.insert("isdeleted", QVariant(1));
            else
                fileinfo.insert("isdeleted", QVariant(0));
            //qDebug() << "newlength:" << newlength << "entrylength:" << entrylength << "namelength:" << namelength;
            //if(newlength < entrylength && entrylength < 264)
            if(newlength < entrylength)
                nextisdeleted = true;
            else
                nextisdeleted = false;
            //qDebug() << "nextisdeleted:" << nextisdeleted;
            //itemtype = itemnode->itemtype; // node type 5=file, 3=dir, 4-del file, 10=vir file, 11=vir dir, -1=not file (evid image, vol, part, fs), 15=carved file
            if(filetype == 0) // unknown type
                fileinfo.insert("itemtype", QVariant(0));
            else if(filetype == 1) // FILE
            {
                if(fileinfo.value("isdeleted").toUInt() == 0)
                    fileinfo.insert("itemtype", QVariant(5));
                else
                    fileinfo.insert("itemtype", QVariant(4));
            }
            else if(filetype == 2) // DIR
            {
                if(fileinfo.value("isdeleted").toUInt() == 0)
                    fileinfo.insert("itemtype", QVariant(3));
                else
                    fileinfo.insert("itemtype", QVariant(2));
            }
            else if(filetype == 3) // character device
                fileinfo.insert("itemtype", QVariant(6));
            else if(filetype == 4) // block device
                fileinfo.insert("itemtype", QVariant(7));
            else if(filetype == 5) // FIFO
                fileinfo.insert("itemtype", QVariant(8));
            else if(filetype == 6) // Unix socket
                fileinfo.insert("itemtype", QVariant(9));
            else if(filetype == 7) // Symbolic Link
                fileinfo.insert("itemtype", QVariant(12));
            // DETERMINE WHICH BLOCK GROUP # THE CURINODE IS IN SO I CAN READ IT'S INODE'S CONTENTS AND GET THE NECCESARY METADATA
	    qulonglong curinodetablestartblock =  0;
	    QByteArray curinodetablebuf;
	    curinodetablebuf.clear();
            int blockgroupnumber = 0;
	    for(int i=1; i <= blockgroups.count(); i++)
	    {
		if(fileinfo.value("extinode").toUInt() < i*fsinfo->value("blockgroupinodecnt").toUInt())
                {
		    curinodetablestartblock = blockgroups.at(i-1).toULongLong();
                    blockgroupnumber = i - 1;
                    break;
                }
	    }
            //qDebug() << "block group number:" << curinodetablestartblock;
            curimg->open(QIODevice::ReadOnly);
            curimg->seek(fsinfo->value("partoffset").toUInt() + (curinodetablestartblock * fsinfo->value("blocksize").toUInt()));
            curinodetablebuf = curimg->read(fsinfo->value("inodesize").toUInt() * fsinfo->value("blockgroupinodecnt").toUInt());
            curimg->close();
            //qDebug() << "curinodetablebuf inode entry number:" << fsinfo->value("inodesize").toUInt() * (fileinfo.value("inode").toUInt() - 1 - (blockgroupnumber * fsinfo->value("blockgroupinodecnt").toUInt()));
	    QByteArray curinodebuf = curinodetablebuf.mid(fsinfo->value("inodesize").toUInt() * (fileinfo.value("extinode").toUInt() - 1 - (blockgroupnumber * fsinfo->value("blockgroupinodecnt").toUInt())), fsinfo->value("inodesize").toUInt());
            uint16_t filemode = qFromLittleEndian<uint16_t>(curinodebuf.mid(0, 2));
            QString filemodestr = "---------";
            if(filemode & 0xc000) // unix socket
                filemodestr.replace(0, 1, "s");
            if(filemode & 0xa000) // symbolic link
                filemodestr.replace(0, 1, "l");
            if(filemode & 0x6000) // block device
                filemodestr.replace(0, 1, "b");
            if(filemode & 0x2000) // char device
                filemodestr.replace(0, 1, "c");
            if(filemode & 0x1000) // FIFO (pipe)
                filemodestr.replace(0, 1, "p");
            if(filemode & 0x8000) // regular file
            {
                if(fsinfo->value("readonlyflags").toUInt() & 0x0002) // LARGE FILE SUPPORT
                {
                    uint32_t lowersize = qFromLittleEndian<uint32_t>(curinodebuf.mid(4, 4));
                    uint32_t uppersize = qFromLittleEndian<uint32_t>(curinodebuf.mid(108, 4));
                    fileinfo.insert("logicalsize", QVariant(((qulonglong)uppersize >> 32) + lowersize));
                }
                else
                {
                    fileinfo.insert("logicalsize", QVariant(qFromLittleEndian<uint32_t>(curinodebuf.mid(4, 4))));
                }
                filemodestr.replace(0, 1, "-");
            }
            else if(filemode & 0x4000) // directory
            {
                fileinfo.insert("logicalsize", QVariant(qFromLittleEndian<uint32_t>(curinodebuf.mid(4, 4))));
                filemodestr.replace(0, 1, "d");
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
            fileinfo.insert("mode", QVariant(filemodestr));

            // STILL NEED TO DO FILE ATTRIBUTES, EXTENDED ATTRIBUTE BLOCK
            uint16_t lowergroupid = qFromLittleEndian<uint16_t>(curinodebuf.mid(24, 2));
            uint16_t uppergroupid = qFromLittleEndian<uint16_t>(curinodebuf.mid(122, 2));
            fileinfo.insert("groupid", QVariant(((uint32_t)uppergroupid >> 16) + lowergroupid));
            uint16_t loweruserid = qFromLittleEndian<uint16_t>(curinodebuf.mid(2, 2));
            uint16_t upperuserid = qFromLittleEndian<uint16_t>(curinodebuf.mid(120, 2));
            fileinfo.insert("userid", QVariant(((uint32_t)upperuserid >> 16) + loweruserid));
            fileinfo.insert("accessdate", qFromLittleEndian<uint32_t>(curinodebuf.mid(8, 4)));
            fileinfo.insert("statusdate", qFromLittleEndian<uint32_t>(curinodebuf.mid(12, 4)));
            fileinfo.insert("modifydate", qFromLittleEndian<uint32_t>(curinodebuf.mid(16, 4)));
            fileinfo.insert("deletedate", qFromLittleEndian<uint32_t>(curinodebuf.mid(20, 4)));
	    fileinfo.insert("linkcount", qFromLittleEndian<uint16_t>(curinodebuf.mid(26, 2)));
	    //qDebug() << "fsinfo type:" << fsinfo->value("typestr").toString();
	    //qDebug() << "create date:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(144, 4));
	    if(fsinfo->value("typestr").toString().startsWith("EXT4"))
		fileinfo.insert("createdate", qFromLittleEndian<uint32_t>(curinodebuf.mid(144, 4)));
            // GET BLOCKLIST FOR THE CURINODEBUF
            QStringList curblkstrlist;
            curblkstrlist.clear();
            QList<uint32_t> curblocklist;
            curblocklist.clear();
            
            uint32_t curinodeflags = qFromLittleEndian<uint32_t>(curinodebuf.mid(32, 4));
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
	    fileinfo.insert("attribute", QVariant(attrstr));

            if((fsinfo->value("incompatflags").toUInt() & 0x40) && (curinodeflags & 0x80000)) // FS USES EXTENTS && INODE USES EXTENTS
            {
                uint16_t extententries = qFromLittleEndian<uint16_t>(curinodebuf.mid(42, 2));
                uint16_t extentdepth = qFromLittleEndian<uint16_t>(curinodebuf.mid(46, 2));
                if(extentdepth == 0) // use ext4_extent
                {
                    for(int i=0; i < extententries; i++)
                    {
			//qDebug() << "logical block:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(52 + i*12, 4));
                        uint16_t blocklength = qFromLittleEndian<uint16_t>(curinodebuf.mid(56 + i*12, 2));
                        uint16_t starthi = qFromLittleEndian<uint16_t>(curinodebuf.mid(58 + i*12, 2));
                        uint32_t startlo = qFromLittleEndian<uint32_t>(curinodebuf.mid(60 + i*12, 4));
                        uint64_t startblock = (((uint64_t)starthi >> 32) + startlo) * fsinfo->value("blocksize").toUInt();
                        curblkstrlist.append(QString::number(startblock) + "," + QString::number(blocklength * fsinfo->value("blocksize").toUInt()));
                    }
                }
                else // use ext4_entent_idx
                {
                    QList<uint32_t> leafnodes;
                    leafnodes.clear();
                    // NEED TO FIGURE OUT HOW TO HANDLE EXTENT DEPTH IN THE LOOP...
                    for(int i=0; i < extententries; i++)
                    {
			//qDebug() << "logical block:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(52 + i*12, 4));
                        leafnodes.append(qFromLittleEndian<uint32_t>(curinodebuf.mid(56 + i*12, 4)));
                        //qDebug() << "ei_leaf_lo: leaf node" << qFromLittleEndian<uint32_t>(curinodebuf.mid(56 + i*12, 4));
                    }
                    //qDebug() << "leafnodes:" << leafnodes;
                    for(int i=0; i < leafnodes.count(); i++)
                    {
                        QByteArray leafnode;
                        leafnode.clear();
                        curimg->open(QIODevice::ReadOnly);
                        curimg->seek(fsinfo->value("partoffset").toUInt() + (leafnodes.at(i) * fsinfo->value("blocksize").toUInt()));
                        leafnode = curimg->read(fsinfo->value("blocksize").toUInt());
                        curimg->close();
                            uint16_t extententries = qFromLittleEndian<uint16_t>(leafnode.mid(2, 2));
                            uint16_t extentdepth = qFromLittleEndian<uint16_t>(leafnode.mid(6, 2));
                            if(extentdepth == 0) // use ext4_extent
                            {
                                for(int j=0; j < extententries; j++)
                                {
				    //qDebug() << "logical block:" << qFromLittleEndian<uint32_t>(leafnode.mid(12 + j*12, 4));
                                    uint16_t blocklength = qFromLittleEndian<uint16_t>(leafnode.mid(16 + j*12, 2));
                                    uint16_t starthi = qFromLittleEndian<uint16_t>(leafnode.mid(18 + j*12, 2));
                                    uint32_t startlo = qFromLittleEndian<uint32_t>(leafnode.mid(20 + j*12, 4));
                                    uint64_t startblock = (((uint64_t)starthi >> 32) + startlo) * fsinfo->value("blocksize").toUInt();
                                    curblkstrlist.append(QString::number(startblock) + "," + QString::number(blocklength * fsinfo->value("blocksize").toUInt()));
                                }
                            }
                            else // use ext4_extent_idx
                            {
                                qDebug() << "repeat leafnode exercise here...";
                            }
                            //qDebug() << "leaf header:" << QString::number(qFromLittleEndian<uint16_t>(leafnode.mid(0, 2)), 16);
                            //qDebug() << "extent entries:" << qFromLittleEndian<uint16_t>(leafnode.mid(2, 2));
                            //qDebug() << "max extent entries:" << qFromLittleEndian<uint16_t>(leafnode.mid(4, 2));
                            //qDebug() << "extent depth:" << qFromLittleEndian<uint16_t>(leafnode.mid(6, 2));
                        //}
                    }
                    //qDebug() << "extent header:" << QString::number(qFromLittleEndian<uint16_t>(curinodebuf.mid(40, 2)), 16);
                    //qDebug() << "extent entries:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(42, 2));
                    //qDebug() << "max extent entries:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(44, 2));
                    //qDebug() << "extent depth:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(46, 2));
                    
                    //qDebug() << "ei_block:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(52, 4));
                    //qDebug() << "ei_leaf_lo:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(56, 4));
                    //qDebug() << "ei_leaf_hi:" << qFromLittleEndian<uint16_t>(curinodebuf.mid(60, 2));
                    //qDebug() << "use extent idx";
                }
            }
            else
            {
                for(int i=0; i < 12; i++)
                {
                    uint32_t curdirectblock = qFromLittleEndian<uint32_t>(curinodebuf.mid(40 + i*4, 4));
                    if(curdirectblock > 0)
                    {
                        curblocklist.append(curdirectblock);
                        curblkstrlist.append(QString::number(curdirectblock * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
                    }
                }
                //qDebug() << fileinfo.value("filename").toString() << "current block list before i get the indirect block pointers.." << curblocklist;
                uint32_t cursingleindirect = qFromLittleEndian<uint32_t>(curinodebuf.mid(88, 4));
                uint32_t curdoubleindirect = qFromLittleEndian<uint32_t>(curinodebuf.mid(92, 4));
                uint32_t curtripleindirect = qFromLittleEndian<uint32_t>(curinodebuf.mid(96, 4));
                if(cursingleindirect > 0)
                {
                    curimg->open(QIODevice::ReadOnly);
                    curimg->seek(fsinfo->value("partoffset").toUInt() + (cursingleindirect * fsinfo->value("blocksize").toUInt()));
                    QByteArray singlebuf = curimg->read(fsinfo->value("blocksize").toUInt());
                    curimg->close();
                        for(int i=0; i < singlebuf.count() / 4; i++)
                        {
                            uint32_t cursingledirect = qFromLittleEndian<uint32_t>(singlebuf.mid(i*4, 4));
                            if(cursingledirect > 0)
                            {
                                curblocklist.append(cursingledirect);
                                curblkstrlist.append(QString::number(cursingledirect * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
                            }
                        }
                    //}
                }
                if(curdoubleindirect > 0)
                {
                    QList<uint32_t> sinlist;
                    sinlist.clear();
                    curimg->open(QIODevice::ReadOnly);
                    curimg->seek(fsinfo->value("partoffset").toUInt() + (curdoubleindirect * fsinfo->value("blocksize").toUInt()));
                    QByteArray doublebuf = curimg->read(fsinfo->value("blocksize").toUInt());
                    curimg->close();
                        for(int i=0; i < doublebuf.count() / 4; i++)
                        {
                            uint32_t sindirect = qFromLittleEndian<uint32_t>(doublebuf.mid(i*4, 4));
                            if(sindirect > 0)
                                sinlist.append(sindirect);
                        }
                    //}
                    for(int i=0; i < sinlist.count(); i++)
                    {
                        curimg->open(QIODevice::ReadOnly);
                        curimg->seek(fsinfo->value("partoffset").toUInt() + (sinlist.at(i) * fsinfo->value("blocksize").toUInt()));
                        QByteArray sinbuf = curimg->read(fsinfo->value("blocksize").toUInt());
                        curimg->close();
                            for(int j=0; j < sinbuf.count() / 4; j++)
                            {
                                uint32_t sdirect = qFromLittleEndian<uint32_t>(sinbuf.mid(j*4, 4));
                                if(sdirect > 0)
                                {
                                    curblkstrlist.append(QString::number(sdirect * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
                                    curblocklist.append(sdirect);
                                }
                            }
                        //}
                    }
                }
                if(curtripleindirect > 0)
                {
                    QList<uint32_t> dinlist;
                    dinlist.clear();
                    QList<uint32_t> sinlist;
                    sinlist.clear();
                    curimg->open(QIODevice::ReadOnly);
                    curimg->seek(fsinfo->value("partoffset").toUInt() + (curtripleindirect * fsinfo->value("blocksize").toUInt()));
                    QByteArray triplebuf = curimg->read(fsinfo->value("blocksize").toUInt());
                    curimg->close();
                        for(int i=0; i < triplebuf.count() / 4; i++)
                        {
                            uint32_t dindirect = qFromLittleEndian<uint32_t>(triplebuf.mid(i*4, 4));
                            if(dindirect > 0)
                                dinlist.append(dindirect);
                        }
                    //}
                    for(int i=0; i < dinlist.count(); i++)
                    {
                        curimg->open(QIODevice::ReadOnly);
                        curimg->seek(fsinfo->value("partoffset").toUInt() + (dinlist.at(i) * fsinfo->value("blocksize").toUInt()));
                        QByteArray dinbuf = curimg->read(fsinfo->value("blocksize").toUInt());
                        curimg->close();
                            for(int j=0; j < dinbuf.count() / 4; j++)
                            {
                                uint32_t sindirect = qFromLittleEndian<uint32_t>(dinbuf.mid(j*4, 4));
                                if(sindirect > 0)
                                    sinlist.append(sindirect);
                            }
                        //}
                        for(int j=0; j < sinlist.count(); j++)
                        {
                            curimg->open(QIODevice::ReadOnly);
                            curimg->seek(fsinfo->value("partoffset").toUInt() + (sinlist.at(j) * fsinfo->value("blocksize").toUInt()));
                            QByteArray sinbuf = curimg->read(fsinfo->value("blocksize").toUInt());
                            curimg->close();
                                for(int k=0; k < sinbuf.count() / 4; k++)
                                {
                                    uint32_t sdirect = qFromLittleEndian<uint32_t>(sinbuf.mid(k*4, 4));
                                    if(sdirect > 0)
                                    {
                                        curblocklist.append(sdirect);
                                        curblkstrlist.append(QString::number(sdirect * fsinfo->value("blocksize").toUInt()) + "," + QString::number(fsinfo->value("blocksize").toUInt()));
                                    }
                                }
                            //}
                        }
                    }
                }
            }
            //qDebug() << "curblkstr:" << curblkstrlist;

            // the physical size is wrong for extent's since it's sum of the length's / blocksize -> that # + 1 (if remainder) * blocksize...
	    qulonglong physize = 0;
	    for(int i=0; i < curblkstrlist.count(); i++)
	    {
		physize += curblkstrlist.at(i).split(",").at(1).toUInt();
	    }
	    int phyblkcnt = physize / fsinfo->value("blocksize").toUInt();
	    int phyremcnt = physize % fsinfo->value("blocksize").toUInt();
	    if(phyremcnt > 0)
		phyblkcnt++;
	    //qDebug() << "physize:" << physize << "phyblkcnt:" << phyblkcnt * fsinfo->value("blocksize").toUInt();
            fileinfo.insert("physicalsize", QVariant(phyblkcnt * fsinfo->value("blocksize").toUInt()));

            //qDebug() << "curblock list:" << curblocklist;
            //fileinfo.insert("physicalsize", QVariant(curblocklist.count() * fsinfo->value("blocksize").toUInt()));
            //qDebug() << "logical/physical size:" << fileinfo.value("logicalsize").toUInt() << fileinfo.value("physicalsize").toUInt();
            //qDebug() << "curblocklist:" << curblocklist;
            QString layout = "";
            if(curblocklist.count() > 0)
                layout = ConvertBlocksToExtents(curblocklist, fsinfo->value("blocksize").toUInt());
            fileinfo.insert("layout", QVariant(layout));
            //qDebug() << fileinfo.value("filename").toString() << "layout:" << layout;

            fileinfolist->append(fileinfo);
            inodecnt++;
            if(filemode & 0x4000) // directory so recurse it's value...
            {
                //qDebug() << "inodecnt before sub dir expansion:" << inodecnt;
                ParseExtDirectory(curimg, fsinfo, fileinfolist, orphanlist, &fileinfo, fileinfo.value("extinode").toULongLong(), inodecnt); // initial attempt to recurse...
                //ParseExtDirectory(estring, fsinfo, fileinfolist, orphanlist, &fileinfo, fileinfo.value("extinode").toULongLong(), inodecnt); // initial attempt to recurse...
                inodecnt = fileinfolist->count();
                //qDebug() << "fileinfolist count after sub dir expansion:" << fileinfolist->count();
            }

        }
        curoffset += newlength;
    }

 */ 


//qDebug() << "create date:" << QDateTime::fromSecsSinceEpoch(fileinfo.value("createdate").toInt(), QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP");
    /*
     *	if(fileinfo->contains("extinode"))
	    out << "EXTFS inode|" << fileinfo->value("extinode").toUInt() << "|EXTFS inode value to locate file in the filesystem" << Qt::endl;
	if(fileinfo->contains("aliasname"))
	    out << "Alias Name|" << fileinfo->value("aliasname").toString() << "|8.3 file name" << Qt::endl;
        out << "File Attributes|" << fileinfo->value("attribute").toString() << "|File attributes." << Qt::endl;
	if(fileinfo->contains("clusterlist"))
	    out << "Cluster List|" << fileinfo->value("clusterlist").toString() << "|Clusters occupied by the file." << Qt::endl;
        out << "Layout|" << fileinfo->value("layout").toString() << "|File offset,size; layout." << Qt::endl;
        out << "Physical Size|" << QString::number(fileinfo->value("physicalsize").toUInt()) << "|Sector Size in Bytes for the file." << Qt::endl;
	if(fileinfo->contains("deletedate"))
	    out << "Deleted Time|" << QDateTime::fromSecsSinceEpoch(fileinfo->value("deletedate").toInt(), QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP") << "|Deleted time for the file." << Qt::endl;
	if(fileinfo->contains("mode"))
	    out << "Mode|" << fileinfo->value("mode").toString() << "|Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type/user/group/other - [rdlcbpv]/rw[sSx]/rw[sSx]/rw[tTx]" << Qt::endl;
	if(fileinfo->contains("userid"))
	    out << "uid / gid|" << QString(QString::number(fileinfo->value("userid").toUInt()) + " / " + QString::number(fileinfo->value("groupid").toUInt())) << "|User ID and Group ID" << Qt::endl;
	if(fileinfo->contains("linkcount"))
	    out << "Link Count|" << QString::number(fileinfo->value("linkcount").toUInt()) << "|Number of files pointing to this file" << Qt::endl;
	if(fileinfo->contains("ntinode"))
	    out << "MFT Entry Record|" << QString::number(fileinfo->value("ntinode").toUInt()) << "|Entry record for the file in the Master File Table (MFT)" << Qt::endl;
	if(fileinfo->contains("mftrecordlayout"))
	    out << "MFT Record Layout|" << fileinfo->value("mftrecordlayout").toString() << "|Byte offset and size for the MFT Entry record" << Qt::endl;
	if(fileinfo->contains("parntinode"))
	    out << "Parent MFT Entry Record|" << QString::number(fileinfo->value("parntinode").toUInt()) << "|Parent Entry Record for the current entry record in the MFT" << Qt::endl;
	if(fileinfo->contains("filecreate"))
	    out << "$FILE_NAME Create Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("filecreate").toUInt()) << "|File Creation time as recorded in the $FILE_NAME attribute" << Qt::endl;
	if(fileinfo->contains("filemodify"))
	    out << "$FILE_NAME Modified Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("filemodify").toUInt()) << "|File Modification time as recorded in the $FILE_NAME attribute" << Qt::endl;
	if(fileinfo->contains("filestatus"))
	    out << "$FILE_NAME Status Changed Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("filestatus").toUInt()) << "|File Status Changed time as recorded in the $FILE_NAME attribute" << Qt::endl;
	if(fileinfo->contains("fileaccess"))
	    out << "$FILE_NAME Accessed Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("fileaccess").toUInt()) << "|File Accessed time as recorded in the $FILE_NAME attribute" << Qt::endl;
        if(fileinfo->contains("mftsequenceid"))
            out << "MFT Sequence ID|" << QString::number(fileinfo->value("mftsequenceid").toUInt()) << "|Sequence ID for the file as recorded in the MFT entry" << Qt::endl;
        if(fileinfo->contains("i30sequenceid"))
            out << "$I30 Sequence ID|" << QString::number(fileinfo->value("i30sequenceid").toUInt()) << "|Sequence ID for the file as recorded in the $I30 entry" << Qt::endl;
        if(fileinfo->contains("i30create"))
            out << "$I30 Create Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("i30create").toUInt()) << "|File Creation time as recorded in the $I30 entry" << Qt::endl;
        if(fileinfo->contains("i30modify"))
            out << "$I30 Modify Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("i30modify").toUInt()) << "|File Modification time as recorded in the $I30 entry" << Qt::endl;
        if(fileinfo->contains("i30status"))
            out << "$I30 Status Changed Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("i30status").toUInt()) << "|File Status Changed time as recorded in the $I30 entry" << Qt::endl;
        if(fileinfo->contains("i30access"))
            out << "$I30 Accessed Date|" << ConvertWindowsTimeToUnixTimeUTC(fileinfo->value("i30access").toUInt()) << "|File Accessed time as recorded in the $I30 entry" << Qt::endl;
     */ 
