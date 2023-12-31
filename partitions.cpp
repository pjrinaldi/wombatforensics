#include "partitions.h"

void LoadPartitions(ForImg* curforimg, std::vector<std::string>* volnames, std::vector<uint64_t>* volsizes, std::vector<uint64_t>* voloffsets, std::vector<std::string>* volprops)
{
    uint16_t mbrsig = 0;
    uint16_t applesig  = 0;
    uint32_t bsdsig = 0;
    uint16_t sunsig = 0;
    uint64_t gptsig = 0;
    ReadForImgContent(curforimg, &mbrsig, 510);
    ReadForImgContent(curforimg, &applesig, 0);
    ReadForImgContent(curforimg, &bsdsig, 0);
    ReadForImgContent(curforimg, &sunsig, 508);
    ReadForImgContent(curforimg, &gptsig, 0);
    if(mbrsig == 0xaa55) // possibly MBR or GPT
    {
	//std::cout << "possibly mbr or gpt" << std::endl;
        uint8_t gptdif = 0x00;
        curforimg->ReadContent(&gptdif, 450, 1);
        if((uint)gptdif == 0xee) // GPT DISK
        {
            ReadForImgContent(curforimg, &gptsig, 512, 8);
            if(gptsig == 0x4546492050415254) // GUID PARTITION TABLE
            {
                LoadGptPartitions(curforimg, volnames, volsizes, voloffsets, volprops);
            }
        }
        else // MBR DISK
        {
	    //std::cout << "mbr disk" << std::endl;
            char* exfattype = new char[6];
            curforimg->ReadContent((uint8_t*)exfattype, 3, 5);
            exfattype[5] = 0;
            char* fattype = new char[6];
            curforimg->ReadContent((uint8_t*)fattype, 54, 5);
            fattype[5] = 0;
            char* fat32type = new char[6];
            curforimg->ReadContent((uint8_t*)fat32type, 82, 5);
            fat32type[5] = 0;
            char* ntfstype = new char[5];
            curforimg->ReadContent((uint8_t*)ntfstype, 3, 4);
            ntfstype[4] = 0;
            char* bfstype = new char[5];
            curforimg->ReadContent((uint8_t*)bfstype, 544, 4);
            bfstype[4] = 0;
            int pcount = 0;
            for(int i=0; i < 4; i++)
            {
                uint32_t partsize = 0;
                ReadForImgContent(curforimg, &partsize, 458 + i*16);
                if(partsize > 0 && partsize <= curforimg->Size())
                    pcount++;
            }
	    //std::cout << "pcount: " << pcount << std::endl;
            if(strcmp(exfattype, "EXFAT") == 0 || strcmp(fattype, "FAT12") == 0 || strcmp(fattype, "FAT16") == 0 || strcmp(fat32type, "FAT32") == 0 || strcmp(ntfstype, "NTFS") == 0 || strcmp(bfstype, "1SFB") == 0) // EXFAT | FAT12 | FAT16 | FAT32 | NTFS | BFS W/O PARTITION TABLE
            {
		pcount = 0;
		//std::cout << "ntfs from mbr" << std::endl;
		GetVolumeProperties(curforimg, 0, volprops);
                volnames->push_back(GetFileSystemName(curforimg, 0));
                volsizes->push_back(curforimg->Size());
                voloffsets->push_back(0);
            }
            for(int i=0; i < pcount; i++)
            {
                uint8_t curparttype = 0;
                curforimg->ReadContent(&curparttype, 450 + i*16, 1);
                uint32_t curoffset = 0;
                ReadForImgContent(curforimg, &curoffset, 454 + i*16);
                uint32_t cursize = 0;
                ReadForImgContent(curforimg, &cursize, 458 + i*16);
                uint64_t sectorcheck = 0;
                if(i == 0)
                    sectorcheck = 0;
                else if(i > 0 && i < pcount - 1)
                {
                    uint32_t prevoffset = 0;
                    ReadForImgContent(curforimg, &prevoffset, 454 + (i-1) * 16);
                    uint32_t prevsize = 0;
                    ReadForImgContent(curforimg, &prevsize, 458 + (i-1) * 16);
                    sectorcheck = prevoffset + prevsize;
                }
                else if(i == pcount - 1)
                    sectorcheck = curforimg->Size() / 512;
                if(curoffset > sectorcheck) // add unallocated partition
                {
                    // Parse Partition(curforimg, sectorcheck, curoffset, ptreecnt, 0);
                    volnames->push_back("UNALLOCATED");
                    volsizes->push_back(curoffset*512);
                    voloffsets->push_back(sectorcheck*512);
                }
                if((uint)curparttype == 0x05) // extended partition
                {
                    LoadExtendedPartitions(curforimg, curoffset * 512, cursize * 512, volnames, volsizes, voloffsets, volprops);
                    // ptreecnt = Parse Extended Partition(curforimg, curoffset, cursize, ptreecnt);
                }
                else if((uint)curparttype == 0x00)
                {
                    // do nothing here cause it's an empty partition
                }
                else if((uint)curparttype == 0x82) // Sun i386
                {
                    // parse sun table here passing pofflist nad psizelist
                }
                else if((uint)curparttype == 0xa5 || (uint)curparttype == 0xa6 || (uint)curparttype == 0xa9) // BSD
                {
                    // parse BSD table here passing pofflist and psizelist
                }
                else
                {
                    if(cursize > 0)
                    {
			//std::cout << "ntfs from mbr partition" << std::endl;
                        // Parse Partition(curforimg, curoffset, cursize, ptreecnt, 1);
			GetVolumeProperties(curforimg, 0, volprops);
                        volnames->push_back(GetFileSystemName(curforimg, curoffset*512));
                        volsizes->push_back(cursize*512);
                        voloffsets->push_back(curoffset*512);
                    }
                }
                if( i == pcount - 1 && curoffset + cursize < curforimg->Size() / 512 - 1) // ADD UNALLOCATED AT END
                {
                    volnames->push_back("UNALLOCATED");
                    volsizes->push_back((curforimg->Size() / 512 - (curoffset + cursize)) * 512);
                    voloffsets->push_back((curoffset + cursize) * 512);
                    // parse partition(curforimg, curoffset + cursize, curimg->Size / 512 - (curoffset + cursize), ptreecnt, 0);
                }
                    
            }
        }
    }
    else if(applesig == 0x504d) // APPLE PARTITION
    {
        std::cout << "apple sig here..." << std::endl;
    }
    else if(bsdsig == 0x82564557) // BSD PARTITION
    {
        std::cout << "bsd part here..." << std::endl;
    }
    else if(sunsig == 0xDABE) // SUN PARTITION
    {
        std::cout << "determine if sparc or i386 and then process partitions." << std::endl;
    }
    else if(gptsig == 0x4546492050415254) // GPT PARTITION
    {
	//std::cout << "ntfs from gpt partitions" << std::endl;
        LoadGptPartitions(curforimg, volnames, volsizes, voloffsets, volprops);
    }
    else // NO PARTITION MAP, JUST A FS AT ROOT OF IMAGE
    {
	//std::cout << "ntfs from no partitions" << std::endl;
	GetVolumeProperties(curforimg, 0, volprops);
        volnames->push_back(GetFileSystemName(curforimg, 0));
        volsizes->push_back(curforimg->Size());
        voloffsets->push_back(0);
    }
}

