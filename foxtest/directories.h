#ifndef DIRECTORIES_H
#define DIRECTORIES_H

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

std::string ConvertDosTimeToHuman(uint16_t* dosdate, uint16_t* dostime);
//void LoadDirectory(CurrentItem* currentitem);
void LoadDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector);
//void LoadFatDirectory(CurrentItem* currentitem, uint64_t curinode);
void LoadFatDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, uint64_t curinode);

#endif
