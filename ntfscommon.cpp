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

/*
std::string GetStandardInformationAttribute(std::ifstream* rawcontent, ntfsinfo* curnt, uint64_t mftentryoffset)
{
    std::string siforensics = "";
    uint8_t* mes = new uint8_t[5];
    ReadContent(rawcontent, mes, mftentryoffset, 4);
    mes[4] = '\0';
    std::string mesigstr((char*)mes);
    delete[] mes;
    //std::cout << "MFT Entry Signature: " << mesigstr << std::endl;
    if(mesigstr.compare("FILE") == 0) // A PROPER MFT ENTRY
    {
        // OFFSET TO THE FIRST ATTRIBUTE
        uint8_t* fao = new uint8_t[2];
        uint16_t firstattributeoffset = 0;
        ReadContent(rawcontent, fao, mftentryoffset + 20, 2);
        ReturnUint16(&firstattributeoffset, fao);
        delete[] fao;
        //std::cout << "First Attribute Offset: " << firstattributeoffset << std::endl;
        // LOOP OVER ATTRIBUTES TO FIND DATA ATTRIBUTE
        uint16_t curoffset = firstattributeoffset;
        while(curoffset < curnt->mftentrysize * curnt->sectorspercluster * curnt->bytespersector)
        {
            //std::cout << "Current Offset: " << curoffset << std::endl;
            /*
            // IS RESIDENT/NON-RESIDENT
            uint8_t* rf = new uint8_t[1];
            uint8_t isnonresident = 0; // 0 - Resident | 1 - Non-Resident
            ReadContent(rawcontent, rf, mftentryoffset + curoffset + 8, 1);
            isnonresident = (uint8_t)rf[0];
            delete[] rf;
            //std::cout << "Is None Resident: " << (int)isnonresident << std::endl;
            */