void LoadGptPartitions(ForImg* curforimg, std::vector<std::string>* volnames, std::vector<uint64_t>* volsizes, std::vector<uint64_t>* voloffsets, std::vector<std::string>* volprops)
{
    uint64_t parttablestart = 0;
    uint32_t partentrycount = 0;
    uint32_t partentrysize = 0;
    ReadForImgContent(curforimg, &parttablestart, 584);
    ReadForImgContent(curforimg, &partentrycount, 592);
    ReadForImgContent(curforimg, &partentrysize, 596);
    //int ptreecnt = 0;
    int pcount = 0;
    for(int i=0; i < partentrycount; i++)
    {
        int cnt = i * partentrysize;
        uint64_t curstartsector = 0;
        uint64_t curendsector = 0;
        ReadForImgContent(curforimg, &curstartsector, parttablestart * 512 + cnt + 32);
        ReadForImgContent(curforimg, &curendsector, parttablestart * 512 + cnt + 40);
        if(curendsector - curstartsector > 0) // PARTITION VALUES MAKE SENSE
            pcount++;
    }
    for(int i=0; i < pcount; i++)
    {
        //itemtype = 2;
        uint64_t sectorcheck = 0;
        int cnt = i * partentrysize;
        uint64_t curstartsector = 0;
        uint64_t curendsector = 0;
        ReadForImgContent(curforimg, &curstartsector, parttablestart * 512 + cnt + 32);
        ReadForImgContent(curforimg, &curendsector, parttablestart * 512 + cnt + 40);
        if(i == 0) // INITIAL PARTITION
            sectorcheck = 0;
        else if(i > 0 && i < partentrycount)
        {
            uint64_t prestart = 0;
            uint64_t preend = 0;
            ReadForImgContent(curforimg, &prestart, parttablestart * 512 + (i-1) * partentrysize + 32);
            ReadForImgContent(curforimg, &preend, parttablestart * 512 + (i-1) * partentrysize + 40);
            sectorcheck = prestart + preend;
        }
        else if(i == pcount - 1)
            sectorcheck = curforimg->Size() / 512;
        if(curendsector - curstartsector > 0) // PARTITION VALUES MAKE SENSE
        {
            if(curstartsector > sectorcheck) // UNALLOCATED PARTITION BEFORE THE FIRST PARTITION
            {
                volnames->push_back("UNALLOCATED");
                volsizes->push_back(curstartsector*512);
                voloffsets->push_back(sectorcheck);
                // LOAD UNALLOCATED PARTITON sectorcheck, curstartsector, 0
            }
            // LOAD ALLOCATED PARTITION READ FROM TABLE curstartsector, (curendsector - curstartsector + 1), 1)
	    GetVolumeProperties(curforimg, 0, volprops);
            volnames->push_back(GetFileSystemName(curforimg, curstartsector*512));
            volsizes->push_back((curendsector - curstartsector + 1)*512);
            voloffsets->push_back(curstartsector*512);
            if(i == pcount - 1) // ADD UNALLOCATED AFTER LAST VALID PARTITION IF EXISTS
            {
                if(curendsector < curforimg->Size() / 512)
                {
                    // LOAD UNALLOCATED curendsector+1, curforimg->Size() / 512 - 1 - curendsector, 0)
                    volnames->push_back("UNALLOCATED");
                    volsizes->push_back((curforimg->Size() / 512 - 1 - curendsector) * 512);
                    voloffsets->push_back((curendsector + 1) * 512);
                }
            }
        }
    }
}

void LoadExtendedPartitions(ForImg* curforimg, uint64_t epoffset, uint64_t epsize, std::vector<std::string>* volnames, std::vector<uint64_t>* volsizes, std::vector<uint64_t>* voloffsets, std::vector<std::string>* volprops)
{
    int pcount = 0;
    for(int i=0; i < 4; i++)
    {
        uint32_t partsize = 0;
        ReadForImgContent(curforimg, &partsize, epoffset + 458 + i*16);
        if(partsize > 0)
            pcount++;
    }
    for(int i=0; i < pcount; i++)
    {
        uint8_t curparttype = 0;
        curforimg->ReadContent(&curparttype, epoffset + 450 + i*16, 1);
        uint32_t curoffset = 0;
        ReadForImgContent(curforimg, &curoffset, epoffset + 454 + i*16);
        uint32_t cursize = 0;
        ReadForImgContent(curforimg, &cursize, epoffset + 458 + i*16);
        uint64_t sectorcheck = 0;
	if(i == 0) // ADD INITIAL UNALLOCATED PARTITION
            sectorcheck = 0;
        else if(i > 0 && i < pcount - 1)
        {
            uint32_t prevoffset = 0;
            ReadForImgContent(curforimg, &prevoffset, epoffset + 454 + (i-1) * 16);
            uint32_t prevsize = 0;
            ReadForImgContent(curforimg, &prevsize, epoffset + 458 + (i-1) * 16);
            sectorcheck = prevoffset + prevsize;
        }
        else if(i == pcount - 1)
            sectorcheck = epsize / 512;
        if(curoffset > sectorcheck) // add unallocated partition
        {
            volnames->push_back("UNALLOCATED");
            volsizes->push_back((curoffset - sectorcheck)*512);
            voloffsets->push_back(epoffset + sectorcheck*512);
        }
        if((uint)curparttype == 0x05) // extended partition
        {
            LoadExtendedPartitions(curforimg, epoffset + curoffset * 512, cursize * 512, volnames, volsizes, voloffsets, volprops);
        }
        else if((uint)curparttype == 0x00)
        {
            // do nothing here cause it's an empty partition
        }
        else if((uint)curparttype == 0x82) // Sun i386
        {
            // parse sun table here passing pofflist nad psizelist
        }
        else if((uint)curparttype == 0xa5 || (uint)curparttype == 0xa6 || (uint)curparttype == 0xa9) // BSD
        {
            // parse BSD table here passing pofflist and psizelist
        }
        else
        {
            if(cursize > 0)
            {
                // Parse Partition(curforimg, curoffset, cursize, ptreecnt, 1);
		GetVolumeProperties(curforimg, 0, volprops);
                volnames->push_back(GetFileSystemName(curforimg, epoffset + curoffset*512));
                volsizes->push_back(cursize*512);
                voloffsets->push_back(epoffset + curoffset*512);
            }
        }
        if( i == pcount - 1 && (epoffset / 512 + curoffset + cursize) < epsize / 512 - 1) // ADD UNALLOCATED AT END
        {
            volnames->push_back("UNALLOCATED");
            volsizes->push_back((epsize / 512 - (curoffset + cursize)) * 512);
            voloffsets->push_back(epoffset + (curoffset + cursize) * 512);
            // parse partition(curforimg, curoffset + cursize, curimg->Size / 512 - (curoffset + cursize), ptreecnt, 0);
        }
    }
}

