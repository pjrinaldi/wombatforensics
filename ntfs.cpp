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
    // GET MFT LAYOUT TO THEN GET THE DIRECTORY CONTENTS TO PARSE
    std::string mftlayout = "";
    uint64_t mftsize = 0;
    GetDataAttributeLayout(currentitem->forimg, bytespercluster, mftentrybytes, mftstartingoffset, &mftlayout); 
    std::cout << "mft layout: " << mftlayout << std::endl;
    // CURRENT INODE
    uint64_t currentinode = 5;
    if(curfileitem != NULL)
    {
	std::size_t ntinodesplit = curfileitem->properties.find(">");
	currentinode = std::stoull(curfileitem->properties.substr(0, ntinodesplit));
    }
    std::cout << "curitem nt inode: " << currentinode << std::endl;
    // GET DIRECTORY'S MFT ENTRY
    std::vector<std::string> mftlayoutlist;
    mftlayoutlist.clear();
    std::istringstream mll(mftlayout);
    std::string mls;
    uint64_t mftentryoffset = 0;
    uint64_t relativentinode = currentinode;
    uint64_t mftoffset = 0;
    uint64_t mftlength = 0;
    while(getline(mll, mls, ';'))
        mftlayoutlist.push_back(mls);
    // MFT MAX ENTRY COUNT && MFT SIZE
    for(int i=0; i < mftlayoutlist.size(); i++)
    {
	std::size_t layoutsplit = mftlayoutlist.at(i).find(",");
	mftsize += std::stoull(mftlayoutlist.at(i).substr(layoutsplit+1));
    }
    std::cout << "MFT Size: " << mftsize << std::endl;
    uint64_t maxmftentrycount = mftsize / mftentrybytes;
    std::cout << "Max MFT Entry Count: " << maxmftentrycount << std::endl;
    for(int i=0; i < mftlayoutlist.size(); i++)
    {
        std::size_t layoutsplit = mftlayoutlist.at(i).find(",");
        mftoffset = std::stoull(mftlayoutlist.at(i).substr(0, layoutsplit));
        mftlength = std::stoull(mftlayoutlist.at(i).substr(layoutsplit+1));
        uint64_t curmaxntinode = mftlength / (mftentrysize * bytespercluster);
        if(relativentinode < curmaxntinode)
            break;
        else
            relativentinode = relativentinode - curmaxntinode;
    }
    // DIRECTORIES MFT ENTRY OFFSET TO THE CONTENT
    mftentryoffset = mftoffset + relativentinode * mftentrysize * bytespercluster;
    std::cout << "relativentinode: " << relativentinode << std::endl;
    std::cout << "mftentryoffset: " << mftentryoffset << std::endl;
    std::string indexlayout = "";
    GetIndexAttributeLayout(currentitem->forimg, bytespercluster, mftentrybytes, mftentryoffset, &indexlayout); 
    std::cout << "index layout: " << indexlayout << std::endl;
    // PARSE INDEX ROOT AND ALLOCATION TO DETERMINE THE DIR/FILE NAME/INODE AND SEE IF THEY MATCH
    std::vector<std::string> indexlayoutlist;
    indexlayoutlist.clear();
    std::istringstream ill(indexlayout);
    std::string ils;
    while(getline(ill, ils, ';'))
        indexlayoutlist.push_back(ils);
    uint32_t indexrecordsize = 0;
    for(int i=0; i < indexlayoutlist.size(); i++)
    {
        std::size_t layoutsplit = indexlayoutlist.at(i).find(",");
        uint64_t indexoffset = std::stoull(indexlayoutlist.at(i).substr(0, layoutsplit));
        uint64_t indexlength = std::stoull(indexlayoutlist.at(i).substr(layoutsplit+1));
	if(i == 0) // $INDEX_ROOT
	{
	    // INDEX RECORD SIZE
	    ReadForImgContent(currentitem->forimg, &indexrecordsize, indexoffset + 8);
	    std::cout << "Index Record Size: " << indexrecordsize << std::endl;
	    // STARTING OFFSET
	    uint32_t startingoffset = 0;
	    ReadForImgContent(currentitem->forimg, &startingoffset, indexoffset + 16);
	    // ENDING OFFSET
	    uint32_t endingoffset = 0;
	    ReadForImgContent(currentitem->forimg, &endingoffset, indexoffset + 20);
            /*
            // ALLOCATION OFFSET - DELETED ENTRIES
            uint32_t allocationoffset = 0;
	    ReadForImgContent(currentitem->forimg, &allocationoffset, indexoffset + 24);
            */
	    uint32_t curpos = startingoffset;
	    while(curpos < endingoffset)
	    {
		// INDEX ENTRY LENGTH
		uint16_t indexentrylength = 0;
		ReadForImgContent(currentitem->forimg, &indexentrylength, indexoffset + 16 + curpos + 8);
                //std::cout << "Index Entry Length: " << indexentrylength << std::endl;
		// FILE NAME ATTRIBUTE LENGTH
		uint16_t filenamelength = 0;
		ReadForImgContent(currentitem->forimg, &filenamelength, indexoffset + 16 + curpos + 10);
                //std::cout << "File Name Attribute Length: " << filenamelength << std::endl;
		// INDEX ENTRY FLAGS
		uint32_t indexentryflags = 0;
		ReadForImgContent(currentitem->forimg, &indexentryflags, indexoffset + 16 + curpos + 12);
                //std::cout << "Index Entry Flags: 0x" << std::hex << indexentryflags << std::dec << std::endl;
		if(indexentryflags & 0x02)
		    break;
		else
		{
		    if(indexentrylength > 0 && filenamelength > 66 && filenamelength < indexentrylength)
		    {
			// I30 SEQUENCE ID
			uint16_t i30sequenceid = 0;
			ReadForImgContent(currentitem->forimg, &i30sequenceid, indexoffset + 16 + curpos + 6);
                        //std::cout << "I30 Sequence ID: " << i30sequenceid << std::endl;
			// CHILD NT INODE
			uint8_t* cni = new uint8_t[6];
			uint64_t childntinode = 0;
			currentitem->forimg->ReadContent(cni, indexoffset + 16 + curpos, 6);
			ReturnUint(&childntinode, cni, 6); 
			delete[] cni;
			childntinode = childntinode & 0x00ffffffffffffff;
			//std::cout << "Child NT Inode: " << childntinode << std::endl;
			if(childntinode <= maxmftentrycount)
			{
			    curpos = curpos + 16; // STARTING ON FILE_NAME ATTRIBUTE
			    // FILE NAME TYPE
			    uint8_t fntype = 0;
			    currentitem->forimg->ReadContent(&fntype, indexoffset + 16 + curpos + 65, 1);
			    //std::cout << "File name type: " << (uint)fntype << std::endl;
			    if(fntype != 0x02)
			    {
				// FILE NAME LENGTH
				uint8_t namelength = 0;
				currentitem->forimg->ReadContent(&namelength, indexoffset + 16 + curpos + 64, 1);
                                //std::cout << "name length: " << (int)namelength << std::endl;
				// FILE NAME
				std::string filename = "";
				for(int j=0; j < namelength; j++)
				{
				    uint16_t singleletter = 0;
				    ReadForImgContent(currentitem->forimg, &singleletter, indexoffset + 16 + curpos + 66 + j*2);
				    filename += (char)singleletter;
				}
				std::cout << "File Name: " << filename << std::endl;
				/*
                                if(filename.compare(childpath) == 0)
                                    return childntinode;
				*/
			    }
			}
		    }
		}
		curpos += indexentrylength;
	    }
	}
	else // $INDEX_ALLOCATION
	{
	    std::cout << "Index Record Size: " << indexrecordsize << std::endl;
	    uint32_t indexrecordcount = indexlength / indexrecordsize;
            std::cout << "Index Record Count: " << indexrecordcount << std::endl;
	    uint64_t curpos = indexoffset;
	    for(uint32_t j=0; j < indexrecordcount; j++)
	    {
                // START OFFSET
		uint32_t startoffset = 0;
		ReadForImgContent(currentitem->forimg, &startoffset, curpos + 24);
                //std::cout << "Start Offset: " << startoffset << std::endl;
                // END OFFSET
                uint32_t endoffset = 0;
		ReadForImgContent(currentitem->forimg, &endoffset, curpos + 28);
                //std::cout << "End Offset: " << endoffset << std::endl;
                curpos = curpos + 24 + startoffset + j*indexrecordsize;
		std::cout << "before while loop - curpos: " << curpos << " indexsize: " << indexoffset + j*indexrecordsize + indexrecordsize << std::endl;
                while(curpos < indexoffset + j*indexrecordsize + indexrecordsize)
                {
		    //std::cout << "Cur Pos: " << curpos << std::endl;
		    // INDEX ENTRY LENGTH
		    uint16_t indexentrylength = 0;
		    ReadForImgContent(currentitem->forimg, &indexentrylength, curpos + 8);
		    //std::cout << "Index Entry Length: " << indexentrylength << std::endl;
		    // FILE NAME ATTRIBUTE LENGTH
		    uint16_t filenamelength = 0;
		    ReadForImgContent(currentitem->forimg, &filenamelength, curpos + 10);
		    //std::cout << "File Name Attribute Length: " << filenamelength << std::endl;
		    if(indexentrylength > 0 && filenamelength > 66 && filenamelength < indexentrylength)
		    {
			// I30 SEQUENCE ID
			uint16_t i30sequenceid = 0;
			ReadForImgContent(currentitem->forimg, &i30sequenceid, curpos + 6);
			//std::cout << "I30 Sequence ID: " << i30sequenceid << std::endl;
			// CHILD NT INODE
			uint8_t* cni = new uint8_t[6];
			uint64_t childntinode = 0;
			currentitem->forimg->ReadContent(cni, curpos, 6);
			ReturnUint(&childntinode, cni, 6);
			delete[] cni;
			childntinode = childntinode & 0x00ffffffffffffff;
			//std::cout << "Child NT Inode: " << childntinode << std::endl;
			if(childntinode <= maxmftentrycount)
			{
                            curpos = curpos + 16; // STARTING ON FILE_NAME ATTRIBUTE
                            // FILE NAME TYPE
                            uint8_t fntype = 0;
			    currentitem->forimg->ReadContent(&fntype, curpos + 65, 1);
                            //std::cout << "file name type: " << (uint)fntype << std::endl;
                            if(fntype != 0x02)
                            {
                                // FILE NAME LENGTH
                                uint8_t namelength = 0;
				currentitem->forimg->ReadContent(&namelength, curpos + 64, 1);
                                //std::cout << "name length: " << (int)namelength << std::endl;
                                // FILE NAME
                                std::string filename = "";
                                for(uint8_t k=0; k < namelength; k++)
                                {
                                    uint16_t singleletter = 0;
				    ReadForImgContent(currentitem->forimg, &singleletter, curpos + 66 + k*2);
				    filename += (char)singleletter;
				}
                                std::cout << "File Name: " << filename << std::endl;
			    }
			}
		    }
		    curpos += indexentrylength - 16;
		}
	    }
	}
    }
}
