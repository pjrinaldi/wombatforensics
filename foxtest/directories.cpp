#include "directories.h"

std::string ConvertDosTimeToHuman(uint16_t* dosdate, uint16_t* dostime)
{
    std::string humanstring = "";
    if(((*dosdate & 0x1e0) >> 5) < 10)
        humanstring += "0";
    humanstring += std::to_string(((*dosdate & 0x1e0) >> 5)); // MONTH
    humanstring += "/"; // DIVIDER
    if(((*dosdate & 0x1f) >> 0) < 10)
        humanstring += "0";
    humanstring += std::to_string(((*dosdate & 0x1f) >> 0)); // MONTH
    humanstring += "/"; // DIVIDER
    humanstring += std::to_string(((*dosdate & 0xfe00) >> 9) + 1980); // YEAR
    humanstring += " ";
    if(dostime == NULL)
	humanstring += "00:00:00";
    else
    {
        if(((*dostime & 0xf800) >> 11) < 10)
            humanstring += "0";
	humanstring += std::to_string(((*dostime & 0xf800) >> 11)); // HOUR
	humanstring += ":";
        if(((*dostime & 0x7e0) >> 5) < 10)
            humanstring += "0";
	humanstring += std::to_string(((*dostime & 0x7e0) >> 5)); // MINUTE
	humanstring += ":";
        if((((*dostime & 0x1f) >> 0) * 2) < 10)
            humanstring += "0";
	humanstring += std::to_string(((*dostime & 0x1f) >> 0) * 2); // SECOND
    }
    //humanstring += " (UTC)";
    
    return humanstring;
}
/*
FXString WombatForensics::GetFileItem(FXString* filecontents, int item)
{
    FXArray<FXint> posarray;
    posarray.clear();
    int found = 0;
    posarray.append(-1);
    while(found > -1)
    {
        found = filecontents->find("|", found+1);
        if(found > -1)
            posarray.append(found);
    }
    posarray.append(filecontents->length());
    if(item < posarray.no())
        return filecontents->mid(posarray.at(item)+1, posarray.at(item+1) - posarray.at(item) - 1);
    else
        return "";
}
*/ 

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
    
    /*
    std::istringstream input("abc|def|gh");
    std::vector<std::array<char, 4>> v;
 
    // note: the following loop terminates when std::ios_base::operator bool()
    // on the stream returned from getline() returns false
    for (std::array<char, 4> a; input.getline(&a[0], 4, '|'); ) {
        v.push_back(a);
    }
 
    for (auto& a : v) {
        std::cout << &a[0] << '\n';
    }

    std::vector<std::string> dirlayoutlist;
    dirlayoutlist.clear();
    std::istringstream rdll(curfat->curdirlayout);
    std::string rdls;
    while(getline(rdll, rdls, ';'))
	dirlayoutlist.push_back(rdls);
    */
}

void ReadDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem)
{
    std::vector<std::string> filearray;
    filearray.clear();
    std::string pathstr = currentitem->tmppath + "burrow/";
    std::string filefilestr = currentitem->forimg->ImageFileName() + "." + std::to_string(currentitem->voloffset);
    if(curfileitem != NULL)
        filefilestr += "." + std::to_string(curfileitem->gid);
    std::cout << "file file string to match: " << filefilestr << std::endl;
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
    if(filearray.size() == 0)
    {
        if(curfileitem == NULL)
            LoadDirectory(currentitem, filevector, NULL);
        else
            LoadDirectory(currentitem, filevector, curfileitem);
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
            tmpitem.path = GetFileItem(&filecontent, 5);
            tmpitem.create = GetFileItem(&filecontent, 6);
            tmpitem.access = GetFileItem(&filecontent, 7);
            tmpitem.modify = GetFileItem(&filecontent, 8);
            tmpitem.layout = GetFileItem(&filecontent, 9);
            //std::cout << "File item Values: " << std::endl;
            //std::cout << "filecontent:" << filecontent << std::endl;
            //std::cout << tmpid << " " << tmpitem.isdeleted << " " << tmpitem.isdirectory << std::endl;
            //std::cout << tmpitem.size << " " << tmpitem.name << std::endl;
            //std::cout << tmpitem.create << " " << tmpitem.access << " " << tmpitem.modify << std::endl;
            filevector->push_back(tmpitem);
        }
    }
}

