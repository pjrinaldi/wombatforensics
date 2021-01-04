#include "newcase.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

// ONCE THE TREE IS FULLY LOADED AND I START TO SAVE CASE, I NEED TO SAVE TREE TO A SINGLE FILE TO LOAD FROM...
void ParseExtendedPartition(QString estring, uint32_t primextoff, uint32_t offset, uint32_t size, QList<qint64>* pofflist, QList<qint64>* psizelist, QList<QHash<QString, QVariant>>* fsinfolist)
{
    //qDebug() << "primary extended size in bytes:" << size*512;
    QFile rawforimg(estring);
    if(!rawforimg.isOpen())
        rawforimg.open(QIODevice::ReadOnly);
    if(primextoff == offset)
        rawforimg.seek(offset * 512);
    else
        rawforimg.seek((primextoff + offset)*512);
    QByteArray sector = rawforimg.peek(512);
    rawforimg.close();
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
		ParseExtendedPartition(estring, primextoff, curoffset, cursize, pofflist, psizelist, fsinfolist);
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
                ParseFileSystemInformation(estring, curoff, fsinfolist);
	    }
	}
    }
    //else
        //qDebug() << "i screwed up the math somewhere...";
}

void ParseVolume(QString estring, qint64 imgsize, QList<qint64>* pofflist, QList<qint64>* psizelist, QList<QHash<QString, QVariant>>* fsinfolist) // would have to add &fsinfolist here...
{
    //qDebug() << "estring:" << estring;
    QFile rawforimg(estring);
    if(!rawforimg.isOpen())
	rawforimg.open(QIODevice::ReadOnly);
    QByteArray sector0 = rawforimg.peek(512);
    rawforimg.close();
    uint16_t mbrsig = qFromLittleEndian<uint16_t>(sector0.mid(510, 2));
    uint16_t applesig = qFromLittleEndian<uint16_t>(sector0.left(2)); // should be in 2nd sector, powerpc mac's not intel mac's
    uint32_t bsdsig = qFromLittleEndian<uint32_t>(sector0.left(4)); // can be at start of partition entry of a dos mbr
    uint16_t sunsig = qFromLittleEndian<uint16_t>(sector0.mid(508, 2)); // worry about it later, i386 sun can be at 2nd sector of partition entry of a dos mbr
    uint64_t gptsig = qFromLittleEndian<uint64_t>(sector0.left(8));
    if(mbrsig == 0xaa55) // POSSIBLY MBR OR GPT
    {
        //qDebug() << "imgsize:" << imgsize;
	if((uint8_t)sector0.at(450) == 0xee) // GPT DISK
        {
            if(!rawforimg.isOpen())
                rawforimg.open(QIODevice::ReadOnly);
            rawforimg.seek(512);
            QByteArray sector1 = rawforimg.read(512);
            rawforimg.close();
	    gptsig = qFromLittleEndian<uint64_t>(sector1.left(8));
	    if(gptsig == 0x5452415020494645) // GPT PARTITION TABLE
	    {
		//qDebug() << "gpt found, parse table now...";
		uint32_t parttablestart = qFromLittleEndian<uint32_t>(sector1.mid(72, 8));
		uint16_t partentrycount = qFromLittleEndian<uint16_t>(sector1.mid(80, 4));
		uint16_t partentrysize = qFromLittleEndian<uint16_t>(sector1.mid(84, 4));
		if(!rawforimg.isOpen())
		    rawforimg.open(QIODevice::ReadOnly);
		rawforimg.seek((parttablestart*512));
		QByteArray partentries = rawforimg.read((partentrycount*partentrysize));
		rawforimg.close();
		for(int i=0; i < partentrycount; i++)
		{
		    int cnt = i*partentrysize;
		    uint32_t curstartsector = qFromLittleEndian<uint32_t>(partentries.mid(cnt + 32, 8));
		    uint32_t curendsector = qFromLittleEndian<uint32_t>(partentries.mid(cnt + 40, 8));
		    if((curendsector - curstartsector) > 0)
		    {
			pofflist->append(curstartsector);
			psizelist->append((curendsector - curstartsector + 1));
                        ParseFileSystemInformation(estring, curstartsector, fsinfolist);
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
                    ParseExtendedPartition(estring, curoffset, curoffset, cursize, pofflist, psizelist, fsinfolist); // add fsinfolist here as well...
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
		    if(cursize <= imgsize && cursize > 0)
		    {
                        //qDebug() << "parse primary partition here...";
			pofflist->append(curoffset);
        		psizelist->append(cursize);
                        ParseFileSystemInformation(estring, curoffset, fsinfolist);
			qDebug() << "part[i]:" << i << "offset:" << curoffset << "cursize:" << cursize << "part type:" << QString::number(curparttype, 16);
		    }
		    else
		    {
			//qDebug() << "bogus partition entry";
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
	if(!rawforimg.isOpen())
	    rawforimg.open(QIODevice::ReadOnly);
	rawforimg.seek((parttablestart*512));
	QByteArray partentries = rawforimg.read((partentrycount*partentrysize));
	rawforimg.close();
	for(int i=0; i < partentrycount; i++)
	{
	    int cnt = i*partentrysize;
	    uint32_t curstartsector = qFromLittleEndian<uint32_t>(partentries.mid(cnt + 32, 8));
	    uint32_t curendsector = qFromLittleEndian<uint32_t>(partentries.mid(cnt + 40, 8));
	    if((curendsector - curstartsector) > 0)
	    {
		pofflist->append(curstartsector);
		psizelist->append((curendsector - curstartsector + 1));
                ParseFileSystemInformation(estring, curstartsector,fsinfolist);
		//qDebug() << "partition[" << i << "] start sector:" << curstartsector << "end sector:" << curendsector << "cur size:" << curendsector - curstartsector + 1;
	    }
	}
    }
}

void ParseFileSystemInformation(QString estring, off64_t partoffset, QList<QHash<QString, QVariant>> *fsinfolist)
{
    //QList<QHash<QString, QVariant>> fileinfolist;
    QHash<QString, QVariant> fsinfo;
    //qDebug() << "estring:" << estring << "partoffset:" << partoffset;
    QByteArray partbuf;
    partbuf.clear();
    QFile efile(estring);
    if(!efile.isOpen())
        efile.open(QIODevice::ReadOnly);
    if(efile.isOpen())
    {
        efile.seek(partoffset*512);
        partbuf = efile.read(69120);
        efile.close();
    }
    // check for various FAT FS's
    uint16_t winsig = qFromLittleEndian<uint16_t>(partbuf.mid(510, 2));
    uint16_t extsig = qFromLittleEndian<uint16_t>(partbuf.mid(1080, 2));
    QString apfssig = QString::fromStdString(partbuf.mid(32, 4).toStdString());
    QString hfssig = QString::fromStdString(partbuf.mid(1024, 2).toStdString());
    QString xfssig = QString::fromStdString(partbuf.mid(0, 4).toStdString());
    QString btrsig = QString::fromStdString(partbuf.mid(65600, 8).toStdString());
    QString bitlcksig = QString::fromStdString(partbuf.mid(0, 8).toStdString());
    QString bfssig = QString::fromStdString(partbuf.mid(544, 4).toStdString());
    if(winsig == 0xaa55) // FAT OR NTFS
    {
	QString exfatstr = QString::fromStdString(partbuf.mid(3, 5).toStdString());
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
            fsinfo.insert("bytespersector", QVariant(qFromLittleEndian<uint16_t>(partbuf.mid(11, 2))));
            qDebug() << "bytes per sector:" << fsinfo.value("bytespersector").toUInt();
            fsinfo.insert("sectorspercluster", QVariant(partbuf.at(13)));
            qDebug() << "sectors per cluster:" << fsinfo.value("sectorspercluster").toUInt();
            fsinfo.insert("totalsectors", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(40, 8))));
            fsinfo.insert("mftstartingcluster", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(48, 8))));
            qDebug() << "MFT starting cluster:" << fsinfo.value("mftstartingcluster").toUInt();
            fsinfo.insert("mftentrysize", QVariant(partbuf.at(64)));
	    unsigned int mftentrybytes = fsinfo.value("mftentrysize").toUInt() * fsinfo.value("bytespersector").toUInt() * fsinfo.value("sectorspercluster").toUInt();
	    qDebug() << "mft entry size in bytes:" << mftentrybytes;
            fsinfo.insert("indexrecordsize", QVariant(partbuf.at(68)));
            fsinfo.insert("serialnum", QVariant(qFromLittleEndian<qulonglong>(partbuf.mid(72, 8))));
            qDebug() << "serial num:" << QString("0x" + QString::number(fsinfo.value("serialnum").toULongLong(), 16));
            fsinfo.insert("mftoffset", QVariant((qulonglong)((partoffset * 512) + (fsinfo.value("mftstartingcluster").toUInt() * fsinfo.value("sectorspercluster").toUInt() * fsinfo.value("bytespersector").toUInt()))));
            qDebug() << "mftoffset:" << fsinfo.value("mftoffset").toUInt();
            // get MFT entry for $MFT to determine cluster's that contain the MFT...
            QByteArray mftentry0;
	    QByteArray mftentry3;
            mftentry0.clear();
	    mftentry3.clear();
            if(!efile.isOpen())
                efile.open(QIODevice::ReadOnly);
            if(efile.isOpen())
            {
                efile.seek(fsinfo.value("mftoffset").toUInt());
                mftentry0 = efile.read(mftentrybytes);
		efile.seek(fsinfo.value("mftoffset").toUInt() + 3 * mftentrybytes);
		mftentry3 = efile.read(mftentrybytes);
                efile.close();
            }
            qDebug() << "MFT ENTRY SIGNATURE:" << QString::fromStdString(mftentry0.left(4).toStdString());
            if(QString::fromStdString(mftentry0.left(4).toStdString()) == "FILE") // a proper mft entry
            {
                int curoffset = 0;
                uint16_t firstattroffset = qFromLittleEndian<uint16_t>(mftentry0.mid(20, 2)); // offset to first attribute
                //uint32_t mftentryusedsize = qFromLittleEndian<uint32_t>(mftentry0.mid(24, 4)); // mft entry used size
                uint16_t attrcount = qFromLittleEndian<uint16_t>(mftentry0.mid(40, 2)); // next attribute id
		uint32_t attrlength = 0;
                qDebug() << "first attr offset:" << firstattroffset << "attr count:" << attrcount;
                curoffset = firstattroffset;
                for(int i=0; i < attrcount; i++)
                {
                    uint32_t atrtype = qFromLittleEndian<uint32_t>(mftentry0.mid(curoffset, 4)); // attribute type
                    uint8_t namelength = qFromLittleEndian<uint8_t>(mftentry0.mid(curoffset + 9, 1)); // length of name
                    attrlength = qFromLittleEndian<uint32_t>(mftentry0.mid(curoffset + 4, 4)); // attribute length
                    qDebug() << "attr type:" << atrtype << "curoffset:" << curoffset << "attrlength:" << attrlength;
		    if(atrtype == 128 && namelength == 0) // $DATA attribute to parse
		    {
			break;
		    }
                    curoffset += attrlength;
                }
		qDebug() << "curoffset of $Data attribute:" << curoffset;
		uint64_t vcnstart = qFromLittleEndian<uint64_t>(mftentry0.mid(curoffset + 16, 8));
		uint64_t vcnend = qFromLittleEndian<uint64_t>(mftentry0.mid(curoffset + 24, 8));
		uint16_t runlistoff = qFromLittleEndian<uint16_t>(mftentry0.mid(curoffset + 32, 2));
		//uint16_t compressszunit = qFromLittleEndian<uint16_t>(mftentry0.mid(curoffset + 34, 2));
		uint64_t actualattrcontentsize = qFromLittleEndian<uint64_t>(mftentry0.mid(curoffset + 48, 8));
		qDebug() << "Starting VCN:" << vcnstart << "Ending VCN:" << vcnend;
		qDebug() << "attrlength:" << attrlength << "runlistoff:" << runlistoff << "actual attr content size:" << actualattrcontentsize;
		curoffset = curoffset + runlistoff;
		qDebug() << "run list start curoffset:" << curoffset;
		int i=0;
		while(curoffset < mftentrybytes) // might have to do a while < mftentrybytes and then go from there.... to build the curoffset = curoffset + 3, 4, etc...
		{
		    if(mftentry0.at(curoffset) > 0)
		    {
			qDebug() << "1st byte:" << QString::number(mftentry0.at(curoffset), 16);
			int runlengthbytes = QString(QString::number(mftentry0.at(curoffset), 16).at(1)).toInt();
			int runlengthoffset = QString(QString::number(mftentry0.at(curoffset), 16).at(0)).toInt();
			if(runlengthbytes == 0 && runlengthoffset == 0)
			    break;
			curoffset++;
			qDebug() << "run [" << i << "] length:" << runlengthbytes << "run [" << i << "] offset:" << runlengthoffset;
			int runlength = qFromLittleEndian<int>(mftentry0.mid(curoffset, runlengthbytes));
			int runoffset = qFromLittleEndian<int>(mftentry0.mid(curoffset + runlengthbytes, runlengthoffset));
			qDebug() << "runoffset cluster:" << runoffset << "runlength (clusters):" << runlength;
			i++;
			curoffset += runlengthbytes + runlengthoffset;
			qDebug() << "current offset after run [" << i-1 << "]" << curoffset;
			qDebug() << "mft byte start:" << runoffset * fsinfo.value("sectorspercluster").toUInt() * fsinfo.value("bytespersector").toUInt();
			qDebug() << "mft byte count:" << runlength * fsinfo.value("sectorspercluster").toUInt() * fsinfo.value("bytespersector").toUInt();
		    }
		    else
			break;
		    //break;
		}
            }
            else
                qDebug() << "error this is not a valid MFT ENTRY...";
	    qDebug() << "MFT ENTRY FOR $VOLUME SIGNATURE:" << QString::fromStdString(mftentry3.left(4).toStdString());
	    if(QString::fromStdString(mftentry3.left(4).toStdString()) == "FILE") // a proper mft entry
	    {
		int curoffset = 0;
                uint16_t firstattroffset = qFromLittleEndian<uint16_t>(mftentry3.mid(20, 2)); // offset to first attribute
                //uint32_t mftentryusedsize = qFromLittleEndian<uint32_t>(mftentry0.mid(24, 4)); // mft entry used size
                uint16_t attrcount = qFromLittleEndian<uint16_t>(mftentry3.mid(40, 2)); // next attribute id
		uint32_t attrlength = 0;
                qDebug() << "first attr offset:" << firstattroffset << "attr count:" << attrcount;
                curoffset = firstattroffset;
                for(int i=0; i < attrcount; i++)
                {
                    uint32_t atrtype = qFromLittleEndian<uint32_t>(mftentry3.mid(curoffset, 4)); // attribute type
                    //uint8_t namelength = qFromLittleEndian<uint8_t>(mftentry3.mid(curoffset + 9, 1)); // length of name
                    attrlength = qFromLittleEndian<uint32_t>(mftentry3.mid(curoffset + 4, 4)); // attribute length
                    qDebug() << "attr type:" << atrtype << "curoffset:" << curoffset << "attrlength:" << attrlength;
		    if(atrtype == 96) // $VOLUME_NAME attribute to parse (always resident)
		    {
			break;
		    }
                    curoffset += attrlength;
                }
		uint32_t contentsize = qFromLittleEndian<uint32_t>(mftentry3.mid(curoffset + 16, 4));
		uint16_t contentoffset = qFromLittleEndian<uint16_t>(mftentry3.mid(curoffset + 20, 2));
		qDebug() << "curoffset of $Volume attribute:" << curoffset;
		qDebug() << "vol content offset:" << contentoffset << "vol  content size:" << contentsize;
		//qDebug() << "volname name:" << QString::fromStdString(mftentry3.mid(curoffset + contentoffset, contentsize).toStdString());
		// EASY WAY TO READ UTF16 STRING !!!!!!!!!!
		fsinfo.insert("vollabel", QVariant(QString::fromUtf16(reinterpret_cast<const ushort*>(mftentry3.mid(curoffset + contentoffset, contentsize).data()))));
		QString volnamestr = QString::fromUtf16(reinterpret_cast<const ushort*>(mftentry3.mid(curoffset + contentoffset, contentsize).data()));
		qDebug() << "volnamestr:" << volnamestr;
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
            //int fatmultiplier = 0;
            /*
            if(fatstr == "FAT12")
                fsinfo.insert("fatmutliplier", QVariant(1));
            else if(fatstr == "FAT16")
                //fatmultiplier = 2;
            else if(fat32str == "FAT32")
                //fatmultiplier = 4;
            */
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
                if(!efile.isOpen())
                    efile.open(QIODevice::ReadOnly);
                if(efile.isOpen())
                {
		    //qDebug() << "fatoffset inside efile read operation:" << fsinfo.value("fatoffset").toUInt();
                    efile.seek(fsinfo.value("fatoffset").toUInt());
		    //qDebug() << "efile pos:" << efile.pos();
                    rootfatbuf = efile.read(fsinfo.value("fatsize").toUInt() * fsinfo.value("bytespersector").toUInt());
                    efile.close();
                }
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
                if(!efile.isOpen())
                    efile.open(QIODevice::ReadOnly);
                if(efile.isOpen())
                {
                    efile.seek(fsinfo.value("fatoffset").toUInt());
                    rootfatbuf = efile.read(fsinfo.value("fatsize").toUInt() * fsinfo.value("bytespersector").toUInt());
                    efile.close();
                }
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
                if(!efile.isOpen())
                    efile.open(QIODevice::ReadOnly);
                if(efile.isOpen())
                {
                    QStringList rootdirlayoutlist = rootdirlayout.split(";", Qt::SkipEmptyParts);
                    for(int j=0; j < rootdirlayoutlist.count(); j++)
                    {
                        efile.seek(rootdirlayoutlist.at(j).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
                        rootdirentry.append(efile.read(rootdirlayoutlist.at(j).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
                    }
                    efile.close();
                }
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
                /*
                qDebug() << "fatoffset:" << fsinfo.value("fatoffset").toUInt();
                qDebug() << "rootdircluster:" << fsinfo.value("rootdircluster").toUInt();
                qDebug() << "cluster offset:" << fsinfo.value("clusteroffset").toUInt();
                qDebug() << "bytes per sector:" << fsinfo.value("bytespersector").toUInt();
                qDebug() << "sectorspercluster:" << fsinfo.value("sectorspercluster").toUInt();
                qDebug() << "rootdir offset (bytes):" << fsinfo.value("rootdiroffset").toUInt();
                qDebug() << "rootdir offset (sectors):" << (fsinfo.value("clusteroffset").toUInt() + ((fsinfo.value("rootdircluster").toUInt() - 2) * fsinfo.value("sectorspercluster").toUInt()));
                */
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
        fsinfo.insert("fragsize", QVariant(1024 * pow(2, qFromLittleEndian<uint32_t>(partbuf.mid(1052, 4)))));
        fsinfo.insert("blockgroupblockcnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1056, 4))));
        fsinfo.insert("blockgroupfragcnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1060, 4))));
        fsinfo.insert("blockgroupinodecnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1064, 4))));
        fsinfo.insert("inodesize", QVariant(qFromLittleEndian<uint16_t>(partbuf.mid(1112, 2))));
        fsinfo.insert("compatflags", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1116, 4))));
        fsinfo.insert("incompatflags", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1120, 4))));
        fsinfo.insert("readonlyflags", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1124, 4))));
        fsinfo.insert("vollabel", QString::fromStdString(partbuf.mid(1144, 16).toStdString()));
        //qDebug() << "compatflags:" << fsinfo.value("compatflags").toUInt() << "incompatflags:" << fsinfo.value("incompatflags").toUInt() << "readonlyflags:" << fsinfo.value("readonlyflags").toUInt();
        if(((fsinfo.value("compatflags").toUInt() & 0x00000200UL) != 0) || ((fsinfo.value("incompatflags").toUInt() & 0x0001f7c0UL) != 0) || ((fsinfo.value("readonlyflags").toUInt() & 0x00000378UL) != 0))
            fsinfo.insert("typestr", QVariant("EXT4"));
        else if(((fsinfo.value("compatflags").toUInt() & 0x00000004UL) != 0) || ((fsinfo.value("incompatflags").toUInt() & 0x0000000cUL) != 0))
            fsinfo.insert("typestr", QVariant("EXT3"));
        else
            fsinfo.insert("typestr", QVariant("EXT2"));
        uint32_t blockgroupcount = fsinfo.value("fsblockcnt").toUInt() / fsinfo.value("blockgroupblockcnt").toUInt();
        if(blockgroupcount == 0)
            blockgroupcount = 1;
        QString inodeaddresstable = "";
        for(uint i=0; i < blockgroupcount; i++)
            inodeaddresstable += QString::number(qFromLittleEndian<uint32_t>(partbuf.mid(2048 + i*32 + 8, 4))) + ",";
        //qDebug() << "inodeaddresstable:" << inodeaddresstable;
        fsinfo.insert("inodeaddresstable", QVariant(inodeaddresstable));
        //qDebug() << "blocks for group descriptor table:" << (32 * (fsinfo.value("fsblockcnt").toUInt() / fsinfo.value("blockgroupblockcnt").toUInt())) / fsinfo.value("blocksize").toUInt();
        /*
        if(fsinfo.value("blockgroupinodecnt").toUInt() > 2)
            fsinfo.insert("rootinodetablestartingaddress", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(2056, 4))));
        else
            fsinfo.insert("rootinodetablestartingaddress", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(2088, 4))));
        */
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
    // need to implement iso, udf, hfs, zfs
    fsinfolist->append(fsinfo);
}

