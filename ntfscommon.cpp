#include "ntfscommon.h"

void GetDataAttributeLayout(ForImg* curimg, uint32_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, std::string* layout) 
{
    char* mftentrysignature = new char[5];
    curimg->ReadContent((uint8_t*)mftentrysignature, offset, 4);
    mftentrysignature[4] = 0;
    if(strcmp(mftentrysignature, "FILE") == 0) // A PROPER MFT ENTRY
    {
	// FIRST ATTRIBUTE OFFSET
	uint16_t firstattributeoffset = 0;
	ReadForImgContent(curimg, &firstattributeoffset, offset + 20);
	uint16_t curoffset = firstattributeoffset;
	// LOOP OVER ATTRIBUTES TO FIND DATA ATTRIBUTE
	while(curoffset < mftentrybytes)
	{
	    // IS RESIDENT/NON-RESIDENT
	    uint8_t isnonresident = 0; // 0 - Resident | 1 - Non-Resident
	    curimg->ReadContent(&isnonresident, offset + curoffset + 8, 1);
	    // ATTRIBUTE LENGTH
	    uint32_t attributelength = 0;
	    ReadForImgContent(curimg, &attributelength, offset + curoffset + 4);
	    // ATTRIBUTE TYPE
	    uint32_t attributetype = 0;
	    ReadForImgContent(curimg, &attributetype, offset + curoffset);
	    if(attributetype == 0x80) // DATA ATTRIBUTE
	    {
		// ATTRIBUTE NAME LENGTH
		uint8_t attributenamelength = 0;
		curimg->ReadContent(&attributenamelength, offset + curoffset + 9, 1);
		if(attributenamelength == 0) // DEFAULT DATA ENTRY
		{
		    if(isnonresident == 1) // NON-RESIDENT
		    {
			GetRunListLayout(curimg, offset + curoffset, bytespercluster, attributelength, layout);
		    }
		    else if(isnonresident == 0) // RESIDENT
		    {
			// CONTENT SIZE
			uint32_t contentsize = 0;
			ReadForImgContent(curimg, &contentsize, offset + curoffset + 16);
			// CONTENT OFFSET
			uint16_t contentoffset = 0;
			ReadForImgContent(curimg, &contentoffset, offset + curoffset + 20);
			*layout = std::to_string(offset + curoffset + contentoffset) + "," + std::to_string(contentsize) + ";";
		    }
		}
	    }
	    if(attributelength == 0 || attributetype == 0xffffffff)
		break;
	    curoffset += attributelength;
	}
    }
}

void GetIndexAttributeLayout(ForImg* curimg, uint32_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, std::string* indexlayout)
{
    char* mftentrysignature = new char[5];
    curimg->ReadContent((uint8_t*)mftentrysignature, offset, 4);
    mftentrysignature[4] = 0;
    if(strcmp(mftentrysignature, "FILE") == 0) // A PROPER MFT ENTRY
    {
	// FIRST ATTRIBUTE OFFSET
	uint16_t firstattributeoffset = 0;
	ReadForImgContent(curimg, &firstattributeoffset, offset + 20);
	uint64_t indexrootoffset = 0;
	uint32_t indexrootlength = 0;
	std::string indexallocationlayout = "";
	// LOOP OVER ATTRIBUTES TO FIND DATA ATTRIBUTE
	uint16_t curoffset = firstattributeoffset;
	while(curoffset < mftentrybytes)
	{
	    // ATTRIBUTE LENGTH
	    uint32_t attributelength = 0;
	    ReadForImgContent(curimg, &attributelength, offset + curoffset + 4);
	    // ATTRIBUTE TYPE
	    uint32_t attributetype = 0;
	    ReadForImgContent(curimg, &attributetype, offset + curoffset);
	    if(attributetype == 0x90) // $INDEX_ROOT - ALWAYS RESIDENT
	    {
		// CONTENT SIZE
		uint32_t contentsize = 0;
		ReadForImgContent(curimg, &contentsize, offset + curoffset + 16);
		indexrootlength = contentsize;
		// CONTENT OFFSET
		uint16_t contentoffset = 0;
		ReadForImgContent(curimg, &contentoffset, offset + curoffset + 20);
		indexrootoffset = offset + curoffset + contentoffset;
	    }
	    else if(attributetype == 0xa0) // $INDEX_ALLOCATION - ALWAYS NON-RESIDENT
		GetRunListLayout(curimg, offset + curoffset, bytespercluster, attributelength, &indexallocationlayout);
	    if(attributelength == 0 || attributetype == 0xffffffff)
		break;
	    curoffset += attributelength;
	}
	*indexlayout = std::to_string(indexrootoffset) + "," + std::to_string(indexrootlength) + ";";
	if(!indexallocationlayout.empty())
	    *indexlayout += indexallocationlayout;
    }
}

