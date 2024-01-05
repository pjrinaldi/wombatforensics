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
void GetStandardInformationAttribute(ForImg* curimg, uint32_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::string* properties);
void GetFileNameAttribute(ForImg* curimg, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::string* properties);
void GetDataAttribute(ForImg* curimg, uint32_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::vector<FileItem>* adsvector, std::string* properties);
void GetIndexRootAttribute(ForImg* curimg, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::vector<FileItem>* adsvector, std::string* properties);

//void GetIndexAllocationAttribute(); 0xa0
//void GetObjectIdAttribute(); // 0x40
//void GetAttributeListAttribute(); // 0x20
//void GetReparsePointAttribute(); // 0xc0
//void GetBitmapAttribute(); // 0xb0
//void GetSecurityDescriptorAttribute(); // 0x50
//void GetLoggedUtilityStreamAttribute(); // 0x100

#endif // NTFSCOMMON_H
