#ifndef PARTITIONS_H
#define PARTITIONS_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <bitset>

#include "common.h"
#include "forensicimage.h"

void LoadPartitions(ForImg* curforimg, std::vector<std::string>* volnames, std::vector<uint64_t>* volsizes, std::vector<uint64_t>* voloffsets, std::vector<std::string>* volprops);
void LoadGptPartitions(ForImg* curforimg, std::vector<std::string>* volnames, std::vector<uint64_t>* volsizes, std::vector<uint64_t>* voloffsets, std::vector<std::string>* volprops);
void LoadExtendedPartitions(ForImg* curforimg, uint64_t epoffset, uint64_t epsize, std::vector<std::string>* volnames, std::vector<uint64_t>* volsizes, std::vector<uint64_t>* voloffsets, std::vector<std::string>* volprops);
std::string GetFileSystemName(ForImg* curforimg, uint64_t offset);
void GetVolumeProperties(ForImg* curforimg, uint64_t offset, std::vector<std::string>* volprops);

#endif
