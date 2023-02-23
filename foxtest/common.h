#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <bitset>

#include <magic.h>

#include "forensicimage.h"

#define DTTMFMT "%F %T %z"
#define DTTMSZ 35

// MAY NEED A STRUCTURE THAT CONTAINS ITEM TYPE, CURFORIMG INDEX, PARENTID, CHILDID OR SOME SUCH
struct CurrentItem
{
    int inode = 0;
    ForImg* forimg = NULL;
    uint64_t voloffset = 0;
    std::string itemtext = "";
    std::string tmppath = "";
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
    std::string hash = "";      // 11
    std::string cat = "";       // 12
    std::string sig = "";       // 13
    std::string tag = "";       // 14
    std::string match = "";     // 15
    std::string change = "";    // 16
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
    };
};

//std::string ConvertUnixTimeToHuman(uint32_t unixtime);

char* GetDateTime(char *buff);
uint8_t* substr(uint8_t* arr, int begin, int len);

std::string ReturnFormattingSize(uint size);

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

//void ReturnUint(uint64_t* tmp, uint8_t* tmp8, unsigned int length);
//void ReturnInt(int64_t* tmp, int8_t* tmp8, unsigned int length);

void GetNextCluster(ForImg* curimg, uint32_t clusternum, uint8_t fstype, uint64_t fatoffset, std::vector<uint>* clusterlist);
std::string ConvertBlocksToExtents(std::vector<uint>* blocklist, uint32_t blocksize, uint64_t rootdiroffset);

void GenerateCategorySignature(CurrentItem* currentitem, std::string* layout, std::string* cat, std::string* sig);

#endif
