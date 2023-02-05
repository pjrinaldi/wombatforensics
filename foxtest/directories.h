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
//void GetNextCluster(ForImg* curimg, uint32_t clusternum, uint8_t fstype, uint64_t fatoffset, std::vector<uint>* clusterlist);

#endif
