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

void LoadDirectory(CurrentItem* currentitem);
void LoadFatDirectory(CurrentItem* currentitem, uint64_t curinode);

#endif