//void LoadDirectory(CurrentItem* currentitem)
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
	    //curinode = ParseFatDirectory(curimg, curstartsector, ptreecnt, 0, "", "");
	    //curinode = AddVirtualFileSystemFiles(curimg, ptreecnt, fatcount, fatsize * bytespersector, curinode);
            char* pname = new char[12];
            curforimg->ReadContent((uint8_t*)fattype, offset + 54, 5);
            if(strcmp(fattype, "FAT12") == 0)
            {
                //LoadFatDirectory(currentitem, filevector, 0);
                LoadFatDirectory(currentitem, filevector, curfileitem);
            }
            else if(strcmp(fattype, "FAT16") == 0)
            {
                //LoadFatDirectory(currentitem, filevector, 0);
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

//void LoadFatDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, uint64_t curinode)
void LoadFatDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem)
{
    // BYTES PER SECTOR
    uint16_t bytespersector = 0;
    ReadForImgContent(currentitem->forimg, &bytespersector, currentitem->voloffset + 11);
    //std::cout << "bytes per sector: " << bytespersector << std::endl;
    // FAT COUNT
    uint8_t fatcount = 0;
    currentitem->forimg->ReadContent(&fatcount, currentitem->voloffset + 16, 1);
    //std::cout << "fat count: " << (uint)fatcount << std::endl;
    // SECTORS PER CLUSTER
    uint8_t sectorspercluster = 0;
    currentitem->forimg->ReadContent(&sectorspercluster, currentitem->voloffset + 13, 1);
    //std::cout << "sectors per cluster: " << (uint) sectorspercluster << std::endl;
    // RESERVED AREA SIZE
    uint16_t reservedareasize = 0;
    ReadForImgContent(currentitem->forimg, &reservedareasize, currentitem->voloffset + 14);
    //std::cout << "reserved area size: " << reservedareasize << std::endl;
    // ROOT DIRECTORY MAX FILES
    uint16_t rootdirmaxfiles = 0;
    ReadForImgContent(currentitem->forimg, &rootdirmaxfiles, currentitem->voloffset + 17);
    //std::cout << "root dir max files: " << rootdirmaxfiles << std::endl;
    if(currentitem->itemtext.find("[FAT12]") != std::string::npos || currentitem->itemtext.find("[FAT16]") != std::string::npos)
    {
        // FAT SIZE
        uint16_t fatsize = 0;
        ReadForImgContent(currentitem->forimg, &fatsize, currentitem->voloffset + 22);
        //std::cout << "fat size: " << fatsize << std::endl;
        // CLUSTER AREA START
        uint64_t clusterareastart = reservedareasize + fatcount * fatsize + ((rootdirmaxfiles * 32) + (bytespersector - 1)) / bytespersector;
        //std::cout << "Cluster area start: " << clusterareastart << std::endl;
        // DIRECTORY OFFSET
        uint64_t diroffset = (reservedareasize + fatcount * fatsize) * bytespersector;
        //std::cout << "dir offset: " << diroffset << std::endl;
        // DIRECTORY SIZE
        uint64_t dirsize = rootdirmaxfiles * 32 + bytespersector - 1;
        //std::cout << "dir size: " << dirsize << std::endl;
        // ROOT DIRECTORY LAYOUT
        std::string rootdirlayout = std::to_string(diroffset) + "," + std::to_string(dirsize) + ";";
        //std::cout << "root dir layout: " << rootdirlayout << std::endl;
        //if(curinode == 0) // root directory
        if(curfileitem == NULL) // root directory
        {
            // SET dirsize, dirlayout here, the rest should be the same...
            // then the CODE DOESN"T REPEAT
            uint direntrycount = dirsize / 32;
            //std::cout << "dir entry count: " << direntrycount << std::endl;
	    // PARSE DIRECTORY ENTRIES
            std::string longnamestring = "";
            for(uint i=0; i < direntrycount; i++)
            {
                FileItem tmpitem;
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
                    uint8_t fileattr = 0;
                    currentitem->forimg->ReadContent(&fileattr, diroffset + i*32 + 11, 1);
                    //std::cout << "file attr: " << std::hex << (uint)fileattr << std::endl;
                    //std::cout << "first char: " << std::hex << (uint)firstchar << std::endl;
                    if((uint)fileattr == 0x0f || (uint)fileattr == 0x3f) // long directory name
                    {
                        uint lsn = ((int)firstchar & 0x0f);
                        if(lsn <= 20)
                        {
                            std::string longname = "";
                            int arr[13] = {1, 3, 5, 7, 9, 14, 16, 18, 20, 22, 24, 28, 30};
                            for(int j=0; j < 13; j++)
                            {
                                uint16_t longletter = 0;
                                ReadForImgContent(currentitem->forimg, &longletter, diroffset + i*32 + arr[j]);
                                if(longletter < 0xFFFF)
                                    longname += (char)longletter;
                            }
                            longnamestring.insert(0, longname);
                        }
                    }
                    else
                    {
                        // FILE OR DIRECTORY
                        //bool isdirectory = false; // file
                        if((uint)fileattr & 0x10)
                            tmpitem.isdirectory = true; // directory
                        //std::cout << "is directory: " << isdirectory << std::endl;
                        // DELETED OR NOT DELETED
                        //bool isdeleted = false; // not deleted
                        if((uint)firstchar == 0x05 || (uint)firstchar == 0xe5)
                            tmpitem.isdeleted = true; // deleted
                        //std::cout << "is deleted: " << isdeleted << std::endl;
                        // FILENAME
                        //std::string filename = "";
                        if(!longnamestring.empty())
                        {
                            tmpitem.name = longnamestring;
                            //filename = longnamestring;
                        }
                        else
                        {
                            char* rname = new char[8];
                            currentitem->forimg->ReadContent((uint8_t*)rname, diroffset + i*32 + 1, 7);
                            rname[7] = 0;
                            std::string restname(rname);
                            char* ename = new char[4];
                            currentitem->forimg->ReadContent((uint8_t*)ename, diroffset + i*32 + 8, 3);
                            ename[3] = 0;
                            std::string extname(ename);
                            while(extname.size() > 0)
                            {
                                std::size_t findempty = extname.find(" ", 0);
                                if(findempty != std::string::npos)
                                    extname.erase(findempty, 1);
                                else
                                    break;
                            }
                            restname.erase(std::remove_if(restname.begin(), restname.end(), isspace), restname.end());
                            if(tmpitem.isdeleted)
                                tmpitem.name = std::string(1, '_') + restname;
                            else
                                tmpitem.name = std::string(1, (char)firstchar) + restname;
                            if(extname.size() > 0)
                                tmpitem.name += "." + extname;
                        }
                        tmpitem.path = "/";
                        //std::cout << "file name: " << filename << std::endl;
                        longnamestring = "";
                        // LOGICAL FILE SIZE
                        uint32_t logicalsize = 0;
                        ReadForImgContent(currentitem->forimg, &logicalsize, diroffset + i*32 + 28);
                        //std::cout << "logical file size: " << logicalsize << std::endl;
                        tmpitem.size = logicalsize;
                        // CREATE DATE
                        uint16_t createdate = 0;
                        ReadForImgContent(currentitem->forimg, &createdate, diroffset + i*32 + 16);
                        uint16_t createtime = 0;
                        ReadForImgContent(currentitem->forimg, &createtime, diroffset + i*32 + 14);
                        tmpitem.create = ConvertDosTimeToHuman(&createdate, &createtime);
                        //std::cout << "Created Date: " << datecreated << std::endl;
                        // ACCESS DATE
                        uint16_t accessdate = 0;
                        ReadForImgContent(currentitem->forimg, &accessdate, diroffset + i*32 + 18);
                        tmpitem.access = ConvertDosTimeToHuman(&accessdate, NULL);
                        //std::cout << "Accessed Date: " << dateaccessed << std::endl;
                        // MODIFY DATE
                        uint16_t modifydate = 0;
                        ReadForImgContent(currentitem->forimg, &modifydate, diroffset + i*32 + 24);
                        uint16_t modifytime = 0;
                        ReadForImgContent(currentitem->forimg, &modifytime, diroffset + i*32 + 22);
                        tmpitem.modify = ConvertDosTimeToHuman(&modifydate, &modifytime);
                        //std::cout << "Modified Date: " << datemodified << std::endl;
                        uint16_t hiclusternum = 0;
                        ReadForImgContent(currentitem->forimg, &hiclusternum, diroffset + i*32 + 20); // always 0 for FAT12/16
                        uint16_t loclusternum = 0;
                        ReadForImgContent(currentitem->forimg, &loclusternum, diroffset + i*32 + 26);
                        uint32_t clusternum = ((uint32_t)hiclusternum >> 16) + loclusternum;
                        std::vector<uint> clusterlist;
                        clusterlist.clear();
                        if(clusternum >= 2)
                        {
                            clusterlist.push_back(clusternum);
                            if(currentitem->itemtext.find("[FAT12]") != std::string::npos)
                                GetNextCluster(currentitem->forimg, clusternum, 1, bytespersector, &clusterlist);
                            else if(currentitem->itemtext.find("[FAT16]") != std::string::npos)
                                GetNextCluster(currentitem->forimg, clusternum, 2, bytespersector, &clusterlist);
		            //GetNextCluster(curforimg, rootdircluster, 4, fatoffset, &clusterlist);
//void GetNextCluster(ForImg* curimg, uint32_t clusternum, uint8_t fstype, uint64_t fatoffset, std::vector<uint>* clusterlist);
                        }
                        //for(int i=0; i < clusterlist.size(); i++)
                        //    std::cout << "cluster " << i << ": " << clusterlist.at(i) << std::endl;
                        if(clusterlist.size() > 0)
                            tmpitem.layout = ConvertBlocksToExtents(&clusterlist, sectorspercluster * bytespersector, clusterareastart * bytespersector);
                        //std::cout << "tmpitem layout: " << tmpitem.layout << std::endl;
                        /*
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
                         */ 
                        if(!tmpitem.layout.empty())
                            filevector->push_back(tmpitem);
                    }
                }
            }
        }
        else // NOT ROOT DIRECTORY
        {
        }
    }
    else // FAT32, EXFAT
    {
    }
}
