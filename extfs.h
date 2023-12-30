#ifndef EXTFS_H
#define EXTFS_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <bitset>
#include <algorithm>

#include "common.h"
#include "forensicimage.h"

// LOAD CURRENTLY SELECTED CONTENTS OF THE EXTFS DIRECTORY
void LoadExtDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem);

void GetContentBlocks(CurrentItem* currentitem, uint32_t blocksize, uint64_t currentoffset, uint32_t incompatflags, std::vector<uint32_t>* blocklist);
std::string ConvertBlocksToExtents(std::vector<uint32_t>* blocklist, uint32_t blocksize);

#endif // EXTFS_H
