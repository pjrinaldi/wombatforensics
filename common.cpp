#include "common.h"
// SVG TO PNG HEADERS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvgrast.h"

/*
std::string ConvertUnixTimeToHuman(uint32_t unixtime)
{
    time_t timet = (time_t)unixtime;
    struct tm* tmtime = gmtime(&timet);
    char hchar[100];
    strftime(hchar, 100, "%m/%d/%Y %I:%M:%S %p UTC", tmtime);
    
    return std::string(hchar);
}
*/
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

std::string ConvertWindowsTimeToUnixTimeUTC(uint64_t input)
{
    uint64_t temp;
    temp = input / TICKS_PER_SECOND; //convert from 100ns intervals to seconds;
    temp = temp - EPOCH_DIFFERENCE;  //subtract number of seconds between epochs
    time_t crtimet = (time_t)temp;
    struct tm* dt;
    dt = gmtime(&crtimet);
    char timestr[30];
    strftime(timestr, sizeof(timestr), "%m/%d/%Y %I:%M:%S %p UTC", dt);

    return timestr;
}

void ConvertSvgToPng(std::string* svgfilestr)
{
    std::string pngfilestr = *svgfilestr + ".png";
    std::cout << "raste: " << pngfilestr << std::endl;
    NSVGimage* image = NULL;
    NSVGrasterizer* rast = NULL;
    unsigned char* img = NULL;
    image = nsvgParseFromFile(svgfilestr->c_str(), "px", 96);
    int w = (int)image->width;
    int h = (int)image->height;
    rast = nsvgCreateRasterizer();
    img = new unsigned char[(w*h*4)];
    nsvgRasterize(rast, image, 0, 0, 1, img, w, h, w*4);
    stbi_write_png(pngfilestr.c_str(), w, h, 4, img, w*4);
    nsvgDeleteRasterizer(rast);
    nsvgDelete(image);
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

std::string ReturnFormattedGuid(uint8_t* guidbuffer)
{
    std::stringstream guidstream;
    guidstream << std::hex << std::setfill('0') << std::setw(2) << (uint)guidbuffer[3] << std::setw(2) << (uint)guidbuffer[2] << std::setw(2) << (uint)guidbuffer[1] << std::setw(2) << (uint)guidbuffer[0] << "-" << std::setw(2) << (uint)guidbuffer[5] << std::setw(2) << (uint)guidbuffer[4] << "-" << std::setw(2) << (uint)guidbuffer[7] << std::setw(2) << (uint)guidbuffer[6] << "-" << std::setw(2) << (uint)guidbuffer[8] << std::setw(2) << (uint)guidbuffer[9] << "-" << std::setw(2) << (uint)guidbuffer[10] << std::setw(2) << (uint)guidbuffer[11] << std::setw(2) << (uint)guidbuffer[12] << std::setw(2) << (uint)guidbuffer[13] << std::setw(2) << (uint)guidbuffer[14] << std::setw(2) << (uint)guidbuffer[15];

    return guidstream.str();
}

/*
void ReadContent(std::ifstream* rawcontent, uint8_t* tmpbuf, uint64_t offset, uint64_t size)
{
    rawcontent->seekg(offset);
    rawcontent->read((char*)tmpbuf, size);
}
*/

/*
void ReadContent(std::ifstream* rawcontent, int8_t* tmpbuf, uint64_t offset, uint64_t size)
{
    rawcontent->seekg(offset);
    rawcontent->read((char*)tmpbuf, size);
}
*/

/*
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
*/

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

/*
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
            curcluster16 = 0;
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

std::string ConvertBlocksToExtents(std::vector<uint>* blocklist, uint32_t blocksize, uint64_t rootdiroffset)
{
    std::string layout = "";
    int blkcnt = 1;
    uint startvalue = blocklist->at(0);
    for(int i=1; i < blocklist->size(); i++)
    {
        uint oldvalue = blocklist->at(i-1);
        uint newvalue = blocklist->at(i);
        if(newvalue - oldvalue == 1)
            blkcnt++;
        else
        {
            if(rootdiroffset > 0)
                layout += std::to_string(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + std::to_string(blkcnt * blocksize) + ";";
            else
                layout += std::to_string(startvalue * blocksize) + "," + std::to_string(blkcnt * blocksize) + ";";
            startvalue = blocklist->at(i);
        }
        if(i == blocklist->size() - 1)
        {
            if(rootdiroffset > 0)
                layout += std::to_string(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + std::to_string(blkcnt * blocksize) + ";";
            else
                layout += std::to_string(startvalue * blocksize) + "," + std::to_string(blkcnt * blocksize) + ";";
            startvalue = blocklist->at(i);
            blkcnt = 1;
        }
    }
    if(blocklist->size() == 1)
    {
        if(rootdiroffset > 0)
            layout += std::to_string(rootdiroffset + ((startvalue - 2) * blocksize)) + "," + std::to_string(blkcnt * blocksize) + ";";
        else
            layout += std::to_string(startvalue * blocksize) + "," + std::to_string(blkcnt * blocksize) + ";";
    }

    return layout;
}

void GetFileContent(ForImg* curforimg, FileItem* curfileitem, bool* inmemory, uint8_t** tmpbuffer, FILE* tmpfile)
{
    //std::cout << "GetFileContent() called to generate tmpfile." << std::endl;
    uint64_t memlimit = 4294967296; // 4GB
    /*
    bool inmemory = true;
    //uint64_t memlimit = 1; // 1 byte for testing
    std::cout << "name: " << curfileitem->name << std::endl;
    std::cout << "layout: " << curfileitem->layout << std::endl;
    std::cout << "size: " << curfileitem->size << std::endl;
    std::cout << "sig: " << curfileitem->sig << std::endl;
    std::cout << "curforimg: " << curforimg->ImageFileName() << std::endl;
    */

    //if(curfileitem->size < memlimit)
    //    *inmemory = true;
    if((curfileitem->sig.compare("Shortcut") == 0 || curfileitem->sig.compare("Recycler") == 0 || curfileitem->sig.compare("Recycle.Bin") == 0 || curfileitem->sig.compare("Prefetch") == 0) && curfileitem->size < memlimit)
        *inmemory = true;
    std::vector<std::string> layoutlist;
    layoutlist.clear();
    std::istringstream layoutstream(curfileitem->layout);
    std::string curlayout;
    while(getline(layoutstream, curlayout, ';'))
        layoutlist.push_back(curlayout);
    uint64_t curlogicalsize = 0;
    uint8_t* tmpbuf = NULL;
    //std::string tmpfilestr = "";
    std::string tmpfilestr = "/tmp/wf/" + std::to_string(curfileitem->gid) + "-" + curfileitem->name + ".tmp";
    tmpfilestr.erase(std::remove(tmpfilestr.begin(), tmpfilestr.end(), '$'), tmpfilestr.end());
    if(*inmemory) // store in memory
        tmpbuf = new uint8_t[curfileitem->size];
    if(!*inmemory)
    {
        //tmpbuf = new uint8_t[524288];
        //std::cout << "tmpfilestr: " << tmpfilestr << std::endl;
        tmpfile = fopen(tmpfilestr.c_str(), "w+");
    }
    uint64_t curpos = 0;
    for(int i=0; i < layoutlist.size(); i++)
    {
        std::size_t layoutsplit = layoutlist.at(i).find(",");
        uint64_t curoffset = std::stoull(layoutlist.at(i).substr(0, layoutsplit));
        uint64_t cursize = std::stoull(layoutlist.at(i).substr(layoutsplit+1));
        //std::cout << "curoffset: " << curoffset << " cursize: " << cursize << std::endl;

        uint8_t* inbuf = NULL;
        curlogicalsize += cursize;
        //std::cout << "curlogicalsize: " << curlogicalsize << "|" << cursize << " :cursize" << std::endl;
        if(curlogicalsize <= curfileitem->size)
        {
            inbuf = new uint8_t[cursize];
            curforimg->ReadContent(inbuf, curoffset, cursize);
            if(*inmemory)
                memcpy(&tmpbuf[curpos], inbuf, cursize);
            else
                fwrite(inbuf, 1, cursize, tmpfile);
            curpos += cursize;
        }
        else
        {
            inbuf = new uint8_t[(cursize - (curlogicalsize - curfileitem->size))];
            curforimg->ReadContent(inbuf, curoffset, (cursize - (curlogicalsize - curfileitem->size)));
            //std::cout << "reduction size: " << cursize - (curlogicalsize - curfileitem->size) << std::endl;
            //std::cout << "inbuf head: " << (char)inbuf[0] << (char)inbuf[1] << std::endl;
            if(*inmemory)
                memcpy(&tmpbuf[curpos], inbuf, (cursize - (curlogicalsize - curfileitem->size)));
            else
                fwrite(inbuf, 1, (cursize - (curlogicalsize - curfileitem->size)), tmpfile);
            curpos += cursize - (curlogicalsize - curfileitem->size);
        }
        delete[] inbuf;
    }
    if(!*inmemory)
        fclose(tmpfile);
    //std::cout << "tmpbuf in function: " << tmpbuf[0] << tmpbuf[1] << std::endl;
    if(*inmemory)
	*tmpbuffer = tmpbuf;
    //delete[] tmpbuf;
}