// QtConcurrent::map(QList<DirEntryInfo> direntrylist, ProcessFileInformation);
//ParseFileSystemInformation(QByteArray* initbuffer, int fstype, QList<FileSystemInfo>* fsinfolist)

void ParseExtDirectory(QString estring, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, qulonglong curinode)
{
    QStringList blockgroups = fsinfo->value("inodeaddresstable").toString().split(",", Qt::SkipEmptyParts);
    qulonglong inodetablestartingblock = 0;
    for(int i=1; i <= blockgroups.count(); i++)
    {
        if(curinode < i*fsinfo->value("blockgroupinodecnt").toUInt())
            inodetablestartingblock = blockgroups.at(i-1).toULongLong();
            //qDebug() << "curinode is located in block group inode table at block:" << blockgroups.at(i-1);
    }
    //qDebug() << "curinode:" << curinode;
    //qDebug() << "inodetablestartingblock:" << inodetablestartingblock;
    //qDebug() << "inode table byteoffset:" << inodetablestartingblock * fsinfo->value("blocksize").toUInt();
    //qDebug() << "inode address table:" << fsinfo->value("inodeaddresstable").toString();
    //qDebug() << "inodes per block group:" << fsinfo->value("blockgroupinodecnt").toUInt();
    /*
        if(fsinfo.value("blockgroupinodecnt").toUInt() > 2)
            fsinfo.insert("rootinodetablestartingaddress", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(2056, 4))));
        else
            fsinfo.insert("rootinodetablestartingaddress", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(2088, 4))));
    */
    QHash<QString, QVariant> fileinfo;
    fileinfo.clear();
    QByteArray inodetablebuf;
    inodetablebuf.clear();
    QFile efile(estring);
    if(!efile.isOpen())
        efile.open(QIODevice::ReadOnly);
    if(efile.isOpen())
    {
        efile.seek(fsinfo->value("partoffset").toUInt() + (inodetablestartingblock * fsinfo->value("blocksize").toUInt()));
        inodetablebuf = efile.read(128 * fsinfo->value("blockgroupinodecnt").toUInt());
        efile.close();
    }
    // NOW I HAVE THE INODE TABLE FOR THE CURRENT BLOCK GROUP. I CAN GO THE CURINODE's OFFSET and parse it...
    QList<uint32_t> blocklist;
    blocklist.clear();
    for(int i=0; i < 12; i++)
    {
        uint32_t curdirectblock = qFromLittleEndian<uint32_t>(inodetablebuf.mid((curinode-1)*128 + (40 + i*4), 4));
        if(curdirectblock > 0)
            blocklist.append(curdirectblock);
            //qDebug() << "direct block pointer for root inode:" << curdirectblock;
        //qDebug() << "direct block value for i:" << i << qFromLittleEndian<uint32_t>(inodetablebuf.mid((curinode-1)*128 + (40 + i*4), 4));
        //if(inodetablebuf.mid((curinode - 1)*128 + (40 + i*4), 4)
        //qDebug() << "direct block pointer for root inode...
    }
    qDebug() << "current block list before i get the indirect block pointers.." << blocklist;
    uint32_t singleindirect = qFromLittleEndian<uint32_t>(inodetablebuf.mid((curinode-1)*128 + 88, 4));
    uint32_t doubleindirect = qFromLittleEndian<uint32_t>(inodetablebuf.mid((curinode-1)*128 + 92, 4));
    uint32_t tripleindirect = qFromLittleEndian<uint32_t>(inodetablebuf.mid((curinode-1)*128 + 96, 4));
    // NEED TO PARSE THESE BLOCKS TO ADD TO THE BLOCKLIST's TOTAL
    if(singleindirect > 0)
    {
        qDebug() << "obtain and parse to add to my blocklist...";
    }
    if(doubleindirect > 0)
    {
        qDebug() << "follow and double parse to add to my blocklist...";
    }
    if(tripleindirect > 0)
    {
        qDebug() << "follow and triple parse to add to my blocklist...";
    }
    QByteArray direntrybuf;
    direntrybuf.clear();
    if(!efile.isOpen())
        efile.open(QIODevice::ReadOnly);
    if(efile.isOpen())
    {
        for(int i=0; i < blocklist.count(); i++)
        {
            efile.seek(fsinfo->value("partoffset").toUInt() + (blocklist.at(i) * fsinfo->value("blocksize").toUInt()));
            direntrybuf.append(efile.read(fsinfo->value("blocksize").toUInt()));
        }
        efile.close();
    }
    int curoffset = 24;
    //qDebug() << "incompatflags:" << fsinfo->value("incompatflags").toUInt();
    //fsinfo.insert("incompatflags", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(1120, 4))));
    while(curoffset < direntrybuf.count() - 8)
    {
        int entrylength = 0;
        int lengthdiv = (8 + qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 6))) / 4;
        int remdiv = (8 + qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 6))) % 4;
        int newlength = lengthdiv * 4 + 4;
        fileinfo.insert("inode", QVariant(qFromLittleEndian<uint32_t>(direntrybuf.mid(curoffset, 4))));
        if(fileinfo.value("inode").toUInt() > 0)
        {
            int namelength = 0;
            int filetype = 0;
            entrylength = qFromLittleEndian<uint16_t>(direntrybuf.mid(curoffset + 4, 2));
            if(fsinfo->value("incompatflags").toUInt() & 0x0002)
            {
                namelength = qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 6));
                filetype = qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 7));
            }
            else
                namelength = qFromLittleEndian<uint16_t>(direntrybuf.mid(curoffset + 6, 2));
            fileinfo.insert("filename", QVariant(QString::fromStdString(direntrybuf.mid(curoffset + 8, namelength).toStdString())));
            //qDebug() << "filetype:" << filetype;
            // NEED TO USE THE INODE TO THEN GET THE RELEVANT METADATA...
            // FILE TYPE GETS US INFO, SO IF IT'S A DIRECTORY, WE CAN PARSE THE DIRECTORY INODE, WITH THIS RECURSIVE FUNCTION...
            
            //qDebug() << "curoffset:" << curoffset;
            //qDebug() << "inode value:" << qFromLittleEndian<uint32_t>(direntrybuf.mid(curoffset, 4));
            //qDebug() << "entry length:" << qFromLittleEndian<uint16_t>(direntrybuf.mid(curoffset + 4, 2));
            //qDebug() << "name length:" << qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 6));
            //qDebug() << "file type:" << qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 7));
            //qDebug() << "file name:" << QString::fromStdString(direntrybuf.mid(curoffset + 8, qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 6))).toStdString());
            if(newlength == entrylength)
                fileinfo.insert("isdeleted", QVariant(0));
            else
            {
                if(newlength < entrylength && entrylength < 264)
                    fileinfo.insert("isdeleted", QVariant(1));
                else
                    fileinfo.insert("isdeleted", QVariant(0));
            }
            if(filetype == 1) // FILE
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
            QByteArray curinodebuf = inodetablebuf.mid(128 * (fileinfo.value("inode").toUInt() - 1), 128);
            //fileinfo.insert("
            //qDebug() << "curinodebuf access time:" << qFromLittleEndian<uint32_t>(curinodebuf.mid(8, 4));
            //qDebug() << "item type:" << fileinfo.value("itemtype").toUInt();
            //qDebug() << "newlength:" << newlength << "entrylength:" << entrylength;
        }
        //qDebug() << "length div:" << lengthdiv << "remdiv:" << remdiv;
        //qDebug() << "new length:" << newlength;
        curoffset += newlength;
            //fileinfo.insert("restname", QString::fromStdString(rootdirbuf.mid(i*32 + 1, 7).toStdString()).replace(" ", ""));
        //curoffset += 7 + qFromLittleEndian<uint8_t>(direntrybuf.at(curoffset + 6));
    }
    //qDebug() << "direntrybuf count:" << direntrybuf.count();
    //qDebug() << "direntry 1st 64 bytes:" << direntrybuf.left(64).toHex();
}