/*            // ATTRIBUTE LENGTH
            uint8_t* al = new uint8_t[4];
            uint32_t attributelength = 0;
            ReadContent(rawcontent, al, mftentryoffset + curoffset + 4, 4);
            ReturnUint32(&attributelength, al);
            delete[] al;
            //std::cout << "Attribute Length: " << attributelength << std::endl;
            // ATTRIBUTE TYPE
            uint8_t* at = new uint8_t[4];
            uint32_t attributetype = 0;
            ReadContent(rawcontent, at, mftentryoffset + curoffset, 4);
            ReturnUint32(&attributetype, at);
            delete[] at;
            //std::cout << "Attribute Type: 0x" << std::hex << attributetype << std::dec << std::endl;
	    if(attributetype == 0x10) // STANDARD_INFORMATION ATTRIBUTE - always resident
	    {
                // CONTENT SIZE
		uint8_t* cs = new uint8_t[4];
		uint32_t contentsize = 0;
		ReadContent(rawcontent, cs, mftentryoffset + curoffset + 16, 4);
		ReturnUint32(&contentsize, cs);
		delete[] cs;
                // CONTENT OFFSET
		uint8_t* co = new uint8_t[2];
		uint16_t contentoffset = 0;
		ReadContent(rawcontent, co, mftentryoffset + curoffset + 20, 2);
		ReturnUint16(&contentoffset, co);
		delete[] co;
                // CREATE DATE
                uint8_t* cd = new uint8_t[8];
		uint64_t createdate = 0;
		ReadContent(rawcontent, cd, mftentryoffset + curoffset + contentoffset, 8);
		ReturnUint64(&createdate, cd);
		delete[] cd;
		siforensics += "Create Date|" + ConvertNtfsTimeToHuman(createdate) + "\n";
                // MODIFY DATE
		uint8_t* md = new uint8_t[8];
		uint64_t modifydate = 0;
		ReadContent(rawcontent, md, mftentryoffset + curoffset + contentoffset + 8, 8);
		ReturnUint64(&modifydate, md);
		delete[] md;
		siforensics += "Modify Date|" + ConvertNtfsTimeToHuman(modifydate) + "\n";
                // STATUS DATE
                uint8_t* sd = new uint8_t[8];
                uint64_t statusdate = 0;
                ReadContent(rawcontent, sd, mftentryoffset + curoffset + contentoffset + 16, 8);
                ReturnUint64(&statusdate, sd);
                delete[] sd;
                siforensics += "Status Date|" + ConvertNtfsTimeToHuman(statusdate) + "\n";
                // ACCESS DATE
                uint8_t* ad = new uint8_t[8];
                uint64_t accessdate = 0;
                ReadContent(rawcontent, ad, mftentryoffset + curoffset + contentoffset + 24, 8);
                ReturnUint64(&accessdate, ad);
                delete[] ad;
                siforensics += "Access Date|" + ConvertNtfsTimeToHuman(accessdate) + "\n";
                // OWNER ID
                uint8_t* oi = new uint8_t[4];
                uint32_t ownerid = 0;
                ReadContent(rawcontent, oi, mftentryoffset + curoffset + contentoffset + 48, 4);
                ReturnUint32(&ownerid, oi);
                delete[] oi;
                siforensics += "Owner ID|" + std::to_string(ownerid) + "\n";
                // SECURITY ID
                uint8_t* si = new uint8_t[4];
                uint32_t securityid = 0;
                ReadContent(rawcontent, si, mftentryoffset + curoffset + contentoffset + 52, 4);
                ReturnUint32(&securityid, si);
                delete[] si;
                siforensics += "Security ID|" + std::to_string(securityid) + "\n";
                // ACCESS FLAGS
                uint8_t* af = new uint8_t[4];
                uint32_t accessflags = 0;
                ReadContent(rawcontent, af, mftentryoffset + curoffset + contentoffset + 32, 4);
                ReturnUint32(&accessflags, af);
                delete[] af;
                siforensics += "Attributes|";
                if(accessflags & 0x01)
                    siforensics += "Read Only,";
                if(accessflags & 0x02)
                    siforensics += "Hidden,";
                if(accessflags & 0x04)
                    siforensics += "System,";
                if(accessflags & 0x20)
                    siforensics += "Archive,";
                if(accessflags & 0x40)
                    siforensics += "Device,";
                if(accessflags & 0x80)
                    siforensics += "Normal,";
                if(accessflags & 0x100)
                    siforensics += "Temporary,";
                if(accessflags & 0x200)
                    siforensics += "Sparse File,";
                if(accessflags & 0x400)
                    siforensics += "Reparse Point,";
                if(accessflags & 0x800)
                    siforensics += "Compressed,";
                if(accessflags & 0x1000)
                    siforensics += "Offline,";
                if(accessflags & 0x2000)
                    siforensics += "Not Indexed,";
                if(accessflags & 0x4000)
                    siforensics += "Encrypted,";
                siforensics += "\n";

		return siforensics;
	    }
            if(attributelength == 0 || attributetype == 0xffffffff)
                break;
	    curoffset += attributelength;
	}
    }

    return siforensics;
}

std::string GetFileNameAttribute(std::ifstream* rawcontent, ntfsinfo* curnt, uint64_t mftentryoffset)
{
    std::string fnforensics = "";
    uint8_t* mes = new uint8_t[5];
    ReadContent(rawcontent, mes, mftentryoffset, 4);
    mes[4] = '\0';
    std::string mesigstr((char*)mes);
    delete[] mes;
    //std::cout << "MFT Entry Signature: " << mesigstr << std::endl;
    if(mesigstr.compare("FILE") == 0) // A PROPER MFT ENTRY
    {
        // OFFSET TO THE FIRST ATTRIBUTE
        uint8_t* fao = new uint8_t[2];
        uint16_t firstattributeoffset = 0;
        ReadContent(rawcontent, fao, mftentryoffset + 20, 2);
        ReturnUint16(&firstattributeoffset, fao);
        delete[] fao;
        //std::cout << "First Attribute Offset: " << firstattributeoffset << std::endl;
        // LOOP OVER ATTRIBUTES TO FIND DATA ATTRIBUTE
        uint16_t curoffset = firstattributeoffset;
        while(curoffset < curnt->mftentrysize * curnt->sectorspercluster * curnt->bytespersector)
        {
            //std::cout << "Current Offset: " << curoffset << std::endl;
            /*
            // IS RESIDENT/NON-RESIDENT
            uint8_t* rf = new uint8_t[1];
            uint8_t isnonresident = 0; // 0 - Resident | 1 - Non-Resident
            ReadContent(rawcontent, rf, mftentryoffset + curoffset + 8, 1);
            isnonresident = (uint8_t)rf[0];
            delete[] rf;
            //std::cout << "Is None Resident: " << (int)isnonresident << std::endl;
            */
            // ATTRIBUTE LENGTH