void GetPreviewContent(ForImg* curforimg, FileItem* curfileitem, uint8_t** prebuf, uint64_t bufsize)
{
    uint8_t* tmpbuf = new uint8_t[bufsize];
    uint64_t curlogicalsize = 0;
    uint64_t curpos = 0;
    std::vector<std::string> layoutlist;
    layoutlist.clear();
    std::istringstream layoutstream(curfileitem->layout);
    std::string curlayout;
    while(getline(layoutstream, curlayout, ';'))
        layoutlist.push_back(curlayout);
    for(int i=0; i < layoutlist.size(); i++)
    {
        std::size_t layoutsplit = layoutlist.at(i).find(",");
        uint64_t curoffset = std::stoull(layoutlist.at(i).substr(0, layoutsplit));
        uint64_t cursize = std::stoull(layoutlist.at(i).substr(layoutsplit+1));
        curlogicalsize += cursize;
        uint8_t* inbuf = NULL; 
        if(curlogicalsize < bufsize)
        {
            inbuf = new uint8_t[cursize];
            curforimg->ReadContent(inbuf, curoffset, cursize);
            memcpy(&tmpbuf[curpos], inbuf, cursize);
            curpos += cursize;
        }
        else
        {
            inbuf = new uint8_t[(cursize - (curlogicalsize - bufsize))];
            curforimg->ReadContent(inbuf, curoffset, (cursize - (curlogicalsize - bufsize)));
            memcpy(&tmpbuf[curpos], inbuf, (cursize - (curlogicalsize - bufsize)));
            curpos += cursize - (curlogicalsize - bufsize);
        }
        delete[] inbuf;
    }
    *prebuf = tmpbuf;
    delete[] tmpbuf;
}

