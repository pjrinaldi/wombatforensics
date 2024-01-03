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

void GetDataAttribute(ForImg* curimg, uint32_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::vector<FileItem>* filevector, std::string* properties)
{
    char* mftentrysignature = new char[5];
    curimg->ReadContent((uint8_t*)mftentrysignature, offset, 4);
    mftentrysignature[4] = 0;
    if(strcmp(mftentrysignature, "FILE") == 0) // A PROPER MFT ENTRY
    {
	// FIRST ATTRIBUTE OFFSET
	uint16_t firstattributeoffset = 0;
	ReadForImgContent(curimg, &firstattributeoffset, offset + 20);
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
		std::cout << "data layout: " << layout << std::endl;
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
		    std::cout << "ads name: " << adsname << std::endl;
		}
	    }
            if(attributelength == 0 || attributetype == 0xffffffff)
                break;
            curoffset += attributelength;
	}
    }
}
