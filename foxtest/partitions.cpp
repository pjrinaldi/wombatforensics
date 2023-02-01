#include "partitions.h"



void LoadPartitions(ForImg* curforimg, std::vector<std::string>* volnames, std::vector<uint64_t>* volsizes, std::vector<uint64_t>* voloffsets)
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
        uint8_t gptdif = 0x00;
        curforimg->ReadContent(&gptdif, 450, 1);
        if((uint)gptdif == 0xee) // GPT DISK
        {
            ReadForImgContent(curforimg, &gptsig, 512, 8);
            if(gptsig == 0x4546492050415254) // GUID PARTITION TABLE
            {
                LoadGptPartitions(curforimg, volnames, volsizes, voloffsets);
            }
        }
        else // MBR DISK
        {
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
            if(strcmp(exfattype, "EXFAT") == 0 || strcmp(fattype, "FAT12") == 0 || strcmp(fattype, "FAT16") == 0 || strcmp(fattype, "FAT32") == 0 || strcmp(ntfstype, "NTFS") == 0 || strcmp(bfstype, "1SFB") == 0) // EXFAT | FAT12 | FAT16 | FAT32 | NTFS | BFS W/O PARTITION TABLE
            {
                volnames->push_back(GetFileSystemName(curforimg, 0));
                volsizes->push_back(curforimg->Size());
                voloffsets->push_back(0);
            }
            //uint8_t ptreecnt = 0;
            int pcount = 0;
            for(int i=0; i < 4; i++)
            {
                uint32_t partsize = 0;
                ReadForImgContent(curforimg, &partsize, 458 + i*16);
                if(partsize > 0)
                    pcount++;
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
                    LoadExtendedPartitions(curforimg, curoffset * 512, cursize * 512, volnames, volsizes, voloffsets);
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
                        // Parse Partition(curforimg, curoffset, cursize, ptreecnt, 1);
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
        LoadGptPartitions(curforimg, volnames, volsizes, voloffsets);
    }
    else // NO PARTITION MAP, JUST A FS AT ROOT OF IMAGE
    {
        volnames->push_back(GetFileSystemName(curforimg, 0));
        volsizes->push_back(curforimg->Size());
        voloffsets->push_back(0);
    }
}

void LoadGptPartitions(ForImg* curforimg, std::vector<std::string>* volnames, std::vector<uint64_t>* volsizes, std::vector<uint64_t>* voloffsets)
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

void LoadExtendedPartitions(ForImg* curforimg, uint64_t epoffset, uint64_t epsize, std::vector<std::string>* volnames, std::vector<uint64_t>* volsizes, std::vector<uint64_t>* voloffsets)
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
            volsizes->push_back(curoffset*512);
            voloffsets->push_back(epoffset + sectorcheck*512);
        }
        if((uint)curparttype == 0x05) // extended partition
        {
            LoadExtendedPartitions(curforimg, epoffset + curoffset * 512, cursize * 512, volnames, volsizes, voloffsets);
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
                volnames->push_back(GetFileSystemName(curforimg, curoffset*512));
                volsizes->push_back(cursize*512);
                voloffsets->push_back(curoffset*512);
            }
        }
        if( i == pcount - 1 && curoffset + cursize < epsize / 512 - 1) // ADD UNALLOCATED AT END
        {
            volnames->push_back("UNALLOCATED");
            volsizes->push_back((epsize / 512 - (curoffset + cursize)) * 512);
            voloffsets->push_back((curoffset + cursize) * 512);
            // parse partition(curforimg, curoffset + cursize, curimg->Size / 512 - (curoffset + cursize), ptreecnt, 0);
        }
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
    return partitionname;
}