void GetFileSlack(ForImg* curforimg, FileItem* curfileitem, uint8_t** tmpbuf, uint64_t* slacksize)
{
    std::vector<std::string> layoutlist;
    layoutlist.clear();
    std::istringstream layoutstream(curfileitem->layout);
    std::string curlayout;
    while(getline(layoutstream, curlayout, ';'))
        layoutlist.push_back(curlayout);
    uint64_t physicalsize = 0;
    uint64_t lastoffset = 0;
    for(int i=0; i < layoutlist.size(); i++)
    {
	std::size_t layoutsplit = layoutlist.at(i).find(",");
	uint64_t curoffset = std::stoull(layoutlist.at(i).substr(0, layoutsplit));
	uint64_t cursize = std::stoull(layoutlist.at(i).substr(layoutsplit+1));
	lastoffset = curoffset + cursize;
	physicalsize += cursize;
    }
    *slacksize = physicalsize - curfileitem->size;
    uint64_t slackoffset = lastoffset - *slacksize;
    *tmpbuf = new uint8_t[*slacksize];
    curforimg->ReadContent(*tmpbuf, slackoffset, *slacksize);
}

void AddFileSlack(ForImg* curforimg, FileItem* curfileitem, uint8_t* slkbuf, std::string* filecontents)
{

}

void HashFile(FileItem* curfileitem, ForImg* curforimg)
{
    //std::cout << "hash it here..." << std::endl;
    //std::cout << "curfileitem layout: " << curfileitem->layout << std::endl;

    bool inmemory = false;
    uint8_t* tmpbuf = NULL;
    FILE* tmpfile = NULL;
    //std::string tmpfilestr = "/tmp/wf/" + curfileitem->name + "-" + std::to_string(curfileitem->gid) + ".tmp";
    std::string tmpfilestr = "/tmp/wf/" + std::to_string(curfileitem->gid) + "-" + curfileitem->name + ".tmp";
    tmpfilestr.erase(std::remove(tmpfilestr.begin(), tmpfilestr.end(), '$'), tmpfilestr.end());
    if(!std::filesystem::exists(tmpfilestr))
        GetFileContent(curforimg, curfileitem, &inmemory, &tmpbuf, tmpfile);

    blake3_hasher hasher;
    blake3_hasher_init(&hasher);
    blake3_hasher_update(&hasher, (char*)tmpbuf, curfileitem->size);
    uint8_t output[BLAKE3_OUT_LEN];
    blake3_hasher_finalize(&hasher, output, BLAKE3_OUT_LEN);
    std::stringstream ss;
    for(int i=0; i < BLAKE3_OUT_LEN; i++)
        ss << std::hex << (int)output[i];
    curfileitem->hash = ss.str();
    delete[] tmpbuf;
}

