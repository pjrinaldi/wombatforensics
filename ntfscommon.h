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
void GetDataAttribute(ForImg* curimg, uint32_t bytespercluster, uint64_t mftentrybytes, uint64_t offset, FileItem* tmpitem, std::vector<FileItem>* filevector, std::string* properties);

#endif // NTFSCOMMON_H
