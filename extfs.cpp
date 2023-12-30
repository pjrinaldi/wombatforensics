#include "extfs.h"

void LoadExtDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem)
{
    // COMPATIBILITY FLAGS
    uint32_t compatflags = 0;
    std::string compatibilityflags = "";
    ReadForImgContent(currentitem->forimg, &compatflags, currentitem->voloffset + 1116);
    // INCOMPATIBLE FLAGS
    uint32_t incompatflags = 0;
    std::string incompatibleflags = "";
    ReadForImgContent(currentitem->forimg, &incompatflags, currentitem->voloffset + 1120);
    // READ ONLY FLAGS
    uint32_t roflags = 0;
    //std::string readonlyflags = "";
    ReadForImgContent(currentitem->forimg, &roflags, currentitem->voloffset + 1124);
    // GROUP DESCRIPTOR SIZE
    uint16_t grpdescsize = 32;
    if(incompatflags & 0x80)
	ReadForImgContent(currentitem->forimg, &grpdescsize, currentitem->voloffset + 1278);
    // FILE SYSTEM BLOCK COUNT
    uint32_t fsblockcnt = 0;
    ReadForImgContent(currentitem->forimg, &fsblockcnt, currentitem->voloffset + 1028);
    // BLOCK GROUP BLOCK COUNT
    uint32_t blockgroupblockcount = 0;
    ReadForImgContent(currentitem->forimg, &blockgroupblockcount, currentitem->voloffset + 1056);
    // BLOCK SIZE
    uint32_t blksize = 0;
    ReadForImgContent(currentitem->forimg, &blksize, currentitem->voloffset + 1048);
    uint32_t blocksize = 1024 * pow(2, blksize);
    // INODE SIZE
    uint16_t inodesize = 0;
    ReadForImgContent(currentitem->forimg, &inodesize, currentitem->voloffset + 1112);
    // BLOCK GROUP INODE COUNT
    uint32_t blockgroupinodecount = 0;
    ReadForImgContent(currentitem->forimg, &blockgroupinodecount, currentitem->voloffset + 1064);
    // INODE ADDRESS TABLE
    uint32_t blockgroupcount = fsblockcnt / blockgroupblockcount;
    uint blkgrpcntrem = fsblockcnt % blockgroupblockcount;
    if(blkgrpcntrem > 0)
	blockgroupcount++;
    if(blockgroupcount == 0)
	blockgroupcount = 1;
    std::vector<uint32_t> inodeaddresstable;
    inodeaddresstable.clear();
    for(uint i=0; i < blockgroupcount; i++)
    {
	uint32_t iat = 0;
	if(blocksize == 1024)
	    ReadForImgContent(currentitem->forimg, &iat, currentitem->voloffset + 2*blocksize + i * grpdescsize + 8);
	else
	    ReadForImgContent(currentitem->forimg, &iat, currentitem->voloffset + blocksize + i * grpdescsize + 8);
	inodeaddresstable.push_back(iat);
    }
    // ROOT INODE TABLE ADDRESS
    uint32_t rootinodetableaddress = 0;
    if(blockgroupinodecount > 2)
	ReadForImgContent(currentitem->forimg, &rootinodetableaddress, currentitem->voloffset + 2056);
    else
	ReadForImgContent(currentitem->forimg, &rootinodetableaddress, currentitem->voloffset + 2088);
    // REVISION LEVEL
    uint32_t revmaj = 0;
    ReadForImgContent(currentitem->forimg, &revmaj, currentitem->voloffset + 1100);
    uint16_t revmin = 0;
    ReadForImgContent(currentitem->forimg, &revmin, currentitem->voloffset + 1086);
    std::string revstr = "";
    revstr.append(std::to_string(revmaj));
    revstr.append(".");
    revstr.append(std::to_string(revmin));
    float revision = std::stof(revstr);

    // CURRENT INODE
    uint64_t currentinode = 2;
    if(curfileitem != NULL)
    {
	std::size_t extinodesplit = curfileitem->properties.find(">");
	std::cout << "cur item ext inode: " << std::stoull(curfileitem->properties.substr(0, extinodesplit)) << std::endl;
	currentinode = std::stoull(curfileitem->properties.substr(0, extinodesplit));
    }
    uint64_t inodestartingblock = 0;
    uint8_t blockgroupnumber = 0;
    for(int i=1; i <= inodeaddresstable.size(); i++)
    {
	if(currentinode < i*blockgroupinodecount)
	{
	    inodestartingblock = inodeaddresstable.at(i-1);
	    blockgroupnumber = i - 1;
	    break;
	}
    }
    uint64_t relativecurrentinode = currentinode - 1 - (blockgroupnumber * blockgroupinodecount);
    uint64_t currentoffset = currentitem->voloffset + inodestartingblock * blocksize + inodesize * relativecurrentinode;
    std::string currentlayout = "";
    if(curfileitem != NULL)
	currentlayout = curfileitem->layout;
    else
    {
	std::vector<uint32_t> blocklist;
	blocklist.clear();
	GetContentBlocks(currentitem, blocksize, currentoffset, incompatflags, &blocklist);
	currentlayout = ConvertBlocksToExtents(&blocklist, blocksize);
    }
    // INODE FLAGS
    uint32_t inodeflags = 0;
    ReadForImgContent(currentitem->forimg, &inodeflags, currentoffset + 32);
    // GET THE DIRECTORY CONTENT OFFSETS/LENGTHS AND THEN LOOP OVER THEM
    std::vector<std::string>layoutlist;
    layoutlist.clear();
    std::istringstream ll(currentlayout);
    std::string ls;
    while(getline(ll, ls, ';'))
	layoutlist.push_back(ls);
    for(int i=0; i < layoutlist.size(); i++)
    {
	std::size_t layoutsplit = layoutlist.at(i).find(",");
	uint64_t curoffset = std::stoull(layoutlist.at(i).substr(0, layoutsplit));
	uint64_t curlength = std::stoull(layoutlist.at(i).substr(layoutsplit+1));
	uint64_t curoff = currentitem->voloffset + curoffset + 24; // SKIP . AND .. ENTRIES WHICH ARE ALWAYS 1ST 2 AND 12 BYTES EACH
	if(inodeflags & 0x1000) // hash trees in use
	{
	    curoff = currentitem->voloffset + curoffset + 40; // THIS SHOULD ACCOUNT FOR HASH TREE DEPTH OF 0, NEED TO TEST FOR 1 - 3
	}
	bool nextisdeleted = false;
	while(curoff < curoffset + curlength - 8)
	{
	    uint16_t entrylength = 0;
	    uint8_t curlen = 0;
	    currentitem->forimg->ReadContent(&curlen, curoff + 6, 1);
	    int lengthdiv = (8 + curlen) / 4;
	    int lengthrem = (8 + curlen) % 4;
	    int newlength = 0;
	    if(lengthrem = 0)
		newlength = lengthdiv * 4;
	    else
		newlength = lengthdiv * 4 + 4;
	    int32_t extinode = 0;
	    uint8_t* einode = new uint8_t[4];
	    currentitem->forimg->ReadContent(einode, curoff, 4);
	    extinode = (int32_t)einode[0] | (int32_t)einode[1] << 8 | (int32_t)einode[2] << 16 | (int32_t)einode[3] << 24;
	    if(extinode > 0)
	    {
		FileItem tmpitem;
		if(curfileitem == NULL)
		    tmpitem.path = "/";
		else
		    tmpitem.path = curfileitem->path + curfileitem->name + "/";
		uint16_t namelength = 0;
		int filetype = -1;
		ReadForImgContent(currentitem->forimg, &entrylength, curoff + 4);
		if(incompatflags & 0x02 || revision > 0.4)
		{
		    uint8_t nl = 0;
		    currentitem->forimg->ReadContent(&nl, curoff + 6, 1);
		    namelength = (uint16_t)nl;
		    uint8_t ft = 0;
		    currentitem->forimg->ReadContent(&ft, curoff + 7, 1);
		    filetype = (int)ft;
		}
		else
		    ReadForImgContent(currentitem->forimg, &namelength, curoff + 6);
		char* filename = new char[namelength + 1];
		currentitem->forimg->ReadContent((uint8_t*)filename, curoff + 8, namelength);
		filename[namelength] = 0;
		tmpitem.name = std::string(filename);
		if(nextisdeleted)
		    tmpitem.isdeleted = true;
		if(newlength < entrylength)
		    nextisdeleted = true;
		else
		    nextisdeleted = false;
		if(filetype == 2) // IS DIRECTORY
		    tmpitem.isdirectory = true;
		uint64_t currentinodetablestartblock = 0;
		uint8_t blockgroupnumber = 0;
		for(int j=1; j <= inodeaddresstable.size(); j++)
		{
		    if(extinode < j * blockgroupinodecount)
		    {
			currentinodetablestartblock = inodeaddresstable.at(j - 1);
			blockgroupnumber = j - 1;
			break;
		    }
		}
		uint64_t logicalsize = 0;
		uint32_t logicsize = 0;
		uint64_t currentinodeoffset = currentitem->voloffset + currentinodetablestartblock * blocksize + inodesize * (extinode - 1 - blockgroupnumber * blockgroupinodecount);
		uint16_t filemode = 0;
		ReadForImgContent(currentitem->forimg, &filemode, currentinodeoffset);
		std::string filemodestring = "---------";
		if(filemode & 0xc000) // unix socket
		    filemodestring.replace(0, 1, "s");
		if(filemode & 0xa000) // symbolic link
		    filemodestring.replace(0, 1, "l");
		if(filemode & 0x6000) // block device
		    filemodestring.replace(0, 1, "b");
		if(filemode & 0x2000) // char device
		    filemodestring.replace(0, 1, "c");
		if(filemode & 0x1000) // FIFO (pipe)
		    filemodestring.replace(0, 1, "p");
		if(filemode & 0x8000) // regular file
		{
		    filemodestring.replace(0, 1, "-");
		    if(roflags & 0x02)
		    {
			uint32_t lowersize = 0;
			ReadForImgContent(currentitem->forimg, &lowersize, currentinodeoffset + 4);
			uint32_t uppersize = 0;
			ReadForImgContent(currentitem->forimg, &uppersize, currentinodeoffset + 108);
			logicalsize = ((uint64_t)uppersize >> 32) + lowersize;
		    }
		    else
		    {
			ReadForImgContent(currentitem->forimg, &logicsize, currentinodeoffset + 4);
			logicalsize = logicsize;
		    }
		}
		if(filemode & 0x4000) // directory
		{
		    ReadForImgContent(currentitem->forimg, &logicsize, currentinodeoffset + 4);
		    logicalsize = logicsize;
		    filemodestring.replace(0, 1, "d");
		}
		if(filemode & 0x100) // user read
		    filemodestring.replace(1, 1, "r");
		if(filemode & 0x080) // user write
		    filemodestring.replace(2, 1, "w");
		if(filemode & 0x040) // user execute
		    filemodestring.replace(3, 1, "x");
		if(filemode & 0x020) // group read
		    filemodestring.replace(4, 1, "r");
		if(filemode & 0x010) // group write
		    filemodestring.replace(5, 1, "w");
		if(filemode & 0x008) // group execute
		    filemodestring.replace(6, 1, "x");
		if(filemode & 0x004) // other read
		    filemodestring.replace(7, 1, "r");
		if(filemode & 0x002) // other write
		    filemodestring.replace(8, 1, "w");
		if(filemode & 0x001) // other execute
		    filemodestring.replace(9, 1, "x");
		uint16_t lowergroupid = 0;
		ReadForImgContent(currentitem->forimg, &lowergroupid, currentinodeoffset + 24);
		uint16_t uppergroupid = 0;
		ReadForImgContent(currentitem->forimg, &uppergroupid, currentinodeoffset + 122);
		uint32_t groupid = ((uint32_t)uppergroupid >> 16) + lowergroupid;
		uint16_t loweruserid = 0;
		ReadForImgContent(currentitem->forimg, &loweruserid, currentinodeoffset + 2);
		uint16_t upperuserid = 0;
		ReadForImgContent(currentitem->forimg, &upperuserid, currentinodeoffset + 120);
		uint32_t userid = ((uint32_t)upperuserid >> 16) + loweruserid;
		uint32_t accessdate = 0;
		ReadForImgContent(currentitem->forimg, &accessdate, currentinodeoffset + 8);
		tmpitem.access = ConvertUnixTimeToHuman(accessdate);
		uint32_t statusdate = 0;
		ReadForImgContent(currentitem->forimg, &statusdate, currentinodeoffset + 12);
		tmpitem.change = ConvertUnixTimeToHuman(statusdate);
		uint32_t modifydate = 0;
		ReadForImgContent(currentitem->forimg, &modifydate, currentinodeoffset + 16);
		tmpitem.modify = ConvertUnixTimeToHuman(modifydate);
		uint32_t deletedate = 0;
		ReadForImgContent(currentitem->forimg, &deletedate, currentinodeoffset + 20);
		uint16_t linkcount = 0;
		ReadForImgContent(currentitem->forimg, &linkcount, currentinodeoffset + 26);
		uint32_t createdate = 0;
		if(((compatflags & 0x00000200UL) != 0) || ((incompatflags & 0x0001f7c0UL) != 0) || ((roflags & 0x00000378UL) != 0)) // EXT4
		    ReadForImgContent(currentitem->forimg, &createdate, currentinodeoffset + 144);
		if(createdate > 0)
		    tmpitem.create = ConvertUnixTimeToHuman(createdate);
		uint32_t curinodeflags = 0;
		ReadForImgContent(currentitem->forimg, &curinodeflags, currentinodeoffset + 32);
		std::string fileattributes = "";
		if(inodeflags & 0x200000)
		    fileattributes += "Stores a Large Extended Attribute,";
		if(inodeflags & 0x80000)
		    fileattributes += "Uses Extents,";
		if(inodeflags & 0x40000)
		    fileattributes += "Huge File,";
		if(inodeflags & 0x20000)
		    fileattributes += "Top of Directory,";
		if(inodeflags & 0x10000)
		    fileattributes += "Synchronous Data Write,";
		if(inodeflags & 0x8000)
		    fileattributes += "File Tail not Merged,";
		if(inodeflags & 0x4000)
		    fileattributes += "File Data Written through Journal,";
		if(inodeflags & 0x2000)
		    fileattributes += "AFS Magic Directory,";
		if(inodeflags & 0x1000)
		    fileattributes += "Hashed Indexed Directory,";
		if(inodeflags & 0x800)
		    fileattributes += "Encrypted,";
		if(inodeflags & 0x400)
		    fileattributes += "No Compression,";
		if(inodeflags & 0x200)
		    fileattributes += "Has Compression in 1 or more blocks,";
		if(inodeflags & 0x100)
		    fileattributes += "Dirty Compression,";
		if(inodeflags & 0x80)
		    fileattributes += "No Update ATIME,";
		if(inodeflags & 0x40)
		    fileattributes += "dump utility ignores file,";
		if(inodeflags & 0x20)
		    fileattributes += "Append Only,";
		if(inodeflags & 0x10)
		    fileattributes += "Immutable,";
		if(inodeflags & 0x08)
		    fileattributes += "Synchronous Writes,";
		if(inodeflags & 0x04)
		    fileattributes += "Compressed,";
		if(inodeflags & 0x02)
		    fileattributes += "Preserved for un-deletion,";
		if(inodeflags & 0x01)
		    fileattributes += "Requires Secure Deletion,";
		if(inodeflags == 0x00)
		    fileattributes = "No attributes";
		std::vector<uint32_t> curblocklist;
		curblocklist.clear();
		GetContentBlocks(currentitem, blocksize, currentinodeoffset, incompatflags, &curblocklist);
		if(curblocklist.size() > 0)
		    tmpitem.layout = ConvertBlocksToExtents(&curblocklist, blocksize);
		uint64_t physicalsize = 0;
		std::vector<std::string> laylist;
		laylist.clear();
		std::istringstream laystream(tmpitem.layout);
		std::string laystring;
		while(getline(laystream, laystring, ';'))
		    laylist.push_back(laystring);
		for(int j=0; j < laylist.size(); j++)
		{
		    std::size_t layoutsplit = laylist.at(i).find(",");
		    physicalsize += std::stoull(laylist.at(i).substr(layoutsplit+1));
		}
		int phyremcnt = physicalsize % blocksize;
		if(phyremcnt > 0)
		    physicalsize += blocksize;
		curblocklist.clear();
		tmpitem.size = logicalsize;
		std::string deletestr = "";
		if(deletedate > 0)
		    deletestr = ConvertUnixTimeToHuman(deletedate);
		tmpitem.properties = std::to_string(extinode) + ">" + filemodestring + ">" + std::to_string(userid) + " / " + std::to_string(groupid) + ">" + deletestr + ">" + std::to_string(linkcount) + ">" + fileattributes + ">" + tmpitem.layout + ">" + std::to_string(physicalsize) + ">" + std::to_string(logicalsize);
		if(tmpitem.isdirectory)
		{
		    //tmpitem.size = physicalsize;
		    tmpitem.cat = "Directory";
		    tmpitem.sig = "Directory";
		}
		else
		{
		    if(logicalsize > 0 && !tmpitem.layout.empty())
			GenerateCategorySignature(currentitem, &tmpitem.name, &(tmpitem.layout), &(tmpitem.cat), &(tmpitem.sig));
		    else
		    {
			tmpitem.cat = "Empty";
			tmpitem.sig = "Empty File";
		    }
		}
		filevector->push_back(tmpitem);
	    }
	    curoff += newlength;
	}
    }
}

