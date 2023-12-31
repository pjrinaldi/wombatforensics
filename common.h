#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <bitset>
#include <list>
//#include <sys/stat.h>

// FILE CATEGORY/SIGNATURE
#include <magic.h>
// CIMG THUMBNAILER
#define cimg_os 1
#define cimg_use_png 1
#define cimg_use_jpeg 1
#define cimg_use_tiff 1
#define cimg_use_heic 1
#include "cimg/CImg.h"
// LIBFFMPEG THUMBNAILER HEADERS
#include <libffmpegthumbnailer/filmstripfilter.h>
#include <libffmpegthumbnailer/videothumbnailer.h>
// HEIF TO JPEG HEADER
//#include <libheif/heif.h>
//#include <jpeglib.h>
//#include <png.h>
// BLAKE3 HASH
#include "blake3/blake3.h"
#include "forensicimage.h"
#include "fatcommon.h"

#define DTTMFMT "%F %T %z"
#define DTTMSZ 35
#define TICKS_PER_SECOND 10000000
#define EPOCH_DIFFERENCE 11644473600LL
#define NSEC_BTWN_1904_1970	(uint32_t) 2082844800U

// MAY NEED A STRUCTURE THAT CONTAINS ITEM TYPE, CURFORIMG INDEX, PARENTID, CHILDID OR SOME SUCH
struct CurrentItem
{
    int inode = 0;
    ForImg* forimg = NULL;
    uint64_t voloffset = 0;
    std::string itemtext = "";
    std::string tmppath = "";
    //uint8_t fstype = 0;
};

struct FileItem
{
    uint64_t gid = 0;           //  0
    bool isdeleted = false;     //  1
    bool isdirectory = false;   //  2
    uint64_t size = 0;          //  3
    std::string name = "";      //  4
    std::string path = "";      //  5
    std::string create = "";    //  6
    std::string access = "";    //  7
    std::string modify = "";    //  8
    std::string layout = "";    //  9
    bool isvirtual = false;     // 10
    std::string cat = "";       // 11
    std::string sig = "";       // 12
    std::string hash = "";      // 13
    std::string tag = "";       // 14
    std::string match = "";     // 15
    std::string change = "";    // 16
    std::string filename = "";  // 17
    std::string properties = "";// 18

    void clear(void)
    {
        gid = 0;
        isdeleted = false;
        isdirectory = false;
        size = 0;
        name = "";
        path = "";
        create = "";
        access = "";
        modify = "";
        layout = "";
        isvirtual = false;
        hash = "";
        cat = "";
        sig = "";
        tag = "";
        match = "";
        change = "";
        filename = "";
	properties = "";
    };
};

std::string ConvertUnixTimeToHuman(uint32_t unixtime);

char* GetDateTime(char *buff);
std::string ConvertDosTimeToHuman(uint16_t* dosdate, uint16_t* dostime);
std::string ConvertWindowsTimeToUnixTimeUTC(uint64_t input);
std::string ConvertExFatTimeToHuman(uint16_t* dosdate, uint16_t* dostime, uint8_t* timezone);
uint8_t* substr(uint8_t* arr, int begin, int len);

std::string ReturnFormattingSize(uint size);
std::string ReturnFormattedGuid(uint8_t* guidbuffer);

//void ReadContent(std::ifstream* rawcontent, uint8_t* tmpbuf, uint64_t offset, uint64_t size);

//void ReadContent(std::ifstream* rawcontent, int8_t* tmpbuf, uint64_t offset, uint64_t size);
void ReturnUint32(uint32_t* tmp32, uint8_t* tmp8, bool isbigendian=false);
void ReturnUint16(uint16_t* tmp16, uint8_t* tmp8, bool isbigendian=false);
void ReturnUint64(uint64_t* tmp64, uint8_t* tmp8, bool isbigendian=false);

//void ReadContent(std::ifstream* rawcontent, uint16_t* val, uint64_t offset);
//void ReadContent(std::ifstream* rawcontent, uint32_t* val, uint64_t offset);
//void ReadContent(std::ifstream* rawcontent, uint64_t* val, uint64_t offset);

void ReadForImgContent(ForImg* forimg, uint16_t* val, uint64_t off, bool isbigendian=false);
void ReadForImgContent(ForImg* forimg, uint32_t* val, uint64_t off, bool isbigendian=false);
void ReadForImgContent(ForImg* forimg, uint64_t* val, uint64_t off, bool isbigendian=false);

void ReadInteger(uint8_t* arr, int begin, uint16_t* val, bool isbigendian=false);
void ReadInteger(uint8_t* arr, int begin, uint32_t* val, bool isbigendian=false);
void ReadInteger(uint8_t* arr, int begin, uint64_t* val, bool isbigendian=false);

void ReturnUint(uint64_t* tmp, uint8_t* tmp8, unsigned int length);
//void ReturnInt(int64_t* tmp, int8_t* tmp8, unsigned int length);

void GenerateCategorySignature(CurrentItem* currentitem, std::string* filename, std::string* layout, std::string* cat, std::string* sig);

void GetFileContent(ForImg* curforimg, FileItem* curfileitem, bool* inmemory, uint8_t** tmpbuf, FILE* tmpfile);
void GetPreviewContent(ForImg* curforimg, FileItem* curfileitem, uint8_t** prebuf, uint64_t bufsize);
//void AddFileContent(ForImg* curforimg, FileItem* curfileitem, bool* inmemory, uint8_t** tmpbuf, FILE* tmpfile);
void GetFileSlack(ForImg* curforimg, FileItem* curfileitem, uint8_t** tmpbuf, uint64_t* slacksize);
void AddFileSlack(ForImg* curforimg, FileItem* curfileitem, uint8_t* slkbuf, std::string* filecontents);

void HashFile(FileItem* curfileitem, ForImg* curforimg);
void ThumbnailImage(ForImg* curforimg, FileItem* curfileitem, int thumbsize, std::string tmppath);
void ThumbnailVideo(ForImg* curforimg, FileItem* curfileitem, int thumbsize, int thumbcount, std::string tmppath);
void ConvertSvgToPng(std::string* tmpfilestr);
bool ConvertHeifToPng(std::string* heifstr);
bool ConvertAvifToPng(std::string* avifstr);

#endif
