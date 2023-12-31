#ifndef NTFS_H
#define NTFS_H

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

void LoadNtfsDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem);
#endif // NTFS_H
