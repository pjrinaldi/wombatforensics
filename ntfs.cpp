#include "ntfs.h"

void LoadNtfsDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem)
{
    // BYTES PER SECTOR
    uint16_t bytespersector = 0;
    ReadForImgContent(currentitem->forimg, &bytespersector, currentitem->voloffset + 11);
    // SECTORS PER CLUSTER
    uint8_t sectorspercluster = 0;
    currentitem->forimg->ReadContent(&sectorspercluster, currentitem->voloffset + 13, 1);
    // BYTES PER CLUSTER
    uint32_t bytespercluster = bytespersector * (uint)sectorspercluster;
    // MFT STARTING CLUSTER
    uint64_t mftstartingcluster = 0;
    ReadForImgContent(currentitem->forimg, &mftstartingcluster, currentitem->voloffset + 48);
    // MFT STARTING OFFSET
    uint64_t mftstartingoffset = currentitem->voloffset + mftstartingcluster * bytespercluster;
    // MFT ENTRY SIZE
    int8_t mftentrysize = 0;
    uint8_t mes = 0;
    currentitem->forimg->ReadContent(&mes, currentitem->voloffset + 64, 1);
    mftentrysize = (int8_t)mes;
    // MFT ENTRY BYTES
    uint64_t mftentrybytes = 0;
    if((int)mftentrysize == -10)
	mftentrybytes = pow(2, abs((int)mftentrysize));
    else
	mftentrybytes = (uint)mftentrysize * bytespercluster;
    std::string mftlayout = "";
    uint64_t mftsize = 0;
    /*
    // MFT LAYOUT
    char* mftentrysignature = new char[5];
    curforimg->ReadContent((uint8_t*)mftentrysignature, mftstartingoffset, 4);
    mftentrysignature[4] = 0;
    std::string mftlayout = "";
    uint64_t mftsize = 0;
    if(strcmp(mftentrysignature, "FILE") == 0) // A PROPER MFT ENTRY
    {
	// FIRST ATTRIBUTE OFFSET
	uint16_t firstattributeoffset = 0;
	ReadForImgContent(curforimg, &firstattributeoffset, mftstartingoffset + 20);
	uint16_t curoffset = firstattributeoffset;
	// LOOP OVER ATTRIBUTES TO FIND DATA ATTRIBUTE
	while(curoffset < mftentrybytes)
	{
	    // IS RESIDENT/NON-RESIDENT
	    uint8_t isnonresident = 0; // 0 - Resident | 1 - Non-Resident
	    curforimg->ReadContent(&isnonresident, mftstartingoffset + curoffset + 8, 1);
	    // ATTRIBUTE LENGTH
	    uint32_t attributelength = 0;
	    ReadForImgContent(curforimg, &attributelength, mftstartingoffset + curoffset + 4);
	    // ATTRIBUTE TYPE
	    uint32_t attributetype = 0;
	    ReadForImgContent(curforimg, &attributetype, mftstartingoffset + curoffset);
	    if(attributetype == 0x80) // DATA ATTRIBUTE
	    {
		// ATTRIBUTE NAME LENGTH
		uint8_t attributenamelength = 0;
		curforimg->ReadContent(&attributenamelength, mftstartingoffset + curoffset + 9, 1);
		if(attributenamelength == 0) // DEFAULT DATA ENTRY
		{
		    if(isnonresident == 1) // NON-RESIDENT
		    {
			// RUN LIST OFFSET
			uint16_t runlistoffset = 0;
			ReadForImgContent(curforimg, &runlistoffset, mftstartingoffset + curoffset + 32);
			uint currentrunoffset = mftstartingoffset + curoffset + runlistoffset;
			std::vector<uint64_t> runoffsets;
			std::vector<uint64_t> runlengths;
			runoffsets.clear();
			runlengths.clear();
			int i = 0;
			while(currentrunoffset < mftstartingoffset + curoffset + attributelength)
			{
			    // RUN INFO
			    uint8_t runinfo = 0;
			    curforimg->ReadContent(&runinfo, currentrunoffset, 1);
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
				    curforimg->ReadContent(&rl, currentrunoffset, 1);
				    runlength = (uint)rl;
				}
				else
				{
				    uint8_t* rl = new uint8_t[runlengthbytes];
				    curforimg->ReadContent(rl, currentrunoffset, runlengthbytes);
				    ReturnUint(&runlength, rl, runlengthbytes);
				}
				runlengths.push_back(runlength);
				if(runoffsetbytes == 1)
				{
				    uint8_t ro = 0;
				    curforimg->ReadContent(&ro, currentrunoffset + runlengthbytes, 1);
				    runoffset = (uint)ro;
				}
				else
				{
				    uint8_t* ro = new uint8_t[runoffsetbytes];
				    curforimg->ReadContent(ro, currentrunoffset + runlengthbytes, runoffsetbytes);
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
			    mftlayout += std::to_string(runoffsets.at(i) * bytespercluster) + "," + std::to_string(runlengths.at(i) * bytespercluster) + ";";
			    mftsize += runlengths.at(i) * bytespercluster;
			}
			runoffsets.clear();
			runlengths.clear();
		    }
		    else if(isnonresident == 0) // RESIDENT
		    {
			// CONTENT SIZE
			uint32_t contentsize = 0;
			ReadForImgContent(curforimg, &contentsize, mftstartingoffset + curoffset + 16);
			// CONTENT OFFSET
			uint16_t contentoffset = 0;
			ReadForImgContent(curforimg, &contentoffset, mftstartingoffset + curoffset + 20);
			mftlayout = std::to_string(mftstartingoffset + curoffset + contentoffset) + "," + std::to_string(contentsize) + ";";
			mftsize = contentsize;
		    }
		}
	    }
	    if(attributelength == 0 || attributetype == 0xffffffff)
		break;
	    curoffset += attributelength;
	}
    }
    std::cout << "mft layout: " << mftlayout << std::endl;
    uint64_t mftmaxentries = mftsize / mftentrybytes;
    */
}

