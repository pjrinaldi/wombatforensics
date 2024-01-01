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
    mftentryoffset = mftoffset + relativentinode * mftentrysize * bytespercluster;
    std::cout << "mftentryoffset: " << mftentryoffset << std::endl;
}
