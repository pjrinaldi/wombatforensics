#include "ntfs.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

quint64 ParseNtfsDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, quint64 parentntinode, quint64 parinode, QString parfilename, QString parlayout, QHash<quint64, quint64>* dirntinodehash, QHash<quint64, uint16_t>* ntinodehash)
{
    QString mftlayout = "";
    uint16_t mftentrybytes = 0;
    uint32_t bytespercluster = 0;
    quint64 maxmftentries = 0;
    quint64 inodecnt = 0;

    /*
    out << "Bytes Per Sector|" << QString::number(qFromLittleEndian<uint16_t>(curimg->ReadContent(curstartsector*512 + 11, 2))) << "|Number of bytes per sector, usually 512." << Qt::endl;
    out << "Sectors Per Cluster|" << QString::number(qFromLittleEndian<uint8_t>(curimg->ReadContent(curstartsector*512 + 13, 1))) << "|Number of sectors per cluster." << Qt::endl;
    out << "Max MFT Entries|" << QString::number((mftsize * bytespercluster)/1024) << "|Max MFT Entries allowed in the MFT" << Qt::endl;
    */

    if(!parfilename.isEmpty())
	inodecnt = parinode + 1;

    QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/prop");
    if(!propfile.isOpen())
	propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(propfile.isOpen())
    {
        while(!propfile.atEnd())
        {
            QString line = propfile.readLine();
	    if(line.startsWith("MFT Layout|"))
		mftlayout = line.split("|").at(1);
	    else if(line.startsWith("MFT Entry Bytes|"))
		mftentrybytes = line.split("|").at(1).toUInt();
	    else if(line.startsWith("Bytes Per Cluster|"))
		bytespercluster = line.split("|").at(1).toUInt();
            else if(line.startsWith("Max MFT Entries|"))
                maxmftentries = line.split("|").at(1).toULongLong();
	}
	propfile.close();
    }
    //qDebug() << "mft layout:" << mftlayout << "mft entry bytes:" << mftentrybytes << "bytes per cluster:" << bytespercluster;
    //qDebug() << "mft entry offset:" << parentntinode * mftentrybytes;

    /*
     * THE USE OF PARLAYOUT WILL DEPEND ON HOW I STORE THE DIRECTORY LAYOUT AND WHETHER I STORE BOTH THE INDX_ROOT AND THE INDX_ALLOCATION...
     * IF NOT, THEN I DON'T NEED THE PARLAYOUT VARIABLE
     *
     * IF THE PARLAYOUT IS THE MFTENTRYLAYOUT, THEN I DON'T NEED THE BELOW MFTLAYOUT LOOP, I CAN JUST USE THE MFTENTRYLAYOUT TO GET THE MFTENTRYOFFSET
    if(parinode > 0)
    {
        dirlayout = parlayout;
        // loop over layout variable to parse the content....
    }
    */

    quint64 mftoffset = 0;
    quint64 mftentryoffset = 0;
    if(parlayout.isEmpty())
    {
	// THIS MATH IS WORKING, BUT MIGHT BE OFF BY 1 DUE TO THE MFT STARTING AT 0, WILL HAVE TO TEST WITH THE ROOT DIR.
	quint64 curmaxntinode = 0;
	quint64 oldmaxntinode = 0;
	quint64 relativeparntinode = parentntinode;
	for(int i=0; i < mftlayout.split(";", Qt::SkipEmptyParts).count(); i++)
	{
	    //qDebug() << "mftoffset:" << mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong() << "mftlayout length:" << mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong();
	    //qDebug() << "parentntinode:" << parentntinode << "i:" << i << "mft entries:" << mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong() / mftentrybytes;
	    curmaxntinode = mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong() / mftentrybytes;
	    //qDebug() << "curmaxntinode:" << curmaxntinode;
	    if(relativeparntinode < curmaxntinode)
	    {
		mftoffset = mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong();
		break;
	    }
	    else
		relativeparntinode = relativeparntinode - curmaxntinode;
	    /*
	    if(parentntinode * mftentrybytes < mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong())
	    {
		mftoffset = mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong();
		break;
	    }
	    */
	}
	//qDebug() << "final mftoffset:" << mftoffset;
	//qDebug() << "relative parent nt inode:" << relativeparntinode;

	//qDebug() << "mftoffset:" << mftoffset;
	mftentryoffset = mftoffset + relativeparntinode * mftentrybytes;
    }
    else // use existing parent layout which is the offset and 1024 for the current dir
    {
	//if(parentntinode == 7797)
	    //qDebug() << "parntinode:" << parentntinode << "inode:" << parinode << "parlayout:" << parlayout;
	mftentryoffset = parlayout.split(";", Qt::SkipEmptyParts).at(0).split(",").at(0).toULongLong();
    }
    //if(parentntinode == 7797)
	//qDebug() << "mftentryoffset:" << mftentryoffset;
    //quint64 mftentryoffset = mftoffset + parentntinode * mftentrybytes;
    //qDebug() << "actual mftentryoffset:" << mftentryoffset;
    // PARSE MFT ENTRY FOR THE DIRECTORY SO I CAN THEN GET IT's CONTENTS AND PARSE THE INDIVIDUAL FILES WITHIN IT...
    quint64 curoffset = 0;
    uint16_t firstattroffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(mftentryoffset + 20, 2));
    uint16_t attrcount = qFromLittleEndian<uint16_t>(curimg->ReadContent(mftentryoffset + 40, 2));
    uint32_t attrlength = 0;
    curoffset = mftentryoffset + firstattroffset;
    //qDebug() << "first attr offset:" << firstattroffset << "attrcount:" << attrcount;
    quint64 indxrootoffset = 0;
    uint32_t indxrootlength = 0;
    QList<quint64> indxallocoffset;
    QList<quint64> indxalloclength;
    indxallocoffset.clear();
    indxalloclength.clear();
    for(uint16_t i=0; i < attrcount; i++)
    {
	//qDebug() << "curoffset:" << curoffset;
	uint32_t attrtype = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset, 4)); // attribute type
	//qDebug() << "attrtype:" << QString::number(attrtype, 16);
	uint8_t namelength = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 9, 1)); // length of attribute name
	attrlength = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 4, 4)); // attribute length
	uint16_t nameoffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 10, 2)); // offset to the attribute name
	QString attrname = "";
	uint16_t attrdataflags = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 12, 2)); // attrdata flags
	if(attrtype == 0x90) // $INDEX_ROOT - ALWAYS RESIDENT
	{
	    uint32_t contentlength = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 16, 4)); // attribute content length
	    uint16_t contentoffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 20, 2)); // attribute content offset
	    indxrootoffset = curoffset + contentoffset;
	    indxrootlength = contentlength;
	}
	else if(attrtype == 0xa0) // $INDEX_ALLOCATION - ALWAYS NON_RESIDENT
	{
	    QString layout = "";
	    GetRunListLayout(curimg, curstartsector, bytespercluster, mftentrybytes, curoffset, &layout);
	    //if(parentntinode == 7797)
		//qDebug() << "curoffset:" << curoffset << "layout:" << layout;
	    for(int j=0; j < layout.split(";", Qt::SkipEmptyParts).count(); j++)
	    {
		indxallocoffset.append(layout.split(";", Qt::SkipEmptyParts).at(j).split(",").at(0).toULongLong() / bytespercluster);
		indxalloclength.append(layout.split(";", Qt::SkipEmptyParts).at(j).split(",").at(1).toULongLong() / bytespercluster);
	    }
	}
	else if(attrtype == 0xffffffff)
	    break;
	curoffset += attrlength;
    }
    /*
    if(parentntinode == 7797)
    {
    qDebug() << "indxrootoffset:" << indxrootoffset << "indxrootlength:" << indxrootlength;
    qDebug() << "indxallocoffset:" << indxallocoffset << "indxalloclength:" << indxalloclength;
    }
    */

    uint32_t indxrecordsize = qFromLittleEndian<uint32_t>(curimg->ReadContent(indxrootoffset + 8, 4)); // INDEX RECORD SIZE (bytes)
    //qDebug() << "indxrecordsize:" << indxrecordsize;

    // PARE $INDEX_ROOT RECORD
    uint32_t startoffset = qFromLittleEndian<uint32_t>(curimg->ReadContent(indxrootoffset + 16, 4));
    uint32_t endoffset = qFromLittleEndian<uint32_t>(curimg->ReadContent(indxrootoffset + 20, 4));
    uint32_t allocoffset = qFromLittleEndian<uint32_t>(curimg->ReadContent(indxrootoffset + 24, 4));
    //qDebug() << "endoffset:" << endoffset;
    //qDebug() << "allocoffset:" << allocoffset;
    uint curpos = indxrootoffset + 16 + startoffset;
    //qDebug() << "initial curpos:" << curpos << "initial end pos:" << indxrootoffset + 16 + startoffset + allocoffset;
    while(curpos < indxrootoffset + 16 + startoffset + allocoffset)
    {
	//qDebug() << "in while loop...";
	//qDebug() << "curpos:" << curpos - indxrootoffset - startoffset - 16;
	uint16_t indxentrylength = qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 8, 2));
	uint16_t filenamelength = qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 10, 2));
	uint16_t i30seqid = qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 6, 2)); // seq number of index entry
	uint64_t ntinode = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos, 6));
	ntinode = ntinode & 0x00ffffffffffffff;
	//qDebug() << "ntinode:" << ntinode;
	//if((ntinode == 0 && relativeparntinode == 5) || (indxentrylength > 0 && filenamelength > 0 && ntinode <= maxmftentries && indxentrylength < indxrecordsize && filenamelength < indxentrylength && filenamelength > 66 && indxentrylength % 4 == 0))
	if((indxentrylength > 0 && filenamelength > 0 && ntinode <= maxmftentries && indxentrylength < indxrecordsize && filenamelength < indxentrylength && filenamelength > 66 && indxentrylength % 4 == 0))
	{
	    curpos = curpos + 16;
	    uint8_t fnametype = qFromLittleEndian<uint8_t>(curimg->ReadContent(curpos + 65, 1));
	    if(fnametype != 0x02)
	    {
		uint8_t fnamelength = qFromLittleEndian<uint8_t>(curimg->ReadContent(curpos + 64, 1));
		QString filename = "";
		for(uint8_t i=0; i < fnamelength; i++)
		    filename += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 66 + i*2, 2))));
		if(filename != "." && filename != ".." && !filename.isEmpty())
		{
		    uint64_t parntinode = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos, 6));
		    uint16_t i30parseqid = qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 6, 2));
		    quint64 i30create = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos + 8, 8));
		    quint64 i30modify = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos + 16, 8));
		    quint64 i30status = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos + 24, 8));
		    quint64 i30access = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos + 32, 8));
		    //qDebug() << "Filename:" << filename;
		    parntinode = parntinode & 0x00ffffffffffffff;
		    if(parntinode <= maxmftentries)
		    {
			if(ntinodehash->contains(ntinode) && ntinodehash->value(ntinode) == i30seqid)
			{
			    //qDebug() << "indxroot: do nothing because ntinode already parsed and sequence id is equal.";
			}
			else
			    inodecnt = GetMftEntryContent(curimg, curstartsector, ptreecnt, ntinode, parentntinode, parntinode, mftlayout, mftentrybytes, bytespercluster, inodecnt, filename, parinode, parfilename, i30seqid, i30parseqid, i30create, i30modify, i30status, i30access, curpos, indxrootoffset + 16 + startoffset + endoffset, dirntinodehash, ntinodehash);
		    }
		}
	    }
	    curpos = curpos - 16 + indxentrylength;
	}
	else
	    curpos = curpos + 4;
    }
    if(indxallocoffset.count() > 0) // INDX ALLOC EXISTS, SO LETS PARSE IT
    {
        for(int i=0; i < indxallocoffset.count(); i++)
        {
            uint32_t indxrecordcount = (indxalloclength.at(i) * bytespercluster) / indxrecordsize;
            for(uint32_t j=0; j < indxrecordcount; j++)
            {
                quint64 curpos = indxallocoffset.at(i) * bytespercluster;
                uint32_t startoffset = qFromLittleEndian<uint32_t>(curimg->ReadContent(curpos + 24, 4));
                uint32_t endoffset = qFromLittleEndian<uint32_t>(curimg->ReadContent(curpos + 28, 4));
                uint32_t allocoffset = qFromLittleEndian<uint32_t>(curimg->ReadContent(curpos + 32, 4));
                curpos = curpos + 24 + startoffset + j*indxrecordsize;
                //qDebug() << "curpos:" << curpos << "indexallocsize:" << (indxallocoffset.at(i) + indxalloclength.at(i)) * bytespercluster;
                while(curpos < (indxallocoffset.at(i) + indxalloclength.at(i)) * bytespercluster)
                {
                    uint64_t ntinode = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos, 6)); // nt inode for index entry
                    ntinode = ntinode & 0x00ffffffffffffff;
                    uint16_t i30seqid = qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 6, 2)); // seq number of index entry
                    uint16_t entrylength = qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 8, 2)); // entry length
                    uint16_t fnattrlength = qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 10, 2)); // $FILE_NAME attr length
                    //if((ntinode == 0 && parentntinode == 5) || (ntinode > 0 && ntinode <= maxmftentries && entrylength > 0 && entrylength < indxrecordsize && fnattrlength < entrylength && fnattrlength > 66 && entrylength % 4 == 0))
                    if((ntinode <= maxmftentries && entrylength > 0 && entrylength < indxrecordsize && fnattrlength < entrylength && fnattrlength > 66 && entrylength % 4 == 0))
                    {
                        uint8_t fntype = qFromLittleEndian<uint8_t>(curimg->ReadContent(curpos + 16 + 65, 1));
                        if(fntype != 0x02)
                        {
                            uint8_t filenamelength = qFromLittleEndian<uint8_t>(curimg->ReadContent(curpos + 16 + 64, 1));
                            QString filename = "";
                            //qDebug() << "filenamelength:" << filenamelength;
                            for(uint8_t k=0; k < filenamelength; k++)
                                filename += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 16 + 66 + k*2, 2))));
                            if(filename != "." && filename != ".." && !filename.isEmpty())
                            {
                                //qDebug() << "curpos:" << curpos + 16;
                                uint64_t parntinode = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos + 16, 6)); // parent nt inode for entry
                                parntinode = parntinode & 0x00ffffffffffffff;
                                uint16_t i30parentsequenceid = qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 16 + 6, 2)); // parent seq number for entry
                                uint64_t i30create = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos + 16 + 8, 8));
                                uint64_t i30modify = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos + 16 + 16, 8));
                                uint64_t i30change = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos + 16 + 24, 8));
                                uint64_t i30access = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos + 16 + 32, 8));
				// may not need this if, have to test...
				//if(ntinode <= maxmftentries && parntinode <= maxmftentries && !ntinodehash->contains(ntinode))
				//{
				//if(parentntinode == 7797)
				    //qDebug() << "ntinode:" << ntinode << inodecnt << "Filename:" << filename << "parntinode:" << QString::number(parntinode) << "maxmftentries:" << maxmftentries;
				if(parntinode <= maxmftentries)
				{
				    if(ntinodehash->contains(ntinode) && ntinodehash->value(ntinode) == i30seqid)
				    {
					//if(parentntinode == 7797)
					    //qDebug() << "indxalloc: do nothing because ntinode already parsed and sequence is equal.";
				    }
				    else
					inodecnt = GetMftEntryContent(curimg, curstartsector, ptreecnt, ntinode, parentntinode, parntinode, mftlayout, mftentrybytes, bytespercluster, inodecnt, filename, parinode, parfilename, i30seqid, i30parentsequenceid, i30create, i30modify, i30change, i30access, curpos, indxallocoffset.at(i) * bytespercluster + 24 + startoffset + j*indxrecordsize + endoffset, dirntinodehash, ntinodehash);
				//}
				}
                            }
                        }
                        curpos = curpos + entrylength;
                    }
                    else
                        curpos = curpos + 4;
		    //qDebug() << "curpos:" << curpos;
                }
            }
        }
    }
    /*
    else // NO INDEX_ALLOCATION
    {
	// THIS DOESN'T PROCESS THE INDEX_ROOT WHEN AN INDEX_ALLOCATION EXISTS. NEED TO POSSIBLY TAKE THIS OUT OF THE ELSE AND ALWAYS PARSE INDEX_ROOT FIRST AND THEN IF INDEX_ALLOCATION EXISTS, PARSE IT AS WELL....
        uint32_t startoffset = qFromLittleEndian<uint32_t>(curimg->ReadContent(indxrootoffset + 16, 4));
        uint32_t endoffset = qFromLittleEndian<uint32_t>(curimg->ReadContent(indxrootoffset + 20, 4));
        uint32_t allocoffset = qFromLittleEndian<uint32_t>(curimg->ReadContent(indxrootoffset + 24, 4));
        //qDebug() << "endoffset:" << endoffset;
        //qDebug() << "allocoffset:" << allocoffset;
        uint curpos = indxrootoffset + 16 + startoffset;
        //qDebug() << "initial curpos:" << curpos << "initial end pos:" << indxrootoffset + 16 + startoffset + allocoffset;
        while(curpos < indxrootoffset + 16 + startoffset + allocoffset)
        {
            //qDebug() << "in while loop...";
            //qDebug() << "curpos:" << curpos - indxrootoffset - startoffset - 16;
            uint16_t indxentrylength = qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 8, 2));
            uint16_t filenamelength = qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 10, 2));
            uint16_t i30seqid = qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 6, 2)); // seq number of index entry
            uint64_t ntinode = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos, 6));
            ntinode = ntinode & 0x00ffffffffffffff;
            //qDebug() << "ntinode:" << ntinode;
            //if((ntinode == 0 && relativeparntinode == 5) || (indxentrylength > 0 && filenamelength > 0 && ntinode <= maxmftentries && indxentrylength < indxrecordsize && filenamelength < indxentrylength && filenamelength > 66 && indxentrylength % 4 == 0))
            if((indxentrylength > 0 && filenamelength > 0 && ntinode <= maxmftentries && indxentrylength < indxrecordsize && filenamelength < indxentrylength && filenamelength > 66 && indxentrylength % 4 == 0))
            {
                curpos = curpos + 16;
                uint8_t fnametype = qFromLittleEndian<uint8_t>(curimg->ReadContent(curpos + 65, 1));
                if(fnametype != 0x02)
                {
                    uint8_t fnamelength = qFromLittleEndian<uint8_t>(curimg->ReadContent(curpos + 64, 1));
                    QString filename = "";
                    for(uint8_t i=0; i < fnamelength; i++)
                        filename += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 66 + i*2, 2))));
                    if(filename != "." && filename != ".." && !filename.isEmpty())
                    {
			uint64_t parntinode = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos, 6));
			uint16_t i30parseqid = qFromLittleEndian<uint16_t>(curimg->ReadContent(curpos + 6, 2));
			quint64 i30create = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos + 8, 8));
			quint64 i30modify = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos + 16, 8));
			quint64 i30status = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos + 24, 8));
			quint64 i30access = qFromLittleEndian<uint64_t>(curimg->ReadContent(curpos + 32, 8));
                        //qDebug() << "Filename:" << filename;
			parntinode = parntinode & 0x00ffffffffffffff;
			if(parntinode <= maxmftentries)
			{
			    if(ntinodehash->contains(ntinode) && ntinodehash->value(ntinode) == i30seqid)
			    {
				//qDebug() << "indxroot: do nothing because ntinode already parsed and sequence id is equal.";
			    }
			    else
				inodecnt = GetMftEntryContent(curimg, curstartsector, ptreecnt, ntinode, parentntinode, parntinode, mftlayout, mftentrybytes, bytespercluster, inodecnt, filename, parinode, parfilename, i30seqid, i30parseqid, i30create, i30modify, i30status, i30access, curpos, indxrootoffset + 16 + startoffset + endoffset, dirntinodehash, ntinodehash);
			}
                    }
                }
                curpos = curpos - 16 + indxentrylength;
            }
            else
                curpos = curpos + 4;
        }
    }
    */
    return inodecnt;
}

