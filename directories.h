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
std::string GetFileItem(std::string*, int item);
std::string SetFileItem(std::string* filecontents, int item, std::string value);
int ReadDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem);
void LoadDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem);
void LoadFatDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem);

#endif
