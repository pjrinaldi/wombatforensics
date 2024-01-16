#ifndef NTFSCOMMON_H
#define NTFSCOMMON_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <bitset>
#include <algorithm>

#include "common.h"

void GetDataAttributeLayout(ForImg* curimg, uint32_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, std::string* layout); 
void GetIndexAttributeLayout(ForImg* curimg, uint32_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, std::string* indexlayout); 
void GetRunListLayout(ForImg* curimg, uint64_t offset, uint32_t bytespercluster, uint32_t attributelength, std::string* layout);
void GetStandardInformationAttribute(ForImg* curimg, uint32_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::string* properties); // 0x10
void GetFileNameAttribute(ForImg* curimg, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::string* properties); // 0x30
void GetDataAttribute(ForImg* curimg, uint32_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::vector<FileItem>* adsvector, std::string* properties); // 0x80
void GetIndexRootAttribute(ForImg* curimg, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::vector<FileItem>* adsvector, std::string* properties); // 0x90
void GetIndexAllocationAttribute(ForImg* curimg, uint64_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::vector<FileItem>* adsvector, std::string* properties); // 0xa0
void GetAttributeListAttribute(ForImg* curimg, uint64_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::string* properties); // 0x20
void GetObjectIdAttribute(ForImg* curimg, uint64_t mftentrybytes, uint64_t offset, std::string* properties); // 0x40
void GetReparsePointAttribute(ForImg* curimg, uint64_t mftentrybytes, uint64_t offset, std::string* properties); // 0xc0

//void GetBitmapAttribute(); // 0xb0
//void GetSecurityDescriptorAttribute(); // 0x50
//void GetLoggedUtilityStreamAttribute(); // 0x100

#endif // NTFSCOMMON_H
