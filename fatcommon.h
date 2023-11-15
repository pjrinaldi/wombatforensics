#ifndef FATCOMMON_H
#define FATCOMMON_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <bitset>
#include <list>

#include "common.h"

void GetNextCluster(ForImg* curimg, uint32_t clusternum, uint8_t fstype, uint64_t fatoffset, std::vector<uint>* clusterlist);
std::string ConvertBlocksToExtents(std::vector<uint>* blocklist, uint32_t blocksize, uint64_t rootdiroffset);

#endif // FATCOMMON_H