void ParseExFatDirEntry(QString estring, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist)
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
    QFile efile(estring);
    if(!efile.isOpen())
	efile.open(QIODevice::ReadOnly);
    if(efile.isOpen())
    {
	QStringList rootdirlayoutlist = fsinfo->value("rootdirlayout").toString().split(";", Qt::SkipEmptyParts);
	for(int i=0; i < rootdirlayoutlist.count(); i++)
	{
	    efile.seek(rootdirlayoutlist.at(i).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
	    rootdirbuf.append(efile.read(rootdirlayoutlist.at(i).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
	}
	efile.seek(fsinfo->value("fatoffset").toUInt());
	fatbuf = efile.read(fsinfo->value("fatsize").toUInt() * fsinfo->value("bytespersector").toUInt());
	efile.close();
    }
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
                ParseExfatSubDir(estring, fsinfo, &fileinfo, fileinfolist, &inodecnt, &fatbuf);
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
    if(!efile.isOpen())
        efile.open(QIODevice::ReadOnly);
    if(efile.isOpen())
    {
        int coffset = 0;
        while(!efile.atEnd())
        {
            QHash<QString, QVariant> orphaninfo;
            orphaninfo.clear();
            efile.seek(coffset);
            QByteArray tmparray = efile.read(32);
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
                efile.seek(coffset + 32);
                QByteArray tmp40array = efile.read(32);
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
                    if(namelength > 0 && namelength < 256)
                    {
                        efile.seek(coffset + 64);
                        QByteArray tmp41array = efile.read((tmparray.at(1)-1) * 32);
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
        efile.close();
    }
}

void ParseExfatSubDir(QString estring, QHash<QString, QVariant>* fsinfo, QHash<QString, QVariant>* parfileinfo, QList<QHash<QString, QVariant>>* fileinfolist, uint* inodecnt, QByteArray* fatbuf)
{
    QString clustersize = QString::number(fsinfo->value("sectorspercluster").toUInt() * fsinfo->value("bytespersector").toUInt());
    QString clusterstr = "";
    uint curinode = *inodecnt;
    QHash<QString, QVariant> fileinfo;
    fileinfo.clear();
    QByteArray dirbuf;
    dirbuf.clear();
    QStringList layoutlist = parfileinfo->value("layout").toString().split(";", Qt::SkipEmptyParts);
    QFile efile(estring);
    if(!efile.isOpen())
        efile.open(QIODevice::ReadOnly);
    if(efile.isOpen())
    {
        for(int i=0; i < layoutlist.count(); i++)
        {
            efile.seek(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
            dirbuf.append(efile.read(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
        }
        efile.close();
    }
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
                ParseExfatSubDir(estring, fsinfo, &fileinfo, fileinfolist, inodecnt, fatbuf);
                curinode = *inodecnt;
            }
        }
    }
}

//void ParseDirectory(QString estring, QHash<QString, QVariant> *fsinfo, QList<QHash<QString, QVariant>> *fileinfolist, QList<QString>* orphanlist)
void ParseFatDirEntry(QString estring, QHash<QString, QVariant> *fsinfo, QList<QHash<QString, QVariant>> *fileinfolist, QList<QHash<QString, QVariant>>* orphanlist)
{
    QHash<QString, QVariant> fileinfo;
    QHash<QString, QVariant> orphaninfo;
    fileinfo.clear();
    QByteArray fatbuf;
    fatbuf.clear();
    QByteArray rootdirbuf;
    rootdirbuf.clear();
    int rootdirentrycount = 0;
    QFile efile(estring);
    if(!efile.isOpen())
        efile.open(QIODevice::ReadOnly);
    if(efile.isOpen())
    {
	//qDebug() << "rootdirlayout:" << fsinfo->value("rootdirlayout").toString();
	QStringList rootdirlayoutlist = fsinfo->value("rootdirlayout").toString().split(";", Qt::SkipEmptyParts);
	for(int j=0; j < rootdirlayoutlist.count(); j++)
	{
	    efile.seek(rootdirlayoutlist.at(j).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
	    rootdirbuf.append(efile.read(rootdirlayoutlist.at(j).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
	}
	efile.seek(fsinfo->value("fatoffset").toUInt());
        fatbuf = efile.read(fsinfo->value("fatsize").toUInt() * fsinfo->value("bytespersector").toUInt());
	efile.close();
    }
    //qDebug() << "dir fat content:" << fatbuf.mid(0, 40).toHex();
    //qDebug() << "rootdirbuf.count():" << rootdirbuf.count();
    rootdirentrycount = rootdirbuf.count() / 32;
    //qDebug() << "rootdirentrycount:" << rootdirentrycount;
    uint inodecnt = 0;
    QString longnamestring = "";
    //for(uint i=0; i < fsinfo->value("rootdirmaxfiles").toUInt(); i++)
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
            /*
	    if(fileattr & 0x08)
	    {
		//qDebug() << "volume directory entry, will need to specially format it, but for now skip it.";
		// THAT DOESN'T FAIL THE PARSING, BUT IT BREAKS OUT OF THE LOOP, SO I NEED TO DO A PROPER IF AND HANDLE THIS ATTR TYPE... AND THEN HANDLE THE OTHER'S AS BEFORE...
		// PROBABLY JUST INDENT EVERYTHING BELOW UNDER THE ELSE TO THIS IF AND MAKE THE SIMPLE VOLUME DIRENTRY TREE ENTRY HERE.
		//break;
	    }
            */
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
		if(!efile.isOpen())
		    efile.open(QIODevice::ReadOnly);
		if(efile.isOpen())
		{
		    for(int j=0; j < layoutlist.count(); j++)
		    {
			efile.seek(layoutlist.at(j).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
			dirsizebuf.append(efile.read(layoutlist.at(j).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
		    }
		    efile.close();
		}
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
		    ParseSubDirectory(estring, fsinfo, &fileinfo, fileinfolist, &inodecnt, &fatbuf, orphanlist);
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

void ParseSubDirectory(QString estring, QHash<QString, QVariant>* fsinfo, QHash<QString, QVariant>* parentinfo, QList<QHash<QString, QVariant>>* fileinfolist, uint* inodecnt, QByteArray* fatbuf, QList<QHash<QString, QVariant>>* orphanlist)
{
    uint curinode = *inodecnt;
    QHash<QString, QVariant> fileinfo;
    QHash<QString, QVariant> orphaninfo;
    fileinfo.clear();
    QByteArray dirbuf;
    dirbuf.clear();
    QStringList layoutlist = parentinfo->value("layout").toString().split(";", Qt::SkipEmptyParts);
    QFile efile(estring);
    if(!efile.isOpen())
        efile.open(QIODevice::ReadOnly);
    if(efile.isOpen())
    {
	for(int i=0; i < layoutlist.count(); i++)
	{
	    if(i == 0)
	    {
		efile.seek(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(0).toULongLong() + 64); // # + 64 skips the first 2 dir entries of . and ..
		dirbuf.append(efile.read(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(1).toULongLong() - 64)); // # - 64 accounts for skipping the first 2 dir entries of . and ..
	    }
	    else
	    {
		efile.seek(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
		dirbuf.append(efile.read(layoutlist.at(i).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
	    }
	}
        efile.close();
    }
    qDebug() << "dir fat content:" << fatbuf->mid(0, 40).toHex();
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
		if(!efile.isOpen())
		    efile.open(QIODevice::ReadOnly);
		if(efile.isOpen())
		{
		    for(int j=0; j < layoutlist.count(); j++)
		    {
			efile.seek(layoutlist.at(j).split(",", Qt::SkipEmptyParts).at(0).toULongLong());
			dirsizebuf.append(efile.read(layoutlist.at(j).split(",", Qt::SkipEmptyParts).at(1).toULongLong()));
		    }
		    efile.close();
		}
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
		    ParseSubDirectory(estring, fsinfo, &fileinfo, fileinfolist, inodecnt, fatbuf, orphanlist);
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

void ProcessVolume(QString evidstring)
{
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
    if(evidstring.toLower().endsWith(".e01") || evidstring.toLower().endsWith(".aff") || evidstring.toLower().endsWith(".000") || evidstring.toLower().endsWith("001") || evidstring.toLower().endsWith(".zmg"))
        emntstring = wombatvariable.imgdatapath + evidstring.split("/").last() + "/" + evidstring.split("/").last() + ".raw";
    QFileInfo efileinfo(emntstring);
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
    ParseVolume(emntstring, imgsize, &pofflist, &psizelist, &fsinfolist);
    if(pofflist.count() == 0)
    {	
	// virtual attempt to process entire image as a filesystem...
	pofflist.append(0);
	psizelist.append(imgsize/512);
        ParseFileSystemInformation(emntstring, 0, &fsinfolist);
    }
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
	    QList<QHash<QString, QVariant>> orphanlist;
            // IF FAT12/16/32 THEN
            // MAYBE CHANGE NAME FROM PARSEDIRECTORY TO PARSEFATDIRENTRY
	    if(fsinfolist.at(i).value("type").toUInt() == 1 || fsinfolist.at(i).value("type").toUInt() == 2 || fsinfolist.at(i).value("type").toUInt() == 3) // FAT12 || FAT16 || FAT32
		ParseFatDirEntry(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);
	    else if(fsinfolist.at(i).value("type").toUInt() == 4) // EXFAT
		ParseExFatDirEntry(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);
            else if(fsinfolist.at(i).value("type").toUInt() == 6) // EXT2/3/4
                ParseExtDirectory(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, 2);
            //ParseDirectory(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);

            PopulateFiles(emntstring, curpartpath, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, evidcnt, ptreecnt); 
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
            QList<QHash<QString, QVariant>> orphanlist;
            // IF FAT12/16/32 THEN
            // MAYBE CHANGE NAME FROM PARSEDIRECTORY TO PARSEFATDIRENTRY
	    if(fsinfolist.at(i).value("type").toUInt() == 1 || fsinfolist.at(i).value("type").toUInt() == 2 || fsinfolist.at(i).value("type").toUInt() == 3) // FAT12 || FAT16 || FAT32
		ParseFatDirEntry(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);
	    else if(fsinfolist.at(i).value("type").toUInt() == 4) // EXFAT
		ParseExFatDirEntry(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);
            else if(fsinfolist.at(i).value("type").toUInt() == 6) // EXT2/3/4
                ParseExtDirectory(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, 2);
            //ParseDirectory(emntstring, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist);
            // ELSE EXFAT THEN
            // ELSE ... THEN

            PopulateFiles(emntstring, curpartpath, (QHash<QString, QVariant>*)&(fsinfolist.at(i)), &fileinfolist, &orphanlist, evidcnt, ptreecnt); 
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

void PopulateFiles(QString emntstring, QString curpartpath, QHash<QString, QVariant>* fsinfo, QList<QHash<QString, QVariant>>* fileinfolist, QList<QHash<QString, QVariant>>* orphanlist, int evidcnt, int ptreecnt)
{
    QList<QVariant> nodedata;
    nodedata.clear();
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
        //qDebug() << "alias name:" << fileinfolist.at(j).value("aliasname").toString() << "long name:" << fileinfolist.at(j).value("longname").toString();
        ba.append(fileinfolist->at(j).value("path").toString().toUtf8());
        nodedata << ba.toBase64() << QVariant(fileinfolist->at(j).value("logicalsize").toUInt()) << QVariant(fileinfolist->at(j).value("createdate").toUInt()) << QVariant(fileinfolist->at(j).value("accessdate").toUInt()) << QVariant(fileinfolist->at(j).value("modifydate").toUInt()) << QVariant("0") << QVariant("0");
        // THIS CAN BE MOVED TO PARSEDIRECTORY/SUBDIRECTORY SECTION....
        if(fileinfolist->at(j).value("itemtype").toUInt() == 3 && fileinfolist->at(j).value("isdeleted").toInt() == 0)
            nodedata << QVariant("Directory") << QVariant("Directory"); // category << signature
        else
        {
            QByteArray sigbuf;
            sigbuf.clear();
            QFile efile(emntstring);
            if(!efile.isOpen())
                efile.open(QIODevice::ReadOnly);
            if(efile.isOpen())
            {
                efile.seek(fileinfolist->at(j).value("layout").toString().split(";").at(0).split(",").at(0).toULongLong());
                sigbuf = efile.read(fileinfolist->at(j).value("layout").toString().split(";").at(0).split(",").at(1).toULongLong());
                efile.close();
            }
            QMimeDatabase mimedb;
            const QMimeType mimetype = mimedb.mimeTypeForFileNameAndData(fileinfolist->at(j).value("filename").toString(), sigbuf);
            QString mimestr = GenerateCategorySignature(mimetype);
	    //qDebug() << "mimestr:" << mimestr;
            if(mimestr.contains("Unknown")) // generate further analysis
            {
                if(sigbuf.at(0) == '\x4c' && sigbuf.at(1) == '\x00' && sigbuf.at(2) == '\x00' && sigbuf.at(3) == '\x00' && sigbuf.at(4) == '\x01' && sigbuf.at(5) == '\x14' && sigbuf.at(6) == '\x02' && sigbuf.at(7) == '\x00') // LNK File
                    mimestr = "Windows System/Shortcut";
                else if(strcmp(fileinfolist->at(j).value("filename").toString().toStdString().c_str(), "INFO2") == 0 && (sigbuf.at(0) == 0x04 || sigbuf.at(0) == 0x05))
                    mimestr = "Windows System/Recycler";
                else if(fileinfolist->at(j).value("filename").toString().startsWith("$I") && (sigbuf.at(0) == 0x01 || sigbuf.at(0) == 0x02))
                    mimestr = "Windows System/Recycle.Bin";
                else if(fileinfolist->at(j).value("filename").toString().endsWith(".pf") && sigbuf.at(4) == 0x53 && sigbuf.at(5) == 0x43 && sigbuf.at(6) == 0x43 && sigbuf.at(7) == 0x41)
                    mimestr = "Windows System/Prefetch";
                else if(fileinfolist->at(j).value("filename").toString().endsWith(".pf") && sigbuf.at(0) == 0x4d && sigbuf.at(1) == 0x41 && sigbuf.at(2) == 0x4d)
                    mimestr = "Windows System/Prefetch";
                else if(sigbuf.at(0) == '\x72' && sigbuf.at(1) == '\x65' && sigbuf.at(2) == '\x67' && sigbuf.at(3) == '\x66') // 72 65 67 66 | regf
                    mimestr = "Windows System/Registry";
                else if(fileinfolist->at(j).value("filename").toString().startsWith("$ALLOC_BITMAP"))
                    mimestr = "System File/Allocation Bitmap";
            }
            if(fileinfolist->at(j).value("filename").toString().startsWith("$UPCASE_TABLE"))
                mimestr = "System File/Up-case Table";
            nodedata << QVariant(mimestr.split("/").at(0)) << QVariant(mimestr.split("/").at(1)); // category << signature
        }
        nodedata << QVariant("0") << QVariant(QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-f" + QString::number(fileinfolist->at(j).value("inode").toUInt())));
        mutex.lock();
        treenodemodel->AddNode(nodedata, parentstr, fileinfolist->at(j).value("itemtype").toInt(), fileinfolist->at(j).value("isdeleted").toInt());
        mutex.unlock();
        if(nodedata.at(11).toString().split("-").count() == 3)
        {
            listeditems.append(nodedata.at(11).toString());
            //listeditems.append(treeout.at(11));
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
            QFile efile(emntstring);
            if(!efile.isOpen())
                efile.open(QIODevice::ReadOnly);
            if(efile.isOpen())
            {
                efile.seek(orphanlist->at(j).value("layout").toString().split(";").at(0).split(",").at(0).toULongLong());
                sigbuf = efile.read(orphanlist->at(j).value("layout").toString().split(";").at(0).split(",").at(1).toULongLong());
                efile.close();
            }
            QMimeDatabase mimedb;
            const QMimeType mimetype = mimedb.mimeTypeForFileNameAndData(orphanlist->at(j).value("filename").toString(), sigbuf);
            mimestr = GenerateCategorySignature(mimetype);
	    //qDebug() << "mimestr:" << mimestr;
            if(mimestr.contains("Unknown")) // generate further analysis
            {
                if(sigbuf.at(0) == '\x4c' && sigbuf.at(1) == '\x00' && sigbuf.at(2) == '\x00' && sigbuf.at(3) == '\x00' && sigbuf.at(4) == '\x01' && sigbuf.at(5) == '\x14' && sigbuf.at(6) == '\x02' && sigbuf.at(7) == '\x00') // LNK File
                    mimestr = "Windows System/Shortcut";
                else if(strcmp(orphanlist->at(j).value("filename").toString().toStdString().c_str(), "INFO2") == 0 && (sigbuf.at(0) == 0x04 || sigbuf.at(0) == 0x05))
                    mimestr = "Windows System/Recycler";
                else if(orphanlist->at(j).value("filename").toString().startsWith("$I") && (sigbuf.at(0) == 0x01 || sigbuf.at(0) == 0x02))
                    mimestr = "Windows System/Recycle.Bin";
                else if(orphanlist->at(j).value("filename").toString().endsWith(".pf") && sigbuf.at(4) == 0x53 && sigbuf.at(5) == 0x43 && sigbuf.at(6) == 0x43 && sigbuf.at(7) == 0x41)
                    mimestr = "Windows System/Prefetch";
                else if(orphanlist->at(j).value("filename").toString().endsWith(".pf") && sigbuf.at(0) == 0x4d && sigbuf.at(1) == 0x41 && sigbuf.at(2) == 0x4d)
                    mimestr = "Windows System/Prefetch";
                else if(sigbuf.at(0) == '\x72' && sigbuf.at(1) == '\x65' && sigbuf.at(2) == '\x67' && sigbuf.at(3) == '\x66') // 72 65 67 66 | regf
                    mimestr = "Windows System/Registry";
                else if(orphanlist->at(j).value("filename").toString().startsWith("$ALLOC_BITMAP"))
                    mimestr = "System File/Allocation Bitmap";
            }
            if(orphanlist->at(j).value("filename").toString().startsWith("$UPCASE_TABLE"))
                mimestr = "System File/Up-case Table";
            if(orphanlist->at(j).value("itemtype").toUInt() == 2)
                mimestr = "Directory/Directory";
        }
            //nodedata << QVariant(mimestr.split("/").at(0)) << QVariant(mimestr.split("/").at(1)); // category << signature

        QString curid = QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt) + "-f" + QString::number(curinode + j));
        nodedata.clear();
        ba.clear();
        ba.append(orphanlist->at(j).value("filename").toString().toUtf8());
        //ba.append(orphanlist->at(j).toUtf8());
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
        // might need an if(fileinfo->value("type").toUInt() == 1) // FAT12
        out << "Alias Name|" << fileinfo->value("aliasname").toString() << "|8.3 file name" << Qt::endl;
        out << "File Attributes|" << fileinfo->value("attribute").toString() << "|File attributes." << Qt::endl;
        out << "Cluster List|" << fileinfo->value("clusterlist").toString() << "|Clusters occupied by the file." << Qt::endl;
        out << "Layout|" << fileinfo->value("layout").toString() << "|File offset,size; layout." << Qt::endl;
        out << "Physical Size|" << QString::number(fileinfo->value("physicalsize").toUInt()) << "|Sector Size in Bytes for the file." << Qt::endl;
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
        out.flush();
        fspropfile.close();
    }
    //qDebug() << "fsinfo type:" << fsinfo->value("typestr");
}

void InitializeEvidenceStructure(QString evidname)
{
    int evidcnt = 0;
    int volcnt = 0;
    int partint = 0;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList(QString(evidname.split("/").last() + ".e*")), QDir::NoSymLinks | QDir::Dirs);
    evidcnt = evidfiles.at(0).split(".e").last().toInt();
    QString evidencename = evidname.split("/").last();
    const TSK_TCHAR** images;
    std::vector<std::string> fullpathvector;
    fullpathvector.clear();
    if(evidname.endsWith(".sfs"))
        fullpathvector.push_back(QString(wombatvariable.imgdatapath + evidencename + "/" + evidencename.split(".sfs").first() + ".dd").toStdString());
    else if(evidname.endsWith(".zmg"))
        fullpathvector.push_back(QString(wombatvariable.imgdatapath + evidencename + "/" + evidencename.split(".zmg").first() + ".dd").toStdString());
    else if(evidname.toLower().endsWith(".aff") || evidname.endsWith(".000") || evidname.endsWith(".001"))
    {
        if(evidname.toLower().endsWith(".aff"))
            fullpathvector.push_back(QString(wombatvariable.imgdatapath + evidencename + "/" + evidencename + ".raw").toStdString());
        else if(evidname.endsWith(".000"))
            fullpathvector.push_back(QString(wombatvariable.imgdatapath + evidencename + "/" + evidencename + ".raw").toStdString());
        else if(evidname.endsWith(".001"))
            fullpathvector.push_back(QString(wombatvariable.imgdatapath + evidencename + "/" + evidencename + ".raw").toStdString());
    }
    else if(evidname.toLower().endsWith(".e01"))
        fullpathvector.push_back(QString(wombatvariable.imgdatapath + evidencename + "/" + evidencename + ".raw").toStdString());
    else
        fullpathvector.push_back(evidname.toStdString());
    images = (const char**)malloc(fullpathvector.size()*sizeof(char*));
    for(uint i=0; i < fullpathvector.size(); i++)
    {
        images[i] = fullpathvector[i].c_str();
    }
    TSK_IMG_INFO* imginfo = NULL;
    imginfo = tsk_img_open(1, images, TSK_IMG_TYPE_DETECT, 0);
    free(images);
    /*
    QString efilepath = imgfile.split(imgfile.split("/").last()).first();
    QDir edir = QDir(imgfile.split(imgfile.split("/").last()).first());
    QStringList efiles = edir.entryList(QStringList() << QString(imgfile.split("/").last().toLower().split(".e01").first() + ".e??") << QString(imgfile.split("/").last().toLower().split(".e01").first() + ".E??"), QDir::NoSymLinks | QDir::Files);
    char* filenames[efiles.count()] = {NULL};
    for(int i=0; i < efiles.count(); i++)
    {
        filenames[i] = QString(efilepath + efiles.at(i)).toLatin1().data();
        printf("filenames[%d] = %s\n", i, filenames[i]);
    }
    globfilecnt = efiles.count();
     */ 
    QString evidencepath = wombatvariable.tmpmntpath + evidencename + ".e" + QString::number(evidcnt) + "/";
    QTextStream out;
    QFile evidfile(evidencepath + "stat");
    if(!evidfile.isOpen())
        evidfile.open(QIODevice::Append | QIODevice::Text);
    if(evidfile.isOpen())
    {
        out.setDevice(&evidfile);
        out << QString::number(imginfo->itype) << "," << QString::number(imginfo->size) << "," << QString::number(imginfo->sector_size) << ",";
        out << evidname << "," << QString::number(1) << ",e" + QString::number(evidcnt) << ",0";
        out.flush();
        evidfile.close();
    }
    QStringList treeout;
    treeout << evidencename << "0" << QString::number(imginfo->size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt)); // NAME IN FIRST COLUMN
    QList<QVariant> nodedata;
    nodedata.clear();
    for(int i=0; i < treeout.count(); i++)
        nodedata << treeout.at(i);
    mutex.lock();
    treenodemodel->AddNode(nodedata,  "-1", -1, -1);
    mutex.unlock();
    // Write Evidence Properties Here...
    if(imginfo != NULL)
        WriteEvidenceProperties(imginfo, evidencepath, evidname);
    TSK_VS_INFO* vsinfo = NULL;
    vsinfo = tsk_vs_open(imginfo, 0, TSK_VS_TYPE_DETECT);
    QString volname = "Dummy Volume (NO PART)";
    int voltype = 240;
    int volsectorsize = (int)imginfo->sector_size;
    qint64 voloffset = 0;
    if(vsinfo != NULL)
    {
        voltype = (int)vsinfo->vstype;
        volname = QString::fromUtf8(tsk_vs_type_todesc(vsinfo->vstype));
        volsectorsize = (int)vsinfo->block_size;
        voloffset = (qint64)vsinfo->offset;
    }
    QDir voldir = QDir(evidencepath);
    QStringList volfiles = voldir.entryList(QStringList(QString("v*")), QDir::NoSymLinks | QDir::Dirs);
    volcnt = volfiles.count();
    QString volumepath = evidencepath + "v" + QString::number(volcnt) + "/";
    QDir dir;
    dir.mkpath(volumepath);
    QFile volfile(volumepath + "stat");
    if(!volfile.isOpen())
        volfile.open(QIODevice::Append | QIODevice::Text);
    if(volfile.isOpen())
    {
        out.setDevice(&volfile);
        out << voltype << "," << (qint64)imginfo->size << "," << volname << "," << volsectorsize << "," << voloffset << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) << ",0";
        out.flush();
        volfile.close();
    }
    treeout.clear();
    treeout << volname << "0" << QString::number(imginfo->size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)); // NAME IN FIRST COLUMN
    nodedata.clear();
    for(int i=0; i < treeout.count(); i++)
        nodedata << treeout.at(i);
    QString reportstring = "";
    mutex.lock();
    reportstring = "<div id='e" + QString::number(evidcnt) + "'><table width='98%'>";
    reportstring += "<tr><th colspan='2'>Evidence Item (E" + QString::number(evidcnt) + "):" + evidname + "</th></tr>";
    reportstring += "<tr class='odd vtop'><td>Image Size:</td><td>" + QString::number(imginfo->size) + " bytes</td></tr>";
    reportstring += "<tr class='even vtop'><td>Sector Size:</td><td>" + QString::number(imginfo->sector_size) + " bytes</td></tr>";
    reportstring += "<tr class='odd vtop'><td>Volume (V" + QString::number(volcnt) + "):</td><td>" + volname + "</td></tr>";
    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), -1, 0);
    mutex.unlock();
    // ADD ManualCarved Folder HERE
    treeout.clear();
    treeout << "$Manual Carved" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-mc");
    nodedata.clear();
    for(int i=0; i < treeout.count(); i++)
        nodedata << treeout.at(i);
    mutex.lock();
    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), 3, 0);
    mutex.unlock();
    const TSK_POOL_INFO* poolinfo = nullptr;
    TSK_IMG_INFO* curimginfo = NULL;
    TSK_FS_INFO* fsinfo = NULL;
    if(vsinfo != NULL)
        WriteVolumeProperties(vsinfo, volumepath);
    if(vsinfo == NULL) // no volume, so a single file system is all there is in the image
    {
        QString pooldesc = "";
        poolinfo = tsk_pool_open_img_sing(imginfo, 0, TSK_POOL_TYPE_APFS);
        if(poolinfo != nullptr) // contains a pool...
        {
            if(poolinfo->num_vols > 0)
            {
                for(int i=0; i < poolinfo->num_vols; i++)
                {
                    TSK_POOL_VOLUME_INFO curpoolvol = poolinfo->vol_list[i];
                    pooldesc = curpoolvol.desc;
                    curimginfo = poolinfo->get_img_info(poolinfo, curpoolvol.block);
                    if(curpoolvol.flags & TSK_POOL_VOLUME_FLAG_ENCRYPTED)
                    {
                        // CURRENTLY TSK DOESN'T WORK FOR ENCRYPTED APFS VOLUMES...
                        fsinfo = tsk_fs_open_img_decrypt(curimginfo, 0, TSK_FS_TYPE_APFS, passwordhash.value(evidname.split("/").last() + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)).toStdString().c_str());
                        //fsinfo = tsk_fs_open_vol_decrypt(partinfo, TSK_FS_TYPE_APFS, "apfspassword");
                        //fsinfo = tsk_fs_open_pool_decrypt(poolinfo, curpoolvol.block, TSK_FS_TYPE_APFS_DETECT, "apfspassword");
                    }
                    else
                    {
                        fsinfo = tsk_fs_open_img(curimginfo, 0, TSK_FS_TYPE_APFS_DETECT);
                        //fsinfo = tsk_fs_open_pool(poolinfo, curpoolvol.block, TSK_FS_TYPE_APFS_DETECT);
                    }
                    if(fsinfo == NULL) // unrecognized filesystem
                    {
                        QString partitionpath = volumepath + "p" + QString::number(partint) + "/";
                        dir.mkpath(partitionpath);
                        QFile pfile(partitionpath + "stat");
                        pfile.open(QIODevice::Append | QIODevice::Text);
                        out.setDevice(&pfile);
                        out << "240," << QString::number(imginfo->size) << ",NON-RECOGNIZED FS,0,0," << (qint64)imginfo->size/volsectorsize << "," << volsectorsize << ",0,0,0,e" << QString::number(evidcnt) + "-v0-p" + QString::number(partint) + ",0";
                        out.flush();
                        pfile.close();
                        treeout.clear();
                        treeout << "NON-RECOGNIZED FS" << "0" << QString::number(imginfo->size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v0-p" + QString::number(partint));
                        nodedata.clear();
                        for(int j=0; j < treeout.count(); j++)
                            nodedata << treeout.at(i);
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v0"), -1, 0);
                        mutex.unlock();
                        // ADD ManualCarved Folder HERE
                        treeout.clear();
                        treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v0-p" + QString::number(partint) + "-pc");
                        nodedata.clear();
                        for(int i=0; i < treeout.count(); i++)
                            nodedata << treeout.at(i);
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v0-p" + QString::number(partint)), 3, 0);
                        mutex.unlock();
			nodedata.clear();
			nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v0-p" + QString::number(partint) + "-uc");
			mutex.lock();
			treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v0-p" + QString::number(partint)), 3, 0);
			mutex.unlock();
                        reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(partint) + "):</td><td>NON RECOGNIZED FS</td></tr>";
                        partitionlist.append("e" + QString::number(evidcnt) + "-v0-p" + QString::number(partint) + ": NON-RECOGNIZED FS");
                    }
                    else
                    {
                        QString partitionpath = volumepath + "p" + QString::number(partint) + "/";
                        dir.mkpath(partitionpath);
                        QFile pfile(partitionpath + "stat");
                        pfile.open(QIODevice::Append | QIODevice::Text);
                        out.setDevice(&pfile);
                        out << fsinfo->ftype << "," << (qint64)fsinfo->block_size * (qint64)fsinfo->block_count << ",";
                        if(poolinfo == nullptr)
                            out << GetFileSystemLabel(fsinfo);
                        else
                            out << pooldesc;
                        out << "," << (qint64)fsinfo->root_inum << "," << (qint64)fsinfo->offset << "," << (qint64)fsinfo->block_count << "," << (int)fsinfo->block_size << ",0,0,0,e" << QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                        out.flush();
                        pfile.close();
                        QString fsdesc = "";
                        if(poolinfo == nullptr)
                            fsdesc = GetFileSystemLabel(fsinfo);
                        else
                            fsdesc = pooldesc;
                        treeout.clear();
                        treeout << QString(fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")") << "0" << QString::number(fsinfo->block_size * fsinfo->block_count) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                        nodedata.clear();
                        for(int j=0; j < treeout.count(); j++)
                            nodedata << treeout.at(j);
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                        mutex.unlock();
                        reportstring += "<tr class='even vtop'><td>Partition (P0):</td><td>" + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")</td></tr>";
                        WriteFileSystemProperties(fsinfo, partitionpath);
                        TSK_STACK* stack = NULL;
                        stack = tsk_stack_create();
                        ProcessDir(fsinfo, stack, fsinfo->root_inum, "/", evidcnt, volcnt, partint, partitionpath);
                        tsk_fs_close(fsinfo);
                        tsk_stack_free(stack);
                        // ADD ManualCarved Folder HERE
                        treeout.clear();
                        treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-pc");
                        nodedata.clear();
                        for(int i=0; i < treeout.count(); i++)
                            nodedata << treeout.at(i);
                        mutex.lock();
                        treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
                        mutex.unlock();
			nodedata.clear();
			nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-uc");
			mutex.lock();
			treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
			mutex.unlock();
                        partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + ": " + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
                    }
                    partint++;
                }
            }
        }
        else
        {
            fsinfo = tsk_fs_open_img(imginfo, 0, TSK_FS_TYPE_DETECT);
            if(fsinfo == NULL) // unrecognized filesystem
            {
                QString partitionpath = volumepath + "p0/";
                dir.mkpath(partitionpath);
                QFile pfile(partitionpath + "stat");
                pfile.open(QIODevice::Append | QIODevice::Text);
                out.setDevice(&pfile);
                out << "240," << QString::number(imginfo->size) << ",NON-RECOGNIZED FS,0,0," << (qint64)imginfo->size/volsectorsize << "," << volsectorsize << ",0,0,0,e" << QString::number(evidcnt) + "-v0-p0,0";
                out.flush();
                pfile.close();
                treeout.clear();
                treeout << "NON-RECOGNIZED FS" << "0" << QString::number(imginfo->size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v0-p0");
                nodedata.clear();
                for(int i=0; i < treeout.count(); i++)
                    nodedata << treeout.at(i);
                mutex.lock();
                treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v0"), -1, 0);
                mutex.unlock();
                // ADD ManualCarved Folder HERE
                treeout.clear();
                treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v0-p0-pc");
                nodedata.clear();
                for(int i=0; i < treeout.count(); i++)
                    nodedata << treeout.at(i);
                mutex.lock();
                treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v0-p0"), 3, 0);
                mutex.unlock();
		nodedata.clear();
		nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v0-p0-uc");
		mutex.lock();
		treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v0-p0"), 3, 0);
		mutex.unlock();
                reportstring += "<tr class='even vtop'><td>Partition (P0):</td><td>NON RECOGNIZED FS</td></tr>";
	        partitionlist.append("e" + QString::number(evidcnt) + "-v0-p0: NON-RECOGNIZED FS");
            }
            else
            {
                QString partitionpath = volumepath + "p0/";
                dir.mkpath(partitionpath);
                QFile pfile(partitionpath + "stat");
                pfile.open(QIODevice::Append | QIODevice::Text);
                out.setDevice(&pfile);
                out << fsinfo->ftype << "," << (qint64)fsinfo->block_size * (qint64)fsinfo->block_count << ",";
                if(poolinfo == nullptr)
                    out << GetFileSystemLabel(fsinfo);
                else
                    out << pooldesc;
                out << "," << (qint64)fsinfo->root_inum << "," << (qint64)fsinfo->offset << "," << (qint64)fsinfo->block_count << "," << (int)fsinfo->block_size << ",0,0,0,e" << QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                out.flush();
                pfile.close();
                QString fsdesc = "";
                if(poolinfo == nullptr)
                    fsdesc = GetFileSystemLabel(fsinfo);
                else
                    fsdesc = pooldesc;
                treeout.clear();
                treeout << QString(fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")") << "0" << QString::number(fsinfo->block_size * fsinfo->block_count) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                nodedata.clear();
                for(int i=0; i < treeout.count(); i++)
                    nodedata << treeout.at(i);
                mutex.lock();
                treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                mutex.unlock();
                reportstring += "<tr class='even vtop'><td>Partition (P0):</td><td>" + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")</td></tr>";
                WriteFileSystemProperties(fsinfo, partitionpath);
                TSK_STACK* stack = NULL;
                stack = tsk_stack_create();
                ProcessDir(fsinfo, stack, fsinfo->root_inum, "/", evidcnt, volcnt, partint, partitionpath);
                tsk_fs_close(fsinfo);
                tsk_stack_free(stack);
                // ADD ManualCarved Folder HERE
                treeout.clear();
                treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-pc");
                nodedata.clear();
                for(int i=0; i < treeout.count(); i++)
                    nodedata << treeout.at(i);
                mutex.lock();
                treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
                mutex.unlock();
		nodedata.clear();
		nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-uc");
		mutex.lock();
		treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
		mutex.unlock();
        	partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + ": " + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
            }
        }
    }
    else
    {
        QFile pfile;
        const TSK_VS_PART_INFO* partinfo = NULL;
        if(vsinfo->part_count > 0)
        {
            for(uint32_t i=0; i < vsinfo->part_count; i++)
            {
                partinfo = tsk_vs_part_get(vsinfo, i);
                if(partinfo == NULL)
                {
                    qDebug() << "tsk_vs_part_get error:";
                    tsk_error_print(stderr);
                }
                QString partitionpath = volumepath + "p" + QString::number(partint) + "/";
                dir.mkpath(partitionpath);
                pfile.setFileName(partitionpath + "stat");
                pfile.open(QIODevice::Append | QIODevice::Text);
                out.setDevice(&pfile);
                if(partinfo->flags == 0x02 || partinfo->flags == 0x04) // unallocated partition or meta entry
                {
                    out << "0," << (qint64)partinfo->len * vsinfo->block_size << "," << QString(partinfo->desc) << ",0," << (qint64)partinfo->start * (int)vsinfo->block_size << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << "," << partinfo->flags << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                    out.flush();
                    pfile.close();
                    treeout.clear();
                    treeout << QString(partinfo->desc) << "0" << QString::number(partinfo->len * vsinfo->block_size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                    nodedata.clear();
                    for(int j=0; j < treeout.count(); j++)
                        nodedata << treeout.at(j);
                    mutex.lock();
                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                    mutex.unlock();
                    reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(partint) + "):</td><td>" + QString(partinfo->desc) + "</td></tr>";
		    partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + ": " + QString(partinfo->desc));
                }
                else if(partinfo->flags == 0x01) // allocated partition
                {
                    QString pooldesc = "";
                    //qDebug() << "partinfo start:" << partinfo->start;
                    TSK_FS_INFO* fsinfo = NULL;
                    TSK_STACK* stack = NULL;
                    stack = tsk_stack_create();
                    const TSK_POOL_INFO* poolinfo = nullptr;
                    poolinfo = tsk_pool_open_sing(partinfo, TSK_POOL_TYPE_DETECT);
                    TSK_IMG_INFO* curimginfo = NULL;
		    //FILE* hfile = NULL;
		    //char* pstatcontent = NULL;
		    //size_t pstatsize;
		    //hfile = open_memstream(&pstatcontent, &pstatsize);
		    //poolinfo->poolstat(poolinfo, hfile);
		    //fclose(hfile);
		    // NOW I HAVE A CHAR* OF POOLSTAT OUTPUT. I NEED TO PARSE THIS AND STORE THAT IN THE POOL/VOL AND FS  PROPERTIES...
		    //qDebug() << "mem stream:" << pstatcontent;
		    //free(pstatcontent);
                    //poolinfo = nullptr;
                    poolinfo = tsk_pool_open_sing(partinfo, TSK_POOL_TYPE_DETECT);
                    if(poolinfo == nullptr)
                    {
                        fsinfo = tsk_fs_open_vol(partinfo, TSK_FS_TYPE_DETECT);
                        if(fsinfo != NULL)
                        {
                            out << fsinfo->ftype << "," << (qint64)fsinfo->block_size * (qint64)fsinfo->block_count << ",";
                            if(poolinfo == nullptr)
                                out << GetFileSystemLabel(fsinfo);
                            else
                                out << pooldesc;
                            out << "," << (qint64)fsinfo->root_inum << "," << (qint64)fsinfo->offset << "," << (qint64)fsinfo->block_count << "," << (int)fsinfo->block_size << "," << partinfo->flags << "," << (qint64)partinfo->len << "," << (int)fsinfo->dev_bsize << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                            out.flush();
                            pfile.close();
                            treeout.clear();
                            QString fsdesc = "";
                            if(poolinfo == nullptr)
                                fsdesc = GetFileSystemLabel(fsinfo);
                            else
                                fsdesc = pooldesc;
                            treeout << QString(fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")") << "0" << QString::number(fsinfo->block_size * fsinfo->block_count) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                            nodedata.clear();
                            for(int j=0; j < treeout.count(); j++)
                                nodedata << treeout.at(j);
                            mutex.lock();
                            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                            mutex.unlock();
                            reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(partint) + "):</td><td>" + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")</td></tr>";
	    		    partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + ": " + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
                            WriteFileSystemProperties(fsinfo, partitionpath);
                            ProcessDir(fsinfo, stack, fsinfo->root_inum, "/", evidcnt, volcnt, partint, partitionpath);
                            // ADD ManualCarved Folder HERE
                            treeout.clear();
                            treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-pc");
                            nodedata.clear();
                            for(int i=0; i < treeout.count(); i++)
                                nodedata << treeout.at(i);
                            mutex.lock();
                            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
                            mutex.unlock();
			    nodedata.clear();
			    nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-uc");
			    mutex.lock();
			    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
			    mutex.unlock();
                        }
                        else
                        {
                            out << "0," << (qint64)partinfo->len * (int)vsinfo->block_size << "," << QString(partinfo->desc) << " (Non-Recognized FS),0," << (qint64)partinfo->start * (int)vsinfo->block_size << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << "," << partinfo->flags << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << ",e" << QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                            out.flush();
                            pfile.close();
                            treeout.clear();
                            treeout << QString(QString(partinfo->desc) + QString(" (Non-Recognized FS)")) << "0" << QString::number(partinfo->len * vsinfo->block_size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                            nodedata.clear();
                            for(int j=0; j < treeout.count(); j++)
                                nodedata << treeout.at(j);
                            mutex.lock();
                            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                            mutex.unlock();
                            // ADD ManualCarved Folder HERE
                            treeout.clear();
                            treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-pc");
                            nodedata.clear();
                            for(int i=0; i < treeout.count(); i++)
                                nodedata << treeout.at(i);
                            mutex.lock();
                            treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
                            mutex.unlock();
			    nodedata.clear();
			    nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-uc");
			    mutex.lock();
			    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
			    mutex.unlock();
                            reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(partint) + "):</td><td>" + QString(partinfo->desc) + " (NON-RECOGNIZED FS)</td></tr>";
    			    partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + ": " + QString(partinfo->desc) + "(NON-RECOGNIZED FS)");
                        }
                    }
                    else
                    {
                        if(poolinfo->num_vols > 0)
                        {
                            if(pfile.isOpen())
                                pfile.close();
                            for(int k=0; k < poolinfo->num_vols; k++)
                            {
				int pint = partint + k;
                                //int pint = j + k;
                                TSK_POOL_VOLUME_INFO curpoolvol = poolinfo->vol_list[k];
                                pooldesc = curpoolvol.desc;
                                curimginfo = poolinfo->get_img_info(poolinfo, curpoolvol.block);
                                if(curpoolvol.flags & TSK_POOL_VOLUME_FLAG_ENCRYPTED)
                                {
                                    // CURRENTLY TSK DOESN'T WORK FOR ENCRYPTED APFS VOLUMES...
                                    fsinfo = tsk_fs_open_img_decrypt(curimginfo, partinfo->start * curimginfo->sector_size, TSK_FS_TYPE_APFS, passwordhash.value(evidname.split("/").last() + "-v" + QString::number(volcnt) + "-p" + QString::number(pint)).toStdString().c_str());
                                    //fsinfo = tsk_fs_open_img_decrypt(curimginfo, 0, TSK_FS_TYPE_APFS, passwordhash.value(evidname + "-v" + QString::number(volcnt) + "-p" + QString::number(partint));
                                    //fsinfo = tsk_fs_open_vol_decrypt(partinfo, TSK_FS_TYPE_APFS, "apfspassword");
                                    //fsinfo = tsk_fs_open_pool_decrypt(poolinfo, curpoolvol.block, TSK_FS_TYPE_APFS_DETECT, "encrypted");
                                }
                                else
                                {
                                    fsinfo = tsk_fs_open_img(curimginfo, partinfo->start * curimginfo->sector_size, TSK_FS_TYPE_APFS_DETECT);
                                    //fsinfo = tsk_fs_open_pool(poolinfo, curpoolvol.block, TSK_FS_TYPE_APFS_DETECT);
                                }
                                QString partitionpath = volumepath + "p" + QString::number(pint) + "/";
                                dir.mkpath(partitionpath);
                                pfile.setFileName(partitionpath + "stat");
                                pfile.open(QIODevice::Append | QIODevice::Text);
                                out.setDevice(&pfile);
                                if(fsinfo != NULL)
                                {
                                    out << fsinfo->ftype << "," << (qint64)fsinfo->block_size * (qint64)fsinfo->block_count << ",";
                                    if(poolinfo == nullptr)
                                        out << GetFileSystemLabel(fsinfo);
                                    else
                                        out << pooldesc;
                                    out << "," << (qint64)fsinfo->root_inum << "," << (qint64)fsinfo->offset << "," << (qint64)fsinfo->block_count << "," << (int)fsinfo->block_size << "," << partinfo->flags << "," << (qint64)partinfo->len << "," << (int)fsinfo->dev_bsize << ",e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(pint) << ",0";
                                    out.flush();
                                    pfile.close();
                                    treeout.clear();
                                    QString fsdesc = "";
                                    if(poolinfo == nullptr)
                                        fsdesc = GetFileSystemLabel(fsinfo);
                                    else
                                        fsdesc = pooldesc;
                                    treeout << QString(fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")") << "0" << QString::number(fsinfo->block_size * fsinfo->block_count) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(pint)); // NAME IN FIRST COLUMN
                                    nodedata.clear();
                                    for(int j=0; j < treeout.count(); j++)
                                        nodedata << treeout.at(j);
                                    mutex.lock();
                                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                                    mutex.unlock();
                                    reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(pint) + "):</td><td>" + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")</td></tr>";
	                	    partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(pint) + ": " + fsdesc + " (" + QString(tsk_fs_type_toname(fsinfo->ftype)).toUpper() + ")");
                                    WriteFileSystemProperties(fsinfo, partitionpath);
                                    ProcessDir(fsinfo, stack, fsinfo->root_inum, "/", evidcnt, volcnt, pint, partitionpath);
                                    // ADD ManualCarved Folder HERE
                                    treeout.clear();
                                    treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-pc");
                                    nodedata.clear();
                                    for(int i=0; i < treeout.count(); i++)
                                        nodedata << treeout.at(i);
                                    mutex.lock();
                                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
                                    mutex.unlock();
				    nodedata.clear();
				    nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-uc");
				    mutex.lock();
				    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
				    mutex.unlock();

                                }
                                else
                                {
                                    out << "0," << (qint64)partinfo->len * (int)vsinfo->block_size << "," << QString(partinfo->desc) << " (Non-Recognized FS),0," << (qint64)partinfo->start * (int)vsinfo->block_size << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << "," << partinfo->flags << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << ",e" << QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                                    out.flush();
                                    pfile.close();
                                    treeout.clear();
                                    treeout << QString(QString(partinfo->desc) + QString(" (Non-Recognized FS)")) << "0" << QString::number(partinfo->len * vsinfo->block_size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                                    nodedata.clear();
                                    for(int j=0; j < treeout.count(); j++)
                                        nodedata << treeout.at(j);
                                    mutex.lock();
                                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                                    mutex.unlock();
                                    // ADD ManualCarved Folder HERE
                                    treeout.clear();
                                    treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-pc");
                                    nodedata.clear();
                                    for(int i=0; i < treeout.count(); i++)
                                        nodedata << treeout.at(i);
                                    mutex.lock();
                                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
                                    mutex.unlock();
                                    reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(partint) + "):</td><td>" + QString(partinfo->desc) + " (NON-RECOGNIZED FS)</td></tr>";
			            partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + ": " + QString(partinfo->desc) + "(NON-RECOGNIZED FS)");
                                }
                            }
                        }
                    }
                    tsk_stack_free(stack);
                    tsk_fs_close(fsinfo);
		    tsk_pool_close(poolinfo);
		    tsk_img_close(curimginfo);
                }
                else
                {
                    out << "0," << (qint64)partinfo->len * (int)vsinfo->block_size << "," << QString(partinfo->desc) << " (Non-Recognized FS),0," << (qint64)partinfo->start * (int)vsinfo->block_size << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << "," << partinfo->flags << "," << (qint64)partinfo->len << "," << (int)vsinfo->block_size << ",e" << QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) << ",0";
                    out.flush();
                    pfile.close();
                    treeout.clear();
                    treeout << QString(QString(partinfo->desc) + QString(" (Non-Recognized FS)")) << "0" << QString::number(partinfo->len * vsinfo->block_size) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)); // NAME IN FIRST COLUMN
                    nodedata.clear();
                    for(int j=0; j < treeout.count(); j++)
                        nodedata << treeout.at(j);
                    mutex.lock();
                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt)), -1, 0);
                    reportstring += "<tr class='even vtop'><td>Partition (P" + QString::number(partint) + "):</td><td>" + QString(partinfo->desc) + " (NON-RECOGNIZED FS)</td></tr>";
                    mutex.unlock();
                    // ADD ManualCarved Folder HERE
                    treeout.clear();
                    treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-pc");
                    nodedata.clear();
                    for(int i=0; i < treeout.count(); i++)
                        nodedata << treeout.at(i);
                    mutex.lock();
                    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
                    mutex.unlock();
		    nodedata.clear();
		    nodedata << QByteArray("$Carved-UnVerified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + "-uc");
		    mutex.lock();
		    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint)), 3, 0);
		    mutex.unlock();
		    partitionlist.append("e" + QString::number(evidcnt) + "-v" + QString::number(volcnt) + "-p" + QString::number(partint) + ": " + QString(partinfo->desc) + " (NON-RECOGNIZED FS)]");
                }
                partint++;
            }
        }
        partinfo = NULL;
    }
    mutex.lock();
    reportstring += "</table></div><br/>\n";
    EvidenceReportData tmpdata;
    tmpdata.evidid = evidcnt;
    tmpdata.evidname = evidencename;
    tmpdata.evidcontent = reportstring;
    evidrepdatalist.append(tmpdata);
    mutex.unlock();
    tsk_vs_close(vsinfo);
    tsk_img_close(imginfo);
}

