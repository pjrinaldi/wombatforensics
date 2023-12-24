#ifndef EXFAT_H
#define EXFAT_H

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
void LoadExFatDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem);

#endif // EXFAT_H
