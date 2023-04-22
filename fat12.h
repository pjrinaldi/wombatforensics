#ifndef FAT12_H
#define FAT12_H

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

void LoadFatDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem);

#endif // FAT12_H