quint64 GetMftEntryContent(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, quint64 ntinode, quint64 parentntinode, uint64_t parntinode, QString mftlayout, uint16_t mftentrybytes, uint32_t bytespercluster, quint64 inodecnt, QString filename, qint64 parinode, QString parfilename, uint16_t i30seqid, uint16_t i30parseqid, uint64_t i30create, uint64_t i30modify, uint64_t i30change, uint64_t i30access, quint64 curpos, quint64 endoffset, QHash<quint64, quint64>* dirntinodehash, QHash<quint64, uint16_t>* ntinodehash)
{
    QList<QList<QVariant>> adsnodelist;
    QList<QList<QString>> adsproplist;
    adsnodelist.clear();
    adsproplist.clear();
    quint64 logicalsize = 0;
    quint64 createdate = 0;
    quint64 modifydate = 0;
    quint64 statusdate = 0;
    quint64 accessdate = 0;
    QString attrstr = "";
    QString dirlayout = "";
    QString layout = "";
    uint8_t itemtype = 0;
    uint8_t isdeleted = 0;

    QTextStream out;
    QFile fileprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
    if(!fileprop.isOpen())
        fileprop.open(QIODevice::Append | QIODevice::Text);
    out.setDevice(&fileprop);
    /*
    quint64 mftoffset = 0;
    for(int i=0; i < mftlayout.split(";", Qt::SkipEmptyParts).count(); i++)
    {
	if(ntinode * mftentrybytes < mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong())
	{
	    mftoffset = mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong();
	    break;
	}
    }
    quint64 curoffset = mftoffset + ntinode * mftentrybytes;
    */
    quint64 mftoffset = 0;
    quint64 curmaxntinode = 0;
    quint64 oldmaxntinode = 0;
    quint64 relativentinode = ntinode;
    for(int i=0; i < mftlayout.split(";", Qt::SkipEmptyParts).count(); i++)
    {
	curmaxntinode = mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong() / mftentrybytes;
	if(relativentinode < curmaxntinode)
	{
	    mftoffset = mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong();
	    break;
	}
	else
	    relativentinode = relativentinode - curmaxntinode;
    }
    quint64 curoffset = mftoffset + relativentinode * mftentrybytes;
    //qDebug() << "ntinode:" << ntinode << "entry signature:" << QString::fromStdString(curimg->ReadContent(curoffset, 4).toStdString());
    QString mftentrylayout = "";
    if(QString::fromStdString(curimg->ReadContent(curoffset, 4).toStdString()) == "FILE") // proper mft entry
    {

        mftentrylayout = QString(QString::number(curoffset) + "," + QString::number(mftentrybytes) + ";");
	out << "NT Inode|" << QString::number(ntinode) << "|NTFS file inode value." << Qt::endl;
	out << "Parent NT Inode|" << QString::number(parntinode) << "|File's parent NTFS inode value." << Qt::endl;
        out << "MFT Entry Layout|" << mftentrylayout << "|Offset and size to the MFT entry record for the file." << Qt::endl;
        // add the entry layout to file properties fileprop.
        uint16_t mftsequenceid = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 16, 2)); // sequence number for entry
        uint16_t firstattroffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 20, 2)); // offset to first attribute
        uint16_t attrflags = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 22, 2)); // attribute flags
        uint16_t attrcount = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 40, 2)); // next attribute id
        out << "MFT Sequence ID|" << QString::number(mftsequenceid) << "|Sequence number for the MFT entry." << Qt::endl;
        out << "$I30 Sequence ID|" << QString::number(i30seqid) << "|Sequence number for the file from the $I30 attribute." << Qt::endl;
        out << "$I30 Create Date|" << ConvertWindowsTimeToUnixTimeUTC(i30create) << "|File Creation time as recorded in the $I30 entry" << Qt::endl;
        out << "$I30 Modify Date|" << ConvertWindowsTimeToUnixTimeUTC(i30modify) << "|File Modification time as recorded in the $I30 entry" << Qt::endl;
        out << "$I30 Status Changed Date|" << ConvertWindowsTimeToUnixTimeUTC(i30change) << "|File Status Changed time as recorded in the $I30 entry" << Qt::endl;
        out << "$I30 Accessed Date|" << ConvertWindowsTimeToUnixTimeUTC(i30access) << "|File Accessed time as recorded in the $I30 entry" << Qt::endl;
	// this get's only hte direcotry ntinode's, but i need all of them to not reparse multiple entries when they are in the indx allocation more than once...
	// the best way to tell is with the mftseqid and the ntinode value both being equal... so i might need another hash to handle this...
	// ntinodehash and dirntinodehash ntinodehash for duplicate index entries and dirntinodehash for valid parent directories for orphan/deleted.
        if(attrflags == 0x02 || attrflags == 0x03)
            dirntinodehash->insert(ntinode, inodecnt);
	if(ntinodehash->contains(ntinode) && ntinodehash->value(ntinode) == mftsequenceid)
	{
	    //qDebug() << "mft entry: don't add the ntinode/mftseqid, the ntinode and mftsequenceid already exist in ntinodehash";
	}
	else
	{
	    ntinodehash->insert(ntinode, mftsequenceid);
	}
	uint32_t accessflags = 0;
        if(attrcount > 0)
        {
            uint32_t attrlength = 0;
            uint16_t curpos = firstattroffset;
            curoffset = curoffset + curpos;
            for(uint16_t i=0; i < attrcount; i++)
            {
                if(curpos + 22 > mftentrybytes)
                    break;
                uint32_t attrtype = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset, 4)); // attribute type
                attrlength = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 4, 4)); // attribute length
		uint8_t resflags = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 8, 1)); // resident/non-resident flag 0/1
		uint8_t namelength = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 9, 1)); // attribute name length
		uint16_t nameoffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 10, 2)); // offset to the attr name
                QString attrname = "";
		//uint16_t attrdataflags = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 12, 2)); // attrdata flags
		uint32_t contentlength = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 16, 4)); // attribute content length
		uint16_t contentoffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 20, 2)); // attribute content offset
                if(attrtype == 0x10) // $STANDARD_INFORMATION - always resident, treenode timestamps
                {
		    createdate = ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 24, 8)));
		    modifydate = ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 32, 8)));
		    statusdate = ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 40, 8)));
		    accessdate = ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 48, 8)));
		    accessflags = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 56, 4));
		    attrstr = "";
		    if(attrflags == 0x00) // unallocated file
			attrstr += "Not Allocated,";
		    else if(attrflags == 0x01) // allocated file
			attrstr += "Allocated,";
		    else if(attrflags == 0x02) // unallocated directory
			attrstr += "Not Allocated,";
		    else if(attrflags == 0x03) // allocated directory
			attrstr += "Allocated,";
		    if(accessflags & 0x01) // READ ONLY
			attrstr += "Read Only,";
		    if(accessflags & 0x02) // Hidden
			attrstr += "Hidden,";
		    if(accessflags & 0x04) // System
			attrstr += "System,";
		    if(accessflags & 0x20) // Archive
			attrstr += "Archive,";
		    if(accessflags & 0x40) // Device
			attrstr += "Device,";
		    if(accessflags & 0x80) // Normal
			attrstr += "Normal,";
		    if(accessflags & 0x100) // Temporary
			attrstr += "Temporary,";
		    if(accessflags & 0x200) // Sparse File
			attrstr += "Sparse File,";
		    if(accessflags & 0x400) // Reparse Point
			attrstr += "Reparse Point,";
		    if(accessflags & 0x800) // Compressed
			attrstr += "Compressed,";
		    if(accessflags & 0x1000) // Offline
			attrstr += "Offline,";
		    if(accessflags & 0x2000) // Not Indexed
			attrstr += "Not Indexed,";
		    if(accessflags & 0x4000) // Encrypted
			attrstr += "Encrypted,";
                    // ADD ATTRSTR TO RETURN FOR THE PROPERTIES FILE
		    out << "File Attributes|" << attrstr << "|Attributes list for the file." << Qt::endl;
                    if(curpos > endoffset)
                    {
                        if(attrflags == 0x00) // unalloc file
                        {
                            if(accessflags & 0x4000) // encrypted
                                itemtype = 13;
                            else
                                itemtype = 4;
                            isdeleted = 1;
                        }
                        else if(attrflags == 0x02) // unalloc dir
                        {
                            if(accessflags & 0x4000) // encrypted
                                itemtype = 13;
                            else
                                itemtype = 2;
                            isdeleted = 1;
                        }
                        else
                        {
                            itemtype = 4;
                            isdeleted = 1;
                        }
                    }
                    else if(!parfilename.isEmpty())
                    {
                        if(parntinode != parentntinode)
                        {
                            if(attrflags == 0x00) // unalloc file
                            {
                                if(accessflags & 0x4000) // encrypted
                                    itemtype = 13;
                                else
                                    itemtype = 4;
                                isdeleted = 1;
                            }
                            else if(attrflags == 0x02) // unalloc dir
                            {
                                if(accessflags & 0x4000) // encrypted
                                    itemtype = 13;
                                else
                                    itemtype = 2;
                                isdeleted = 1;
                            }
                            else
                            {
                                itemtype = 4;
                                isdeleted = 1;
                            }
                        }
                        else
                        {
                            if(attrflags == 0x01) // alloc file
                            {
                                if(accessflags & 0x4000) // encrypted
                                    itemtype = 13;
                                else
                                    itemtype = 5;
                                isdeleted = 0;
                            }
                            else if(attrflags == 0x03) // alloc dir
                            {
                                if(accessflags & 0x4000) // encrypted
                                    itemtype = 13;
                                else
                                    itemtype = 3;
                                isdeleted = 0;
                            }
                            else
                            {
                                itemtype = 5;
                                isdeleted = 0;
                            }
                        }
                    }
                    else
                    {
                        if(attrflags == 0x01) // alloc file
                        {
                            if(accessflags & 0x4000) // encrypted
                                itemtype = 13;
                            else
                                itemtype = 5;
                            isdeleted = 0;
                        }
                        else if(attrflags == 0x03) // alloc dir
                        {
                            if(accessflags & 0x4000) // encrypted
                                itemtype = 13;
                            else
                                itemtype = 3;
                            isdeleted = 0;
                        }
                        else
                        {
                            itemtype = 5;
                            isdeleted = 0;
                        }
                    }
                }
                else if(attrtype == 0x30) // $FILE_NAME - always resident
                {
		    uint8_t filenamespace = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 89, 1));
		    if(filenamespace != 0x02)
		    {
			uint64_t filecreate = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 32, 8));
			uint64_t filemodify = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 40, 8));
			uint64_t filestatus = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 48, 8));
			uint64_t fileaccess = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 56, 8));
                        out << "$FILE_NAME Create|" << ConvertWindowsTimeToUnixTimeUTC(filecreate) << "|File creation time as recorded in the $FILE_NAME attribute." << Qt::endl;
                        out << "$FILE_NAME Modify|" << ConvertWindowsTimeToUnixTimeUTC(filemodify) << "|File modification time as recorded in the $FILE_NAME attribute." << Qt::endl;
                        out << "$FILE_NAME Status Changed|" << ConvertWindowsTimeToUnixTimeUTC(filestatus) << "|File status changed time as recorded in the $FILE_NAME attribute." << Qt::endl;
                        out << "$FILE_NAME Accessed|" << ConvertWindowsTimeToUnixTimeUTC(fileaccess) << "|File accessed time as recorded in the $FILE_NAME attribute." << Qt::endl;
		    }
                }
                else if(attrtype == 0x80) // $DATA - resident or non-resident
                {
		    if(namelength == 0) // main file content - not alternate data stream
		    {
			logicalsize = 0;
			quint64 physicalsize = 0;
			layout = "";
			if(resflags == 0x00) // resident
			{
			    uint32_t contentsize = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 16, 4));
			    uint16_t contentoffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 20, 2));
			    logicalsize = contentsize;
			    physicalsize = contentsize;
			    dirlayout = QString(QString::number(curoffset + contentoffset) + "," + QString::number(contentsize) + ";");
                            // RETURN LOGICALSIZE FOR NODE DATA, PHYSICALSIZE AND LAYOUT FOR PROPERTIES FILE
			}
			else if(resflags == 0x01) // non-resident
			{
			    logicalsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 48, 8));
			    GetRunListLayout(curimg, curstartsector, bytespercluster, mftentrybytes, curoffset, &dirlayout);
			    //qDebug() << "layout:" << layout;
			    for(int j=0; j < dirlayout.split(";", Qt::SkipEmptyParts).count(); j++)
			    {
				physicalsize += dirlayout.split(";", Qt::SkipEmptyParts).at(j).split(",").at(1).toULongLong();
			    }
                            // RETURN LOGICALSIZE FOR THE NODE DATA, PHYSICALSIZE AND LAYOUT FOR PROPERTIES FILE
			}
                        out << "Physical Size|" << QString::number(physicalsize) << "|Physical size in bytes for the file." << Qt::endl;
			out << "Logical Size|" << QString::number(logicalsize) << "|Size in Bytes for the file." << Qt::endl;
                        out << "Layout|" << dirlayout << "|File layout in bytes and formatted as offset,size; entries." << Qt::endl;
		    }
		    else // alternate data stream
		    {
			attrname = "";
			for(int k=0; k < namelength; k++)
			    attrname += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + nameoffset + k*2, 2))));
			//qDebug() << "ads:" << QString("$DATA:" + attrname);
			quint64 logicalsize = 0;
			quint64 physicalsize = 0;
			layout = "";
			if(resflags == 0x00) // resident
			{
			    uint32_t contentsize = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 16, 4));
			    uint16_t contentoffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 20, 2));
			    logicalsize = contentsize;
			    physicalsize = contentsize;
			    layout = QString(QString::number(curoffset + contentoffset) + "," + QString::number(contentsize) + ";");
			}
			else if(resflags == 0x01) // non-resident
			{
			    logicalsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 48, 8));
			    GetRunListLayout(curimg, curstartsector, bytespercluster, mftentrybytes, curoffset, &layout);
			    for(int j=0; j < layout.split(";", Qt::SkipEmptyParts).count(); j++)
				physicalsize += layout.split(";", Qt::SkipEmptyParts).at(j).split(",").at(1).toULongLong();
			}
                        QList<QVariant> tmpnode;
                        tmpnode.clear();
                        QList<QString> tmpprop;
                        tmpprop.clear();
                        tmpnode << QString("$DATA:" + attrname) << logicalsize;
                        tmpprop.append(QString("Physical Size|" + QString::number(physicalsize) + "|Physical size for the file in bytes."));
			tmpprop.append(QString("Logical Size|" + QString::number(logicalsize) + "|Logical size for the file in bytes."));
                        tmpprop.append(QString("Layout|" + layout + "|File layout in bytes as offset,size;."));
                        adsnodelist.append(tmpnode);
                        adsproplist.append(tmpprop);
                        tmpnode.clear();
                        tmpprop.clear();
                        // RETURN ATTRNAME, LOGICALSIZE FOR ADS NODE DATA AND PHYSICALSIZE AND LAYOUT FOR PROPERTIES FILE
			// NEED TO DO SOMETHING WITH THE ADS PROPERTIES AS WELL AS THE FILE PROPERTIES...
		    }
                }
                else if(attrtype == 0x90) // $INDEX_ROOT - always resident
                {
		    if(attrflags == 0x02 || attrflags == 0x03 || (accessflags & 0x02 && accessflags & 0x04))
		    {
			attrname = "";
			quint64 physicalsize = 0;
			layout = "";
			dirlayout = "";
			if(namelength > 0) // get $I30 default dir attribute
			{
			    for(int j=0; j < namelength; j++)
				attrname += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + nameoffset + j*2, 2))));
			    if(attrname.startsWith("$I30"))
			    {
				logicalsize = contentlength;
				physicalsize = contentlength;
				dirlayout = QString(QString::number(curoffset + contentoffset) + "," + QString::number(contentlength) + ";");
                                out << "Physical Size|" << QString::number(physicalsize) << "|Physical size for the file in bytes." << Qt::endl;
				out << "Logical Size|" << QString::number(logicalsize) << "|Size in Bytes for the file." << Qt::endl;
                                out << "Layout|" << dirlayout << "|File layout in bytes as offset,size;." << Qt::endl;
                                // RETURN APPROPRIATE LOGICALSIZE,PHYSICALSIZE,LAYOUT FOR THE NODEDATA AND PROPERTIES FILE
			    }
			    else // alternate stream
			    {
				//qDebug() << "ads:" << QString("$INDEX_ROOT:" + attrname);
                                QList<QVariant> tmpnode;
                                QList<QString> tmpprop;
                                tmpnode.clear();
                                tmpprop.clear();
                                tmpnode << QString("$INDEX_ROOT:" + attrname) << contentlength;
                                tmpprop.append(QString("Physical Size|" + QString::number(contentlength) + "|Physical size for the file in bytes."));
				tmpprop.append(QString("Logical Size|" + QString::number(contentlength) + "|Logical size for the file in bytes."));
                                tmpprop.append(QString("Layout|" + QString(QString::number(curoffset + contentoffset) + ",") + QString::number(contentlength) + ";" + "|File layout in bytes as offset,size;."));
                                adsnodelist.append(tmpnode);
                                adsproplist.append(tmpprop);
                                tmpnode.clear();
                                tmpprop.clear();
			    }
			}
		    }
                }
                else if(attrtype == 0xa0) // INDEX_ALLOCATION - always non-resident
                {
		    attrname = "";
		    if(namelength > 0)
		    {
			for(int j=0; j < namelength; j++)
			    attrname += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + nameoffset + j*2, 2))));
			if(!attrname.startsWith("$I30")) // alternate data stream
			{
			    quint64 logicalsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 48, 8));
			    quint64 physicalsize = 0;
			    layout = "";
			    GetRunListLayout(curimg, curstartsector, bytespercluster, mftentrybytes, curoffset, &layout);
			    for(int j=0; j < layout.split(";", Qt::SkipEmptyParts).count(); j++)
				physicalsize += layout.split(";", Qt::SkipEmptyParts).at(j).split(",").at(1).toULongLong();
			    //qDebug() << "ads:" << QString("$INDEX_ALLOCATION" + attrname);
                            QList<QVariant> tmpnode;
                            QList<QString> tmpprop;
                            tmpnode.clear();
                            tmpprop.clear();
                            tmpnode << QString("$INDEX_ALLOCATION:" + attrname) << logicalsize;
                            tmpprop.append(QString("Physical Size|" + QString::number(physicalsize) + "|Physical size for the file in bytes."));
			    tmpprop.append(QString("Logical Size|" + QString::number(logicalsize) + "|Logical size for the file in bytes."));
                            tmpprop.append(QString("Layout|" + layout + "|File layout in bytes as offset,size;."));
                            adsnodelist.append(tmpnode);
                            adsproplist.append(tmpprop);
                            tmpnode.clear();
                            tmpprop.clear();
                            // RETURN THE VALUES FOR THE ADS APPROPRIATELY...
			}
		    }
                }
                else if(attrtype == 0xffffffff)
                    break;
                curoffset += attrlength;
            }
        }
    }
    else if(QString::fromStdString(curimg->ReadContent(curoffset, 4).toStdString()) == "BAAD") // probably orphan
    {
        qDebug() << "BAAD ENTRY... Do something with it.";
    }
    // TAKE THE STORED NODEDATA AND ADS DATA TO WRITE TO THE RESPECTIVE PROPERTIES FILE AND CONTENT...
    // NEED TO FIGURE OUT THE BEST WAY TO STORE THAT AND ACCESS IT, SOME KIND OF LIST OR NESTED LIST...
    // FOR NODEDATA, CLEARLY IT WILL BE THE NODEDATA. FOR THE ADS, IT WILL HAVE TO BE A LIST, OR MAYBE A LIST OF NODELISTS TO LOOP OVER AS WELL AS A LIST OF LISTS OF PROPERTIES FOR EACH TO WRITE AS WELL...

    QString filepath = "/";
    if(!parfilename.isEmpty())
        filepath = parfilename;

    // NEED TO FIGURE OUT ITEMTYPE AND ISDELETED...
    QHash<QString, QVariant> nodedata;
    nodedata.clear();
    nodedata.insert("name", QByteArray(filename.toUtf8()).toBase64());
    nodedata.insert("path", QByteArray(filepath.toUtf8()).toBase64());
    nodedata.insert("size", logicalsize);
    nodedata.insert("create", createdate);
    nodedata.insert("access", accessdate);
    nodedata.insert("modify", modifydate);
    nodedata.insert("status", statusdate);
    //nodedata.insert("hash", "0");
    if(logicalsize > 0) // Get Category/Signature
    {
        if(itemtype == 3 && isdeleted == 0)
        {
            nodedata.insert("cat", "Directory");
            nodedata.insert("sig", "Directory");
        }
        else
        {
            QString catsig = GenerateCategorySignature(curimg, filename, dirlayout.split(";").at(0).split(",").at(0).toULongLong());
            nodedata.insert("cat", catsig.split("/").first());
            nodedata.insert("sig", catsig.split("/").last());
        }
    }
    else
    {
        nodedata.insert("cat", "Empty");
        nodedata.insert("sig", "Zero File");
    }
    quint64 adsparentinode = inodecnt; // adsparentinode = curfile inode
    QString adsparentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)); // adsparentstr = curfile id
    //nodedata.insert("tag", "0");
    nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
    //nodedata.insert("match", 0);
    QString parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
    if(parinode > 0)
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
    inodecnt++;
    nodedata.clear();
    out.flush();
    fileprop.close();
    //qDebug() << "adsproplist:" << adsproplist;
    if(itemtype == 2 || itemtype == 3) // directory
    {
	inodecnt = ParseNtfsDirectory(curimg, curstartsector, ptreecnt, ntinode, adsparentinode, QString(filepath + filename + "/"), mftentrylayout, dirntinodehash, ntinodehash);
    }
    for(int i=0; i < adsnodelist.count(); i++)
    {
        // do catsig here and adsfilepath here as well... filepath + filename + "/"
        QHash<QString, QVariant> adsnode;
        adsnode.clear();
	adsnode.insert("name", QByteArray(adsnodelist.at(i).at(0).toString().toUtf8()).toBase64());
        adsnode.insert("path", QByteArray(QString(filepath + filename + "/").toUtf8()).toBase64());
        adsnode.insert("size", adsnodelist.at(i).at(1));
        adsnode.insert("create", "0");
        adsnode.insert("access", "0");
        adsnode.insert("modify", "0");
        adsnode.insert("status", "0");
        adsnode.insert("hash", "0");
	if(adsnodelist.at(i).at(1).toULongLong() > 0)
	{
	    QString catsig = GenerateCategorySignature(curimg, filename, adsproplist.at(i).at(1).split(";").at(0).split(",").at(0).toULongLong());
	    adsnode.insert("cat", catsig.split("/").first());
            adsnode.insert("sig", catsig.split("/").last());
	}
	else
        {
	    adsnode.insert("cat", "Empty");
            adsnode.insert("sig", "Zero File");
        }
	adsnode.insert("tag", "0");
        adsnode.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
	adsnode.insert("match", 0);
	mutex.lock();
	treenodemodel->AddNode(adsnode, adsparentstr, 10, 0);
	mutex.unlock();
	// WRITE ADS PROPERTIES
	QTextStream adsout;
	QFile adsprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
	if(!adsprop.isOpen())
	    adsprop.open(QIODevice::Append | QIODevice::Text);
	adsout.setDevice(&adsprop);
	adsout << adsproplist.at(i).at(0) << Qt::endl;
	adsout << adsproplist.at(i).at(1) << Qt::endl;
        adsout << adsproplist.at(i).at(2) << Qt::endl;
	adsout.flush();
	adsprop.close();
	inodecnt++;
    }
    /*
    if(itemtype == 2 || itemtype == 3) // directory
    {
	//qDebug() << "adsparentinode:" << adsparentinode;
	inodecnt = ParseNtfsDirectory(curimg, curstartsector, ptreecnt, ntinode, adsparentinode, QString(filepath + filename + "/"), mftentrylayout, dirntinodehash, ntinodehash);
	//inodecnt = ParseNtfsDirectory(curimg, curstartsector, ptreecnt, ntinode, adsparentinode, QString(filepath + filename + "/"), mftentrylayout, dirntinodehash, ntinodehash);
	//inodecnt = ParseNtfsDirectory(curimg, curstartsector, ptreecnt, ntinode, adsparentinode, QString(filepath + filename + "/"), dirlayout, dirntinodehash, ntinodehash);
	//qDebug() << "inodecnt on recursive return:" << inodecnt;
    }
    */

    //qDebug() << "inodecnt before getmft return:" << inodecnt;
    return inodecnt;
}