void GetContentBlocks(CurrentItem* currentitem, uint32_t blocksize, uint64_t currentoffset, uint32_t incompatflags, std::vector<uint32_t>* blocklist)
{
    uint32_t inodeflags = 0;
    ReadForImgContent(currentitem->forimg, &inodeflags, currentoffset + 32);
    if(incompatflags & 0x40 && inodeflags & 0x80000) // FS USES EXTENTS AND INODE USES EXTENTS
    {
	uint16_t extententries = 0;
	ReadForImgContent(currentitem->forimg, &extententries, currentoffset + 42);
	uint16_t extentdepth = 0;
	ReadForImgContent(currentitem->forimg, &extentdepth, currentoffset + 46);
	if(extentdepth == 0) // use ext4_extent
	{
	    for(uint16_t i=0; i < extententries; i++)
	    {
		uint16_t blocklength = 0;
		ReadForImgContent(currentitem->forimg, &blocklength, currentoffset + 56 + i*12);
		uint16_t starthi = 0;
		ReadForImgContent(currentitem->forimg, &starthi, currentoffset + 58 + i*12);
		uint32_t startlo = 0;
		ReadForImgContent(currentitem->forimg, &startlo, currentoffset + 60 + i*12);
		uint64_t startblock = (((uint64_t)starthi >> 32) + startlo); // block #, not bytes
		blocklist->push_back(startblock);
	    }
	}
	else // use ext4_extent_idx
	{
	    std::vector<uint32_t> leafnodes;
	    leafnodes.clear();
	    for(uint16_t i=0; i < extententries; i++)
	    {
		uint32_t leafnode = 0;
		ReadForImgContent(currentitem->forimg, &leafnode, currentoffset + 56 + i*12);
		leafnodes.push_back(leafnode);
	    }
	    for(int i=0; i < leafnodes.size(); i++)
	    {
		uint16_t lextententries = 0;
		ReadForImgContent(currentitem->forimg, &lextententries, currentoffset + leafnodes.at(i) * blocksize + 2);
		uint16_t lextentdepth = 0;
		ReadForImgContent(currentitem->forimg, &lextentdepth, currentoffset + leafnodes.at(i) * blocksize + 6);
		if(lextentdepth == 0) // use ext4_extent
		{
		    for(uint16_t j=0; j < lextententries; j++)
		    {
			uint16_t blocklength = 0;
			ReadForImgContent(currentitem->forimg, &blocklength, currentoffset + leafnodes.at(i) * blocksize + 16 + j*12);
			uint16_t starthi = 0;
			ReadForImgContent(currentitem->forimg, &starthi, currentoffset + leafnodes.at(i) * blocksize + 18 + j*12);
			uint32_t startlo = 0;
			ReadForImgContent(currentitem->forimg, &startlo, currentoffset + leafnodes.at(i) * blocksize + 20 + j*12);
			uint64_t startblock = (((uint64_t)starthi >> 32) + startlo);
			blocklist->push_back(startblock);
		    }
		}
		else // use ext4_extent_idx
		{
		    std::cout << "repeat leafnode exercise here...";
		}
	    }
	}
    }
    else // direct and indirect blocks
    {
	for(int i=0; i < 12; i++)
	{
	    uint32_t curdirectblock = 0;
	    ReadForImgContent(currentitem->forimg, &curdirectblock, currentoffset + 40 + i*4);
	    if(curdirectblock > 0)
		blocklist->push_back(curdirectblock);
	}
	uint32_t singleindirect = 0;
	ReadForImgContent(currentitem->forimg, &singleindirect, currentoffset + 88);
	uint32_t doubleindirect = 0;
	ReadForImgContent(currentitem->forimg, &doubleindirect, currentoffset + 92);
	uint32_t tripleindirect = 0;
	ReadForImgContent(currentitem->forimg, &tripleindirect, currentoffset + 96);
	if(singleindirect > 0)
	{
	    for(uint i=0; i < blocksize / 4; i++)
	    {
		uint32_t cursingledirect = 0;
		ReadForImgContent(currentitem->forimg, &cursingledirect, currentoffset + singleindirect * blocksize + i*4);
		if(cursingledirect > 0)
		    blocklist->push_back(cursingledirect);
	    }
	}
	if(doubleindirect > 0)
	{
	    std::vector<uint32_t> sinlist;
	    sinlist.clear();
	    for(uint i=0; i < blocksize / 4; i++)
	    {
		uint32_t sindirect = 0;
		ReadForImgContent(currentitem->forimg, &sindirect, currentoffset + doubleindirect * blocksize + i*4);
		if(sindirect > 0)
		    sinlist.push_back(sindirect);
	    }
	    for(int i=0; i < sinlist.size(); i++)
	    {
		for(uint j=0; j < blocksize / 4; j++)
		{
		    uint32_t sdirect = 0;
		    ReadForImgContent(currentitem->forimg, &sdirect, currentoffset + sinlist.at(i) * blocksize + j*4);
		    if(sdirect > 0)
			blocklist->push_back(sdirect);
		}
	    }
	    sinlist.clear();
	}
	if(tripleindirect > 0)
	{
	    std::vector<uint32_t> dinlist;
	    std::vector<uint32_t> sinlist;
	    dinlist.clear();
	    sinlist.clear();
	    for(uint i=0; i < blocksize / 4; i++)
	    {
		uint32_t dindirect = 0;
		ReadForImgContent(currentitem->forimg, &dindirect, currentoffset + tripleindirect * blocksize + i*4);
		if(dindirect > 0)
		    dinlist.push_back(dindirect);
	    }
	    for(int i=0; i < dinlist.size(); i++)
	    {
		for(uint j=0; j < blocksize / 4; j++)
		{
		    uint32_t sindirect = 0;
		    ReadForImgContent(currentitem->forimg, &sindirect, currentoffset + dinlist.at(i) * blocksize + j*4);
		    if(sindirect > 0)
			sinlist.push_back(sindirect);
		}
		for(int j=0; j < sinlist.size(); j++)
		{
		    for(uint k=0; k < blocksize / 4; k++)
		    {
			uint32_t sdirect = 0;
			ReadForImgContent(currentitem->forimg, &sdirect, currentoffset + sinlist.at(j) * blocksize + k*4);
			if(sdirect > 0)
			    blocklist->push_back(sdirect);
		    }
		}
	    }
	}
    }
}

std::string ConvertBlocksToExtents(std::vector<uint32_t>* blocklist, uint32_t blocksize)
{
    std::string layout = "";
    int blockcount = 1;
    unsigned int startvalue = blocklist->at(0);
    for(int i=1; i < blocklist->size(); i++)
    {
	unsigned int oldvalue = blocklist->at(i-1);
	unsigned int newvalue = blocklist->at(i);
	if(newvalue - oldvalue == 1)
	    blockcount++;
	else
	{
	    layout += std::to_string(startvalue * blocksize) + "," + std::to_string(blockcount * blocksize) + ";";
	    startvalue = blocklist->at(i);
	    blockcount = 1;
	}
	if(i == blocklist->size() - 1)
	{
	    layout += std::to_string(startvalue * blocksize) + "," + std::to_string(blockcount * blocksize) + ";";
	    startvalue = blocklist->at(i);
	    blockcount = 1;
	}
    }
    if(blocklist->size() == 1)
    {
	layout += std::to_string(startvalue * blocksize) + "," + std::to_string(blockcount * blocksize) + ";";
    }

    return layout;
}
