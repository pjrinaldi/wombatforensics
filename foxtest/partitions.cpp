#include "partitions.h"



void LoadPartitions(ForImg* curforimg, std::string* volname, uint64_t* partsize)
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
                //LoadGptPartitions(curforimg);
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
                *volname = GetFileSystemName(curforimg, 0);
                std::cout << *volname << std::endl;
                /*
                // table initialization
                tablelist->setTableSize(1, 14);
                tablelist->setColumnText(0, "");
                tablelist->setColumnText(1, "ID");
                tablelist->setColumnText(2, "Name");
                tablelist->setColumnText(3, "Path");
                tablelist->setColumnText(4, "Size (bytes)");
                tablelist->setColumnText(5, "Created");
                tablelist->setColumnText(6, "Accessed");
                tablelist->setColumnText(7, "Modified");
                tablelist->setColumnText(8, "Changed");
                tablelist->setColumnText(9, "Hash");
                tablelist->setColumnText(10, "Category");
                tablelist->setColumnText(11, "Signature");
                tablelist->setColumnText(12, "Tagged");
                tablelist->setColumnText(13, "Hash Match");

                // partition information
                itemtype = 2;
                tablelist->setItem(0, 0, new CheckTableItem(tablelist, NULL, NULL, ""));
                tablelist->setItemData(0, 1, &itemtype);
                tablelist->setItemText(0, 1, FXString::value(0));
                tablelist->setItemData(0, 2, curforimg);
                tablelist->setItemText(0, 2, GetFileSystemName(curforimg, 0));
                tablelist->setItemIcon(0, 2, partitionicon);
                tablelist->setItemIconPosition(0, 2, FXTableItem::BEFORE);
                tablelist->setItemText(0, 4, FXString(ReturnFormattingSize(curforimg->Size()).c_str()));

                // table formatting
                tablelist->fitColumnsToContents(0);
                tablelist->setColumnWidth(0, tablelist->getColumnWidth(0) + 25);
                FitColumnContents(1);
                FitColumnContents(2);
                FitColumnContents(4);
                AlignColumn(tablelist, 1, FXTableItem::LEFT);
                AlignColumn(tablelist, 2, FXTableItem::LEFT);
                */

                //std::cout << "parse fs here.." << std::endl;
                // parse partition(curofrimg, 0, curforimg->Size/512, 0, 1);
            }
            uint8_t ptreecnt = 0;
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
                }
                if((uint)curparttype == 0x05) // extended partition
                {
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
                    }
                }
                if( i == pcount - 1 && curoffset + cursize < curforimg->Size() / 512 - 1) // ADD UNALLOCATED AT END
                {
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
        //LoadGptPartitions(curforimg);
    }
    else // NO PARTITION MAP, JUST A FS AT ROOT OF IMAGE
    {
        *volname = GetFileSystemName(curforimg, 0);
        std::cout << *volname << std::endl;
        /*
        // table initialization
        tablelist->setTableSize(1, 14);
        tablelist->setColumnText(0, "");
        tablelist->setColumnText(1, "ID");
        tablelist->setColumnText(2, "Name");
        tablelist->setColumnText(3, "Path");
        tablelist->setColumnText(4, "Size (bytes)");
        tablelist->setColumnText(5, "Created");
        tablelist->setColumnText(6, "Accessed");
        tablelist->setColumnText(7, "Modified");
        tablelist->setColumnText(8, "Changed");
        tablelist->setColumnText(9, "Hash");
        tablelist->setColumnText(10, "Category");
        tablelist->setColumnText(11, "Signature");
        tablelist->setColumnText(12, "Tagged");
        tablelist->setColumnText(13, "Hash Match");

        // partition information
        itemtype = 2;
        tablelist->setItem(0, 0, new CheckTableItem(tablelist, NULL, NULL, ""));
        tablelist->setItemData(0, 1, &itemtype);
        tablelist->setItemText(0, 1, FXString::value(0));
        tablelist->setItemData(0, 2, curforimg);
        tablelist->setItemText(0, 2, GetFileSystemName(curforimg, 0));
        tablelist->setItemIcon(0, 2, partitionicon);
        tablelist->setItemIconPosition(0, 2, FXTableItem::BEFORE);
        tablelist->setItemText(0, 4, FXString(ReturnFormattingSize(curforimg->Size()).c_str()));

        // ParsePartition(curforimg, 0, curimg->Size()/512, 0 (part id), 1 (allocated));
        // table formatting
        tablelist->fitColumnsToContents(0);
        tablelist->setColumnWidth(0, tablelist->getColumnWidth(0) + 25);
        FitColumnContents(1);
        FitColumnContents(2);
        FitColumnContents(4);
        AlignColumn(tablelist, 1, FXTableItem::LEFT);
        AlignColumn(tablelist, 2, FXTableItem::LEFT);
        */
    }
}