void GetRunListLayout(ForImg* curimg, uint32_t curstartsector, uint32_t bytespercluster, uint16_t mftentrybytes, quint64 curoffset, QString* layout)
{
    uint16_t runlistoff = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 32, 2));
    uint currunoff = curoffset + runlistoff;
    int i = 0;
    QStringList runlist;
    runlist.clear();
    while(currunoff < curoffset + mftentrybytes)
    {
	if(qFromLittleEndian<uint8_t>(curimg->ReadContent(currunoff, 1)) > 0)
	{
	    QString runstr = QString("%1").arg(qFromLittleEndian<uint8_t>(curimg->ReadContent(currunoff, 1)), 8, 2, QChar('0'));
	    uint runlengthbytes = runstr.right(4).toInt(nullptr, 2);
	    uint runlengthoffset = runstr.left(4).toInt(nullptr, 2);
	    if(runlengthbytes == 0 && runlengthoffset == 0)
		break;
	    currunoff++;
	    uint runlength = 0;
	    int runoffset = 0;
	    if(runlengthbytes == 1)
		runlength = qFromLittleEndian<uint8_t>(curimg->ReadContent(currunoff, runlengthbytes));
	    else
		runlength = qFromLittleEndian<uint>(curimg->ReadContent(currunoff, runlengthbytes));
	    if(runlengthoffset == 1)
		runoffset = qFromLittleEndian<int8_t>(curimg->ReadContent(currunoff + runlengthbytes, runlengthoffset));
	    else
		runoffset = qFromLittleEndian<int>(curimg->ReadContent(currunoff + runlengthbytes, runlengthoffset));
	    if(i > 0)
	    {
		if(i > 1 && QString::number(runoffset, 16).right(1).toInt() == 1)
		    runoffset = runoffset - 0xffff - 1;
		runoffset = runoffset + runlist.at(i-1).split(",").at(0).toUInt();
	    }
	    runlist.append(QString(QString::number(runoffset) + "," + QString::number(runlength)));
	    i++;
	    currunoff += runlengthbytes + runlengthoffset;
	}
	else
	    break;
    }
    for(int k=0; k < runlist.count(); k++)
	layout->append(QString(QString::number(curstartsector * 512 + (runlist.at(k).split(",").at(0).toUInt() * bytespercluster)) + "," + QString::number(runlist.at(k).split(",").at(1).toUInt() * bytespercluster) + ";"));
    runlist.clear();
}

