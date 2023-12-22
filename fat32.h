#ifndef FAT32_H
#define FAT32_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <bitset>
#include <algorithm>

#include "fatcommon.h"
#include "forensicimage.h"

// LOAD CURRENTLY SELECTED CONTENTS OF THE FAT DIRECTORY
void LoadFat32Directory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem);

#endif // FAT32_H