void ThumbnailImage(ForImg* curforimg, FileItem* curfileitem, int thumbsize, std::string tmppath)
{
    bool inmemory = false;
    uint8_t* tmpbuf = NULL;
    //std::string tmpfilestr = "/tmp/wf/" + curfileitem->name + "-" + std::to_string(curfileitem->gid) + ".tmp";
    std::string thumbfilestr = tmppath + "imgthumbs/" + std::to_string(curfileitem->gid) + "-" + curfileitem->name + ".png";
    thumbfilestr.erase(std::remove(thumbfilestr.begin(), thumbfilestr.end(), '$'), thumbfilestr.end());
    //std::cout << "tmpfilestr: " << tmpfilestr << " thumbfilestr: " << thumbfilestr << std::endl;
    Magick::Image imgexists;
    bool thumbexists = false;
    try // attempt to open existing thumbnail
    {
	imgexists.read(thumbfilestr);
	thumbexists = true;
    }
    catch(Magick::Exception &error) // thumbnail doesn't exist
    {
	//std::cout << "Error: " << error.what() << std::endl;
    }
    //std::cout << "thumb file str: " << thumbfilestr << std::endl;
    FILE* tmpfile;
    std::string tmpfilestr = "/tmp/wf/" + std::to_string(curfileitem->gid) + "-" + curfileitem->name + ".tmp";
    tmpfilestr.erase(std::remove(tmpfilestr.begin(), tmpfilestr.end(), '$'), tmpfilestr.end());
    if(!std::filesystem::exists(tmpfilestr))
        GetFileContent(curforimg, curfileitem, &inmemory, &tmpbuf, tmpfile);
    Magick::Geometry thumbgeometry(thumbsize, thumbsize);
    if(thumbexists && (imgexists.size().width() == thumbsize || imgexists.size().height() == thumbsize))
    {
	// MOVE CATCH ERRORS TO THE LOGFILE AND MSGLOG DISPLAY
	std::cout << "Thumbnail exists for " << curfileitem->name << "-" << std::to_string(curfileitem->gid) << ". skipping." << std::endl;
    }
    else
    {
	if(curfileitem->size > 0)
	{
	    try
	    {
		Magick::Blob inblob(tmpbuf, curfileitem->size);
		Magick::Image master(inblob);
		master.quiet(false);
		master.resize(thumbgeometry);
		master.magick("PNG");
		master.write(thumbfilestr);
	    }
	    catch(Magick::Exception &error)
	    {
		// MOVE CATCH ERRORS TO THE LOGFILE AND MSGLOG DISPLAY
		std::cout << "error encountered for: " << curfileitem->name + "-" + std::to_string(curfileitem->gid) << ": " << error.what() << std::endl;
		try
		{
		    Magick::Image inimage("/tmp/wf/mt.png");
		    inimage.quiet(false);
		    inimage.resize(thumbgeometry);
		    inimage.magick("PNG");
		    inimage.write(thumbfilestr);
		}
		catch(Magick::Exception &error)
		{
		    // MOVE CATCH ERRORS TO THE LOGFILE AND MSGLOG DISPLAY
		    std::cout << "File: " << tmpfilestr << " magick error: " << error.what() << std::endl;
		}
	    }
	}
	else // file size is zero
	{
	    try
	    {
		Magick::Image inimage("/tmp/wf/mt.png");
		inimage.quiet(false);
		inimage.resize(thumbgeometry);
		inimage.magick("PNG");
		inimage.write(thumbfilestr);
	    }
	    catch(Magick::Exception &error)
	    {
		// MOVE CATCH ERRORS TO THE LOGFILE AND MSGLOG DISPLAY
		std::cout << "File: " << tmpfilestr << " magick error:" << error.what() << std::endl;
	    }
	}
    }
}