/*            uint8_t* al = new uint8_t[4];
            uint32_t attributelength = 0;
            ReadContent(rawcontent, al, mftentryoffset + curoffset + 4, 4);
            ReturnUint32(&attributelength, al);
            delete[] al;
            //std::cout << "Attribute Length: " << attributelength << std::endl;
            // ATTRIBUTE TYPE
            uint8_t* at = new uint8_t[4];
            uint32_t attributetype = 0;
            ReadContent(rawcontent, at, mftentryoffset + curoffset, 4);
            ReturnUint32(&attributetype, at);
            delete[] at;
            //std::cout << "Attribute Type: 0x" << std::hex << attributetype << std::dec << std::endl;
	    if(attributetype == 0x30) // FILE_NAME ATTRIBUTE - always resident
	    {
                // CONTENT SIZE
		uint8_t* cs = new uint8_t[4];
		uint32_t contentsize = 0;
		ReadContent(rawcontent, cs, mftentryoffset + curoffset + 16, 4);
		ReturnUint32(&contentsize, cs);
		delete[] cs;
                // CONTENT OFFSET
		uint8_t* co = new uint8_t[2];
		uint16_t contentoffset = 0;
		ReadContent(rawcontent, co, mftentryoffset + curoffset + 20, 2);
		ReturnUint16(&contentoffset, co);
		delete[] co;
                // CREATE DATE
                uint8_t* cd = new uint8_t[8];
		uint64_t createdate = 0;
		ReadContent(rawcontent, cd, mftentryoffset + curoffset + contentoffset + 8, 8);
		ReturnUint64(&createdate, cd);
		delete[] cd;
		fnforensics += "Create Date|" + ConvertNtfsTimeToHuman(createdate) + "\n";
                // MODIFY DATE
                uint8_t* md = new uint8_t[8];
		uint64_t modifydate = 0;
		ReadContent(rawcontent, md, mftentryoffset + curoffset + contentoffset + 16, 8);
		ReturnUint64(&modifydate, md);
		delete[] md;
		fnforensics += "Modify Date|" + ConvertNtfsTimeToHuman(modifydate) + "\n";
                // STATUS DATE
                uint8_t* sd = new uint8_t[8];
		uint64_t statusdate = 0;
		ReadContent(rawcontent, sd, mftentryoffset + curoffset + contentoffset + 24, 8);
		ReturnUint64(&statusdate, sd);
		delete[] sd;
		fnforensics += "Status Date|" + ConvertNtfsTimeToHuman(statusdate) + "\n";
                // ACCESS DATE
                uint8_t* ad = new uint8_t[8];
		uint64_t accessdate = 0;
		ReadContent(rawcontent, ad, mftentryoffset + curoffset + contentoffset + 32, 8);
		ReturnUint64(&accessdate, ad);
		delete[] ad;
		fnforensics += "Access Date|" + ConvertNtfsTimeToHuman(accessdate) + "\n";
                // FILE NAMESPACE
                uint8_t* fns = new uint8_t[1];
                uint8_t filenamespace = 0;
                ReadContent(rawcontent, fns, mftentryoffset + curoffset + contentoffset + 65, 1);
                filenamespace = (uint8_t)fns[0];
                delete[] fns;
                if(filenamespace != 0x02)
                {
                    // NAME LENGTH
                    uint8_t* nl = new uint8_t[1];
                    uint8_t namelength = 0;
                    ReadContent(rawcontent, nl, mftentryoffset + curoffset + contentoffset + 64, 1);
                    namelength = (uint8_t)nl[0];
                    delete[] nl;
                    // FILE NAME
                    std::string filename = "";
                    for(uint8_t j=0; j < namelength; j++)
                    {
                        uint8_t* sl = new uint8_t[2];
                        uint16_t singleletter = 0;
                        ReadContent(rawcontent, sl, mftentryoffset + curoffset + contentoffset + 66 + j*2, 2);
                        ReturnUint16(&singleletter, sl);
                        delete[] sl;
                        filename += (char)singleletter;
                    }
                    fnforensics += "\nName|" + filename + "\n";
                }
            }
            if(attributelength == 0 || attributetype == 0xffffffff)
                break;
            curoffset += attributelength;
        }
    }
    
    return fnforensics;
}

std::string GetDataAttribute(std::ifstream* rawcontent, ntfsinfo* curnt, uint64_t mftentryoffset)
{
    std::string dataforensics = "";
    std::string runliststr = "";
    uint8_t* mes = new uint8_t[5];
    ReadContent(rawcontent, mes, mftentryoffset, 4);
    mes[4] = '\0';
    std::string mesigstr((char*)mes);
    delete[] mes;
    //std::cout << "MFT Entry Signature: " << mesigstr << std::endl;
    if(mesigstr.compare("FILE") == 0) // A PROPER MFT ENTRY
    {
        // OFFSET TO THE FIRST ATTRIBUTE
        uint8_t* fao = new uint8_t[2];
        uint16_t firstattributeoffset = 0;
        ReadContent(rawcontent, fao, mftentryoffset + 20, 2);
        ReturnUint16(&firstattributeoffset, fao);
        delete[] fao;
        //std::cout << "First Attribute Offset: " << firstattributeoffset << std::endl;
        // LOOP OVER ATTRIBUTES TO FIND DATA ATTRIBUTE
        uint16_t curoffset = firstattributeoffset;
        while(curoffset < curnt->mftentrysize * curnt->sectorspercluster * curnt->bytespersector)
        {
            //std::cout << "Current Offset: " << curoffset << std::endl;
            // IS RESIDENT/NON-RESIDENT
            uint8_t* rf = new uint8_t[1];
            uint8_t isnonresident = 0; // 0 - Resident | 1 - Non-Resident
            ReadContent(rawcontent, rf, mftentryoffset + curoffset + 8, 1);
            isnonresident = (uint8_t)rf[0];
            delete[] rf;
            //std::cout << "Is None Resident: " << (int)isnonresident << std::endl;
            // ATTRIBUTE LENGTH
            uint8_t* al = new uint8_t[4];
            uint32_t attributelength = 0;
            ReadContent(rawcontent, al, mftentryoffset + curoffset + 4, 4);
            ReturnUint32(&attributelength, al);
            delete[] al;
            //std::cout << "Attribute Length: " << attributelength << std::endl;
            // ATTRIBUTE TYPE
            uint8_t* at = new uint8_t[4];
            uint32_t attributetype = 0;
            ReadContent(rawcontent, at, mftentryoffset + curoffset, 4);
            ReturnUint32(&attributetype, at);
            delete[] at;
            //std::cout << "Attribute Type: 0x" << std::hex << attributetype << std::dec << std::endl;
	    if(attributetype == 0x80) // DATA ATTRIBUTE - resident/non-resident
	    {
                uint8_t* anl = new uint8_t[1];
                uint8_t attributenamelength = 0;
                ReadContent(rawcontent, anl, mftentryoffset + curoffset + 9, 1);
                attributenamelength = (uint8_t)anl[0];
                delete[] anl;
                //std::cout << "Attribute Name Length: " << (int)attributenamelength << std::endl;
                if(attributenamelength == 0) // DEFAULT DATA ENTRY
                {
                    dataforensics += "Default||Layout|";
                }
                else // Alternate data stream
                {
                    std::string adsname = "";
                    uint8_t* no = new uint8_t[2];
                    uint16_t nameoffset = 0;
                    ReadContent(rawcontent, no, mftentryoffset + curoffset + 10, 2);
                    ReturnUint16(&nameoffset, no);
                    delete[] no;
                    for(int j=0; j < attributenamelength; j++)
                    {
                        uint8_t* sl = new uint8_t[2];
                        uint16_t singleletter = 0;
                        ReadContent(rawcontent, sl, mftentryoffset + curoffset + nameoffset + j*2, 2);
                        ReturnUint16(&singleletter, sl);
                        delete[] sl;
                        adsname += (char)singleletter;
                    }
                    dataforensics += "Alternate||Name|" + adsname + "||Layout|";
                }
                    if(isnonresident == 1)
                    {
                        // GET RUN LIST AND RETURN LAYOUT
                        uint64_t totalmftsize = 0;
                        GetRunListLayout(rawcontent, curnt, mftentryoffset + curoffset, attributelength, &runliststr);
                        dataforensics += runliststr + "\n";
                        //std::cout << "Run List Layout: " << runliststr << std::endl;
                        break;
                    }
                    else // is resident 0
                    {
                        // CONTENT SIZE
                        uint8_t* cs = new uint8_t[4];
                        uint32_t contentsize = 0;
                        ReadContent(rawcontent, cs, mftentryoffset + curoffset + 16, 4);
                        ReturnUint32(&contentsize, cs);
                        delete[] cs;
                        // CONTENT OFFSET
                        uint8_t* co = new uint8_t[2];
                        uint16_t contentoffset = 0;
                        ReadContent(rawcontent, co, mftentryoffset + curoffset + 20, 2);
                        ReturnUint16(&contentoffset, co);
                        delete[] co;
                        dataforensics += std::to_string(mftentryoffset + curoffset + contentoffset) + "," + std::to_string(contentsize) + ";" + "\n";
                    }
            }
            if(attributelength == 0 || attributetype == 0xffffffff)
                break;
            curoffset += attributelength;
        }
    }

    return dataforensics;
}
*/
