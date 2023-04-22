#include "directories.h"

std::string GetFileItem(std::string* filecontents, int item)
{
    std::vector<std::string> contentlist;
    contentlist.clear();
    std::istringstream contents(*filecontents);
    std::string itemcontent;
    while(getline(contents, itemcontent, '|'))
        contentlist.push_back(itemcontent);
    if(item < contentlist.size())
        return contentlist.at(item);
    else
        return "";
}

std::string SetFileItem(std::string* filecontents, int item, std::string value)
{
    std::vector<std::string> contentlist;
    contentlist.clear();
    std::istringstream contents(*filecontents);
    std::string itemcontent;
    while(getline(contents, itemcontent, '|'))
        contentlist.push_back(itemcontent);
    if(item < contentlist.size())
        contentlist[item] = value;
    std::string newcontents = "";
    for(int i=0; i < contentlist.size(); i++)
        newcontents += contentlist.at(i) + "|";

    return newcontents;
}

int ReadDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem)
{
    int filecount = 0;
    std::vector<std::string> filearray;
    filearray.clear();
    std::string pathstr = currentitem->tmppath + "burrow/";
    std::string filefilestr = currentitem->forimg->ImageFileName() + "." + std::to_string(currentitem->voloffset);
    if(curfileitem != NULL)
        filefilestr += "." + std::to_string(curfileitem->gid);
    //std::cout << "file file string to match: " << filefilestr << std::endl;
    for (const auto & entry : std::filesystem::directory_iterator(pathstr))
    {
        if(filefilestr.compare(entry.path().stem()) == 0)
        {
            filearray.push_back(entry.path().string());
            //std::cout << "child file match: " << entry.path().string() << std::endl;
        }
        //std::cout << entry.path().root_path() << " " << entry.path().stem() << " " << entry.path().extension() << std::endl;
        //std::cout << filefilestr << std::endl;
    }
    filecount = filearray.size();
    //std::cout << "filecount: " << filecount << std::endl;
    if(filearray.size() == 0)
    {
        if(curfileitem == NULL)
            LoadDirectory(currentitem, filevector, NULL);
        else
        {
            //std::cout << "load sub dir..." << std::endl;
            LoadDirectory(currentitem, filevector, curfileitem);
        }
    }
    else
    {
        for(int i=0; i < filearray.size(); i++)
        {
            //std::cout << "filestr " << i << ": " << filearray.at(i) << std::endl;
            std::ifstream filestream;
            filestream.open(std::string(filearray.at(i)).c_str(), std::ios::in | std::ios::ate);
            auto readsize = filestream.tellg();
            std::string filecontent(readsize, '\0');
            filestream.seekg(0);
            filestream.read(&filecontent[0], readsize);
            filestream.close();
            FileItem tmpitem;
            tmpitem.gid = std::stoull(GetFileItem(&filecontent, 0).c_str());
            tmpitem.isdeleted = std::stoi(GetFileItem(&filecontent, 1).c_str());
            tmpitem.isdirectory = std::stoi(GetFileItem(&filecontent, 2).c_str());
            tmpitem.size = std::stoull(GetFileItem(&filecontent, 3).c_str());
            tmpitem.name = GetFileItem(&filecontent, 4);
            //std::cout << "read file name: " << tmpitem.name << " isdirectory: " << tmpitem.isdirectory << std::endl;
            tmpitem.path = GetFileItem(&filecontent, 5);
            tmpitem.create = GetFileItem(&filecontent, 6);
            tmpitem.access = GetFileItem(&filecontent, 7);
            tmpitem.modify = GetFileItem(&filecontent, 8);
            tmpitem.layout = GetFileItem(&filecontent, 9);
            tmpitem.isvirtual = std::stoi(GetFileItem(&filecontent, 10).c_str());
            tmpitem.cat = GetFileItem(&filecontent, 11);
            tmpitem.sig = GetFileItem(&filecontent, 12);
            tmpitem.hash = GetFileItem(&filecontent, 13);
            tmpitem.tag = GetFileItem(&filecontent, 14);
            tmpitem.match = GetFileItem(&filecontent, 15);
            tmpitem.filename = GetFileItem(&filecontent, 16);
            //std::cout << "File item Values: " << std::endl;
            //std::cout << "filecontent:" << filecontent << std::endl;
            //std::cout << tmpid << " " << tmpitem.isdeleted << " " << tmpitem.isdirectory << std::endl;
            //std::cout << tmpitem.size << " " << tmpitem.name << std::endl;
            //std::cout << tmpitem.create << " " << tmpitem.access << " " << tmpitem.modify << std::endl;
            filevector->push_back(tmpitem);
        }
    }
    return filecount;
}

void LoadDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem)
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
            char* pname = new char[12];
            curforimg->ReadContent((uint8_t*)fattype, offset + 54, 5);
            if(strcmp(fattype, "FAT12") == 0)
            {
                LoadFatDirectory(currentitem, filevector, curfileitem);
            }
            else if(strcmp(fattype, "FAT16") == 0)
            {
                LoadFatDirectory(currentitem, filevector, curfileitem);
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