/*
void WombatForensics::LoadGptPartitions(ForImg* curforimg)
{
    uint64_t parttablestart = 0;
    uint32_t partentrycount = 0;
    uint32_t partentrysize = 0;
    ReadForImgContent(curforimg, &parttablestart, 584);
    ReadForImgContent(curforimg, &partentrycount, 592);
    ReadForImgContent(curforimg, &partentrysize, 596);
    int ptreecnt = 0;
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
    tablelist->setTableSize(pcount * 2 + 1, 14);
    tablelist->setColumnText(0, "");
    tablelist->setColumnText(1, "ID");
    tablelist->setColumnText(2, "Name");
    tablelist->setColumnText(3, "Path");
    tablelist->setColumnText(4, "Size (bytes)");
    tablelist->setColumnText(5, "Created");
    tablelist->setColumnText(6, "Accessed");
    tablelist->setColumnText(7, "Modified");
    tablelist->setColumnText(8, "Changed");
    tablelist->setColumnText(9, "Hash");
    tablelist->setColumnText(10, "Category");
    tablelist->setColumnText(11, "Signature");
    tablelist->setColumnText(12, "Tagged");
    tablelist->setColumnText(13, "Hash Match");
    for(int i=0; i < pcount; i++)
    {
        itemtype = 2;
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
                tablelist->setItem(ptreecnt, 0, new CheckTableItem(tablelist, NULL, NULL, ""));
                tablelist->setItemData(ptreecnt, 1, &itemtype);
                tablelist->setItemText(ptreecnt, 1, FXString::value(ptreecnt));
                tablelist->setItemText(ptreecnt, 2, "UNALLOCATED");
                tablelist->setItemData(ptreecnt, 2, curforimg);
                tablelist->setItemIcon(ptreecnt, 2, partitionicon);
                tablelist->setItemIconPosition(ptreecnt, 2, FXTableItem::BEFORE);

                tablelist->setItemText(ptreecnt, 4, FXString(ReturnFormattingSize(curstartsector*512).c_str()));
                // LOAD UNALLOCATED PARTITON sectorcheck, curstartsector, 0
                ptreecnt++;
            }
            // LOAD ALLOCATED PARTITION READ FROM TABLE curstartsector, (curendsector - curstartsector + 1), 1)
            tablelist->setItem(ptreecnt, 0, new CheckTableItem(tablelist, NULL, NULL, ""));
            tablelist->setItemData(ptreecnt, 1, &itemtype);
            tablelist->setItemText(ptreecnt, 1, FXString::value(ptreecnt));
            tablelist->setItemText(ptreecnt, 2, GetFileSystemName(curforimg, curstartsector*512));
            tablelist->setItemData(ptreecnt, 2, curforimg);
            tablelist->setItemIcon(ptreecnt, 2, partitionicon);
            tablelist->setItemIconPosition(ptreecnt, 2, FXTableItem::BEFORE);
            tablelist->setItemText(ptreecnt, 4, FXString(ReturnFormattingSize((curendsector - curstartsector + 1)*512).c_str()));
            ptreecnt++;
            if(i == pcount - 1) // ADD UNALLOCATED AFTER LAST VALID PARTITION IF EXISTS
            {
                if(curendsector < curforimg->Size() / 512)
                {
                    // LOAD UNALLOCATED curendsector+1, curforimg->Size() / 512 - 1 - curendsector, 0)
                    tablelist->setItem(ptreecnt, 0, new CheckTableItem(tablelist, NULL, NULL, ""));
                    tablelist->setItemData(ptreecnt, 1, &itemtype);
                    tablelist->setItemText(ptreecnt, 1, FXString::value(ptreecnt));
                    tablelist->setItemText(ptreecnt, 2, "UNALLOCATED");
                    tablelist->setItemData(ptreecnt, 2, curforimg);
                    tablelist->setItemIcon(ptreecnt, 2, partitionicon);
                    tablelist->setItemIconPosition(ptreecnt, 2, FXTableItem::BEFORE);
                    tablelist->setItemText(ptreecnt, 4, FXString(ReturnFormattingSize((curforimg->Size() / 512 - 1 - curendsector)*512).c_str()));
                }
            }
        }
    }
    tablelist->fitColumnsToContents(0);
    tablelist->setColumnWidth(0, tablelist->getColumnWidth(0) + 25);
    FitColumnContents(1);
    FitColumnContents(2);
    FitColumnContents(4);
    AlignColumn(tablelist, 1, FXTableItem::LEFT);
    AlignColumn(tablelist, 2, FXTableItem::LEFT);
}
*/

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
        else if(std::string(fattype).find("NTFS") > -1)
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
            //curcluster = (qFromLittleEndian<uint16_t>(curimg->ReadContent(fatoffset + fatbyte1, 2)) >> 4);
        else // EVEN
	{
	    ReadForImgContent(curimg, &curcluster16, fatoffset + fatbyte1);
	    curcluster16 = curcluster16 & 0x0FFF;
	}
            //curcluster = (qFromLittleEndian<uint16_t>(curimg->ReadContent(fatoffset + fatbyte1, 2)) & 0x0FFF);
        //if(curcluster != 0x0FF7)
        //    clusterlist->append(curcluster);
        //if(curcluster < 0x0FF8 && curcluster >= 2)
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
	    
            //curcluster = qFromLittleEndian<uint16_t>(curimg->ReadContent(fatoffset + fatbyte1, 2));
            //if(curcluster != 0xFFF7)
            //    clusterlist->append(curcluster);
            //if(curcluster < 0xFFF8 && curcluster >= 2)
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
            //curcluster = (qFromLittleEndian<uint32_t>(curimg->ReadContent(fatoffset + fatbyte1, 4)) & 0x0FFFFFFF);
	    //if(curcluster != 0x0FFFFFF7)
		//clusterlist->append(curcluster);
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
            //curcluster = qFromLittleEndian<uint32_t>(curimg->ReadContent(fatoffset + fatbyte1, 4));
	    //if(curcluster != 0xFFFFFFF7)
		//clusterlist->append(curcluster);
	    if(curcluster32 < 0xFFFFFFF7 && curcluster32 >= 2)
            {
                clusterlist->push_back(curcluster32);
		GetNextCluster(curimg, curcluster32, fstype, fatoffset, clusterlist);
            }
        }
    }
}