/*
void DetermineFileSystem(std::string devicestring, int* fstype)
{
    // get ext2,3,4 signature
    devicebuffer.seekg(1080);
    devicebuffer.read((char*)extsig, 2); // 0x53, 0xef
    // get windows mbr signature (FAT, NTFS, BFS)
    devicebuffer.seekg(510);
    devicebuffer.read((char*)winsig, 2); // 0x55, 0xaa
    // get BFS signature
    devicebuffer.seekg(544);
    devicebuffer.read(bfssig, 4);
    std::string bfsigstr(bfssig);
    delete[] bfssig;
    // get apfs signature
    devicebuffer.seekg(32);
    devicebuffer.read(apfsig, 4);
    std::string apfsigstr(apfsig);
    delete[] apfsig;
    // get hfs signature
    devicebuffer.seekg(1024);
    devicebuffer.read(hfssig, 2);
    std::string hfssigstr(hfssig);
    delete[] hfssig;
    // get xfs signature
    devicebuffer.seekg(0);
    devicebuffer.read(xfssig, 4);
    std::string xfssigstr(xfssig);
    delete[] xfssig;
    // get btrfs signature
    devicebuffer.seekg(65600);
    devicebuffer.read(btrsig, 8);
    std::string btrsigstr(btrsig);
    delete[] btrsig;
    // get bitlocker signature
    devicebuffer.seekg(0);
    devicebuffer.read(btlsig, 8);
    std::string btlsigstr(btlsig);
    delete[] btlsig;
    // get iso signature
    devicebuffer.seekg(32769);
    devicebuffer.read(isosig, 5);
    std::string isosigstr(isosig);
    delete[] isosig;
    // get udf signature
    devicebuffer.seekg(40961);
    devicebuffer.read(udfsig, 5);
    std::string udfsigstr(udfsig);
    delete[] udfsig;
    // get refs signature
    devicebuffer.seekg(3);
    devicebuffer.read((char*)refsig, 8);
    // get f2fs signature
    devicebuffer.seekg(1024);
    devicebuffer.read((char*)f2fsig, 4);
    // get zfs signature
    devicebuffer.seekg(135168);
    devicebuffer.read((char*)zfssig, 4);
    // get bcachefs signature
    devicebuffer.seekg(4120);
    devicebuffer.read((char*)bcfsig, 16);
    // get zonefs signature
    devicebuffer.seekg(0);
    devicebuffer.read((char*)zonsig, 4);
    //std::cout << "compare:" << bfsigstr.substr(0,4).compare("1SFB") << std::endl;
    //std::cout << "extsig1 array: " << std::hex << static_cast<int>((unsigned char)extsig[1]) << std::endl;
    //std::cout << "extsig1 " << std::hex << static_cast<int>((unsigned char)extsig1) << std::endl;
    if(extsig[0] == 0x53 && extsig[1] == 0xef) // EXT2,3,4 SIGNATURE == 0
    {
        *fstype = 0;
    }
    else if(winsig[0] == 0x55 && winsig[1] == 0xaa && bfsigstr.find("1SFB") == std::string::npos) // FAT NTFS, BFS SIGNATURE
    {
        char* exfatbuf = new char[5];
        char* fatbuf = new char[5];
        char* fat32buf = new char[5];
        devicebuffer.seekg(3);
        devicebuffer.read(exfatbuf, 5);
        std::string exfatstr(exfatbuf);
        delete[] exfatbuf;
        devicebuffer.seekg(54);
        devicebuffer.read(fatbuf, 5);
        std::string fatstr(fatbuf);
        delete[] fatbuf;
        devicebuffer.seekg(82);
        devicebuffer.read(fat32buf, 5);
        std::string fat32str(fat32buf);
        delete[] fat32buf;
        if(fatstr.find("FAT12") != std::string::npos)
            *fstype = 1;
        else if(fatstr.find("FAT16") != std::string::npos)
            *fstype = 2;
        else if(fat32str.find("FAT32") != std::string::npos)
            *fstype = 3;
        else if(exfatstr.find("EXFAT") != std::string::npos)
            *fstype = 4;
        else if(exfatstr.find("NTFS") != std::string::npos)
            *fstype = 5;
    }
    else if(apfsigstr.find("NXSB") != std::string::npos) // APFS
        *fstype = 6;
    else if(hfssigstr.find("H+") != std::string::npos) // HFS+
        *fstype = 7;
    else if(hfssigstr.find("HX") != std::string::npos) // HFSX
        *fstype = 8;
    else if(xfssigstr.find("XFSB") != std::string::npos) // XFS
        *fstype = 9;
    else if(btrsigstr.find("_BHRfS_M") != std::string::npos) // BTRFS
        *fstype = 10;
    else if(btlsigstr.find("-FVE-FS-") != std::string::npos) // BTILOCKER
        *fstype = 11;
    else if(bfsigstr.find("1SFB") != std::string::npos) // BFS
        *fstype = 12;
    else if(f2fsig[0] == 0x10 && f2fsig[1] == 0x20 && f2fsig[3] == 0xf5 && f2fsig[3] == 0xf2) // F2FS
        *fstype = 13;
    else if(isosigstr.find("CD001") != std::string::npos && udfsigstr.find("BEA01") == std::string::npos) // ISO9660
        *fstype = 14;
    else if(isosigstr.find("CD001") != std::string::npos && udfsigstr.find("BEA01") != std::string::npos) // UDF
        *fstype = 15;
    else if(hfssigstr.find("BD") != std::string::npos) // Legacy HFS
        *fstype = 16;
    else if(zfssig[0] == 0x0c && zfssig[1] == 0xb1 && zfssig[2] == 0xba && zfssig[3] == 0x00) // ZFS
        *fstype = 17;
    else if(refsig[0] == 0x00 && refsig[1] == 0x00 && refsig[2] == 0x00 && refsig[3] == 0x00 && refsig[4] == 0x53 && refsig[5] == 0x46 && refsig[6] == 0x65 && refsig[7] == 0x52) // ReFS
        *fstype = 18;
    else if(f2fsig[0] == 0xe2 && f2fsig[1] == 0xe1 && f2fsig[2] == 0x5e && f2fsig[3] == 0x0f) // EROFS
        *fstype = 19;
    else if(bcfsig[0] == 0xc6 && bcfsig[1] == 0x85 && bcfsig[2] == 0x73 && bcfsig[3] == 0xf6 && bcfsig[4] == 0x4e && bcfsig[5] == 0x1a && bcfsig[6] == 0x45 && bcfsig[7] == 0xca && bcfsig[8] == 0x82 && bcfsig[9] == 0x65 && bcfsig[10] == 0xf5 && bcfsig[11] == 0x7f && bcfsig[12] == 0x48 && bcfsig[13] == 0xba && bcfsig[14] == 0x6d && bcfsig[15] == 0x81) // BCACHEFS
        *fstype = 20;
    else if(zonsig[0] == 0x5a && zonsig[1] == 0x4f && zonsig[2] == 0x46 && zonsig[3] == 0x53) // ZONEFS
        *fstype = 21;
    else // UNKNOWN FILE SYSTEM SO FAR
        *fstype = 50; 
    devicebuffer.close();
    delete[] extsig;
    delete[] winsig;
    delete[] refsig;
    delete[] f2fsig;
    delete[] zfssig;
    delete[] bcfsig;
    delete[] zonsig;
}
 */ 