void ThumbnailVideo(ForImg* curforimg, FileItem* curfileitem, int thumbsize, int thumbcount, std::string tmppath)
{
    bool inmemory = false;
    uint8_t* tmpbuf = NULL;
    //std::string tmpfilestr = "/tmp/wf/" + curfileitem->name + "-" + std::to_string(curfileitem->gid) + ".tmp";
    std::string thumbfilestr = tmppath + "vidthumbs/" + std::to_string(curfileitem->gid) + "-" + curfileitem->name + ".png";
    thumbfilestr.erase(std::remove(thumbfilestr.begin(), thumbfilestr.end(), '$'), thumbfilestr.end());
    //std::cout << "thumb file str: " << thumbfilestr << std::endl;
    Magick::Image imgexists;
    bool thumbexists = false;
    try // attempt to open existing thumbnail
    {
	imgexists.read(thumbfilestr);
	thumbexists = true;
    }
    catch(Magick::Exception &error)
    {
    }
    FILE* tmpfile;
    std::string tmpfilestr = "/tmp/wf/" + std::to_string(curfileitem->gid) + "-" + curfileitem->name + ".tmp";
    tmpfilestr.erase(std::remove(tmpfilestr.begin(), tmpfilestr.end(), '$'), tmpfilestr.end());
    if(!std::filesystem::exists(tmpfilestr))
        GetFileContent(curforimg, curfileitem, &inmemory, &tmpbuf, tmpfile);
    if(inmemory)
    {
	std::ofstream sfile(tmpfilestr.c_str(), std::ios::binary);
	sfile.seekp(0, std::ios::beg);
	sfile.write((char*)tmpbuf, curfileitem->size);
    }
    delete[] tmpbuf;
    Magick::Geometry thumbgeometry(thumbsize, thumbsize);
    if(thumbexists && (imgexists.size().width() == thumbsize || imgexists.size().height() == thumbsize))
    {
	// MOVE CATCH ERRORS TO THE LOGFILE AND MSGLOG DISPLAY
	std::cout << "Thumbnail exists for " << curfileitem->name << "-" << std::to_string(curfileitem->gid) << ". skipping." << std::endl;
    }
    else
    {
        if(curfileitem->size > 0)
        {
            std::vector<std::string> tlist;
            tlist.clear();
            try
            {
                ffmpegthumbnailer::VideoThumbnailer videothumbnailer(0, true, true, 8, false);
                videothumbnailer.setThumbnailSize(thumbsize);
                std::unique_ptr<ffmpegthumbnailer::FilmStripFilter> filmstripfilter;
		filmstripfilter.reset(new ffmpegthumbnailer::FilmStripFilter());
                videothumbnailer.addFilter(filmstripfilter.get());
                videothumbnailer.setPreferEmbeddedMetadata(false);
                int vtcnt = 100 / thumbcount;
		//std::cout << "vtcnt: " << vtcnt << std::endl;
                for(int i=0; i < vtcnt; i++)
                {
                    int seekpercentage = i * thumbcount;
                    if(seekpercentage == 0)
                        seekpercentage = 5;
                    if(seekpercentage == 100)
                        seekpercentage = 95;
		    //std::cout << "seek percentage: " << seekpercentage << std::endl;
                    std::string tmpoutfile = "/tmp/wf/" + std::to_string(curfileitem->gid) + ".t" + std::to_string(seekpercentage) + ".png";
                    tlist.push_back(tmpoutfile);
                    videothumbnailer.setSeekPercentage(seekpercentage);
		    videothumbnailer.generateThumbnail(tmpfilestr, Png, tmpoutfile);
                }
		try
		{
		    std::list<Magick::Image> thumbimages;
		    std::list<Magick::Image> montage;
		    Magick::Image image;
		    for(int i=0; i < tlist.size(); i++)
		    {
			image.read(tlist.at(i));
			thumbimages.push_back(image);
		    }
		    Magick::Montage montageoptions;
		    Magick::Color color("rgba(0,0,0,0)");
		    montageoptions.geometry(std::to_string(thumbsize) + "x" + std::to_string(thumbsize) + "+1+1");
		    montageoptions.tile(std::to_string(tlist.size()) + "x1");
		    montageoptions.backgroundColor(color);
		    montageoptions.fileName(thumbfilestr);
		    Magick::montageImages(&montage, thumbimages.begin(), thumbimages.end(), montageoptions);
		    if(montage.size() == 1)
		    {
			Magick::Image& montageimage = montage.front();
			montageimage.magick("png");
			montageimage.write(thumbfilestr);
		    }
		    else
		    {
			std::cout << curfileitem->name << "-" << std::to_string(curfileitem->gid) << "issue with montage" << montage.size() << ". Missing video thumbnail will be used.";
			try
			{
			    Magick::Image vimage("/tmp/wf/ve.png");
			    vimage.quiet(false);
			    vimage.resize(thumbgeometry);
			    vimage.magick("PNG");
			    vimage.write(thumbfilestr);
			}
			catch(Magick::Exception &error)
			{
			    std::cout << "File: " << curfileitem->name << "-" << std::to_string(curfileitem->gid) << " error: " << error.what() << std::endl;
			}
		    }
		}
		catch(Magick::Exception &error)
		{
		    try
		    {
			Magick::Image vimage("/tmp/wf/ve.png");
			vimage.quiet(false);
			vimage.resize(thumbgeometry);
			vimage.magick("PNG");
			vimage.write(thumbfilestr);
		    }
		    catch(Magick::Exception &error)
		    {
			std::cout << "File: " << curfileitem->name << "-" << std::to_string(curfileitem->gid) << " error: " << error.what() << std::endl;
		    }
		}
            }
            catch(std::exception &e)
            {
		try
		{
		    Magick::Image vimage("/tmp/wf/ve.png");
		    vimage.quiet(false);
		    vimage.resize(thumbgeometry);
		    vimage.magick("PNG");
		    vimage.write(thumbfilestr);
		}
		catch(Magick::Exception &error)
		{
		    std::cout << "File: " << curfileitem->name << "-" << std::to_string(curfileitem->gid) << " error: " << error.what() << std::endl;
		}
            }
        }
        else
        {
	    try
	    {
		Magick::Image vimage("/tmp/wf/ve.png");
		vimage.quiet(false);
		vimage.resize(thumbgeometry);
		vimage.magick("PNG");
		vimage.write(thumbfilestr);
	    }
	    catch(Magick::Exception &error)
	    {
		std::cout << "File: " << curfileitem->name << "-" << std::to_string(curfileitem->gid) << " error: " << error.what() << std::endl;
	    }
        }
    }
}

