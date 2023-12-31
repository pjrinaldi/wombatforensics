#include "ntfs.h"

void LoadNtfsDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem)
{
}

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