/*
FXString WombatForensics::ConvertBlocksToExtents(FXArray<uint> blocklist, uint blocksize, uint64_t rootdiroffset)
{
    // FirstSectorOfCluster = ((N-2) * sectorspercluster) + firstdatasector [rootdirstart];
    //QString rootdirlayout = QString::number(rootdiroffset + ((rootdircluster - 2) * sectorspercluster * bytespersector)) + "," + QString::number(sectorspercluster * bytespersector) + ";";
    FXString extentstring = "";
    int blkcnt = 1;
    uint startvalue = blocklist.at(0);
    for(int i=1; i < blocklist.no(); i++)
    {
        uint oldvalue = blocklist.at(i-1);
        uint newvalue = blocklist.at(i);
        if(newvalue - oldvalue == 1)
            blkcnt++;
        else
        {
            if(rootdiroffset > 0)
	    {
                //extentstring += QString::number(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + QString::number(blkcnt * blocksize) + ";";
                extentstring += FXString::value(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + FXString::value(blkcnt * blocksize) + ";";
	    }
            else
	    {
                extentstring += FXString::value(startvalue * blocksize) + "," + FXString::value(blkcnt * blocksize) + ";";
	    }
            startvalue = blocklist.at(i);
            blkcnt = 1;
        }
        if(i == blocklist.no() - 1)
        {
            if(rootdiroffset > 0)
	    {
                extentstring += FXString::value(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + FXString::value(blkcnt * blocksize) + ";";
	    }
            else
	    {
                extentstring += FXString::value(startvalue * blocksize) + "," + FXString::value(blkcnt * blocksize) + ";";
	    }
            startvalue = blocklist.at(i);
            blkcnt = 1;
        }
    }
    if(blocklist.no() == 1)
    {
        if(rootdiroffset > 0)
	{
            extentstring += FXString::value(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + FXString::value(blkcnt * blocksize) + ";";
	}
        else
	{
            extentstring += FXString::value(startvalue * blocksize) + "," + FXString::value(blkcnt * blocksize) + ";";
	}
    }
    return extentstring;
}
*/