void GetVolumeProperties(ForImg* curforimg, uint64_t offset, std::vector<std::string>* volprops)
{
    std::string properties = "";
    uint16_t sig16 = 0;
    uint32_t sig32 = 0;
    uint64_t sig64 = 0;
    ReadForImgContent(curforimg, &sig32, offset + 544);
    ReadForImgContent(curforimg, &sig16, offset + 510);
    if(sig32 == 0x42465331) // BFS1 - BeFS
    {
    }
    if(sig16 == 0xaa55) // FAT12, FAT16, FAT3, EXFAT, or NTFS
    {
	char* fattype = new char[6];
	curforimg->ReadContent((uint8_t*)fattype, offset + 3, 5);
	fattype[5] = 0;
	if(strcmp(fattype, "EXFAT") == 0)
	{
	    // BYTES PER SECTOR
	    uint8_t bps = 0;
	    curforimg->ReadContent(&bps, offset + 108, 1);
	    uint16_t bytespersector = pow(2, (uint)bps);
	    // FAT COUNT
	    uint8_t fatcount = 0;
	    curforimg->ReadContent(&fatcount, offset + 110, 1);
	    // SECTORS PER CLUSTER
	    uint8_t spc = 0;
	    curforimg->ReadContent(&spc, offset + 109, 1);
	    uint16_t sectorspercluster = pow(2, (uint)spc);
	    // RESERVED AREA SIZE
	    uint16_t reservedareasize = 0;
	    ReadForImgContent(curforimg, &reservedareasize, offset + 14);
	    // ROOT DIRECTORY MAX FILES
	    uint16_t rootdirmaxfiles = 0;
	    ReadForImgContent(curforimg, &rootdirmaxfiles, offset + 17);
	    // FAT SIZE
	    uint32_t fatsize = 0;
	    ReadForImgContent(curforimg, &fatsize, offset + 84);
	    // FAT OFFSET
	    uint32_t fatoff = 0;
	    ReadForImgContent(curforimg, &fatoff, offset + 80);
	    uint64_t fatoffset = fatoff + offset;
	    // ROOT DIR CLUSTER
	    uint32_t rootdircluster = 0;
	    ReadForImgContent(curforimg, &rootdircluster, offset + 96);
	    // CLUSTER AREA START
	    uint32_t clusterstart = 0;
	    ReadForImgContent(curforimg, &clusterstart, offset + 88);
	    //std::cout << "cluster start: " << clusterstart << std::endl;
	    // ROOT DIRECTORY OFFSET
	    uint64_t rootdiroffset = (uint64_t)(offset + (((rootdircluster - 2) * sectorspercluster) + clusterstart) * bytespersector);
	    //std::cout << "root dir offset: " << rootdiroffset << std::endl;
	    // ROOT DIRECTORY SIZE
	    uint64_t rootdirsize = (rootdirmaxfiles * 32) + (bytespersector - 1);
            std::vector<uint> clusterlist;
	    clusterlist.clear();
	    if(rootdircluster >= 2)
	    {
		clusterlist.push_back(rootdircluster);
		GetNextCluster(curforimg, rootdircluster, 4, fatoffset, &clusterlist);
	    }
	    std::string rootdirlayout = ConvertBlocksToExtents(&clusterlist, sectorspercluster * bytespersector, clusterstart * bytespersector);
	    //std::cout << "rootdirlayout: " << rootdirlayout << std::endl;
	    std::string partitionname = "";
	    for(int i=0; i < clusterlist.size(); i++)
	    {
		//std::cout << "cluster: " << i << " | " << clusterlist.at(i) << std::endl;
		uint64_t clustersize = sectorspercluster * bytespersector;
		//std::cout << "clustersize: " << clustersize << std::endl;
		uint curoffset = 0;
		while(curoffset < clustersize)
		{
		    uint64_t clusteroffset = (((clusterlist.at(i) - 2) * sectorspercluster) + clusterstart) * bytespersector;
		    //std::cout << "clusteroffset: " << clusteroffset << std::endl;
		    uint8_t firstbyte = 0;
		    curforimg->ReadContent(&firstbyte, rootdiroffset + curoffset, 1);
		    //std::cout  << "firstbyte: " << std::hex << (uint)firstbyte << std::endl;
		    if((uint)firstbyte == 0x83)
			break;
		    curoffset += 32;
		}
		//std::cout << "curoffset: " << curoffset << std::endl;
		if(curoffset < clustersize)
		{
		    uint8_t secondbyte = 0;
		    curforimg->ReadContent(&secondbyte, rootdiroffset + curoffset + 1, 1);
		    //std::cout << "second byte: " << (uint)secondbyte << std::endl;
		    if((uint)secondbyte > 0)
		    {
			for(uint j=0; j < (uint)secondbyte; j++)
			{
			    uint16_t singleletter = 0;
			    ReadForImgContent(curforimg, &singleletter, rootdiroffset + curoffset + 2 + j*2);
			    partitionname += (char)singleletter;
			    //std::cout << "singleletter: " << (char)singleletter << std::endl;
			}
		    }
		}
	    }
	    properties = std::to_string(reservedareasize) + ">" + std::to_string(rootdirmaxfiles) + ">" + std::to_string(fatsize) + ">" + partitionname + ">" + std::to_string(bytespersector) + ">" + std::to_string(sectorspercluster) + ">" + std::to_string(fatcount) + ">" + std::to_string(fatoffset) + ">" + std::to_string(clusterstart) + ">" + std::to_string(rootdiroffset) + ">" + std::to_string(rootdirsize) + ">" + rootdirlayout;
	    volprops->push_back(properties);
	}
        else if(std::string(fattype).find("NTFS") != std::string::npos)
	{
	    // BYTES PER SECTOR
	    uint16_t bytespersector = 0;
	    ReadForImgContent(curforimg, &bytespersector, offset + 11);
	    // SECTORS PER CLUSTER
	    uint8_t sectorspercluster = 0;
	    curforimg->ReadContent(&sectorspercluster, offset + 13, 1);
	    // TOTAL SETORS
	    uint64_t totalsectors = 0;
	    ReadForImgContent(curforimg, &totalsectors, offset + 40);
	    // BYTES PER CLUSTER
	    uint32_t bytespercluster = bytespersector * (uint)sectorspercluster;
	    // MFT STARTING CLUSTER
	    uint64_t mftstartingcluster = 0;
	    ReadForImgContent(curforimg, &mftstartingcluster, offset + 48);
	    // MFT STARTING OFFSET
	    uint64_t mftstartingoffset = offset + mftstartingcluster * bytespercluster;
	    // MFT ENTRY SIZE
	    int8_t mftentrysize = 0;
	    uint8_t mes = 0;
	    curforimg->ReadContent(&mes, offset + 64, 1);
	    mftentrysize = (int8_t)mes;
	    // MFT ENTRY BYTES
	    uint64_t mftentrybytes = 0;
	    if((int)mftentrysize == -10)
		mftentrybytes = pow(2, abs((int)mftentrysize));
	    else
		mftentrybytes = (uint)mftentrysize * bytespercluster;
	    // VOLUME SERIAL
	    uint64_t volserial = 0;
	    ReadForImgContent(curforimg, &volserial, offset + 72);
	    std::stringstream serialstream;
	    serialstream << "0x" << std::setfill('0') << std::setw(sizeof(uint8_t)*2) << std::hex << volserial;
	    uint8_t indexrecordsize = 0;
	    curforimg->ReadContent(&indexrecordsize, offset + 68, 1);
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
	    //std::cout << "mft layout: " << mftlayout << std::endl;
	    //std::cout << "mft size: " << mftsize << std::endl;
	    uint64_t mftmaxentries = mftsize / mftentrybytes;
	    //std::cout << "mft max entries: " << mftmaxentries << std::endl;
	    // $VOLUME_NAME
	    uint64_t volnameoffset = mftstartingoffset + (3 * mftentrybytes);
	    char* mftentrysignature2 = new char[5];
	    curforimg->ReadContent((uint8_t*)mftentrysignature2, volnameoffset, 4);
	    mftentrysignature2[4] = 0;
	    std::string volumelabel = "";
	    if(strcmp(mftentrysignature2, "FILE") == 0) // A PROPER MFT ENTRY
	    {
		// FIRST ATTRIBUTE OFFSET
		uint16_t firstattributeoffset = 0;
		ReadForImgContent(curforimg, &firstattributeoffset, volnameoffset + 20);
		uint16_t curoffset = firstattributeoffset;
		// ATTRIBUTE COUNT
		uint16_t attributecount = 0;
		ReadForImgContent(curforimg, &attributecount, volnameoffset + 40);
		// LOOP OVER ATTRIBUTES TO FIND DATA ATTRIBUTE
		for(int i=0; i < attributecount; i++)
		{
		    // ATTRIBUTE TYPE
		    uint32_t attributetype = 0;
		    ReadForImgContent(curforimg, &attributetype, volnameoffset + curoffset);
		    // ATTRIBUTE LENGTH
		    uint32_t attributelength = 0;
		    ReadForImgContent(curforimg, &attributelength, volnameoffset + curoffset + 4);
		    if(attributetype == 0x60)
			break;
		    if(attributelength == 0 || attributetype == 0xffffffff)
			break;
		    curoffset += attributelength;
		}
		// VOLUME LABEL LENGTH
		uint32_t vollabellength = 0;
		ReadForImgContent(curforimg, &vollabellength, volnameoffset + curoffset + 16);
		// VOLUME LABEL OFFSET
		uint16_t vollabeloffset = 0;
		ReadForImgContent(curforimg, &vollabeloffset, volnameoffset + curoffset + 20);
		for(uint k=0; k < vollabellength/2; k++)
		{
		    uint16_t singleletter = 0;
		    ReadForImgContent(curforimg, &singleletter, volnameoffset + curoffset + vollabeloffset + k*2);
		    volumelabel += (char)singleletter;
		}
	    }
	    //std::cout << "volume label: " << volumelabel << std::endl;
	    properties = std::to_string(bytespersector) + ">" + std::to_string((uint)sectorspercluster) + ">" + std::to_string(totalsectors) + ">" + std::to_string(bytespercluster) + ">" + std::to_string(mftstartingcluster) + ">" + std::to_string(mftstartingoffset) + ">" + std::to_string((int)mftentrysize) + ">" + std::to_string(mftentrybytes) + ">" + serialstream.str() + ">" + std::to_string((uint)indexrecordsize) + ">" + mftlayout + ">" + std::to_string(mftmaxentries) + ">" + volumelabel;
	    volprops->push_back(properties);
	}
        else
	{
            //char* pname = new char[12];
            curforimg->ReadContent((uint8_t*)fattype, offset + 54, 5);
            if(strcmp(fattype, "FAT12") == 0)
	    {
		// BYTES PER SECTOR
		uint16_t bytespersector = 0;
		ReadForImgContent(curforimg, &bytespersector, offset + 11);
		// FAT COUNT
		uint8_t fatcount = 0;
		curforimg->ReadContent(&fatcount, offset + 16, 1);
		// SECTORS PER CLUSTER
		uint8_t sectorspercluster = 0;
		curforimg->ReadContent(&sectorspercluster, offset + 13, 1);
		// RESERVED AREA SIZE
		uint16_t reservedareasize = 0;
		ReadForImgContent(curforimg, &reservedareasize, offset + 14);
		// ROOT DIRECTORY MAX FILES
		uint16_t rootdirmaxfiles = 0;
		ReadForImgContent(curforimg, &rootdirmaxfiles, offset + 17);
		// FAT SIZE
		uint16_t fatsize = 0;
		ReadForImgContent(curforimg, &fatsize, offset + 22);
		// CLUSTER AREA START
		uint64_t clusterareastart = reservedareasize + fatcount * fatsize + ((rootdirmaxfiles * 32) + (bytespersector - 1)) / bytespersector;
		// DIRECTORY OFFSET
		uint64_t diroffset = offset + (reservedareasize + fatcount * fatsize) * bytespersector;
		// DIRECTORY SIZE
		uint64_t dirsize = rootdirmaxfiles * 32 + bytespersector - 1;
		// DIRECTORY SECTORS
		uint64_t dirsectors = dirsize / bytespersector;
		// ROOT DIRECTORY LAYOUT
		std::string rootdirlayout = std::to_string(diroffset) + "," + std::to_string(dirsize) + ";";
		// FAT OFFSET
		uint64_t fatoffset = offset + reservedareasize * bytespersector;
		// FILE SYSTEM SECTOR COUNT
		uint16_t filesystemsectorcount = 0;
		ReadForImgContent(curforimg, &filesystemsectorcount, offset + 19);
		// FILE SYSTEM SERIAL NUMBER
		uint32_t volserial = 0;
		ReadForImgContent(curforimg, &volserial, offset + 39);
		std::stringstream serialstream;
		serialstream << "0x" << std::setfill('0') << std::setw(sizeof(uint8_t)*2) << std::hex << volserial;
		//std::cout << serialstream.str() << std::endl;
		// FILE SYSTEM LABEL
		char* vollabel = new char[12];
		curforimg->ReadContent((uint8_t*)vollabel, offset + 43, 11);
		vollabel[11] = 0;
		properties = std::to_string(reservedareasize) + ">" + std::to_string(rootdirmaxfiles) + ">" + std::to_string(fatcount) + ">" + std::to_string(bytespersector) + ">" + std::to_string(sectorspercluster) + ">" + std::to_string(filesystemsectorcount) + ">" + std::to_string(fatsize) + ">" + serialstream.str() + ">" + std::string(vollabel) + ">" + std::to_string(fatoffset) + ">" + std::to_string(diroffset) + ">" + std::to_string(dirsectors) + ">" + std::to_string(dirsize) + ">" + std::to_string(clusterareastart) + ">" + rootdirlayout;
		volprops->push_back(properties);
	    }
            else if(strcmp(fattype, "FAT16") == 0)
            {
		// BYTES PER SECTOR
		uint16_t bytespersector = 0;
		ReadForImgContent(curforimg, &bytespersector, offset + 11);
		// FAT COUNT
		uint8_t fatcount = 0;
		curforimg->ReadContent(&fatcount, offset + 16, 1);
		// SECTORS PER CLUSTER
		uint8_t sectorspercluster = 0;
		curforimg->ReadContent(&sectorspercluster, offset + 13, 1);
		// RESERVED AREA SIZE
		uint16_t reservedareasize = 0;
		ReadForImgContent(curforimg, &reservedareasize, offset + 14);
		// ROOT DIRECTORY MAX FILES
		uint16_t rootdirmaxfiles = 0;
		ReadForImgContent(curforimg, &rootdirmaxfiles, offset + 17);
		// FAT SIZE
		uint16_t fatsize = 0;
		ReadForImgContent(curforimg, &fatsize, offset + 22);
		// CLUSTER AREA START
		uint64_t clusterareastart = reservedareasize + fatcount * fatsize + ((rootdirmaxfiles * 32) + (bytespersector - 1)) / bytespersector;
		// DIRECTORY OFFSET
		uint64_t diroffset = offset + (reservedareasize + fatcount * fatsize) * bytespersector;
		// DIRECTORY SIZE
		uint64_t dirsize = rootdirmaxfiles * 32 + bytespersector - 1;
		// DIRECTORY SECTORS
		uint64_t dirsectors = dirsize / bytespersector;
		// ROOT DIRECTORY LAYOUT
		std::string rootdirlayout = std::to_string(diroffset) + "," + std::to_string(dirsize) + ";";
		// FAT OFFSET
		uint64_t fatoffset = offset + reservedareasize * bytespersector;
		// FILE SYSTEM SECTOR COUNT
		uint16_t filesystemsectorcount = 0;
		ReadForImgContent(curforimg, &filesystemsectorcount, offset + 19);
		// FILE SYSTEM SERIAL NUMBER
		uint32_t volserial = 0;
		ReadForImgContent(curforimg, &volserial, offset + 39);
		std::stringstream serialstream;
		serialstream << "0x" << std::setfill('0') << std::setw(sizeof(uint8_t)*2) << std::hex << volserial;
		//std::cout << serialstream.str() << std::endl;
		// FILE SYSTEM LABEL
		char* vollabel = new char[12];
		curforimg->ReadContent((uint8_t*)vollabel, offset + 43, 11);
		vollabel[11] = 0;
		properties = std::to_string(reservedareasize) + ">" + std::to_string(rootdirmaxfiles) + ">" + std::to_string(fatcount) + ">" + std::to_string(bytespersector) + ">" + std::to_string(sectorspercluster) + ">" + std::to_string(filesystemsectorcount) + ">" + std::to_string(fatsize) + ">" + serialstream.str() + ">" + std::string(vollabel) + ">" + std::to_string(fatoffset) + ">" + std::to_string(diroffset) + ">" + std::to_string(dirsectors) + ">" + std::to_string(dirsize) + ">" + std::to_string(clusterareastart) + ">" + rootdirlayout;
		volprops->push_back(properties);
	    }
	    else
	    {
                curforimg->ReadContent((uint8_t*)fattype, offset + 82, 5);
                fattype[5] = 0;
                if(strcmp(fattype, "FAT32") == 0)
                {
		    // BYTES PER SECTOR
		    uint16_t bytespersector = 0;
		    ReadForImgContent(curforimg, &bytespersector, offset + 11);
		    // FAT COUNT
		    uint8_t fatcount = 0;
		    curforimg->ReadContent(&fatcount, offset + 16, 1);
		    // SECTORS PER CLUSTER
		    uint8_t sectorspercluster = 0;
		    curforimg->ReadContent(&sectorspercluster, offset + 13, 1);
		    // RESERVED AREA SIZE
		    uint16_t reservedareasize = 0;
		    ReadForImgContent(curforimg, &reservedareasize, offset + 14);
		    // FAT SIZE
		    uint32_t fatsize = 0;
		    ReadForImgContent(curforimg, &fatsize, offset + 36);
		    // CLUSTER AREA START
		    uint64_t clusterareastart = (offset/ bytespersector) + reservedareasize + fatcount * fatsize;
		    // DIRECTORY OFFSET
		    uint64_t diroffset = offset + (reservedareasize + fatcount * fatsize) * bytespersector;
		    // ROOT DIRECTORY CLUSTER
		    uint32_t rootdircluster = 0;
		    ReadForImgContent(curforimg, &rootdircluster, offset + 44);
		    // FAT OFFSET
		    uint64_t fatoffset = offset + reservedareasize * bytespersector;
		    // ROOT DIRECTORY LAYOUT
		    std::vector<uint> clusterlist;
		    clusterlist.clear();
		    // FirstSectorOfCluster = ((N-2) * sectorspercluster) + firstdatasector [rootdirstart];
		    if(rootdircluster >= 2)
		    {
			clusterlist.push_back(rootdircluster);
			GetNextCluster(curforimg, rootdircluster, 3, fatoffset, &clusterlist);
		    }
		    std::string rootdirlayout = ConvertBlocksToExtents(&clusterlist, sectorspercluster * bytespersector, diroffset);
		    // FILE SYSTEM SECTOR COUNT
		    uint32_t filesystemsectorcount = 0;
		    ReadForImgContent(curforimg, &filesystemsectorcount, offset + 32);
		    // FILE SYSTEM SERIAL NUMBER
		    uint32_t volserial = 0;
		    ReadForImgContent(curforimg, &volserial, offset + 67);
		    std::stringstream serialstream;
		    serialstream << "0x" << std::setfill('0') << std::setw(sizeof(uint8_t)*2) << std::hex << volserial;
		    //std::cout << serialstream.str() << std::endl;
		    // FILE SYSTEM LABEL
		    char* vollabel = new char[12];
		    curforimg->ReadContent((uint8_t*)vollabel, offset + 71, 11);
		    vollabel[11] = 0;

		    properties = std::to_string(reservedareasize) + ">" + std::to_string(fatcount) + ">" + std::to_string(bytespersector) + ">" + std::to_string(sectorspercluster) + ">" + std::to_string(filesystemsectorcount) + ">" + std::to_string(fatsize) + ">" + serialstream.str() + ">" + std::string(vollabel) + ">" + std::to_string(fatoffset) + ">" + std::to_string(diroffset) + ">" + std::to_string(rootdircluster) + ">" + std::to_string(clusterareastart) + ">" + rootdirlayout;
		    volprops->push_back(properties);
		}
	    }
	}
    }
    ReadForImgContent(curforimg, &sig16, offset + 1080); // EXT2/3/4
    if(sig16 == 0xef53) // EXT2/3/4
    {
	// VOLUME LABEL
	char* volname = new char[17];
	curforimg->ReadContent((uint8_t*)volname, offset + 1144, 16);
	volname[16] = 0;
	// CREATED DATETIME
	uint32_t createdtime = 0;
	ReadForImgContent(curforimg, &createdtime, offset + 1288);
	// MOUNTED DATETIME
	uint32_t mounttime = 0;
	ReadForImgContent(curforimg, &mounttime, offset + 1068);
	// WRITE DATETIME
	uint32_t writetime = 0;
	ReadForImgContent(curforimg, &writetime, offset + 1072);
	// LAST CHECKED DATETIME
	uint32_t checktime = 0;
	ReadForImgContent(curforimg, &checktime, offset + 1088);
	// CURRENT STATE
	uint16_t curstate = 0;
	std::string currentstate = "";
	ReadForImgContent(curforimg, &curstate, offset + 1082);
	if(curstate == 0x01)
	    currentstate = "Cleanly unmounted";
	else
	    currentstate = "Errors detected";
	// COMPATIBILITY FLAGS
	uint32_t compatflags = 0;
	std::string compatibilityflags = "";
	ReadForImgContent(curforimg, &compatflags, offset + 1116);
	if(compatflags & 0x200)
	    compatibilityflags += "Sparse Super Block,";
	if(compatflags & 0x100)
	    compatibilityflags += "Exlude Bitmap,";
	if(compatflags & 0x80)
	    compatibilityflags += "Exlude Inodes,";
	if(compatflags & 0x40)
	    compatibilityflags += "Lazy Block Groups,";
	if(compatflags & 0x20)
	    compatibilityflags += "Indexed Directories,";
	if(compatflags & 0x10)
	    compatibilityflags += "Reserved GDT,";
	if(compatflags & 0x08)
	    compatibilityflags += "Extended Attributes,";
	if(compatflags & 0x04)
	    compatibilityflags += "Journal,";
	if(compatflags & 0x02)
	    compatibilityflags += "Imagic Inodes,";
	if(compatflags & 0x01)
	    compatibilityflags += "Directory pre-allocation,";
	// INCOMPATIBLE FLAGS
	uint32_t incompatflags = 0;
	std::string incompatibleflags = "";
	ReadForImgContent(curforimg, &incompatflags, offset + 1120);
	if(incompatflags & 0x10000)
	    incompatibleflags += "Encrypted inodes,";
	if(incompatflags & 0x8000)
	    incompatibleflags += "Data in inode,";
	if(incompatflags & 0x4000)
	    incompatibleflags += "Large Directory greater than 2GB or 3-level htree,";
	if(incompatflags & 0x2000)
	    incompatibleflags += "Metadata checksum seed in superblock,";
	if(incompatflags & 0x1000)
	    incompatibleflags += "Data in Directory Entry,";
	if(incompatflags & 0x400)
	    incompatibleflags += "Inodes can store large extended attributes,";
	if(incompatflags & 0x200)
	    incompatibleflags += "Flexible block groups,";
	if(incompatflags & 0x100)
	    incompatibleflags += "Multiple mount protection,";
	if(incompatflags & 0x80)
	    incompatibleflags += "FS size over 2^32 blocks,";
	if(incompatflags & 0x40)
	    incompatibleflags += "Files use Extents,";
	if(incompatflags & 0x10)
	    incompatibleflags += "Meta block groups,";
	if(incompatflags & 0x08)
	    incompatibleflags += "Separate Journal device,";
	if(incompatflags & 0x04)
	    incompatibleflags += "FS needs journal recovery,";
	if(incompatflags & 0x02)
	    incompatibleflags += "Directory entries record file type,";
	if(incompatflags & 0x01)
	    incompatibleflags += "Compression,";
	// READ ONLY FLAGS
	uint32_t roflags = 0;
	std::string readonlyflags = "";
	ReadForImgContent(curforimg, &roflags, offset + 1124);
	if(roflags & 0x2000)
	    readonlyflags += "Tracks project quotas,";
	if(roflags & 0x1000)
	    readonlyflags += "Read only FS,";
	if(roflags & 0x800)
	    readonlyflags += "Replica support,";
	if(roflags & 0x400)
	    readonlyflags += "Metadata checksumming support,";
	if(roflags & 0x200)
	    readonlyflags += "Bigalloc support,";
	if(roflags & 0x100)
	    readonlyflags += "Quota is handled transactionally with journal,";
	if(roflags & 0x80)
	    readonlyflags += "Snapshot,";
	if(roflags & 0x40)
	    readonlyflags += "Large Inodes exist,";
	if(roflags & 0x20)
	    readonlyflags += "EXT3 32k subdir limit does not exist,";
	if(roflags & 0x10)
	    readonlyflags += "Group descriptors have checksums,";
	if(roflags & 0x08)
	    readonlyflags += "Space usage stored in i_blocks in units of fs_blocks, not 512-byte sectors,";
	if(roflags & 0x04)
	    readonlyflags += "Was intended for use with htree directories,";
	if(roflags & 0x02)
	    readonlyflags += "Allow storing files larger than 2GB,";
	if(roflags & 0x01)
	    readonlyflags += "Sparse superblocks,";
	// FILE SYSTEM FORMAT VERSION
	std::string format = " [EXT2]";
        if(((compatflags & 0x00000200UL) != 0) || ((incompatflags & 0x0001f7c0UL) != 0) || ((roflags & 0x00000378UL) != 0))
	    format = " [EXT4]";
        else if(((compatflags & 0x00000004UL) != 0) || ((incompatflags & 0x0000000cUL) != 0))
	    format = " [EXT3]";
	// GROUP DESCRIPTOR SIZE
	uint16_t grpdescsize = 32;
	if(incompatflags & 0x80)
	    ReadForImgContent(curforimg, &grpdescsize, offset + 1278);
	// FILE SYSTEM INODE COUNT
	uint32_t fsinodecnt = 0;
	ReadForImgContent(curforimg, &fsinodecnt, offset + 1024);
	// FILE SYSTEM BLOCK COUNT
	uint32_t fsblockcnt = 0;
	ReadForImgContent(curforimg, &fsblockcnt, offset + 1028);
	// BLOCK GROUP BLOCK COUNT
	uint32_t blockgroupblockcount = 0;
	ReadForImgContent(curforimg, &blockgroupblockcount, offset + 1056);
	// BLOCK SIZE
	uint32_t blksize = 0;
	ReadForImgContent(curforimg, &blksize, offset + 1048);
	uint32_t blocksize = 1024 * pow(2, blksize);
	// BLOCK GROUP 0 START BLOCK
	uint32_t blockgroup0startblock = 0;
	ReadForImgContent(curforimg, &blockgroup0startblock, offset + 1044);
	// FRAGMENT SIZE
	uint32_t fragsize = 0;
	ReadForImgContent(curforimg, &fragsize, offset + 1052);
	uint32_t fragmentsize = 1024 * pow(2, fragsize);
	// BLOCK GROUP FRAGMENT COUNT
	uint32_t blockgroupfragmentcount = 0;
	ReadForImgContent(curforimg, &blockgroupfragmentcount, offset + 1060);
	// BLOCK GROUP INODE COUNT
	uint32_t blockgroupinodecount = 0;
	ReadForImgContent(curforimg, &blockgroupinodecount, offset + 1064);
	// CREATOR OS
	uint32_t createos = 0;
	std::string creatoros = "";
	ReadForImgContent(curforimg, &createos, offset + 1096);
	if(createos == 0x00)
	    creatoros = "Linux";
	else if(createos == 0x03)
	    creatoros = "FreeBSD";
	// INODE SIZE
	uint16_t inodesize = 0;
	ReadForImgContent(curforimg, &inodesize, offset + 1112);
	// LAST MOUNTED PATH
	char* lastmountedpath = new char[65];
	curforimg->ReadContent((uint8_t*)lastmountedpath, offset + 1160, 64);
	lastmountedpath[64] = 0;
	// INODE ADDRESS TABLE
	uint32_t blockgroupcount = fsblockcnt / blockgroupblockcount;
	uint blkgrpcntrem = fsblockcnt % blockgroupblockcount;
	if(blkgrpcntrem > 0)
	    blockgroupcount++;
	if(blockgroupcount == 0)
	    blockgroupcount = 1;
	std::string inodeaddresstable = "";
	for(uint i=0; i < blockgroupcount; i++)
	{
	    uint32_t iat = 0;
	    if(blocksize == 1024)
		ReadForImgContent(curforimg, &iat, offset + 2*blocksize + i * grpdescsize + 8);
	    else
		ReadForImgContent(curforimg, &iat, offset + blocksize + i * grpdescsize + 8);
	    inodeaddresstable += std::to_string(iat);
	}
	// ROOT INODE TABLE ADDRESS
	uint32_t rootinodetableaddress = 0;
	if(blockgroupinodecount > 2)
	    ReadForImgContent(curforimg, &rootinodetableaddress, offset + 2056);
	else
	    ReadForImgContent(curforimg, &rootinodetableaddress, offset + 2088);
	// REVISION LEVEL
	uint32_t revmaj = 0;
	ReadForImgContent(curforimg, &revmaj, offset + 1100);
	uint16_t revmin = 0;
	ReadForImgContent(curforimg, &revmin, offset + 1086);
	properties = std::string(volname) + ">" + ConvertUnixTimeToHuman(createdtime) + ">" + ConvertUnixTimeToHuman(mounttime) + ">" + ConvertUnixTimeToHuman(writetime) + ">" + ConvertUnixTimeToHuman(checktime) + ">" + currentstate + ">" + compatibilityflags + ">" + incompatibleflags + ">" + readonlyflags + ">" + format + ">" + std::to_string(grpdescsize) + ">" + std::to_string(fsinodecnt) + ">" + std::to_string(fsblockcnt) + ">" + std::to_string(blockgroup0startblock) + ">" + std::to_string(blocksize) + ">" + std::to_string(fragmentsize) + ">" + std::to_string(blockgroupblockcount) + ">" + std::to_string(blockgroupfragmentcount) + ">" + std::to_string(blockgroupinodecount) + ">" + creatoros + ">" + std::to_string(inodesize) + ">" + std::string(lastmountedpath) + ">" + inodeaddresstable + ">" + std::to_string(rootinodetableaddress) + ">" + std::to_string(revmaj) + "." + std::to_string(revmin);
	volprops->push_back(properties);
    }
}

