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
    QHash<QString, QVariant> fsinfo;
    qDebug() << "estring:" << estring << "partoffset:" << partoffset;
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
            fsinfo.insert("mftoffset", QVariant((fsinfo.value("mftstartingcluster").toUInt() * fsinfo.value("sectorspercluster").toUInt() * fsinfo.value("bytespersector").toUInt())));
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
            fsinfo.insert("fatsize", QVariant(qFromLittleEndian<uint16_t>(partbuf.mid(22, 2))));
            fsinfo.insert("fs32sectorcnt", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(32, 4))));
            int rootdirsectors = 0;
            if(fatstr == "FAT12" || fatstr == "FAT16")
            {
                rootdirsectors = ((fsinfo.value("rootdirmaxfiles").toUInt() * 32) + (fsinfo.value("bytespersector").toUInt() - 1)) / fsinfo.value("bytespersector").toUInt();
                fsinfo.insert("extbootsig", QVariant(partbuf.at(38)));
                fsinfo.insert("volserialnum", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(39, 4))));
                qDebug() << "volserialnum:" << QString("0x" + QString::number(fsinfo.value("volserialnum").toUInt(), 16));
                fsinfo.insert("vollabel", QVariant(QString::fromStdString(partbuf.mid(43, 11).toStdString())));
                fsinfo.insert("fatlabel", QVariant(QString::fromStdString(partbuf.mid(54, 8).toStdString())));
                fsinfo.insert("rootdiroffset", QVariant((fsinfo.value("reservedareasize").toUInt() + (fsinfo.value("fatcount").toUInt() * fsinfo.value("fatsize").toUInt())) * fsinfo.value("bytespersector").toUInt()));
                fsinfo.insert("fatoffset", QVariant(fsinfo.value("reservedareasize").toUInt() * fsinfo.value("bytespersector").toUInt()));
                qDebug() << "rootdiroffset:" << fsinfo.value("rootdiroffset").toUInt() << fsinfo.value("rootdiroffset").toUInt()/fsinfo.value("bytespersector").toUInt();
                qDebug() << "fatoffset:" << fsinfo.value("fatoffset").toUInt() << fsinfo.value("fatoffset").toUInt()/fsinfo.value("bytespersector").toUInt();
                qDebug() << "reserved area size:" << fsinfo.value("reservedareasize").toUInt() << "fatcount:" << fsinfo.value("fatcount").toUInt() << "fatsize:" << fsinfo.value("fatsize").toUInt();
                qDebug() << "root dir start:" << fsinfo.value("reservedareasize").toUInt() + (fsinfo.value("fatcount").toUInt() * fsinfo.value("fatsize").toUInt());
                qDebug() << "cluster area/data start:" << fsinfo.value("reservedareasize").toUInt() + (fsinfo.value("fatcount").toUInt() * fsinfo.value("fatsize").toUInt()) + rootdirsectors;
            }
            else if(fat32str == "FAT32")
            {
                fsinfo.insert("extbootsig", QVariant(partbuf.at(66)));
                fsinfo.insert("volserialnum", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(67, 4))));
                fsinfo.insert("vollabel", QVariant(QString::fromStdString(partbuf.mid(71, 11).toStdString())));
                fsinfo.insert("fatlabel", QVariant(QString::fromStdString(partbuf.mid(82, 8).toStdString())));
                fsinfo.insert("fat32size", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(36, 4))));
                fsinfo.insert("rootdircluster", QVariant(qFromLittleEndian<uint32_t>(partbuf.mid(44, 4))));
                fsinfo.insert("rootdiroffset", QVariant((fsinfo.value("reservedareasize").toUInt() + (fsinfo.value("fatcount").toUInt() * fsinfo.value("fat32size").toUInt()) + rootdirsectors) * fsinfo.value("bytespersector").toUInt()));
                fsinfo.insert("fatoffset", QVariant(fsinfo.value("reservedareasize").toUInt() * fsinfo.value("bytespersector").toUInt()));
                qDebug() << "rootdiroffset:" << fsinfo.value("rootdiroffset").toUInt() << fsinfo.value("rootdiroffset").toUInt()/fsinfo.value("bytespersector").toUInt();
                qDebug() << "fatoffset:" << fsinfo.value("fatoffset").toUInt() << fsinfo.value("fatoffset").toUInt()/fsinfo.value("bytespersector").toUInt();
                qDebug() << "reserved area size:" << fsinfo.value("reservedareasize").toUInt() << "fatcount:" << fsinfo.value("fatcount").toUInt() << "fatsize:" << fsinfo.value("fat32size").toUInt();
                qDebug() << "rootdirsectors:" << rootdirsectors;
                qDebug() << "cluster area/data start (also root dir):" << fsinfo.value("reservedareasize").toUInt() + (fsinfo.value("fatcount").toUInt() * fsinfo.value("fat32size").toUInt()) + rootdirsectors;
                // FirstSectorOfCluster = ((N-2) * sectorspercluster) + firstdatasector [rootdirstart];
            }
        }
    }
    else if(extsig == 0xef53) // EXT2/3/4
    {
	//fsinfo.fstype = 6; // EXT2/3/4
    }
    else if(apfssig == "NXSB") // APFS Container
    {
	//fsinfo.fstype = 7; // APFS Container
    }
    else if(hfssig == "H+" || hfssig == "HX") // HFS+/HFSX
    {
	//fsinfo.fstype = 8; // HFS+/HFSX
    }
    else if(xfssig == "XFSB") // XFS
    {
	//fsinfo.fstype = 9; // XFS
    }
    else if(btrsig == "_BHRfS_M") // BTRFS
    {
	//fsinfo.fstype = 10; // BTRFS
    }
    else if(bitlcksig == "-FVE-FS-") // BITLOCKER
    {
        //fsinfo.fstype = 11; // BITLOCKER
    }
    // need to implement iso, udf, hfs, zfs
    fsinfolist->append(fsinfo);
}