void ParseNtfsOrphans(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, quint64 curinode, QHash<quint64, quint64>* dirntinodehash, QHash<quint64, uint16_t>* ntinodehash)
{
    QString mftlayout = "";
    uint16_t mftentrybytes = 0;
    uint32_t bytespercluster = 0;
    quint64 maxmftentries = 0;
    quint64 currentntinode = 0;

    QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/prop");
    if(!propfile.isOpen())
	propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(propfile.isOpen())
    {
        while(!propfile.atEnd())
        {
            QString line = propfile.readLine();
	    if(line.startsWith("MFT Layout|"))
		mftlayout = line.split("|").at(1);
	    else if(line.startsWith("MFT Entry Bytes|"))
		mftentrybytes = line.split("|").at(1).toUInt();
	    else if(line.startsWith("Bytes Per Cluster|"))
		bytespercluster = line.split("|").at(1).toUInt();
            else if(line.startsWith("Max MFT Entries|"))
                maxmftentries = line.split("|").at(1).toULongLong();
	}
	propfile.close();
    }
    //
    // ** Orphan File
    // * This occurs when the file is deleted and either:
    // * - The parent is no longer a directory
    // * - The sequence number of the parent is no longer correct
    // NOW PARSE THE MFT TO LOOK FOR NON-ALLOCATED ENTRIES

    quint64 entriespersize = 0;
    qDebug() << "mftlayout count:" << mftlayout.split(";", Qt::SkipEmptyParts).count();
    for(int i=0; i < mftlayout.split(";", Qt::SkipEmptyParts).count(); i++)
    {
	//qDebug() << i << "mftoffset:" << mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong() << "mftlength:" << mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong();
	entriespersize = mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong() / mftentrybytes;
	//qDebug() << "mft entries per size:" << entriespersize;
	// parse each mft block run for entries...
	for(quint64 j=0; j < entriespersize; j++)
	{
	    //qDebug() << "currentntinode:" << currentntinode << entriespersize;
            QList<QList<QVariant>> adsnodelist;
            QList<QList<QString>> adsproplist;
            adsnodelist.clear();
            adsproplist.clear();
            quint64 logicalsize = 0;
            quint64 createdate = 0;
            quint64 modifydate = 0;
            quint64 statusdate = 0;
            quint64 accessdate = 0;
            QString attrstr = "";
            QString dirlayout = "";
            QString layout = "";
            QString filename = "";
            uint8_t itemtype = 0;
            uint64_t parntinode = 0;
            uint16_t parseqid = 0;
            uint16_t accessflags = 0;
            QString filepath = "";
            //uint8_t isdeleted = 1;

	    quint64 curoffset = mftlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong();
	    //qDebug() << "curoffset at start of the mft:" << curoffset;
	    curoffset = curoffset + j*mftentrybytes;
	    //qDebug() << QString("curoffset at start of the " + QString::number(j) + " mft entry record:") << curoffset;
            QTextStream out;
            QFile fileprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(curinode) + ".prop");
            if(!fileprop.isOpen())
                fileprop.open(QIODevice::Append | QIODevice::Text);
            out.setDevice(&fileprop);
	    if(QString::fromStdString(curimg->ReadContent(curoffset, 4).toStdString()) == "FILE") // a proper mft entry
	    {
		uint16_t seqid = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 16, 2)); // sequence number for entry
		uint16_t firstattroffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 20, 2)); // offset to 1st attribute
		uint16_t attrflags = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 22, 2)); // attribute flags
		uint16_t attrcount = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 40, 2)); // next attribute id
		//qDebug() << "attrflags:" << QString::number(attrflags, 16);
		if(attrflags == 0x00 || attrflags == 0x02) // not allocated file or directory [only looking for deleted or orphaned files]
		{
		    uint32_t attrlength = 0;
		    curoffset = curoffset + firstattroffset;
		    while(curoffset < curoffset + mftentrybytes)
		    {
			if(curoffset + 22 > curoffset + mftentrybytes)
			    break;
			uint32_t attrtype = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset, 4)); // attribute type
			//qDebug() << "attrtype:" << QString::number(attrtype, 16);
			attrlength = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 4, 4)); // attribute length
			uint8_t resflag = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 8, 1)); // resident/non-resident flag 0/1
			uint8_t namelength = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 9, 1)); // attribute name length
			uint16_t nameoffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 10, 2)); // offset to attribute name
			QString attrname = "";
			uint16_t attrdataflags = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 12, 2)); // attribute data flags
			uint32_t contentlength = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 16, 4)); // attribute content length
			uint16_t contentoffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 20, 2)); // attribute content offset
			if(attrtype == 0x10) // $STANDARD_INFORMATION - always resident, treenode timestamps
			{
			    createdate = ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 24, 8)));
			    modifydate = ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 32, 8)));
			    statusdate = ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 40, 8)));
			    accessdate = ConvertNtfsTimeToUnixTime(qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 48, 8)));
			    accessflags = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 56, 4));
			    attrstr = "";
			    if(attrflags == 0x00) // unallocated file
                            {
				attrstr += "Not Allocated,";
                                itemtype = 4;
                            }
			    else if(attrflags == 0x02) // unallocated directory
                            {
				attrstr += "Not Allocated,";
                                itemtype = 2;
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
				attrstr += "Device,";
			    if(accessflags & 0x80) // Normal
				attrstr += "Normal,";
			    if(accessflags & 0x100) // Temporary
				attrstr += "Temporary,";
			    if(accessflags & 0x200) // Sparse File
				attrstr += "Sparse File,";
			    if(accessflags & 0x400) // Reparse Point
				attrstr += "Reparse Point,";
			    if(accessflags & 0x800) // Compressed
				attrstr += "Compressed,";
			    if(accessflags & 0x1000) // Offline
				attrstr += "Offline,";
			    if(accessflags & 0x2000) // Not Indexed
				attrstr += "Not Indexed,";
			    if(accessflags & 0x4000) // Encrypted
				attrstr += "Encrypted,";
			    // ADD ATTRSTR TO RETURN FOR THE PROPERTIES FILE
			    out << "File Attributes|" << attrstr << "|Attributes list for the file." << Qt::endl;
			}
			else if(attrtype == 0x30) // $FILE_NAME - always resident
			{
                            parntinode = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 24, 6)); // parent mft entry inode
                            parseqid = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 30, 2)); // parent sequence id
			    parntinode = parntinode & 0x00ffffffffffffff;
                            //qDebug() << "ntinode:" << j << "parntinode:" << parntinode;
			    uint8_t filenamespace = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 89, 1));
                            uint8_t filenamelength = qFromLittleEndian<uint8_t>(curimg->ReadContent(curoffset + 88, 1));
			    if(filenamespace != 0x02)
			    {
                                filename = "";
                                for(uint8_t k=0; k < filenamelength; k++)
                                    filename += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 90 + k*2, 2))));
                                //qDebug() << "filename:" << filename;
	                        out << "Parent NT Inode|" << QString::number(parntinode) << "|File's parent NTFS inode value." << Qt::endl;
                                out << "Parent Sequence ID|" << QString::number(parseqid) << "|File's parent sequence id." << Qt::endl;
				uint64_t filecreate = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 32, 8));
				uint64_t filemodify = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 40, 8));
				uint64_t filestatus = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 48, 8));
				uint64_t fileaccess = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 56, 8));
				out << "$FILE_NAME Create|" << ConvertWindowsTimeToUnixTimeUTC(filecreate) << "|File creation time as recorded in the $FILE_NAME attribute." << Qt::endl;
				out << "$FILE_NAME Modify|" << ConvertWindowsTimeToUnixTimeUTC(filemodify) << "|File modification time as recorded in the $FILE_NAME attribute." << Qt::endl;
				out << "$FILE_NAME Status Changed|" << ConvertWindowsTimeToUnixTimeUTC(filestatus) << "|File status changed time as recorded in the $FILE_NAME attribute." << Qt::endl;
				out << "$FILE_NAME Accessed|" << ConvertWindowsTimeToUnixTimeUTC(fileaccess) << "|File accessed time as recorded in the $FILE_NAME attribute." << Qt::endl;
			    }
				//fileinfo.insert("path", QVariant("/"));
				//fileinfo.insert("parentinode", QVariant(-1));
				//qDebug() << "nt mft entry:" << i << "filename:" << filename << "sequence number:" << sequenceid << "parmftentry:" << parmftentry << "parent sequence number;" << parsequence;
			}
			else if(attrtype == 0x80) // $DATA - resident or non-resident
			{
			    if(namelength == 0) // main file content - not alternate data stream
                            {
                                logicalsize = 0;
                                quint64 physicalsize = 0;
                                layout = "";
                                if(resflag == 0x00) // resident
                                {
                                    uint32_t contentsize = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 16, 4));
                                    uint16_t contentoffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 20, 2));
                                    logicalsize = contentsize;
                                    physicalsize = contentsize;
                                    dirlayout = QString(QString::number(curoffset + contentoffset) + "," + QString::number(contentsize) + ";");
                                    // RETURN LOGICALSIZE FOR NODE DATA, PHYSICALSIZE AND LAYOUT FOR PROPERTIES FILE
                                }
                                else if(resflag == 0x01) // non-resident
                                {
                                    logicalsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 48, 8));
                                    GetRunListLayout(curimg, curstartsector, bytespercluster, mftentrybytes, curoffset, &dirlayout);
                                    //qDebug() << "layout:" << layout;
                                    for(int k=0; k < dirlayout.split(";", Qt::SkipEmptyParts).count(); k++)
                                    {
                                        physicalsize += dirlayout.split(";", Qt::SkipEmptyParts).at(k).split(",").at(1).toULongLong();
                                    }
                                    // RETURN LOGICALSIZE FOR THE NODE DATA, PHYSICALSIZE AND LAYOUT FOR PROPERTIES FILE
                                }
                                out << "Physical Size|" << QString::number(physicalsize) << "|Physical size in bytes for the file." << Qt::endl;
				out << "Logical Size|" << QString::number(logicalsize) << "|Logical size in bytes for the file." << Qt::endl;
                                out << "Layout|" << dirlayout << "|File layout in bytes and formatted as offset,size; entries." << Qt::endl;
                            }
                            else // alternate data stream
                            {
                                attrname = "";
                                for(uint8_t k=0; k < namelength; k++)
                                    attrname += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + nameoffset + k*2, 2))));
                                //qDebug() << "ads:" << QString("$DATA:" + attrname);
                                quint64 logicalsize = 0;
                                quint64 physicalsize = 0;
                                layout = "";
                                if(resflag == 0x00) // resident
                                {
                                    uint32_t contentsize = qFromLittleEndian<uint32_t>(curimg->ReadContent(curoffset + 16, 4));
                                    uint16_t contentoffset = qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + 20, 2));
                                    logicalsize = contentsize;
                                    physicalsize = contentsize;
                                    layout = QString(QString::number(curoffset + contentoffset) + "," + QString::number(contentsize) + ";");
                                }
                                else if(resflag == 0x01) // non-resident
                                {
                                    logicalsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 48, 8));
                                    GetRunListLayout(curimg, curstartsector, bytespercluster, mftentrybytes, curoffset, &layout);
                                    for(int k=0; k < layout.split(";", Qt::SkipEmptyParts).count(); k++)
                                        physicalsize += layout.split(";", Qt::SkipEmptyParts).at(k).split(",").at(1).toULongLong();
                                }
                                QList<QVariant> tmpnode;
                                tmpnode.clear();
                                QList<QString> tmpprop;
                                tmpprop.clear();
                                tmpnode << QString("$DATA:" + attrname) << logicalsize;
                                tmpprop.append(QString("Physical Size|" + QString::number(physicalsize) + "|Physical size for the file in bytes."));
				tmpprop.append(QString("Logical Size|" + QString::number(logicalsize) + "|Logical size for the file in bytes."));
                                tmpprop.append(QString("Layout|" + layout + "|File layout in bytes as offset,size;."));
                                adsnodelist.append(tmpnode);
                                adsproplist.append(tmpprop);
                                tmpnode.clear();
                                tmpprop.clear();
                            }
			    /*
			    else // alternate data stream
			    {
				adsinfo.insert("isdeletd", QVariant(fileinfo.value("isdeleted").toUInt()));
				adsinfo.insert("itemtype", QVariant(10));
				adsinfo.insert("path", QVariant(QString(fileinfo.value("path").toString() + fileinfo.value("filename").toString() + "/")));
			    }
			     */ 
			}
			else if(attrtype == 0x90) // $INDEX_ROOT - always resident
			{
			    if(attrflags == 0x02 || attrflags == 0x03 || (accessflags & 0x02 && accessflags & 0x04))
                            {
                                attrname = "";
                                quint64 physicalsize = 0;
                                layout = "";
                                dirlayout = "";
                                if(namelength > 0) // get $I30 default dir attribute
                                {
                                    for(uint8_t k=0; k < namelength; k++)
                                        attrname += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + nameoffset + k*2, 2))));
                                    if(attrname.startsWith("$I30"))
                                    {
                                        logicalsize = contentlength;
                                        physicalsize = contentlength;
                                        dirlayout = QString(QString::number(curoffset + contentoffset) + "," + QString::number(contentlength) + ";");
                                        out << "Physical Size|" << QString::number(physicalsize) << "|Physical size for the file in bytes." << Qt::endl;
					out << "Logical Size|" << QString::number(logicalsize) << "|Size in Bytes for the file." << Qt::endl;
                                        out << "Layout|" << dirlayout << "|File layout in bytes as offset,size;." << Qt::endl;
                                        // RETURN APPROPRIATE LOGICALSIZE,PHYSICALSIZE,LAYOUT FOR THE NODEDATA AND PROPERTIES FILE
                                    }
                                    else // alternate stream
                                    {
                                        //qDebug() << "ads:" << QString("$INDEX_ROOT:" + attrname);
                                        QList<QVariant> tmpnode;
                                        QList<QString> tmpprop;
                                        tmpnode.clear();
                                        tmpprop.clear();
                                        tmpnode << QString("$INDEX_ROOT:" + attrname) << contentlength;
                                        tmpprop.append(QString("Physical Size|" + QString::number(contentlength) + "|Physical size for the file in bytes."));
					tmpprop.append(QString("Logical Size|" + QString::number(contentlength) + "|Logical size for the file in bytes."));
                                        tmpprop.append(QString("Layout|" + QString(QString::number(curoffset + contentoffset) + ",") + QString::number(contentlength) + ";" + "|File layout in bytes as offset,size;."));
                                        adsnodelist.append(tmpnode);
                                        adsproplist.append(tmpprop);
                                        tmpnode.clear();
                                        tmpprop.clear();
                                    }
                                }
                            }
			    /*
				    adsinfo.insert("ntinode", QVariant(fileinfo.value("ntinode").toUInt()));
				    adsinfo.insert("parentinode", QVariant(fileinfo.value("inode").toUInt()));
				    adsinfo.insert("parntinode", QVariant(fileinfo.value("ntinode").toUInt()));
				    adsinfo.insert("isdeletd", QVariant(fileinfo.value("isdeleted").toUInt()));
				    adsinfo.insert("itemtype", QVariant(10));
				    adsinfo.insert("path", QVariant(QString(fileinfo.value("path").toString() + fileinfo.value("filename").toString() + "/")));
				}
			    }
			     */ 
			}
			else if(attrtype == 0xa0) // $INDEX_ALLOCATION - always non-resident
			{
			    attrname = "";
			    if(namelength > 0)
			    {
				for(uint8_t k=0; k < namelength; k++)
				    attrname += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(curoffset + nameoffset + k*2, 2))));
				if(!attrname.startsWith("$I30")) // alternate data stream
				{
				    quint64 logicalsize = qFromLittleEndian<uint64_t>(curimg->ReadContent(curoffset + 48, 8));
				    quint64 physicalsize = 0;
				    layout = "";
				    GetRunListLayout(curimg, curstartsector, bytespercluster, mftentrybytes, curoffset, &layout);
				    for(int k=0; k < layout.split(";", Qt::SkipEmptyParts).count(); k++)
					physicalsize += layout.split(";", Qt::SkipEmptyParts).at(k).split(",").at(1).toULongLong();
				    //qDebug() << "ads:" << QString("$INDEX_ALLOCATION" + attrname);
				    QList<QVariant> tmpnode;
				    QList<QString> tmpprop;
				    tmpnode.clear();
				    tmpprop.clear();
				    tmpnode << QString("$INDEX_ALLOCATION:" + attrname) << logicalsize;
				    tmpprop.append(QString("Physical Size|" + QString::number(physicalsize) + "|Physical size for the file in bytes."));
				    tmpprop.append(QString("Logical Size|" + QString::number(logicalsize) + "|Logical size for the file in bytes."));
				    tmpprop.append(QString("Layout|" + layout + "|File layout in bytes as offset,size;."));
				    adsnodelist.append(tmpnode);
				    adsproplist.append(tmpprop);
				    tmpnode.clear();
				    tmpprop.clear();
				    // RETURN THE VALUES FOR THE ADS APPROPRIATELY...
				}
			    }
			    /*
				    adsinfo.insert("ntinode", QVariant(fileinfo.value("ntinode").toUInt()));
				    adsinfo.insert("parentinode", QVariant(fileinfo.value("inode").toUInt()));
				    adsinfo.insert("parntinode", QVariant(fileinfo.value("ntinode").toUInt()));
				    adsinfo.insert("isdeletd", QVariant(fileinfo.value("isdeleted").toUInt()));
				    adsinfo.insert("itemtype", QVariant(10));
				    adsinfo.insert("path", QVariant(QString(fileinfo.value("path").toString() + fileinfo.value("filename").toString() + "/")));
				}
			    }
			     */ 
			}
			else if(attrtype == 0xffffffff)
			    break;
			curoffset += attrlength;
		    }
		}
	    }
	    else if(QString::fromStdString(curimg->ReadContent(curoffset, 4).toStdString()) == "BAAD") // a proper mft entry with error
	    {
		qDebug() << "a BAAD MFT to try to read... maybe an orphan..";
	    }
            if(!filename.isEmpty())
            {
		//qDebug() << "orphan filename:" << filename;
                //qDebug() << "ntinodehash:" << *ntinodehash;
                //qDebug() << "parntinode:" << parntinode;
                QHash<QString, QVariant> nodedata;
                uint8_t hasparent = 0;
                if(parntinode == 5) // root directory is the parent
                {
                    hasparent = 1;
                    nodedata.clear();
                    nodedata.insert("name", QByteArray(filename.toUtf8()).toBase64());
                    nodedata.insert("path", QByteArray("/").toBase64());
                    nodedata.insert("size", logicalsize);
                    nodedata.insert("create", createdate);
                    nodedata.insert("access", accessdate);
                    nodedata.insert("modify", modifydate);
                    nodedata.insert("status", statusdate);
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
                            QString catsig = GenerateCategorySignature(curimg, filename, dirlayout.split(";").at(0).split(",").at(0).toULongLong());
                            nodedata.insert("cat", catsig.split("/").first());
                            nodedata.insert("sig", catsig.split("/").last());
                        }
                    }
                    else
                    {
                        nodedata.insert("cat", "Empty");
                        nodedata.insert("sig", "Zero File");
                    }
                    quint64 adsparentinode = curinode; // adsparentinode = curfile inode
                    QString adsparentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(curinode)); // adsparentstr = curfile id
                    nodedata.insert("tag", "0");
                    nodedata.insert("id", adsparentstr);
		    nodedata.insert("match", 0);
                    QString parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
                    mutex.lock();
                    treenodemodel->AddNode(nodedata, parentstr, itemtype, 1);
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
                    for(int k=0; k < adsnodelist.count(); k++)
                    {
                        QHash<QString, QVariant> adsnode;
                        adsnode.clear();
                        adsnode.insert("name", QByteArray(adsnodelist.at(k).at(0).toString().toUtf8()).toBase64());
                        adsnode.insert("path", QByteArray(QString("/" + filename + "/").toUtf8()).toBase64());
                        adsnode.insert("size", adsnodelist.at(i).at(1));
                        adsnode.insert("create", "0");
                        adsnode.insert("access", "0");
                        adsnode.insert("modify", "0");
                        adsnode.insert("status", "0");
                        adsnode.insert("hash", "0");
                        if(adsnodelist.at(i).at(1).toULongLong() > 0)
                        {
                            // MAY WANT TO SWITCH FILENAME FOR ADSNODELIST.AT(K).AT(0).TOSTRING().SPLIT(":").LAST()
                            QString catsig = GenerateCategorySignature(curimg, filename, adsproplist.at(i).at(1).split(";").at(0).split(",").at(0).toULongLong());
                            adsnode.insert("cat", catsig.split("/").first());
                            adsnode.insert("sig", catsig.split("/").last());
                        }
                        else
                        {
                            adsnode.insert("cat", "Empty");
                            adsnode.insert("sig", "Zero File");
                        }
                        adsnode.insert("tag", "0");
                        adsnode.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(curinode)));
			adsnode.insert("match", 0);
                        mutex.lock();
                        treenodemodel->AddNode(adsnode, adsparentstr, 10, 1);
                        mutex.unlock();
                        QTextStream adsout;
                        QFile adsprop(curimg->MountPath() + "/" + QString::number(ptreecnt) + "/f" + QString::number(curinode) + ".prop");
                        if(!adsprop.isOpen())
                            adsprop.open(QIODevice::Append | QIODevice::Text);
                        adsout.setDevice(&adsprop);
                        adsout << adsproplist.at(i).at(0) << Qt::endl;
                        adsout << adsproplist.at(i).at(1) << Qt::endl;
                        adsout.flush();
                        adsprop.close();
                        curinode++;
                    }
                }
                else // POSSIBLY HAS A PARENT WHICH STILL EXISTS
                {
                    if(dirntinodehash->contains(parntinode))
                    {
                        hasparent = 1;
			QString parentid = "e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(dirntinodehash->value(parntinode));
                        //qDebug() << "parentid:" << parentid;
                        QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(parentid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
                        if(indxlist.count() == 1)
                        {
                            //qDebug() << "parent filename:" << indxlist.at(0).sibling(indxlist.at(0).row(), 0).data().toString();
                            TreeNode* itemnode = static_cast<TreeNode*>(indxlist.at(0).internalPointer());
                        }
			/*
			QModelIndexList indxlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(parentid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
			if(indxlist.count() == 1)
			    qDebug() << "parent filename:" << indxlist.at(0).sibling(indxlist.at(0).row(), 0).data().toString();
			*/
			//QModelIndexList indexlist = treenodemodel->match(treenodemodel->index(0, 11, QModelIndex()), Qt::DisplayRole, QVariant(objectid), -1, Qt::MatchFlags(Qt::MatchExactly | Qt::MatchRecursive));
			//selectedindex.sibling(selectedindex.row(), 11).data().toString()
			//TreeNode* itemnode = static_cast<TreeNode*>(selectedindex.internalPointer());
			//exportlist.append(itemnode->Data(11).toString());
                        //qDebug() << "itemnode itemtype:" << itemnode->itemtype;
                        //qDebug() << "ntinode/inodecnt:" << parntinode << ntinodehash->value(parntinode);
                    }
                    /*
                            // loop over prop file to get the itemtype, or maybe acess the treenodemodel to get it's NodeData or indexitem values for the filename, filepath, etc..
                            // i have the parseqid for the "deleted/orhaned" file, i need the seqid for the potential parent to compare them... using the mft sequence id which is in the properties
                            // NEED TO GET THE ID, FILENAME, FILEPATH, ITEMTYPE (is it directory)
                            // MIGHT HAVE TO MOVE NTINODELIST TO A KEY/VALUE HASH OF NTINODE KEY AND ID VALUE FOR EASE OF ACCESS TO GET THE FILE PROPERTIES WITHOUT HAVING TO OPEN/PARSE EACH FILE
                            // OR I CAN MAKE THE LIST OF STRINGS WHICH IS THE NTINODE|ID
                            // IF I HAVE A HASH, I DON'T HAVE TO FOR LOOP, I CAN JUST HASKEY
                    */
                    // IF PARNTINODE == ONE OF THE EXISTING FILES NTINODE (LOOP OVER .PROP FILES??? OR HAVE AN QLIST<QUINT64> WITH EXISTING NTINODE'S TO LOOP THROUGH, THEN DELETED...
                    // ELSE ORPHAN
                }
                if(hasparent == 0) // orphan, parent no longer exists
                {
                    // NEED TO CREATE ORPHAN DIRECTORY AND THEN ADD IT TO THAT DIRECTORY
                }
            }
            // SHOULD I ACCOUNT AN ORPHAN WITHOUT A FILE NAME ?????
            /*
	    QString filepath = "/";
	    if(!parfilename.isEmpty())
		filepath = parfilename;
	     */ 
	    currentntinode++;
	}
    }
}

