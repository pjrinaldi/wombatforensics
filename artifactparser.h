#ifndef ARTIFACTPARSER_H
#define ARTIFACTPARSER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <bitset>

#include "common.h"
#include "forensicimage.h"

#include "rapidxml.hpp" // XML PARSING
#include <zip.h> // ZIP PARSING

void ParseArtifact(ForImg* curforimg, FileItem* curfileitem, bool* inmemory, uint8_t* tmpbuf, FILE* tmpfile, std::string* filecontents);
void GetXmlText(rapidxml::xml_node<>* curnode, std::string* contents);

#endif // ARTIFACTPARSER_H