void GetRunListLayout(ForImg* curimg, uint64_t offset, uint32_t bytespercluster, uint32_t attributelength, std::string* layout)
{
    // RUN LIST OFFSET
    uint16_t runlistoffset = 0;
    ReadForImgContent(curimg, &runlistoffset, offset + 32);
    uint currentrunoffset = offset + runlistoffset;
    std::vector<uint64_t> runoffsets;
    std::vector<uint64_t> runlengths;
    runoffsets.clear();
    runlengths.clear();
    int i = 0;
    while(currentrunoffset < offset + attributelength)
    {
	// RUN INFO
	uint8_t runinfo = 0;
	curimg->ReadContent(&runinfo, currentrunoffset, 1);
	if(runinfo > 0)
	{
	    std::bitset<8> runbits((uint)runinfo);
	    std::bitset<4> runlengthbits;
	    std::bitset<4> runoffsetbits;
	    for(int j=0; j < 4; j++)
	    {
		runlengthbits.set(j, runbits[j]);
		runoffsetbits.set(j, runbits[j+4]);
	    }
	    uint runlengthbytes = runlengthbits.to_ulong();
	    uint runoffsetbytes = runoffsetbits.to_ulong();
	    if(runlengthbytes == 0 && runoffsetbytes == 0)
		break;
	    currentrunoffset++;
	    uint64_t runlength = 0;
	    uint64_t runoffset = 0;
	    if(runlengthbytes == 1)
	    {
		uint8_t rl = 0;
		curimg->ReadContent(&rl, currentrunoffset, 1);
		runlength = (uint)rl;
	    }
	    else
	    {
		uint8_t* rl = new uint8_t[runlengthbytes];
		curimg->ReadContent(rl, currentrunoffset, runlengthbytes);
		ReturnUint(&runlength, rl, runlengthbytes);
	    }
	    runlengths.push_back(runlength);
	    if(runoffsetbytes == 1)
	    {
		uint8_t ro = 0;
		curimg->ReadContent(&ro, currentrunoffset + runlengthbytes, 1);
		runoffset = (uint)ro;
	    }
	    else
	    {
		uint8_t* ro = new uint8_t[runoffsetbytes];
		curimg->ReadContent(ro, currentrunoffset + runlengthbytes, runoffsetbytes);
		ReturnUint(&runoffset, ro, runoffsetbytes);
	    }
	    if(i > 0)
	    {
		std::bitset<8> runoffsetbits(runoffset);
		if(i > 1 && runoffsetbits[0] == 1)
		    runoffset = runoffset - 0xffffffff - 1;
		runoffset = runoffset + runoffsets.at(i-1);
	    }
	    runoffsets.push_back(runoffset);
	    i++;
	    currentrunoffset += runlengthbytes + runoffsetbytes;
	}
	else
	    break;
    }
    for(int i=0; i < runoffsets.size(); i++)
    {
	*layout += std::to_string(runoffsets.at(i) * bytespercluster) + "," + std::to_string(runlengths.at(i) * bytespercluster) + ";";
    }
    runoffsets.clear();
    runlengths.clear();

}

