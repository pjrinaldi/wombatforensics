#include "directories.h"


void LoadDirectory(CurrentItem* currentitem)
{
    //std::cout << "currentitem text: " << currentitem->itemtext << std::endl;
    //std::cout << "currentitem voloffset: " << currentitem->voloffset << std::endl;
    //std::cout << "currentitem inode: " << currentitem->inode << std::endl;
    uint16_t sig16 = 0;
    uint32_t sig32 = 0;
    uint64_t sig64 = 0;

    ForImg* curforimg = currentitem->forimg;
    uint64_t curinode = currentitem->inode;
    uint64_t offset = currentitem->voloffset;
    std::string partitionname = currentitem->itemtext;

    ReadForImgContent(curforimg, &sig32, offset + 544); // BFS1
    ReadForImgContent(curforimg, &sig16, offset + 510); // FAT, NTFS, or BFS
    if(sig32 == 0x42465331) // BFS1 - BeFS
    {
        /*
        char* bfsname = new char[33];
        curforimg->ReadContent((uint8_t*)bfsname, offset + 512, 32);
        bfsname[32] = 0;
        partitionname = std::string(bfsname) + " [BFS]";
        */
    }
    if(sig16 == 0xaa55) // FAT12, FAT16, FAT32, EXFAT, or NTFS
    {
        char* fattype = new char[6];
        curforimg->ReadContent((uint8_t*)fattype, offset + 3, 5);
        fattype[5] = 0;
        if(strcmp(fattype, "EXFAT") == 0)
        {
            /*
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
            */
	    //partitionname += " [EXFAT]";
	    //std::cout << "part name: " << partitionname.text() << std::endl;
        }
        else if(std::string(fattype).find("NTFS") != std::string::npos)
        {
            /*
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
            */
        }
        else
        {
	    //curinode = ParseFatDirectory(curimg, curstartsector, ptreecnt, 0, "", "");
	    //curinode = AddVirtualFileSystemFiles(curimg, ptreecnt, fatcount, fatsize * bytespersector, curinode);
            char* pname = new char[12];
            curforimg->ReadContent((uint8_t*)fattype, offset + 54, 5);
            if(strcmp(fattype, "FAT12") == 0)
            {
                LoadFatDirectory(currentitem, 0);
            }
            else if(strcmp(fattype, "FAT16") == 0)
            {
                LoadFatDirectory(currentitem, 0);
            }
            else
            {
                /*
                curforimg->ReadContent((uint8_t*)fattype, offset + 82, 5);
                fattype[5] = 0;
                if(strcmp(fattype, "FAT32") == 0)
                {
                    curforimg->ReadContent((uint8_t*)pname, offset + 71, 11);
                    pname[11] = 0;
                    partitionname = std::string(pname) + " [FAT32]";
                }
                */
            }
        }
    }
    ReadForImgContent(curforimg, &sig16, offset + 1080); // EXT2/3/4
    if(sig16 == 0xef53) // EXT2/3/4
    {
        /*
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
        */
    }
    char* apfssig = new char[5];
    curforimg->ReadContent((uint8_t*)apfssig, offset + 32, 4);
    apfssig[4] = 0;
    if(strcmp(apfssig, "NXSB") == 0) // APFS
    {
	//partitionname = "APFS Container [APFS]";
    }
    char* hfssig = new char[3];
    curforimg->ReadContent((uint8_t*)hfssig, offset + 1024, 2);
    hfssig[2] = 0;
    if(strcmp(hfssig, "H+") == 0 || strcmp(hfssig, "HX") == 0) // HFS+/X
    {
        /*
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
        */
    }
    else if(strcmp(hfssig, "BD") == 0) // LEGACY HFS
    {
        /*
	uint8_t volnamelength = 0;
	curforimg->ReadContent(&volnamelength, offset + 1060, 1);
	char* volname = new char[(uint)volnamelength + 1];
	curforimg->ReadContent((uint8_t*)volname, offset + 1061, (uint)volnamelength);
	volname[(uint)volnamelength] = 0;
	partitionname = std::string(volname) + " [HFS]";
        */
    }
    char* xfssig = new char[5];
    curforimg->ReadContent((uint8_t*)xfssig, offset, 4);
    xfssig[4] = 0;
    if(strcmp(xfssig, "XFSB") == 0)
    {
        /*
	char* volname = new char[13];
	curforimg->ReadContent((uint8_t*)volname, offset + 108, 12);
	volname[12] = 0;
	partitionname = std::string(volname) + " [XFS]";
        */
    }
    char* btrsig = new char[9];
    curforimg->ReadContent((uint8_t*)btrsig, offset + 65600, 8);
    btrsig[8] = 0;
    if(strcmp(btrsig, "_BHRfS_M") == 0)
    {
        /*
	char* volname = new char[101];
	curforimg->ReadContent((uint8_t*)volname, offset + 65536 + 0x12b, 100);
	volname[100] = 0;
	partitionname = std::string(volname) + " [BTRFS]";
        */
    }
    char* bitlocksig = new char[9];
    curforimg->ReadContent((uint8_t*)bitlocksig, offset, 8);
    bitlocksig[8] = 0;
    if(strcmp(bitlocksig, "-FVE-FS-") == 0)
    {
        /*
	uint16_t vollength = 0;
	ReadForImgContent(curforimg, &vollength, offset + 112);
	vollength = vollength - 8;
	char* volname = new char[vollength + 1];
	curforimg->ReadContent((uint8_t*)volname, offset + 118, vollength);
	volname[vollength] = 0;
	partitionname = std::string(volname) + " [BITLOCKER]";
        */
    }
    ReadForImgContent(curforimg, &sig32, offset + 1024);
    if(sig32 == 0xf2f52010) // F2FS
    {
	//partitionname = " [F2FS]";
    }
    else if(sig32 == 0xe0f5e1e2) // EROFS
    {
	//partitionname = " [EROFS]";
    }
    ReadForImgContent(curforimg, &sig32, offset);
    if(sig32 == 0x5a4f4653)
    {
	//partitionname = " [ZONEFS]";
    }
    char* isosig = new char[6];
    char* udfsig = new char[6];
    curforimg->ReadContent((uint8_t*)isosig, offset + 32769, 5);
    isosig[5] = 0;
    curforimg->ReadContent((uint8_t*)udfsig, offset + 40961, 5);
    udfsig[5] = 0;
    if(strcmp(isosig, "CD001") == 0 && strcmp(udfsig, "BEA01") != 0) // ISO9660
    {
        /*
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
        */
    }
    else if(strcmp(isosig, "CD001") == 0 && strcmp(udfsig, "BEA01") == 0) // UDF
    {
	//partitionname = " [UDF]";
    }
    ReadForImgContent(curforimg, &sig64, offset + 135168);
    if(sig64 == 0x00bab10c) // ZFS
    {
	//partitionname = " [ZFS]";
    }
    ReadForImgContent(curforimg, &sig64, offset + 3);
    if(sig64 == 0x5265465300000000) // ReFS
    {
	//partitionname = " [REFS]";
    }
    ReadForImgContent(curforimg, &sig64, offset + 4120, true);
    uint64_t bcfsig = 0;
    ReadForImgContent(curforimg, &bcfsig, offset + 4128, true);
    if(sig64 == 0xc68573f64e1a45ca && bcfsig == 0x8265f57f48ba6d81) // BCACHEFS
    {
	//partitionname = " [BCACHEFS]";
    }
    
    //return partitionname;
}