/*
std::string ConvertUnixTimeToHuman(uint32_t unixtime)
{
    time_t timet = (time_t)unixtime;
    struct tm* tmtime = gmtime(&timet);
    char hchar[100];
    strftime(hchar, 100, "%m/%d/%Y %I:%M:%S %p UTC", tmtime);
    
    return std::string(hchar);
}

uint8_t* substr(uint8_t* arr, int begin, int len)
{
    uint8_t* res = new uint8_t[len + 1];
    for (int i = 0; i < len; i++)
        res[i] = *(arr + begin + i);
    res[len] = 0;
    return res;
}

char* GetDateTime(char *buff)
{
    time_t t = time(0);
    strftime(buff, DTTMSZ, DTTMFMT, localtime(&t));
    return buff;
};

std::string ReturnFormattingSize(uint size)
{
    std::stringstream ss;
    ss.imbue(std::locale(""));
    ss << std::fixed << size;
    return ss.str();
}

void ReadContent(std::ifstream* rawcontent, uint8_t* tmpbuf, uint64_t offset, uint64_t size)
{
    rawcontent->seekg(offset);
    rawcontent->read((char*)tmpbuf, size);
}

void ReadContent(std::ifstream* rawcontent, int8_t* tmpbuf, uint64_t offset, uint64_t size)
{
    rawcontent->seekg(offset);
    rawcontent->read((char*)tmpbuf, size);
}

void ReadContent(std::ifstream* rawcontent, uint16_t* val, uint64_t offset)
{
    uint8_t* tmp8 = new uint8_t[2];
    rawcontent->seekg(offset);
    rawcontent->read((char*)tmp8, 2);
    ReturnUint16(val, tmp8);
    delete[] tmp8;
}

void ReadContent(std::ifstream* rawcontent, uint32_t* val, uint64_t offset)
{
    uint8_t* tmp8 = new uint8_t[4];
    rawcontent->seekg(offset);
    rawcontent->read((char*)tmp8, 4);
    ReturnUint32(val, tmp8);
    delete[] tmp8;
}

void ReadContent(std::ifstream* rawcontent, uint64_t* val, uint64_t offset)
{
    uint8_t* tmp8 = new uint8_t[8];
    rawcontent->seekg(offset);
    rawcontent->read((char*)tmp8, 8);
    ReturnUint64(val, tmp8);
    delete[] tmp8;
}

void ReadForImgContent(ForImg* forimg, uint16_t* val, uint64_t offset, bool isbigendian)
{
    uint8_t* tmp8 = new uint8_t[2];
    forimg->ReadContent(tmp8, offset, 2);
    ReturnUint16(val, tmp8, isbigendian);
    delete[] tmp8;
}

void ReadForImgContent(ForImg* forimg, uint32_t* val, uint64_t offset, bool isbigendian)
{
    uint8_t* tmp8 = new uint8_t[4];
    forimg->ReadContent(tmp8, offset, 4);
    ReturnUint32(val, tmp8, isbigendian);
    delete[] tmp8;
}

void ReadForImgContent(ForImg* forimg, uint64_t* val, uint64_t offset, bool isbigendian)
{
    uint8_t* tmp8 = new uint8_t[8];
    forimg->ReadContent(tmp8, offset, 8);
    ReturnUint64(val, tmp8, isbigendian);
    delete[] tmp8;
}


void ReadInteger(uint8_t* arr, int begin, uint16_t* val, bool isbigendian)
{
    uint8_t* tmp8 = new uint8_t[2];
    tmp8 = substr(arr, begin, 2);
    ReturnUint16(val, tmp8, isbigendian);
    delete[] tmp8;
}

void ReadInteger(uint8_t* arr, int begin, uint32_t* val, bool isbigendian)
{
    uint8_t* tmp8 = new uint8_t[4];
    tmp8 = substr(arr, begin, 4);
    ReturnUint32(val, tmp8, isbigendian);
    delete[] tmp8;
}
void ReadInteger(uint8_t* arr, int begin, uint64_t* val, bool isbigendian)
{
    uint8_t* tmp8 = new uint8_t[8];
    tmp8 = substr(arr, begin, 8);
    ReturnUint64(val, tmp8, isbigendian);
    delete[] tmp8;
}
void ReturnUint32(uint32_t* tmp32, uint8_t* tmp8, bool isbigendian)
{
    if(isbigendian)
        *tmp32 = __builtin_bswap32((uint32_t)tmp8[0] | (uint32_t)tmp8[1] << 8 | (uint32_t)tmp8[2] << 16 | (uint32_t)tmp8[3] << 24);
    else
        *tmp32 = (uint32_t)tmp8[0] | (uint32_t)tmp8[1] << 8 | (uint32_t)tmp8[2] << 16 | (uint32_t)tmp8[3] << 24;
}

void ReturnUint16(uint16_t* tmp16, uint8_t* tmp8, bool isbigendian)
{
    if(isbigendian)
        *tmp16 = __builtin_bswap16((uint16_t)tmp8[0] | (uint16_t)tmp8[1] << 8);
    else
        *tmp16 = (uint16_t)tmp8[0] | (uint16_t)tmp8[1] << 8;
}

void ReturnUint64(uint64_t* tmp64, uint8_t* tmp8, bool isbigendian)
{
    if(isbigendian)
        *tmp64 = __builtin_bswap64((uint64_t)tmp8[0] | (uint64_t)tmp8[1] << 8 | (uint64_t)tmp8[2] << 16 | (uint64_t)tmp8[3] << 24 | (uint64_t)tmp8[4] << 32 | (uint64_t)tmp8[5] << 40 | (uint64_t)tmp8[6] << 48 | (uint64_t)tmp8[7] << 56);
    else
        *tmp64 = (uint64_t)tmp8[0] | (uint64_t)tmp8[1] << 8 | (uint64_t)tmp8[2] << 16 | (uint64_t)tmp8[3] << 24 | (uint64_t)tmp8[4] << 32 | (uint64_t)tmp8[5] << 40 | (uint64_t)tmp8[6] << 48 | (uint64_t)tmp8[7] << 56;
}

void ReturnUint(uint64_t* tmp, uint8_t* tmp8, unsigned int length)
{
    for(unsigned int i=0; i < length; i++)
        *tmp |= (uint64_t)tmp8[i] << i * 8;
}

void ReturnInt(int64_t* tmp, int8_t* tmp8, unsigned int length)
{
    for(unsigned int i=0; i < length; i++)
        *tmp |= (int64_t)tmp8[i] << i * 8;
}
*/