void GetStandardInformationAttribute(ForImg* curimg, uint32_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::string* properties)
{
    char* mftentrysignature = new char[5];
    curimg->ReadContent((uint8_t*)mftentrysignature, offset, 4);
    mftentrysignature[4] = 0;
    if(strcmp(mftentrysignature, "FILE") == 0) // A PROPER MFT ENTRY
    {
	// FIRST ATTRIBUTE OFFSET
	uint16_t firstattributeoffset = 0;
	ReadForImgContent(curimg, &firstattributeoffset, offset + 20);
	// ATTRIBUTE FLAGS
	uint16_t attributeflags = 0;
	ReadForImgContent(curimg, &attributeflags, offset + 22);
	if(attributeflags == 0x00 || attributeflags == 0x02)
	    tmpitem->isdeleted = true;
	if(attributeflags == 0x02 || attributeflags == 0x03)
	    tmpitem->isdirectory = true;
	// LOOP OVER ATTRIBUTES TO FIND DATA ATTRIBUTE
	uint16_t curoffset = firstattributeoffset;
	while(curoffset < mftentrybytes)
	{
	    // ATTRIBUTE LENGTH
	    uint32_t attributelength = 0;
	    ReadForImgContent(curimg, &attributelength, offset + curoffset + 4);
	    // ATTRIBUTE TYPE
	    uint32_t attributetype = 0;
	    ReadForImgContent(curimg, &attributetype, offset + curoffset);
	    if(attributetype == 0x10) // $STANDARD_INFORMATION ATTRIBUTE - always resident
	    {
		// CONTENT SIZE
		uint32_t contentsize = 0;
		ReadForImgContent(curimg, &contentsize, offset + curoffset + 16);
		// CONTENT OFFSET
		uint16_t contentoffset = 0;
		ReadForImgContent(curimg, &contentoffset, offset + curoffset + 20);
		// $STANDARD_INFORMATION CREATE DATE
		uint64_t createdate = 0;
		ReadForImgContent(curimg, &createdate, offset + curoffset + contentoffset);
		tmpitem->create = ConvertWindowsTimeToUnixTimeUTC(createdate);
		//std::cout << "Create Date: " << ConvertWindowsTimeToUnixTimeUTC(createdate) << std::endl;
		// $STANDARD_INFORMATION MODIFY DATE
		uint64_t modifydate = 0;
		ReadForImgContent(curimg, &modifydate, offset + curoffset + contentoffset + 8);
		tmpitem->modify = ConvertWindowsTimeToUnixTimeUTC(modifydate);
		// $STANDARD_INFORMATION STATUS CHANGE DATE
		uint64_t statusdate = 0;
		ReadForImgContent(curimg, &statusdate, offset + curoffset + contentoffset + 16);
		tmpitem->change = ConvertWindowsTimeToUnixTimeUTC(statusdate);
		uint64_t accessdate = 0;
		ReadForImgContent(curimg, &accessdate, offset + curoffset + contentoffset + 24);
		tmpitem->access = ConvertWindowsTimeToUnixTimeUTC(accessdate);
		// OWNER ID
		uint32_t ownerid = 0;
		ReadForImgContent(curimg, &ownerid, offset + curoffset + contentoffset + 48);
		// SECURITY ID
		uint32_t securityid = 0;
		ReadForImgContent(curimg, &securityid, offset + curoffset + contentoffset + 52);
		// ACCESS FLAGS
		uint32_t accessflags = 0;
		ReadForImgContent(curimg, &accessflags, offset + curoffset + contentoffset + 32);
		std::string attributes = "";
		if(attributeflags == 0x00 || attributeflags == 0x02)
		    attributes += "Not Allocated,";
		else if(attributeflags == 0x01 || attributeflags == 0x03)
		    attributes += "Allocated,";
		if(attributeflags == 0x02 || attributeflags == 0x03)
		    attributes += "Directory,";
		if(accessflags & 0x01)
		    attributes += "Read Only,";
		if(accessflags & 0x02)
		    attributes += "Hidden,";
		if(accessflags & 0x04)
		    attributes += "System,";
		if(accessflags & 0x20)
		    attributes += "Archive,";
		if(accessflags & 0x40)
		    attributes += "Device,";
		if(accessflags & 0x80)
		    attributes += "Normal,";
		if(accessflags & 0x100)
		    attributes += "Temporary,";
		if(accessflags & 0x200)
		    attributes += "Sparse File,";
		if(accessflags & 0x400)
		    attributes += "Reparse Point,";
		if(accessflags & 0x800)
		    attributes += "Compressed,";
		if(accessflags & 0x1000)
		    attributes += "Offline,";
		if(accessflags & 0x2000)
		    attributes += "Not Indexed,";
		if(accessflags & 0x4000)
		    attributes += "Encrypted,";
	    }
            if(attributelength == 0 || attributetype == 0xffffffff)
                break;
	    curoffset += attributelength;
	}
    }
}

/*
    siforensics += "Owner ID|" + std::to_string(ownerid) + "\n";
    siforensics += "Security ID|" + std::to_string(securityid) + "\n";
*/