/*
    // MFT STARTING CLUSTER
    uint64_t mftstartingcluster = 0;
    ReadForImgContent(curforimg, &mftstartingcluster, offset + 48);
    //std::cout << "mft starting cluster: " << mftstartingcluster << std::endl;
    //uint16_t bytespersector = pow(2, (uint)tmp8);
    // MFT ENTRY SIZE
    int8_t mftentrysize = 0;
    curforimg->ReadContent((uint8_t*)&mftentrysize, offset + 64, 1);
    // MFT ENTRY SIZE IN BYTES
    uint64_t mftentrybytesize = 0;
    if((int)mftentrysize == -10)
	mftentrybytesize = pow(2, abs((int)mftentrysize));
    else
	mftentrybytesize = (int)mftentrysize * sectorspercluster * bytespersector;
    //uint64_t mftentrybytesize = (uint)mftentrysize * sectorspercluster * bytespersector;
    //std::cout << "mftentrysize: " << (int)mftentrysize << std::endl;
    //std::cout << "mftentrybytesize: " << mftentrybytesize << std::endl;
    //mftentrybytesize = 1024;
    uint64_t mftoffset = offset + mftstartingcluster * sectorspercluster * bytespersector;
    //std::cout << "mft offset: " << mftoffset << std::endl;
    char* mftsig = new char[5];
    curforimg->ReadContent((uint8_t*)mftsig, mftoffset + 3 * mftentrybytesize, 4);
    mftsig[4] = 0;
    //std::cout << "mftsig: |" << mftsig << "|" << std::endl;
    //if(FXString(mftsig).find("FILE") > -1) // proper mft entry
    if(strcmp(mftsig, "FILE") == 0) // proper mft entry
    {
	uint16_t curoffset = 0;
	ReadForImgContent(curforimg, &curoffset, mftoffset + 3*mftentrybytesize + 20);
	uint16_t attrcnt = 0;
	ReadForImgContent(curforimg, &attrcnt, mftoffset + 3*mftentrybytesize + 40);
	//std::cout << "attrcnt:" << attrcnt << std::endl;
	for(uint i=0; i < attrcnt; i++)
	{
	    uint32_t attrtype = 0;
	    ReadForImgContent(curforimg, &attrtype, mftoffset + 3*mftentrybytesize + curoffset);
	    //std::cout << "attr type: " << std::hex << attrtype << std::endl;
	    if(attrtype == 0x60)
		break;
	    uint32_t attrlen = 0;
	    ReadForImgContent(curforimg, &attrlen, mftoffset + 3*mftentrybytesize + curoffset + 4);
	    //std::cout << "attrlen: " << attrlen << std::endl;
	    if(attrlen == 0 || attrtype == 0xffffffff)
		break;
	    curoffset += attrlen;
	    //std::cout << "curoffset: " << curoffset << std::endl;
	}
	//std::cout << "curoffset: " << curoffset;
	uint32_t attrnamelength = 0;
	ReadForImgContent(curforimg, &attrnamelength, mftoffset + 3*mftentrybytesize + curoffset + 16);
	for(uint k=0; k < attrnamelength / 2; k++)
	{
	    uint16_t singleletter = 0;
	    uint16_t attrnameoffset = 0;
	    ReadForImgContent(curforimg, &attrnameoffset, mftoffset + 3*mftentrybytesize + curoffset + 20);
	    ReadForImgContent(curforimg, &singleletter, mftoffset + 3*mftentrybytesize + curoffset + attrnameoffset + k*2);
	    partitionname += (char)singleletter;
	}
	partitionname += " [NTFS]";
	//std::cout << "partition name: " << partitionname.text() << std::endl;
    }
*/
