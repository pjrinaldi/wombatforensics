#ifndef DIRECTORIES_H
#define DIRECTORIES_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <bitset>

#include "common.h"
#include "forensicimage.h"

void LoadDirectory(CurrentItem* currentitem);
void LoadFatDirectory(CurrentItem* currentitem, uint64_t curinode);
/*
void LoadPartitions(ForImg* curforimg, std::vector<std::string>* volnames, std::vector<uint64_t>* volsizes, std::vector<uint64_t>* voloffsets);
void LoadGptPartitions(ForImg* curforimg, std::vector<std::string>* volnames, std::vector<uint64_t>* volsizes, std::vector<uint64_t>* voloffsets);
void LoadExtendedPartitions(ForImg* curforimg, uint64_t epoffset, uint64_t epsize, std::vector<std::string>* volnames, std::vector<uint64_t>* volsizes, std::vector<uint64_t>* voloffsets);
std::string GetFileSystemName(ForImg* curforimg, uint64_t offset);
void GetNextCluster(ForImg* curimg, uint32_t clusternum, uint8_t fstype, uint64_t fatoffset, std::vector<uint>* clusterlist);
std::string ConvertBlocksToExtents(std::vector<uint> blocklist, uint blocksize, uint64_t rootdiroffset);
*/

#endif