void GenerateCategorySignature(CurrentItem* currentitem, std::string* filename, std::string* layout, std::string* cat, std::string* sig)
{
    int found = layout->find(";");
    //std::cout << "; found at: " << found;
    std::string curlayout = layout->substr(0, found);
    std::size_t layoutsplit = curlayout.find(",");
    uint64_t curoffset = std::stoull(curlayout.substr(0, layoutsplit));
    uint64_t cursize = std::stoull(curlayout.substr(layoutsplit+1));
    uint64_t bufsize = 1024;
    if(cursize < 1024)
        bufsize = cursize;
    uint8_t* sigbuf = new uint8_t[bufsize + 1];
    currentitem->forimg->ReadContent(sigbuf, curoffset, bufsize);
    sigbuf[bufsize] = 0;
    magic_t magical;
    const char* catsig;
    magical = magic_open(MAGIC_MIME_TYPE);
    magic_load(magical, NULL);
    catsig = magic_buffer(magical, sigbuf, bufsize);
    std::string catsigstr(catsig);
    magic_close(magical);
    //std::cout << "cat/sig: " << catsigstr << std::endl;
    // CONVERT THE CAT/SIG TO CAPITALS
    for(int i=0; i < catsigstr.size(); i++)
    {
        if(i == 0 || catsigstr.at(i-1) == ' ' || catsigstr.at(i-1) == '-' || catsigstr.at(i-1) == '/')
            catsigstr[i] = std::toupper(catsigstr[i]);
    }
    std::size_t split = catsigstr.find("/");
    std::string tmpcat = catsigstr.substr(0, split);
    std::string tmpsig = catsigstr.substr(split+1);
    if(catsigstr.compare("Application/Octet-Stream") == 0)
    {
        if(sigbuf[0] == 0x72 && sigbuf[1] == 0x65 && sigbuf[2] == 0x67 && sigbuf[3] == 0x66) // 72 65 67 66 | regf
        {
            *cat = "Windows System";
            *sig = "Registry";
        }
        else if(sigbuf[0] == 0x4c && sigbuf[1] == 0x00 && sigbuf[2] == 0x00 && sigbuf[3] == 0x00 && sigbuf[4] == 0x01 && sigbuf[5] == 0x14 && sigbuf[6] == 0x02 && sigbuf[7] == 0x00) // LNK file
        {
            *cat = "Windows System";
            *sig = "Shortcut";
        }
        else if(filename->compare("INFO2") == 0 && (sigbuf[0] == 0x04 || sigbuf[0] == 0x05))
        {
            *cat = "Windows System";
            *sig = "Recycler";
        }
        else if(filename->find("$I") != std::string::npos && (sigbuf[0] = 0x01 || sigbuf[0] == 0x02))
        {
            *cat = "Windows System";
            *sig = "Recycle.Bin";
        }
        else if(filename->find(".pf") != std::string::npos && sigbuf[4] == 0x53 && sigbuf[5] == 0x43 && sigbuf[6] == 0x43 && sigbuf[7] == 0x41)
        {
            *cat = "Windows System";
            *sig = "Prefetch";
        }
        else if(filename->find(".pf") != std::string::npos & sigbuf[0] == 0x4d && sigbuf[1] == 0x41 && sigbuf[2] == 0x4d)
        {
            *cat = "Windows System";
            *sig = "Prefetch";
        }
        else if(sigbuf[0] == 0x44 && sigbuf[1] == 0x44 && sigbuf[2] == 0x53 && sigbuf[3] == 0x20)
        {
            *cat = "Image";
            *sig = "Dds";
        }
        else if(sigbuf[0] == 0x59 && sigbuf[1] == 0xa6 && sigbuf[2] == 0x6a && sigbuf[3] == 0x95)
        {
            *cat = "Image";
            *sig = "Ras";
        }
        else if(sigbuf[0] == 0x01 && sigbuf[1] == 0xda)
        {
            *cat = "Image";
            *sig = "Sgi";
        }
        else
        {
            *cat = "Application";
            *sig = "Octet-Stream";
        }
    }
    else if(catsigstr.compare("Application/X-Ms-Shortcut") == 0)
    {
        *cat = "Windows System";
        *sig = "Shortcut";
    }
    else if(catsigstr.compare("Application/Pdf") == 0)
    {
	*cat = "Document";
	*sig = "Pdf";
    }
    else if(catsigstr.find("Image/Vnd.microsoft.icon") != std::string::npos)
    {
        *cat = "Image";
        *sig = "Ico";
    }
    else if(catsigstr.find("Image/X-Tga") != std::string::npos)
    {
        *cat = "Image";
        *sig = "Tga";
    }
    else if(catsigstr.find("Image/X-Pcx") != std::string::npos)
    {
        *cat = "Image";
        *sig = "Pcx";
    }
    else if(catsigstr.find("Image/X-Portable-Pixmap") != std::string::npos)
    {
        *cat = "Image";
        *sig = "Ppm";
    }
    else if(catsigstr.find("Image/Svg+xml") != std::string::npos)
    {
        *cat = "Image";
        *sig = "Svg";
    }
    else if(catsigstr.find("Image/X-Xpixmap") != std::string::npos)
    {
        *cat = "Image";
        *sig = "Xpm";
    }
    else if(catsigstr.find("Text/") != std::string::npos)
    {
        if(filename->find(".mbox") != std::string::npos)
        {
            *cat = "Email";
            *sig = "MBox";
        }
        else if(filename->find(".xbm") != std::string::npos)
        {
            *cat = "Image";
            *sig = "Xbm";
        }
        else
        {
            *cat = tmpcat;
            *sig = tmpsig;
        }

    }
    else if(catsigstr.find("/Zip") != std::string::npos)
    {
        bool isoffice = false;
        int officesize = 4096;
        if(cursize < officesize)
            officesize = cursize;
        uint8_t* officecheck = new uint8_t[officesize + 1];
        currentitem->forimg->ReadContent(officecheck, curoffset, officesize);
        officecheck[officesize] = 0;
        for(int i=4; i < officesize; i++)
        {
            if(officecheck[i-4] == 0x77 && officecheck[i-3] == 0x6f && officecheck[i-2] == 0x72 && officecheck[i-1] == 0x64 && officecheck[i] == 0x2f) // word/ (.docx)
            {
                isoffice = true;
                *cat = "Office Document";
                *sig = "Microsoft Word 2007+";
                break;
            }
            else if(officecheck[i-4] == 0x78 && officecheck[i-3] == 0x6c && officecheck[i-2] == 0x2f) // "xl/" (.xlsx)
            {
                isoffice = true;
                *cat = "Office Document";
                *sig = "Microsoft Excel 2007+";
                break;
            }
            else if(officecheck[i-4] == 0x70 && officecheck[i-3] == 0x70 && officecheck[i-2] == 0x74 && officecheck[i-1] == 0x2f) // "ppt/" (.pptx)
            {
                isoffice = true;
                *cat = "Office Document";
                *sig = "Microsoft Powerpoint 2007+";
                break;
            }
        }
        if(!isoffice)
        {
            *cat = "Archive";
            *sig = "Zip";
        }
    }
    else if(catsigstr.find("/Vnd.oasis.opendocument.text") != std::string::npos)
    {
        *cat = "Office Document";
        *sig = "Open Document Text";
    }
    else
    {
        *cat = tmpcat;
        *sig = tmpsig;
    }
    //std::cout << "cat/sig: " << catsigstr << std::endl;
}
/*

	    // might not need the full contents, just need more hex and see if it has the:
	    // word/ folder (0x776f72642f) for .docx
	    // xl/ folder (0x786c2f) for .xlsx
	    // ppt/ folder (0x7070742f) for .pptx

    //else if(filename.startsWith("$INDEX_ROOT:") || filename.startsWith("$DATA:") || filename.startWith("$INDEX_ALLOCATION:"))

    return mimestr;
    //return QString(mimecategory + "/" + mimesignature);
    // WILL NEED TO REIMPLEMENT ALL THE BELOW BASED ON THE NEW LIBMAGIC STUFF
    //if(mimesignature.contains("text"))
//	mimecategory = "Text";
//    else
//	mimecategory = "Unknown";

//    if(mimestr.isEmpty())
//        return QString(mimecategory + "/" + mimesignature);
//    else
//        return mimestr;

    QString mimestr = "";
    // maybe a problem with file size requested being larger than the actual file size..
    QByteArray sigbuf = curimg->ReadContent(fileoffset, 1024);
    qDebug() << filename << QString::fromStdString(sigbuf.left(6).toStdString());
    QMimeDatabase mimedb;
    const QMimeType mimetype = mimedb.mimeTypeForData(sigbuf);
    QString geniconstr = mimetype.genericIconName();
    QString mimesignature = mimetype.comment();
    //qDebug() << "filename:" << filename << "mimesignature:" << mimesignature;
    QString mimecategory = "";
    if(geniconstr.contains("document")) // Document
        mimecategory = "Document";
    else if(geniconstr.contains("video")) // Video
        if(mimesignature.contains("WPL"))
            mimecategory = "HTML";
        else
            mimecategory = "Video";
    else if(geniconstr.contains("image")) // Image
        mimecategory = "Image";
    else if(geniconstr.contains("package")) // Archive
        mimecategory = "Archive";
    else if(geniconstr.contains("font")) // Font
        mimecategory = "Font";
    else if(geniconstr.contains("text")) // Text
        if(mimesignature.contains("email") || mimesignature.contains("mail") || mimesignature.contains("reference to remote file"))
            mimecategory = "Email";
        else if(mimesignature.contains("html", Qt::CaseInsensitive))
            mimecategory = "HTML";
        else
            mimecategory = "Text";
    else if(geniconstr.contains("audio")) // Audio
        mimecategory = "Audio";
    else if(geniconstr.contains("spreadsheet")) // Office Spreadsheet
        mimecategory = "Spreadsheet";
    else if(geniconstr.contains("presentation")) // Office Presentation
        mimecategory = "Presentation";
    else if(geniconstr.contains("multipart")) // MultiPart
        mimecategory = "MultiPart";
    else if(geniconstr.contains("inode")) // Inode
        mimecategory = "Inode";
    else if(geniconstr.contains("model")) // Binary
        mimecategory = "Binary";
    else if(geniconstr.contains("application-x")) // Try iconName() database, java, document, text, image, executable, certificate, bytecode, library, Data, Trash, zerosize, 
    {
       if(mimesignature.contains("certificate") || mimesignature.contains("private key") || mimesignature.contains("keystore")) 
            mimecategory = "Certificate";
       else if(mimesignature.contains("Metafile") || mimesignature.contains("profile"))
            mimecategory = "Metafile";
       else if(mimesignature.contains("video"))
           mimecategory = "Video";
       else if(mimesignature.contains("TNEF message") || mimesignature.contains("email"))
           mimecategory = "Email";
       else if(mimesignature.contains("Microsoft Word Document") || mimesignature.contains("OpenDocument Master Document Template") || mimesignature.contains("MIF"))
           mimecategory = "Document";
       else if(mimesignature.contains("ROM") || mimesignature.contains("Atari") || mimesignature.contains("Thomson"))
           mimecategory = "ROM";
       else if(mimesignature.contains("database") || mimesignature.contains("Database") || mimesignature.contains("SQL"))
           mimecategory = "Database";
       else if(mimesignature.contains("filesystem") || mimesignature.contains("disk image") || mimesignature.contains("AppImage") || mimesignature.contains("WiiWare"))
           mimecategory = "Disk Image";
       else if(mimesignature.contains("executable") || mimesignature.contains("Windows Intaller") || mimesignature.contains("library"))
           mimecategory = "Executable";
       else if(mimesignature.contains("Internet shortcut") || mimesignature.contains("backup file") || mimesignature.contains("VBScript") || mimesignature.contains("RDF") || mimesignature.contains("Streaming playlist") || mimesignature.contains("cache file") || mimesignature.contains("Remmina") || mimesignature.contains("GML") || mimesignature.contains("GPX") || mimesignature.contains("MathML") || mimesignature.contains("Metalink") || mimesignature.contains("XML") || mimesignature.contains("RDF") || mimesignature.contains("KML") || mimesignature.contains("FictionBook") || mimesignature.contains("NewzBin"))
           mimecategory = "Text";
       else if(mimesignature.contains("Windows animated cursor"))
           mimecategory = "Image";
       else if(mimesignature.contains("SPSS") || mimesignature.contains("MHTML"))
           mimecategory = "Archive";
       else if(mimesignature.contains("empty"))
           mimecategory = "Empty File";
       else
           mimecategory = "Binary";
    }
    else if(geniconstr.contains("x-content-x-generic")) 
    {
        if(mimesignature.contains("audio"))
            mimecategory = "Audio";
        else if(mimesignature.contains("blank"))
            mimecategory = "Disk Image";
        else if(mimesignature.contains("e-book"))
            mimecategory = "Document";
        else if(mimesignature.contains("photos") || mimesignature.contains("Picture"))
            mimecategory = "Image";
        else if(mimesignature.contains("software"))
            mimecategory = "Executable";
        else if(mimesignature.contains("video") || mimesignature.contains("Video"))
            mimecategory = "Video";
    }
    //if(orphanlist->at(j).value("itemtype").toUInt() == 2)
    //    mimestr = "Directory/Directory";
		    //else if(sigbuf.left(4) == "FILE")
			//mimestr = "Windows System/MFT File Entry";
    if(mimestr.isEmpty())
        return QString(mimecategory + "/" + mimesignature);
    else
        return mimestr;
}
*/ 
