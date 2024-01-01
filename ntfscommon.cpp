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
			// RUN LIST OFFSET
			uint16_t runlistoffset = 0;
			ReadForImgContent(curimg, &runlistoffset, offset + curoffset + 32);
			uint currentrunoffset = offset + curoffset + runlistoffset;
			std::vector<uint64_t> runoffsets;
			std::vector<uint64_t> runlengths;
			runoffsets.clear();
			runlengths.clear();
			int i = 0;
			while(currentrunoffset < offset + curoffset + attributelength)
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
			    //mftsize += runlengths.at(i) * bytespercluster;
			}
			runoffsets.clear();
			runlengths.clear();
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
			//mftsize = contentsize;
		    }
		}
	    }
	    if(attributelength == 0 || attributetype == 0xffffffff)
		break;
	    curoffset += attributelength;
	}
    }
    //std::cout << "layout: " << *layout << std::endl;
    //uint64_t mftmaxentries = mftsize / mftentrybytes;
}