void GetFileNameAttribute(ForImg* curimg, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::string* properties)
{
    char* mftentrysignature = new char[5];
    curimg->ReadContent((uint8_t*)mftentrysignature, offset, 4);
    mftentrysignature[4] = 0;
    if(strcmp(mftentrysignature, "FILE") == 0) // A PROPER MFT ENTRY
    {
	// FIRST ATTRIBUTE OFFSET
	uint16_t firstattributeoffset = 0;
	ReadForImgContent(curimg, &firstattributeoffset, offset + 20);
	//uint16_t attrflags = qFromLittleEndian<uint16_t>(curimg->ReadContent(offset + 22, 2)); // attribute flags
	// LOOP OVER ATTRIBUTES TO FIND DATA ATTRIBUTE
	uint16_t curoffset = firstattributeoffset;
	while(curoffset < mftentrybytes)
	{
	    // ATTRIBUTE LENGTH
	    uint32_t attributelength = 0;
	    ReadForImgContent(curimg, &attributelength, offset + curoffset + 4);
	    // ATTRIBUTE TYPE
	    uint32_t attributetype = 0;
	    ReadForImgContent(curimg, &attributetype, offset + curoffset);
	    if(attributetype == 0x30) // $FILE_NAME ATTRIBUTE - always resident
	    {
		// CONTENT SIZE
		uint32_t contentsize = 0;
		ReadForImgContent(curimg, &contentsize, offset + curoffset + 16);
		// CONTENT OFFSET
		uint16_t contentoffset = 0;
		ReadForImgContent(curimg, &contentoffset, offset + curoffset + 20);
		// $FILE_NAME CREATE DATE
		uint64_t createdate = 0;
		ReadForImgContent(curimg, &createdate, offset + curoffset + 8);
		// $FILE_NAME MODIFY DATE
		uint64_t modifydate = 0;
		ReadForImgContent(curimg, &modifydate, offset + curoffset + 16);
		// $FILE_NAME STATUS CHANGED DATE
		uint64_t statusdate = 0;
		ReadForImgContent(curimg, &statusdate, offset + curoffset + 24);
		// $FILE_NAME ACCESS DATE
		uint64_t accessdate = 0;
		ReadForImgContent(curimg, &accessdate, offset + curoffset + 32);
		// $FILE_NAME FILE NAMESPACE
		uint8_t filenamespace = 0;
		curimg->ReadContent(&filenamespace, offset + curoffset + contentoffset + 65, 1);
		if(filenamespace != 0x02)
		{
		    // NAME LENGTH
		    uint8_t namelength = 0;
		    curimg->ReadContent(&namelength, offset + curoffset + contentoffset + 64, 1);
		    // FILE NAME
		    std::string filename = "";
		    for(uint8_t j=0; j < namelength; j++)
		    {
			uint16_t singleletter = 0;
			ReadForImgContent(curimg, &singleletter, offset + curoffset + contentoffset + 66 + j*2);
			filename += (char)singleletter;
		    }
		    //std::cout << "File name: " << filename << std::endl;
		}
	    }
            if(attributelength == 0 || attributetype == 0xffffffff)
                break;
	    curoffset += attributelength;
	}
    }
}

