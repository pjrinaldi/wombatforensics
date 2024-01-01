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

//std::string GetIndexAttributesLayout(std::ifstream* rawcontent, ntfsinfo* curnt, uint64_t mftentryoffset);
//void GetRunListLayout(std::ifstream* rawcontent, ntfsinfo* curnt, uint64_t curoffset, uint32_t attributelength, std::string* runliststr);
/*
std::string GetStandardInformationAttribute(std::ifstream* rawcontent, ntfsinfo* curnt, uint64_t mftentryoffset);
std::string GetFileNameAttribute(std::ifstream* rawcontent, ntfsinfo* curnt, uint64_t mftentryoffset);
std::string GetDataAttribute(std::ifstream* rawcontent, ntfsinfo* curnt, uint64_t mftentryoffset);
*/

#endif // NTFSCOMMON_H