void GetNextCluster(ForImg* curimg, uint32_t clusternum, uint8_t fstype, uint64_t fatoffset, std::vector<uint>* clusterlist)
{
    uint32_t curcluster32 = 0;
    uint16_t curcluster16 = 0;
    int fatbyte1 = 0;
    if((uint)fstype == 1) // FAT12
    {
        fatbyte1 = clusternum + (clusternum / 2);
        if(clusternum & 0x0001) // ODD
	{
	    ReadForImgContent(curimg, &curcluster16, fatoffset + fatbyte1);
	    curcluster16 = curcluster16 >> 4;
	}
        else // EVEN
	{
	    ReadForImgContent(curimg, &curcluster16, fatoffset + fatbyte1);
	    curcluster16 = curcluster16 & 0x0FFF;
	}
        if(curcluster16 < 0x0FF7 && curcluster16 >= 2)
        {
            clusterlist->push_back(curcluster16);
            GetNextCluster(curimg, curcluster16, fstype, fatoffset, clusterlist);
        }
    }
    else if((uint)fstype == 2) // FAT16
    {
        if(clusternum >= 2)
        {
            fatbyte1 = clusternum * 2;
	    ReadForImgContent(curimg, &curcluster16, fatoffset + fatbyte1);
	    
            if(curcluster16 < 0xFFF7 && curcluster16 >= 2)
            {
                clusterlist->push_back(curcluster16);
                GetNextCluster(curimg, curcluster16, fstype, fatoffset, clusterlist);
            }
        }
    }
    else if((uint)fstype == 3) // FAT32
    {
        if(clusternum >= 2)
        {
            fatbyte1 = clusternum * 4;
	    ReadForImgContent(curimg, &curcluster32, fatoffset + fatbyte1);
	    curcluster32 = curcluster32 & 0x0FFFFFFF;
	    if(curcluster32 < 0x0FFFFFF7 && curcluster32 >= 2)
            {
                clusterlist->push_back(curcluster32);
		GetNextCluster(curimg, curcluster32, fstype, fatoffset, clusterlist);
            }
        }
    }
    else if((uint)fstype == 4) // EXFAT
    {
        if(clusternum >= 2)
        {
            fatbyte1 = clusternum * 4;
	    ReadForImgContent(curimg, &curcluster32, fatoffset + fatbyte1);
	    if(curcluster32 < 0xFFFFFFF7 && curcluster32 >= 2)
            {
                clusterlist->push_back(curcluster32);
		GetNextCluster(curimg, curcluster32, fstype, fatoffset, clusterlist);
            }
        }
    }
}