void GetDataAttribute(ForImg* curimg, uint32_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::vector<FileItem>* adsvector, std::string* properties)
{
    char* mftentrysignature = new char[5];
    curimg->ReadContent((uint8_t*)mftentrysignature, offset, 4);
    mftentrysignature[4] = 0;
    if(strcmp(mftentrysignature, "FILE") == 0) // A PROPER MFT ENTRY
    {
	// FIRST ATTRIBUTE OFFSET
	uint16_t firstattributeoffset = 0;
	ReadForImgContent(curimg, &firstattributeoffset, offset + 20);
	//uint16_t attrflags = qFromLittleEndian<uint16_t>(curimg->ReadContent(offset + 22, 2)); // attribute flags
	// LOOP OVER ATTRIBUTES TO FIND DATA ATTRIBUTE
	uint16_t curoffset = firstattributeoffset;
	while(curoffset < mftentrybytes)
	{
	    // IS RESIDENT/NON-RESIDENT
	    uint8_t isnonresident = 0; // 0 - Resident | 1 - Non-Resident
	    curimg->ReadContent(&isnonresident, offset + curoffset + 8, 1);
	    // ATTRIBUTE LENGTH
	    uint32_t attributelength = 0;
	    ReadForImgContent(curimg, &attributelength, offset + curoffset + 4);
	    // ATTRIBUTE TYPE
	    uint32_t attributetype = 0;
	    ReadForImgContent(curimg, &attributetype, offset + curoffset);
	    if(attributetype == 0x80) // $DATA ATTRIBUTE
	    {
		std::string layout = "";
		if(isnonresident == 1) // NON-RESIDENT
		{
		    // GET RUN LIST AND RETURN LAYOUT
		    GetRunListLayout(curimg, offset + curoffset, bytespercluster, attributelength, &layout);

		}
		else // RESIDENT
		{
		    // CONTENT SIZE
		    uint32_t contentsize = 0;
		    ReadForImgContent(curimg, &contentsize, offset + curoffset + 16);
		    // CONTENT OFFSET
		    uint16_t contentoffset = 0;
		    ReadForImgContent(curimg, &contentoffset, offset + curoffset + 20);
		    layout = std::to_string(offset + curoffset + contentoffset) + "," + std::to_string(contentsize) + ";";
		}
		//std::cout << "data layout: " << layout << std::endl;
		// ATTRIBUTE NAME LENGTH
		uint8_t attributenamelength = 0;
		curimg->ReadContent(&attributenamelength, offset + curoffset + 9, 1);
		if(attributenamelength == 0) // DEFAULT $DATA ATTRIBUTE
		{
		}
		else // ALTERNATE DATA STREAM
		{
		    std::string adsname = "";
		    uint16_t nameoffset = 0;
		    ReadForImgContent(curimg, &nameoffset, offset + curoffset + 10);
		    for(int j=0; j < attributenamelength; j++)
		    {
			uint16_t singleletter = 0;
			ReadForImgContent(curimg, &singleletter, offset + curoffset + nameoffset + j*2);
			adsname += (char)singleletter;
		    }
		    //std::cout << "ads name: " << adsname << std::endl;
		}
	    }
            if(attributelength == 0 || attributetype == 0xffffffff)
                break;
            curoffset += attributelength;
	}
    }
}
/*
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
/*    quint64 mftoffset = 0;
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
*/

void GetIndexRootAttribute(ForImg* curimg, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::vector<FileItem>* adsvector, std::string* properties)
{
    char* mftentrysignature = new char[5];
    curimg->ReadContent((uint8_t*)mftentrysignature, offset, 4);
    mftentrysignature[4] = 0;
    if(strcmp(mftentrysignature, "FILE") == 0) // A PROPER MFT ENTRY
    {
	// FIRST ATTRIBUTE OFFSET
	uint16_t firstattributeoffset = 0;
	ReadForImgContent(curimg, &firstattributeoffset, offset + 20);
	// ATTRIBUTE FLAGS
	uint16_t attributeflags = 0;
	ReadForImgContent(curimg, &attributeflags, offset + 22);
	// LOOP OVER ATTRIBUTES TO FIND DATA ATTRIBUTE
	uint16_t curoffset = firstattributeoffset;
	while(curoffset < mftentrybytes)
	{
	    // ATTRIBUTE LENGTH
	    uint32_t attributelength = 0;
	    ReadForImgContent(curimg, &attributelength, offset + curoffset + 4);
	    // ATTRIBUTE TYPE
	    uint32_t attributetype = 0;
	    ReadForImgContent(curimg, &attributetype, offset + curoffset);
	    if(attributetype == 0x90) // $INDEX_ROOT ATTRIBUTE - ALWAYS RESIDENT
	    {
		// ATTRIBUTE NAME LENGTH
		uint8_t attributenamelength = 0;
		curimg->ReadContent(&attributenamelength, offset + curoffset + 9, 1);
		// ATTRIBUTE NAME OFFSET
		uint16_t attributenameoffset = 0;
		ReadForImgContent(curimg, &attributenamelength, offset + curoffset + 10);
		// ATTRIBUTE CONTENT LENGTH
		uint32_t attributecontentlength = 0;
		ReadForImgContent(curimg, &attributecontentlength, offset + curoffset + 16);
		// ATTRIBUTE CONTENT OFFSET
		uint16_t attributecontentoffset = 0;
		ReadForImgContent(curimg, &attributecontentoffset, offset + curoffset + 20);
		// ACCESS FLAGS
		uint32_t accessflags = 0;
		ReadForImgContent(curimg, &accessflags, offset + curoffset + attributecontentoffset + 32);
		if(attributeflags == 0x02 || attributeflags == 0x03 || (accessflags & 0x02 && accessflags & 0x04))
		{
		    std::string attributename = "";
		    uint64_t physicalsize = 0;
		    uint64_t logicalsize = 0;
		    std::string layout = "";
		    if(attributenamelength > 0)
		    {
			for(int j=0; j < attributenamelength; j++)
			{
			    uint16_t singleletter = 0;
			    ReadForImgContent(curimg, &singleletter, offset + curoffset + attributenameoffset + j*2);
			    attributename += (char)singleletter;
			}
			if(attributename.compare("$I30") == 0) // default directory content
			{
			    logicalsize = attributecontentlength;
			    physicalsize = attributecontentlength;
			    layout = std::to_string(offset + curoffset + attributecontentoffset) + "," + std::to_string(attributecontentlength) + ";";
			}
			else
			{
			    std::cout << "ads name: " << "$INDEX_ROOT:" + attributename << std::endl;
			    logicalsize = attributecontentlength;
			    physicalsize = attributecontentlength;
			    layout = std::to_string(offset + curoffset + attributecontentoffset) + "," + std::to_string(attributecontentlength) + ";";
			}
		    }
		}
	    }
            if(attributelength == 0 || attributetype == 0xffffffff)
                break;
            curoffset += attributelength;
	}
    }
}