void LoadFatDirectory(CurrentItem* currentitem, uint64_t curinode)
{
    // BYTES PER SECTOR
    uint16_t bytespersector = 0;
    ReadForImgContent(currentitem->forimg, &bytespersector, currentitem->voloffset + 11);
    std::cout << "bytes per sector: " << bytespersector << std::endl;
    // FAT COUNT
    uint8_t fatcount = 0;
    currentitem->forimg->ReadContent(&fatcount, currentitem->voloffset + 16, 1);
    std::cout << "fat count: " << (uint)fatcount << std::endl;
    // SECTORS PER CLUSTER
    uint8_t sectorspercluster = 0;
    currentitem->forimg->ReadContent(&sectorspercluster, currentitem->voloffset + 13, 1);
    std::cout << "sectors per cluster: " << (uint) sectorspercluster << std::endl;
    // RESERVED AREA SIZE
    uint16_t reservedareasize = 0;
    ReadForImgContent(currentitem->forimg, &reservedareasize, currentitem->voloffset + 14);
    std::cout << "reserved area size: " << reservedareasize << std::endl;
    // ROOT DIRECTORY MAX FILES
    uint16_t rootdirmaxfiles = 0;
    ReadForImgContent(currentitem->forimg, &rootdirmaxfiles, currentitem->voloffset + 17);
    std::cout << "root dir max files: " << rootdirmaxfiles << std::endl;
    if(currentitem->itemtext.find("[FAT12]") != std::string::npos || currentitem->itemtext.find("[FAT16]") != std::string::npos)
    {
        // FAT SIZE
        uint16_t fatsize = 0;
        ReadForImgContent(currentitem->forimg, &fatsize, currentitem->voloffset + 22);
        std::cout << "fat size: " << fatsize << std::endl;
        // CLUSTER AREA START
        uint64_t clusterareastart = reservedareasize + fatcount * fatsize + ((rootdirmaxfiles * 32) + (bytespersector - 1)) / bytespersector;
        std::cout << "Cluster area start: " << clusterareastart << std::endl;
        // DIRECTORY OFFSET
        uint64_t diroffset = (reservedareasize + fatcount * fatsize) * bytespersector;
        std::cout << "dir offset: " << diroffset << std::endl;
        // DIRECTORY SIZE
        uint64_t dirsize = rootdirmaxfiles * 32 + bytespersector - 1;
        std::cout << "dir size: " << dirsize << std::endl;
        if(curinode == 0) // root directory
        {
            uint direntrycount = dirsize / 32;
            std::cout << "dir entry count: " << direntrycount << std::endl;
	    // PARSE DIRECTORY ENTRIES
            std::string longnamestring = "";
            for(uint i=0; i < direntrycount; i++)
            {
                //std::string filename = "";
                //uint32_t clusternum = 0;
                //uint64_t physicalsize = 0;
	        // FIRST CHARACTER
                uint8_t firstchar = 0;
                currentitem->forimg->ReadContent(&firstchar, diroffset + i*32, 1);
                if((uint)firstchar == 0x00) // entry is free and all remaining are free
                    break;
                else
                {
                    std::cout << "first char: " << std::hex << (uint)firstchar << std::endl;
                }
            }
        }
    }
}
/*
	for(unsigned int j=0; j < direntrycount; j++)
	{
            if(firstchar == 0xe5) // deleted entry, skip
            {
            }
            else if(firstchar == 0x00) // entry is free and all remaining are free
                break;
	    else if(firstchar == 0x85) // EXFAT File/Dir Directory Entry
	    {
		uint8_t* sc = new uint8_t[1];
		uint8_t secondarycount = 0;
		ReadContent(rawcontent, sc, diroffset + j*32 + 1, 1);
		secondarycount = (uint8_t)sc[0];
		delete[] sc;
		//std::cout << "secondary count: " << secondarycount << std::endl;
		// FILE ATTRIBUTE
		uint8_t* fa = new uint8_t[1];
		uint8_t fileattr = 0;
		ReadContent(rawcontent, fa, diroffset + j*32 + 4, 1);
		fileattr = (uint8_t)fa[0];
		delete[] fa;
		//std::cout << "file attr: " << (int)fileattr << std::endl;
		if(fileattr & 0x10) // Sub Directory
		{
		    int fatchain = 0;
		    uint8_t namelength = 0;
		    uint8_t curlength = 0;
		    for(uint8_t k=1; k <= secondarycount; k++)
		    {
			uint8_t* set = new uint8_t[1];
			uint8_t subentrytype = 0;
			ReadContent(rawcontent, set, diroffset + j*32 + k*32, 1);
			subentrytype = (uint8_t)set[0];
			delete[] set;
			//std::cout << "Sub entry type: 0x" << std::hex << (int)subentrytype << std::endl;
			if(subentrytype == 0xc0) // Stream Extension Directory Entry
			{
			    // NAME LENGTH
			    uint8_t* nl = new uint8_t[1];
			    ReadContent(rawcontent, nl, diroffset + (j+k)*32 + 3, 1);
			    namelength = (uint8_t)nl[0];
			    delete[] nl;
			    // FLAGS
			    uint8_t* ff = new uint8_t[1];
			    ReadContent(rawcontent, ff, diroffset + (j+k)*32 + 1, 1);
			    std::bitset<8> flagbits{(uint8_t)ff[0]};
			    delete[] ff;
			    //std::cout << "flagbits: " << flagbits << std::endl;
			    // FAT CHAIN
			    int fatchain = flagbits[1];
			    //std::cout << "fatchain: " << fatchain << std::endl;
			    // CLUSTER NUM
			    uint8_t* cn = new uint8_t[4];
			    //uint32_t clusternum = 0;
			    ReadContent(rawcontent, cn, diroffset + (j+k)*32 + 20, 4);
			    ReturnUint32(&clusternum, cn);
			    delete[] cn;
			    //std::cout << "Cluster Number: " << clusternum << std::endl;
			    // PHYSICAL SIZE
			    uint8_t* ps = new uint8_t[8];
			    //uint64_t physicalsize = 0;
			    ReadContent(rawcontent, ps, diroffset + (j+k)*32 + 24, 8);
			    ReturnUint64(&physicalsize, ps);
			    delete[] ps;
			    //std::cout << "Physical Size: " << physicalsize << std::endl;
			}
			else if(subentrytype == 0xc1) // File Name Directory Entry
			{
			    // GET FILE NAME
			    curlength += 15;
			    if(curlength <= namelength)
			    {
				for(int m=1; m < 16; m++)
				{
				    uint8_t* sl = new uint8_t[2];
				    uint16_t singleletter = 0;
				    ReadContent(rawcontent, sl, diroffset + (j+k)*32 + m*2, 2);
				    ReturnUint16(&singleletter, sl);
				    delete[] sl;
				    filename += (char)singleletter;
				}
			    }
			    else
			    {
				int remaining = namelength + 16 - curlength;
				for(int m=1; m < remaining; m++)
				{
				    uint8_t* sl = new uint8_t[2];
				    uint16_t singleletter = 0;
				    ReadContent(rawcontent, sl, diroffset + (j+k)*32 + m*2, 2);
				    ReturnUint16(&singleletter, sl);
				    delete[] sl;
				    filename += (char)singleletter;
				}
			    }
			}
		    }
		    if(filename.find(childpath) != std::string::npos)
		    {
			std::string layout = "";
			//std::cout << "cur file name: " << filename << std::endl;
			// GET DIRECTORY LAYOUT
			if(fatchain == 0 && clusternum > 1)
			{
			    std::vector<uint32_t> clusterlist;
			    clusterlist.clear();
			    clusterlist.push_back(clusternum);
			    GetNextCluster(rawcontent, clusternum, curfat, &clusterlist);
			    layout = ConvertClustersToExtents(&clusterlist, curfat);
			    clusterlist.clear();
			}
			else if(fatchain == 1)
			{
			    uint clustercout = (uint)ceil((float)physicalsize / (curfat->bytespersector * curfat->sectorspercluster));
			    layout = std::to_string(curfat->clusterareastart * curfat->bytespersector + ((clusternum - 2) * curfat->bytespersector * curfat->sectorspercluster)) + "," + std::to_string(clustercout * curfat->bytespersector * curfat->sectorspercluster) + ";";
			}
			
			return layout;
		    }
		}
	    }
            else
            {
                uint8_t* fa = new uint8_t[1];
                uint8_t fileattr = 0;
                ReadContent(rawcontent, fa, diroffset + j*32 + 11, 1);
                fileattr = (uint8_t)fa[0];
                delete[] fa;
                if(fileattr == 0x0f || fileattr == 0x3f) // Long Directory Name
                {
                    unsigned int lsn = ((int)firstchar & 0x0f);
                    if(lsn <= 20)
                    {
                        // process long file name part here... then add to the long file name...
                        std::string longname = "";
                        int arr[13] = {1, 3, 5, 7, 9, 14, 16, 18, 20, 22, 24, 28, 30};
                        for(int k=0; k < 13; k++)
                        {
                            uint16_t longletter = 0;
                            uint8_t* ll = new uint8_t[3];
                            ReadContent(rawcontent, ll, diroffset + j*32 + arr[k], 2);
                            ReturnUint16(&longletter, ll);
                            delete[] ll;
                            if(longletter < 0xFFFF)
                            {
                                longname += (char)longletter;
                            }
                        }
                        longnamestring.insert(0, longname);
                    }
                }
		else
		{
		    if(fileattr == 0x10)
		    {
			//std::cout << "long name: " << longnamestring << "|" << std::endl;
			if(longnamestring.find(childpath) != std::string::npos)
			{
			    uint8_t* hcn = new uint8_t[2];
			    uint16_t hiclusternum = 0;
			    ReadContent(rawcontent, hcn, diroffset + j*32 + 20, 2); // always zero for fat12/16
			    ReturnUint16(&hiclusternum, hcn);
			    delete[] hcn;
			    uint8_t* lcn = new uint8_t[2];
			    uint16_t loclusternum = 0;
			    ReadContent(rawcontent, lcn, diroffset + j*32 + 26, 2);
			    ReturnUint16(&loclusternum, lcn);
			    delete[] lcn;
			    uint32_t clusternum = ((uint32_t)hiclusternum >> 16) + loclusternum;
			    std::vector<uint32_t> clusterlist;
			    clusterlist.clear();
			    //std::cout << "first cluster: " << clusternum << std::endl;
			    if(clusternum >= 2)
			    {
				clusterlist.push_back(clusternum);
				GetNextCluster(rawcontent, clusternum, curfat, &clusterlist);
			    }
			    std::string layout = "";
			    if(clusterlist.size() > 0)
			    {
				layout = ConvertClustersToExtents(&clusterlist, curfat);
			    }
			    clusterlist.clear();
			    //std::cout << "layout: " << layout << std::endl;
			    return layout;
			}
		    }
		    longnamestring = "";
		}
            }
	}
*/ 