void ProcessDir(TSK_FS_INFO* fsinfo, TSK_STACK* stack, TSK_INUM_T dirinum, const char* path, int eint, int vint, int pint, QString partpath)
{
    TSK_FS_DIR* fsdir = NULL;
    fsdir = tsk_fs_dir_open_meta(fsinfo, dirinum);
    if(fsdir != NULL)
    {
        std::string path2 = "";
        for(uint i=0; i < tsk_fs_dir_getsize(fsdir); i++)
        {
            TSK_FS_FILE* fsfile = NULL;
            fsfile = tsk_fs_dir_get(fsdir, i);
            if(fsfile->meta == NULL)
            {
                if(fsfile->name->meta_addr || (fsfile->name->flags & TSK_FS_NAME_FLAG_ALLOC))
                    fsdir->fs_info->file_add_meta(fsdir->fs_info, fsfile, fsfile->name->meta_addr);
            }
            if(fsfile != NULL && !TSK_FS_ISDOT(fsfile->name->name))
            {
                // DO MY STUFF HERE...
                QString parentstr = "";
		int charsize = 1024;
                QStringList treeout;
                treeout.clear();
                QByteArray ba;
                ba.clear();
                ba.append(QString(fsfile->name->name).toUtf8());
                treeout << ba.toBase64(); // NAME - 0
                ba.clear();
                ba.append(QString::fromStdString(std::string(path)).toUtf8());
                treeout << ba.toBase64(); // FULL PATH - 1
                if(fsfile->name->par_addr == fsfile->fs_info->root_inum)
                    parentstr = "e" + QString::number(eint) + "-v" + QString::number(vint) + "-p" + QString::number(pint);
                else
                    parentstr = "e" + QString::number(eint) + "-v" + QString::number(vint) + "-p" + QString::number(pint) + "-f" + QString::number(fsfile->name->par_addr);
                if(fsfile->meta != NULL)
		{
		    if(fsfile->meta->size < 1024)
			charsize = fsfile->meta->size;
                    treeout << QString::number(fsfile->meta->size) << QString::number(fsfile->meta->crtime) << QString::number(fsfile->meta->mtime) << QString::number(fsfile->meta->atime) << QString::number(fsfile->meta->ctime); // SIZE, 4-DATES - 2, 3, 4, 5, 6
		}
                else
                {
                    treeout << "0" << "0" << "0" << "0" << "0"; // SIZE, 4-DATES - 2, 3, 4, 5, 6
                }
                treeout << "0"; // HASH - 7
		//qDebug() << "fsfile->name->type:" << pint << fsfile->name->name << fsfile->name->meta_addr << fsfile->name->type;
                if(fsfile->name->type == TSK_FS_NAME_TYPE_DIR) // DIRECTORY
                    treeout << "Directory" << "Directory"; // CAT/SIG - 8/9
                else if(fsfile->name->type == TSK_FS_NAME_TYPE_VIRT_DIR || QString(fsfile->name->name).contains("$OrphanFiles")) // VIRTUAL DIRECTORY
                    treeout << "Directory" << "Virtual Directory"; // CAT/SIG - 8,9
                else
                {
                    char* magicbuffer = new char[0];
                    magicbuffer = new char[charsize];
                    QByteArray tmparray("intro");
                    tmparray.clear();
		    mutex.lock();
                    tsk_fs_file_read(fsfile, 0, magicbuffer, charsize, TSK_FS_FILE_READ_FLAG_NONE);
		    mutex.unlock();
                    tmparray = QByteArray::fromRawData(magicbuffer, charsize);
                    QMimeDatabase mimedb;
                    //const QMimeType mimetype = mimedb.mimeTypeForData(tmparray);
                    const QMimeType mimetype = mimedb.mimeTypeForFileNameAndData(QString::fromStdString(fsfile->name->name), tmparray);
                    QString mimestr = GenerateCategorySignature(mimetype);
                    if(mimestr.contains("Unknown")) // generate further analysis
                    {
                        if(tmparray.at(0) == '\x4c' && tmparray.at(1) == '\x00' && tmparray.at(2) == '\x00' && tmparray.at(3) == '\x00' && tmparray.at(4) == '\x01' && tmparray.at(5) == '\x14' && tmparray.at(6) == '\x02' && tmparray.at(7) == '\x00') // LNK File
                            mimestr = "Windows System/Shortcut";
                        else if(strcmp(fsfile->name->name, "INFO2") == 0 && (tmparray.at(0) == 0x04 || tmparray.at(0) == 0x05))
                            mimestr = "Windows System/Recycler";
                        else if(QString::fromStdString(fsfile->name->name).startsWith("$I") && (tmparray.at(0) == 0x01 || tmparray.at(0) == 0x02))
                            mimestr = "Windows System/Recycle.Bin";
			else if(QString::fromStdString(fsfile->name->name).endsWith(".pf") && tmparray.at(4) == 0x53 && tmparray.at(5) == 0x43 && tmparray.at(6) == 0x43 && tmparray.at(7) == 0x41)
			    mimestr = "Windows System/Prefetch";
			else if(QString::fromStdString(fsfile->name->name).endsWith(".pf") && tmparray.at(0) == 0x4d && tmparray.at(1) == 0x41 && tmparray.at(2) == 0x4d)
			    mimestr = "Windows System/Prefetch";
                        else if(tmparray.at(0) == '\x72' && tmparray.at(1) == '\x65' && tmparray.at(2) == '\x67' && tmparray.at(3) == '\x66') // 72 65 67 66 | regf
                            mimestr = "Windows System/Registry";
                    }
                    //qDebug() << "mimestr:" << mimestr;
                    delete[] magicbuffer;
                    treeout << mimestr.split("/").at(0) << mimestr.split("/").at(1); // CAT/SIG - 8, 9
                }
                treeout << "0"; // TAG - 10
                // PUT ID INFO HERE FOR NAME IN FIRST COLUMN
                if(fsfile->name->meta_addr == 0 && strcmp(fsfile->name->name, "$MFT") != 0)
                    treeout << "e" + QString::number(eint) + "-v" + QString::number(vint) + "-p" + QString::number(pint) + "-fo" + QString::number(orphancount) + "-a" + QString::number(fsfile->name->par_addr); // Orphan ID - 11
                else if(fsfile->meta == NULL)
                    treeout << "e" + QString::number(eint) + "-v" + QString::number(vint) + "-p" + QString::number(pint) + "-fd" + QString::number(fsfile->name->meta_addr) + "-a" + QString::number(fsfile->name->par_addr); // Deleted Recovered ID - 11
                else
                    treeout << "e" + QString::number(eint) + "-v" + QString::number(vint) + "-p" + QString::number(pint) + "-f" + QString::number(fsfile->name->meta_addr) + "-a" + QString::number(fsfile->name->par_addr); // ID - 11
                //if(fsfile->meta != NULL)
                //    treeout << QString::number(fsfile->meta->type); // file type - 12
                //else
                if(QString(fsfile->name->name).contains("$OrphanFiles"))
                    treeout << "11"; // file type - 12
                else
                    treeout << QString::number(fsfile->name->type); // file type - 12
                if(fsfile->name->meta_addr == 0 && strcmp(fsfile->name->name, "$MFT") != 0)
                {
                    treeout << "1"; // orphan - 13
                }
                else
                {
                    if(fsfile->meta != NULL)
                    {
                        if(((fsfile->meta->flags & TSK_FS_META_FLAG_UNALLOC) == 2) && ((fsfile->meta->flags & TSK_FS_META_FLAG_USED) == 4))
                        {
                            treeout << "1"; // UNALLOC - 13
                        }
                        else
                        {
                            treeout << "0"; // ALLOC - 13
                        }
                    }
                    else
                    {
                        if((fsfile->name->flags & TSK_FS_NAME_FLAG_UNALLOC) == 0x02)
                        {
                            treeout << "1"; // UNALLOC - 13
                        }
                        else
                        {
                            treeout << "0"; // ALLOC = 13
                        }
                    }
                }
                //if(QString(fsfile->name->name).contains("$OrphanFiles"))
                    //qDebug() << treeout;
                QList<QVariant> nodedata;
                nodedata.clear();
                for(int i=0; i < 12; i++)
                    nodedata << treeout.at(i);
                mutex.lock();
                treenodemodel->AddNode(nodedata, parentstr, treeout.at(12).toInt(), treeout.at(13).toInt());
                mutex.unlock();
                listeditems.append(treeout.at(11));
                filesfound++;
		//qDebug() << "begin write properties...";
                WriteFileProperties(fsfile, partpath);
		//qDebug() << "end write properties...";
                isignals->ProgUpd();
                if(fsfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
                {
                    char* startoffset = new char[0];
                    QByteArray adsba;
                    adsba.clear();
                    qint64 adssize = 0;
                    TSK_OFF_T curmftentrystart = 0;
                    NTFS_INFO* ntfsinfo = (NTFS_INFO*)fsfile->fs_info;
                    int recordsize = 0;
                    if(ntfsinfo->fs->mft_rsize_c > 0)
                        recordsize = ntfsinfo->fs->mft_rsize_c * ntfsinfo->fs->csize * tsk_getu16(fsfile->fs_info->endian, ntfsinfo->fs->ssize);
                    else
                        recordsize = 1 << -ntfsinfo->fs->mft_rsize_c;
                    if(fsfile->meta != NULL)
                        curmftentrystart = tsk_getu16(fsfile->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(fsfile->fs_info->endian, ntfsinfo->fs->mft_clust) + recordsize * fsfile->meta->addr + 20;
                    else
                        curmftentrystart = tsk_getu16(fsfile->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(fsfile->fs_info->endian, ntfsinfo->fs->mft_clust) + recordsize + 20;
                    startoffset = new char[2];
                    tsk_fs_read(fsfile->fs_info, curmftentrystart, startoffset, 2);
                    uint16_t teststart = startoffset[1] * 256 + startoffset[0];
                    adssize = (qint64)teststart;
                    int cnt, i;
                    cnt = tsk_fs_file_attr_getsize(fsfile);
                    for(i = 0; i < cnt; i++)
                    {
                        const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_idx(fsfile, i);
                        // NEED TO IMPLEMENT THE $DATA ATTRIBUTE: RESIDENT/NON RESIDENT value for the properties file
                        adssize += 24;
                        adssize += (qint64)fsattr->size;
                        if(fsattr->type == 128 && fsattr->name != NULL)
                        {
                            bool isresident = false;
                            if(fsattr->flags & TSK_FS_ATTR_RES)
                            isresident = true;
                            char* fbuf = new char[fsattr->size];
                            ssize_t flen = tsk_fs_attr_read(fsattr, 0, fbuf, fsattr->size, TSK_FS_FILE_READ_FLAG_NONE);
                            QByteArray fdata = QByteArray::fromRawData(fbuf, flen);
                            QMimeDatabase adsmimedb;
                            QMimeType adsmimetype = adsmimedb.mimeTypeForData(fdata);
                            QString mimestr = GenerateCategorySignature(adsmimetype);
                            if(mimestr.contains("Unknown")) // generate further analysis
                                mimestr = "Windows System/Alternate Data Stream";
                            delete[] fbuf;
                            adsba.append(QString(QString(fsfile->name->name) + QString(":") + QString(fsattr->name)).toUtf8());
                            treeout.clear();
                            treeout << adsba.toBase64() << ba.toBase64() << QString::number(fsattr->size) << "0" << "0" << "0" << "0" << "0" << mimestr.split("/").at(0) << mimestr.split("/").at(1) << "0" << QString("e" + QString::number(eint) + "-v" + QString::number(vint) + "-p" + QString::number(pint) + "-fa" + QString::number(fsattr->id) + "-a" + QString::number(fsfile->name->meta_addr)) << "10" << "0"; // NAME IN FIRST COLUMN
                            nodedata.clear();
                            for(int i=0;  i < 12; i++)
                                nodedata << treeout.at(i);
                            mutex.lock();
                            treenodemodel->AddNode(nodedata, QString("e" + QString::number(eint) + "-v" + QString::number(vint) + "-p" + QString::number(pint) + "-f" + QString::number(fsfile->name->meta_addr)), treeout.at(12).toInt(), treeout.at(13).toInt());
                            mutex.unlock();
                            listeditems.append(treeout.at(11)); // UNCOMMENT ONCE I CAN CAPTURE ADS IN GEN HASH AND IMG THUMB
                            filesfound++;
                            isignals->ProgUpd();
                            WriteAlternateDataStreamProperties(fsfile, QString(fsfile->name->name) + QString(":") + QString(fsattr->name), QString::number(fsattr->id), partpath, isresident);
                        }
                    }
                    delete[] startoffset;
                }
                if(fsfile->name != NULL)
                {
                    if(fsfile->name->type == TSK_FS_NAME_TYPE_DIR || QString(fsfile->name->name).contains("$OrphanFiles")) // DIRECTORY
                    {
                        if(TSK_FS_ISDOT(fsfile->name->name) == 0)
                        {
                            if(tsk_stack_find(stack, fsfile->name->meta_addr) == 0) // process if it's not on stack
                            {
                                // DO MY RECURSE HERE...
                                tsk_stack_push(stack, fsfile->name->meta_addr);
                                std::string strpath(path);
                                if(strpath.find_last_of("/") == (strpath.length() - 1))
                                    path2 = std::string(path) + std::string(fsfile->name->name);
                                else
                                    path2 = std::string(path) + "/" + std::string(fsfile->name->name);
                                //path2 = std::string(path) + "/" + std::string(fsfile->name->name);
                                ProcessDir(fsinfo, stack, fsfile->name->meta_addr, path2.c_str(), eint, vint, pint, partpath);
                                tsk_stack_pop(stack);
                            }
                        }
                    }
                }
            }
            // FIX THIS FUNCTION TOO..
            if(fsfile->name->meta_addr == 0 && strcmp(fsfile->name->name, "$MFT") != 0)
                orphancount++;
            tsk_fs_file_close(fsfile);
        }
    }
    tsk_fs_dir_close(fsdir);
}