void GetIndexAllocationAttribute(ForImg* curimg, uint64_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::vector<FileItem>* adsvector, std::string* properties)
{
    char* mftentrysignature = new char[5];
    curimg->ReadContent((uint8_t*)mftentrysignature, offset, 4);
    mftentrysignature[4] = 0;
    if(strcmp(mftentrysignature, "FILE") == 0) // A PROPER MFT ENTRY
    {
	// FIRST ATTRIBUTE OFFSET
	uint16_t firstattributeoffset = 0;
	ReadForImgContent(curimg, &firstattributeoffset, offset + 20);
	// ATTRIBUTE FLAGS
	uint16_t attributeflags = 0;
	ReadForImgContent(curimg, &attributeflags, offset + 22);
	// LOOP OVER ATTRIBUTES TO FIND DATA ATTRIBUTE
	uint16_t curoffset = firstattributeoffset;
	while(curoffset < mftentrybytes)
	{
	    // ATTRIBUTE LENGTH
	    uint32_t attributelength = 0;
	    ReadForImgContent(curimg, &attributelength, offset + curoffset + 4);
	    // ATTRIBUTE TYPE
	    uint32_t attributetype = 0;
	    ReadForImgContent(curimg, &attributetype, offset + curoffset);
	    if(attributetype == 0xa0) // $INDEX_ALLOCATION ATTRIBUTE - ALWAYS NON-RESIDENT
	    {
		// ATTRIBUTE NAME LENGTH
		uint8_t attributenamelength = 0;
		curimg->ReadContent(&attributenamelength, offset + curoffset + 9, 1);
		// ATTRIBUTE NAME OFFSET
		uint16_t attributenameoffset = 0;
		ReadForImgContent(curimg, &attributenamelength, offset + curoffset + 10);
		if(attributenamelength > 0)
		{
		    for(int j=0; j < attributenamelength; j++)
		    {
			uint16_t singleletter = 0;
			ReadForImgContent(curimg, &singleletter, offset + curoffset + attributenameoffset + j*2);
			attributename += (char)singleletter;
		    }
		    uint64_t physicalsize = 0;
		    uint64_t logicalsize = 0;
		    ReadForImgContent(curimg, &logicalsize, offset + curoffset + 48);
		    std::string layout = "";
		    GetRunListLayout(curimg, offset + curoffset, bytespercluster, attributelength, &layout);
		}
	    }
            if(attributelength == 0 || attributetype == 0xffffffff)
                break;
            curoffset += attributelength;
	}
    }
}

/*
                else if(attrtype == 0xa0) // INDEX_ALLOCATION - always non-resident
                {
		    attrname = "";
		    if(namelength > 0)
		    {
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
/*    return inodecnt;
}

 */ 