/*
 qulonglong ParseFatDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt, qulonglong parinode, QString parfilename, QString dirlayout)
{
    qulonglong inodecnt = 0;
    //uint32_t fatsize = 0;
    qulonglong fatoffset = 0;
    uint16_t bytespersector = 0;
    uint8_t sectorspercluster = 0;
    uint8_t fstype = 0;
    qulonglong clusterareastart = 0;
    QString rootdirlayout = "";
    QFile propfile(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/prop");
    if(!propfile.isOpen())
	propfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(propfile.isOpen())
    {
        while(!propfile.atEnd())
        {
            QString line = propfile.readLine();
            if(line.startsWith("Bytes Per Sector|"))
                bytespersector = line.split("|").at(1).toUInt();
            else if(line.startsWith("Sectors Per Cluster|"))
                sectorspercluster = line.split("|").at(1).toUInt();
            else if(line.startsWith("Root Directory Layout|"))
                rootdirlayout = line.split("|").at(1);
            else if(line.startsWith("FAT Offset|"))
                fatoffset = line.split("|").at(1).toULongLong();
            //else if(line.startsWith("FAT Size|"))
            //    fatsize = line.split("|").at(1).toUInt();
            else if(line.startsWith("Cluster Area Start|"))
                clusterareastart = line.split("|").at(1).toULongLong();
	    else if(line.startsWith("File System Type Int|"))
		fstype = line.split("|").at(1).toUInt();
        }
        propfile.close();
    }
    if(!dirlayout.isEmpty())
	rootdirlayout = dirlayout;
    //qDebug() << "rootdirlayout:" << rootdirlayout;
    //qDebug() << "bps:" << bytespersector << "fo:" << fatoffset << "fs:" << fatsize << "rdl:" << rootdirlayout;
    for(int i=0; i < rootdirlayout.split(";", Qt::SkipEmptyParts).count(); i++)
    {
        //qDebug() << "root dir entry:" << i;
	qulonglong rootdiroffset = 0;
	qulonglong rootdirsize = 0;
	if(i == 0)
	{
	    if(dirlayout.isEmpty()) // root directory
	    {
	        rootdiroffset = rootdirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong();
		rootdirsize = rootdirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong();
	    }
	    else // sub directory
	    {
		rootdiroffset = rootdirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(0).toULongLong() + 64; // skip . and .. directories
		rootdirsize = rootdirlayout.split(";", Qt::SkipEmptyParts).at(i).split(",").at(1).toULongLong() - 64; // adjust read size for the 64 byte skip
	    }
	}
        uint rootdirentrycount = rootdirsize / 32;
        //qDebug() << "current rootdirentrycount:" << rootdirentrycount;
	if(parinode == 0)
	    inodecnt = 0;
	else
	    inodecnt = parinode + 1;
	//qDebug() << "inodecnt at start of parent comparison:" << inodecnt;
        QString longnamestring = "";
        for(uint j=0; j < rootdirentrycount; j++)
        {
	    QString longname = "";
            QTextStream out;
            QFile fileprop(curimg->MountPath() + "/p" + QString::number(ptreecnt) + "/f" + QString::number(inodecnt) + ".prop");
            if(!fileprop.isOpen())
                fileprop.open(QIODevice::Append | QIODevice::Text);
            out.setDevice(&fileprop);

            uint8_t firstchar = qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32, 1));
            if(firstchar == 0x00) // entry is free and all remaining are free
                break;
            uint8_t fileattr = qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 11, 1));
            //qDebug() << "firstchar:" << QString::number(firstchar, 16) << "fileattr:" << QString::number(fileattr, 16);
            if(fileattr != 0x0f && fileattr != 0x00 && fileattr != 0x3f) // need to process differently // 0x3f is ATTR_LONG_NAME_MASK which is a long name entry sub component
            {
                if(!longnamestring.isEmpty())
                {
                    out << "Long Name|" << longnamestring << "|Long name for the file." << Qt::endl;
		    longname = longnamestring;
                    longnamestring = "";
                }
                else
                    out << "Long Name| |Long name for the file." << Qt::endl;
                out << "File Attributes|";
                if(fileattr & 0x01)
                    out << "Read Only,";
                if(fileattr & 0x02)
                    out << "Hidden File,";
                if(fileattr & 0x04)
                    out << "System File,";
                if(fileattr & 0x08)
                    out << "Volume ID,";
                if(fileattr & 0x10)
                    out << "SubDirectory,";
                if(fileattr & 0x20)
                    out << "Archive File,";
                out << "|File attributes." << Qt::endl;
                uint8_t isdeleted = 0;
                if(firstchar == 0xe5 || firstchar == 0x05) // was allocated but now free
                    isdeleted = 1;
                QString restname = QString::fromStdString(curimg->ReadContent(rootdiroffset + j*32 + 1, 7).toStdString()).replace(" ", "");
                QString extname = QString::fromStdString(curimg->ReadContent(rootdiroffset + j*32 + 8, 3).toStdString()).replace(" ", "");
                QString aliasname = QString(char(firstchar));
                out << "Alias Name|";
                if(extname.count() > 0)
                {
                    aliasname += QString(restname.toUtf8() + "." + extname.toUtf8());
                }
                else
                {
                    aliasname += QString(restname.toUtf8());
                }
                out << aliasname << "|8.3 file name." << Qt::endl;
		//uint8_t createtenth = rootdirbuf.at(i*32 + 13); // NOT GOING TO USE RIGHT NOW...
                qint64 createdate = ConvertDosTimeToUnixTime(qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 15, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 14, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 17, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 16, 1)));
                qint64 accessdate = ConvertDosTimeToUnixTime(0x00, 0x00, qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 19, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 18, 1)));
                qint64 modifydate = ConvertDosTimeToUnixTime(qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 23, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 22, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 25, 1)), qFromLittleEndian<uint8_t>(curimg->ReadContent(rootdiroffset + j*32 + 24, 1)));
                uint16_t hiclusternum = qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 20, 2)); // always zero for fat12/16
                uint16_t loclusternum = qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 26, 2));
                uint32_t clusternum = ((uint32_t)hiclusternum >> 16) + loclusternum;
                QList<uint> clusterlist;
                clusterlist.clear();
                if(clusternum >= 2)
                {
                    clusterlist.append(clusternum);
                    GetNextCluster(curimg, clusternum, fstype, fatoffset, &clusterlist);
                }
                QString layout = "";
                if(clusterlist.count() > 0)
		    layout = ConvertBlocksToExtents(clusterlist, sectorspercluster * bytespersector, clusterareastart * bytespersector);
		out << "Layout|" << layout << "|File offset,size; layout in bytes." << Qt::endl;
                qulonglong physicalsize = clusterlist.count() * sectorspercluster * bytespersector;
		out << "Physical Size|" << QString::number(physicalsize) << "|Sector Size in Bytes for the file." << Qt::endl;
                clusterlist.clear();
		uint32_t logicalsize = 0;
		uint8_t itemtype = 0;
		if(fileattr & 0x10) // sub directory attribute
		{
		    if(firstchar == 0xe5 || firstchar == 0x05) // deleted directory
			itemtype = 2;
		    else // directory
			itemtype = 3;
                    qulonglong curdirsize = 0;
                    int lastdirentry = 0;
                    int curdircnt = 0;
                    for(int k = 0; k < layout.split(";", Qt::SkipEmptyParts).count(); k++)
                    {
                        curdirsize += layout.split(";").at(k).split(",").at(1).toULongLong();
                    }
                    for(int k=0; k < layout.split(";", Qt::SkipEmptyParts).count(); k++)
                    {
                        for(uint m=0; m < curdirsize / 32; m++)
                        {
                            uint8_t curfirstchar = qFromLittleEndian<uint8_t>(curimg->ReadContent(layout.split(";").at(k).split(",").at(0).toULongLong() + m*32, 1));
                            if(curfirstchar == 0x00) // entry is free and all remaining are free
                            {
                                lastdirentry = curdircnt;
                                break;
                            }
                            curdircnt++;
                        }
                    }
                    logicalsize = lastdirentry * 32;
		}
		else
		{
		    if(firstchar == 0xe5 || firstchar == 0x05) // deleted file
			itemtype = 4;
		    else
			itemtype = 5;
		    logicalsize = qFromLittleEndian<uint32_t>(curimg->ReadContent(rootdiroffset + j*32 + 28, 4));
		}
		out << "Logical Size|" << QString::number(logicalsize) << "|Size in Bytes for the file." << Qt::endl;
		//qDebug() << "logicalsize:" << logicalsize;
		// ADD FILE INFO TO THE NODE TREE...
		QString filepath = "";
		QString filename = "";
		if(longname.isEmpty())
		    filename = aliasname;
		else
		    filename = longname;
		QHash<QString, QVariant> nodedata;
		nodedata.clear();
		nodedata.insert("name", QByteArray(filename.toStdString().c_str()).toBase64());
		if(parfilename.isEmpty())
		    filepath = "/";
		else
		    filepath = parfilename;
		nodedata.insert("path", QByteArray(filepath.toUtf8()).toBase64());
                nodedata.insert("size", logicalsize);
                nodedata.insert("create", createdate);
                nodedata.insert("access", accessdate);
                nodedata.insert("modify", modifydate);
                //nodedata.insert("status", "0");
                //nodedata.insert("hash", "0");
		if(logicalsize > 0) // Get Category/Signature
		{
		    if(itemtype == 3 && isdeleted == 0)
                    {
			nodedata.insert("cat", "Directory");
                        nodedata.insert("sig", "Directory"); // category << signature
                    }
		    else
		    {
			//QString catsig = GenerateCategorySignature(curimg, filename, layout); might not need this method, could just read more and see if it has word/ in hex.
			QString catsig = GenerateCategorySignature(curimg, filename, layout.split(";").at(0).split(",").at(0).toULongLong());
			//qDebug() << filename.left(20) << catsig;
                        nodedata.insert("cat", catsig.split("/").first());
                        nodedata.insert("sig", catsig.split("/").last());
		    }
		}
                else
                {
                    nodedata.insert("cat", "Empty");
                    nodedata.insert("sig", "Zero File");
                }
                //nodedata.insert("tag", "0");
                nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
		//nodedata.insert("match", 0);
                QString parentstr = "";
                if(parinode == 0)
                    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt));
                else
                    parentstr = QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(parinode));
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
                out.flush();
                fileprop.close();
                nodedata.clear();
		//qDebug() << "inodecnt:" << inodecnt << filename << layout;
                if(fileattr & 0x10 && logicalsize > 0) // sub directory
                {
                    if(firstchar != 0xe5 && firstchar != 0x05) // not deleted
			inodecnt = ParseFatDirectory(curimg, curstartsector, ptreecnt, inodecnt-1, QString(filepath + filename + "/"), layout);
                }
            }
            else if(fileattr == 0x0f || 0x3f) // long directory entry for succeeding short entry...
            {
                if(firstchar & 0x40)
                {
                    if(!longnamestring.isEmpty()) // orphan long entry
                    {
			QHash<QString, QVariant> nodedata;
			nodedata.clear();
			nodedata.insert("name", QByteArray(longnamestring.toUtf8()).toBase64());
                        nodedata.insert("path", QByteArray("/orphans/").toBase64());
                        nodedata.insert("size", 0);
                        nodedata.insert("create", "0");
                        nodedata.insert("access", "0");
                        nodedata.insert("modify", "0");
                        nodedata.insert("status", "0");
                        nodedata.insert("hash", "0");
                        nodedata.insert("cat", "Empty");
                        nodedata.insert("sig", "Empty File");
                        nodedata.insert("tag", "0");
                        nodedata.insert("id", QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-f" + QString::number(inodecnt)));
			nodedata.insert("match", 0);
			mutex.lock();
			treenodemodel->AddNode(nodedata, QString("e" + curimg->MountPath().split("/").last().split("-e").last() + "-p" + QString::number(ptreecnt) + "-o"), 4, 1);
			mutex.unlock();
			nodedata.clear();
			inodecnt++;
                        //qDebug() << "orphan:" << longnamestring;
                        //orphaninfo.clear();
                        //orphaninfo.insert("filename", QVariant(longnamestring));
                        //orphanlist->append(orphaninfo);
                        //orphanlist->append(longnamestring);
                        longnamestring = "";
                    }
                }
                QString l3 = "";
                QString l2 = "";
                QString l1 = "";
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 28, 2)) < 0xFFFF)
                    l3 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 28, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 30, 2)) < 0xFFFF)
                    l3 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 30, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 14, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 14, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 16, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 16, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 18, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 18, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 20, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 20, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 22, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 22, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 24, 2)) < 0xFFFF)
                    l2 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 24, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 1, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 1, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 3, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 3, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 5, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 5, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 7, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 7, 2))));
                if(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 9, 2)) < 0xFFFF)
                    l1 += QString(QChar(qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 9, 2))));
                longnamestring.prepend(QString(l1 + l2 + l3).toUtf8());
            }
        }
    }
    return inodecnt;
}
 */ 