// QtConcurrent::map(QList<DirEntryInfo> direntrylist, ProcessFileInformation);
//ParseFileSystemInformation(QByteArray* initbuffer, int fstype, QList<FileSystemInfo>* fsinfolist)

void ProcessVolume(QString evidstring)
{
    QList<qint64> pofflist;
    pofflist.clear();
    QList<qint64> psizelist;
    psizelist.clear();
    //QList<FileSystemInfo> fsinfolist;
    QList<QHash<QString, QVariant>> fsinfolist;
    fsinfolist.clear();
    qint64 imgsize = 0;
    // if evidstring .exists, then we find dir with evidstring in it and split at evidstring -e
    //qDebug() << "evidstring:" << evidstring;
    int evidcnt = 0;
    QDir eviddir = QDir(wombatvariable.tmpmntpath);
    QStringList evidfiles = eviddir.entryList(QStringList("*" + evidstring.split("/").last() + "*"), QDir::NoSymLinks | QDir::Dirs);
    evidcnt = evidfiles.first().split(QString(evidstring.split("/").last() + "-e")).last().toInt();
    //qDebug() << "evidcnt for evidstring:" << evidcnt << evidstring;
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
    //int pvret = ParseVolume(emntstring, imgsize, &pofflist, &psizelist);
    if(pofflist.count() == 0)
    {	
	// virtual attempt to process entire image as a filesystem...
	pofflist.append(0);
	psizelist.append(imgsize/512);
        ParseFileSystemInformation(emntstring, 0, &fsinfolist);
    }
    //qDebug() << "pofflist:" << pofflist;
    //qDebug() << "psizelist:" << psizelist;
    // add partitions to tree and decide about stat/prop files and where to put them...
    int ptreecnt = 0;
    QString curpartpath;
    QDir dir;
    QFile pstatfile;
    //int fstype = 0;
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
		nodedata.clear();
		nodedata << "UNALLOCATED" << "0" << QString::number(pofflist.at(i)*512) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
		mutex.lock();
		treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), -1, 0);
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
                //out << QString(fsinfolist.at(i).vollabel + " [" + fsinfolist.at(i).fatlabel.left(5) + "]") << "," << QString::number(pofflist.at(i)*512) << "," << QString::number(psizelist.at(i)*512) << ",1," << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
                out << "ALLOCATED," << QString::number(pofflist.at(i)*512) << "," << QString::number(psizelist.at(i)*512) << ",1," << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
                out.flush();
                pstatfile.close();
            }
            //qDebug() << "allocated partition[" << i << "]:" << fsinfolist.at(i).value("vollabel").toString() << "[" << fsinfolist.at(i).value("typestr").toString() << "]";
	    nodedata.clear();
	    nodedata << QString(fsinfolist.at(i).value("vollabel").toString() + " [" + fsinfolist.at(i).value("typestr").toString() + "]") << "0" << QString::number(psizelist.at(i)*512) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
	    //nodedata << "ALLOCATED" << "0" << QString::number(psizelist.at(i)*512) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
	    mutex.lock();
	    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), -1, 0);
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
		nodedata.clear();
		nodedata << "UNALLOCATED" << "0" << QString::number(512*(pofflist.at(i) - (pofflist.at(i-1) + psizelist.at(i-1)))) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
		mutex.lock();
		treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), -1, 0);
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
            //qDebug() << "allocated partition[" << i << "]:" << fsinfolist.at(i).value("vollabel").toString() << "[" << fsinfolist.at(i).value("typestr").toString() << "]";
	    nodedata << QString(fsinfolist.at(i).value("vollabel").toString() + " [" + fsinfolist.at(i).value("typestr").toString() + "]") << "0" << QString::number(psizelist.at(i)*512) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
	    //nodedata << "ALLOCATED" << "0" << QString::number(psizelist.at(i)*512) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
	    mutex.lock();
	    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), -1, 0);
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
	    nodedata.clear();
	    nodedata << "UNALLOCATED" << "0" << QString::number(imgsize - 512*(pofflist.at(i) + psizelist.at(i))) << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "0" << QString("e" + QString::number(evidcnt) + "-p" + QString::number(ptreecnt));
	    mutex.lock();
	    treenodemodel->AddNode(nodedata, QString("e" + QString::number(evidcnt)), -1, 0);
	    mutex.unlock();
	    //add final unalloc partition to tree here...
	    ptreecnt++;
	    //qDebug() << "unalloc exists after last partition...";
	}
    }
    /*
        // ADD ManualCarved Folder HERE
        treeout.clear();
        treeout << QByteArray("$Carved-Verified").toBase64() << "0" << "0" << "0" << "0" << "0" << "0" << "0" << "Directory" << "Virtual Directory" << "0" << QString("e" + QString::number(evidcnt) + "-v0-p0-pc"); // should make -vc for VerifiedCarved
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
        */
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

