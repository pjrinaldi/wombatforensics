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

//std::string GetDataAttributeLayout(std::ifstream* rawcontent, ntfsinfo* curnt, uint64_t mftoffset);
void GetDataAttributeLayout(ForImg* curimg, uint64_t mftentrybytes, uint64_t offset, std::string* layout); 

#endif // NTFSCOMMON_H
