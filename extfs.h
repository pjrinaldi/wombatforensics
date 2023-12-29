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

//void GetContentBlocks(std::ifstream* devicebuffer, uint32_t blocksize, uint64_t curoffset, uint32_t incompatflags, std::vector<uint32_t>* blocklist)
//std::string ConvertBlocksToExtents(std::vector<uint32_t>* blocklist, uint32_t blocksize)

//void GetContentBlocks(ForImg* curimg, uint32_t curstartsector, uint32_t blocksize, quint64 curoffset, QString* incompatflags, QList<uint32_t>* blocklist)

#endif // EXTFS_H