std::string GetFileSystemName(ForImg* curforimg, uint64_t offset)
{
    std::string partitionname = "";
    uint16_t sig16 = 0;
    uint32_t sig32 = 0;
    uint64_t sig64 = 0;
    ReadForImgContent(curforimg, &sig32, offset + 544); // BFS1
    ReadForImgContent(curforimg, &sig16, offset + 510); // FAT, NTFS, or BFS
    if(sig32 == 0x42465331) // BFS1 - BeFS
    {
        char* bfsname = new char[33];
        curforimg->ReadContent((uint8_t*)bfsname, offset + 512, 32);
        bfsname[32] = 0;
        partitionname = std::string(bfsname) + " [BFS]";
    }
    if(sig16 == 0xaa55) // FAT12, FAT16, FAT32, EXFAT, or NTFS
    {
        char* fattype = new char[6];
        curforimg->ReadContent((uint8_t*)fattype, offset + 3, 5);
        fattype[5] = 0;
        if(strcmp(fattype, "EXFAT") == 0)
        {
	    uint32_t fatsize = 0;
	    ReadForImgContent(curforimg, &fatsize, offset + 84);
	    uint8_t tmp8 = 0;
	    curforimg->ReadContent(&tmp8, offset + 108, 1);
	    uint16_t bytespersector = pow(2, (uint)tmp8);
	    curforimg->ReadContent(&tmp8, offset + 109, 1);
	    uint16_t sectorspercluster = pow(2, (uint)tmp8);
	    uint8_t fatcount = 0;
	    curforimg->ReadContent(&fatcount, offset + 110, 1);
	    uint32_t fatoff = 0;
	    ReadForImgContent(curforimg, &fatoff, offset + 80);
	    uint64_t fatoffset = fatoff + offset;
	    uint32_t rootdircluster = 0;
	    ReadForImgContent(curforimg, &rootdircluster, offset + 96);
	    uint32_t clusterstart = 0;
	    ReadForImgContent(curforimg, &clusterstart, offset + 88);
	    //std::cout << "cluster start: " << clusterstart << std::endl;
	    uint64_t rootdiroffset = (uint64_t)(offset + (((rootdircluster - 2) * sectorspercluster) + clusterstart) * bytespersector);
	    //std::cout << "root dir offset: " << rootdiroffset << std::endl;
            std::vector<uint> clusterlist;
	    clusterlist.clear();
	    if(rootdircluster >= 2)
	    {
		clusterlist.push_back(rootdircluster);
		GetNextCluster(curforimg, rootdircluster, 4, fatoffset, &clusterlist);
	    }
            //std::cout << "it's exfat..." << std::endl;
	    //FXString rootdirlayout = ConvertBlocksToExtents(clusterlist, sectorspercluster * bytespersector, clusterstart * bytespersector);
	    //std::cout << "rootdirlayout: " << rootdirlayout.text() << std::endl;
	    for(int i=0; i < clusterlist.size(); i++)
	    {
		//std::cout << "cluster: " << i << " | " << clusterlist.at(i) << std::endl;
		uint64_t clustersize = sectorspercluster * bytespersector;
		//std::cout << "clustersize: " << clustersize << std::endl;
		uint curoffset = 0;
		while(curoffset < clustersize)
		{
		    uint64_t clusteroffset = (((clusterlist.at(i) - 2) * sectorspercluster) + clusterstart) * bytespersector;
		    //std::cout << "clusteroffset: " << clusteroffset << std::endl;
		    uint8_t firstbyte = 0;
		    curforimg->ReadContent(&firstbyte, rootdiroffset + curoffset, 1);
		    //std::cout  << "firstbyte: " << std::hex << (uint)firstbyte << std::endl;
		    if((uint)firstbyte == 0x83)
			break;
		    curoffset += 32;
		}
		//std::cout << "curoffset: " << curoffset << std::endl;
		if(curoffset < clustersize)
		{
		    uint8_t secondbyte = 0;
		    curforimg->ReadContent(&secondbyte, rootdiroffset + curoffset + 1, 1);
		    //std::cout << "second byte: " << (uint)secondbyte << std::endl;
		    if((uint)secondbyte > 0)
		    {
			for(uint j=0; j < (uint)secondbyte; j++)
			{
			    uint16_t singleletter = 0;
			    ReadForImgContent(curforimg, &singleletter, rootdiroffset + curoffset + 2 + j*2);
			    partitionname += (char)singleletter;
			    //std::cout << "singleletter: " << (char)singleletter << std::endl;
			}
		    }
		}
	    }
	    partitionname += " [EXFAT]";
	    //std::cout << "part name: " << partitionname.text() << std::endl;
        }
        else if(std::string(fattype).find("NTFS") != std::string::npos)
        {
	    uint16_t bytespersector = 0;
	    ReadForImgContent(curforimg, &bytespersector, offset + 11);
	    uint8_t sectorspercluster = 0;
	    curforimg->ReadContent(&sectorspercluster, offset + 13, 1);
	    uint64_t mftstartingcluster = 0;
	    ReadForImgContent(curforimg, &mftstartingcluster, offset + 48);
	    //std::cout << "mft starting cluster: " << mftstartingcluster << std::endl;
	    //uint16_t bytespersector = pow(2, (uint)tmp8);
	    int8_t mftentrysize = 0;
	    curforimg->ReadContent((uint8_t*)&mftentrysize, offset + 64, 1);
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
        }
        else
        {
            char* pname = new char[12];
            curforimg->ReadContent((uint8_t*)fattype, offset + 54, 5);
            if(strcmp(fattype, "FAT12") == 0)
            {
                curforimg->ReadContent((uint8_t*)pname, offset + 43, 11);
                pname[11] = 0;
                partitionname = std::string(pname) + " [FAT12]";
            }
            else if(strcmp(fattype, "FAT16") == 0)
            {
                curforimg->ReadContent((uint8_t*)pname, offset + 43, 11);
                pname[11] = 0;
                partitionname = std::string(pname) + " [FAT16]";
            }
            else
            {
                curforimg->ReadContent((uint8_t*)fattype, offset + 82, 5);
                fattype[5] = 0;
                if(strcmp(fattype, "FAT32") == 0)
                {
                    curforimg->ReadContent((uint8_t*)pname, offset + 71, 11);
                    pname[11] = 0;
                    partitionname = std::string(pname) + " [FAT32]";
                }
            }
        }
    }
    ReadForImgContent(curforimg, &sig16, offset + 1080); // EXT2/3/4
    if(sig16 == 0xef53) // EXT2/3/4
    {
	char* volname = new char[17];
	curforimg->ReadContent((uint8_t*)volname, offset + 1144, 16);
	volname[16] = 0;
	uint32_t compatflags = 0;
	uint32_t incompatflags = 0;
	uint32_t readonlyflags = 0;
	ReadForImgContent(curforimg, &compatflags, offset + 1116);
	ReadForImgContent(curforimg, &incompatflags, offset + 1120);
	ReadForImgContent(curforimg, &readonlyflags, offset + 1124);
	std::string format = " [EXT2]";
        if(((compatflags & 0x00000200UL) != 0) || ((incompatflags & 0x0001f7c0UL) != 0) || ((readonlyflags & 0x00000378UL) != 0))
	    format = " [EXT4]";
        else if(((compatflags & 0x00000004UL) != 0) || ((incompatflags & 0x0000000cUL) != 0))
	    format = " [EXT3]";
	partitionname = std::string(volname) + format;
    }
    char* apfssig = new char[5];
    curforimg->ReadContent((uint8_t*)apfssig, offset + 32, 4);
    apfssig[4] = 0;
    if(strcmp(apfssig, "NXSB") == 0) // APFS
	partitionname = "APFS Container [APFS]";
    char* hfssig = new char[3];
    curforimg->ReadContent((uint8_t*)hfssig, offset + 1024, 2);
    hfssig[2] = 0;
    if(strcmp(hfssig, "H+") == 0 || strcmp(hfssig, "HX") == 0) // HFS+/X
    {
	uint32_t catalogstartoffset = 0;
	ReadForImgContent(curforimg, &catalogstartoffset, offset + 1312, true);
	uint32_t catalogstartinbytes = catalogstartoffset * 4096 + 4096;
	uint16_t keydatalength = 0;
	ReadForImgContent(curforimg, &keydatalength, catalogstartinbytes + 20, true);
	for(uint16_t i=0; i < keydatalength; i++)
	{
	    uint16_t singleletter = 0;
	    ReadForImgContent(curforimg, &singleletter, catalogstartinbytes + 22 + i*2, true);
	    partitionname += (char)singleletter;
	    //std::cout << "singleletter: " << (char)singleletter << std::endl;
	}
	if(strcmp(hfssig, "H+") == 0)
	    partitionname += " [HFS+]";
	else if(strcmp(hfssig, "HX") == 0)
	    partitionname += " [HFSX]";
    }
    else if(strcmp(hfssig, "BD") == 0) // LEGACY HFS
    {
	uint8_t volnamelength = 0;
	curforimg->ReadContent(&volnamelength, offset + 1060, 1);
	char* volname = new char[(uint)volnamelength + 1];
	curforimg->ReadContent((uint8_t*)volname, offset + 1061, (uint)volnamelength);
	volname[(uint)volnamelength] = 0;
	partitionname = std::string(volname) + " [HFS]";
    }
    char* xfssig = new char[5];
    curforimg->ReadContent((uint8_t*)xfssig, offset, 4);
    xfssig[4] = 0;
    if(strcmp(xfssig, "XFSB") == 0)
    {
	char* volname = new char[13];
	curforimg->ReadContent((uint8_t*)volname, offset + 108, 12);
	volname[12] = 0;
	partitionname = std::string(volname) + " [XFS]";
    }
    char* btrsig = new char[9];
    curforimg->ReadContent((uint8_t*)btrsig, offset + 65600, 8);
    btrsig[8] = 0;
    if(strcmp(btrsig, "_BHRfS_M") == 0)
    {
	char* volname = new char[101];
	curforimg->ReadContent((uint8_t*)volname, offset + 65536 + 0x12b, 100);
	volname[100] = 0;
	partitionname = std::string(volname) + " [BTRFS]";
    }
    char* bitlocksig = new char[9];
    curforimg->ReadContent((uint8_t*)bitlocksig, offset, 8);
    bitlocksig[8] = 0;
    if(strcmp(bitlocksig, "-FVE-FS-") == 0)
    {
	uint16_t vollength = 0;
	ReadForImgContent(curforimg, &vollength, offset + 112);
	vollength = vollength - 8;
	char* volname = new char[vollength + 1];
	curforimg->ReadContent((uint8_t*)volname, offset + 118, vollength);
	volname[vollength] = 0;
	partitionname = std::string(volname) + " [BITLOCKER]";
    }
    ReadForImgContent(curforimg, &sig32, offset + 1024);
    if(sig32 == 0xf2f52010) // F2FS
    {
	partitionname = " [F2FS]";
    }
    else if(sig32 == 0xe0f5e1e2) // EROFS
    {
	partitionname = " [EROFS]";
    }
    ReadForImgContent(curforimg, &sig32, offset);
    if(sig32 == 0x5a4f4653)
    {
	partitionname = " [ZONEFS]";
    }
    char* isosig = new char[6];
    char* udfsig = new char[6];
    curforimg->ReadContent((uint8_t*)isosig, offset + 32769, 5);
    isosig[5] = 0;
    curforimg->ReadContent((uint8_t*)udfsig, offset + 40961, 5);
    udfsig[5] = 0;
    if(strcmp(isosig, "CD001") == 0 && strcmp(udfsig, "BEA01") != 0) // ISO9660
    {
	for(int i=16; i < (curforimg->Size() / 2048) - 15; i++)
	{
	    uint64_t curoffset = offset + 2048*i;
	    uint8_t voldesctype = 0;
	    curforimg->ReadContent(&voldesctype, curoffset, 1);
	    if((uint)voldesctype == 0x01) // PRIMARY VOLUME DESCRIPTOR
	    {
		// Primary Volume Descriptor
		char* volname = new char[32];
		curforimg->ReadContent((uint8_t*)volname, curoffset + 40, 31);
		volname[31] = 0;
		partitionname = std::string(volname) + " [ISO9660]";
	    }
	}

    }
    else if(strcmp(isosig, "CD001") == 0 && strcmp(udfsig, "BEA01") == 0) // UDF
    {
	partitionname = " [UDF]";
    }
    ReadForImgContent(curforimg, &sig64, offset + 135168);
    if(sig64 == 0x00bab10c) // ZFS
    {
	partitionname = " [ZFS]";
    }
    ReadForImgContent(curforimg, &sig64, offset + 3);
    if(sig64 == 0x5265465300000000) // ReFS
    {
	partitionname = " [REFS]";
    }
    ReadForImgContent(curforimg, &sig64, offset + 4120, true);
    uint64_t bcfsig = 0;
    ReadForImgContent(curforimg, &bcfsig, offset + 4128, true);
    if(sig64 == 0xc68573f64e1a45ca && bcfsig == 0x8265f57f48ba6d81) // BCACHEFS
    {
	partitionname = " [BCACHEFS]";
    }
    
    return partitionname;
}
